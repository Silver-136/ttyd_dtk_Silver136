#include "event/evt_map.h"
#include "event/evt_cmd.h"

s32 evtGetValue(EventEntry* event, s32 value);
f32 evtGetFloat(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void N_mapDispOn(void);
void N_mapDispOff(void);
void mapBlendOff(void);
void mapBlendOff2(void);
void mapFogOn(void);
void mapFogOff(void);
void mapSetPlayRate(s32 id, f32 rate);
void mapSetTevCallback(s32 id, s32 callback);
void mapObjClearOffScreen(s32 id);
void mapGrpClearOffScreen(s32 id);
void mapCheckAnimation(s32 id, s32* outDone, f32* outFrame);
void mapReStartAnimation(s32 id);
void mapReStartAnimationAll(void);
void mapPauseAnimation(s32 id);
void mapPauseAnimationAll(void);
void mapPlayAnimationLv(s32 id, s32 anim, s32 level);

USER_FUNC(N_evt_mapdisp_onoff) {
    if (evtGetValue(event, event->args[0]) != 0) {
        N_mapDispOn();
    } else {
        N_mapDispOff();
    }
    return 2;
}

USER_FUNC(evt_map_blend_off) {
    if (evtGetValue(event, event->args[0]) == 0) {
        mapBlendOff();
    } else {
        mapBlendOff2();
    }
    return 2;
}

USER_FUNC(evt_map_fog_onoff) {
    if (evtGetValue(event, event->args[0]) != 0) {
        mapFogOn();
    } else {
        mapFogOff();
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_map_set_playrate) {
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    f32 rate = evtGetFloat(event, args[1]);
    mapSetPlayRate(id, rate);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_map_set_tevcallback) {
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 callback = evtGetValue(event, args[1]);
    mapSetTevCallback(id, callback);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_mapobj_clear_offscreen) {
    s32* args = event->args;
    s32 group = evtGetValue(event, args[0]);
    s32 id = evtGetValue(event, args[1]);
    if (group == 0) {
        mapObjClearOffScreen(id);
    } else {
        mapGrpClearOffScreen(id);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_map_checkanim) {
    s32* args = event->args;
    s32 done;
    f32 frame;
    s32 id = evtGetValue(event, args[0]);
    mapCheckAnimation(id, &done, &frame);
    evtSetValue(event, args[1], done);
    evtSetValue(event, args[2], (s32)frame);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_map_blend_set_flag(void* pEvt) {
    extern s32 strcmp(char* lhs, char* rhs);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* evtNpcNameToPtr(EventEntry* event, char* name);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern char str_mario_8041fa4c[6];
    extern char str_party_8041fa54[6];
    extern char str_extparty_802c019c[];

    s32* args = ((EventEntry*)pEvt)->args;
    s32 mode = evtGetValue((EventEntry*)pEvt, args[0]);
    char* name = (char*)evtGetValue((EventEntry*)pEvt, args[1]);
    u32 flag = evtGetValue((EventEntry*)pEvt, args[2]);

    if (strcmp(name, str_mario_8041fa4c) == 0) {
        void* mario = marioGetPtr();
        if (mode == 1) {
            if (*(s32*)((s32)mario + 0x22C) != -1) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x22C), flag);
            }
            if (*(s32*)((s32)mario + 0x230) != -1) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x230), flag);
            }
            if (*(s32*)((s32)mario + 0x234) != -1) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x234), flag);
            }
        } else {
            if (*(s32*)((s32)mario + 0x22C) != -1) {
                animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x22C), flag);
            }
            if (*(s32*)((s32)mario + 0x230) != -1) {
                animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x230), flag);
            }
            if (*(s32*)((s32)mario + 0x234) != -1) {
                animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x234), flag);
            }
        }
    } else if (strcmp(name, str_party_8041fa54) == 0) {
        void* party = partyGetPtr(marioGetPartyId());
        if (mode == 1) {
            animPoseSetMaterialFlagOn(*(s32*)((s32)party + 0xC), flag);
        } else {
            animPoseSetMaterialFlagOff(*(s32*)((s32)party + 0xC), flag);
        }
    } else if (strcmp(name, str_extparty_802c019c) == 0) {
        void* party = partyGetPtr(marioGetExtraPartyId());
        if (mode == 1) {
            animPoseSetMaterialFlagOn(*(s32*)((s32)party + 0xC), flag);
        } else {
            animPoseSetMaterialFlagOff(*(s32*)((s32)party + 0xC), flag);
        }
    } else {
        void* npc = evtNpcNameToPtr((EventEntry*)pEvt, name);
        if (mode == 1) {
            animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), flag);
        } else {
            animPoseSetMaterialFlagOff(*(s32*)((s32)npc + 0x104), flag);
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_map_entry_airport_harbor(void* pEvt) {
    extern void* mapGetMapObj(char* name);
    extern s32 animGroupBaseAsync(char* name, s32 mode, s32 flags);
    extern s32 pouchCheckItem(s32 itemId);
    extern void evtRunCaseEntry(s32 type, s32 flag, s32 caseId, s32 unk, s32* script, void* data);
    extern char str_p_hikohki_802c0188[];
    extern char str_p_hune_802c0194[];
    extern s32 evt_ride_airport[];
    extern s32 evt_nonride_airport[];

    s32* args = ((EventEntry*)pEvt)->args;
    s32 mode = evtGetValue((EventEntry*)pEvt, args[0]);
    char* name = (char*)evtGetValue((EventEntry*)pEvt, args[1]);
    s32 id = evtGetValue((EventEntry*)pEvt, args[2]);
    struct {
        char* name;
        s32 id;
        u8 pad[0x40];
    } local;

    mapGetMapObj(name);

    switch (mode) {
        case 0:
            if (animGroupBaseAsync(str_p_hikohki_802c0188, 0, 0) == 0) {
                return 0;
            }
            if (pouchCheckItem(4) == 1) {
                local.name = name;
                local.id = id;
                evtRunCaseEntry(0, 0, id, 0, evt_ride_airport, &local);
                local.name = name;
                local.id = id;
                evtRunCaseEntry(1, 0, id, 0, evt_nonride_airport, &local);
            }
            break;
        case 1:
            if (animGroupBaseAsync(str_p_hune_802c0194, 0, 0) == 0) {
                return 0;
            }
            if (pouchCheckItem(5) == 1) {
                local.name = name;
                local.id = id;
                evtRunCaseEntry(0, 0, id, 0, evt_ride_airport, &local);
                local.name = name;
                local.id = id;
                evtRunCaseEntry(1, 0, id, 0, evt_nonride_airport, &local);
            }
            break;
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_mapobj_color(void* pEvt) {
    return 0;
}


s32 evt_map_set_fog(int param_1) {
    return 0;
}


s32 evt_map_replace_mapobj(void* pEvt) {
    return 0;
}


s32 evt_map_set_blend(void* pEvt) {
    return 0;
}


s32 evt_mapobj_scale(void* pEvt) {
    return 0;
}


s32 evt_mapobj_rotate(void* pEvt) {
    return 0;
}


s32 evt_mapobj_trans(void* pEvt) {
    return 0;
}


s32 evt_mapobj_get_position(int param_1) {
    return 0;
}


s32 evt_map_set_flush_color(int param_1) {
    return 0;
}


s32 evt_map_get_fog(int param_1) {
    return 0;
}


s32 evt_mapobj_flag_onoff(void* pEvt) {
    return 0;
}


s32 evt_map_set_flush_onoff(int param_1) {
    return 0;
}


s32 evt_map_blend_set_mobj_flag(int param_1) {
    return 0;
}


s32 evt_map_get_flush_color(int param_1) {
    return 0;
}


s32 check2(void* pEvt) {
    return 0;
}


s32 check(void* pEvt) {
    return 0;
}


s32 evt_mapobj_set_offscreen(void* pEvt) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_map_playanim(int param_1) {
    s32* args;
    EventEntry* event;
    s32 id;
    s32 anim;
    s32 level;

    event = (EventEntry*)param_1;
    args = event->args;
    id = evtGetValue(event, args[0]);
    anim = evtGetValue(event, args[1]);
    level = evtGetValue(event, args[2]);
    mapPlayAnimationLv(id, anim, level);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_map_replayanim(int param_1) {
    EventEntry* event;
    s32* args;
    s32 all;
    s32 id;

    event = (EventEntry*)param_1;
    args = event->args;
    all = evtGetValue(event, args[0]);
    id = evtGetValue(event, args[1]);
    if (all != 0) {
        mapReStartAnimationAll();
    } else {
        mapReStartAnimation(id);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_map_pauseanim(int param_1) {
    EventEntry* event;
    s32* args;
    s32 all;
    s32 id;

    event = (EventEntry*)param_1;
    args = event->args;
    all = evtGetValue(event, args[0]);
    id = evtGetValue(event, args[1]);
    if (all != 0) {
        mapPauseAnimationAll();
    } else {
        mapPauseAnimation(id);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
