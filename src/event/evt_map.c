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
    extern s32 evtGetValue(void*, s32);
    extern void mapObjSetColor(s32, u8*);
    extern void mapGrpSetColor(s32, u8*);
    s32* args;
    s32 group;
    s32 id;
    u8 color[4];

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    color[0] = evtGetValue(pEvt, args[2]);
    color[1] = evtGetValue(pEvt, args[3]);
    color[2] = evtGetValue(pEvt, args[4]);
    color[3] = evtGetValue(pEvt, args[5]);
    if (group == 0) {
        mapObjSetColor(id, color);
    } else {
        mapGrpSetColor(id, color);
    }
    return 2;
}

s32 evt_map_set_fog(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void mapSetFog(s32, f32, f32, unsigned char*);
    s32* args;
    s32 mode;
    f32 start;
    f32 end;
    unsigned char color[4];

    args = *(s32**)((s32)pEvt + 0x18);
    mode = evtGetValue(pEvt, args[0]);
    start = evtGetFloat(pEvt, args[1]);
    end = evtGetFloat(pEvt, args[2]);
    color[0] = evtGetValue(pEvt, args[3]);
    color[1] = evtGetValue(pEvt, args[4]);
    color[2] = evtGetValue(pEvt, args[5]);
    color[3] = 0;
    mapSetFog(mode, start, end, color);
    return 2;
}

s32 evt_map_replace_mapobj(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void mapObjGetPos(s32, f32*);
    extern void mapObjFlagOn(s32, u32);
    extern void effTorchEntry(s32, f32, f32, f32, f32);
    extern void* effFireEntry(s32, s32, f32, f32, f32, f32);
    s32* args;
    s32 id;
    s32 kind;
    f32 pos[3];
    void* eff;

    args = *(s32**)((s32)pEvt + 0x18);
    id = evtGetValue(pEvt, args[0]);
    kind = evtGetValue(pEvt, args[1]);
    mapObjGetPos(id, pos);
    mapObjFlagOn(id, 0x4000);
    switch (kind) {
        case 0:
            effTorchEntry(0, pos[0], pos[1] - 3.0f, pos[2], 0.6f);
            break;
        case 1:
            eff = effFireEntry(5, 0, pos[0], pos[1], pos[2], 0.4f);
            *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x88) = 1;
            break;
    }
    return 2;
}

s32 evt_map_set_blend(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void mapSetBlend(u8*);
    extern void mapSetBlend2(u8*);
    s32* args;
    s32 mode;
    u8 color[4];

    args = *(s32**)((s32)pEvt + 0x18);
    mode = evtGetValue(pEvt, args[0]);
    color[0] = evtGetValue(pEvt, args[1]);
    color[1] = evtGetValue(pEvt, args[2]);
    color[2] = evtGetValue(pEvt, args[3]);
    color[3] = evtGetValue(pEvt, args[4]);
    if (mode == 0) {
        mapSetBlend(color);
    } else {
        mapSetBlend2(color);
    }
    return 2;
}

s32 evt_mapobj_scale(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void mapObjScale(s32, f32, f32, f32);
    s32* args;
    s32 group;
    s32 id;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    y = evtGetFloat(pEvt, args[3]);
    z = evtGetFloat(pEvt, args[4]);
    if (group == 0) {
        mapObjScale(id, x, y, z);
    } else {
        mapObjScale(id, x, y, z);
    }
    return 2;
}

s32 evt_mapobj_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void mapObjRotate(s32, f32, f32, f32);
    s32* args;
    s32 group;
    s32 id;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    y = evtGetFloat(pEvt, args[3]);
    z = evtGetFloat(pEvt, args[4]);
    if (group == 0) {
        mapObjRotate(id, x, y, z);
    } else {
        mapObjRotate(id, x, y, z);
    }
    return 2;
}

s32 evt_mapobj_trans(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void mapObjTranslate(s32, f32, f32, f32);
    s32* args;
    s32 group;
    s32 id;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    y = evtGetFloat(pEvt, args[3]);
    z = evtGetFloat(pEvt, args[4]);
    if (group == 0) {
        mapObjTranslate(id, x, y, z);
    } else {
        mapObjTranslate(id, x, y, z);
    }
    return 2;
}

s32 evt_mapobj_get_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern s32 evtSetValue(void*, s32, s32);
    extern void mapObjGetPos(s32, float*);
    s32* args;
    float pos[3];

    args = *(s32**)((s32)pEvt + 0x18);
    mapObjGetPos(evtGetValue(pEvt, args[0]), pos);
    evtSetValue(pEvt, args[1], (s32)(pos[0] * 1024.0f));
    evtSetValue(pEvt, args[2], (s32)(pos[1] * 1024.0f));
    evtSetValue(pEvt, args[3], (s32)(pos[2] * 1024.0f));
    return 2;
}

s32 evt_map_set_flush_color(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void mapObjSetFlushColor(s32, u8, u8, u8, u8);
    extern void mapGrpSetFlushColor(s32, u8, u8, u8, u8);
    s32* args;
    s32 group;
    s32 id;
    s32 r;
    s32 g;
    s32 b;
    s32 a;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    r = evtGetValue(pEvt, args[2]);
    g = evtGetValue(pEvt, args[3]);
    b = evtGetValue(pEvt, args[4]);
    a = evtGetValue(pEvt, args[5]);
    if (group == 0) {
        mapObjSetFlushColor(id, r, g, b, a);
    } else {
        mapGrpSetFlushColor(id, r, g, b, a);
    }
    return 2;
}

