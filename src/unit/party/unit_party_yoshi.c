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


u8 _gundan_yoshi_run_effect(void) {
    return 0;
}


u8 btl_yoshi_yoroyoro_jump_move(void) {
    return 0;
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


u8 btl_yoshi_yoroyoro_jump_calc_param(void) {
    return 0;
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

u32 _wait_yoshig_complete(int param_1) {
    return 0;
}


void _yoshi_slide_move_sound(void* unit, s32 param_2) {
}
