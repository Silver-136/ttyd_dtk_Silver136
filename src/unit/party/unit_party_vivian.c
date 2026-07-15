#include "unit/party/unit_party_vivian.h"

s32 _disp_heart_entry_stop_check(void* evt) {
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    evtSetValue(evt, **(s32**)((s32)evt + 0x18), *(s32*)((s32)evt + 0x8C));
    return 2;
}

s32 _disp_heart_entry_stop(void* evt) {
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void* evtGetPtrID(s32 id);

    void* ptr = evtGetPtrID(evtGetValue(evt, **(s32**)((s32)evt + 0x18)));

    *(s32*)((s32)ptr + 0x8C) = 1;
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 battle_evt_majo_disp_on(void* evt, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern f32 evtGetFloat(void* evt, s32 arg);
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
    extern void animPoseSetPaperAnimGroup(s32 poseId, char* group, s32 flags);
    extern char str_PTR_D_4_802f84d0[];
    extern f32 float_0_804241fc;
    extern f32 float_1_80424204;
    extern f32 float_2_8042421c;
    extern f32 float_200_80424228;
    extern f32 float_0p66667_8042422c;
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
        *(f32*)((s32)unit + 0x3C) = x;
        *(f32*)((s32)unit + 0x40) = y - float_200_80424228;
        *(f32*)((s32)unit + 0x44) = z;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* _battleWorkPointer;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_8038B6B0[];
    s32 techLv;
    void* party;
    u8* data;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_8038B6B0;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
    *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x830;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
    *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
        *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
    *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
        msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x8F0;
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
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x9B0;
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
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0xA70;
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

s32 battle_evt_majo_disp_off(void* evt, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
    extern s32 animEffectAsync(char* name, s32 flags);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 frame, s32 duration);
    extern void animPoseSetEffect(s32 poseId, char* name, s32 flags);
    extern void animPoseSetEffectAnim(s32 poseId, char* name, s32 flags);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void animPoseSetLocalTime(void* anim, f32 time);
    extern f32 animPoseGetLoopTimes(void* anim);
    extern void psndSFXOn_3D(char* name, void* pos);
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 group, s32 flags);
    extern char str_PTR_D_4_802f84d0[];
    extern f32 float_0_804241fc;
    extern f32 float_1_80424204;
    extern f32 float_2_8042421c;
    extern f32 float_8_80424220;
    extern f32 float_500_80424224;
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

void _disp_heart(s32 unused, void* work) {
    extern void* _battleWorkPointer;
    extern f32 float_0_804241fc;
    extern f32 float_1p2_80424200;
    extern f32 float_1_80424204;
    extern f32 float_10_80424208;
    extern f32 float_16_8042420c;
    extern f32 float_5_80424210;
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 frame, s32 duration);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 _disp_heart_entry(void* evt, int isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void dispEntry(s32 prio, s32 cameraId, void* callback, void* param, f32 z);
    extern void _disp_heart(void);
    extern f32 float_0_804241fc;

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


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 _get_move_frame(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
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

s32 _make_kagenuke_weapon(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
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

s32 _vivian_make_extra_work_area(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
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



/* CHATGPT FALLBACK MISSING STUBS: main/unit/party/unit_party_vivian 20260624_191429 */

/* fallback stub-fill: map=unk_80182cc4 addr=0x80182cc4 size=0x00000260 */
int unk_80182cc4(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_GetScale(void* unit, f32* x, f32* y, f32* z);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern f32 float_neg30_80424214;
    extern f32 float_20_80424218;
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

