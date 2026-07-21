#include "event/evt_mobj.h"

s32 evtGetValue(void* evt, s32 value);
s32 evtSetValue(void* evt, s32 var, s32 value);
s32 evtSetFloat(void* evt, s32 var, f32 value);
void mobjDelete(s32 name);
void* mobjNameToPtr(s32 name);
void* mobjNameToPtrNoAssert(s32 name);
void* marioGetPtr(void);
s32 kpaGetLevel(void);
void* _mapAlloc(void* heap, u32 size);
void PSMTXTrans(f32* mtx, f32 x, f32 y, f32 z);
void PSMTXScale(f32* mtx, f32 x, f32 y, f32 z);
void PSMTXConcat(f32* a, f32* b, f32* out);
void FontDrawStart(void);
void FontDrawColor(void* color);
void FontDrawStringMtx(f32* mtx, void* string);
f32 animPoseGetLoopTimes(void* pose);
s32 strcmp(const char* a, const char* b);
s32 mobj_lock(void* pMobj);

extern void* mapalloc_base_ptr;
void* psw;
extern f32 float_0_80420240;
extern f32 float_300_80420248;
extern f32 float_0p5_8042024c;
extern f32 float_1_80420250;
extern u32 dat_80420224;
extern const char str_me_80420324[3];
s32 evt_mobj_entry(void* evt) {
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern s32* gp;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    const char* kind = (const char*)evtGetValue(evt, args[1]);

    if (animGroupBaseAsync(kind, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }
    mobjEntry(name, kind);
    return 2;
}

s32 evt_mobj_delete(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    mobjDelete(evtGetValue(evt, args[0]));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_check(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[1], (s32)mobjNameToPtrNoAssert(evtGetValue(evt, args[0])));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_flag_onoff(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 on = evtGetValue(evt, args[0]);
    s32 name = evtGetValue(evt, args[1]);
    u32 flag = args[2];
    void* mobj = mobjNameToPtr(name);

    if (on != 0) {
        *(u32*)mobj |= flag;
    } else {
        *(u32*)mobj = *(u32*)mobj & ~flag;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_get_kindname(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    void* mobj = mobjNameToPtr(evtGetValue(evt, args[0]));
    evtSetValue(evt, args[1], (s32)((s32)mobj + 0x15));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_scale(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern const u32 vec3_802c2058[3];
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    f32 x;
    f32 y;
    f32 z;
    u32 scale[3];

    if (strcmp((const char*)name, str_me_80420324) == 0) {
        mobj = *(void**)((s32)evt + 0x174);
    } else {
        mobj = mobjNameToPtr(name);
    }
    x = evtGetFloat(evt, args[1]);
    y = evtGetFloat(evt, args[2]);
    z = evtGetFloat(evt, args[3]);
    scale[0] = vec3_802c2058[0];
    scale[1] = vec3_802c2058[1];
    *(f32*)&scale[0] = x;
    scale[2] = vec3_802c2058[2];
    *(f32*)&scale[1] = y;
    *(f32*)&scale[2] = z;
    *(u32*)((s32)mobj + 0x44) = scale[0];
    *(u32*)((s32)mobj + 0x48) = scale[1];
    *(u32*)((s32)mobj + 0x4C) = scale[2];
    *(u32*)mobj |= 4;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_get_position(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    evtSetFloat(evt, args[1], *(f32*)((s32)mobj + 0x38));
    evtSetFloat(evt, args[2], *(f32*)((s32)mobj + 0x3C));
    evtSetFloat(evt, args[3], *(f32*)((s32)mobj + 0x40));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_get_x_position(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    evtSetFloat(evt, args[1], *(f32*)((s32)mobj + 0x38));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_get_y_position(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    evtSetFloat(evt, args[1], *(f32*)((s32)mobj + 0x3C));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_get_z_position(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    evtSetFloat(evt, args[1], *(f32*)((s32)mobj + 0x40));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_position(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern const u32 vec3_802c2064[3];
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    void* mobj;
    u32 pos[3];

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    pos[0] = vec3_802c2064[0];
    pos[1] = vec3_802c2064[1];
    *(f32*)&pos[0] = x;
    pos[2] = vec3_802c2064[2];
    *(f32*)&pos[1] = y;
    *(f32*)&pos[2] = z;
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(u32*)mobj |= 4;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_x_position(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    u32 pos[3];

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    pos[0] = *(u32*)((s32)mobj + 0x38);
    pos[1] = *(u32*)((s32)mobj + 0x3C);
    pos[2] = *(u32*)((s32)mobj + 0x40);
    *(f32*)&pos[0] = evtGetFloat(evt, args[1]);
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(u32*)mobj |= 4;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_y_position(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    u32 pos[3];

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    pos[0] = *(u32*)((s32)mobj + 0x38);
    pos[1] = *(u32*)((s32)mobj + 0x3C);
    pos[2] = *(u32*)((s32)mobj + 0x40);
    *(f32*)&pos[1] = evtGetFloat(evt, args[1]);
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(u32*)mobj |= 4;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on




#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_z_position(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    u32 pos[3];

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    pos[0] = *(u32*)((s32)mobj + 0x38);
    pos[1] = *(u32*)((s32)mobj + 0x3C);
    pos[2] = *(u32*)((s32)mobj + 0x40);
    *(f32*)&pos[2] = evtGetFloat(evt, args[1]);
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(u32*)mobj |= 4;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_exec_cancel(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 strcmp(const char* s1, const char* s2);
    extern void* mobjNameToPtr(s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* mobj;

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)pEvt + 0x174);
    }
    *(u32*)mobj |= 0x1000;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_gravity_bound(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 gravity = evtGetFloat(evt, args[1]);
    f32 bound = evtGetFloat(evt, args[2]);
    void* mobj;

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    *(f32*)((s32)mobj + 0x68) = gravity;
    *(f32*)((s32)mobj + 0x6C) = bound;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_anim(void* evt) {
    extern void animPoseSetAnim(void* pose, s32 anim, s32 force);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 anim = evtGetValue(evt, args[1]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    animPoseSetAnim(*(void**)((s32)mobj + 0x70), anim, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_wait_animation_end(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    if (animPoseGetLoopTimes(*(void**)((s32)mobj + 0x70)) > float_1_80420250) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_set_camid(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    u8 value = evtGetValue(evt, args[1]);
    void* mobj;
    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    *(u8*)((s32)mobj + 4) = value;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_hitevt_onoff(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 on = evtGetValue(evt, args[1]);
    void* mobj;

    if (strcmp((const char*)name, str_me_80420324) != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = *(void**)((s32)evt + 0x174);
    }
    if (on != 0) {
        *(u32*)mobj &= ~0x10;
    } else {
        *(u32*)mobj |= 0x10;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_hit_onoff(void* evt) {
    extern void hitGrpFlagOff(void* hit, u32 flag);
    extern void hitGrpFlagOn(void* hit, u32 flag);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 on = evtGetValue(evt, args[1]);
    register void* hit = mobjNameToPtr(name);
    s32 i;

    if (on) {
        for (i = 0; i < 2; i++) {
            if ((s32)hit + 0x78 != 0) {
                hitGrpFlagOff((void*)((s32)hit + 0x78), 1);
            }
            hit = (void*)((s32)hit + 0x88);
        }
    } else {
        for (i = 0; i < 2; i++) {
            if ((s32)hit + 0x78 != 0) {
                hitGrpFlagOn((void*)((s32)hit + 0x78), 1);
            }
            hit = (void*)((s32)hit + 0x88);
        }
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 mobj_switch_blue(void* mobj) {
    extern void* mobjRunEvent(void* mobj, void* evtCode);
    extern void marioChgMot(s32 motionId);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void* effSmallStarEntry(s32 flags, s32 type, f32 x, f32 y, f32 z, f32 a, f32 b, f32 c);
    extern void effSmallStarCamChg(void* eff, u8 camId);
    extern s32 evtSetValue(void* evt, s32 var, s32 value);
    extern void mobjDelete(char* name);
    extern const char str_A_1_80420208[];
    extern const char str_S_1_8042021c[];
    extern f32 float_1_80420250;
    extern f32 float_0_80420240;
    extern f32 float_12p5_80420268;
    extern f32 float_neg1_80420304;
    s32 action = *(s32*)((s32)mobj + 0x1DC);
    void* eff;

    switch (action) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                if ((*(u32*)((s32)mobj + 0x1E0) == 0x200000) || (*(u32*)((s32)mobj + 0x1E0) == 0x1000000)) {
                    marioChgMot(5);
                }
                psndSFXOn_3D(0x1D2, (void*)((s32)mobj + 0x38));
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(s32*)((s32)mobj + 0x1DC) = *(s32*)((s32)mobj + 0x1DC) + 1;
            }
            break;
        case 1:
            if (animPoseGetLoopTimes(*(s32*)((s32)mobj + 0x70)) >= float_1_80420250) {
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                eff = effSmallStarEntry(0, 4, *(f32*)((s32)mobj + 0x38), *(f32*)((s32)mobj + 0x3C) + float_12p5_80420268,
                                        *(f32*)((s32)mobj + 0x40), float_0_80420240, float_neg1_80420304, float_0_80420240);
                effSmallStarCamChg(eff, *(u8*)((s32)mobj + 4));
                evtSetValue(0, *(s32*)((s32)mobj + 0x1E4), 1);
                *(s32*)((s32)mobj + 0x1DC) = 99;
            }
            break;
        case 99:
            mobjDelete((char*)((s32)mobj + 5));
            break;
    }
    return 0;
}

s32 evt_mobj_switch_blue(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void* gp;
    extern char vec3_802c2058[];
    extern void mobj_switch_blue(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* base;
    s32 kind;
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    s32 swSet;
    char* poseName;
    s32 inBattle;
    void* mobj;

    base = vec3_802c2058;
    kind = evtGetValue(pEvt, args[0]);
    name = (char*)evtGetValue(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    y = evtGetFloat(pEvt, args[3]);
    z = evtGetFloat(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    swSet = args[6];
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (kind == 1) {
        poseName = base + 0x9E8;
    } else if (kind >= 0 && kind < 1) {
        poseName = base + 0x9D8;
    } else {
        poseName = base + 0x9FC;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_switch_blue;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 0x44;
    *(u32*)mobj |= 0xA10000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (kind == 2) {
        *(u32*)mobj |= 2;
    }
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    mobjCalcMtx(mobj);
    return 2;
}

s32 evt_mobj_floatswitch_blue(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void* mobjNameToPtr(s32 name);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern void mobjCalcMtx(void* mobj);
    extern s32* gp;
    extern const char str_MOBJ_BlueSwitchBlock_802c2a18[];
    extern s32 mobj_switch_blue(void* mobj);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 unused = evtGetValue(evt, args[0]);
    s32 name = evtGetValue(evt, args[1]);
    f32 x = evtGetFloat(evt, args[2]);
    f32 y = evtGetFloat(evt, args[3]);
    f32 z = evtGetFloat(evt, args[4]);
    s32 eventCode = evtGetValue(evt, args[5]);
    s32 switchVar = args[6];
    void* mobj;

    if (animGroupBaseAsync(str_MOBJ_BlueSwitchBlock_802c2a18, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_BlueSwitchBlock_802c2a18);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_switch_blue;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x400000;
    *(s32*)((s32)mobj + 0x1E4) = switchVar;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue(evt, switchVar) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    mobjCalcMtx(mobj);
    return 2;
}

s32 evt_mobj_tornadoswitch_blue(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void* mobjNameToPtr(s32 name);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern void mobjCalcMtx(void* mobj);
    extern s32* gp;
    extern const char str_MOBJ_BlueTornadoSwit_802c2a00[];
    extern s32 mobj_switch_blue(void* mobj);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 unused = evtGetValue(evt, args[0]);
    s32 name = evtGetValue(evt, args[1]);
    f32 x = evtGetFloat(evt, args[2]);
    f32 y = evtGetFloat(evt, args[3]);
    f32 z = evtGetFloat(evt, args[4]);
    s32 eventCode = evtGetValue(evt, args[5]);
    s32 switchVar = args[6];
    void* mobj;

    if (animGroupBaseAsync(str_MOBJ_BlueTornadoSwit_802c2a00, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_BlueTornadoSwit_802c2a00);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_switch_blue;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x1000000;
    *(s32*)((s32)mobj + 0x1E4) = switchVar;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue(evt, switchVar) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    mobjCalcMtx(mobj);
    return 2;
}



s32 mobj_switch_red(void* pMobj) {
    extern void* mobjRunEvent(void* mobj, void* evtCode);
    extern void marioChgMot(s32 motionId);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern const char str_A_1_80420208[];
    extern const char str_S_1_8042021c[];
    extern f32 float_1_80420250;
    void* mobj = pMobj;
    s32 action = *(s32*)((s32)mobj + 0x1DC);
    u32 activation;

    switch (action) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                activation = *(u32*)((s32)mobj + 0x1E0);
                if (activation == 0x200000 || activation == 0x1000000) {
                    marioChgMot(5);
                }
                psndSFXOn_3D(0x1D2, (void*)((s32)mobj + 0x38));
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(u32*)mobj &= ~8;
            }
            break;
        case 1:
            if ((*(u32*)mobj & 8) != 0) {
                psndSFXOn_3D(0x1D2, (void*)((s32)mobj + 0x38));
                if (*(u32*)((s32)mobj + 0x1E0) == 0x200000) {
                    marioChgMot(5);
                }
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(u32*)mobj &= ~8;
            }
            if (animPoseGetLoopTimes(*(s32*)((s32)mobj + 0x70)) >= float_1_80420250) {
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                *(u32*)mobj &= ~8;
                *(s32*)((s32)mobj + 0x1DC) = 0;
            }
            break;
    }
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_switch_red(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void* gp;
    extern const char str_MOBJ_RedSwitch_802c29dc[];
    extern const char str_MOBJ_WhiteSwitch_802c29ec[];
    extern void mobj_switch_red(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 kind;
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    s32 swSet;
    const char* poseName;
    s32 inBattle;
    void* mobj;

    kind = evtGetValue(pEvt, args[0]);
    name = (char*)evtGetValue(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    y = evtGetFloat(pEvt, args[3]);
    z = evtGetFloat(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    swSet = args[6];
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if ((kind == 1) || (kind > 0) || (kind < 0)) {
        poseName = str_MOBJ_WhiteSwitch_802c29ec;
    } else {
        poseName = str_MOBJ_RedSwitch_802c29dc;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_switch_red;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 0x44;
    *(u32*)mobj |= 0xA10000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_mobj_floatswitch_red(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void* mobjNameToPtr(s32 name);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern void mobjCalcMtx(void* mobj);
    extern s32* gp;
    extern const char str_MOBJ_RedSwitchBlock_802c29c8[];
    extern s32 mobj_switch_red(void* mobj);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 unused = evtGetValue(evt, args[0]);
    s32 name = evtGetValue(evt, args[1]);
    f32 x = evtGetFloat(evt, args[2]);
    f32 y = evtGetFloat(evt, args[3]);
    f32 z = evtGetFloat(evt, args[4]);
    s32 eventCode = evtGetValue(evt, args[5]);
    s32 switchVar = args[6];
    void* mobj;

    if (animGroupBaseAsync(str_MOBJ_RedSwitchBlock_802c29c8, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_RedSwitchBlock_802c29c8);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_switch_red;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x400000;
    *(s32*)((s32)mobj + 0x1E4) = switchVar;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue(evt, switchVar) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    mobjCalcMtx(mobj);
    return 2;
}

s32 mobj_timerswitch(void* pMobj) {
    extern void* mobjNameToPtr(char*);
    extern void marioChgMot(s32);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void* evtEntryType(void*, s8, u32, u32);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern void* mobjRunEvent(void*, void*);
    extern s32 strcmp(const char*, const char*);
    extern void effSmallStarEntry(void);
    extern void evtSetValue(void*, s32, s32);
    extern void mobjDelete(char*);
    extern void* evt_shake;
    extern const char str_A_1_80420208[];
    extern const char str_A_2_8042020c[];
    extern const char str_S_1_8042021c[];
    extern const char str_MOBJ_RedTimerSwitch_802c2088[];
    extern f32 float_0p3_80420320;
    void* other = mobjNameToPtr((char*)((s32)pMobj + 0x1B8));
    u32 action = *(u32*)((s32)pMobj + 0x1DC);

    if (action == 0) {
        if ((*(u32*)pMobj & 8) != 0) {
            if (*(u32*)((s32)pMobj + 0x1E0) == 0x04000000) {
                marioChgMot(0x39);
            }
            if (*(s32*)((s32)other + 0x1DC) == 2) {
                psndSFXOn_3D(0x1C2, (void*)((s32)pMobj + 0x38));
            } else {
                psndSFXOn_3D(0x1C1, (void*)((s32)pMobj + 0x38));
            }
            evtEntryType(&evt_shake, 0, 0, 0);
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            *(u32*)pMobj &= ~8;
        }
    } else if (action == 1) {
        if ((*(u32*)pMobj & 8) != 0) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
            animPoseSetAnim(*(s32*)((s32)other + 0x70), str_S_1_8042021c, 1);
            *(s32*)((s32)pMobj + 0x1DC) = 0;
        } else if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_0p3_80420320) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_2_8042020c, 1);
            animPoseSetAnim(*(s32*)((s32)other + 0x70), str_A_2_8042020c, 1);
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            *(s32*)((s32)pMobj + 0x1B0) = *(s32*)((s32)pMobj + 0x1B4);
            *(u32*)pMobj &= ~8;
        }
    } else if (action == 2) {
        if ((*(u32*)pMobj & 8) != 0) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
            animPoseSetAnim(*(s32*)((s32)other + 0x70), str_S_1_8042021c, 1);
            *(s32*)((s32)pMobj + 0x1DC) = 0;
        } else {
            *(s32*)((s32)pMobj + 0x1B0) -= 1;
            if (*(s32*)((s32)pMobj + 0x1B0) < 1) {
                *(u32*)pMobj &= ~8;
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
                animPoseSetAnim(*(s32*)((s32)other + 0x70), str_S_1_8042021c, 1);
                *(s32*)((s32)pMobj + 0x1DC) = 0;
            } else if (*(s32*)((s32)other + 0x1DC) == 2) {
                if ((marioKeyOffChk() == 0) && (marioCtrlOffChk() == 0)) {
                    mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
                    if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_RedTimerSwitch_802c2088) == 0) {
                        *(u32*)pMobj &= ~8;
                        *(u32*)other &= ~8;
                        animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
                        animPoseSetAnim(*(s32*)((s32)other + 0x70), str_S_1_8042021c, 1);
                        *(s32*)((s32)pMobj + 0x1DC) = 0;
                        *(s32*)((s32)other + 0x1DC) = 0;
                    } else {
                        effSmallStarEntry();
                        effSmallStarEntry();
                        evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
                        evtSetValue(0, *(s32*)((s32)other + 0x1E4), 1);
                        mobjDelete((char*)((s32)pMobj + 5));
                        mobjDelete((char*)((s32)other + 5));
                    }
                } else {
                    *(s32*)((s32)pMobj + 0x1DC) = 0;
                }
            }
        }
    }
    return 0;
}
s32 evt_mobj_timerswitch(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern char* strcpy(char* dst, const char* src);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void* gp;
    extern const char* dat_ptrarr_802c209c[2];
    extern void mobj_timerswitch(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 kind;
    const char* poseNames[2];
    const char* poseName;
    s32 inBattle;
    char* names[2];
    f32 xs[2];
    f32 ys[2];
    f32 zs[2];
    void* mobjs[2];
    s32 duration;
    void* evtCode;
    s32 swSet;
    s32 i;
    char* copyName;
    void* mobj;

    kind = evtGetValue(pEvt, args[0]);
    poseNames[0] = dat_ptrarr_802c209c[0];
    poseNames[1] = dat_ptrarr_802c209c[1];
    inBattle = *(s32*)((s32)gp + 0x14);
    poseName = poseNames[kind];
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    names[0] = (char*)evtGetValue(pEvt, args[1]);
    xs[0] = evtGetFloat(pEvt, args[2]);
    ys[0] = evtGetFloat(pEvt, args[3]);
    zs[0] = evtGetFloat(pEvt, args[4]);
    names[1] = (char*)evtGetValue(pEvt, args[5]);
    xs[1] = evtGetFloat(pEvt, args[6]);
    ys[1] = evtGetFloat(pEvt, args[7]);
    zs[1] = evtGetFloat(pEvt, args[8]);
    duration = evtGetValue(pEvt, args[9]);
    evtCode = (void*)evtGetValue(pEvt, args[10]);
    swSet = args[11];
    if (kind == 0 && evtGetValue(pEvt, swSet) != 0) {
        return 2;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    for (i = 0; i < 2; i++) {
        mobjEntry(names[i], poseName);
        mobj = mobjNameToPtr(names[i]);
        mobjs[i] = mobj;
        copyName = names[0];
        if (i == 0) {
            copyName = names[1];
        }
        strcpy((char*)((s32)mobj + 0x1B8), copyName);
        mobjSetPosition(xs[i], ys[i], zs[i], names[i]);
        *(void**)((s32)mobj + 0x1D0) = mobj_timerswitch;
        *(void**)((s32)mobj + 0x1D4) = evtCode;
        *(u32*)mobj |= 0x44;
        *(u32*)mobj |= 0xA10000;
        *(s32*)((s32)mobj + 0x1E4) = swSet;
        *(s32*)((s32)mobj + 0x1DC) = 0;
        *(s32*)((s32)mobj + 0x1B4) = duration;
        if (evtGetValue(pEvt, swSet) != 0) {
            *(s32*)((s32)mobj + 0x1DC) = 99;
        }
        mobjCalcMtx(mobj);
    }
    return 2;
}

s32 mobj_rideswitch_lightblue(void* pMobj) {
    extern void* mobjRunEvent(void* mobj, void* evtCode);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void mobjCalcMtx2(void* mobj);
    extern void* animPoseGetAnimBaseDataPtr(s32 poseId);
    extern f32 PSVECMag(void* vec);
    extern void* marioGetPtr(void);
    extern f32 PSVECDistance(void* a, void* b);
    extern const char str_A_1_80420208[];
    extern const char str_A_2_8042020c[];
    extern f32 float_0p2_8042028c;
    extern f32 float_0p8_804202a0;
    extern f32 float_0p9_80420318;
    extern f32 float_10_80420294;
    extern f32 float_15_8042031c;
    extern const u32 vec3_802c2058[3];
    extern const u32 vec3_802c20d4[3];
    extern const u32 vec3_802c20ec[3];
    void* mobj = pMobj;
    s32 action = *(s32*)((s32)mobj + 0x1DC);
    f32 sx;
    f32 sy;
    f32 sz;
    f32 radius;
    void* anim;
    void* mario;
    u32 vec[3];

    switch (action) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                psndSFXOn_3D(0x1D3, (void*)((s32)mobj + 0x38));
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(s32*)((s32)mobj + 0x1DC) = 1;
            }
            break;
        case 1:
            sy = *(f32*)((s32)mobj + 0x188) - float_0p9_80420318;
            sx = *(f32*)((s32)mobj + 0x184);
            sz = *(f32*)((s32)mobj + 0x18C);
            if (sy <= float_0p2_8042028c) {
                *(s32*)((s32)mobj + 0x1DC) = 2;
                *(u32*)mobj &= ~8;
                sy = float_0p2_8042028c;
            }
            vec[0] = vec3_802c20d4[0];
            vec[1] = vec3_802c20d4[1];
            vec[2] = vec3_802c20d4[2];
            *(f32*)&vec[0] = sx;
            *(f32*)&vec[1] = sy;
            *(f32*)&vec[2] = sz;
            *(u32*)((s32)mobj + 0x184) = vec[0];
            *(u32*)((s32)mobj + 0x188) = vec[1];
            *(u32*)((s32)mobj + 0x18C) = vec[2];
            mobjCalcMtx2(mobj);
            break;
        case 2:
            anim = animPoseGetAnimBaseDataPtr(*(s32*)((s32)mobj + 0x70));
            vec[0] = vec3_802c2058[0];
            vec[1] = vec3_802c2058[1];
            vec[2] = vec3_802c2058[2];
            *(f32*)&vec[0] = float_0p8_804202a0 * float_10_80420294 * *(f32*)((s32)anim + 0xDC);
            *(f32*)&vec[1] = float_10_80420294 * *(f32*)((s32)anim + 0xE0);
            *(f32*)&vec[2] = float_0p8_804202a0 * float_10_80420294 * *(f32*)((s32)anim + 0xE4);
            radius = PSVECMag(vec);
            mario = marioGetPtr();
            if (PSVECDistance((void*)((s32)mobj + 0x38), (void*)((s32)mario + 0x8C)) > radius) {
                if ((*(u32*)mobj & 8) == 0) {
                    *(s32*)((s32)mobj + 0x1DC) = 3;
                    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_2_8042020c, 1);
                    psndSFXOn_3D(0x1D4, (void*)((s32)mobj + 0x38));
                    mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D8));
                } else {
                    *(u32*)mobj &= ~8;
                }
            }
            break;
        case 3:
            sy = *(f32*)((s32)mobj + 0x188) + float_0p9_80420318;
            sx = *(f32*)((s32)mobj + 0x184);
            sz = *(f32*)((s32)mobj + 0x18C);
            if (sy >= float_15_8042031c) {
                *(s32*)((s32)mobj + 0x1DC) = 0;
                sy = float_15_8042031c;
            }
            vec[0] = vec3_802c20ec[0];
            vec[1] = vec3_802c20ec[1];
            vec[2] = vec3_802c20ec[2];
            *(f32*)&vec[0] = sx;
            *(f32*)&vec[1] = sy;
            *(f32*)&vec[2] = sz;
            *(u32*)((s32)mobj + 0x184) = vec[0];
            *(u32*)((s32)mobj + 0x188) = vec[1];
            *(u32*)((s32)mobj + 0x18C) = vec[2];
            mobjCalcMtx2(mobj);
            *(u32*)mobj &= ~8;
            break;
    }
    return 0;
}

u8 evt_mobj_rideswitch_lightblue(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx2(void* mobj);
    extern void* gp;
    extern const char str_MOBJ_RideSwitchC_802c2974[];
    extern f32 float_0p8_804202a0;
    extern void mobj_rideswitch_lightblue(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    void* evtCode2;
    s32 swSet;
    const char* poseName;
    s32 inBattle;
    void* mobj;
    f32 scale;

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    evtCode = (void*)evtGetValue(pEvt, args[4]);
    evtCode2 = (void*)evtGetValue(pEvt, args[5]);
    inBattle = *(s32*)((s32)gp + 0x14);
    poseName = str_MOBJ_RideSwitchC_802c2974;
    swSet = args[6];
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    scale = float_0p8_804202a0;
    *(f32*)((s32)mobj + 0x184) *= scale;
    *(f32*)((s32)mobj + 0x18C) *= scale;
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_rideswitch_lightblue;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = evtCode2;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x200000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx2(mobj);
    return 2;
}

s32 mobj_jumpstand_red(void* pMobj) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* mobjRunEvent(void* mobj, void* evtCode);
    extern s32 strcmp(const char* a, const char* b);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void psndSFXOn(s32 id);
    extern s32 irand(s32 max);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern s32 evtCheckID(s32 id);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void* gp;
    extern const char str_MOBJ_RedJumpStand_802c20f8[];
    extern const char str_A_1_80420208[];
    extern const char str_S_1_8042021c[];
    void* mobj = pMobj;
    void* mario = marioGetPtr();
    s32 partyId = marioGetPartyId();
    void* party = partyGetPtr(partyId);
    s32 action = *(s32*)((s32)mobj + 0x1DC);
    void* evt;
    s32 value;
    s32 rnd;

    (void)party;
    switch (action) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                *(u32*)((s32)gp + 0x18) |= 0x10;
                evt = mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                if (evt != 0) {
                    if (*(f32*)((s32)mario + 0xD8) < *(f32*)((s32)mario + 0x90)) {
                        value = 0;
                    } else if (*(f32*)((s32)mario + 0xD8) < (*(f32*)((s32)mobj + 0x3C) + *(f32*)((s32)mario + 0x1BC))) {
                        value = 0;
                    } else {
                        value = 1;
                    }
                    *(s32*)((s32)evt + 0x9C) = value;
                }
                if (*(u8*)((s32)mario + 0x3C) == 2) {
                    if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RedJumpStand_802c20f8) == 0) {
                        psndSFXOn(0x1EB);
                    } else {
                        psndSFXOn(0x1EC);
                    }
                } else {
                    if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RedJumpStand_802c20f8) == 0) {
                        psndSFXOn_3D(0x1EB, (void*)((s32)mobj + 0x38));
                    } else {
                        psndSFXOn_3D(0x1EC, (void*)((s32)mobj + 0x38));
                    }
                    rnd = irand(4);
                    switch (rnd) {
                        case 0:
                            psndSFXOn_3D(0xAB, (void*)((s32)mobj + 0x38));
                            break;
                        case 1:
                            psndSFXOn_3D(0xAC, (void*)((s32)mobj + 0x38));
                            break;
                        case 2:
                            psndSFXOn_3D(0xAD, (void*)((s32)mobj + 0x38));
                            break;
                        case 3:
                            psndSFXOn_3D(0xAE, (void*)((s32)mobj + 0x38));
                            break;
                        case 4:
                            psndSFXOn_3D(0xAF, (void*)((s32)mobj + 0x38));
                            break;
                    }
                }
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(u32*)mobj &= ~8;
                *(s32*)((s32)mobj + 0x1DC) += 1;
            }
            break;
        case 1:
            if ((*(u32*)mobj & 8) != 0) {
                if (evtCheckID(*(s32*)((s32)mobj + 0x1CC)) == 0) {
                    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                    *(s32*)((s32)mobj + 0x1DC) = 0;
                    break;
                }
            }
            if (evtCheckID(*(s32*)((s32)mobj + 0x1CC)) != 1) {
                *(u32*)((s32)gp + 0x18) &= ~0x10;
                if (animPoseGetLoopTimes(*(s32*)((s32)mobj + 0x70)) >= 1.0f) {
                    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                    *(s32*)((s32)mobj + 0x1DC) = 0;
                }
            }
            break;
    }
    return 0;
}

s32 evt_mobj_jumpstand_red(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void* mobjNameToPtr(s32 name);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern void mobjCalcMtx(void* mobj);
    extern s32* gp;
    extern const char* lbl_802C2120[];
    extern s32 mobj_jumpstand_red(void* mobj);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 kind = evtGetValue(evt, args[0]);
    s32 name = evtGetValue(evt, args[1]);
    f32 x = evtGetFloat(evt, args[2]);
    f32 y = evtGetFloat(evt, args[3]);
    f32 z = evtGetFloat(evt, args[4]);
    s32 eventCode = evtGetValue(evt, args[5]);
    s32 script = args[6];
    const char* pose = lbl_802C2120[kind];
    void* mobj;

    if (animGroupBaseAsync(pose, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }
    mobjEntry(name, pose);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_jumpstand_red;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 0x44;
    *(u32*)mobj |= 0x200000;
    *(s32*)((s32)mobj + 0x1E4) = script;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    return 2;
}

s32 mobj_jumpstand_blue(void* pMobj) {
    extern void* marioGetPtr(void);
    extern void* mobjRunEvent(void* mobj, void* evtCode);
    extern void marioChgJumpStandMotion(f32 height);
    extern s32 strcmp(const char* a, const char* b);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void psndSFXOn(s32 id);
    extern s32 irand(s32 max);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern s32 evtCheckID(s32 id);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void* gp;
    extern const char str_MOBJ_BlueJumpStand_802c2128[];
    extern const char str_A_1_80420208[];
    extern const char str_S_1_8042021c[];
    void* mobj = pMobj;
    void* mario = marioGetPtr();
    s32 action = *(s32*)((s32)mobj + 0x1DC);
    s32 rnd;

    switch (action) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                *(u32*)((s32)gp + 0x18) |= 0x10;
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                if (*(u32*)((s32)mobj + 0x1E0) == 0x200000) {
                    marioChgJumpStandMotion(*(f32*)((s32)mobj + 0x1AC));
                }
                if (*(u8*)((s32)mario + 0x3C) == 2) {
                    if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BlueJumpStand_802c2128) == 0) {
                        psndSFXOn(0x1EB);
                    } else {
                        psndSFXOn(0x1EC);
                    }
                } else {
                    if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BlueJumpStand_802c2128) == 0) {
                        psndSFXOn_3D(0x1EB, (void*)((s32)mobj + 0x38));
                    } else {
                        psndSFXOn_3D(0x1EC, (void*)((s32)mobj + 0x38));
                    }
                    rnd = irand(4);
                    switch (rnd) {
                        case 0:
                            psndSFXOn_3D(0xAB, (void*)((s32)mobj + 0x38));
                            break;
                        case 1:
                            psndSFXOn_3D(0xAC, (void*)((s32)mobj + 0x38));
                            break;
                        case 2:
                            psndSFXOn_3D(0xAD, (void*)((s32)mobj + 0x38));
                            break;
                        case 3:
                            psndSFXOn_3D(0xAE, (void*)((s32)mobj + 0x38));
                            break;
                        case 4:
                            psndSFXOn_3D(0xAF, (void*)((s32)mobj + 0x38));
                            break;
                    }
                }
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(u32*)mobj &= ~8;
                *(s32*)((s32)mobj + 0x1DC) += 1;
            }
            break;
        case 1:
            if ((*(u32*)mobj & 8) != 0) {
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                *(s32*)((s32)mobj + 0x1DC) = 0;
            } else if (evtCheckID(*(s32*)((s32)mobj + 0x1CC)) != 1) {
                *(u32*)((s32)gp + 0x18) &= ~0x10;
                if (animPoseGetLoopTimes(*(s32*)((s32)mobj + 0x70)) >= 1.0f) {
                    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                    *(s32*)((s32)mobj + 0x1DC) = 0;
                }
            }
            break;
    }
    return 0;
}



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 evt_mobj_jumpstand_blue(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void* gp;
    extern const char* lbl_802C2150[2];
    extern void mobj_jumpstand_blue(void);
    void* evt = pEvt;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 kind;
    char* name;
    f32 height;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    const char* poseNames[2];
    s32 swSet;
    const char* poseName;
    s32 inBattle;
    void* mobj;

    kind = evtGetValue(evt, args[0]);
    name = (char*)evtGetValue(evt, args[1]);
    height = evtGetFloat(evt, args[2]);
    x = evtGetFloat(evt, args[3]);
    y = evtGetFloat(evt, args[4]);
    z = evtGetFloat(evt, args[5]);
    evtCode = (void*)evtGetValue(evt, args[6]);
    poseNames[0] = lbl_802C2150[0];
    poseNames[1] = lbl_802C2150[1];
    swSet = args[7];
    poseName = poseNames[kind];
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_jumpstand_blue;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 0x44;
    *(u32*)mobj |= 0x200000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(f32*)((s32)mobj + 0x1AC) = height;
    mobjCalcMtx(mobj);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 mobj_lv_blk_evt(void* pMobj) {
    extern void* marioGetPtr(void);
    extern char* strcat(char*, const char*);
    extern u32 animGroupBaseAsync(const char*, s32, s32);
    extern s32 strcmp(const char*, const char*);
    extern s32 pouchGetHammerLv(void);
    extern void* mobjRunEvent(void*, void*);
    extern void evtSetValue(void*, s32, s32);
    extern s32 animPaperPoseEntry(const char*, s32);
    extern s32 offscreenEntry(char*);
    extern s32 imgEntry(char*, s32);
    extern s32 offscreenNameToId(char*);
    extern s32 evt_img_alloc_capture(void*);
    extern void* imgNameToPtr(char*, s32);
    extern s32 animPaperPoseGetId(const char*, s32);
    extern f64 animTimeGetTime(s32);
    extern void imgSetShadow(void*, s32);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void imgFreeCapture(void*, s32);
    extern void imgRelease(void*);
    extern void offscreenDelete(char*);
    extern void mobjDelete(char*);
    extern void* gp;
    extern const char str_o_802c2158[];
    extern const char str_i_802c2168[];
    extern const char str_P_box_y_802c2618[];
    extern const char str_A_1_80420208[];
    extern const char str_MOBJ_Lv1Block_802c2904[];
    extern const char str_MOBJ_Lv1BigBlock_802c2914[];
    extern const char str_MOBJ_Lv1BigBigBlock_802c2928[];
    extern const char str_MOBJ_Lv2Block_802c293c[];
    extern const char str_MOBJ_Lv2BigBlock_802c294c[];
    extern const char str_MOBJ_Lv2BigBigBlock_802c2960[];
    extern f32 vec3_802c2178;
    extern f32 DAT_802c217c;
    extern f32 DAT_802c2180;
    char offName[32];
    char imgName[32];
    char evtA[0x1B0];
    char evtB[0x1B0];
    void* argsA[8];
    void* argsB[8];
    void* img;
    void* player;
    s32 heapType = *(s32*)((s32)gp + 0x14) != 0;
    u32 action = *(u32*)((s32)pMobj + 0x1DC);

    offName[0] = 0;
    imgName[0] = 0;
    strcat(offName, str_o_802c2158);
    strcat(imgName, str_i_802c2168);
    strcat(offName, (char*)((s32)pMobj + 5));
    strcat(imgName, (char*)((s32)pMobj + 5));
    if (action == 0) {
        animGroupBaseAsync(str_P_box_y_802c2618, heapType, 0);
        if ((*(u32*)pMobj & 8) != 0) {
            player = marioGetPtr();
            if ((strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv1Block_802c2904) != 0) &&
                (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv1BigBlock_802c2914) != 0)) {
                if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv1BigBigBlock_802c2928) == 0) {
                    if ((pouchGetHammerLv() < 2) || (*(s16*)((s32)player + 0x3C) != 0x13)) {
                        *(u32*)pMobj &= ~8;
                        return 0;
                    }
                } else if ((strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv2Block_802c293c) == 0) ||
                           (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv2BigBlock_802c294c) == 0)) {
                    if (pouchGetHammerLv() < 3) {
                        *(u32*)pMobj &= ~8;
                        return 0;
                    }
                } else if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv2BigBigBlock_802c2960) == 0) {
                    if ((pouchGetHammerLv() < 3) || (*(s16*)((s32)player + 0x3C) != 0x13)) {
                        *(u32*)pMobj &= ~8;
                        return 0;
                    }
                } else {
                    *(u32*)pMobj &= ~8;
                    return 0;
                }
            }
            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
            evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
            *(s32*)((s32)pMobj + 0x1DC) = 10;
        }
    } else if ((s32)action > 9 && (s32)action < 11) {
        animPaperPoseEntry(str_P_box_y_802c2618, heapType);
        offscreenEntry(offName);
        imgEntry(imgName, heapType);
        *(s32*)((s32)pMobj + 0x19C) = offscreenNameToId(offName);
        argsA[0] = imgName;
        argsA[1] = offName;
        argsA[2] = 0;
        argsA[3] = (void*)1;
        argsA[4] = 0;
        argsA[5] = 0;
        argsA[6] = 0;
        argsA[7] = 0;
        *(void**)(evtA + 0x18) = argsA;
        evt_img_alloc_capture(evtA);
        argsB[0] = imgName;
        argsB[1] = offName;
        argsB[2] = (void*)1;
        argsB[3] = (void*)1;
        argsB[4] = 0;
        argsB[5] = 0;
        argsB[6] = 0;
        argsB[7] = 0;
        *(void**)(evtB + 0x18) = argsB;
        evt_img_alloc_capture(evtB);
        img = imgNameToPtr(imgName, heapType);
        *(s32*)((s32)img + 0x70) = animPaperPoseGetId(str_P_box_y_802c2618, heapType);
        *(const char**)((s32)img + 0x108) = str_A_1_80420208;
        *(f64*)((s32)img + 0x110) = animTimeGetTime(heapType);
        imgSetShadow(img, 2);
        if ((strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv1Block_802c2904) == 0) ||
            (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv2Block_802c293c) == 0)) {
            psndSFXOn_3D(0x1C7, (void*)((s32)pMobj + 0x38));
        } else if ((strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv1BigBlock_802c2914) == 0) ||
                   (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_Lv2BigBlock_802c294c) == 0)) {
            psndSFXOn_3D(0x1C8, (void*)((s32)pMobj + 0x38));
        } else {
            psndSFXOn_3D(0x1C9, (void*)((s32)pMobj + 0x38));
        }
        *(s32*)((s32)pMobj + 0x1DC) = 0xB;
    } else if (action == 0xB) {
        img = imgNameToPtr(imgName, heapType);
        if (*(f32*)((s32)img + 0x118) >= 1.0f || (*(u32*)img & 0x10000000) != 0) {
            *(s32*)((s32)pMobj + 0x1DC) = 0xC;
        }
        *(f32*)((s32)pMobj + 0x38) = vec3_802c2178;
        *(f32*)((s32)pMobj + 0x3C) = DAT_802c217c;
        *(f32*)((s32)pMobj + 0x40) = DAT_802c2180;
        *(u32*)pMobj |= 4;
        *(u32*)pMobj |= 8;
    } else if ((s32)action < 99 && (s32)action < 0xD) {
        img = imgNameToPtr(imgName, heapType);
        imgFreeCapture(img, 0);
        imgFreeCapture(img, 1);
        imgSetShadow(img, 0);
        imgRelease(img);
        offscreenDelete(offName);
        *(s32*)((s32)pMobj + 0x19C) = -1;
        *(s32*)((s32)pMobj + 0x1DC) = 99;
    } else if (action == 99) {
        mobjDelete((char*)((s32)pMobj + 5));
    }
    return 0;
}

