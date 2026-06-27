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


u8 battle_evt_majo_disp_on(void) {
    return 0;
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
    ;
}


s32 battle_evt_majo_disp_off(int param_1, int param_2) {
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

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, -2));

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
int unk_80182cc4() {
    return 0;
}
