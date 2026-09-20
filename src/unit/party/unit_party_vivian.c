#include "unit/party/unit_party_vivian.h"

extern void* _battleWorkPointer;
extern s32 evtGetValue(void* evt, s32 arg);
extern f32 evtGetFloat(void* evt, s32 arg);
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);
extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
extern s32 animEffectAsync(char* name, s32 flags);
extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 frame, s32 duration);
extern void animPoseSetEffect(s32 poseId, char* name, s32 flags);
extern void animPoseSetEffectAnim(s32 poseId, char* name, s32 flags);
extern void* animPoseGetAnimPosePtr(s32 poseId);
extern f32 animPoseGetLoopTimes(void* anim);
extern void psndSFXOn_3D(char* name, void* pos);
extern char str_PTR_D_4_802f84d0[];
extern const f32 float_0_804241fc;
extern const f32 float_1_80424204;
extern const f32 float_2_8042421c;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 battle_evt_majo_disp_on(void* evt, s32 isFirstCall) {
    typedef struct MajoDispPos {
        f32 x;
        f32 y;
        f32 z;
    } MajoDispPos;
    extern void animPoseSetPaperAnimGroup(s32 poseId, char* group, s32 flags);
    extern const f32 float_200_80424228;
    extern const f32 float_0p66667_8042422c;
    s32* args;
    s32 unitId;
    s32 partsId;
    f32 x;
    f32 y;
    f32 z;
    s32 clearFlag;
    void* unit;
    void* parts;
    s32 poseId;
    MajoDispPos pos;

    args = *(s32**)((s32)evt + 0x18);
    unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
    partsId = evtGetValue(evt, args[1]);
    x = evtGetFloat(evt, args[2]);
    y = evtGetFloat(evt, args[3]);
    z = evtGetFloat(evt, args[4]);
    clearFlag = evtGetValue(evt, args[5]);
    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    parts = BattleGetUnitPartsPtr(unitId, partsId);
    poseId = *(s32*)((s32)parts + 0x1C0);

    if (isFirstCall != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        *(s32*)((s32)evt + 0x7C) = 0;
        *(s32*)((s32)evt + 0x80) = 0;
        *(s32*)((s32)evt + 0x84) = 0;
        pos = *(MajoDispPos*)((s32)unit + 0xEC);
        pos.x = x;
        pos.y = y - float_200_80424228;
        pos.z = z;
        *(MajoDispPos*)((s32)unit + 0x3C) = pos;
        animEffectAsync(str_PTR_D_4_802f84d0 + 0x3E8, 1);
    }

    switch (*(s32*)((s32)evt + 0x78)) {
        case 0:
            *(f32*)((s32)parts + 0x78) =
                intplGetValue(0, float_0_804241fc, float_2_8042421c, *(s32*)((s32)evt + 0x7C), 0x1E);
            if (*(s32*)((s32)evt + 0x7C) == 0) {
                psndSFXOn_3D(str_PTR_D_4_802f84d0 + 0x414, (void*)((s32)unit + 0x3C));
            }
            *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            if (*(s32*)((s32)evt + 0x7C) > 0xF) {
                *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
            }
            break;
        case 1:
            if (*(s32*)((s32)evt + 0x7C) < 0x1F) {
                *(f32*)((s32)parts + 0x78) =
                    intplGetValue(0, float_0_804241fc, float_2_8042421c, *(s32*)((s32)evt + 0x7C), 0x1E);
                *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            }
            if (animEffectAsync(str_PTR_D_4_802f84d0 + 0x3E8, 1) == 0) {
                return 0;
            }
            *(f32*)((s32)unit + 0x3C) = x;
            *(f32*)((s32)unit + 0x40) = y;
            *(f32*)((s32)unit + 0x44) = z;
            animPoseSetEffect(poseId, str_PTR_D_4_802f84d0 + 0x3E8, 1);
            animPoseSetEffectAnim(poseId, str_PTR_D_4_802f84d0 + 0x430, 1);
            *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
            break;
        case 2:
            if (*(s32*)((s32)evt + 0x7C) < 0x1F) {
                *(f32*)((s32)parts + 0x78) =
                    intplGetValue(0, float_0_804241fc, float_2_8042421c, *(s32*)((s32)evt + 0x7C), 0x1E);
                *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            }
            if (animPoseGetLoopTimes(*(void**)((s32)animPoseGetAnimPosePtr(poseId) + 0x90)) >= float_0p66667_8042422c) {
                animPoseSetPaperAnimGroup(poseId, 0, 1);
                *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
                *(s32*)((s32)evt + 0x7C) = 0;
            }
            break;
        case 3:
            *(f32*)((s32)parts + 0x78) =
                intplGetValue(0, float_2_8042421c, float_1_80424204, *(s32*)((s32)evt + 0x7C), 0xF);
            *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            if (*(s32*)((s32)evt + 0x7C) > 0xF) {
                *(f32*)((s32)parts + 0x78) = float_1_80424204;
                animPoseSetEffect(poseId, 0, 1);
                if (clearFlag != 0) {
                    *(u32*)((s32)unit + 0x1C) &= ~0x10000;
                }
                return 2;
            }
            break;
    }

    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 battle_evt_majo_disp_off(void* evt, s32 isFirstCall) {
    extern void animPoseSetLocalTime(void* anim, f32 time);
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 group, s32 flags);
    extern const f32 float_8_80424220;
    extern const f32 float_500_80424224;
    s32* args;
    s32 unitId;
    s32 partsId;
    s32 setFlag;
    s32 partsFlag;
    void* unit;
    void* parts;
    s32 poseId;

    args = *(s32**)((s32)evt + 0x18);
    unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
    partsId = evtGetValue(evt, args[1]);
    setFlag = evtGetValue(evt, args[2]);
    partsFlag = evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    parts = BattleGetUnitPartsPtr(unitId, partsId);
    poseId = *(s32*)((s32)parts + 0x1C0);

    if (isFirstCall != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        *(s32*)((s32)evt + 0x7C) = 0;
        animEffectAsync(str_PTR_D_4_802f84d0 + 0x3E8, 1);
    }

    switch (*(s32*)((s32)evt + 0x78)) {
        case 0:
            *(f32*)((s32)parts + 0x78) = intplGetValue(0, float_1_80424204, float_2_8042421c, *(s32*)((s32)evt + 0x7C), 0x1E);
            if (setFlag != 0) {
                *(u32*)((s32)unit + 0x1C) |= 0x10000;
            }
            *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            if (*(s32*)((s32)evt + 0x7C) > 0xF) {
                *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
            }
            break;
        case 1:
            if (*(s32*)((s32)evt + 0x7C) <= 0x1E) {
                *(f32*)((s32)parts + 0x78) = intplGetValue(0, float_1_80424204, float_2_8042421c, *(s32*)((s32)evt + 0x7C), 0x1E);
                *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            }
            if (animEffectAsync(str_PTR_D_4_802f84d0 + 0x3E8, 1) == 0) {
                return 0;
            }
            animPoseSetEffect(poseId, str_PTR_D_4_802f84d0 + 0x3E8, 1);
            animPoseSetEffectAnim(poseId, str_PTR_D_4_802f84d0 + 0x3F0, 1);
            animPoseSetLocalTime(*(void**)((s32)animPoseGetAnimPosePtr(poseId) + 0x90), float_8_80424220);
            *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
            break;
        case 2:
            if (*(s32*)((s32)evt + 0x7C) <= 0x1E) {
                *(f32*)((s32)parts + 0x78) = intplGetValue(0, float_1_80424204, float_2_8042421c, *(s32*)((s32)evt + 0x7C), 0x1E);
                *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            }
            if (animPoseGetLoopTimes(*(void**)((s32)animPoseGetAnimPosePtr(poseId) + 0x90)) >= float_1_80424204) {
                *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
                *(s32*)((s32)evt + 0x7C) = 0;
                psndSFXOn_3D(str_PTR_D_4_802f84d0 + 0x3F8, (void*)((s32)unit + 0x3C));
                if (partsFlag != 0) {
                    *(u32*)((s32)parts + 0x1AC) |= 0x1000000;
                }
            }
            break;
        case 3:
            *(f32*)((s32)parts + 0x78) = intplGetValue(0, float_2_8042421c, float_0_804241fc, *(s32*)((s32)evt + 0x7C), 0x14);
            *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
            if (*(s32*)((s32)evt + 0x7C) > 0x14) {
                *(f32*)((s32)parts + 0x78) = float_0_804241fc;
                *(f32*)((s32)unit + 0x40) -= float_500_80424224;
                animPoseSetEffect(poseId, 0, 1);
                animPoseSetPaperAnimGroup(poseId, 0, 1);
                return 2;
            }
            break;
    }
    return 0;
}