s32 evt_mobj_lv_blk(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void* gp;
    extern const char str_P_box_y_802c2618[];
    extern const u32 vec3_802c2184[3];
    extern void mobj_lv_blk_evt(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 evtCode;
    s32 swSet;
    char* poseName;
    s32 inBattle;
    void* mobj;
    u32 pos[3];

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    evtCode = evtGetValue(pEvt, args[4]);
    swSet = args[5];
    poseName = (char*)evtGetValue(pEvt, args[6]);
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (evtGetValue(pEvt, swSet) != 0) {
        return 2;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    if (animGroupBaseAsync(str_P_box_y_802c2618, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 0x44;
    pos[0] = vec3_802c2184[0];
    pos[1] = vec3_802c2184[1];
    pos[2] = vec3_802c2184[2];
    *(f32*)&pos[0] = x;
    *(f32*)&pos[1] = y;
    *(f32*)&pos[2] = z;
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(void**)((s32)mobj + 0x1D0) = mobj_lv_blk_evt;
    *(s32*)((s32)mobj + 0x1D4) = evtCode;
    *(s32*)((s32)mobj + 0x1D8) = 0;
    *(u32*)mobj |= 0x10000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    return 2;
}

s32 mobj_float_blk(void* mobj) {
    extern s32 hitCheckFilter(s32 flags, f32* outX, f32* outY, f32* outZ, f32* radius, f32* minY, f32* maxY);
    extern char* animPoseGetCurrentAnim(s32 poseId);
    extern s32 strcmp(const char* a, const char* b);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* mobjNameToPtr(char* name);
    extern f32 __fabsf(f32 value);
    extern const char str_S_1_8042021c[];
    extern const char str_A_1_80420208[];
    extern f32 float_0_80420240;
    extern f32 float_1_80420250;
    extern f32 float_0p5_8042024c;
    extern f32 float_neg1_80420304;
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 radius;
    f32 minY;
    f32 maxY;
    char* anim;

    if (*(s32*)((s32)mobj + 0x1DC) == 99) {
        if (*(s8*)((s32)mobj + 0x1B8) == 0) {
            radius = float_1_80420250 + __fabsf(*(f32*)((s32)mobj + 0x60));
            if (hitCheckFilter(0, &outX, &outY, &outZ, &radius, &minY, &maxY) != 0) {
                anim = animPoseGetCurrentAnim(*(s32*)((s32)mobj + 0x70));
                if (strcmp(anim, str_S_1_8042021c) != 0) {
                    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                    *(u32*)mobj |= 2;
                }
            } else {
                anim = animPoseGetCurrentAnim(*(s32*)((s32)mobj + 0x70));
                if (strcmp(anim, str_A_1_80420208) != 0) {
                    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                    *(u32*)mobj &= ~2U;
                }
            }
        } else {
            anim = animPoseGetCurrentAnim(*(s32*)((s32)mobjNameToPtr((char*)((s32)mobj + 0x1B8)) + 0x70));
            if ((anim[0] == 'A') && (anim[2] >= '1') && (anim[2] <= '3')) {
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 0);
            } else {
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
            }
        }
    }
    return 0;
}

s32 evt_mobj_float_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 ax, f32 ay, f32 az, s32 flags,
                              void* out0, void* out1, void* out2, void* out3,
                              void* out4, void* out5, void* out6);
    extern void* gp;
    extern const u32 vec3_802c2058[3];
    extern const char str_S_1_8042021c[4];
    extern const char str_A_1_80420208[4];
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_neg1_80420304;
    extern void mobj_float_blk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    const u32* base = vec3_802c2058;
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 color;
    s32 size;
    s32 swSet;
    const char* poseName;
    s32 inBattle;
    void* mobj;
    s32 out5;
    s32 out4;
    f32 limit;
    s32 out2;
    s32 out1;
    s32 out0;
    s32 out6[2];

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    color = args[4];
    size = args[5];
    evtGetValue(pEvt, args[6]);
    swSet = args[7];
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    switch (color) {
        case 0:
            if (size == 0) {
                poseName = (char*)((s32)base + 0x7A0);
            } else if (size == 1) {
                poseName = (char*)((s32)base + 0x7B8);
            } else {
                poseName = (char*)((s32)base + 0x7D4);
            }
            break;
        case 1:
            if (size == 0) {
                poseName = (char*)((s32)base + 0x7F0);
            } else if (size == 1) {
                poseName = (char*)((s32)base + 0x808);
            } else {
                poseName = (char*)((s32)base + 0x824);
            }
            break;
        case 2:
            if (size == 0) {
                poseName = (char*)((s32)base + 0x840);
            } else if (size == 1) {
                poseName = (char*)((s32)base + 0x858);
            } else {
                poseName = (char*)((s32)base + 0x874);
            }
            break;
        case 3:
            poseName = (char*)((s32)base + 0x890);
            break;
        default:
            if (size == 0) {
                poseName = (char*)((s32)base + 0x7A0);
            } else if (size == 1) {
                poseName = (char*)((s32)base + 0x7B8);
            } else {
                poseName = (char*)((s32)base + 0x7D4);
            }
            break;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_float_blk;
    *(u32*)mobj |= 4;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 99;
    mobjCalcMtx(mobj);
    limit = float_1_80420250 + __fabs(*(f32*)((s32)mobj + 0x60));
    if (hitCheckFilter(*(f32*)((s32)mobj + 0x38),
                       float_0p5_8042024c + *(f32*)((s32)mobj + 0x3C),
                       *(f32*)((s32)mobj + 0x40),
                       float_0_80420240, float_neg1_80420304, float_0_80420240,
                       0, &out0, &out1, &out2, &limit, &out4, &out5, out6) != 0) {
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
        *(u32*)mobj |= 2;
    } else {
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
    }
    return 2;
}


s32 mobj_switch_float_blk(void* pMobj) {
    extern char* animPoseGetCurrentAnim(s32 poseId);
    extern s32 strcmp(const char* a, const char* b);
    extern void* mobjNameToPtr(char* name);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags,
                              void* out0, void* out1, void* out2, void* out3,
                              void* out4, void* out5, void* out6);
    extern s32 evtCheckID(s32 evtId);
    extern void* mobjRunEvent(void* pMobj, void* evtCode);
    extern void marioChgMot(s32 motion);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* animName, s32 flags);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern f32 __fabsf(f32 x);
    extern const char* _switch_anim[7];
    extern const char str_A_3_80420210[4];
    extern const char str_S_1_8042021c[4];
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_neg1_80420304;

    u32 flags;
    char* anim;
    void* other;
    s32 hit;
    s32 i;
    s32 next;
    const char** animTable;
    f32 hitOutA[7];
    f32 hitOutB[7];
    f32 vy;

#define MOBJ_U32(o) (*(u32*)((s32)pMobj + (o)))
#define MOBJ_S32(o) (*(s32*)((s32)pMobj + (o)))
#define MOBJ_F32(o) (*(f32*)((s32)pMobj + (o)))
#define MOBJ_S8(o) (*(s8*)((s32)pMobj + (o)))

    switch (MOBJ_S32(0x1DC)) {
        case 0:
            if ((MOBJ_U32(0) & 8) == 0) {
                break;
            }

            anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
            next = 0;
            animTable = _switch_anim;
            for (i = 0; i < 6; i++, animTable++) {
                if (strcmp(*animTable, anim) == 0) {
                    next = i;
                    break;
                }
            }

            MOBJ_U32(0x1A4) = (MOBJ_U32(0x1A4) & 0x40000000U) | next;

            if (MOBJ_S8(0x1B8) != 0) {
                other = mobjNameToPtr((char*)((s32)pMobj + 0x1B8));
                vy = *(f32*)((s32)other + 0x60);
                hitOutA[3] = float_1_80420250 + __fabsf(vy);
                hit = hitCheckFilter(*(f32*)((s32)other + 0x38),
                                     *(f32*)((s32)other + 0x3C) + float_0p5_8042024c,
                                     *(f32*)((s32)other + 0x40),
                                     float_0_80420240, float_neg1_80420304,
                                     float_0_80420240, 0,
                                     &hitOutA[0], &hitOutA[1], &hitOutA[2], &hitOutA[3],
                                     &hitOutA[4], &hitOutA[5], &hitOutA[6]);

                if (hit != 0) {
                    if (evtCheckID(MOBJ_S32(0x1CC)) != 0) {
                        anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
                        if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '1') && ((s8)anim[2] <= '3')) {
                            next = 0;
                        } else {
                            next = 3;
                        }
                    } else {
                        next = 0;
                        MOBJ_U32(0x1A4) |= 0x80000000U;
                    }
                } else {
                    if (evtCheckID(MOBJ_S32(0x1CC)) != 0) {
                        anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
                        if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '1') && ((s8)anim[2] <= '2')) {
                            next = 0;
                        } else if (((s8)anim[0] == 'A') && (anim[2] == '3')) {
                            next = 0;
                            MOBJ_U32(0x1A4) |= 0x40000000U;
                        } else if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '4') && ((s8)anim[2] <= '5')) {
                            next = 3;
                        } else {
                            next = 3;
                        }
                    } else {
                        next = 3;
                        MOBJ_U32(0x1A4) |= 0x80000000U;
                    }
                }
            } else {
                anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
                if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '1') && ((s8)anim[2] <= '3')) {
                    next = 3;
                } else {
                    next = 0;
                }
                MOBJ_U32(0x1A4) |= 0x80000000U;
            }

            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
            if (MOBJ_S32(0x1E0) == 0x00200000) {
                marioChgMot(5);
            }
            psndSFXOn_3D(0x1B5, (void*)((s32)pMobj + 0x38));

            if (MOBJ_S32(0x1E0) == 0x00400000) {
                animPoseSetAnim(MOBJ_S32(0x70), _switch_anim[next + 1], 1);
            } else {
                animPoseSetAnim(MOBJ_S32(0x70), _switch_anim[next], 1);
            }

            MOBJ_U32(0) &= ~8U;
            if (MOBJ_S32(0x1B0) != 0) {
                MOBJ_S32(0x1DC) = 10;
            } else {
                MOBJ_S32(0x1DC)++;
            }
            break;

        case 1:
            flags = MOBJ_U32(0);
            if ((flags & 8) != 0) {
                MOBJ_S32(0x1DC) = 0;
            } else if ((flags & 0x1000) != 0) {
                MOBJ_U32(0) = flags & ~0x1000U;
                MOBJ_S32(0x1DC) = 10;
            } else if (animPoseGetLoopTimes(MOBJ_S32(0x70)) >= float_1_80420250) {
                if (MOBJ_S8(0x1B8) != 0) {
                    mobjNameToPtr((char*)((s32)pMobj + 0x1B8));
                }
                anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
                if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '1') && ((s8)anim[2] <= '3')) {
                    animPoseSetAnim(MOBJ_S32(0x70), str_A_3_80420210, 1);
                } else {
                    animPoseSetAnim(MOBJ_S32(0x70), str_S_1_8042021c, 1);
                }
                MOBJ_S32(0x1DC)++;
            }
            break;

        case 2:
            flags = MOBJ_U32(0);
            if ((flags & 8) != 0) {
                MOBJ_S32(0x1DC) = 0;
            } else {
                if ((flags & 0x1000) != 0) {
                    MOBJ_U32(0) = flags & ~0x1000U;
                    MOBJ_S32(0x1DC) = 10;
                }
                if (evtCheckID(MOBJ_S32(0x1CC)) == 0) {
                    MOBJ_S32(0x1DC) = 0;
                    MOBJ_U32(0x1A4) = 0;
                }
            }
            break;

        case 10:
            if ((MOBJ_U32(0) & 8) != 0) {
                MOBJ_S32(0x1DC) = 0;
                break;
            }

            if (animPoseGetLoopTimes(MOBJ_S32(0x70)) < float_1_80420250) {
                break;
            }

            anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
            if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '1') && ((s8)anim[2] <= '3')) {
                animPoseSetAnim(MOBJ_S32(0x70), str_A_3_80420210, 1);
            } else {
                animPoseSetAnim(MOBJ_S32(0x70), str_S_1_8042021c, 1);
            }
            MOBJ_S32(0x1B0) = 0x78;
            MOBJ_S32(0x1DC)++;
            break;

        case 11:
            if ((MOBJ_U32(0) & 8) != 0) {
                MOBJ_S32(0x1DC) = 0;
                break;
            }

            MOBJ_S32(0x1B0)--;
            if (MOBJ_S32(0x1B0) == 0) {
                if (MOBJ_S8(0x1B8) != 0) {
                    other = mobjNameToPtr((char*)((s32)pMobj + 0x1B8));
                    vy = *(f32*)((s32)other + 0x60);
                    hitOutB[3] = float_1_80420250 + __fabsf(vy);
                    hit = hitCheckFilter(*(f32*)((s32)other + 0x38),
                                         *(f32*)((s32)other + 0x3C) + float_0p5_8042024c,
                                         *(f32*)((s32)other + 0x40),
                                         float_0_80420240, float_neg1_80420304,
                                         float_0_80420240, 0,
                                         &hitOutB[0], &hitOutB[1], &hitOutB[2], &hitOutB[3],
                                         &hitOutB[4], &hitOutB[5], &hitOutB[6]);
                    if (hit == 0) {
                        animPoseSetAnim(MOBJ_S32(0x70), str_A_3_80420210, 0);
                    } else {
                        animPoseSetAnim(MOBJ_S32(0x70), str_S_1_8042021c, 0);
                    }
                } else {
                    anim = animPoseGetCurrentAnim(MOBJ_S32(0x70));
                    if (((s8)anim[0] == 'A') && ((s8)anim[2] >= '1') && ((s8)anim[2] <= '3')) {
                        animPoseSetAnim(MOBJ_S32(0x70), str_A_3_80420210, 1);
                    } else {
                        animPoseSetAnim(MOBJ_S32(0x70), str_S_1_8042021c, 1);
                    }
                }
                MOBJ_S32(0x1DC) = 0;
            }
            break;
    }

