#include "unit/unit_mario.h"

void mapSetTevCallback(s32 type, void* callback);
void callback(void*);
s32 evtSetValue(void* event, s32 target, s32 value);
void* pouchGetPtr(void);
s32 BattleTransID(void* event, s32 type);
void* BattleGetUnitPartsPtr(s32 unitId, s32 partId);
void* animPoseGetAnimPosePtr(s32 poseId);
void BtlUnit_snd_se(void* unit, const char* name, u32 flags, s32 zero);

extern char str_SFX_VOICE_MARIO_RELI_802f4250[];
extern char str_SFX_VOICE_MARIO_RELI_802f4268[];
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_local_frame(void* event) {
    s32 arg0 = **(s32**)((s32)event + 0x18);
    void* parts = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    void* pose = animPoseGetAnimPosePtr(*(s32*)((s32)parts + 0x1C0));
    evtSetValue(event, arg0, (s32)*(f32*)((s32)pose + 0x24));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
void mario_pinch_pose_sound_callback(void* unit) {
    if (*(s32*)((s32)unit + 0x238) & 1) {
        BtlUnit_snd_se(unit, str_SFX_VOICE_MARIO_RELI_802f4250, 0xF1194D80, 0);
    } else {
        BtlUnit_snd_se(unit, str_SFX_VOICE_MARIO_RELI_802f4268, 0xF1194D80, 0);
    }
    *(s32*)((s32)unit + 0x238) = *(s32*)((s32)unit + 0x238) + 1;
}

void callback(void* tev) {
    extern u32 dat_80422f68;
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 stage, s32 rasSel, s32 texSel);
    u32 color;
    s32 stage;
    s32 texCoord;
    s32 texMap;
    s32 colorChan;

    color = dat_80422f68;
    stage = *(s32*)((s32)tev + 0xC);
    texCoord = *(s32*)((s32)tev + 0x10);
    texMap = *(s32*)((s32)tev + 0x14);
    colorChan = *(s32*)((s32)tev + 0x18);
    GXSetTevColor(1, &color);
    GXSetTevOrder(stage, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(stage, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(stage, 0, 0, 0, 1, 0);
    GXSetTevColorIn(stage, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(stage, 7, 7, 7, 0);
    GXSetTevSwapMode(stage, 0, 0);
    *(s32*)((s32)tev + 0xC) = stage + 1;
    *(s32*)((s32)tev + 0x14) = texMap;
    *(s32*)((s32)tev + 0x18) = colorChan;
    *(s32*)((s32)tev + 0x10) = texCoord;
}

void faker_mario_makkuro_set(void) {
    mapSetTevCallback(0xB, callback);
}
s32 _mario_makkuro_set(void) {
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);

    if (mario != 0) {
        mapSetTevCallback(0xB, callback);
        animPoseSetMaterialFlagOn(*(s32*)((s32)BtlUnit_GetPartsPtr(mario, 1) + 0x1C0), 0x0B000000);
        animPoseSetMaterialFlagOn(*(s32*)((s32)BtlUnit_GetPartsPtr(mario, 2) + 0x1C0), 0x0B000000);
        animPoseSetMaterialFlagOn(*(s32*)((s32)BtlUnit_GetPartsPtr(mario, 3) + 0x1C0), 0x0B000000);
    }
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _mario_super_emblem_anim_set(void* event, s32 releaseOld) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* event, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animEffectAsync(char* name, s32 battle);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern s32 pouchEquipCheckBadge(s32 badge);
    extern char str_M_Z_10_802f3f70[];
    char* base;
    s32 unitId;
    char* poseName;
    void* part;
    void* battleWork;
    s32 badgeFlags;
    s32 id;

    base = str_M_Z_10_802f3f70;
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, **(s32**)((s32)event + 0x18));
    unitId = BattleTransID(event, id);
    BattleGetUnitPtr(battleWork, unitId);

    badgeFlags = 0;
    if (pouchEquipCheckBadge(0x147) != 0) {
        badgeFlags = 1;
    }
    if (pouchEquipCheckBadge(0x148) != 0) {
        badgeFlags += 2;
    }

    switch (badgeFlags) {
        case 1:
            poseName = base + 0x2BC;
            break;
        case 2:
            poseName = base + 0x2C8;
            break;
        case 3:
            poseName = base + 0x2D4;
            break;
        default:
            poseName = 0;
            break;
    }

    if (releaseOld != 0) {
        if (poseName == 0) {
            return 2;
        }
        part = BattleGetUnitPartsPtr(unitId, 1);
        animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }
        part = BattleGetUnitPartsPtr(unitId, 2);
        animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }
        part = BattleGetUnitPartsPtr(unitId, 3);
        animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }
    }

    if (animEffectAsync(poseName, 1) != 0) {
        part = BattleGetUnitPartsPtr(unitId, 1);
        *(s32*)((s32)part + 0x1C0) = animPoseEntry(poseName, 1);
        part = BattleGetUnitPartsPtr(unitId, 2);
        *(s32*)((s32)part + 0x1C0) = animPoseEntry(poseName, 1);
        part = BattleGetUnitPartsPtr(unitId, 3);
        *(s32*)((s32)part + 0x1C0) = animPoseEntry(poseName, 1);
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_mario_hammer_lv(void* event) {
    s32 arg0 = **(s32**)((s32)event + 0x18);
    evtSetValue(event, arg0, *(s8*)((s32)pouchGetPtr() + 0x99));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