s32 evt_map_get_fog(void* pEvt) {
    extern s32 evtSetValue(void*, s32, s32);
    extern f32 evtSetFloat(void*, s32, f32);
    extern void mapGetFog(s32*, f32*, f32*, unsigned char*);
    s32* args;
    s32 mode;
    f32 start;
    f32 end;
    unsigned char color[4];

    args = *(s32**)((s32)pEvt + 0x18);
    mapGetFog(&mode, &start, &end, color);
    evtSetValue(pEvt, args[0], mode);
    evtSetFloat(pEvt, args[1], start);
    evtSetFloat(pEvt, args[2], end);
    evtSetValue(pEvt, args[3], color[0]);
    evtSetValue(pEvt, args[4], color[1]);
    evtSetValue(pEvt, args[5], color[2]);
    return 2;
}

s32 evt_mapobj_flag_onoff(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void mapObjFlagOff(s32, s32);
    extern void mapObjFlagOn(s32, s32);
    extern void mapGrpFlagOff(s32, s32);
    extern void mapGrpFlagOn(s32, s32);
    s32* args;
    s32 group;
    s32 onoff;
    s32 id;
    s32 flag;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    onoff = evtGetValue(pEvt, args[1]);
    id = evtGetValue(pEvt, args[2]);
    flag = args[3];
    if (group == 0) {
        if (onoff == 0) {
            mapObjFlagOff(id, flag);
        } else {
            mapObjFlagOn(id, flag);
        }
    } else {
        if (onoff == 0) {
            mapGrpFlagOff(id, flag);
        } else {
            mapGrpFlagOn(id, flag);
        }
    }
    return 2;
}

s32 evt_map_set_flush_onoff(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void mapObjFlushOn(s32);
    extern void mapObjFlushOff(s32);
    extern void mapGrpFlushOn(s32);
    extern void mapGrpFlushOff(s32);
    s32* args;
    s32 group;
    s32 id;
    s32 onoff;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    onoff = evtGetValue(pEvt, args[2]);
    if (group == 0) {
        if (onoff != 0) {
            mapObjFlushOn(id);
        } else {
            mapObjFlushOff(id);
        }
    } else {
        if (onoff != 0) {
            mapGrpFlushOn(id);
        } else {
            mapGrpFlushOff(id);
        }
    }
    return 2;
}

s32 evt_map_blend_set_mobj_flag(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* mobjNameToPtr(char*);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void animPoseSetMaterialFlagOff(s32, u32);
    s32* args;
    s32 mode;
    char* name;
    u32 flag;
    void* mobj;

    args = *(s32**)((s32)pEvt + 0x18);
    mode = evtGetValue(pEvt, args[0]);
    name = (char*)evtGetValue(pEvt, args[1]);
    flag = evtGetValue(pEvt, args[2]);
    mobj = mobjNameToPtr(name);
    if (mode == 1) {
        animPoseSetMaterialFlagOn(*(s32*)((s32)mobj + 0x70), flag);
    } else {
        animPoseSetMaterialFlagOff(*(s32*)((s32)mobj + 0x70), flag);
    }
    return 2;
}

s32 evt_map_get_flush_color(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern s32 evtSetValue(void*, s32, s32);
    extern void mapObjGetFlushColor(s32, u8*, u8*, u8*, u8*);
    s32* args;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    args = *(s32**)((s32)pEvt + 0x18);
    mapObjGetFlushColor(evtGetValue(pEvt, args[0]), &r, &g, &b, &a);
    evtSetValue(pEvt, args[1], r);
    evtSetValue(pEvt, args[2], g);
    evtSetValue(pEvt, args[3], b);
    evtSetValue(pEvt, args[4], a);
    return 2;
}

s32 check2(void* pEvt) {
    extern void* marioGetPtr(void);
    extern char* hitGetName(void*);
    extern s32 strcmp(char*, char*);
    extern s32 evtSetValue(void*, s32, s32);
    s32* args;
    void* mario;
    void* hit;

    args = *(s32**)((s32)pEvt + 0x18);
    mario = marioGetPtr();
    evtSetValue(pEvt, args[0], 0);
    hit = *(void**)((s32)mario + 0x1E8);
    if (hit != 0 && strcmp(*(char**)((s32)pEvt + 0xA0), hitGetName(hit)) != 0) {
        evtSetValue(pEvt, args[0], 1);
    }
    return 2;
}

s32 check(void* pEvt) {
    extern void* marioGetPtr(void);
    extern char* hitGetName(void*);
    extern s32 strcmp(char*, char*);
    extern s32 evtSetValue(void*, s32, s32);
    s32* args;
    void* mario;
    void* hit;

    args = *(s32**)((s32)pEvt + 0x18);
    mario = marioGetPtr();
    evtSetValue(pEvt, args[0], 0);
    hit = *(void**)((s32)mario + 0x1E8);
    if (hit != 0 && strcmp(*(char**)((s32)pEvt + 0xA0), hitGetName(hit)) == 0) {
        evtSetValue(pEvt, args[0], 1);
    }
    return 2;
}

s32 evt_mapobj_set_offscreen(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void mapObjSetOffScreen(s32, s32);
    extern void mapGrpSetOffScreen(s32, s32);
    s32* args;
    s32 group;
    s32 id;
    s32 onoff;

    args = *(s32**)((s32)pEvt + 0x18);
    group = evtGetValue(pEvt, args[0]);
    id = evtGetValue(pEvt, args[1]);
    onoff = evtGetValue(pEvt, args[2]);
    if (group == 0) {
        mapObjSetOffScreen(id, onoff);
    } else {
        mapGrpSetOffScreen(id, onoff);
    }
    return 2;
}

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