#undef MOBJ_U32
#undef MOBJ_S32
#undef MOBJ_F32
#undef MOBJ_S8

    return 0;
}

s32 evt_mobj_switch_float_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern char* strcpy(char* dst, const char* src);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 ax, f32 ay, f32 az, s32 flags,
                              void* out0, void* out1, void* out2, void* out3,
                              void* out4, void* out5, void* out6);
    extern void* gp;
    extern const u32 vec3_802c2058[3];
    extern const char str_S_1_8042021c[4];
    extern const char str_A_3_80420210[4];
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_neg1_80420304;
    extern void mobj_switch_float_blk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    const u32* base = vec3_802c2058;
    char* name;
    f32 x;
    f32 y;
    f32 z;
    char* linkName;
    s32 kind;
    s32 evtCode;
    s32 swSet;
    const char* poseName;
    s32 inBattle;
    void* mobj;
    void* other;
    s32 out0;
    s32 out1;
    s32 out2;
    f32 limit;
    s32 out4;
    s32 out5;
    s32 out6[2];

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    linkName = (char*)evtGetValue(pEvt, args[4]);
    kind = args[5];
    evtCode = evtGetValue(pEvt, args[6]);
    swSet = args[7];
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    switch (kind) {
        case 0:
            poseName = (char*)((s32)base + 0x730);
            break;
        case 1:
            poseName = (char*)((s32)base + 0x74C);
            break;
        case 2:
            poseName = (char*)((s32)base + 0x768);
            break;
        case 3:
            poseName = (char*)((s32)base + 0x784);
            break;
        default:
            poseName = (char*)((s32)base + 0x730);
            break;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_switch_float_blk;
    *(s32*)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0xE10000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(s32*)((s32)mobj + 0x1B0) = 0;
    mobjCalcMtx(mobj);
    strcpy((char*)((s32)mobj + 0x1B8), linkName);
    if (*(s8*)((s32)mobj + 0x1B8) != 0) {
        other = mobjNameToPtr((char*)((s32)mobj + 0x1B8));
        strcpy((char*)((s32)other + 0x1B8), (char*)((s32)mobj + 5));
        limit = float_1_80420250 + __fabs(*(f32*)((s32)other + 0x60));
        if (hitCheckFilter(*(f32*)((s32)other + 0x38),
                           float_0p5_8042024c + *(f32*)((s32)other + 0x3C),
                           *(f32*)((s32)other + 0x40),
                           float_0_80420240, float_neg1_80420304, float_0_80420240,
                           0, &out0, &out1, &out2, &limit, &out4, &out5, out6) != 0) {
            animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
        } else {
            animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_3_80420210, 1);
        }
    }
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_lock_unlock(void* evt) {
    extern void marioKeyOff(void);
    extern void partyKeyOff(void);
    extern void marioStSystemLevel(s32 level);
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_MOBJ_Lock_802c2770[];
    s32* args = *(s32**)((s32)evt + 0x18);
    void* mobj = mobjNameToPtr(evtGetValue(evt, args[0]));

    if (strcmp((const char*)((s32)mobj + 0x15), str_MOBJ_Lock_802c2770) != 0) {
        return 2;
    }
    if (*(void**)((s32)mobj + 0x1D0) == 0) {
        *(void**)((s32)mobj + 0x1D0) = mobj_lock;
    }
    *(u32*)mobj |= 8;
    marioKeyOff();
    partyKeyOff();
    marioStSystemLevel(1);
    *(s32*)((s32)mobj + 0x1DC) = 3;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 mobj_lock(void* pMobj) {
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern void partyKeyOff(void);
    extern void partyKeyOn(void);
    extern void marioStSystemLevel(s32);
    extern void* mobjRunEvent(void*, void*);
    extern s32 evtCheckID(s32);
    extern s32 pouchCheckItem(s32);
    extern void pouchRemoveItem(s32);
    extern char* msgSearch(const char*);
    extern s32 msgWindow_Entry(char*, s32, s32);
    extern void* evtEntryType(void*, s8, u32, u32);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern void evtSetValue(void*, s32, s32);
    extern s32 hitCheckFilter(f32, f32, f32, f32, f32, f32, s32, void*, void*, void*, void*, void*, void*, void*);
    extern u32 psndSFXOn_3D(s32, void*);
    extern s32 windowCheckID(s32);
    extern void mobjDelete(char*);
    extern void* evt_lock_sound;
    extern const char str_sys_no_key_802c277c[];
    extern const char str_S_1_8042021c[];
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_4_80420314;
    extern f32 float_neg0p8_80420310;
    extern f32 float_1024_80420290;
    u32 action = *(u32*)((s32)pMobj + 0x1DC);
    s32 item;
    s32 i;
    s32 found;
    void* evt;
    f32 hitY;
    f32 hitDist;
    s32 out0, out2, out3, out4, out5, out6;

    if (action == 0) {
        if ((*(u32*)pMobj & 8) != 0) {
            marioKeyOff();
            partyKeyOff();
            marioStSystemLevel(1);
            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
            *(s32*)((s32)pMobj + 0x1DC) += 1;
        }
    } else if (action == 1) {
        if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) == 0) {
            if ((*(u32*)pMobj & 0x20) == 0) {
                *(s32*)((s32)pMobj + 0x1CC) = 0;
                *(s32*)((s32)pMobj + 0x1DC) += 1;
            } else {
                *(u32*)pMobj &= ~0x20;
                *(u32*)pMobj &= ~8;
                marioKeyOn();
                partyKeyOn();
                marioStSystemLevel(0);
                *(s32*)((s32)pMobj + 0x1CC) = 0;
                *(s32*)((s32)pMobj + 0x1DC) = 0;
            }
        }
    } else if (action == 2) {
        item = *(s32*)((s32)pMobj + 0x1A0);
        found = -1;
        if (item < 0) {
            if (pouchCheckItem(item) != 0) {
                found = item;
            }
        } else {
            for (i = 0; ; i++) {
                item = *(s32*)(*(s32*)((s32)pMobj + 0x1A0) + i * 4);
                if (item == -1) {
                    break;
                }
                if (pouchCheckItem(item) != 0) {
                    found = item;
                    break;
                }
            }
        }
        if (found == -1) {
            *(s32*)((s32)pMobj + 0x1A8) = msgWindow_Entry(msgSearch(str_sys_no_key_802c277c), 0, 0);
            *(s32*)((s32)pMobj + 0x1DC) = 10;
        } else {
            pouchRemoveItem(found);
            *(s32*)((s32)pMobj + 0x1DC) += 1;
        }
    } else if (action == 3) {
        evt = evtEntryType(&evt_lock_sound, 0, 0, 0);
        *(s32*)((s32)evt + 0x18) = (s32)(float_1024_80420290 * *(f32*)((s32)pMobj + 0x38)) - 230000000;
        *(s32*)((s32)evt + 0x1C) = (s32)(float_1024_80420290 * *(f32*)((s32)pMobj + 0x3C)) - 230000000;
        *(s32*)((s32)evt + 0x20) = (s32)(float_1024_80420290 * *(f32*)((s32)pMobj + 0x40)) - 230000000;
        animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
        *(s32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action == 4) {
        if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_1_80420250) {
            *(f32*)((s32)pMobj + 0x60) = float_0_80420240;
            *(f32*)((s32)pMobj + 0x68) = float_0p5_8042024c;
            *(f32*)((s32)pMobj + 0x6C) = float_0_80420240;
            evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D8));
            *(s32*)((s32)pMobj + 0x1DC) += 1;
        }
    } else if (action == 5) {
        hitDist = float_1_80420250 + (*(f32*)((s32)pMobj + 0x60) < 0.0f ? -*(f32*)((s32)pMobj + 0x60) : *(f32*)((s32)pMobj + 0x60));
        if (hitCheckFilter(*(f32*)((s32)pMobj + 0x38), float_1_80420250 + *(f32*)((s32)pMobj + 0x3C),
                           *(f32*)((s32)pMobj + 0x40), float_0_80420240, *(f32*)((s32)pMobj + 0x60),
                           float_0_80420240, 0, &out0, &hitY, &out2, &hitDist, &out3, &out4, &out5) == 0) {
            *(f32*)((s32)pMobj + 0x3C) += *(f32*)((s32)pMobj + 0x60);
            *(f32*)((s32)pMobj + 0x60) -= *(f32*)((s32)pMobj + 0x68);
        } else {
            *(f32*)((s32)pMobj + 0x3C) = hitY;
            *(f32*)((s32)pMobj + 0x60) *= float_neg0p8_80420310;
            *(f32*)((s32)pMobj + 0x6C) += float_1_80420250;
            if (*(f32*)((s32)pMobj + 0x6C) < float_4_80420314) {
                psndSFXOn_3D(0x1E6, (void*)((s32)pMobj + 0x38));
            }
        }
        if ((*(f32*)((s32)pMobj + 0x6C) >= float_4_80420314) && (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) == 0)) {
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            *(s32*)((s32)pMobj + 0x1B0) = 0x14;
        }
    } else if (action == 6) {
        *(s32*)((s32)pMobj + 0x1B0) -= 1;
        if ((*(s32*)((s32)pMobj + 0x1B0) & 2) == 0) {
            *(u32*)pMobj &= ~0x10;
        } else {
            *(u32*)pMobj |= 0x10;
        }
        if (*(s32*)((s32)pMobj + 0x1B0) == 0) {
            *(s32*)((s32)pMobj + 0x1DC) = 99;
        }
    } else if (action == 10) {
        if (windowCheckID(*(s32*)((s32)pMobj + 0x1A8)) == 0) {
            *(u32*)pMobj &= ~8;
            marioKeyOn();
            partyKeyOn();
            marioStSystemLevel(0);
            *(s32*)((s32)pMobj + 0x1DC) = 0;
        }
    } else if (action == 99) {
        marioKeyOn();
        partyKeyOn();
        marioStSystemLevel(0);
        mobjDelete((char*)((s32)pMobj + 5));
    }
    return 0;
}

s32 evt_mobj_lock(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjHitEntry(void* mobj, s32 kind);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gp;
    extern const char str_MOBJ_Lock_802c2770[];
    extern const char str_Z_1_8042030c[4];
    void* evt = pEvt;
    s32* args = *(s32**)((s32)evt + 0x18);
    char* name;
    s32 kind;
    f32 x;
    f32 y;
    f32 z;
    f32 rotY;
    s32 evtCode;
    s32 evtCode2;
    s32 swSet;
    s32 inBattle;
    const char* poseName;
    void* mobj;

    name = (char*)evtGetValue(evt, args[0]);
    kind = evtGetValue(evt, args[1]);
    x = evtGetFloat(evt, args[2]);
    y = evtGetFloat(evt, args[3]);
    z = evtGetFloat(evt, args[4]);
    rotY = evtGetFloat(evt, args[5]);
    evtCode = evtGetValue(evt, args[6]);
    evtCode2 = evtGetValue(evt, args[7]);
    swSet = args[8];
    poseName = str_MOBJ_Lock_802c2770;
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (evtGetValue(evt, swSet) != 0) {
        return 2;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjHitEntry(mobj, 2);
    *(s32*)((s32)mobj + 0x1A0) = kind;
    mobjSetPosition(x, y, z, name);
    *(f32*)((s32)mobj + 0x54) = rotY;
    *(void**)((s32)mobj + 0x1D0) = mobj_lock;
    *(s32*)((s32)mobj + 0x1D4) = evtCode;
    *(s32*)((s32)mobj + 0x1D8) = evtCode2;
    *(u32*)mobj |= 0x26;
    *(u32*)mobj |= 0x40000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_Z_1_8042030c, 1);
    return 2;
}

s32 mobj_itembox(void* pMobj) {
    extern s32 strcmp(const char*, const char*);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern void* animPoseGetAnimBaseDataPtr(s32);
    extern void PSVECAdd(void*, void*, void*);
    extern void PSVECScale(f32, void*, void*);
    extern void mobjCalcMtx2(void*);
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern void partyKeyOff(void);
    extern void partyKeyOn(void);
    extern void marioStSystemLevel(s32);
    extern void* mobjRunEvent(void*, void*);
    extern s32 evtCheckID(s32);
    extern void* evtEntryType(void*, s8, u32, u32);
    extern void evtSetValue(void*, s32, s32);
    extern void* evt_box_sound1;
    extern void* evt_box_sound2;
    extern void* evt_box_sound3;
    extern const char str_MOBJ_TreasureBox_802c26f8[];
    extern const char str_MOBJ_GrayTreasureBox_802c2720[];
    extern const char str_MOBJ_BlackTreasureBo_802c2738[];
    extern const char str_A_1_80420208[];
    extern const char str_S_2_804202b0[];
    extern const char str_S_4_80420308[];
    extern f64 double_0p2_802c2768;
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_1p4_804202f8;
    extern f32 float_1p2_804202fc;
    extern f32 float_1024_80420290;
    u32 action = *(u32*)((s32)pMobj + 0x1DC);
    void* evt;
    void* pose;
    f32 min[3];
    f32 max[3];
    s32 ix;
    s32 iy;
    s32 iz;

    if (action == 0) {
        if ((*(u32*)pMobj & 8) != 0) {
            marioKeyOff();
            partyKeyOff();
            marioStSystemLevel(1);
            *(s32*)((s32)pMobj + 0x1CC) = 0;
            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
            *(s32*)((s32)pMobj + 0x1DC) += 1;
        }
    } else if (action == 1) {
        if (*(s32*)((s32)pMobj + 0x1CC) != 0) {
            if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) != 0) {
                return 0;
            }
            if ((*(u32*)pMobj & 0x20) != 0) {
                *(u32*)pMobj &= ~0x20;
                *(u32*)pMobj &= ~8;
                marioKeyOn();
                partyKeyOn();
                marioStSystemLevel(0);
                *(s32*)((s32)pMobj + 0x1CC) = 0;
                *(s32*)((s32)pMobj + 0x1DC) = 0;
                return 0;
            }
        }
        *(s32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action == 2) {
        if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_TreasureBox_802c26f8) == 0 ||
            strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_GrayTreasureBox_802c2720) == 0) {
            evt = evtEntryType(&evt_box_sound1, 0, 0, 0);
        } else if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_BlackTreasureBo_802c2738) == 0) {
            evt = evtEntryType(&evt_box_sound3, 0, 0, 0);
        } else {
            evt = evtEntryType(&evt_box_sound2, 0, 0, 0);
        }
        ix = (s32)(float_1024_80420290 * *(f32*)((s32)pMobj + 0x38));
        iy = (s32)(float_1024_80420290 * *(f32*)((s32)pMobj + 0x3C));
        iz = (s32)(float_1024_80420290 * *(f32*)((s32)pMobj + 0x40));
        *(s32*)((s32)evt + 0x18) = ix - 230000000;
        *(s32*)((s32)evt + 0x1C) = iy - 230000000;
        *(s32*)((s32)evt + 0x20) = iz - 230000000;
        animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
        *(s32*)((s32)pMobj + 0x1CC) = 0;
        mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D8));
        *(s32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action == 3) {
        if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_1_80420250 &&
            (*(s32*)((s32)pMobj + 0x1CC) == 0 || evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) == 0)) {
            *(u32*)pMobj &= ~0x20;
            pose = animPoseGetAnimBaseDataPtr(*(s32*)((s32)pMobj + 0x70));
            min[0] = *(f32*)((s32)pose + 0x20);
            min[1] = *(f32*)((s32)pose + 0x24);
            min[2] = *(f32*)((s32)pose + 0x28);
            max[0] = *(f32*)((s32)pose + 0x2C);
            max[1] = *(f32*)((s32)pose + 0x30);
            max[2] = *(f32*)((s32)pose + 0x34);
            min[2] = (f32)-(double_0p2_802c2768 * ((max[1] - min[1]) < 0.0f ? -(max[1] - min[1]) : (max[1] - min[1])) - (double)min[2]);
            PSVECAdd(max, min, (void*)((s32)pMobj + 0x184));
            PSVECScale(float_0p5_8042024c, (void*)((s32)pMobj + 0x184), (void*)((s32)pMobj + 0x184));
            PSVECScale(10.0f, (void*)((s32)pMobj + 0x184), (void*)((s32)pMobj + 0x184));
            *(f32*)((s32)pMobj + 0x194) = float_0_80420240;
            *(f32*)((s32)pMobj + 0x48) *= float_1p4_804202f8;
            *(f32*)((s32)pMobj + 0x4C) *= float_1p2_804202fc;
            mobjCalcMtx2(pMobj);
            marioKeyOn();
            partyKeyOn();
            marioStSystemLevel(0);
            *(s32*)((s32)pMobj + 0x1CC) = 0;
            *(s32*)((s32)pMobj + 0x1DC) = 99;
        }
    } else if (action == 99) {
        evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
        *(u32*)pMobj |= 8;
        *(u32*)pMobj &= ~0x20;
        if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_BlackTreasureBo_802c2738) == 0) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_4_80420308, 1);
        } else {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_2_804202b0, 1);
        }
    }
    return 0;
}

