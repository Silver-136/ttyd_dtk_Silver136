#include "eff/eff_fire.h"
#include "driver/animdrv.h"
#include "driver/hitdrv.h"
#include "driver/mapdrv.h"
#include "driver/mobjdrv.h"
#include "evt/evt_case.h"
#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "party.h"
#include <dolphin/gx.h>
#include <string.h>
#include "evt/evt_map.h"

EffectEntry* effTorchEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
EffectEntry* effFireEntry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale);

void mapObjGetPos(const char* name, Vec* pos);
void mapObjFlagOn(const char* name, u32 flags);
void mapObjFlagOff(const char* name, u32 flags);
void mapGrpFlagOn(const char* name, u32 flags);
void mapGrpFlagOff(const char* name, u32 flags);
void mapObjGetFlushColor(const char* name, u8* r, u8* g, u8* b, u8* a);
void mapObjSetFlushColor(const char* name, u8 r, u8 g, u8 b, u8 a);
void mapGrpSetFlushColor(const char* name, u8 r, u8 g, u8 b, u8 a);
void mapObjFlushOn(const char* name);
void mapObjFlushOff(const char* name);
void mapGrpFlushOn(const char* name);
void mapGrpFlushOff(const char* name);
void mapObjClearOffScreen(const char* name);
void mapGrpClearOffScreen(const char* name);
void mapObjSetOffScreen(const char* name, s32 val);
void mapGrpSetOffScreen(const char* name, s32 val);
void mapSetPlayRate(const char* name, f32 rate);
void mapReStartAnimation(const char* name);
void mapReStartAnimationAll(const char* name);
void mapPauseAnimation(const char* name);
void mapPauseAnimationAll(const char* name);
void mapPlayAnimationLv(const char* name, s32 mode, s32 level);
void mapObjSetColor(const char* name, GXColor color);
void mapGrpSetColor(const char* name, GXColor color);
void mapBlendOff(void);
void mapBlendOff2(void);
void mapSetBlend(GXColor color);
void mapSetBlend2(GXColor color);
void mapFogOn(void);
void mapFogOff(void);
void mapGetFog(s32* type, f32* start, f32* end, GXColor* color);
void mapSetFog(s32 type, f32 start, f32 end, GXColor color);
void mapObjScale(const char* name, f32 x, f32 y, f32 z);
void mapObjRotate(const char* name, f32 x, f32 y, f32 z);
void mapObjTranslate(const char* name, f32 x, f32 y, f32 z);

USER_FUNC(N_evt_mapdisp_onoff);
USER_FUNC(evt_map_replace_mapobj);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(check2);
static USER_FUNC(check);
USER_FUNC(evt_map_get_flush_color);
USER_FUNC(evt_map_set_flush_color);
USER_FUNC(evt_map_set_flush_onoff);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_clear_offscreen);
USER_FUNC(evt_mapobj_set_offscreen);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_map_set_playrate);
USER_FUNC(evt_map_replayanim);
USER_FUNC(evt_map_pauseanim);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_map_blend_set_mobj_flag);
USER_FUNC(evt_map_blend_set_flag);
USER_FUNC(evt_map_blend_off);
USER_FUNC(evt_map_set_blend);
USER_FUNC(evt_map_fog_onoff);
USER_FUNC(evt_map_get_fog);
USER_FUNC(evt_map_set_fog);
USER_FUNC(evt_mapobj_scale);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_trans);

