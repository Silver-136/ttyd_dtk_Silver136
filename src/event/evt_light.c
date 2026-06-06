#include "event/evt_light.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_light_npc_lighting_onoff(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* npcNameToPtr(s32 name);
    extern void animPoseSetMaterialLightFlagOn(void* pose, s32 flag);
    extern void animPoseSetMaterialLightFlagOff(void* pose, s32 flag);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 on = evtGetValue(event, args[0]);
    void* npc = npcNameToPtr(evtGetValue(event, args[1]));

    if (on != 0) {
        animPoseSetMaterialLightFlagOn(*(void**)((s32)npc + 0x104), 2);
    } else {
        animPoseSetMaterialLightFlagOff(*(void**)((s32)npc + 0x104), 2);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_light_party_lighting_onoff(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void animPoseSetMaterialLightFlagOn(void* pose, s32 flag);
    extern void animPoseSetMaterialLightFlagOff(void* pose, s32 flag);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 on = evtGetValue(event, args[0]);
    void* party = partyGetPtr(marioGetPartyId());

    if (on != 0) {
        animPoseSetMaterialLightFlagOn(*(void**)((s32)party + 0xC), 2);
    } else {
        animPoseSetMaterialLightFlagOff(*(void**)((s32)party + 0xC), 2);
    }
    return 2;
}


typedef struct EvtLightVec {
    f32 x;
    f32 y;
    f32 z;
} EvtLightVec;

extern EvtLightVec vec3_802f3748[];
extern EvtLightVec vec3_802f3754[];

s32 evt_light_bind_mapobj(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* lightNameToPtr(s32 name);
    extern void* mapGetMapObj(s32 name);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSVECAdd(void* a, void* b, void* out);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 lightName = evtGetValue(event, args[0]);
    s32 mapObjName = evtGetValue(event, args[1]);
    s32 bindType = evtGetValue(event, args[2]);
    void* light = lightNameToPtr(lightName);
    void* mapObj = mapGetMapObj(mapObjName);
    EvtLightVec pos;
    EvtLightVec base;
    f32 mtx[3][4];

    if (isFirstCall != 0) {
        pos = vec3_802f3748[0];
        PSMTXMultVec((void*)((s32)mapObj + 0x1C), &pos, &pos);
        PSVECSubtract((void*)((s32)light + 0x3C), &pos, &pos);
        *(f32*)((s32)event + 0x78) = pos.x;
        *(f32*)((s32)event + 0x7C) = pos.y;
        *(f32*)((s32)event + 0x80) = pos.z;
    }

    pos.x = *(f32*)((s32)event + 0x78);
    pos.y = *(f32*)((s32)event + 0x7C);
    pos.z = *(f32*)((s32)event + 0x80);
    switch (bindType) {
        case 0:
            PSMTXMultVec((void*)((s32)mapObj + 0xAC), &pos, &pos);
            break;
        case 1:
            PSMTXTrans(
                mtx,
                *(f32*)((s32)mapObj + 0xB8),
                *(f32*)((s32)mapObj + 0xC8),
                *(f32*)((s32)mapObj + 0xD8)
            );
            PSMTXMultVec(mtx, &pos, &pos);
            break;
    }

    base = vec3_802f3754[0];
    PSMTXMultVec((void*)((s32)mapObj + 0x1C), &base, &base);
    PSVECAdd(&base, &pos, (void*)((s32)light + 0x24));
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_light_charalight_all_onoff(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 lightGetEntryNum(void);
    extern void* lightNumberToPtr(s32 index);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 on = evtGetValue(event, args[0]);
    s32 i;

    if (on != 0) {
        for (i = 0; i < lightGetEntryNum(); i++) {
            *(u16*)lightNumberToPtr(i) |= 0x80;
        }
    } else {
        for (i = 0; i < lightGetEntryNum(); i++) {
            *(u16*)lightNumberToPtr(i) &= ~0x80;
        }
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_light_mario_lighting_onoff(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* marioGetPtr(void);
    extern void animPoseSetMaterialLightFlagOn(void* pose, s32 flag);
    extern void animPoseSetMaterialLightFlagOff(void* pose, s32 flag);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 on = evtGetValue(event, args[0]);
    void* mario = marioGetPtr();

    if (on != 0) {
        animPoseSetMaterialLightFlagOn(*(void**)((s32)mario + 0x22C), 2);
        animPoseSetMaterialLightFlagOn(*(void**)((s32)mario + 0x230), 2);
        animPoseSetMaterialLightFlagOn(*(void**)((s32)mario + 0x234), 2);
    } else {
        animPoseSetMaterialLightFlagOff(*(void**)((s32)mario + 0x22C), 2);
        animPoseSetMaterialLightFlagOff(*(void**)((s32)mario + 0x230), 2);
        animPoseSetMaterialLightFlagOff(*(void**)((s32)mario + 0x234), 2);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