s32 evt_mobj_itembox(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern u32 animGroupBaseAsync(const char*, s32, s32);
    extern s32 mobjEntry(char*, const char*);
    extern void* mobjNameToPtr(char*);
    extern void mobjHitEntry(void*, s32);
    extern void mobjSetPosition(f32, f32, f32, char*);
    extern void mobjCalcMtx(void*);
    extern void mobjCalcMtx2(void*);
    extern void* animPoseGetAnimBaseDataPtr(s32);
    extern void PSVECAdd(void*, void*, void*);
    extern void PSVECScale(f32, void*, void*);
    extern s32 hitCheckFilter(f32, f32, f32, f32, f32, f32, s32, void*, void*, void*, void*, void*, void*, void*);
    extern void* gp;
    extern const char str_MOBJ_TreasureBox_802c26f8[];
    extern const char str_MOBJ_BigTreasureBox_802c270c[];
    extern const char str_MOBJ_GrayTreasureBox_802c2720[];
    extern const char str_MOBJ_BlackTreasureBo_802c2738[];
    extern const char str_MOBJ_GoldenTreasureB_802c2750[];
    extern f64 double_0p2_802c2768;
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_1p4_804202f8;
    extern f32 float_1p2_804202fc;
    extern f32 float_5_80420300;
    extern f32 float_neg1_80420304;
    extern void mobj_itembox(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name = (char*)evtGetValue(pEvt, args[0]);
    f32 y = evtGetFloat(pEvt, args[1]);
    f32 x = evtGetFloat(pEvt, args[2]);
    f32 z = evtGetFloat(pEvt, args[3]);
    s32 kind = evtGetValue(pEvt, args[4]);
    void* evtCode = (void*)evtGetValue(pEvt, args[5]);
    void* evtCode2 = (void*)evtGetValue(pEvt, args[6]);
    s32 swSet = args[7];
    s32 inBattle = *(s32*)((s32)gp + 0x14);
    const char* poseName;
    void* mobj;
    void* pose;
    f32 min[3];
    f32 max[3];
    f32 out[3];
    f32 hitDist;
    s32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;

    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (kind == 0) {
        poseName = str_MOBJ_TreasureBox_802c26f8;
    } else if (kind == 1) {
        poseName = str_MOBJ_BigTreasureBox_802c270c;
    } else if (kind == 2) {
        poseName = str_MOBJ_GrayTreasureBox_802c2720;
    } else if (kind == 3) {
        poseName = str_MOBJ_BlackTreasureBo_802c2738;
    } else {
        poseName = str_MOBJ_GoldenTreasureB_802c2750;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjHitEntry(mobj, 2);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_itembox;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = evtCode2;
    *(u32*)mobj |= 0x64;
    *(u32*)mobj |= 0x00040000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    if (evtGetValue(pEvt, swSet) != 0) {
        pose = animPoseGetAnimBaseDataPtr(*(s32*)((s32)mobj + 0x70));
        min[0] = *(f32*)((s32)pose + 0x20);
        min[1] = *(f32*)((s32)pose + 0x24);
        min[2] = *(f32*)((s32)pose + 0x28);
        max[0] = *(f32*)((s32)pose + 0x2C);
        max[1] = *(f32*)((s32)pose + 0x30);
        max[2] = *(f32*)((s32)pose + 0x34);
        min[2] = (f32)-(double_0p2_802c2768 * ((max[1] - min[1]) < 0.0f ? -(max[1] - min[1]) : (max[1] - min[1])) - (double)min[2]);
        PSVECAdd(max, min, (void*)((s32)mobj + 0x184));
        PSVECScale(float_0p5_8042024c, (void*)((s32)mobj + 0x184), (void*)((s32)mobj + 0x184));
        PSVECScale(10.0f, (void*)((s32)mobj + 0x184), (void*)((s32)mobj + 0x184));
        *(f32*)((s32)mobj + 0x194) = float_0_80420240;
        *(f32*)((s32)mobj + 0x48) *= float_1p4_804202f8;
        *(f32*)((s32)mobj + 0x4C) *= float_1p2_804202fc;
        mobjCalcMtx2(mobj);
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    hitDist = float_5_80420300 + (*(f32*)((s32)mobj + 0x60) < 0.0f ? -*(f32*)((s32)mobj + 0x60) : *(f32*)((s32)mobj + 0x60));
    if (hitCheckFilter(*(f32*)((s32)mobj + 0x38), float_1_80420250 + *(f32*)((s32)mobj + 0x3C),
                       *(f32*)((s32)mobj + 0x40), float_0_80420240, float_neg1_80420304,
                       float_0_80420240, 0, &tmp0, &tmp1, &tmp2, &hitDist, &tmp3, &tmp4, &tmp5) != 0) {
        *(u32*)mobj |= 2;
        *(u32*)mobj &= ~0x20;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 mobj_signboard(void* mobj) {
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern s32 evtCheckID(s32 id);
    extern void* mobjRunEvent(void* mobj, void* code);
    s32 state = *(s32*)((s32)mobj + 0x1DC);

    switch (state) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                marioKeyOff();
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                *(s32*)((s32)mobj + 0x1DC) += 1;
            }
            break;
        case 1:
            if (evtCheckID(*(s32*)((s32)mobj + 0x1CC)) != 1) {
                marioKeyOn();
                *(s32*)((s32)mobj + 0x1CC) = 0;
                *(u32*)mobj &= ~8;
                *(s32*)((s32)mobj + 0x1DC) = 0;
            }
            break;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
s32 evt_mobj_signboard(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern s32 mobj_signboard(void* mobj);
    extern s32* gp;
    extern const char str_MOBJ_Signboard_802c26e8[];
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 script;
    const char* kind;
    s32 eventCode;
    void* mobj;
    s32 mode;

    eventCode = evtGetValue(evt, args[4]);
    kind = str_MOBJ_Signboard_802c26e8;
    script = args[5];
    mode = gp[5];
    mode = ((u32)(-mode) | (u32)mode) >> 31;

    if (animGroupBaseAsync(kind, mode, 0) == 0) {
        return 0;
    }
    mobjEntry(name, kind);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_signboard;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 0x24;
    *(u32*)mobj |= 0x40000;
    *(s32*)((s32)mobj + 0x1E4) = script;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    return 2;
}
#pragma no_register_save_helpers off


s32 mobj_recovery_blk(void* pMobj) {
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern void* camGetPtr(s32);
    extern u32 psndSFXOn_3D(s32, void*);
    extern s32 swGet(s32);
    extern void swSet(s32);
    extern s32 pouchGetCoin(void);
    extern void effSoftDelete(void*);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern void statusWinForceOpen(void);
    extern void statusWinForceOff(void);
    extern void effStardustEntry(f64, f64, f64, f64, f64, s32, s32, s32);
    extern void* marioGetPtr(void);
    extern void* pouchGetPtr(void);
    extern void marioChgPose(const char*);
    extern s32 pouchAddCoin(s16);
    extern char* msgSearch(const char*);
    extern s32 msgWindow_Entry(char*, s32, s32);
    extern s32 windowCheckID(s32);
    extern void* effCoinFukidashiEntry(f64, f64, f64, s32, s32, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern const char str_A_1_80420208[];
    extern const char str_A_2_8042020c[];
    extern const char str_S_1_8042021c[];
    extern const char str_M_I_1_804202e8[];
    extern const char str_M_S_1_804202f0[];
    extern const char str_sys_no_coin_802c26bc[];
    extern const char str_kaifuku_first_hit_802c26c8[];
    extern void* gp;
    extern f32 float_1_80420250;
    extern f32 float_10_80420294;
    extern f32 float_25_80420230;
    extern f32 float_45_804202e4;
    extern f32 float_50_8042023c;
    u32 action = *(u32*)((s32)pMobj + 0x1DC);
    void* cam;
    void* mario;
    void* pouch;
    void* eff;
    s32 count;
    s32 i;
    u16 flags;
    char* msg;

    switch (action) {
        case 0:
            if ((*(u32*)pMobj & 8) == 0) {
                return 0;
            }
            marioKeyOff();
            cam = camGetPtr(1);
            *(u16*)cam |= 0x200;
            psndSFXOn_3D(0x1D9, (void*)((s32)pMobj + 0x38));
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            /* fallthrough */
        case 1:
            if (((*(u32*)((s32)gp + 0x18) & 0x1000) == 0) && (swGet(0xEA) == 0)) {
                swSet(0xEA);
                *(s32*)((s32)pMobj + 0x1DC) = 0x14;
                break;
            }
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            /* fallthrough */
        case 2:
            if (pouchGetCoin() < *(s32*)((s32)pMobj + 0x1A4)) {
                *(s32*)((s32)pMobj + 0x1DC) = 10;
                break;
            }
            if (*(void**)((s32)pMobj + 0x1C8) != 0) {
                effSoftDelete(*(void**)((s32)pMobj + 0x1C8));
                *(s32*)((s32)pMobj + 0x1C8) = 0;
            }
            psndSFXOn_3D(0x1DA, (void*)((s32)pMobj + 0x38));
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_2_8042020c, 1);
            if (*(s32*)((s32)pMobj + 0x1A4) == 0) {
                *(s32*)((s32)pMobj + 0x1DC) = 3;
            } else {
                *(s32*)((s32)pMobj + 0x1DC) = 4;
                statusWinForceOpen();
            }
            *(s32*)((s32)pMobj + 0x1B0) = 0x78;
            break;
        case 3:
        case 4:
            if (*(s32*)((s32)pMobj + 0x1B0) == 100) {
                effStardustEntry(*(f32*)((s32)pMobj + 0x38),
                                  float_45_804202e4 + *(f32*)((s32)pMobj + 0x3C),
                                  *(f32*)((s32)pMobj + 0x40), float_50_8042023c,
                                  float_50_8042023c, 1, 0x1E, 0x3C);
                psndSFXOn_3D(0x1DB, (void*)((s32)pMobj + 0x38));
                mario = marioGetPtr();
                pouch = pouchGetPtr();
                effStardustEntry(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90),
                                  *(f32*)((s32)mario + 0x94), float_50_8042023c,
                                  float_50_8042023c, 2, 0x32, 100);
                marioChgPose(str_M_I_1_804202e8);
                *(s16*)((s32)pouch + 0x2) = *(s16*)((s32)pouch + 0x0);
                *(s16*)((s32)pouch + 0x6) = *(s16*)((s32)pouch + 0x4);
                for (i = 0; i < 8; i++) {
                    flags = *(u16*)((s32)pouch + 0x68 + i * 0x14);
                    if (((flags & 1) != 0) && ((flags & 2) == 0)) {
                        *(s16*)((s32)pouch + 0x70 + i * 0x14) = *(s16*)((s32)pouch + 0x72 + i * 0x14);
                    }
                }
                if (action == 3) {
                    pouchAddCoin(0);
                } else {
                    pouchAddCoin(-(s16)*(s32*)((s32)pMobj + 0x1A4));
                }
            }
            *(s32*)((s32)pMobj + 0x1B0) -= 1;
            if (*(s32*)((s32)pMobj + 0x1B0) < 1) {
                *(s32*)((s32)pMobj + 0x1DC) = 5;
            }
            break;
        case 5:
            if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_1_80420250) {
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
                marioChgPose(str_M_S_1_804202f0);
                marioKeyOn();
                statusWinForceOff();
                cam = camGetPtr(1);
                *(u16*)cam &= ~0x200;
                *(s32*)((s32)pMobj + 0x1DC) += 1;
            }
            break;
        case 6:
            if (*(void**)((s32)pMobj + 0x1C8) == 0) {
                eff = effCoinFukidashiEntry(*(f32*)((s32)pMobj + 0x38),
                                            float_25_80420230 + *(f32*)((s32)pMobj + 0x3C),
                                            *(f32*)((s32)pMobj + 0x40) - float_10_80420294, 0,
                                            *(s32*)((s32)pMobj + 0x1A4), 0);
                *(void**)((s32)pMobj + 0x1C8) = eff;
            }
            if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_1_80420250) {
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
                *(u32*)pMobj &= ~8;
                *(s32*)((s32)pMobj + 0x1DC) = 0;
            }
            break;
        case 10:
            msg = msgSearch(str_sys_no_coin_802c26bc);
            *(s32*)((s32)pMobj + 0x1A8) = msgWindow_Entry(msg, 0, 0);
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            break;
        case 11:
            if (windowCheckID(*(s32*)((s32)pMobj + 0x1A8)) == 0) {
                marioKeyOn();
                cam = camGetPtr(1);
                *(u16*)cam &= ~0x200;
                *(u32*)pMobj &= ~8;
                *(s32*)((s32)pMobj + 0x1DC) = 0;
            }
            break;
        case 0x14:
            msg = msgSearch(str_kaifuku_first_hit_802c26c8);
            *(s32*)((s32)pMobj + 0x1A8) = msgWindow_Entry(msg, 0, 0);
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            break;
        case 0x15:
            if (windowCheckID(*(s32*)((s32)pMobj + 0x1A8)) == 0) {
                marioKeyOn();
                cam = camGetPtr(1);
                *(u16*)cam &= ~0x200;
                *(u32*)pMobj &= ~8;
                *(s32*)((s32)pMobj + 0x1DC) = 0;
            }
            break;
        case 0xFFFFFFFF:
            eff = effCoinFukidashiEntry(*(f32*)((s32)pMobj + 0x38),
                                        float_25_80420230 + *(f32*)((s32)pMobj + 0x3C),
                                        *(f32*)((s32)pMobj + 0x40) - float_10_80420294, 0,
                                        *(s32*)((s32)pMobj + 0x1A4), 0);
            *(void**)((s32)pMobj + 0x1C8) = eff;
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            break;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_recovery_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gp;
    extern const char str_MOBJ_RecoveryBlock_802c268c[];
    extern const char str_MOBJ_CoinTakenRecove_802c26a0[];
    extern const char str_S_1_8042021c[4];
    extern void mobj_recovery_blk(void);
    void* evt = pEvt;
    s32* args = *(s32**)((s32)evt + 0x18);
    char* name;
    s32 kind;
    f32 x;
    f32 y;
    f32 z;
    s32 swSet;
    const char* poseName;
    s32 inBattle;
    void* mobj;

    name = (char*)evtGetValue(evt, args[0]);
    kind = evtGetValue(evt, args[1]);
    x = evtGetFloat(evt, args[2]);
    y = evtGetFloat(evt, args[3]);
    z = evtGetFloat(evt, args[4]);
    evtGetValue(evt, args[5]);
    poseName = str_MOBJ_CoinTakenRecove_802c26a0;
    inBattle = *(s32*)((s32)gp + 0x14);
    swSet = args[6];
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (kind == 0) {
        poseName = str_MOBJ_RecoveryBlock_802c268c;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    *(s32*)((s32)mobj + 0x1A4) = kind;
    mobjSetPosition(x, y, z, name);
    *(u32*)mobj |= 4;
    *(void**)((s32)mobj + 0x1D0) = mobj_recovery_blk;
    *(s32*)((s32)mobj + 0x1D4) = 0;
    *(s32*)((s32)mobj + 0x1D8) = 0;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x400000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    if (kind == 0) {
        *(s32*)((s32)mobj + 0x1DC) = 0;
    } else {
        *(s32*)((s32)mobj + 0x1DC) = -1;
    }
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 mobj_save_blk(void* pMobj) {
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern void marioStSystemLevel(s32 level);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* camGetPtr(s32 camId);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void mobjRunEvent(void* mobj, void* evtCode);
    extern s32 evtCheckID(s32 evtId);
    extern const char str_A_1_80420208[4];
    extern const char str_S_1_8042021c[4];
    extern f32 float_1_80420250;
    s32 action = *(s32*)((s32)pMobj + 0x1DC);
    void* cam;

    switch (action) {
        case 0:
            if ((*(u32*)pMobj & 8) != 0) {
                marioKeyOff();
                marioStSystemLevel(1);
                psndSFXOn_3D(0x1AF, (void*)((s32)pMobj + 0x38));
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
                cam = camGetPtr(8);
                *(u16*)cam |= 0x200;
                *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
            }
            break;
        case 1:
            if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) < float_1_80420250) {
                break;
            }
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
            if (*(void**)((s32)pMobj + 0x1D4) != NULL) {
                mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
                *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
            } else {
                *(s32*)((s32)pMobj + 0x1DC) = 3;
            }
            break;
        case 2:
            if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) != 1) {
                *(s32*)((s32)pMobj + 0x1CC) = 0;
                *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
            }
            break;
        case 3:
            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D8));
            *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
            break;
        case 4:
            if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC)) != 1) {
                marioStSystemLevel(0);
                cam = camGetPtr(8);
                *(u16*)cam &= (u16)~0x200;
                *(u32*)pMobj &= ~8;
                marioKeyOn();
                *(s32*)((s32)pMobj + 0x1DC) = 0;
            }
            break;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_save_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gp;
    extern const char str_MOBJ_SaveBlock_802c267c[];
    extern const char str_S_1_8042021c[4];
    extern void mobj_save_blk(void);
    extern void mobj_save_blk_sysevt(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 evtCode;
    s32 swSet;
    const char* poseName;
    void* mobj;
    s32 inBattle;
    u32 flags;

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    evtCode = evtGetValue(pEvt, args[4]);
    poseName = str_MOBJ_SaveBlock_802c267c;
    swSet = args[5];
    flags = *(u32*)gp;
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if ((flags & 0x20) != 0) {
        return 2;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_save_blk;
    *(s32*)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = mobj_save_blk_sysevt;
    *(u32*)mobj |= 0x84;
    *(u32*)mobj |= 0x400000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



s32 mobj_blk(void* mobj) {
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* mobjRunEvent(void* mobj, void* evtCode);
    extern s32 evtCheckID(s32 id);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern const char str_A_1_80420208[];
    extern const char str_S_1_8042021c[];
    extern f32 float_1_80420250;
    s32 action = *(s32*)((s32)mobj + 0x1DC);

    switch (action) {
        case 0:
            if ((*(u32*)mobj & 8) != 0) {
                psndSFXOn_3D(0x1B7, (void*)((s32)mobj + 0x38));
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_1_80420208, 1);
                *(u32*)mobj &= ~8;
                *(s32*)((s32)mobj + 0x1DC) = *(s32*)((s32)mobj + 0x1DC) + 1;
            }
            break;
        case 1:
            if (*(void**)((s32)mobj + 0x1D4) != 0) {
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                *(s32*)((s32)mobj + 0x1DC) = *(s32*)((s32)mobj + 0x1DC) + 1;
            } else {
                *(s32*)((s32)mobj + 0x1DC) = 3;
            }
            break;
        case 2:
            if (evtCheckID(*(s32*)((s32)mobj + 0x1CC)) != 1) {
                *(s32*)((s32)mobj + 0x1CC) = 0;
                *(s32*)((s32)mobj + 0x1DC) = *(s32*)((s32)mobj + 0x1DC) + 1;
            }
            break;
        case 3:
            if ((*(u32*)mobj & 8) == 0) {
                *(s32*)((s32)mobj + 0x1DC) = 0;
            } else if (animPoseGetLoopTimes(*(s32*)((s32)mobj + 0x70)) >= float_1_80420250) {
                animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
                *(s32*)((s32)mobj + 0x1DC) = 0;
                *(u32*)mobj &= ~8;
            }
            break;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gp;
    extern const u32 vec3_802c2058[3];
    extern const char str_S_1_8042021c[4];
    extern void mobj_blk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    const u32* base;
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 kind;
    void* evtCode;
    s32 swSet;
    char* poseName;
    void* mobj;
    s32 inBattle;

    base = vec3_802c2058;
    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    kind = args[4];
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    poseName = (char*)((s32)base + 0x450);
    swSet = args[6];
    if (kind == 1) {
        poseName = (char*)((s32)base + 0x5C8);
    } else if (kind < 1) {
        if (kind <= -1) {
            poseName = (char*)((s32)base + 0x450);
        }
    } else {
        poseName = (char*)((s32)base + 0x450);
    }
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_blk;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x40000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 mobj_badgeblk(void* pMobj) {
    extern s32 strcmp(const char*, const char*);
    extern char* strcpy(char*, const char*);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void* mobjRunEvent(void*, void*);
    extern s32 pouchAddCoin(s16);
    extern void* itemEntry(char*, s32, u32, s32, void*, f32, f32, f32);
    extern void* itemNameToPtr(char*);
    extern void itemFlagOn(void*, u16);
    extern void evtSetValue(void*, s32, s32);
    extern const char str_MOBJ_BadgeBlock_802c266c[];
    extern const char str_MOBJ_BrickBadgeBlock_802c25bc[];
    extern const char str_MOBJ_HiddenBadgeBloc_802c25e8[];
    extern const char str_MOBJ_Block_802c24a8[];
    extern const char str_MOBJ_PinkBlock_802c2620[];
    extern const char str_A_1_80420208[];
    extern const char str_A_2_8042020c[];
    extern const char str_S_2_804202b0[];
    extern f32 float_1_80420250;
    extern f32 float_40_804202dc;
    extern f32 float_0p7_804202b8;
    void* item;
    u32 action = *(u32*)((s32)pMobj + 0x1DC);

    if (action == 0) {
        if ((*(u32*)pMobj & 8) != 0) {
            if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_BadgeBlock_802c266c) == 0 ||
                strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_BrickBadgeBlock_802c25bc) == 0 ||
                strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_HiddenBadgeBloc_802c25e8) == 0) {
                psndSFXOn_3D(0x1BB, (void*)((s32)pMobj + 0x38));
            } else {
                psndSFXOn_3D(0x1BA, (void*)((s32)pMobj + 0x38));
            }
            if ((*(u32*)pMobj & 0x800) != 0) {
                *(u32*)pMobj &= ~0x800;
            }
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
            if (*(void**)((s32)pMobj + 0x1D4) == 0) {
                if (*(s32*)((s32)pMobj + 0x1A0) == 0x79) {
                    pouchAddCoin(1);
                    itemEntry(0, 0x79, 0xF, -1, 0, *(f32*)((s32)pMobj + 0x38),
                              *(f32*)((s32)pMobj + 0x3C), *(f32*)((s32)pMobj + 0x40));
                    evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
                    psndSFXOn_3D(0x5B, (void*)((s32)pMobj + 0x38));
                } else {
                    itemEntry((char*)((s32)pMobj + 5), *(s32*)((s32)pMobj + 0x1A0), 4,
                              *(s32*)((s32)pMobj + 0x1E4), 0,
                              *(f32*)((s32)pMobj + 0x38),
                              float_40_804202dc + *(f32*)((s32)pMobj + 0x3C),
                              *(f32*)((s32)pMobj + 0x40));
                    item = itemNameToPtr((char*)((s32)pMobj + 5));
                    itemFlagOn(item, 0x10);
                    *(f32*)((s32)item + 0x34) = float_0p7_804202b8;
                    *(f32*)((s32)item + 0x38) = float_1_80420250;
                    *(s32*)((s32)item + 0x3C) = 0;
                    *(f32*)((s32)item + 0x40) = 360.0f;
                    psndSFXOn_3D(0x1BC, (void*)((s32)pMobj + 0x38));
                }
            } else {
                mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
            }
            *(s32*)((s32)pMobj + 0x1DC) += 1;
        }
    } else if (action == 1) {
        if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_1_80420250) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_2_804202b0, 1);
            *(u32*)pMobj &= ~8;
            *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
            if (strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_BrickBadgeBlock_802c25bc) == 0 ||
                strcmp((char*)((s32)pMobj + 0x15), str_MOBJ_HiddenBadgeBloc_802c25e8) == 0) {
                strcpy((char*)((s32)pMobj + 0x15), str_MOBJ_PinkBlock_802c2620);
            } else {
                strcpy((char*)((s32)pMobj + 0x15), str_MOBJ_Block_802c24a8);
            }
        }
    } else if (action == 0x5A) {
        if ((*(u32*)pMobj & 8) != 0) {
            *(u32*)pMobj &= ~8;
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_2_8042020c, 1);
            psndSFXOn_3D(0x1B7, (void*)((s32)pMobj + 0x38));
            *(s32*)((s32)pMobj + 0x1DC) += 1;
        }
    } else if ((s32)action < 0x5C) {
        if ((*(u32*)pMobj & 8) == 0) {
            *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
        } else if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= (double)float_1_80420250) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_2_804202b0, 1);
            *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
            *(u32*)pMobj &= ~8;
        }
    }
    return 0;
}
s32 evt_mobj_badgeblk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern char* strcpy(char* dst, const char* src);
    extern void* gp;
    extern const char str_MOBJ_HatenaBlock_802c24cc[];
    extern const char str_MOBJ_BadgeBlock_802c266c[];
    extern const char str_MOBJ_Block_802c24a8[];
    extern const char str_MOBJ_PinkBlock_802c2620[];
    extern const char str_S_1_8042021c[];
    extern const char str_S_2_804202b0[];
    extern void mobj_badgeblk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 y;
    f32 x;
    f32 z;
    s32 item;
    void* evtCode;
    s32 swSet;
    s32 pink;
    const char* poseName;
    const char* blockName;
    void* mobj;
    s32 inBattle;

    name = (char*)evtGetValue(pEvt, args[0]);
    y = evtGetFloat(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    item = evtGetValue(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    pink = args[7];
    poseName = str_MOBJ_HatenaBlock_802c24cc;
    swSet = args[6];
    if (pink != 0) {
        poseName = str_MOBJ_BadgeBlock_802c266c;
    }
    inBattle = ((u32)(-*(s32*)((s32)gp + 0x14)) | (u32)*(s32*)((s32)gp + 0x14)) >> 31;
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_badgeblk;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(s32*)((s32)mobj + 0x1A0) = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 0x5A;
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_2_804202b0, 1);
        blockName = str_MOBJ_Block_802c24a8;
        if (pink != 0) {
            blockName = str_MOBJ_PinkBlock_802c2620;
        }
        strcpy((char*)((s32)mobj + 0x15), blockName);
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 mobj_powerupblk(void* pMobj) {
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern s32 marioKeyOff(void);
    extern s32 marioCtrlOff(void);
    extern s32 marioKeyOn(void);
    extern s32 marioCtrlOn(void);
    extern s32 strcmp(const char* a, const char* b);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* itemEntry(char* name, s32 id, u32 mode, s32 collected, void* script, f32 x, f32 y, f32 z);
    extern f64 animPoseGetLoopTimes(s32 poseId);
    extern void itemForceGet(void* item);
    extern s32 evtSetValue(void* evt, s32 var, s32 value);
    extern void mobjDelete(char* name);
    extern void* gp;
    extern const u32 vec3_802c2058[3];
    extern const char str_A_1_80420208[4];
    extern const char str_A_2_8042020c[4];
    extern f32 float_1_80420250;
    extern f32 float_40_804202dc;
    const u32* base = vec3_802c2058;
    s32 action = *(s32*)((s32)pMobj + 0x1DC);

    if (action != 2) {
        if (action < 2) {
            if (action == 0) {
                if ((*(u32*)pMobj & 8) != 0) {
                    if ((marioKeyOffChk() != 0) || (marioCtrlOffChk() != 0)) {
                        *(u32*)pMobj &= ~8;
                    } else {
                        marioKeyOff();
                        if (strcmp((char*)((s32)gp + 0x12C), (char*)((s32)base + 0x5EC)) == 0) {
                            marioCtrlOff();
                        } else if (((strcmp((char*)((s32)gp + 0x12C), (char*)((s32)base + 0x5F4)) == 0) ||
                                    (strcmp((char*)((s32)gp + 0x12C), (char*)((s32)base + 0x5FC)) == 0)) &&
                                   (*(u32*)((s32)pMobj + 0x1E0) == 0x00800000)) {
                            marioCtrlOff();
                        }
                        *(u32*)((s32)gp + 0x18) |= 3;
                        psndSFXOn_3D(0x201C0, (void*)((s32)pMobj + 0x38));
                        animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
                        itemEntry((char*)((s32)base + 0x604), 0x57, 0xF, -1, 0,
                                  *(f32*)((s32)pMobj + 0x38),
                                  *(f32*)((s32)pMobj + 0x3C),
                                  *(f32*)((s32)pMobj + 0x40));
                        *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
                    }
                }
            } else if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= float_1_80420250) {
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_2_8042020c, 1);
                *(u32*)pMobj &= ~8;
                *(s32*)((s32)pMobj + 0x1A4) = 1;
                *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
            }
        } else if (action == 0x5A) {
            mobjDelete((char*)((s32)pMobj + 5));
        }
    } else if (*(s32*)((s32)pMobj + 0x1A4) == 0) {
        itemForceGet(itemEntry((char*)((s32)base + 0x60C), 0x57, 0x11, -1, 0,
                               *(f32*)((s32)pMobj + 0x38),
                               float_40_804202dc + *(f32*)((s32)pMobj + 0x3C),
                               *(f32*)((s32)pMobj + 0x40)));
        evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
        marioKeyOn();
        if (strcmp((char*)((s32)gp + 0x12C), (char*)((s32)base + 0x5EC)) == 0) {
            marioCtrlOn();
        } else if (((strcmp((char*)((s32)gp + 0x12C), (char*)((s32)base + 0x5F4)) == 0) ||
                    (strcmp((char*)((s32)gp + 0x12C), (char*)((s32)base + 0x5FC)) == 0)) &&
                   (*(u32*)((s32)pMobj + 0x1E0) == 0x00800000)) {
            marioCtrlOn();
        }
        *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
    } else {
        *(s32*)((s32)pMobj + 0x1A4) = *(s32*)((s32)pMobj + 0x1A4) - 1;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_mobj_powerupblk(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void* mobjNameToPtr(s32 name);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern s32* gp;
    extern const char str_MOBJ_PowerUpBlock_802c2630[];
    extern const char str_S_1_8042021c[];
    extern const char str_A_2_8042020c[];
    extern s32 mobj_powerupblk(void* mobj);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 eventCode = evtGetValue(evt, args[4]);
    s32 switchVar = args[5];
    f32 rotY = evtGetFloat(evt, args[6]);
    void* mobj;

    if (animGroupBaseAsync(str_MOBJ_PowerUpBlock_802c2630, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_PowerUpBlock_802c2630);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_powerupblk;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0xC10000;
    *(s32*)((s32)mobj + 0x1E4) = switchVar;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(f32*)((s32)mobj + 0x54) = rotY;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    if (evtGetValue(evt, switchVar) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 0x5A;
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_A_2_8042020c, 1);
    }
    return 2;
}

s32 mobj_brickblk(void* pMobj) {
    extern char* strcat(char*, const char*);
    extern u32 animGroupBaseAsync(const char*, s32, s32);
    extern void* mobjRunEvent(void*, void*);
    extern void evtSetValue(void*, s32, s32);
    extern s32 animPaperPoseEntry(const char*, s32);
    extern s32 offscreenEntry(char*);
    extern s32 imgEntry(char*, s32);
    extern s32 offscreenNameToId(char*);
    extern s32 evt_img_alloc_capture(void*);
    extern void* imgNameToPtr(char*, s32);
    extern s32 animPaperPoseGetId(const char*, s32);
    extern f64 animTimeGetTime(s32);
    extern void imgSetShadow(void*, s32);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void imgFreeCapture(void*, s32);
    extern void imgRelease(void*);
    extern void offscreenDelete(char*);
    extern void mobjDelete(char*);
    extern void* gp;
    extern const char str_o_802c2290[];
    extern const char str_i_802c22a0[];
    extern const char str_P_box_y_802c2618[];
    extern const char str_A_1_80420208[];
    extern f32 vec3_802c22b0;
    extern f32 DAT_802c22b4;
    extern f32 DAT_802c22b8;
    char offName[32];
    char imgName[32];
    char evtA[0x1B0];
    char evtB[0x1B0];
    void* argsA[8];
    void* argsB[8];
    void* img;
    s32 heapType;
    u32 action;

    heapType = *(s32*)((s32)gp + 0x14) != 0;
    offName[0] = 0;
    imgName[0] = 0;
    strcat(offName, str_o_802c2290);
    strcat(imgName, str_i_802c22a0);
    strcat(offName, (char*)((s32)pMobj + 5));
    strcat(imgName, (char*)((s32)pMobj + 5));
    action = *(u32*)((s32)pMobj + 0x1DC);

    if (action == 0) {
        animGroupBaseAsync(str_P_box_y_802c2618, heapType, 0);
        if ((*(u32*)pMobj & 8) != 0) {
            mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
            evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
            *(s32*)((s32)pMobj + 0x1DC) = 10;
        }
    } else if ((s32)action > 9 && (s32)action < 11) {
        animPaperPoseEntry(str_P_box_y_802c2618, heapType);
        offscreenEntry(offName);
        imgEntry(imgName, heapType);
        *(s32*)((s32)pMobj + 0x19C) = offscreenNameToId(offName);

        argsA[0] = imgName;
        argsA[1] = offName;
        argsA[2] = 0;
        argsA[3] = (void*)1;
        argsA[4] = 0;
        argsA[5] = 0;
        argsA[6] = 0;
        argsA[7] = 0;
        *(void**)(evtA + 0x18) = argsA;
        evt_img_alloc_capture(evtA);

        argsB[0] = imgName;
        argsB[1] = offName;
        argsB[2] = (void*)1;
        argsB[3] = (void*)1;
        argsB[4] = 0;
        argsB[5] = 0;
        argsB[6] = 0;
        argsB[7] = 0;
        *(void**)(evtB + 0x18) = argsB;
        evt_img_alloc_capture(evtB);

        img = imgNameToPtr(imgName, heapType);
        *(s32*)((s32)img + 0x104) = animPaperPoseGetId(str_P_box_y_802c2618, heapType);
        *(const char**)((s32)img + 0x108) = str_A_1_80420208;
        *(f64*)((s32)img + 0x110) = animTimeGetTime(heapType);
        imgSetShadow(img, 2);
        psndSFXOn_3D(0x1B8, (void*)((s32)pMobj + 0x38));
        *(s32*)((s32)pMobj + 0x1DC) = 11;
    } else if (action == 11) {
        img = imgNameToPtr(imgName, heapType);
        if (*(f32*)((s32)img + 0x118) >= 1.0f ||
            (*(u32*)img & 0x10000000) != 0) {
            *(s32*)((s32)pMobj + 0x1DC) = 12;
        }
        *(f32*)((s32)pMobj + 0x38) = vec3_802c22b0;
        *(f32*)((s32)pMobj + 0x3C) = DAT_802c22b4;
        *(f32*)((s32)pMobj + 0x40) = DAT_802c22b8;
        *(u32*)pMobj |= 4;
        *(u32*)pMobj |= 8;
    } else if ((s32)action < 99 && (s32)action < 13) {
        img = imgNameToPtr(imgName, heapType);
        imgFreeCapture(img, 0);
        imgFreeCapture(img, 1);
        imgSetShadow(img, 0);
        imgRelease(img);
        offscreenDelete(offName);
        *(s32*)((s32)pMobj + 0x19C) = -1;
        *(s32*)((s32)pMobj + 0x1DC) = 99;
    } else if (action == 99) {
        mobjDelete((char*)((s32)pMobj + 5));
    }
    return 0;
}

s32 mobj_10countblk(void* pMobj) {
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern s32 evtSetValue(void* evt, s32 var, s32 value);
    extern void* mobjRunEvent(void* pMobj, void* evtCode);
    extern s32 pouchAddCoin(s16 coins);
    extern void* itemEntry(char* name, s32 id, u32 mode, s32 collected, void* script, f32 x, f32 y, f32 z);
    extern f64 animPoseGetLoopTimes(s32 poseId);
    extern const char str_A_1_80420208[];
    extern const char str_A_3_80420210[];
    extern const char str_S_2_804202b0[];
    extern const char str_MOBJ_Block_802c24a8[];
    extern f32 float_1_80420250;
    u32 action = *(u32*)((s32)pMobj + 0x1DC);
    u32 flags;

    if (action == 0x5A) {
        if ((*(u32*)pMobj & 8) != 0) {
            psndSFXOn_3D(0x1B7, (void*)((s32)pMobj + 0x38));
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_3_80420210, 1);
            *(u32*)pMobj &= ~8;
            *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
        }
    } else if ((s32)action < 0x5A) {
        if (action == 1) {
            if (*(s32*)((s32)pMobj + 0x1B0) > 0) {
                *(s32*)((s32)pMobj + 0x1B0) = *(s32*)((s32)pMobj + 0x1B0) - 1;
            }
            if (*(s32*)((s32)pMobj + 0x1B0) < 1) {
                *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_2_804202b0, 1);
                strcpy((char*)((s32)pMobj + 0x15), str_MOBJ_Block_802c24a8);
            } else if ((*(u32*)pMobj & 8) != 0) {
                psndSFXOn_3D(0x1BF, (void*)((s32)pMobj + 0x38));
                pouchAddCoin(1);
                itemEntry(0, 0x79, 0xF, -1, 0,
                          *(f32*)((s32)pMobj + 0x38),
                          *(f32*)((s32)pMobj + 0x3C),
                          *(f32*)((s32)pMobj + 0x40));
                *(u32*)pMobj &= ~8;
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
            }
        } else if ((s32)action < 1) {
            if (((s32)action > -1) && ((*(u32*)pMobj & 8) != 0)) {
                flags = *(u32*)pMobj;
                if ((flags & 0x800) != 0) {
                    *(u32*)pMobj = flags & ~0x800;
                    strcpy((char*)((s32)pMobj + 0x15), str_MOBJ_Block_802c24a8);
                }
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
                psndSFXOn_3D(0x1BE, (void*)((s32)pMobj + 0x38));
                evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
                if (*(void**)((s32)pMobj + 0x1D4) == 0) {
                    pouchAddCoin(1);
                    itemEntry(0, 0x79, 0xF, -1, 0,
                              *(f32*)((s32)pMobj + 0x38),
                              *(f32*)((s32)pMobj + 0x3C),
                              *(f32*)((s32)pMobj + 0x40));
                    *(s32*)((s32)pMobj + 0x1B0) = 200;
                } else {
                    mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
                }
                *(s32*)((s32)pMobj + 0x1DC) = *(s32*)((s32)pMobj + 0x1DC) + 1;
                *(u32*)pMobj &= ~8;
            }
        }
    } else if ((s32)action < 0x5C) {
        if ((*(u32*)pMobj & 8) == 0) {
            *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
        } else if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) >= float_1_80420250) {
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_2_804202b0, 1);
            *(s32*)((s32)pMobj + 0x1DC) = 0x5A;
            *(u32*)pMobj &= ~8;
        }
    }
    return 0;
}

