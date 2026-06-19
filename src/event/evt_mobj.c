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

extern void* mapalloc_base_ptr;
void* psw;
extern f32 float_0_80420240;
extern f32 float_300_80420248;
extern f32 float_0p5_8042024c;
extern f32 float_1_80420250;
extern u32 dat_80420224;
extern const char str_me_80420324[3];

s32 mobj_koopa_stone_blk_evt(void) {
    return 0;
}

s32 evt_mobj_delete(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    mobjDelete(evtGetValue(evt, args[0]));
    return 2;
}

s32 kpa_clear_jump_data(void) {
    void* mario = marioGetPtr();
    *(s16*)((s32)mario + 0x50) = 0;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 kpa_get_width(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[0], (s32)*(f32*)((s32)marioGetPtr() + 0x1B8));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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

u8 mobj_kururing_floor(void* pMobj) {
    return 0;
}


s32 mobj_switch_float_blk(void* pMobj) {
    return 0;
}


s32 mobj_recovery_blk(void* pMobj) {
    return 0;
}


u8 mobj_koopa_fireber_dodai_evt(void) {
    return 0;
}


s32 mobj_itembox(void* pMobj) {
    return 0;
}


s32 mobj_lv_blk_evt(void* pMobj) {
    return 0;
}


s32 mobj_lock(void* pMobj) {
    return 0;
}


s32 mobj_koopa_pole_evt(void* pMobj) {
    return 0;
}


s32 mobj_koopa_sango_evt(u32* param_1) {
    return 0;
}


u8 evt_mobj_itembox(s32 pEvt) {
    return 0;
}


s32 mobj_koopa_brickblk(u32* param_1) {
    return 0;
}


u8 kururing_capture(int param_1, int param_2) {
    return 0;
}


s32 evt_mobj_brick(void* pEvt) {
    return 0;
}


s32 mobj_timerswitch(void* pMobj) {
    return 0;
}


u8 evt_mobj_koopa_fireber_dodai(s32 pEvt) {
    return 0;
}


s32 mobj_koopa_badgeblk(u32* param_1) {
    return 0;
}


s32 mobj_badgeblk(void* pMobj) {
    return 0;
}


s32 mobj_brickblk(void* pMobj) {
    return 0;
}


u8 evt_mobj_float_blk(void) {
    return 0;
}


u8 mobj_rideswitch_lightblue(void* pMobj) {
    return 0;
}


s32 mobj_powerupblk(void* pMobj) {
    return 0;
}


u8 evt_mobj_koopa_pole(s32 pEvt) {
    return 0;
}


u8 evt_mobj_switch_float_blk(s32 pEvt) {
    return 0;
}


s32 mobj_jumpstand_red(void* pMobj) {
    return 0;
}


s32 mobj_10countblk(void* pMobj) {
    return 0;
}


s32 mobj_koopa_ojama_blk_evt(u32* param_1) {
    return 0;
}


s32 evt_mobj_timerswitch(void* pEvt) {
    return 0;
}


s32 mobj_jumpstand_blue(void* pMobj) {
    return 0;
}


s32 mobj_koopa_dokan_evt(u32* param_1) {
    return 0;
}


u8 evt_mobj_kururing_floor(s32 pEvt) {
    return 0;
}


s32 evt_mobj_koopa_dokan(void* pEvt) {
    return 0;
}


s32 evt_mobj_koopa_brick(int param_1) {
    return 0;
}


s32 evt_mobj_breaking_floor(void* pEvt) {
    return 0;
}


s32 evt_mobj_badgeblk(void* pEvt) {
    return 0;
}


s32 evt_mobj_lv_blk(void* pEvt) {
    return 0;
}


u8 evt_mobj_lock(void) {
    return 0;
}


s32 evt_mobj_switch_blue(void* pEvt) {
    return 0;
}


s32 evt_mobj_koopa_hidden_blk(void* pEvt) {
    return 0;
}


s32 evt_mobj_recovery_blk(void* pEvt) {
    return 0;
}


s32 evt_mobj_blk(void* pEvt) {
    return 0;
}


s32 evt_mobj_koopa_sango(void* pEvt) {
    return 0;
}


s32 evt_mobj_koopa_badgeblk(int param_1) {
    return 0;
}


s32 mobj_save_blk(void* pMobj) {
    return 0;
}


s32 evt_mobj_switch_red(void* pEvt) {
    return 0;
}


s32 evt_mobj_koopa_ojama_blk(void* pEvt) {
    return 0;
}


s32 mobj_breaking_floor(void* pMobj) {
    return 0;
}


s32 evt_mobj_save_blk(void* pEvt) {
    return 0;
}


u8 evt_mobj_jumpstand_blue(void) {
    return 0;
}


u8 evt_mobj_rideswitch_lightblue(s32 pEvt) {
    return 0;
}


u8 evt_mobj_powerupblk(void) {
    return 0;
}


s32 evt_mobj_jumpstand_red(int param_1) {
    return 0;
}


s32 mobj_float_blk(void* pMobj) {
    return 0;
}


u8 fire_func(s32 pEvt, s32 param_2) {
    return 0;
}


s32 evt_mobj_koopa_blk(int param_1) {
    return 0;
}


s32 evt_mobj_floatswitch_red(int param_1) {
    return 0;
}


s32 evt_mobj_tornadoswitch_blue(void* pEvt) {
    return 0;
}


s32 evt_mobj_floatswitch_blue(int param_1) {
    return 0;
}


s32 evt_mobj_koopa_stone_blk(void* pEvt) {
    return 0;
}


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


s32 mobj_blk(void* pMobj) {
    return 0;
}


s32 mobj_koopa_blk(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma no_register_save_helpers on
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

#pragma no_register_save_helpers off

#pragma no_register_save_helpers off


s32 mobj_switch_red(void* pMobj) {
    return 0;
}


s32 mobj_switch_blue(void* pMobj) {
    return 0;
}


u8 kpa_disp_pole_score_main(void) {
    return 0;
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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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

s32 evt_mobj_entry(void* pEvt) {
    return 0;
}


s32 kpa_powerup(void* pEvt) {
    return 0;
}


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
