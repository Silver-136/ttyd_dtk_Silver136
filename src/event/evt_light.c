#include "evt/evt_light.h"
#include "manager/evtmgr_cmd.h"
#include "driver/animdrv.h"
#include "driver/lightdrv.h"
#include "driver/mapdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"

// BOOL on
USER_FUNC(evt_light_charalight_all_onoff) {
    s32 i;

    if (evtGetValue(event, *event->args)) {
        for (i = 0; i < lightGetEntryNum(); i++) {
            lightNumberToPtr(i)->flags |= LIGHT_CHARA;
        }
    } else {
        for (i = 0; i < lightGetEntryNum(); i++) {
            lightNumberToPtr(i)->flags &= ~LIGHT_CHARA;
        }
    }
    return EVT_RETURN_DONE;
}

// BOOL on
USER_FUNC(evt_light_mario_lighting_onoff) {
    BOOL on;
    MarioWork* mario;

    on = evtGetValue(event, *event->args);
    mario = marioGetPtr();
    if (on) {
        animPoseSetMaterialLightFlagOn(mario->paperAnimGroupId[0], 2);
        animPoseSetMaterialLightFlagOn(mario->paperAnimGroupId[1], 2);
        animPoseSetMaterialLightFlagOn(mario->paperAnimGroupId[2], 2);
    } else {
        animPoseSetMaterialLightFlagOff(mario->paperAnimGroupId[0], 2);
        animPoseSetMaterialLightFlagOff(mario->paperAnimGroupId[1], 2);
        animPoseSetMaterialLightFlagOff(mario->paperAnimGroupId[2], 2);
    }
    return EVT_RETURN_DONE;
}

// BOOL on
USER_FUNC(evt_light_party_lighting_onoff) {
    BOOL on;
    PartyEntry* party;

    on = evtGetValue(event, *event->args);
    party = partyGetPtr(marioGetPartyId());
    if (on) {
        animPoseSetMaterialLightFlagOn(party->paperAnimGroup, 2);
    } else {
        animPoseSetMaterialLightFlagOff(party->paperAnimGroup, 2);
    }
    return EVT_RETURN_DONE;
}

// BOOL on, const char* npcName
USER_FUNC(evt_light_npc_lighting_onoff) {
    EvtWord* args = event->args;
    BOOL on;
    NpcEntry* npc;

    on = evtGetValue(event, args[0]);
    npc = npcNameToPtr((const char*)evtGetValue(event, args[1]));
    if (on) {
        animPoseSetMaterialLightFlagOn(npc->poseId, 2);
    } else {
        animPoseSetMaterialLightFlagOff(npc->poseId, 2);
    }
    return EVT_RETURN_DONE;
}

// const char* lightName, const char* mapobjName, s32 mode
USER_FUNC(evt_light_bind_mapobj) {
    EvtWord* args = event->args;
    const char* lightName;
    const char* mapobjName;
    LightEntry* light;
    MapObject* mapobj;
    s32 mode;
    Vec offset;
    Vec origin;
    Mtx mtx;

    lightName = (const char*)evtGetValue(event, args[0]);
    mapobjName = (const char*)evtGetValue(event, args[1]);
    mode = evtGetValue(event, args[2]);
    light = lightNameToPtr(lightName);
    mapobj = mapGetMapObj(mapobjName);
    if (isFirstCall) {
        offset = (Vec){ 0.0f, 0.0f, 0.0f };
        PSMTXMultVec(mapobj->modelWorldMtx, &offset, &offset);
        PSVECSubtract(&light->basePosition, &offset, &offset);
        event->userdataf[0] = offset.x;
        event->userdataf[1] = offset.y;
        event->userdataf[2] = offset.z;
    }
    offset.x = event->userdataf[0];
    offset.y = event->userdataf[1];
    offset.z = event->userdataf[2];
    switch (mode) {
        case 0:
            PSMTXMultVec(mapobj->transformMtx, &offset, &offset);
            break;
        case 1:
            PSMTXTrans(mtx, mapobj->transformMtx[0][3], mapobj->transformMtx[1][3], mapobj->transformMtx[2][3]);
            PSMTXMultVec(mtx, &offset, &offset);
            break;
    }
    origin = (Vec){ 0.0f, 0.0f, 0.0f };
    PSMTXMultVec(mapobj->modelWorldMtx, &origin, &origin);
    PSVECAdd(&origin, &offset, &light->position);
    return EVT_RETURN_BLOCK;
}