s32 evt_mobj_brick(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern u32 animGroupBaseAsync(const char*, s32, s32);
    extern s32 mobjEntry(char*, const char*);
    extern void* mobjNameToPtr(char*);
    extern void mobjSetPosition(f32, f32, f32, char*);
    extern void mobjCalcMtx(void*);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern char* strcpy(char*, const char*);
    extern void* gp;
    extern const char str_MOBJ_FlowBrickBlock_802c24e0[];
    extern const char str_MOBJ_HiddenHatenaBlo_802c24b4[];
    extern const char str_MOBJ_BrickHatenaBloc_802c25a4[];
    extern const char str_MOBJ_BrickBadgeBlock_802c25bc[];
    extern const char str_MOBJ_10CountBlock_802c25d4[];
    extern const char str_MOBJ_HiddenBadgeBloc_802c25e8[];
    extern const char str_MOBJ_Hidden10CountBl_802c2600[];
    extern const char str_MOBJ_Block_802c24a8[];
    extern const char str_MOBJ_PinkBlock_802c2620[];
    extern const char str_P_box_y_802c2618[];
    extern const char str_S_1_8042021c[];
    extern const char str_S_2_804202b0[];
    extern const char str_S_3_80420214[];
    extern void mobj_brickblk(void);
    extern void mobj_badgeblk(void);
    extern void mobj_10countblk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name = (char*)evtGetValue(pEvt, args[0]);
    f32 y = evtGetFloat(pEvt, args[1]);
    f32 x = evtGetFloat(pEvt, args[2]);
    f32 z = evtGetFloat(pEvt, args[3]);
    s32 item = evtGetValue(pEvt, args[4]);
    s32 kind = args[5];
    void* evtCode = (void*)evtGetValue(pEvt, args[6]);
    s32 swSet = args[7];
    s32 inBattle = *(s32*)((s32)gp + 0x14);
    const char* poseName;
    const char* doneName;
    void* mainFunc;
    void* mobj;

    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    switch (kind) {
        case 0:
            poseName = str_MOBJ_FlowBrickBlock_802c24e0;
            mainFunc = mobj_brickblk;
            break;
        case 1:
            poseName = str_MOBJ_BrickHatenaBloc_802c25a4;
            mainFunc = mobj_badgeblk;
            break;
        case 2:
            poseName = str_MOBJ_BrickBadgeBlock_802c25bc;
            mainFunc = mobj_badgeblk;
            break;
        case 3:
            poseName = str_MOBJ_10CountBlock_802c25d4;
            mainFunc = mobj_10countblk;
            break;
        case 10:
        case 11:
            poseName = str_MOBJ_HiddenHatenaBlo_802c24b4;
            mainFunc = (kind == 10) ? mobj_brickblk : mobj_badgeblk;
            break;
        case 12:
            poseName = str_MOBJ_HiddenBadgeBloc_802c25e8;
            mainFunc = mobj_badgeblk;
            break;
        case 13:
            poseName = str_MOBJ_Hidden10CountBl_802c2600;
            mainFunc = mobj_10countblk;
            break;
        default:
            poseName = str_MOBJ_FlowBrickBlock_802c24e0;
            mainFunc = mobj_brickblk;
            break;
    }
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    if (animGroupBaseAsync(str_P_box_y_802c2618, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, name);
    if (kind > 9) {
        *(u32*)mobj |= 0x800;
    }
    *(void**)((s32)mobj + 0x1D0) = mainFunc;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0xC10000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(s32*)((s32)mobj + 0x1A0) = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), (kind == 13) ? str_S_3_80420214 : str_S_1_8042021c, 1);
    if (evtGetValue(pEvt, swSet) != 0) {
        doneName = str_MOBJ_Block_802c24a8;
        if (kind == 0 || kind == 10) {
            animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
            *(s32*)((s32)mobj + 0x1DC) = 99;
        } else {
            if (kind == 2 || kind == 12) {
                doneName = str_MOBJ_PinkBlock_802c2620;
            }
            animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_2_804202b0, 1);
            strcpy((char*)((s32)mobj + 0x15), doneName);
            *(s32*)((s32)mobj + 0x1DC) = 0x5A;
        }
        *(u32*)mobj &= ~0x800;
    }
    return 2;
}

