// Script commands for character and map lighting.

#include "evt/evt_light.h"
#include "manager/evtmgr_cmd.h"
#include "driver/animdrv.h"
#include "driver/lightdrv.h"
#include "driver/mapdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"

/** BOOL on, const char* lightName */
// Original name and size (0x78) from the JP demo map (UNUSED there: no script uses it); the map lists it before
// evt_light_charalight_all_onoff, whose flag it sets or clears on one named light (arguments in the order of
// evt_light_npc_lighting_onoff).
USER_FUNC(evt_light_charalight_onoff) {
    EvtWord* args = event->args;
    BOOL on;
    LightEntry* light;

    on = evtGetValue(event, args[0]);
    light = lightNameToPtr((const char*)evtGetValue(event, args[1]));
    if (on) {
        light->flags |= LIGHT_CHARA;
    } else {
        light->flags &= ~LIGHT_CHARA;
    }
    return EVT_RETURN_DONE;
}

/** BOOL on */
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

/** BOOL on */
USER_FUNC(evt_light_mario_lighting_onoff) {
    BOOL on;
    MarioWork* mario;

    on = evtGetValue(event, *event->args);
    mario = marioGetPtr();
    if (on) {
        animPoseSetMaterialLightFlagOn(mario->paperAnimGroupId[0], MATERIAL_LIGHT_CHARA);
        animPoseSetMaterialLightFlagOn(mario->paperAnimGroupId[1], MATERIAL_LIGHT_CHARA);
        animPoseSetMaterialLightFlagOn(mario->paperAnimGroupId[2], MATERIAL_LIGHT_CHARA);
    } else {
        animPoseSetMaterialLightFlagOff(mario->paperAnimGroupId[0], MATERIAL_LIGHT_CHARA);
        animPoseSetMaterialLightFlagOff(mario->paperAnimGroupId[1], MATERIAL_LIGHT_CHARA);
        animPoseSetMaterialLightFlagOff(mario->paperAnimGroupId[2], MATERIAL_LIGHT_CHARA);
    }
    return EVT_RETURN_DONE;
}

/** BOOL on */
USER_FUNC(evt_light_party_lighting_onoff) {
    BOOL on;
    PartyEntry* party;

    on = evtGetValue(event, *event->args);
    party = partyGetPtr(marioGetPartyId());
    if (on) {
        animPoseSetMaterialLightFlagOn(party->paperAnimGroup, MATERIAL_LIGHT_CHARA);
    } else {
        animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_CHARA);
    }
    return EVT_RETURN_DONE;
}

/** BOOL on, const char* npcName */
USER_FUNC(evt_light_npc_lighting_onoff) {
    EvtWord* args = event->args;
    BOOL on;
    NpcEntry* npc;

    on = evtGetValue(event, args[0]);
    npc = npcNameToPtr((const char*)evtGetValue(event, args[1]));
    if (on) {
        animPoseSetMaterialLightFlagOn(npc->poseId, MATERIAL_LIGHT_CHARA);
    } else {
        animPoseSetMaterialLightFlagOff(npc->poseId, MATERIAL_LIGHT_CHARA);
    }
    return EVT_RETURN_DONE;
}

/** const char* lightName, out: x, y, z */
// evt_light_get_position, evt_light_get_dir, evt_light_get_color and evt_light_get_distattn: original names and
// sizes (0x80, 0x80, 0x90, 0x80) from the JP demo map (UNUSED there: no script uses them). Each looks the light up as
// evt_light_get_flag does and returns the LightEntry member its name gives: the position, the direction (lightEntry's
// rotation), the four colour bytes and the three distance attenuation coefficients. The map's setters and
// evt_light_get_intensity are not restored (their bodies are not known).
USER_FUNC(evt_light_get_position) {
    EvtWord* args = event->args;
    LightEntry* light = lightNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetFloat(event, args[1], light->position.x);
    evtSetFloat(event, args[2], light->position.y);
    evtSetFloat(event, args[3], light->position.z);
    return EVT_RETURN_DONE;
}

/** const char* lightName, out: x, y, z */
// Original name and size from the JP demo map (see evt_light_get_position).
USER_FUNC(evt_light_get_dir) {
    EvtWord* args = event->args;
    LightEntry* light = lightNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetFloat(event, args[1], light->rotation.x);
    evtSetFloat(event, args[2], light->rotation.y);
    evtSetFloat(event, args[3], light->rotation.z);
    return EVT_RETURN_DONE;
}

/** const char* lightName, out: r, g, b, a */
// Original name and size from the JP demo map (see evt_light_get_position).
USER_FUNC(evt_light_get_color) {
    EvtWord* args = event->args;
    LightEntry* light = lightNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetValue(event, args[1], light->color.r);
    evtSetValue(event, args[2], light->color.g);
    evtSetValue(event, args[3], light->color.b);
    evtSetValue(event, args[4], light->color.a);
    return EVT_RETURN_DONE;
}

/** const char* lightName, out: the three distance attenuation coefficients */
// Original name and size from the JP demo map (see evt_light_get_position).
USER_FUNC(evt_light_get_distattn) {
    EvtWord* args = event->args;
    LightEntry* light = lightNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetFloat(event, args[1], light->attenDistCoef.x);
    evtSetFloat(event, args[2], light->attenDistCoef.y);
    evtSetFloat(event, args[3], light->attenDistCoef.z);
    return EVT_RETURN_DONE;
}

/** const char* lightName, out: the light's flags */
// Original name and size (0x54) from the JP demo map (UNUSED there: no script uses it); looks the light up as
// evt_light_bind_mapobj does and returns its flags word.
USER_FUNC(evt_light_get_flag) {
    EvtWord* args = event->args;
    LightEntry* light = lightNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetValue(event, args[1], light->flags);
    return EVT_RETURN_DONE;
}

/** const char* lightName, const char* mapobjName, s32 mode */
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
