#include "unit/unit_mario.h"

void mapSetTevCallback(s32 type, void* callback);
void callback(void);
s32 evtSetValue(void* event, s32 target, s32 value);
void* pouchGetPtr(void);
s32 BattleTransID(void* event, s32 type);
void* BattleGetUnitPartsPtr(s32 unitId, s32 partId);
void* animPoseGetAnimPosePtr(s32 poseId);
void BtlUnit_snd_se(void* unit, const char* name, u32 flags, s32 zero);

extern char str_SFX_VOICE_MARIO_RELI_802f4250[];
extern char str_SFX_VOICE_MARIO_RELI_802f4268[];


s32 _get_mario_hammer_lv(void* event) {
    s32* args = *(s32**)((s32)event + 0x18);
    evtSetValue(event, args[0], *(s8*)((s32)pouchGetPtr() + 0x99));
    return 2;
}

s32 _get_local_frame(void* event) {
    s32 arg0 = **(s32**)((s32)event + 0x18);
    void* parts = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    void* pose = animPoseGetAnimPosePtr(*(s32*)((s32)parts + 0x1C0));
    evtSetValue(event, arg0, (s32)*(f32*)((s32)pose + 0x24));
    return 2;
}

void mario_pinch_pose_sound_callback(void* unit) {
    if (*(s32*)((s32)unit + 0x238) & 1) {
        BtlUnit_snd_se(unit, str_SFX_VOICE_MARIO_RELI_802f4250, 0xF1194D80, 0);
    } else {
        BtlUnit_snd_se(unit, str_SFX_VOICE_MARIO_RELI_802f4268, 0xF1194D80, 0);
    }
    *(s32*)((s32)unit + 0x238) = *(s32*)((s32)unit + 0x238) + 1;
}

void faker_mario_makkuro_set(void) {
    mapSetTevCallback(0xB, callback);
}


s32 _mario_super_emblem_anim_set(int param_1, int param_2) {
    return 0;
}


s32 _mario_makkuro_set(void) {
    return 0;
}