s32 mobj_breaking_floor(void* pMobj) {
    extern void* marioGetPtr(void);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* mobjRunEvent(void* pMobj, void* evtCode);
    extern s32 evtSetValue(void* evt, s32 var, s32 value);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern u32 animPoseGetMaterialEvtColor(s32 poseId);
    extern void mobjDelete(char* name);
    extern const char str_A_1_80420208[];
    extern u32 dat_80420220;
    s32 action;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    u8* colorByte;
    u16* hit;

    marioGetPtr();
    action = *(s32*)((s32)pMobj + 0x1DC);
    colorA = dat_80420220;
    colorB = colorA;
    colorC = colorA;
    if (action != 1) {
        if (action < 1) {
            if ((action >= 0) && ((*(u32*)pMobj & 8) != 0)) {
                psndSFXOn_3D(0x401E8, (void*)((s32)pMobj + 0x38));
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
                if (*(void**)((s32)pMobj + 0x1D4) != 0) {
                    mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D4));
                }
                hit = *(u16**)((s32)pMobj + 0x74);
                if (hit != 0) {
                    *hit |= 1;
                }
                *(u32*)pMobj |= 2;
                evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
                *(s32*)((s32)pMobj + 0x1DC) = 1;
                animPoseSetMaterialFlagOn(*(s32*)((s32)pMobj + 0x70), 0x40);
                colorA = colorB;
                animPoseSetMaterialEvtColor(*(s32*)((s32)pMobj + 0x70), &colorA);
                *(s32*)((s32)pMobj + 0x1B0) = 0x40;
            }
        } else if (action == 99) {
            mobjDelete((char*)((s32)pMobj + 5));
        }
    } else {
        colorC = animPoseGetMaterialEvtColor(*(s32*)((s32)pMobj + 0x70));
        colorByte = ((u8*)&colorC) + 3;
        if (*colorByte < 4) {
            *colorByte = 0;
            *(s32*)((s32)pMobj + 0x1DC) = 99;
        } else {
            *colorByte = *colorByte - 4;
        }
        colorA = colorC;
        animPoseSetMaterialEvtColor(*(s32*)((s32)pMobj + 0x70), &colorA);
        *(s32*)((s32)pMobj + 0x1B0) = *(s32*)((s32)pMobj + 0x1B0) - 1;
        if (*(s32*)((s32)pMobj + 0x1B0) <= 0) {
            *(s32*)((s32)pMobj + 0x1DC) = 99;
        }
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mobj_breaking_floor(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern char* strcpy(char* dst, const char* src);
    extern u32 animGroupBaseAsync(char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gp;
    extern const char str_MOBJ_BreakingFloor_802c2578[];
    extern const char str_MOBJ_BlackBreakingFl_802c258c[];
    extern const char str_S_1_8042021c[];
    extern void mobj_breaking_floor(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 a;
    f32 b;
    f32 c;
    s32 kind;
    void* evtCode;
    s32 swSet;
    void* mobj;
    char pose[136];
    s32 inBattle;

    name = (char*)evtGetValue(pEvt, args[0]);
    a = evtGetFloat(pEvt, args[1]);
    b = evtGetFloat(pEvt, args[2]);
    c = evtGetFloat(pEvt, args[3]);
    kind = evtGetValue(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    inBattle = *(s32*)((s32)gp + 0x14);
    swSet = args[6];
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (kind == 1) {
        strcpy(pose, str_MOBJ_BlackBreakingFl_802c258c);
    } else if (kind < 1) {
        if (kind > -1) {
            strcpy(pose, str_MOBJ_BreakingFloor_802c2578);
        } else {
            strcpy(pose, str_MOBJ_BreakingFloor_802c2578);
        }
    } else {
        strcpy(pose, str_MOBJ_BreakingFloor_802c2578);
    }
    if (animGroupBaseAsync(pose, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, pose);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(c, b, a, name);
    *(void**)((s32)mobj + 0x1D0) = mobj_breaking_floor;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x01000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    } else {
        *(s32*)((s32)mobj + 0x1DC) = 0;
    }
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 kururing_capture(s32 camId, void* mobj) {
    extern void* camGetPtr(s32);
    extern void* memcpy(void*, const void*, u32);
    extern void* animPoseGetAnimBaseDataPtr(s32);
    extern f64 tan(f64);
    extern void C_MTXPerspective(f32, f32, f32, f32, void*);
    extern void C_MTXLookAt(void*, void*, void*, void*);
    extern void GXSetProjection(void*, s32);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void test_kururing_mapdisp(s32);
    extern void sysWaitDrawSync(void);
    extern s32 GXGetTexBufferSize(u16, u16, s32, s32, s32);
    extern void* smartAlloc(u32, s32);
    extern void GXSetTexCopySrc(s32, s32, s32, s32);
    extern void GXSetTexCopyDst(u16, u16, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXCopyTex(void*, s32);
    extern void GXInvalidateTexAll(void);
    extern void GXPixModeSync(void);
    extern void GXTexModeSync(void);
    extern void GXInitTexObj(void*, void*, u16, u16, s32, s32, s32, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern void PSMTXIdentity(void*);
    extern char vec3_802c2058[];
    extern f32 float_0_80420240;
    extern f32 float_0p5_8042024c;
    extern f32 float_1_80420250;
    extern f32 float_10_80420294;
    extern f32 float_25_80420230;
    extern f32 float_32767_804202d4;
    u8 camCopy[0x260];
    char* base = vec3_802c2058;
    void* cam;
    void* pose;
    s32 w;
    s32 h;
    u32 width;
    u32 height;
    u16 texW;
    u16 texH;
    f32 aspect;
    f32 tanv;
    s32 size;

    cam = camGetPtr(camId);
    memcpy(camCopy, cam, 0x260);
    pose = animPoseGetAnimBaseDataPtr(*(s32*)((s32)mobj + 0x70));
    w = (s32)(float_10_80420294 * (*(f32*)((s32)pose + 0xD4) - *(f32*)((s32)pose + 0xD0)));
    h = (s32)(float_10_80420294 * (*(f32*)((s32)pose + 0xE0) - *(f32*)((s32)pose + 0xDC)));
    cam = camGetPtr(camId);
    *(u32*)((s32)cam + 0x24) = *(u32*)(base + 0x27C);
    *(u32*)((s32)cam + 0x28) = *(u32*)(base + 0x280);
    *(u32*)((s32)cam + 0x2C) = *(u32*)(base + 0x284);
    *(u32*)((s32)cam + 0x18) = *(u32*)((s32)mobj + 0x38);
    *(u32*)((s32)cam + 0x1C) = *(u32*)((s32)mobj + 0x3C);
    *(u32*)((s32)cam + 0x20) = *(u32*)((s32)mobj + 0x40);
    *(u32*)((s32)cam + 0x0C) = *(u32*)((s32)mobj + 0x38);
    *(u32*)((s32)cam + 0x10) = *(u32*)((s32)mobj + 0x3C);
    *(u32*)((s32)cam + 0x14) = *(u32*)((s32)mobj + 0x40);
    tanv = (f32)tan(*(f64*)(base + 0x510));
    height = (u32)(h + 1) & 0xFFFFFFFE;
    width = (u32)(w + 1) & 0xFFFFFFFE;
    aspect = (f32)(width / height);
    *(f32*)((s32)cam + 0x10) += (float_0p5_8042024c * (f32)height) / tanv;
    C_MTXPerspective(float_25_80420230, aspect, float_1_80420250, float_32767_804202d4, (void*)((s32)cam + 0x15C));
    *(s32*)((s32)cam + 0x19C) = 0;
    C_MTXLookAt((void*)((s32)cam + 0x11C), (void*)((s32)cam + 0x0C), (void*)((s32)cam + 0x24), (void*)((s32)cam + 0x18));
    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
    GXSetViewport(float_0_80420240, float_0_80420240, (f32)width, (f32)height, float_0_80420240, float_1_80420250);
    GXSetScissor(0, 0, width, height);
    test_kururing_mapdisp(camId);
    sysWaitDrawSync();
    texW = width >> 1;
    texH = height >> 1;
    size = GXGetTexBufferSize(texW, texH, 6, 0, 0);
    *(void**)((s32)mobj + 0x208) = smartAlloc(size, 3);
    GXSetTexCopySrc(0, 0, width, height);
    GXSetTexCopyDst(texW, texH, 6, 1);
    GXSetZMode(1, 3, 1);
    GXCopyTex(**(void***)((s32)mobj + 0x208), 1);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXInitTexObj((void*)((s32)mobj + 0x1E8), **(void***)((s32)mobj + 0x208), texW, texH, 6, 0, 0, 0);
    GXInitTexObjLOD((void*)((s32)mobj + 0x1E8), 0, 0, float_0_80420240, float_0_80420240, float_0_80420240, 0, 0, 0);
    PSMTXIdentity((void*)((s32)mobj + 0x20C));
    cam = camGetPtr(camId);
    memcpy(cam, camCopy, 0x260);
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 mobj_kururing_floor(void* pMobj) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 __fabsf(f32 x);
    extern s32 pouchGetJumpLv(void);
    extern void animPoseSetAnim(s32 poseId, const char* anim, s32 flags);
    extern u32 psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void* itemNameToPtr(char* name);
    extern void* itemEntry(char* name, s32 itemId, u32 mode, s32 switchVar, void* script, f32 x, f32 y, f32 z);
    extern void itemFlagOn(void* item, u16 flags);
    extern void* animGetPtr(void);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void dispEntry(s32 prio, s32 cameraId, void* callback, void* param, f32 order);
    extern u8 kururing_capture(s32 camId, void* pMobj);
    extern void* itemHitCheck(f32 x, f32 y, f32 z, f32 radius);
    extern void* npcNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
    extern void* itemNearDistCheck(f32 x, f32 y, f32 z, f32 radius);
    extern void* npcGetWorkPtr(void);
    extern const char str_A_1_80420208[4];
    extern const char str_A_2_8042020c[4];
    extern const char str_S_1_8042021c[4];
    extern f32 float_0_80420240;
    extern f32 float_0p6_804202d0;
    extern f32 float_1_80420250;
    extern f32 float_2_80420280;
    extern f32 float_10_80420294;
    extern f32 float_30_80420298;
    extern f32 float_42p42_804202cc;
    extern f32 float_60_8042029c;
    extern f32 float_100_80420238;
    extern f32 float_300_80420248;
    extern f32 float_1E05_804202c8;

    void* mario = marioGetPtr();
    void* party = partyGetPtr(marioGetPartyId());
    s32 action = *(s32*)((s32)pMobj + 0x1DC);
    s32 result;
    f32 dist;
    f32 dy;
    void* item;
    void* anim;
    void* base;
    void* work;
    void* npc;
    s32 i;
    s32 count;
    u32* poseFlags;

    switch (action) {
        case 0:
            if ((*(u32*)pMobj & 8) == 0) {
                break;
            }

            dist = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                           *(f32*)((s32)pMobj + 0x38), *(f32*)((s32)pMobj + 0x40));
            dy = __fabsf(*(f32*)((s32)mario + 0x90) - *(f32*)((s32)pMobj + 0x3C));
            if (dy > float_10_80420294) {
                dist = float_1E05_804202c8;
            }

            result = 0;
            if (*(u32*)((s32)pMobj + 0x1E0) == 0x01000000) {
                if (pouchGetJumpLv() < 2) {
                    result = 0;
                } else if (dist < float_60_8042029c) {
                    result = 2;
                } else if (dist < float_100_80420238) {
                    result = 1;
                } else {
                    result = 0;
                }
            }
            if (*(u32*)((s32)pMobj + 0x1E0) == 0x00010000) {
                result = 1;
            }

            if (result == 0) {
                *(u32*)pMobj &= ~8U;
                break;
            }

            if ((*(f32*)((s32)pMobj + 0x38) - float_30_80420298 <= *(f32*)((s32)mario + 0x8C)) &&
                (*(f32*)((s32)mario + 0x8C) <= *(f32*)((s32)pMobj + 0x38) + float_30_80420298) &&
                (*(f32*)((s32)pMobj + 0x40) - float_30_80420298 <= *(f32*)((s32)mario + 0x94)) &&
                (*(f32*)((s32)mario + 0x94) <= *(f32*)((s32)pMobj + 0x40) + float_30_80420298)) {
                result = 1;
            }
            if (party != 0) {
                if ((*(f32*)((s32)pMobj + 0x38) - float_30_80420298 <= *(f32*)((s32)party + 0x58)) &&
                    (*(f32*)((s32)party + 0x58) <= *(f32*)((s32)pMobj + 0x38) + float_30_80420298) &&
                    (*(f32*)((s32)pMobj + 0x40) - float_30_80420298 <= *(f32*)((s32)party + 0x60)) &&
                    (*(f32*)((s32)party + 0x60) <= *(f32*)((s32)pMobj + 0x40) + float_30_80420298)) {
                    result = 1;
                }
            }
            if (itemHitCheck(*(f32*)((s32)pMobj + 0x38), *(f32*)((s32)pMobj + 0x3C),
                             *(f32*)((s32)pMobj + 0x40), float_42p42_804202cc) != 0) {
                result = 1;
            }
            if (npcNearDistCheck(*(f32*)((s32)pMobj + 0x38), *(f32*)((s32)pMobj + 0x3C),
                                 *(f32*)((s32)pMobj + 0x40), float_42p42_804202cc) != 0) {
                result = 1;
            }

            if (result == 0) {
                *(u32*)pMobj &= ~8U;
                break;
            }

            if (result == 1) {
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_1_80420208, 1);
                psndSFXOnEx_3D(0x1EE, (void*)((s32)pMobj + 0x38), 0xFF, 0xFF, 0x64, 0x10);
                *(s32*)((s32)pMobj + 0x1DC) = 10;
            } else if (result == 2) {
                animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_A_2_8042020c, 1);
                psndSFXOn_3D(0x1EF, (void*)((s32)pMobj + 0x38));
                *(s32*)((s32)pMobj + 0x1DC) = 1;

                item = itemNameToPtr((char*)((s32)pMobj + 5));
                if ((item == 0) || ((*(u16*)item & 1) == 0)) {
                    if (itemEntry((char*)((s32)pMobj + 5), 0x7D, 4, *(s32*)((s32)pMobj + 0x1E4), 0,
                                  *(f32*)((s32)pMobj + 0x38),
                                  *(f32*)((s32)pMobj + 0x3C) + float_30_80420298,
                                  *(f32*)((s32)pMobj + 0x40)) != 0) {
                        item = itemNameToPtr((char*)((s32)pMobj + 5));
                        itemFlagOn(item, 0x10);
                        *(f32*)((s32)item + 0x80) = float_0p6_804202d0;
                        *(f32*)((s32)item + 0x84) = float_1_80420250;
                        *(s32*)((s32)item + 0x88) = 5;
                        *(f32*)((s32)item + 0x58) = float_300_80420248;
                    }
                }
            }

            anim = animGetPtr();
            poseFlags = (u32*)((s32)*(void**)((s32)anim + 0x10) + *(s32*)((s32)pMobj + 0x70) * 0x170);
            *poseFlags |= 0x10;
            *(u32*)pMobj |= 0x400;
            dispEntry(2, 0, kururing_capture, pMobj, float_0_80420240);
            break;

        case 1:
            anim = animGetPtr();
            poseFlags = (u32*)((s32)*(void**)((s32)anim + 0x10) + *(s32*)((s32)pMobj + 0x70) * 0x170);
            *poseFlags |= 0x10;
            *(u32*)pMobj |= 0x400;
            dispEntry(2, 0, kururing_capture, pMobj, float_0_80420240);

            if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) < float_1_80420250) {
                break;
            }
            anim = animGetPtr();
            poseFlags = (u32*)((s32)*(void**)((s32)anim + 0x10) + *(s32*)((s32)pMobj + 0x70) * 0x170);
            *poseFlags &= ~0x10U;
            *(u32*)pMobj &= ~0x400U;
            *(u32*)pMobj &= ~8U;
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
            psndSFXOn_3D(0x1F0, (void*)((s32)pMobj + 0x38));
            *(s32*)((s32)pMobj + 0x1DC) = 0;
            break;

        case 10:
            anim = animGetPtr();
            poseFlags = (u32*)((s32)*(void**)((s32)anim + 0x10) + *(s32*)((s32)pMobj + 0x70) * 0x170);
            *poseFlags |= 0x10;
            *(u32*)pMobj |= 0x400;
            dispEntry(2, 0, kururing_capture, pMobj, float_0_80420240);

            base = *(void**)((s32)animPoseGetAnimPosePtr(*(s32*)((s32)pMobj + 0x70)) + 0x15C);
            base = *(void**)base;

            mario = marioGetPtr();
            if ((*(f32*)((s32)pMobj + 0x38) - float_30_80420298 <= *(f32*)((s32)mario + 0x8C)) &&
                (*(f32*)((s32)mario + 0x8C) <= *(f32*)((s32)pMobj + 0x38) + float_30_80420298) &&
                (*(f32*)((s32)pMobj + 0x40) - float_30_80420298 <= *(f32*)((s32)mario + 0x94)) &&
                (*(f32*)((s32)mario + 0x94) <= *(f32*)((s32)pMobj + 0x40) + float_30_80420298)) {
                *(f32*)((s32)mario + 0x90) = *(f32*)((s32)base + 0x10);
            }

            party = partyGetPtr(marioGetPartyId());
            if ((party != 0) && ((*(u32*)party & 0x100) == 0)) {
                if ((*(f32*)((s32)pMobj + 0x38) - float_30_80420298 <= *(f32*)((s32)party + 0x58)) &&
                    (*(f32*)((s32)party + 0x58) <= *(f32*)((s32)pMobj + 0x38) + float_30_80420298) &&
                    (*(f32*)((s32)pMobj + 0x40) - float_30_80420298 <= *(f32*)((s32)party + 0x60)) &&
                    (*(f32*)((s32)party + 0x60) <= *(f32*)((s32)pMobj + 0x40) + float_30_80420298)) {
                    *(f32*)((s32)party + 0x5C) = *(f32*)((s32)base + 0x10);
                }
            }

            party = partyGetPtr(marioGetExtraPartyId());
            if ((party != 0) && ((*(u32*)party & 0x100) == 0)) {
                if ((*(f32*)((s32)pMobj + 0x38) - float_30_80420298 <= *(f32*)((s32)party + 0x58)) &&
                    (*(f32*)((s32)party + 0x58) <= *(f32*)((s32)pMobj + 0x38) + float_30_80420298) &&
                    (*(f32*)((s32)pMobj + 0x40) - float_30_80420298 <= *(f32*)((s32)party + 0x60)) &&
                    (*(f32*)((s32)party + 0x60) <= *(f32*)((s32)pMobj + 0x40) + float_30_80420298)) {
                    *(f32*)((s32)party + 0x5C) = *(f32*)((s32)base + 0x10);
                }
            }

            work = npcGetWorkPtr();
            npc = *(void**)((s32)work + 0x0C);
            count = *(s32*)((s32)work + 4);
            for (i = 0; i < count; i++) {
                if ((*(u32*)npc & 1) != 0) {
                    if ((*(f32*)((s32)pMobj + 0x38) - float_42p42_804202cc <= *(f32*)((s32)npc + 0x8C)) &&
                        (*(f32*)((s32)npc + 0x8C) <= *(f32*)((s32)pMobj + 0x38) + float_42p42_804202cc) &&
                        (*(f32*)((s32)pMobj + 0x40) - float_42p42_804202cc <= *(f32*)((s32)npc + 0x94)) &&
                        (*(f32*)((s32)npc + 0x94) <= *(f32*)((s32)pMobj + 0x40) + float_42p42_804202cc)) {
                        *(f32*)((s32)npc + 0x90) = *(f32*)((s32)base + 0x10);
                    }
                }
                npc = (void*)((s32)npc + 0x340);
            }

            item = itemNearDistCheck(*(f32*)((s32)pMobj + 0x38), *(f32*)((s32)pMobj + 0x3C),
                                     *(f32*)((s32)pMobj + 0x40), float_42p42_804202cc);
            if (item != 0) {
                *(f32*)((s32)item + 0x40) = *(f32*)((s32)base + 0x10);
            }

            if (animPoseGetLoopTimes(*(s32*)((s32)pMobj + 0x70)) < float_2_80420280) {
                break;
            }
            anim = animGetPtr();
            poseFlags = (u32*)((s32)*(void**)((s32)anim + 0x10) + *(s32*)((s32)pMobj + 0x70) * 0x170);
            *poseFlags &= ~0x10U;
            *(u32*)pMobj &= ~0x400U;
            animPoseSetAnim(*(s32*)((s32)pMobj + 0x70), str_S_1_8042021c, 1);
            psndSFXOn_3D(0x1F0, (void*)((s32)pMobj + 0x38));
            *(s32*)((s32)pMobj + 0x1B0) = 20;
            *(s32*)((s32)pMobj + 0x1DC) += 1;
            break;

        case 11:
            *(s32*)((s32)pMobj + 0x1B0) -= 1;
            if (*(s32*)((s32)pMobj + 0x1B0) == 0) {
                *(s32*)((s32)pMobj + 0x1DC) += 1;
            }
            break;

        case 12:
            *(u32*)pMobj &= ~8U;
            *(s32*)((s32)pMobj + 0x1DC) = 0;
            break;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 evt_mobj_kururing_floor(s32 pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void mobjCalcMtx2(void* mobj);
    extern void mapGrpFlagOn(char* name, u32 flags);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void animPoseWorldPositionEvalOn(s32 poseId, u8 value);
    extern void* gp;
    extern const u32 vec3_802c2058[3];
    extern f32 float_0p1_804202c4;
    extern void mobj_kururing_floor(void);
    s32* args;
    const u32* base;
    char* name;
    char* group;
    f32 x;
    f32 y;
    f32 z;
    s32 swSet;
    void* mobj;
    s32 inBattle;
    u8 evalArg;
    u32 pos[3];
    u32 scale[3];

    args = *(s32**)(pEvt + 0x18);
    base = vec3_802c2058;
    name = (char*)evtGetValue((void*)pEvt, args[0]);
    x = evtGetFloat((void*)pEvt, args[1]);
    y = evtGetFloat((void*)pEvt, args[2]);
    z = evtGetFloat((void*)pEvt, args[3]);
    group = (char*)evtGetValue((void*)pEvt, args[4]);
    evtGetValue((void*)pEvt, args[5]);
    pEvt = (s32)((s32)base + 0x4FC);
    swSet = args[6];
    inBattle = *(s32*)((s32)gp + 0x14);
    inBattle = ((u32)(-inBattle) | (u32)inBattle) >> 31;
    if (animGroupBaseAsync((char*)pEvt, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, (char*)pEvt);
    mobj = mobjNameToPtr(name);
    pos[0] = base[0];
    pos[1] = base[1];
    pos[2] = base[2];
    *(f32*)&pos[0] = x;
    *(f32*)&pos[1] = float_0p1_804202c4 + y;
    *(f32*)&pos[2] = z;
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(void**)((s32)mobj + 0x1D0) = mobj_kururing_floor;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(u32*)mobj |= 0x01010000;
    *(u32*)mobj |= 2;
    mobjCalcMtx(mobj);
    scale[1] = *(u32*)((s32)base + 0x298);
    scale[0] = *(u32*)((s32)base + 0x294);
    *(f32*)&scale[1] = *(f32*)((s32)mobj + 0x188);
    scale[2] = *(u32*)((s32)base + 0x29C);
    *(u32*)((s32)mobj + 0x184) = scale[0];
    *(u32*)((s32)mobj + 0x188) = scale[1];
    *(u32*)((s32)mobj + 0x18C) = scale[2];
    mobjCalcMtx2(mobj);
    mapGrpFlagOn(group, 0x400000);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), (char*)((s32)base - 0x3C), 1);
    animPoseWorldPositionEvalOn(*(s32*)((s32)mobj + 0x70), evalArg);
    return 2;
}

s32 kpa_powerup(void* evt) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 evtGetFloat(void* evt, s32 value);
    extern void kpaAddScorePos(s32 score, VecLocal* pos);
    extern s32 kpaGetLevel(void);
    extern void kpaAddScore(s32 score);
    extern void kpaPowUp(void);
    s32* args = *(s32**)((s32)evt + 0x18);
    VecLocal pos;

    pos.x = evtGetFloat(evt, args[0]);
    pos.y = evtGetFloat(evt, args[1]);
    pos.z = evtGetFloat(evt, args[2]);
    kpaAddScorePos(500, &pos);
    if (kpaGetLevel() == 3) {
        kpaAddScore(1000);
    }
    kpaPowUp();
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 mobj_koopa_ojama_blk_evt(u32* mobj) {
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void effMObjBrokenEntry(double x, double y, double z, s32 type);
    extern void* itemEntry(char* name, s32 id, u32 mode, s32 collected, void* script, f32 x, f32 y, f32 z);
    extern void itemStatusOn(void* item, u32 flags);
    extern void itemFlagOn(void* item, u16 flags);
    extern void* mobjRunEvent(void* pMobj, void* evtCode);
    extern s32 evtSetValue(void* evt, s32 var, s32 value);
    extern s32 npcKoopaModeMobjBoundDeadCheck(void* hit);
    extern s32 kpaGetLevel(void);
    extern s32 kpaMutekiCheck(void);
    extern void kpaAddScore(s32 score);
    extern void mobjDelete(char* name);
    extern void* kpa_powerup_evt;
    extern f32 float_25_80420230;
    extern f32 float_0p45_804202b4;
    extern f32 float_0p7_804202b8;
    extern f32 float_500_804202bc;
    extern f32 float_120_804202c0;
    s32 action;
    s32 ok;
    u32 hitKind;
    u16* hit;
    void* item;

    ok = 0;
    action = mobj[0x77];
    if (action != 10) {
        if (action < 10) {
            if ((action == 0) && ((*mobj & 8) != 0)) {
                action = kpaGetLevel();
                if (action == 2) {
                    hitKind = mobj[0x78];
                    if ((hitKind == 0x20000000) || (hitKind == 0x10000000) || (hitKind == 0x04000000)) {
                        ok = 1;
                    }
                } else if (action < 2) {
                    if (action >= 0) {
                        hitKind = mobj[0x78];
                        if ((hitKind == 0x20000000) || (hitKind == 0x04000000)) {
                            ok = 1;
                        }
                    }
                } else if (action < 4) {
                    ok = 1;
                }
                if (ok == 0) {
                    *mobj &= ~8;
                } else {
                    mobjRunEvent(mobj, (void*)mobj[0x75]);
                    evtSetValue(0, mobj[0x79], 1);
                    if (mobj[0x78] == 0x04000000) {
                        npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
                    }
                    hit = (u16*)mobj[0x1D];
                    if (hit != 0) {
                        *hit |= 1;
                    }
                    *mobj |= 2;
                    if (kpaMutekiCheck() != 0) {
                        kpaAddScore(500);
                    } else {
                        kpaAddScore(400);
                    }
                    mobj[0x77] = 10;
                }
            }
        } else if (action == 99) {
            mobjDelete((char*)((s32)mobj + 5));
        }
    } else {
        psndSFXOn_3D(0x1C4, (void*)((s32)mobj + 0x38));
        effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38),
                           float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                           float_25_80420230 + *(f32*)((s32)mobj + 0x40), 5);
        item = itemEntry((char*)((s32)mobj + 5), 0x59, 4, -1, kpa_powerup_evt,
                         *(f32*)((s32)mobj + 0x38),
                         float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                         *(f32*)((s32)mobj + 0x40));
        if (item != 0) {
            itemStatusOn(item, 0x10);
            itemFlagOn(item, 0x14);
            *(s32*)((s32)item + 0x7C) = 10000;
            *(s32*)((s32)item + 0x78) = 0;
            *(f32*)((s32)item + 0x80) = float_0p45_804202b4;
            *(f32*)((s32)item + 0x84) = float_0p7_804202b8;
            *(s32*)((s32)item + 0x88) = 0;
            *(f32*)((s32)item + 0x58) = float_500_804202bc;
            *(f32*)((s32)item + 0x50) = float_120_804202c0;
            *(s32*)((s32)item + 0x54) = 0x5A;
            psndSFXOn_3D(0x836, (void*)((s32)mobj + 0x38));
        }
        mobj[0x77] = 99;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_mobj_koopa_ojama_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void* gpGlobals;
    extern void mobj_koopa_ojama_blk_evt(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    s32 swSet;
    char* poseName;
    void* mobj;
    s32 inBattle;

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    evtCode = (void*)evtGetValue(pEvt, args[4]);
    swSet = args[5];
    poseName = (char*)evtGetValue(pEvt, args[6]);
    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;

    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 4;
    *(f32*)((s32)mobj + 0x38) = x;
    *(f32*)((s32)mobj + 0x3C) = y;
    *(f32*)((s32)mobj + 0x40) = z;
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_ojama_blk_evt;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = 0;
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    return 2;
}

s32 mobj_koopa_stone_blk_evt(void) {
    return 0;
}

s32 evt_mobj_koopa_stone_blk(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void* mobjNameToPtr(s32 name);
    extern s32* gp;
    extern const u32 vec3_802c233c[3];
    extern s32 mobj_koopa_stone_blk_evt(void);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 eventCode = evtGetValue(evt, args[4]);
    s32 script = args[5];
    s32 kind = evtGetValue(evt, args[6]);
    void* mobj;
    u32 pos[3];

    if (animGroupBaseAsync((const char*)kind, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }

    mobjEntry(name, (const char*)kind);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 4;
    pos[0] = vec3_802c233c[0];
    pos[1] = vec3_802c233c[1];
    pos[2] = vec3_802c233c[2];
    *(f32*)&pos[0] = x;
    *(f32*)&pos[1] = y;
    *(f32*)&pos[2] = z;
    *(u32*)((s32)mobj + 0x38) = pos[0];
    *(u32*)((s32)mobj + 0x3C) = pos[1];
    *(u32*)((s32)mobj + 0x40) = pos[2];
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_stone_blk_evt;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(s32*)((s32)mobj + 0x1D8) = 0;
    *(s32*)((s32)mobj + 0x1E4) = script;
    *(s32*)((s32)mobj + 0x1DC) = 99;
    return 2;
}

s32 mobj_koopa_brickblk(u32* mobj) {
    extern s32 kpaGetLevel(void);
    extern s32 npcKoopaModeMobjBoundDeadCheck(void*);
    extern void* mobjRunEvent(void*, void*);
    extern void evtSetValue(void*, s32, s32);
    extern s32 kpaMutekiCheck(void);
    extern void kpaAddScore(s32);
    extern void psndSFXOn_3D(s32, void*);
    extern s32 strcmp(const char*, const char*);
    extern void effMObjBrokenEntry(double, double, double, s32);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern double animPoseGetLoopTimes(s32);
    extern void mobjDelete(char*);
    extern const char str_MOBJ_FlowBrickBlock_802c24e0[];
    extern const char str_MOBJ_block_ky_802c24f4[];
    extern const char str_MOBJ_bigblock_01_802c2504[];
    extern const char str_MOBJ_bigblock_02_802c2518[];
    extern const char str_A_1_80420208[];
    extern const char str_S_1_8042021c[];
    extern f32 float_1_80420250;
    extern f32 float_12p5_80420268;
    extern f32 float_25_80420230;
    s32 level;
    s32 score;
    s32 valid;
    u32 hitKind;

    switch (mobj[0x77]) {
        case 0:
            if ((*mobj & 8) == 0) {
                break;
            }
            valid = 0;
            level = kpaGetLevel();
            hitKind = mobj[0x78];
            if (level == 2) {
                if (hitKind == 0x20000000 || hitKind == 0x10000000 ||
                    hitKind == 0x04000000) {
                    valid = 1;
                }
            } else if (level < 2) {
                if (level == 0) {
                    if (hitKind == 0x20000000) {
                        valid = 1;
                    }
                } else if (level > -1 &&
                           (hitKind == 0x20000000 || hitKind == 0x04000000)) {
                    valid = 1;
                }
            } else if (level < 4) {
                valid = 1;
            }

            if (valid != 0) {
                mobjRunEvent(mobj, (void*)mobj[0x75]);
                evtSetValue(0, mobj[0x79], 1);
                if (mobj[0x78] == 0x04000000) {
                    npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
                }
                if ((void*)mobj[0x1D] != 0) {
                    *(u16*)mobj[0x1D] |= 1;
                }
                *mobj |= 2;
                score = 400;
                if (kpaMutekiCheck() != 0) {
                    score = 500;
                }
                kpaAddScore(score);
                mobj[0x77] = 10;
            } else if (mobj[0x78] == 0x04000000) {
                npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
                mobj[0x77] = 20;
            } else {
                *mobj &= ~8;
            }
            break;

        case 10:
            psndSFXOn_3D(0x1B8, mobj + 0xE);
            if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_FlowBrickBlock_802c24e0) == 0) {
                effMObjBrokenEntry((double)(f32)mobj[0xE],
                                   (double)(float_12p5_80420268 + (f32)mobj[0xF]),
                                   (double)(float_12p5_80420268 + (f32)mobj[0x10]), 0);
            } else if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_block_ky_802c24f4) == 0) {
                effMObjBrokenEntry((double)(f32)mobj[0xE],
                                   (double)(float_12p5_80420268 + (f32)mobj[0xF]),
                                   (double)(float_12p5_80420268 + (f32)mobj[0x10]), 1);
            } else if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_bigblock_01_802c2504) == 0) {
                effMObjBrokenEntry((double)(f32)mobj[0xE],
                                   (double)(float_25_80420230 + (f32)mobj[0xF]),
                                   (double)(float_25_80420230 + (f32)mobj[0x10]), 3);
            } else if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_bigblock_02_802c2518) == 0) {
                effMObjBrokenEntry((double)(f32)mobj[0xE],
                                   (double)(float_25_80420230 + (f32)mobj[0xF]),
                                   (double)(float_25_80420230 + (f32)mobj[0x10]), 4);
            }
            mobj[0x77] = 99;
            break;

        case 20:
            psndSFXOn_3D(0x1B7, mobj + 0xE);
            animPoseSetAnim(mobj[0x1C], str_A_1_80420208, 1);
            mobj[0x77] = 21;
            break;

        case 21:
            if ((*mobj & 8) == 0) {
                mobj[0x77] = 0;
            } else if (animPoseGetLoopTimes(mobj[0x1C]) >= (double)float_1_80420250) {
                animPoseSetAnim(mobj[0x1C], str_S_1_8042021c, 1);
                mobj[0x77] = 0;
                *mobj &= ~8;
            }
            break;

        case 99:
            mobjDelete((char*)((s32)mobj + 5));
            break;
    }
    return 0;
}