s32 _vivian_make_extra_work_area(void* evt) {
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
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_8038B6B0[];
    s32 techLv;
    void* party;
    u8* data;
    u8* entry;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_8038B6B0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x90) = -1;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x94) = 0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(void**)(entry + 0x80) = data + 0x830;
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
        *(void**)(entry + 0x80) = data + 0x8F0;
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
        *(void**)(entry + 0x80) = data + 0x9B0;
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
        *(void**)(entry + 0x80) = data + 0xA70;
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


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 _get_move_frame(void* evt) {
    extern void PSVECSubtract(f32* a, f32* b, f32* out);
    extern f64 sqrt(f64 value);

    s32* args = *(s32**)((s32)evt + 0x18);
    f32 start[3];
    f32 end[3];
    f32 diff[3];
    f32 speed;
    s32 out;
    s32 frames;

    start[0] = evtGetFloat(evt, args[0]);
    start[1] = evtGetFloat(evt, args[1]);
    start[2] = evtGetFloat(evt, args[2]);
    end[0] = evtGetFloat(evt, args[3]);
    end[1] = evtGetFloat(evt, args[4]);
    end[2] = evtGetFloat(evt, args[5]);
    speed = evtGetFloat(evt, args[6]);
    out = args[7];
    PSVECSubtract(start, end, diff);
    frames = (s32)((f32)sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]) / speed);
    if (frames > 15) {
        frames = 15;
    }
    evtSetValue(evt, out, frames);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on




/* CHATGPT FALLBACK MISSING STUBS: main/unit/party/unit_party_vivian 20260624_191429 */

