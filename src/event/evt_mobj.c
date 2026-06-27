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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
    s32 action;
    s32 level;
    s32 ok;
    u16* hit;
    s32 score;

    action = mobj[0x77];
    if (action == 10) {
        if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_GreenPipe_ks_802c246c) == 0) {
            effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38), float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                               float_25_80420230 + *(f32*)((s32)mobj + 0x40), 6);
        } else if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_GreenPipe_kl_802c2480) == 0) {
            effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38),
                               float_7p5_804202ac + float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                               float_25_80420230 + *(f32*)((s32)mobj + 0x40), 6);
        } else {
            effMObjBrokenEntry(*(f32*)((s32)mobj + 0x38) - float_25_80420230,
                               float_25_80420230 + *(f32*)((s32)mobj + 0x3C),
                               float_25_80420230 + *(f32*)((s32)mobj + 0x40), 6);
        }
        psndSFXOn_3D(0x834, (void*)((s32)mobj + 0x38));
        mobj[0x77] = 99;
    } else if (action < 10) {
        if (action == 0 && ((*mobj & 8) != 0)) {
            ok = 0;
            level = kpaGetLevel();
            if (level == 3) {
                ok = 1;
            } else if (level < 3 && level != 0 && level > -1 && mobj[0x78] == 0x20000000) {
                ok = 1;
            }
            if (ok != 0) {
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
                score = 400;
                if (kpaMutekiCheck() != 0) {
                    score = 500;
                }
                kpaAddScore(score);
                mobj[0x77] = 10;
            } else {
                *mobj &= ~8;
            }
        }
    } else if (action == 99) {
        mobjDelete((s32)mobj + 5);
    }
    return 0;
}

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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
    extern void mobj_lock(void);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