s32 evt_mobj_koopa_brick(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, s32 name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gpGlobals;
    extern const char str_MOBJ_FlowBrickBlock_802c24e0[];
    extern const char str_MOBJ_block_ky_802c24f4[];
    extern const char str_MOBJ_bigblock_01_802c2504[];
    extern const char str_MOBJ_bigblock_02_802c2518[];
    extern const char str_S_1_8042021c[];
    extern void mobj_koopa_brickblk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 item;
    s32 kind;
    void* evtCode;
    s32 swSet;
    const char* poseName;
    void* mobj;
    s32 inBattle;

    name = (char*)evtGetValue(pEvt, args[0]);
    y = evtGetFloat(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    item = evtGetValue(pEvt, args[4]);
    kind = args[5];
    evtCode = (void*)evtGetValue(pEvt, args[6]);
    swSet = args[7];

    if (kind == 2) {
        poseName = str_MOBJ_bigblock_01_802c2504;
    } else if (kind < 2) {
        if (kind == 0) {
            poseName = str_MOBJ_FlowBrickBlock_802c24e0;
        } else if (kind > -1) {
            poseName = str_MOBJ_block_ky_802c24f4;
        }
    } else {
        poseName = str_MOBJ_bigblock_02_802c2518;
    }

    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, (s32)name);
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_brickblk;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(s32*)((s32)mobj + 0x1A0) = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    if (evtGetValue(pEvt, swSet) != 0) {
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    return 2;
}

s32 mobj_koopa_badgeblk(u32* mobj) {
    extern void mobjHitEntry(void*, s32);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern double animPoseGetLoopTimes(s32);
    extern char* strcpy(char*, const char*);
    extern void* mobjRunEvent(void*, void*);
    extern void* itemEntry(char*, s32, s32, s32, void*, f32, f32, f32);
    extern void* itemNameToPtr(char*);
    extern void itemStatusOn(void*, s32);
    extern void itemFlagOn(void*, s32);
    extern void kpaAddCoin(s32);
    extern void evtSetValue(void*, s32, s32);
    extern void psndSFXOn_3D(s32, void*);
    extern s32 npcKoopaModeMobjBoundDeadCheck(void*);
    extern s32 kpaGetLevel(void);
    extern void effMObjBrokenEntry(double, double, double, s32);
    extern void mobjDelete(char*);
    extern void* kpa_powerup_evt;
    extern const char str_A_1_80420208[];
    extern const char str_S_2_804202b0[];
    extern const char str_MOBJ_Block_802c24a8[];
    extern f32 float_1_80420250;
    extern f32 float_12p5_80420268;
    extern f32 float_0p45_804202b4;
    extern f32 float_0p7_804202b8;
    extern f32 float_500_804202bc;
    extern f32 float_120_804202c0;
    void* item;
    u32 action = mobj[0x77];

    if (action == 0) {
        if ((*mobj & 8) != 0) {
            if (mobj[0x78] == 0x4000000) {
                if ((*mobj & 0x800) != 0) {
                    mobjHitEntry(mobj, 0);
                }
                animPoseSetAnim(mobj[0x1C], str_A_1_80420208, 1);
                if ((void*)mobj[0x75] == 0) {
                    if (mobj[0x68] == 0x59) {
                        itemEntry((char*)((s32)mobj + 5), 0x59, 4, -1, &kpa_powerup_evt,
                                  (f32)mobj[0xE], float_12p5_80420268 + (f32)mobj[0xF], (f32)mobj[0x10]);
                        item = itemNameToPtr((char*)((s32)mobj + 5));
                        itemStatusOn(item, 0x10);
                        itemFlagOn(item, 0x14);
                        *(s32*)((s32)item + 0x48) = 0;
                        *(s32*)((s32)item + 0x4C) = 10000;
                        *(f32*)((s32)item + 0x34) = float_0p45_804202b4;
                        *(f32*)((s32)item + 0x38) = float_0p7_804202b8;
                        *(f32*)((s32)item + 0x3C) = 0.0f;
                        *(f32*)((s32)item + 0x40) = float_500_804202bc;
                        *(f32*)((s32)item + 0x50) = float_120_804202c0;
                        *(s32*)((s32)item + 0x54) = 0x5A;
                        psndSFXOn_3D(0x836, mobj + 0xE);
                    } else {
                        kpaAddCoin(1);
                        itemEntry(0, 0x79, 0xF, -1, 0, (f32)mobj[0xE], (f32)mobj[0xF], (f32)mobj[0x10]);
                        evtSetValue(0, mobj[0x79], 1);
                        psndSFXOn_3D(0x5B, mobj + 0xE);
                    }
                } else {
                    mobjRunEvent(mobj, (void*)mobj[0x75]);
                }
                npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
                mobj[0x77]++;
            } else {
                *mobj &= ~8;
            }
        }
    } else if (action == 1) {
        if (animPoseGetLoopTimes(mobj[0x1C]) >= (double)float_1_80420250) {
            animPoseSetAnim(mobj[0x1C], str_S_2_804202b0, 1);
            *mobj &= ~8;
            mobj[0x77] = 0x5A;
            strcpy((char*)((s32)mobj + 0x15), str_MOBJ_Block_802c24a8);
        }
    } else if (action == 0x5A) {
        if ((*mobj & 8) != 0) {
            if (kpaGetLevel() == 3) {
                if (mobj[0x78] == 0x4000000) {
                    npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
                }
                if ((void*)mobj[0x1D] != 0) {
                    *(u16*)mobj[0x1D] |= 1;
                }
                *mobj |= 2;
                mobj[0x77]++;
            } else {
                *mobj &= ~8;
            }
        }
    } else if (action == 99) {
        mobjDelete((char*)((s32)mobj + 5));
    } else if ((s32)action < 0x5C) {
        psndSFXOn_3D(0x1B8, mobj + 0xE);
        effMObjBrokenEntry((double)(f32)mobj[0xE], (double)(float_12p5_80420268 + (f32)mobj[0xF]),
                           (double)(float_12p5_80420268 + (f32)mobj[0x10]), 2);
        mobj[0x77] = 99;
    }
    return 0;
}

s32 evt_mobj_koopa_badgeblk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, s32 name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gpGlobals;
    extern const char str_MOBJ_HatenaBlock_802c24cc[];
    extern const char str_S_1_8042021c[];
    extern const char str_S_2_804202b0[];
    extern void mobj_koopa_badgeblk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 item;
    void* evtCode;
    s32 swSet;
    void* mobj;
    s32 inBattle;

    name = (char*)evtGetValue(pEvt, args[0]);
    y = evtGetFloat(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    item = evtGetValue(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    swSet = args[6];
    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;

    if (animGroupBaseAsync(str_MOBJ_HatenaBlock_802c24cc, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_HatenaBlock_802c24cc);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, (s32)name);
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_badgeblk;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(s32*)((s32)mobj + 0x1A0) = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 0x5A;
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_2_804202b0, 1);
    }
    return 2;
}

s32 evt_mobj_koopa_hidden_blk(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjSetPosition(f32 x, f32 y, f32 z, s32 name);
    extern void mobjHitEntry(void* mobj, s32 type);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gpGlobals;
    extern const char str_MOBJ_HiddenHatenaBlo_802c24b4[];
    extern const char str_S_1_8042021c[];
    extern const char str_S_2_804202b0[];
    extern void mobj_koopa_badgeblk(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    s32 item;
    void* evtCode;
    s32 swSet;
    void* mobj;
    s32 inBattle;

    name = (char*)evtGetValue(pEvt, args[0]);
    y = evtGetFloat(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    item = evtGetValue(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[6]);
    swSet = args[7];
    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;

    if (animGroupBaseAsync(str_MOBJ_HiddenHatenaBlo_802c24b4, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_HiddenHatenaBlo_802c24b4);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(x, y, z, (s32)name);
    *(u32*)mobj |= 0x800;
    mobjHitEntry(mobj, 1);
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_badgeblk;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    *(s32*)((s32)mobj + 0x1A0) = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 0x5A;
        animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_2_804202b0, 1);
    }
    return 2;
}

s32 mobj_koopa_blk(u32* mobj) {
    extern s32 kpaGetLevel(void);
    extern void* mobjRunEvent(void*, void*);
    extern void evtSetValue(void*, s32, s32);
    extern s32 npcKoopaModeMobjBoundDeadCheck(void*);
    extern void psndSFXOn_3D(s32, void*);
    extern void effMObjBrokenEntry(double, double, double, s32);
    extern void mobjDelete(char*);
    extern f32 float_12p5_80420268;
    u16* hit;

    switch (mobj[0x77]) {
        case 0:
            if ((*mobj & 8) == 0) {
                break;
            }
            if (kpaGetLevel() != 3) {
                *mobj &= ~8;
                break;
            }
            mobjRunEvent(mobj, (void*)mobj[0x75]);
            evtSetValue(0, mobj[0x79], 1);
            if (mobj[0x78] == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
            }
            hit = (u16*)mobj[0x1D];
            if (hit != 0) {
                *hit |= 1;
            }
            *mobj |= 2;
            mobj[0x77] = 10;
            break;
        case 10:
            psndSFXOn_3D(0x1B8, mobj + 0xE);
            effMObjBrokenEntry((double)(f32)mobj[0xE],
                               (double)(float_12p5_80420268 + (f32)mobj[0xF]),
                               (double)(float_12p5_80420268 + (f32)mobj[0x10]), 2);
            mobj[0x77] = 99;
            break;
        case 99:
            mobjDelete((char*)((s32)mobj + 5));
            break;
    }
    return 0;
}

s32 evt_mobj_koopa_blk(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 animGroupBaseAsync(const char* name, s32 mode, s32 flags);
    extern void mobjEntry(s32 name, const char* kind);
    extern void mobjSetPosition(s32 name, f32 x, f32 y, f32 z);
    extern void* mobjNameToPtr(s32 name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern s32* gp;
    extern const char str_MOBJ_Block_802c24a8[];
    extern const char str_S_1_8042021c[];
    extern s32 mobj_koopa_blk(void* mobj);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 eventCode = evtGetValue(evt, args[5]);
    s32 script = args[6];
    void* mobj;

    if (animGroupBaseAsync(str_MOBJ_Block_802c24a8, ((u32)(-gp[5]) | (u32)gp[5]) >> 31, 0) == 0) {
        return 0;
    }

    mobjEntry(name, str_MOBJ_Block_802c24a8);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_blk;
    *(s32*)((s32)mobj + 0x1D4) = eventCode;
    *(u32*)mobj |= 4;
    *(u32*)mobj |= 0x3C0000;
    *(s32*)((s32)mobj + 0x1E4) = script;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    return 2;
}

s32 mobj_koopa_dokan_evt(u32* mobj) {
    extern s32 strcmp(const char* a, const char* b);
    extern void effMObjBrokenEntry(double x, double y, double z, s32 type);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void* mobjRunEvent(void* pMobj, void* evtCode);
    extern s32 evtSetValue(void* evt, s32 var, s32 value);
    extern s32 npcKoopaModeMobjBoundDeadCheck(void* hit);
    extern s32 kpaMutekiCheck(void);
    extern void kpaAddScore(s32 score);
    extern void mobjDelete(s32 name);
    extern s32 kpaGetLevel(void);
    extern const char str_MOBJ_GreenPipe_ks_802c246c[];
    extern const char str_MOBJ_GreenPipe_kl_802c2480[];
    extern f32 float_25_80420230;
    extern f32 float_7p5_804202ac;
    s32 level;
    s32 valid = 0;
    u16* hit;

    switch (mobj[0x77]) {
        case 0:
            if ((*mobj & 8) == 0) {
                break;
            }
            level = kpaGetLevel();
            if (level == 3) {
                valid = 1;
            } else if (level < 3 && level != 0 && level > -1 &&
                       mobj[0x78] == 0x20000000) {
                valid = 1;
            }
            if (valid == 0) {
                *mobj &= ~8;
                break;
            }
            mobjRunEvent(mobj, (void*)mobj[0x75]);
            evtSetValue(0, mobj[0x79], 1);
            if (mobj[0x78] == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
            }
            hit = (u16*)mobj[0x1D];
            if (hit != 0) {
                *hit |= 1;
            }
            *mobj |= 2;
            kpaAddScore(kpaMutekiCheck() != 0 ? 500 : 400);
            mobj[0x77] = 10;
            break;

        case 10:
            if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_GreenPipe_ks_802c246c) == 0) {
                effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38),
                                   float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                                   float_25_80420230 + *(f32*)((s32)mobj + 0x40), 6);
            } else if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_GreenPipe_kl_802c2480) == 0) {
                effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38),
                                   float_7p5_804202ac + float_25_80420230 +
                                       *(f32*)((s32)mobj + 0x3C),
                                   float_25_80420230 + *(f32*)((s32)mobj + 0x40), 6);
            } else {
                effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38) - float_25_80420230,
                                   float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                                   float_25_80420230 + *(f32*)((s32)mobj + 0x40), 6);
            }
            psndSFXOn_3D(0x834, (void*)((s32)mobj + 0x38));
            mobj[0x77] = 99;
            break;

        case 99:
            mobjDelete((s32)mobj + 5);
            break;
    }
    return 0;
}

s32 evt_mobj_koopa_dokan(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void mobjCalcMtx(void* mobj);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern void* gpGlobals;
    extern const char str_MOBJ_GreenPipe_ks_802c246c[];
    extern const char str_MOBJ_GreenPipe_kl_802c2480[];
    extern const char str_MOBJ_GreenPipe_ky_802c2494[];
    extern const char str_S_1_8042021c[];
    extern void mobj_koopa_dokan_evt(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 kind;
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    s32 swSet;
    const char* poseName;
    void* mobj;
    s32 inBattle;

    kind = evtGetValue(pEvt, args[0]);
    name = (char*)evtGetValue(pEvt, args[1]);
    x = evtGetFloat(pEvt, args[2]);
    y = evtGetFloat(pEvt, args[3]);
    z = evtGetFloat(pEvt, args[4]);
    evtCode = (void*)evtGetValue(pEvt, args[5]);
    swSet = args[6];

    if (kind == 1) {
        poseName = str_MOBJ_GreenPipe_kl_802c2480;
    } else if (kind < 1) {
        if (kind > -1) {
            poseName = str_MOBJ_GreenPipe_ks_802c246c;
        }
    } else if (kind < 3) {
        poseName = str_MOBJ_GreenPipe_ky_802c2494;
    }

    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;
    if (animGroupBaseAsync(poseName, inBattle, 0) == 0) {
        return 0;
    }

    mobjEntry(name, poseName);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 6;
    *(f32*)((s32)mobj + 0x38) = x;
    *(f32*)((s32)mobj + 0x3C) = y;
    *(f32*)((s32)mobj + 0x40) = z;
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_dokan_evt;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(*(s32*)((s32)mobj + 0x70), str_S_1_8042021c, 1);
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    return 2;
}

s32 fire_func(void* evt, s32 init) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern s32 irand(s32 max);
    extern void* effFireEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 arg);
    extern f32 float_1_80420250;
    extern f32 float_10_80420294;
    extern f32 float_0p8_804202a0;
    extern f32 float_0p125_804202a4;
    extern f32 float_0p99_804202a8;
    s32* args = *(s32**)((s32)evt + 0x18);
    f32 x = evtGetFloat(evt, args[0]);
    f32 y = evtGetFloat(evt, args[1]);
    f32 z = evtGetFloat(evt, args[2]);
    s32 arg = evtGetValue(evt, args[3]);
    void* fire;
    void* inner;
    f32 sx;
    f32 sy;

    if (init != 0) {
        *(void**)((s32)evt + 0x78) = effFireEntry(5, x, y, z, float_0p8_804202a0 + ((f32)irand(10) / float_10_80420294), arg);
    }

    fire = *(void**)((s32)evt + 0x78);
    sx = *(f32*)((s32)evt + 0x7C) + ((float_1_80420250 - *(f32*)((s32)evt + 0x7C)) * float_0p125_804202a4);
    sy = *(f32*)((s32)evt + 0x80) + ((float_1_80420250 - *(f32*)((s32)evt + 0x80)) * float_0p125_804202a4);
    inner = *(void**)((s32)fire + 0xC);
    *(f32*)((s32)inner + 0x78) = sx;
    inner = *(void**)((s32)fire + 0xC);
    *(f32*)((s32)inner + 0x7C) = sy;

    if (sx >= float_0p99_804202a8) {
        inner = *(void**)((s32)fire + 0xC);
        *(f32*)((s32)inner + 0x78) = float_1_80420250;
        inner = *(void**)((s32)fire + 0xC);
        *(f32*)((s32)inner + 0x7C) = float_1_80420250;
        return 2;
    }

    *(f32*)((s32)evt + 0x7C) = sx;
    *(f32*)((s32)evt + 0x80) = sy;
    return 0;
}

