#include "event/evt_light.h"

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