/* fallback stub-fill: map=unk_80182cc4 addr=0x80182cc4 size=0x00000260 */
int unk_80182cc4(void* evt) {
    extern void BtlUnit_GetScale(void* unit, f32* x, f32* y, f32* z);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern const f32 float_neg30_80424214;
    extern const f32 float_20_80424218;
    s32* args;
    void* battleWork;
    s32 targetId;
    s32 outX;
    s32 outY;
    s32 outZ;
    s32 addZ;
    void* owner;
    void* target;
    f32 sx;
    f32 sy;
    f32 sz;
    s32 x;
    s32 y;
    s32 z;
    f32 floor;
    s8 targetDir;
    s8 ownerDir;
    f32 ownerScale;
    void* parts;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    targetId = evtGetValue(evt, args[0]);
    outX = args[1];
    evtGetValue(evt, outX);
    outY = args[2];
    evtGetValue(evt, outY);
    outZ = args[3];
    addZ = evtGetValue(evt, outZ);
    owner = BattleGetUnitPtr(battleWork, BattleTransID(evt, -2));
    target = BattleGetUnitPtr(battleWork, BattleTransID(evt, targetId));
    BtlUnit_GetScale(target, &sx, &sy, &sz);
    targetDir = *(s8*)((s32)target + 0x189);
    parts = *(void**)((s32)target + 0x10);
    ownerDir = *(s8*)((s32)owner + 0x189);
    ownerScale = *(f32*)((s32)owner + 0x114);
    x = (s32)((f32)ownerDir * (float_neg30_80424214 * ownerScale) + (f32)targetDir * (*(f32*)((s32)parts + 0x54) * sx));
    y = (s32)(-float_20_80424218 * ownerScale);
    z = addZ + (s32)(*(f32*)((s32)parts + 0x58) * sz);
    floor = BattleGetFloorHeight(battleWork, (f32)x, (f32)y, (f32)z);
    if (floor > (f32)y) {
        y = (s32)floor;
    }
    evtSetValue(evt, outX, x);
    evtSetValue(evt, outY, y);
    evtSetValue(evt, outZ, z);
    return 2;
}
s32 _make_kagenuke_weapon(void* evt) {
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
    *(u8*)((s32)weapon + 0x90) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 _disp_heart_entry(void* evt, int isFirstCall) {
    extern void dispEntry(s32 prio, s32 cameraId, void* callback, void* param, f32 z);
    extern void _disp_heart(void);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 unitId = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, unitId);

    if (isFirstCall != 0) {
        *(void**)((s32)evt + 0x78) = unit;
        *(s32*)((s32)evt + 0x7C) = 0;
        *(s32*)((s32)evt + 0x80) = evtGetValue(evt, args[0]);
        *(s32*)((s32)evt + 0x84) = evtGetValue(evt, args[1]);
        *(s32*)((s32)evt + 0x88) = evtGetValue(evt, args[2]);
        *(s32*)((s32)evt + 0x8C) = 0;
        *(s32*)((s32)evt + 0x90) = evtGetValue(evt, args[3]);
        *(s32*)((s32)evt + 0x94) = evtGetValue(evt, args[4]);
    }
    if (*(s32*)((s32)evt + 0x8C) != 0) {
        return 2;
    }
    *(s32*)((s32)evt + 0x7C) += 1;
    dispEntry(4, 1, _disp_heart, evt, float_0_804241fc);
    if (*(s32*)((s32)evt + 0x7C) < *(s32*)((s32)evt + 0x88)) {
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void _disp_heart(s32 unused, void* work) {
    extern const f32 float_1p2_80424200;
    extern const f32 float_10_80424208;
    extern const f32 float_16_8042420c;
    extern const f32 float_5_80424210;
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void BtlUnit_GetHitPos(void* unit, void* parts, f32* x, f32* y, f32* z);
    extern void iconDispGx(f32* pos, s32 flags, s32 iconId, f32 scale);

    void* unit;
    void* parts;
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
    f32 posA[3];
    f32 posB[3];
    s32 frame;
    s32 a;
    s32 b;

    if (work == 0) {
        return;
    }
    if (*(void**)((s32)work + 0x78) == 0) {
        return;
    }
    frame = *(s32*)((s32)work + 0x7C);
    a = *(s32*)((s32)work + 0x80);
    if (frame <= a) {
        scale = intplGetValue(5, float_0_804241fc, float_1p2_80424200, frame, a);
    } else {
        b = *(s32*)((s32)work + 0x84);
        if (frame <= b) {
            scale = intplGetValue(0xB, float_1p2_80424200, float_1_80424204, frame - a, b - a);
        } else {
            scale = float_1_80424204;
        }
    }
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(work, *(s32*)((s32)work + 0x90)));
    parts = BtlUnit_GetPartsPtr(unit, *(s32*)((s32)work + 0x94));
    BtlUnit_GetHitPos(unit, parts, &x, &y, &z);
    z += float_10_80424208;
    posA[0] = x;
    posA[1] = y;
    posA[2] = z;
    posB[0] = x;
    posB[1] = y + float_16_8042420c * scale;
    posB[2] = z + float_5_80424210;
    iconDispGx(posA, 0, 0x1F9, scale);
    iconDispGx(posB, 0, 0x191, scale);
}

s32 _disp_heart_entry_stop(void* evt) {
    extern void* evtGetPtrID(s32 id);

    void* ptr = evtGetPtrID(evtGetValue(evt, **(s32**)((s32)evt + 0x18)));

    *(s32*)((s32)ptr + 0x8C) = 1;
    return 2;
}

s32 _disp_heart_entry_stop_check(void* evt) {
    evtSetValue(evt, **(s32**)((s32)evt + 0x18), *(s32*)((s32)evt + 0x8C));
    return 2;
}


/* Recovered Vivian battle-event dependencies. */
extern const char str_btl_un_vivian_802f8540[];
extern const char str_c_vivian_802f8550[];
extern const char str_SFX_PARTY_DAMAGE1_802f855c[];
extern const char str_SFX_MARIO_DAMAGE_FIR_802f8570[];
extern const char str_SFX_MARIO_DAMAGE_ICE_802f8588[];
extern const char str_SFX_MARIO_DAMAGE_BIR_802f85a0[];
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern const char str_PTR_C_1_802f8538[];
extern const char str_PTR_D_1_802f8510[];
extern const char str_PTR_D_2_802f84e0[];
extern const char str_PTR_D_3_802f84f0[];
extern const char str_PTR_G_1_802f8508[];
extern const char str_PTR_G_2_802f8500[];
extern const char str_PTR_I_1_802f8530[];
extern const char str_PTR_N_1_802f84e8[];
extern const char str_PTR_R_1_802f8520[];
extern const char str_PTR_S_1_802f84f8[];
extern const char str_PTR_T_1_802f8528[];
extern const char str_PTR_W_1_802f8518[];
extern const char str_PTR_X_1_802f84d8[];
extern const char str_btl_wn_ptr_lv1_802f86ac[];
extern const char str_btl_wn_ptr_lv2_802f86e4[];
extern const char str_btl_wn_ptr_lv3_802f8724[];
extern const char str_btl_wn_ptr_normal_802f8674[];
extern const char str_msg_ac_kagegakure_802f86d0[];
extern const char str_msg_ac_kagenuke_802f869c[];
extern const char str_msg_ac_mahou_no_kona_802f870c[];
extern const char str_msg_ac_meromero_kiss_802f874c[];
extern const char str_msg_ptr_kagegakure_802f86bc[];
extern const char str_msg_ptr_kagenuke_802f8688[];
extern const char str_msg_ptr_mahou_no_kon_802f86f4[];
extern const char str_msg_ptr_meromero_kis_802f8734[];
extern u32 subset_attack_ready_jump_event[];
extern s32 weaponGetACOutputParam();
extern s32 weaponGetPowerFromPartyAttackLv();
static u32 battle_entry_event[59];
static u32 init_event[68];
static u32 damage_event[16];
extern u32 vivian_hide_event[];
extern u32 partyVivianAttack_NormalAttack[];
extern u32 partyVivianAttack_ShadowGuard[];
extern u32 partyVivianAttack_MagicalPowder[];
extern u32 partyVivianAttack_CharmKissAttack[];
extern u32 vivian_shadow_tail_event[];
extern u32 vivian_counter_damage_event[];
extern u32 vivian_appeal_event[];
static u32 party_win_reaction[18];
static u32 attack_audience_event[189];
static u32 attack_event[2];
static u32 wait_event[6];
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
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AttackDeclareAll();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_AudienceDeclareACResult();
extern s32 btlevtcmd_AudienceDeclareAcrobatResult();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CommandCheckDamage();
extern s32 btlevtcmd_CommandGetWeaponActionLv();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPartyTechLv();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageAudienceAreaSize();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_GetWidth();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffAttribute();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OnAttribute();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_OnUnitFlag();
extern s32 btlevtcmd_PhaseEventStartDeclare();
extern s32 btlevtcmd_PreCheckDamage();
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
extern s32 btlevtcmd_SetJumpSound();
extern s32 btlevtcmd_SetMoveSoundDataWork();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
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
extern s32 evt_btl_camera_set_posoffset();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_eff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_random();
extern u32 partyWeapon_VivianShadowGuard[];
extern const char str_M_B_3_804241e8[];
extern const char str_PTR_A1_1_802f87d0[];
extern const char str_PTR_A1_2_802f87ac[];
extern const char str_PTR_A2_1_802f87dc[];
extern const char str_PTR_A2_2_802f87e8[];
extern const char str_PTR_A2_3_802f87f4[];
extern const char str_PTR_A3_1_802f882c[];
extern const char str_PTR_A4_1_802f8844[];
extern const char str_PTR_A4_2_802f8864[];
extern const char str_PTR_O_1_802f888c[];
extern const char str_PTR_O_2_802f88a8[];
extern const char str_PTR_Y_1_802f866c[];
extern const char str_SFX_BTL_VIVIAN_ATTAC_802f8764[];
extern const char str_SFX_BTL_VIVIAN_ATTAC_802f877c[];
extern const char str_SFX_BTL_VIVIAN_ATTAC_802f8794[];
extern const char str_SFX_BTL_VIVIAN_ATTAC_802f87b8[];
extern const char str_SFX_BTL_VIVIAN_MAGIC_802f8814[];
extern const char str_SFX_BTL_VIVIAN_MEROM_802f8870[];
extern const char str_SFX_MARIO_APPEAL1_802f8894[];
extern const char str_SFX_PARTY_BATTLE_HAN_802f8620[];
extern const char str_SFX_PARTY_BATTLE_JUM_802f8638[];
extern const char str_SFX_PARTY_BATTLE_LAN_802f8650[];
extern const char str_SFX_PARTY_BATTLE_MOV_802f85f0[];
extern const char str_SFX_PARTY_BATTLE_MOV_802f8608[];
extern const char str_SFX_PARTY_BATTLE_SUB_802f85d4[];
extern const char str_SFX_PARTY_MOJIMOJI1_802f8850[];
extern const char str_kemuri_test_802f8838[];
extern const char str_kiss_804241f0[];
extern const char str_ripple_802f8800[];
extern const char str_stardust_802f8808[];
extern const u8 zero_804241f8[];

/* Recovered target Vivian .sdata unit metadata prefix. */
static u8 defence[5] = {0, 0, 0, 0, 0};
static u8 defence_attr[5] = {0, 0, 0, 0, 0};
static u8 regist[22] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

/* Recovered target Vivian event/event-like arrays. */
static u32 attack_event[2] = {
    0x00000002, 0x00000001
};

static u32 wait_event[6] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

/* Recovered target Vivian pose/data-table core. */
u32 pose_table_vivian_stay[48] = {
    0x00000009, (u32)(char*)str_PTR_D_4_802f84d0, 0x00000002, (u32)(char*)str_PTR_D_4_802f84d0, 0x00000003, (u32)(char*)str_PTR_X_1_802f84d8,
    0x00000004, (u32)(char*)str_PTR_D_2_802f84e0, 0x00000005, (u32)(char*)str_PTR_X_1_802f84d8, 0x00000001, (u32)(char*)str_PTR_N_1_802f84e8,
    0x0000001B, (u32)(char*)str_PTR_D_3_802f84f0, 0x0000001C, (u32)(char*)str_PTR_S_1_802f84f8, 0x00000025, (u32)(char*)str_PTR_G_2_802f8500,
    0x0000001F, (u32)(char*)str_PTR_G_1_802f8508, 0x00000027, (u32)(char*)str_PTR_D_1_802f8510, 0x00000028, (u32)(char*)str_PTR_W_1_802f8518,
    0x0000002A, (u32)(char*)str_PTR_R_1_802f8520, 0x00000039, (u32)(char*)str_PTR_D_2_802f84e0, 0x00000038, (u32)(char*)str_PTR_D_2_802f84e0,
    0x0000003C, (u32)(char*)str_PTR_D_4_802f84d0, 0x0000003D, (u32)(char*)str_PTR_D_2_802f84e0, 0x00000041, (u32)(char*)str_PTR_T_1_802f8528,
    0x00000034, (u32)(char*)str_PTR_I_1_802f8530, 0x00000043, (u32)(char*)str_PTR_C_1_802f8538, 0x00000040, (u32)(char*)str_PTR_X_1_802f84d8,
    0x00000031, (u32)(char*)str_PTR_D_1_802f8510, 0x0000002D, (u32)(char*)str_PTR_G_1_802f8508, 0x00000045, (u32)(char*)str_PTR_S_1_802f84f8
};

static u32 data_table[40] = {
    0x00000003, (u32)vivian_hide_event, 0x00000005, (u32)vivian_appeal_event, 0x00000006, (u32)attack_audience_event,
    0x00000001, (u32)__makeTechMenuFunc, 0x00000019, (u32)vivian_counter_damage_event, 0x0000001A, (u32)vivian_counter_damage_event,
    0x0000001B, (u32)vivian_counter_damage_event, 0x0000001C, (u32)vivian_counter_damage_event, 0x0000001D, (u32)vivian_counter_damage_event,
    0x0000001E, (u32)vivian_counter_damage_event, 0x0000001F, (u32)vivian_counter_damage_event, 0x00000020, (u32)vivian_counter_damage_event,
    0x00000021, (u32)vivian_counter_damage_event, 0x00000022, (u32)vivian_counter_damage_event, 0x00000023, (u32)vivian_counter_damage_event,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000
};

/* Recovered target Vivian BattleUnitKindPart/BattleUnitKind records. */
u32 unitpartsdata_Party_Vivian[19] = {
    0x00000001, (u32)(char*)str_btl_un_vivian_802f8540, (u32)(char*)str_c_vivian_802f8550, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000,
    0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_vivian_stay
};

u32 unitdata_Party_Vivian[49] = {
    0x000000E4, (u32)(char*)str_btl_un_vivian_802f8540, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x0021000E, 0x00240000, 0x00000000, 0x41400000, 0x00000000, 0x00000000,
    0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x41AB999A, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f855c, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f8570, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f8588,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f85a0, 0x02000004, (u32)regist, 0x01000000, (u32)unitpartsdata_Party_Vivian, (u32)init_event,
    (u32)data_table
};

static u32 battle_entry_event[59] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f85d4, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f85d4, 0x00000000, 0x00000006, 0x00000006,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

static u32 init_event[68] = {
    0x0001005B, (u32)_vivian_make_extra_work_area, 0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event,
    0x0003005B, (u32)btlevtcmd_SetEventAttack, 0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage,
    0xFFFFFFFE, (u32)damage_event, 0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x0001005C, (u32)vivian_shadow_tail_event,
    0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f85f0, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8608, 0x00000000,
    0x00000014, 0x00000014, 0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f85f0,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8608, 0x00000000, 0x0000000B, 0x0000000B, 0x0007005B, (u32)btlevtcmd_SetDiveSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f8620, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f8620, 0x00000000, 0x0000000B, 0x0000000B,
    0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f8638, (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f8650, 0x0003005B,
    (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE, (u32)battle_entry_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

static u32 damage_event[16] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001
};

u32 vivian_hide_event[209] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000001, 0x00020036, 0xFE363C80, 0x00000001, 0x00000021,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C80, 0x0002001C,
    0xFE363C80, 0x00000000, 0x00020032, 0xFE363C83, 0xFFFFFFFD, 0x0002005B,
    (u32)btlevtcmd_PhaseEventStartDeclare, 0xFE363C83, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C83, 0xFE363C80,
    0x0000000F, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000006B, 0x0003005B, (u32)btlevtcmd_OffAttribute, 0xFE363C83, 0x00000010,
    0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFE363C83, 0x00000001, 0x03000000, 0x0007005B,
    (u32)battle_evt_majo_disp_on, 0xFE363C83, 0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C83, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFE363C83, 0xF24A8280, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x0000006D, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE,
    0x00000001, 0x00010000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x01000000, 0x0007005B, (u32)battle_evt_majo_disp_on, 0xFFFFFFFE, 0x00000001, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A8280, 0xFE363C8F, 0x0001006F,
    0xFE363C8E, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C8F, 0xFFFFFFFF, 0x00000000, 0x00000070, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0xFE363C8F, 0xFE363C8F, 0x0000000F,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0001005F, 0xFE363C8E, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    (u32)partyWeapon_VivianShadowGuard, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_Y_1_802f866c, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8280, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E, 0x00000031, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C83, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000021, 0x00000002, 0x00000001
};

/* Recovered target Vivian BattleWeapon records. */
u32 partyWeapon_VivianNormalAttack[48] = {
    (u32)(char*)str_btl_wn_ptr_normal_802f8674, 0x01580000, 0x00000000, (u32)(char*)str_msg_ptr_kagenuke_802f8688, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000003, 0x00000003, 0x00000004, 0x00000004,
    0x00000005, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_kagenuke_802f869c, 0x00030007,
    0x00000002, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x02000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyVivianAttack_NormalAttack, 0x00000064, 0x00040200, 0x00000000
};

u32 partyWeapon_VivianShadowGuard[48] = {
    (u32)(char*)str_btl_wn_ptr_lv1_802f86ac, 0x01590000, 0x00000000, (u32)(char*)str_msg_ptr_kagegakure_802f86bc, 0x64010000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000002, 0x00000002, 0x00000003,
    0x00000003, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101160, 0x10000002, 0x00000302, (u32)(char*)str_msg_ac_kagegakure_802f86d0, 0x00020008,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyVivianAttack_ShadowGuard, 0x00000064, 0x00000000, 0x00000000
};

u32 partyWeapon_VivianMagicalPowder[48] = {
    (u32)(char*)str_btl_wn_ptr_lv2_802f86e4, 0x015A0000, 0x00000000, (u32)(char*)str_msg_ptr_mahou_no_kon_802f86f4, 0x64060000, 0x3F800000,
    0x01056432, (u32)weaponGetACOutputParam, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x01000302, (u32)(char*)str_msg_ac_mahou_no_kona_802f870c, 0x00020047,
    0x000007FF, 0x80002000, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x03000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyVivianAttack_MagicalPowder, 0x00000064, 0x000A0500, 0x00000000
};

u32 partyWeapon_VivianCharmKissAttack[48] = {
    (u32)(char*)str_btl_wn_ptr_lv3_802f8724, 0x015B0000, 0x00000000, (u32)(char*)str_msg_ptr_meromero_kis_802f8734, 0x64040000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_meromero_kiss_802f874c, 0x00020000,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00640300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyVivianAttack_CharmKissAttack, 0x00000064, 0x000A0500, 0x00000000
};

u32 partyVivianAttack_NormalAttack[581] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x0003005B, (u32)_make_kagenuke_weapon, 0xFE363C8C, 0x00000002, 0x00010024,
    0x00000001, 0x0003005B, (u32)_make_kagenuke_weapon, 0xFE363C8C, 0x00000002, 0x0000002A,
    0x0003005B, (u32)_make_kagenuke_weapon, 0xFE363C8C, 0x00000002, 0x00000031, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000001,
    0x00000001, 0x00000028, 0x00000001, 0x00000002, 0x00000000, 0x00000003,
    0x00000000, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000017, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000012, 0x00000001, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_VIVIAN_ATTAC_802f8764, 0x00000000, 0x0005005B, (u32)battle_evt_majo_disp_off, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)unk_80182cc4, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80,
    0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000032, 0x00000000, 0x00000005, 0x00000000, 0xFFFFFFFF, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x00000001, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_VIVIAN_ATTAC_802f877c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0007005B, (u32)battle_evt_majo_disp_on,
    0xFFFFFFFE, 0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C86,
    0xFE363C87, 0xFE363C88, 0x0005005B, (u32)unk_80182cc4, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)unk_80182cc4, 0xFE363C83, 0xFE363C86,
    0xFE363C87, 0xFE363C88, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C86, 0xFE363C87,
    0xFE363C88, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFE363C83, 0xFE363C8F, 0x00020037,
    0xFE363C8F, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C86, 0xFE363C87, 0xFE363C88,
    0x0009005B, (u32)_get_move_frame, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86,
    0xFE363C87, 0xFE363C88, 0xF24A8A80, 0xFE363C86, 0x0002001D, 0xFE363C86,
    0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_VIVIAN_ATTAC_802f8794, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000C, 0x00000000, 0x00000005, 0x00000000,
    0xFFFFFFFF, 0x00000021, 0x00010003, 0x00000009, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00020018,
    0xFE363C85, 0x00000005, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021, 0x0001005B,
    (u32)btlevtcmd_ResultAC, 0x00020032, 0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A,
    0x0002001D, 0xFE363C8A, 0x00000001, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00100100, 0xFE363C85, 0x00000020,
    0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x00000021, 0x00010022, 0xFE363C85, 0x0001002B,
    0x00000002, 0x0001002B, 0x00000003, 0x0001002B, 0x00000006, 0x0001002B,
    0x00000004, 0x0000002E, 0x00010024, 0x00000001, 0x0000002A, 0x00000031,
    0x00020019, 0xFE363C85, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PTR_A1_2_802f87ac, 0x0001000A, 0x000000A6, 0x00020018, 0xFE363C85,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021,
    0x00020018, 0xFE363C85, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000027, 0x00000021, 0x00020018, 0xFE363C85, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x0001000A, 0x000001F4,
    0x00010004, 0x0000005A, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PTR_A1_2_802f87ac, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000004, 0xFE363C80,
    0x0001000A, 0xFE363C80, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_VIVIAN_ATTAC_802f87b8, 0x00000000,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00020100, 0xFE363C85, 0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x20000100, 0xFE363C85, 0x00000021,
    0x0002001E, 0xFE363C80, 0x00000002, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFE363C86, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF,
    0x00000021, 0x0001000A, 0x000001F4, 0x00010003, 0x0000005A, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_A1_1_802f87d0, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0x00000000, 0x00000005,
    0x00000000, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003,
    0x00000000, 0x0005005B, (u32)battle_evt_majo_disp_off, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000028, 0x00000000, 0x00000005, 0x00000000, 0xFFFFFFFF,
    0x0000006E, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000000E, 0x0000001D,
    0x0000001D, 0x00000000, 0x00000070, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)battle_evt_majo_disp_on, 0xFFFFFFFE,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018, 0xFE363C86, 0x00000002,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C80, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_Y_1_802f866c, 0x00010009, 0x00000034,
    0x00000021, 0x00010003, 0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyVivianAttack_ShadowGuard[430] = {
    0x00020032, 0xFE363C83, 0xFFFFFFFD, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFE363C83,
    0xFE363C84, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0005005B,
    (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000014, 0x00000000, 0x00000000, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFFFFFFFF, 0x00000010,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_A2_1_802f87dc, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFE363C83, 0xFE363C80, 0x0000000F, 0x00020035, 0xFE363C82, 0x00000005,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8280, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83,
    0xFFFFFFFF, 0x00000010, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x000001A4, 0x00010024, 0xFFFFFFFE,
    0x00020032, 0xFE363C80, 0x0000014A, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x0000010E, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80,
    0x000000F0, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x000000C8,
    0x00010024, 0x00000002, 0x00020032, 0xFE363C80, 0x000000A0, 0x0000002A,
    0x00020032, 0xFE363C80, 0x00000078, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0xFE363C80, 0x00000001, 0x00000005, 0xFFFFFFFE, 0x00000005, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x0000000A, 0x00000001, 0x00000000, 0x00010009,
    0x00000016, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001F, 0xFE363C80, 0x00000002,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8280, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x00010004, 0x0000005A, 0x00000021, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFFFFFFFF, 0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFE363C86, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E,
    0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_A2_2_802f87e8, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PTR_A2_3_802f87f4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0000006E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C83, 0x00000001, (u32)(char*)str_M_B_3_804241e8,
    0x0005005B, (u32)battle_evt_majo_disp_off, 0xFE363C83, 0x00000001, 0x00000001, 0x00000000,
    0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x00000010, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute,
    0xFE363C83, 0x00000001, 0x03000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000070,
    0x0005005B, (u32)battle_evt_majo_disp_off, 0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000001,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C85, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C80, 0xFE363C85, 0x00020038, 0xFE363C80, 0x00000002,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x00000001, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_ripple_802f8800,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0001000A, 0x0000014D, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x01010000, 0x00010009, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000004, 0x00000001, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00010009, 0x00000001, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000004, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x0001000A, 0x000001F4, 0x00010003,
    0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003,
    0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00010003, 0x00000063, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyVivianAttack_MagicalPowder[448] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x00010009,
    0x00000014, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036,
    0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD,
    0x00020032, 0xFE363C80, 0x00000168, 0x00010024, 0xFFFFFFFE, 0x00020032,
    0xFE363C80, 0x0000010E, 0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80,
    0x000000D2, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80, 0x000000B4,
    0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x000000A0, 0x00010024,
    0x00000002, 0x00020032, 0xFE363C80, 0x0000008C, 0x0000002A, 0x00020032,
    0xFE363C80, 0x00000078, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0xFE363C80,
    0x00000001, 0x00000005, 0xFFFFFFFD, 0x00000001, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000007, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x0000000A, 0x00000001, 0x00000000, 0x00010009, 0x00000016,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000190, 0x0005005B,
    (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000014, 0x00000000, 0x00000000, 0x0002005B,
    (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetWidth, 0xFFFFFFFE, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C86, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_stardust_802f8808, 0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C85,
    0xFE363C86, 0x0000001E, 0x0000003C, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000070, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_VIVIAN_MAGIC_802f8814,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PTR_A3_1_802f882c, 0x0003005B, (u32)btlevtcmd_ftof, 0x00000014, 0xFE363C80,
    0x00010009, 0x00000014, 0x0003005B, (u32)btlevtcmd_ftof, 0x0000001A, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_ftof, 0x0000002E, 0xFE363C81, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C81, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029,
    0x00000002, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PTR_Y_1_802f866c, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000028, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00010003, 0x0000000A, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_kemuri_test_802f8838, 0x00000006, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C86, 0x00020019, 0xFE363C86, 0x00000001, 0x00020018,
    0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026,
    0x00000021, 0x00020018, 0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C86, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x00010004,
    0x00000032, 0x00000021, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80,
    0x0002001C, 0xFE363C80, 0x00000003, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00020100, 0xFE363C85, 0x00000020, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x00000021, 0x00020018, 0xFE363C85, 0x00000012, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000000, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010028, 0x00000000,
    0x00020032, 0xFE363C80, 0xFFFFFFFF, 0x0001002B, 0x00000001, 0x0001002B,
    0x00000002, 0x00020032, 0xFE363C80, 0x00000000, 0x0000002E, 0x0001002B,
    0x00000003, 0x0001002B, 0x00000004, 0x00020032, 0xFE363C80, 0x00000001,
    0x0000002E, 0x0000002A, 0x00020032, 0xFE363C80, 0x00000002, 0x00000031,
    0x0002001D, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0xFE363C80, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFE363C86, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF,
    0x00000021, 0x00000021, 0x00010003, 0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C83, 0xFE363C84, 0x00020019, 0xFE363C83, 0xFFFFFFFF, 0x00020035,
    0xFE363C8A, 0x00000001, 0x00010004, 0x0000000A, 0x00000021, 0x00010003,
    0x00000050, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003,
    0x0000005A, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PTR_A1_1_802f87d0, 0x0001000A, 0x00000320, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyVivianAttack_CharmKissAttack[464] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x00010009,
    0x00000014, 0x00020032, 0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_A4_1_802f8844, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_PARTY_MOJIMOJI1_802f8850, 0x00000000, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty,
    0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80,
    0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C85, 0x0000003C, 0x00010024,
    0xFFFFFFFE, 0x00020032, 0xFE363C85, 0x0000002D, 0x00010024, 0xFFFFFFFF,
    0x00020032, 0xFE363C85, 0x0000001E, 0x00010024, 0x00000000, 0x00020032,
    0xFE363C85, 0x00000019, 0x00010024, 0x00000001, 0x00020032, 0xFE363C85,
    0x00000014, 0x00010024, 0x00000002, 0x00020032, 0xFE363C85, 0x0000000C,
    0x0000002A, 0x00020032, 0xFE363C85, 0x00000006, 0x00000031, 0x0003005B,
    (u32)evt_sub_random, 0xFE363C81, 0x00000059, 0x00020035, 0xFE363C81, 0x0000001E,
    0x00020032, 0xFE363C82, 0xFE363C81, 0x00020036, 0xFE363C82, 0xFE363C85,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000005, 0xFE363C81, 0xFE363C82, 0xFE363C81,
    0xF1194D80, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002001B, 0xFE363C8A,
    0x00000000, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000009, 0x00000020, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000008, 0x00000021, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE,
    0x0000000B, 0x00000001, 0x00000000, 0x00010009, 0x00000016, 0x0002001B,
    0xFE363C8A, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000000, 0x00000020,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x00000021, 0x0001006F, 0xFE363C8F,
    0x00020032, 0xFE363C80, 0x00000005, 0x00010005, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000,
    0x0006005B, (u32)_disp_heart_entry, 0x0000000A, 0x00000014, 0x00000019, 0xFE363C83,
    0xFE363C84, 0x00010009, 0x00000001, 0x00000007, 0x00000021, 0x0002005B,
    (u32)_disp_heart_entry_stop_check, 0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000, 0x00000007,
    0x00000021, 0x00020032, 0xFE363C80, 0x00000005, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C87, 0x0002006A, 0xFE363C8F, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0002005B, (u32)_disp_heart_entry_stop, 0xFE363C8F, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_A4_2_802f8864, 0x0001000A, 0x000000C8,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_VIVIAN_MEROM_802f8870, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000F,
    0x00020035, 0xFE363C81, 0x0000001E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C86, 0x00020037, 0xFE363C86, 0x0000002D, 0x000F005B, (u32)evt_eff,
    0x00000000, (u32)(char*)str_kiss_804241f0, 0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C86, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C86, 0x00020019, 0xFE363C86, 0x00000001,
    0x00020018, 0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C86, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C86,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x00010004, 0x00000032, 0x00000021, 0x0002001E, 0xFE363C87, 0x00000002,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100,
    0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x20000100, 0xFE363C85, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000012, 0x0002001E, 0xFE363C87, 0x00000002, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x0000000A, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kiss_804241f0,
    0x00000005, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000021, 0x00000021, 0x0000006E,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000001E, 0x0000001E,
    0x00000000, 0x00000070, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018, 0xFE363C86,
    0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_Y_1_802f866c, 0x00010009,
    0x00000020, 0x00000021, 0x00010009, 0x00000005, 0x00010003, 0x00000032,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020019, 0xFE363C83,
    0xFFFFFFFF, 0x00020035, 0xFE363C8A, 0x00000001, 0x00010004, 0x0000000A,
    0x00000021, 0x00010003, 0x00000050, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005A, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_A1_1_802f87d0,
    0x00010009, 0x00000030, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

u32 vivian_shadow_tail_event[78] = {
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000002, 0xFE363C82, 0x00010003, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetUnitId,
    0xFFFFFFFE, 0xFE363C85, 0x00020018, 0xFE363C85, 0xFFFFFFFF, 0x00000002,
    0x00000021, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFE363C85,
    0x00020018, 0xFE363C85, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFE363C81,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFE363C82, 0x00000021,
    0x00010009, 0x00000001, 0x00010004, 0x00000000, 0x00000002, 0x00000001
};

u32 vivian_counter_damage_event[177] = {
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80, 0xF24A7E80,
    0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFFFFFFFE, 0x00000001, 0x00020018, 0xFE363C80,
    0x0000002B, 0x00020032, 0xFE363C8E, 0x00000024, 0x00000020, 0x00020032,
    0xFE363C8E, 0x00000027, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C8B, 0xFE363C8E, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7E80,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C82, 0x00000019, 0x00020035,
    0xFE363C83, 0x00000019, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C85, 0x00020019, 0xFE363C85, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000001, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000002, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0x00000000, 0x00000005, 0x00000000, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x0005005B,
    (u32)battle_evt_majo_disp_off, 0xFFFFFFFE, 0x00000001, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x000A005B,
    (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050,
    0x00000000, 0x00000005, 0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)battle_evt_majo_disp_on,
    0xFFFFFFFE, 0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000020, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF,
    0x00000021, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 vivian_appeal_event[80] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_O_1_802f888c, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_APPEAL1_802f8894,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010009, 0x0000000A,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_O_2_802f88a8, 0x0001000A,
    0x0000014D, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000F,
    0x00020035, 0xFE363C81, 0x0000001E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C8E, 0x00020037, 0xFE363C8E, 0x0000002D, 0x000F005B, (u32)evt_eff,
    (u32)(u8*)zero_804241f8, (u32)(char*)str_kiss_804241f0, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C8E, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0001000A, 0x0000014D, 0x0002005B, (u32)btlevtcmd_AudienceAppeal,
    0xFFFFFFFE, 0x0001000A, 0x000001F4, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

static u32 party_win_reaction[18] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PTR_Y_1_802f866c, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0001000A, 0x000001F4, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00000002, 0x00000001
};

static u32 attack_audience_event[189] = {
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
    0x00000000, 0x000000C8, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PTR_A1_2_802f87ac, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000004, 0xFE363C80, 0x0001000A,
    0xFE363C80, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_VIVIAN_ATTAC_802f87b8, 0x00000000, 0x0003005B,
    (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0001006F, 0xFE363C8F, 0x0001000A,
    0x000005DC, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A,
    0x000003E8, 0x00000070, 0x0001000A, 0x000001F4, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8A80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos,
    0xFFFFFFFE, 0xFE363C88, 0xFE363C81, 0xFE363C82, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001
};

/* Recovered target Vivian .rodata safe prefix strings; code-coupled PTR_D_4 parked. */
const char str_PTR_X_1_802f84d8[] = "PTR_X_1";
const char str_PTR_D_2_802f84e0[] = "PTR_D_2";
const char str_PTR_N_1_802f84e8[] = "PTR_N_1";
const char str_PTR_D_3_802f84f0[] = "PTR_D_3";
const char str_PTR_S_1_802f84f8[] = "PTR_S_1";
const char str_PTR_G_2_802f8500[] = "PTR_G_2";
const char str_PTR_G_1_802f8508[] = "PTR_G_1";
const char str_PTR_D_1_802f8510[] = "PTR_D_1";
const char str_PTR_W_1_802f8518[] = "PTR_W_1";
const char str_PTR_R_1_802f8520[] = "PTR_R_1";
const char str_PTR_T_1_802f8528[] = "PTR_T_1";
const char str_PTR_I_1_802f8530[] = "PTR_I_1";
const char str_PTR_C_1_802f8538[] = "PTR_C_1";
const char str_btl_un_vivian_802f8540[] = "btl_un_vivian";
const char str_c_vivian_802f8550[] = "c_vivian";
const char str_SFX_PARTY_DAMAGE1_802f855c[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f8570[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f8588[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f85a0[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";

/* Recovered target Vivian .rodata zero-vector pair. */
const f32 vec3_802f85bc[3] = {0.0f, 0.0f, 0.0f};
const f32 vec3_802f85c8[3] = {0.0f, 0.0f, 0.0f};

/* Recovered target Vivian .rodata middle string catalog; stop before conversion double. */
const char str_SFX_PARTY_BATTLE_SUB_802f85d4[] = "SFX_PARTY_BATTLE_SUBHANG1";
const char str_SFX_PARTY_BATTLE_MOV_802f85f0[] = "SFX_PARTY_BATTLE_MOVE1L";
const char str_SFX_PARTY_BATTLE_MOV_802f8608[] = "SFX_PARTY_BATTLE_MOVE1R";
const char str_SFX_PARTY_BATTLE_HAN_802f8620[] = "SFX_PARTY_BATTLE_HANG1";
const char str_SFX_PARTY_BATTLE_JUM_802f8638[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f8650[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_PTR_Y_1_802f866c[] = "PTR_Y_1";
const char str_btl_wn_ptr_normal_802f8674[] = "btl_wn_ptr_normal";
const char str_msg_ptr_kagenuke_802f8688[] = "msg_ptr_kagenuke";
const char str_msg_ac_kagenuke_802f869c[] = "msg_ac_kagenuke";
const char str_btl_wn_ptr_lv1_802f86ac[] = "btl_wn_ptr_lv1";
const char str_msg_ptr_kagegakure_802f86bc[] = "msg_ptr_kagegakure";
const char str_msg_ac_kagegakure_802f86d0[] = "msg_ac_kagegakure";
const char str_btl_wn_ptr_lv2_802f86e4[] = "btl_wn_ptr_lv2";
const char str_msg_ptr_mahou_no_kon_802f86f4[] = "msg_ptr_mahou_no_kona";
const char str_msg_ac_mahou_no_kona_802f870c[] = "msg_ac_mahou_no_kona";
const char str_btl_wn_ptr_lv3_802f8724[] = "btl_wn_ptr_lv3";
const char str_msg_ptr_meromero_kis_802f8734[] = "msg_ptr_meromero_kiss";
const char str_msg_ac_meromero_kiss_802f874c[] = "msg_ac_meromero_kiss";
const char str_SFX_BTL_VIVIAN_ATTAC_802f8764[] = "SFX_BTL_VIVIAN_ATTACK1";
const char str_SFX_BTL_VIVIAN_ATTAC_802f877c[] = "SFX_BTL_VIVIAN_ATTACK2";
const char str_SFX_BTL_VIVIAN_ATTAC_802f8794[] = "SFX_BTL_VIVIAN_ATTACK3";
const char str_PTR_A1_2_802f87ac[] = "PTR_A1_2";
const char str_SFX_BTL_VIVIAN_ATTAC_802f87b8[] = "SFX_BTL_VIVIAN_ATTACK4";
const char str_PTR_A1_1_802f87d0[] = "PTR_A1_1";
const char str_PTR_A2_1_802f87dc[] = "PTR_A2_1";
const char str_PTR_A2_2_802f87e8[] = "PTR_A2_2";
const char str_PTR_A2_3_802f87f4[] = "PTR_A2_3";
const char str_ripple_802f8800[] = "ripple";
const char str_stardust_802f8808[] = "stardust";
const char str_SFX_BTL_VIVIAN_MAGIC_802f8814[] = "SFX_BTL_VIVIAN_MAGIC1";
const char str_PTR_A3_1_802f882c[] = "PTR_A3_1";
const char str_kemuri_test_802f8838[] = "kemuri_test";
const char str_PTR_A4_1_802f8844[] = "PTR_A4_1";
const char str_SFX_PARTY_MOJIMOJI1_802f8850[] = "SFX_PARTY_MOJIMOJI1";
const char str_PTR_A4_2_802f8864[] = "PTR_A4_2";
const char str_SFX_BTL_VIVIAN_MEROM_802f8870[] = "SFX_BTL_VIVIAN_MEROMERO1";
const char str_PTR_O_1_802f888c[] = "PTR_O_1";
const char str_SFX_MARIO_APPEAL1_802f8894[] = "SFX_MARIO_APPEAL1";
const char str_PTR_O_2_802f88a8[] = "PTR_O_2";

/* Recovered target Vivian late .rodata strings; PTR_D_4/double remain parked. */
const char str_p_bibi_802f88b8[] = "p_bibi";
const char str_PM_B_1_802f88c0[] = "PM_B_1";
const char str_SFX_BTL_VIVIAN_SHADO_802f88c8[] = "SFX_BTL_VIVIAN_SHADOW1_2";
const char str_SFX_BTL_VIVIAN_SHADO_802f88e4[] = "SFX_BTL_VIVIAN_SHADOW1_1";
const char str_PM_B_2_802f8900[] = "PM_B_2";

/* Recovered target Vivian .sdata2 family, defined after all consumers. */
const char str_M_B_3_804241e8[] = "M_B_3";
const char str_kiss_804241f0[] = "kiss";
const u8 zero_804241f8[1] = {0};
const f32 float_0_804241fc = 0.0f;
const f32 float_1p2_80424200 = 1.2f;
const f32 float_1_80424204 = 1.0f;
const f32 float_10_80424208 = 10.0f;
const f32 float_16_8042420c = 16.0f;
const f32 float_5_80424210 = 5.0f;
const f32 float_neg30_80424214 = -30.0f;
const f32 float_20_80424218 = 20.0f;
const f32 float_2_8042421c = 2.0f;
const f32 float_8_80424220 = 8.0f;
const f32 float_500_80424224 = 500.0f;
const f32 float_200_80424228 = 200.0f;
const f32 float_0p66667_8042422c = 0.6666667f;