s32 mobj_koopa_sango_evt(u32* mobj) {
    extern void* evtEntry(void*, s32, u32);
    extern void* mobjRunEvent(void*, void*);
    extern void evtSetValue(void*, s32, s32);
    extern s32 npcKoopaModeMobjBoundDeadCheck(void*);
    extern s32 kpaGetLevel(void);
    extern s32 kpaMutekiCheck(void);
    extern void kpaAddScore(s32);
    extern void psndSFXOn_3D(s32, void*);
    extern void effMObjBrokenEntry(double, double, double, s32);
    extern void mobjDelete(char*);
    extern void* evt_fire;
    extern f32 float_10_80420294;
    extern f32 float_30_80420298;
    extern f32 float_60_8042029c;
    extern f32 float_1024_80420290;
    void* evt;
    s32 level;
    s32 score;
    u32 action = mobj[0x77];

    if (action == 0) {
        if ((*mobj & 8) == 0) {
            return 0;
        }
        level = kpaGetLevel();
        if (!(level == 3 || (level > -1 && level < 3 && mobj[0x78] == 0x20000000))) {
            *mobj &= ~8;
            return 0;
        }
        mobjRunEvent(mobj, (void*)mobj[0x75]);
        evtSetValue(0, mobj[0x79], 1);
        if (mobj[0x78] == 0x4000000) {
            npcKoopaModeMobjBoundDeadCheck((void*)mobj[0x1D]);
        }
        if ((void*)mobj[0x1D] != 0) {
            *(u16*)mobj[0x1D] |= 1;
        }
        *mobj |= 2;
        score = kpaMutekiCheck() != 0 ? 500 : 400;
        kpaAddScore(score);
        mobj[0x77] = 10;
        return 0;
    }
    if (action == 10) {
        if (mobj[0x78] == 0x20000000) {
            psndSFXOn_3D(0xB67, mobj + 0xE);
        }
        evt = evtEntry(&evt_fire, 0, 0);
        *(s32*)((s32)evt + 0x15C) = (s32)(float_1024_80420290 * (f32)mobj[0xE]) - 230000000;
        *(s32*)((s32)evt + 0x160) = (s32)(float_1024_80420290 * (f32)mobj[0xF]) - 230000000;
        *(s32*)((s32)evt + 0x164) = (s32)(float_1024_80420290 * (float_10_80420294 + (f32)mobj[0x10])) - 230000000;
        *(s32*)((s32)evt + 0x168) = 0x5A;
        mobj[0x77]++;
        return 0;
    }
    if (action == 0x14) {
        evt = evtEntry(&evt_fire, 0, 0);
        *(s32*)((s32)evt + 0x15C) = (s32)(float_1024_80420290 * (float_10_80420294 + (f32)mobj[0xE])) - 230000000;
        *(s32*)((s32)evt + 0x160) = (s32)(float_1024_80420290 * (float_30_80420298 + (f32)mobj[0xF])) - 230000000;
        *(s32*)((s32)evt + 0x164) = (s32)(float_1024_80420290 * (float_10_80420294 + (f32)mobj[0x10])) - 230000000;
        *(s32*)((s32)evt + 0x168) = 0x50;
        mobj[0x77]++;
        return 0;
    }
    if (action == 0x1E) {
        evt = evtEntry(&evt_fire, 0, 0);
        *(s32*)((s32)evt + 0x15C) = (s32)(float_1024_80420290 * ((f32)mobj[0xE] - float_10_80420294)) - 230000000;
        *(s32*)((s32)evt + 0x160) = (s32)(float_1024_80420290 * (float_60_8042029c + (f32)mobj[0xF])) - 230000000;
        *(s32*)((s32)evt + 0x164) = (s32)(float_1024_80420290 * (float_10_80420294 + (f32)mobj[0x10])) - 230000000;
        *(s32*)((s32)evt + 0x168) = 0x46;
        mobj[0x77]++;
        return 0;
    }
    if (action == 0x62) {
        effMObjBrokenEntry((double)(f32)mobj[0xE], (double)(f32)mobj[0xF], (double)(float_10_80420294 + (f32)mobj[0x10]), 7);
        mobj[0x77]++;
        return 0;
    }
    if ((s32)action < 100 && (s32)action >= 0) {
        mobj[0x77]++;
        return 0;
    }
    mobjDelete((char*)((s32)mobj + 5));
    return 0;
}
s32 evt_mobj_koopa_sango(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern void* gpGlobals;
    extern const char str_MOBJ_sango_802c2460[];
    extern void mobj_koopa_sango_evt(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    s32 swSet;
    s32 inBattle;
    void* mobj;

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    evtCode = (void*)evtGetValue(pEvt, args[4]);
    swSet = args[5];
    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;

    if (animGroupBaseAsync(str_MOBJ_sango_802c2460, inBattle, 0) == 0) {
        return 0;
    }

    mobjEntry(name, str_MOBJ_sango_802c2460);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 0x84;
    *(f32*)((s32)mobj + 0x38) = x;
    *(f32*)((s32)mobj + 0x3C) = y;
    *(f32*)((s32)mobj + 0x40) = z;
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_sango_evt;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = 0;
    *(u32*)mobj |= 0x3C000000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue(pEvt, swSet) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    return 2;
}

u8 mobj_koopa_fireber_dodai_evt(void* mobj) {
    extern void* marioGetPtr(void);
    extern double distABf(double, double, double, double);
    extern s32 psndSFXChk(u32);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void psndSFXOff(s32);
    extern s32 sprintf(char*, const char*, ...);
    extern void* npcNameToPtr(char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXRotRad(void*, double, char);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXMultVec(void*, void*, void*);
    extern s32 kpaGetLevel(void);
    extern void* mobjRunEvent(void*, void*);
    extern void evtSetValue(void*, s32, s32);
    extern s32 kpaMutekiCheck(void);
    extern void kpaAddScore(s32);
    extern void effMObjBrokenEntry(double, double, double, s32);
    extern s32 irand(s32);
    extern void npcDelete(void*);
    extern void mobjDelete(char*);
    extern const char str_PCTsPCT2x_80420258[];
    extern f32 float_150_80420260;
    extern f32 float_deg2rad_80420264;
    extern f32 float_12p5_80420268;
    extern f32 float_neg1000_8042026c;
    extern f32 float_200_80420270;
    extern f32 float_6_80420274;
    extern f32 float_75_80420278;
    extern f32 float_4p8_8042027c;
    extern f32 float_2_80420280;
    extern f32 float_0p04_80420284;
    extern f32 float_3_80420288;
    extern f32 float_0p2_8042028c;
    void* player = marioGetPtr();
    void* npc = 0;
    char buf[32];
    f32 m0[3][4];
    f32 m1[3][4];
    s32 i;
    s32 score;
    u32 action;

    if (*(s32*)((s32)mobj + 0x1DC) < 10) {
        if (distABf((double)*(f32*)((s32)player + 0x8C), (double)*(f32*)((s32)player + 0x90),
                    (double)*(f32*)((s32)mobj + 0x38), (double)*(f32*)((s32)mobj + 0x3C)) >=
            (double)float_150_80420260) {
            if (psndSFXChk(*(u32*)((s32)mobj + 0x1B4)) == 0) {
                psndSFXOff(*(s32*)((s32)mobj + 0x1B4));
                *(s32*)((s32)mobj + 0x1B4) = -1;
            }
        } else if (psndSFXChk(*(u32*)((s32)mobj + 0x1B4)) != 0) {
            *(u32*)((s32)mobj + 0x1B4) = psndSFXOn_3D(0x1CD, (void*)((s32)mobj + 0x38));
        }
        PSMTXIdentity(m0);
        PSMTXRotRad(m1, (double)(float_deg2rad_80420264 * *(f32*)((s32)mobj + 0x1AC)), 'Z');
        PSMTXConcat(m1, m0, m0);
        for (i = 0; i < *(s32*)((s32)mobj + 0x1A4); i++) {
            sprintf(buf, str_PCTsPCT2x_80420258, (char*)((s32)mobj + 5), i);
            npc = npcNameToPtr(buf);
            PSMTXMultVec(m0, (void*)((s32)npc + 0x8C), (void*)((s32)npc + 0x8C));
        }
    }

    action = *(u32*)((s32)mobj + 0x1DC);
    if (action == 0) {
        if ((*(u32*)mobj & 8) != 0) {
            i = kpaGetLevel();
            if (i == 3 || (i > -1 && i < 3 && *(s32*)((s32)mobj + 0x1E0) == 0x4000000)) {
                mobjRunEvent(mobj, *(void**)((s32)mobj + 0x1D4));
                evtSetValue(0, *(s32*)((s32)mobj + 0x1E4), 1);
                if (*(void**)((s32)mobj + 0x74) != 0) {
                    *(u16*)*(s32*)((s32)mobj + 0x74) |= 1;
                }
                *(u32*)mobj |= 2;
                score = kpaMutekiCheck() != 0 ? 500 : 400;
                kpaAddScore(score);
                *(s32*)((s32)mobj + 0x1DC) = 10;
            } else {
                *(u32*)mobj &= ~8;
            }
        }
    } else if (action == 10) {
        psndSFXOn_3D(0x1B8, (void*)((s32)mobj + 0x38));
        effMObjBrokenEntry((double)*(f32*)((s32)mobj + 0x38),
                           (double)(float_12p5_80420268 + *(f32*)((s32)mobj + 0x3C)),
                           (double)(float_12p5_80420268 + *(f32*)((s32)mobj + 0x40)), 0);
        *(s32*)((s32)mobj + 0x1DC) = *(s32*)((s32)mobj + 0x1DC) + 1;
    } else if (action == 0xB) {
        *(f32*)((s32)mobj + 0x3C) = float_neg1000_8042026c;
        for (i = 0; i < *(s32*)((s32)mobj + 0x1A4); i++) {
            sprintf(buf, str_PCTsPCT2x_80420258, (char*)((s32)mobj + 5), i);
            npc = npcNameToPtr(buf);
            *(f32*)((s32)npc + 0x11C) = *(f32*)((s32)npc + 0x8C);
            *(f32*)((s32)npc + 0x120) = *(f32*)((s32)npc + 0x90);
            *(f32*)((s32)npc + 0x124) = *(f32*)((s32)npc + 0x94);
            *(f32*)((s32)npc + 0x128) = (f32)irand(0x65) / float_200_80420270;
            *(f32*)((s32)npc + 0x12C) = float_6_80420274 - (f32)irand(0x65) / float_75_80420278;
            *(f32*)((s32)npc + 0x130) = float_4p8_8042027c;
            *(f32*)((s32)npc + 0x1A4) = float_2_80420280;
            *(u32*)((s32)npc + 0x2B4) |= 4;
        }
        if (psndSFXChk(*(u32*)((s32)mobj + 0x1B4)) == 0) {
            psndSFXOff(*(s32*)((s32)mobj + 0x1B4));
            *(s32*)((s32)mobj + 0x1B4) = -1;
        }
        *(s32*)((s32)mobj + 0x1DC) = *(s32*)((s32)mobj + 0x1DC) + 1;
    } else if (action == 99) {
        for (i = 0; i < *(s32*)((s32)mobj + 0x1A4); i++) {
            sprintf(buf, str_PCTsPCT2x_80420258, (char*)((s32)mobj + 5), i);
            npcDelete(npcNameToPtr(buf));
        }
        mobjDelete((char*)((s32)mobj + 5));
    } else if ((s32)action < 99 && (s32)action < 0xD) {
        for (i = 0; i < *(s32*)((s32)mobj + 0x1A4); i++) {
            sprintf(buf, str_PCTsPCT2x_80420258, (char*)((s32)mobj + 5), i);
            npc = npcNameToPtr(buf);
            *(f32*)((s32)npc + 0x198) += *(f32*)((s32)npc + 0x128);
            *(f32*)((s32)npc + 0x90) += *(f32*)((s32)npc + 0x12C);
            *(f32*)((s32)npc + 0x19C) += *(f32*)((s32)npc + 0x130);
            *(f32*)((s32)npc + 0xA4) += float_0p04_80420284;
            if (*(f32*)((s32)npc + 0xA4) > float_3_80420288) {
                *(f32*)((s32)npc + 0xA4) = float_3_80420288;
            }
            *(f32*)((s32)npc + 0xA8) = *(f32*)((s32)npc + 0xA4);
            *(f32*)((s32)npc + 0xAC) = *(f32*)((s32)npc + 0xA4);
            *(f32*)((s32)npc + 0x12C) -= float_0p2_8042028c;
        }
        if (npc != 0 && *(f32*)((s32)npc + 0x1A4) <= *(f32*)((s32)npc + 0x1A0)) {
            *(s32*)((s32)mobj + 0x1DC) = 99;
        }
    }
    return 0;
}

u8 evt_mobj_koopa_fireber_dodai(s32 pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern s32 animGroupBaseAsync(const char*, s32, s32);
    extern s32 mobjEntry(char*, const char*);
    extern void* mobjNameToPtr(char*);
    extern s32 sprintf(char*, const char*, ...);
    extern s32 npcEntry(char*, const char*);
    extern void* npcNameToPtr(char*);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern void* gp;
    extern const char str_MOBJ_fire_dodai_802c2430[];
    extern const char str_c_fireber_802c2440[];
    extern const char str_FRB_S_1_802c244c[];
    extern const char str_PCTsPCT2x_80420258[];
    extern f32 float_0_80420240;
    extern f32 float_1_80420250;
    extern f32 float_25_80420230;
    extern f32 float_175_80420254;
    s32* args = *(s32**)(pEvt + 0x18);
    s32 dir = evtGetValue((void*)pEvt, args[0]);
    char* name = (char*)evtGetValue((void*)pEvt, args[1]);
    f32 x = evtGetFloat((void*)pEvt, args[2]);
    f32 y = evtGetFloat((void*)pEvt, args[3]);
    f32 z = evtGetFloat((void*)pEvt, args[4]);
    s32 count = evtGetValue((void*)pEvt, args[5]);
    f32 step = evtGetFloat((void*)pEvt, args[6]);
    void* evtCode = (void*)evtGetValue((void*)pEvt, args[7]);
    s32 sw = args[8];
    s32 inBattle = *(s32*)((s32)gp + 0x14) != 0;
    void* mobj;
    void* npc;
    char buf[36];
    s32 i;

    if (animGroupBaseAsync(str_MOBJ_fire_dodai_802c2430, inBattle, 0) == 0) {
        return 0;
    }
    if (animGroupBaseAsync(str_c_fireber_802c2440, inBattle, 0) == 0) {
        return 0;
    }
    mobjEntry(name, str_MOBJ_fire_dodai_802c2430);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 4;
    *(f32*)((s32)mobj + 0x38) = x;
    *(f32*)((s32)mobj + 0x3C) = y;
    *(f32*)((s32)mobj + 0x40) = z;
    *(void**)((s32)mobj + 0x68) = mobj_koopa_fireber_dodai_evt;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = 0;
    *(u32*)mobj |= 0x9C0000;
    *(s32*)((s32)mobj + 0x1E4) = sw;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    if (evtGetValue((void*)pEvt, sw) != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    if (step == float_0_80420240) {
        step = float_1_80420250;
    }
    *(s32*)((s32)mobj + 0x1A4) = count;
    if (dir == 0) {
        if (step > 0.0f) step = -step;
    } else if (step < 0.0f) {
        step = -step;
    }
    *(f32*)((s32)mobj + 0x1AC) = step;
    *(s32*)((s32)mobj + 0x1B4) = 0;
    for (i = 0; i < count; i++) {
        sprintf(buf, str_PCTsPCT2x_80420258, name, i);
        npcEntry(buf, str_c_fireber_802c2440);
        npc = npcNameToPtr(buf);
        *(u32*)npc |= 0x8000000;
        *(u32*)((s32)npc + 0x2B8) |= 0xF;
        *(f32*)((s32)npc + 0x8C) = x;
        *(f32*)((s32)npc + 0x90) = float_25_80420230 * (f32)i + float_25_80420230 + y;
        *(f32*)((s32)npc + 0x94) = float_175_80420254 + z + (f32)i;
        animPoseSetAnim(*(s32*)((s32)npc + 0xB0), str_FRB_S_1_802c244c, 1);
        *(u32*)npc |= 0x40000;
    }
    return 2;
}

s32 kpa_get_pole_name(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    void* work = *(void**)((s32)evt + 0x174);
    if (work != 0) {
        evtSetValue(evt, args[0], (s32)((s32)work + 5));
    } else {
        evtSetValue(evt, args[0], 0);
    }
    return 2;
}

s32 kpa_get_hata_name(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    void* work = *(void**)((s32)evt + 0x174);
    if (work != 0) {
        evtSetValue(evt, args[0], (s32)((s32)work + 0x1B8));
    } else {
        evtSetValue(evt, args[0], 0);
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 kpa_get_level(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[0], kpaGetLevel());
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 kpa_get_width(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[0], (s32)*(f32*)((s32)marioGetPtr() + 0x1B8));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 kpa_score_to_str(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void* psw;
    s32 score = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    s32 thousands;
    s32 rest;
    s32 hundreds;
    s32 tens;
    s32 ones;
    s32 pos = 0;

    thousands = score / 1000;
    rest = score % 1000;
    hundreds = rest / 100;
    rest = rest % 100;
    tens = rest / 10;
    ones = rest % 10;

    if (pos != 0 || thousands != 0) {
        *(u8*)((s32)psw + pos) = thousands + '0';
        pos++;
    }
    if (pos != 0 || hundreds != 0) {
        *(u8*)((s32)psw + pos) = hundreds + '0';
        pos++;
    }
    if (pos != 0 || tens != 0) {
        *(u8*)((s32)psw + pos) = tens + '0';
        pos++;
    }
    if (pos != 0 || ones != 0) {
        *(u8*)((s32)psw + pos) = ones + '0';
    }
    *(u8*)((s32)psw + 4) = 0;
    return 2;
}

void kpa_disp_pole_score(void) {
    f32 z = float_300_80420248;
    u32 color;
    f32 trans[12];
    f32 scale[12];

    PSMTXTrans(trans, *(f32*)((s32)psw + 8), *(f32*)((s32)psw + 0xC), z);
    PSMTXScale(scale, float_0p5_8042024c, float_0p5_8042024c, float_0p5_8042024c);
    PSMTXConcat(trans, scale, trans);
    FontDrawStart();
    color = dat_80420224;
    FontDrawColor(&color);
    FontDrawStringMtx(trans, psw);
}

s32 kpa_disp_pole_score_main(void* evt, s32 isFirstCall) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 order);
    extern void kpa_disp_pole_score(void);
    extern f32 float_0_80420240;
    extern f32 float_230_80420244;
    s32* args;
    f32 x;
    f32 y;
    f32 z;
    f32 speed;

    args = *(s32**)((s32)evt + 0x18);
    x = evtGetFloat(evt, args[0]);
    y = evtGetFloat(evt, args[1]);
    z = evtGetFloat(evt, args[2]);
    speed = evtGetFloat(evt, args[3]);
    if (isFirstCall != 0) {
        *(f32*)((s32)psw + 8) = x;
        *(f32*)((s32)psw + 0xC) = y;
        *(f32*)((s32)psw + 0x10) = z;
    }
    if (*(f32*)((s32)psw + 0xC) >= float_230_80420244) {
        *(f32*)((s32)psw + 0xC) = float_230_80420244;
    } else {
        *(f32*)((s32)psw + 0xC) += speed;
    }
    dispEntry(4, 2, kpa_disp_pole_score, 0, float_0_80420240);
    return 0;
}

s32 kpa_disp_pole_init(void) {
    f32 zero;
    psw = _mapAlloc(mapalloc_base_ptr, 0x14);
    zero = float_0_80420240;
    *(volatile u8*)psw = 0;
    *(f32*)((s32)psw + 0x10) = zero;
    *(f32*)((s32)psw + 0xC) = zero;
    *(f32*)((s32)psw + 8) = zero;
    return 2;
}

s32 kpa_clear_jump_data(void) {
    void* mario = marioGetPtr();
    *(s16*)((s32)mario + 0x50) = 0;
    return 2;
}

s32 mobj_koopa_pole_evt(void* pMobj) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetLevel(void);
    extern void kpaAddScore(s32);
    extern void* mobjRunEvent(void*, void*);
    extern s32 evtCheckID(s32);
    extern void evtSetValue(void*, s32, s32);
    extern void marioKeyOff(void);
    extern void* npcGetWorkPtr(void);
    extern void npcGroupDead(void*, s32);
    extern void* kpa_pole;
    extern void* kpa_pole_move;
    extern void* kpa_move_to_toride;
    extern f32 float_25_80420230;
    extern f32 float_50_8042023c;
    extern f32 float_100_80420238;
    extern f32 float_170_80420234;
    void* player = marioGetPtr();
    u32 action = *(u32*)((s32)pMobj + 0x1DC);
    s32 score = 0;
    s32 level;
    s32 i;
    s32 npcOff;
    void* npcWork;
    void* npc;

    if (action == 0) {
        if ((*(u32*)pMobj & 8) != 0) {
            if (*(s8*)((s32)player + 0x3C) == 2) {
                level = kpaGetLevel();
                if (level >= 0 && level < 2) {
                    if (*(f32*)((s32)player + 0x90) >= float_25_80420230) {
                        *(u32*)((s32)pMobj + 0x1DC) = 10;
                    } else {
                        *(u32*)pMobj &= ~8;
                    }
                } else if (level >= 2 && level < 4) {
                    *(u32*)((s32)pMobj + 0x1DC) = 0x32;
                }
                if (*(u32*)((s32)pMobj + 0x1DC) != 0) {
                    npcOff = 0;
                    for (i = 0, npcWork = npcGetWorkPtr(); i < *(s32*)((s32)npcWork + 0x10); i++, npcOff += 0x340) {
                        npcWork = npcGetWorkPtr();
                        npc = (void*)(*(s32*)(*(s32*)npcWork + 0) + npcOff);
                        if (((*(u32*)npc & 1) != 0) && ((*(u32*)((s32)npc + 0x2B4) & 4) == 0) &&
                            ((*(u32*)npc & 0x8000000) != 0) && ((*(u32*)npc & 8) != 0)) {
                            npcGroupDead(npc, 0x20);
                        }
                    }
                    marioKeyOff();
                    evtSetValue(0, *(s32*)((s32)pMobj + 0x1E4), 1);
                    *(s32*)((s32)player + 0x78) = 0;
                }
            } else {
                *(u32*)pMobj &= ~8;
            }
        }
    } else if (action == 10) {
        level = kpaGetLevel();
        if (level == 3) {
            score = 2000;
        } else if (level < 2) {
            if (*(f32*)((s32)player + 0x90) < float_170_80420234) {
                if (*(f32*)((s32)player + 0x90) < float_100_80420238) {
                    score = (*(f32*)((s32)player + 0x90) < float_50_8042023c) ? 100 : 200;
                } else {
                    score = 500;
                }
            } else {
                score = 1000;
            }
        } else {
            score = 1000;
        }
        kpaAddScore(score);
        player = mobjRunEvent(pMobj, &kpa_pole);
        *(s32*)((s32)player + 0x15C) = score;
        *(u32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action > 10 && action < 0x32) {
        if (!evtCheckID(*(s32*)((s32)pMobj + 0x1CC))) {
            *(u32*)((s32)pMobj + 0x1DC) = 0x5A;
        }
    } else if (action == 0x32) {
        level = kpaGetLevel();
        if (level == 3) {
            score = 2000;
        } else if (level < 2) {
            if (*(f32*)((s32)player + 0x90) < float_170_80420234) {
                if (*(f32*)((s32)player + 0x90) < float_100_80420238) {
                    score = (*(f32*)((s32)player + 0x90) < float_50_8042023c) ? 100 : 200;
                } else {
                    score = 500;
                }
            } else {
                score = 1000;
            }
            kpaAddScore(score);
        } else {
            kpaAddScore(1000);
        }
        mobjRunEvent(pMobj, &kpa_pole_move);
        *(u32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action == 0x33) {
        if (!evtCheckID(*(s32*)((s32)pMobj + 0x1CC))) {
            *(u32*)((s32)pMobj + 0x1DC) = 0x5A;
        }
    } else if (action == 0x5A) {
        mobjRunEvent(pMobj, &kpa_move_to_toride);
        *(u32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action == 0x5B || action == 0x5D) {
        if (evtCheckID(*(s32*)((s32)pMobj + 0x1CC))) {
            return 0;
        }
        *(s32*)((s32)pMobj + 0x1CC) = 0;
        *(u32*)((s32)pMobj + 0x1DC) += 1;
    } else if (action == 0x5C) {
        mobjRunEvent(pMobj, *(void**)((s32)pMobj + 0x1D8));
        *(u32*)((s32)pMobj + 0x1DC) += 1;
    }
    return 0;
}

s32 evt_mobj_koopa_pole(void* pEvt) {
    extern f32 evtGetFloat(void* evt, s32 value);
    extern u32 animGroupBaseAsync(const char* name, s32 inBattle, s32 unused);
    extern s32 mobjEntry(char* name, const char* poseName);
    extern void* mobjNameToPtr(char* name);
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern void* gpGlobals;
    extern const char str_MOBJ_goal_pole_802c2410[];
    extern const char str_MOBJ_goal_hata_802c2420[];
    extern const char str_h_80420228[];
    extern f32 float_180_8042022c;
    extern void mobj_koopa_pole_evt(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    char* name;
    f32 x;
    f32 y;
    f32 z;
    void* evtCode;
    void* evtCode2;
    s32 swSet;
    s32 inBattle;
    void* mobj;
    void* hata;
    s32 swValue;

    name = (char*)evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    evtCode = (void*)evtGetValue(pEvt, args[4]);
    evtCode2 = (void*)evtGetValue(pEvt, args[5]);
    swSet = args[6];
    inBattle = ((u32)(-*(s32*)((s32)gpGlobals + 0x14)) | (u32)*(s32*)((s32)gpGlobals + 0x14)) >> 31;

    if (animGroupBaseAsync(str_MOBJ_goal_pole_802c2410, inBattle, 0) == 0) {
        return 0;
    }
    if (animGroupBaseAsync(str_MOBJ_goal_hata_802c2420, inBattle, 0) == 0) {
        return 0;
    }

    mobjEntry(name, str_MOBJ_goal_pole_802c2410);
    mobj = mobjNameToPtr(name);
    *(u32*)mobj |= 0x84;
    *(f32*)((s32)mobj + 0x38) = x;
    *(f32*)((s32)mobj + 0x3C) = y;
    *(f32*)((s32)mobj + 0x40) = z;
    *(void**)((s32)mobj + 0x1D0) = mobj_koopa_pole_evt;
    *(void**)((s32)mobj + 0x1D4) = evtCode;
    *(void**)((s32)mobj + 0x1D8) = evtCode2;
    *(u32*)mobj |= 0x1C280000;
    *(s32*)((s32)mobj + 0x1E4) = swSet;
    *(s32*)((s32)mobj + 0x1DC) = 0;
    swValue = evtGetValue(pEvt, swSet);
    if (swValue != 0) {
        *(s32*)((s32)mobj + 0x1DC) = 99;
    }
    strcpy((char*)((s32)mobj + 0x1B8), name);
    strcat((char*)((s32)mobj + 0x1B8), str_h_80420228);

    mobjEntry((char*)((s32)mobj + 0x1B8), str_MOBJ_goal_hata_802c2420);
    hata = mobjNameToPtr((char*)((s32)mobj + 0x1B8));
    *(f32*)((s32)hata + 0x38) = x;
    *(f32*)((s32)hata + 0x3C) = y + float_180_8042022c;
    *(f32*)((s32)hata + 0x40) = z;
    *(void**)((s32)hata + 0x1D0) = 0;
    *(void**)((s32)hata + 0x1D4) = 0;
    *(void**)((s32)hata + 0x1D8) = 0;
    *(s32*)((s32)hata + 0x1E4) = 0;
    *(s32*)((s32)hata + 0x1DC) = 0;
    strcpy((char*)((s32)hata + 0x1B8), name);
    if (*(void**)((s32)hata + 0x74) != 0) {
        *(u16*)*(void**)((s32)hata + 0x74) |= 1;
    }
    return 2;
}