USER_FUNC(evt_mapobj_trans) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    if (type == 0) {
        mapObjTranslate(name, x, y, z);
    } else {
        mapObjTranslate(name, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_rotate) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    if (type == 0) {
        mapObjRotate(name, x, y, z);
    } else {
        mapObjRotate(name, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_scale) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    if (type == 0) {
        mapObjScale(name, x, y, z);
    } else {
        mapObjScale(name, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_set_fog) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 start = evtGetFloat(event, args[1]);
    f32 end = evtGetFloat(event, args[2]);
    s32 r = evtGetValue(event, args[3]);
    s32 g = evtGetValue(event, args[4]);
    s32 b = evtGetValue(event, args[5]);
    mapSetFog(type, start, end, (GXColor){r, g, b, 255});
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_get_fog) {
    EvtWord* args = event->args;
    s32 type;
    f32 start;
    f32 end;
    GXColor color;
    mapGetFog(&type, &start, &end, &color);
    evtSetValue(event, args[0], type);
    evtSetFloat(event, args[1], start);
    evtSetFloat(event, args[2], end);
    evtSetValue(event, args[3], color.r);
    evtSetValue(event, args[4], color.g);
    evtSetValue(event, args[5], color.b);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_fog_onoff) {
    if (evtGetValue(event, event->args[0])) {
        mapFogOn();
    } else {
        mapFogOff();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_set_blend) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 r = evtGetValue(event, args[1]);
    s32 g = evtGetValue(event, args[2]);
    s32 b = evtGetValue(event, args[3]);
    s32 a = evtGetValue(event, args[4]);
    if (type == 0) {
        mapSetBlend((GXColor){r, g, b, a});
    } else {
        mapSetBlend2((GXColor){r, g, b, a});
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_blend_off) {
    if (evtGetValue(event, event->args[0]) == 0) {
        mapBlendOff();
    } else {
        mapBlendOff2();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_blend_set_flag) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    u32 flags = evtGetValue(event, args[2]);
    if (strcmp(name, "mario") == 0) {
        MarioWork* mp = marioGetPtr();
        s32* poses = &mp->paperAnimGroupId[0];
        if (on == 1) {
            if (poses[0] != -1) animPoseSetMaterialFlagOn(poses[0], flags);
            if (poses[1] != -1) animPoseSetMaterialFlagOn(poses[1], flags);
            if (poses[2] != -1) animPoseSetMaterialFlagOn(poses[2], flags);
        } else {
            if (poses[0] != -1) animPoseSetMaterialFlagOff(poses[0], flags);
            if (poses[1] != -1) animPoseSetMaterialFlagOff(poses[1], flags);
            if (poses[2] != -1) animPoseSetMaterialFlagOff(poses[2], flags);
        }
    } else if (strcmp(name, "party") == 0) {
        PartyEntry* party = partyGetPtr(marioGetPartyId());
        if (on == 1) {
            animPoseSetMaterialFlagOn(party->paperAnimGroup, flags);
        } else {
            animPoseSetMaterialFlagOff(party->paperAnimGroup, flags);
        }
    } else if (strcmp(name, "extparty") == 0) {
        PartyEntry* party = partyGetPtr(marioGetExtraPartyId());
        if (on == 1) {
            animPoseSetMaterialFlagOn(party->paperAnimGroup, flags);
        } else {
            animPoseSetMaterialFlagOff(party->paperAnimGroup, flags);
        }
    } else {
        NpcEntry* npc = evtNpcNameToPtr(event, name);
        if (on == 1) {
            animPoseSetMaterialFlagOn(npc->poseId, flags);
        } else {
            animPoseSetMaterialFlagOff(npc->poseId, flags);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_blend_set_mobj_flag) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    u32 flags = evtGetValue(event, args[2]);
    MapObjectEntry* mobj = mobjNameToPtr(name);
    if (on == 1) {
        animPoseSetMaterialFlagOn(mobj->poseId, flags);
    } else {
        animPoseSetMaterialFlagOff(mobj->poseId, flags);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_color) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    u8 r = evtGetValue(event, args[2]);
    u8 g = evtGetValue(event, args[3]);
    u8 b = evtGetValue(event, args[4]);
    u8 a = evtGetValue(event, args[5]);
    if (type == 0) {
        mapObjSetColor(name, (GXColor){r, g, b, a});
    } else {
        mapGrpSetColor(name, (GXColor){r, g, b, a});
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_playanim) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 a = evtGetValue(event, args[1]);
    s32 b = evtGetValue(event, args[2]);
    mapPlayAnimationLv(name, a, b);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_checkanim) {
    EvtWord* args = event->args;
    BOOL valid;
    f32 f;
    mapCheckAnimation((const char*)evtGetValue(event, args[0]), &valid, &f);
    evtSetValue(event, args[1], valid);
    evtSetValue(event, args[2], (s32)f);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_pauseanim) {
    EvtWord* args = event->args;
    s32 all = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    if (all) {
        mapPauseAnimationAll(name);
    } else {
        mapPauseAnimation(name);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_replayanim) {
    EvtWord* args = event->args;
    s32 all = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    if (all) {
        mapReStartAnimationAll(name);
    } else {
        mapReStartAnimation(name);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_set_playrate) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    mapSetPlayRate(name, evtGetFloat(event, args[1]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_flag_onoff) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    const char* name = (const char*)evtGetValue(event, args[2]);
    u32 flags = args[3];
    if (type == 0) {
        if (on == 0) {
            mapObjFlagOff(name, flags);
        } else {
            mapObjFlagOn(name, flags);
        }
    } else {
        if (on == 0) {
            mapGrpFlagOff(name, flags);
        } else {
            mapGrpFlagOn(name, flags);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_set_offscreen) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    s32 val = evtGetValue(event, args[2]);
    if (type == 0) {
        mapObjSetOffScreen(name, val);
    } else {
        mapGrpSetOffScreen(name, val);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_clear_offscreen) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    if (type == 0) {
        mapObjClearOffScreen(name);
    } else {
        mapGrpClearOffScreen(name);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mapobj_get_position) {
    EvtWord* args = event->args;
    Vec pos;
    mapObjGetPos((const char*)evtGetValue(event, args[0]), &pos);
    evtSetValue(event, args[1], FLOAT(pos.x));
    evtSetValue(event, args[2], FLOAT(pos.y));
    evtSetValue(event, args[3], FLOAT(pos.z));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_set_tevcallback) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    MapTevCallback cb = (MapTevCallback)evtGetValue(event, args[1]);
    mapSetTevCallback(index, cb);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_set_flush_onoff) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    const char* name = (const char*)evtGetValue(event, args[2]);
    if (type == 0) {
        if (on) {
            mapObjFlushOn(name);
        } else {
            mapObjFlushOff(name);
        }
    } else {
        if (on) {
            mapGrpFlushOn(name);
        } else {
            mapGrpFlushOff(name);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_set_flush_color) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    s32 r = evtGetValue(event, args[2]);
    s32 g = evtGetValue(event, args[3]);
    s32 b = evtGetValue(event, args[4]);
    s32 a = evtGetValue(event, args[5]);
    if (type == 0) {
        mapObjSetFlushColor(name, r, g, b, a);
    } else {
        mapGrpSetFlushColor(name, r, g, b, a);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_get_flush_color) {
    EvtWord* args = event->args;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    mapObjGetFlushColor((const char*)evtGetValue(event, args[0]), &r, &g, &b, &a);
    evtSetValue(event, args[1], r);
    evtSetValue(event, args[2], g);
    evtSetValue(event, args[3], b);
    evtSetValue(event, args[4], a);
    return EVT_RETURN_DONE;
}

static USER_FUNC(check) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    evtSetValue(event, args[0], 0);
    if (mp->hitobjStandOn) {
        if (strcmp((const char*)event->lwData[1], hitGetName(mp->hitobjStandOn)) == 0) {
            evtSetValue(event, args[0], 1);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(check2) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    evtSetValue(event, args[0], 0);
    if (mp->hitobjStandOn) {
        if (strcmp((const char*)event->lwData[1], hitGetName(mp->hitobjStandOn)) != 0) {
            evtSetValue(event, args[0], 1);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_entry_airport_harbor) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    const char* hit = (const char*)evtGetValue(event, args[2]);
    EvtWord lw[16];
    mapGetMapObj(name);
    switch (type) {
    case 0:
        if (!animGroupBaseAsync("p_hikohki", 0, NULL)) return EVT_RETURN_BLOCK;
        if ((s32)pouchCheckItem(4) == 1) {
            lw[0] = (EvtWord)name;
            lw[1] = (EvtWord)hit;
            evtRunCaseEntry(0, 0, hit, 0, evt_ride_airport, lw);
            lw[0] = (EvtWord)name;
            lw[1] = (EvtWord)hit;
            evtRunCaseEntry(1, 0, hit, 0, evt_nonride_airport, lw);
        }
        break;
    case 1:
        if (!animGroupBaseAsync("p_hune", 0, NULL)) return EVT_RETURN_BLOCK;
        if ((s32)pouchCheckItem(5) == 1) {
            lw[0] = (EvtWord)name;
            lw[1] = (EvtWord)hit;
            evtRunCaseEntry(0, 0, hit, 0, evt_ride_airport, lw);
            lw[0] = (EvtWord)name;
            lw[1] = (EvtWord)hit;
            evtRunCaseEntry(1, 0, hit, 0, evt_nonride_airport, lw);
        }
        break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_map_replace_mapobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    Vec pos;
    mapObjGetPos(name, &pos);
    mapObjFlagOn(name, 0x4000);
    switch (type) {
    case 0:
        effTorchEntry(0, pos.x, pos.y - 3.0f, pos.z, 0.6f);
        break;
    case 1: {
        EffectEntry* eff = effFireEntry(5, 0, pos.x, pos.y, pos.z, 0.4f);
        ((EffFireWork*)eff->userdata)->useMapBlend = 1;
        break;
    }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(N_evt_mapdisp_onoff) {
    if (evtGetValue(event, event->args[0])) {
        N_mapDispOn();
    } else {
        N_mapDispOff();
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_ride_airport)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), 256)
    LABEL(0)
    CALL(evt_map_get_flush_color, LW(0), LW(10), LW(11), LW(12), LW(13))
    ADD(LW(13), 2)
    IF_INT_GREATER(LW(13), 100)
        SET_VALUE(LW(13), 100)
    END_IF
    CALL(evt_map_set_flush_color, 1, LW(0), 240, 240, 100, LW(13))
    CALL(evt_map_set_flush_onoff, 1, 1, LW(0))
    CALL(check, LW(10))
    IF_INT_EQUAL(LW(10), 1)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_nonride_airport)
    LABEL(0)
    CALL(evt_map_get_flush_color, LW(0), LW(10), LW(11), LW(12), LW(13))
    SUBTRACT(LW(13), 2)
    IF_INT_LESS(LW(13), 0)
        SET_VALUE(LW(13), 0)
    END_IF
    CALL(evt_map_set_flush_color, 1, LW(0), 240, 240, 100, LW(13))
    CALL(check2, LW(10))
    IF_INT_EQUAL(LW(10), 1)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()
