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

s32 evt_mobj_check(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[1], (s32)mobjNameToPtrNoAssert(evtGetValue(evt, args[0])));
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

s32 kpa_get_level(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[0], kpaGetLevel());
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

s32 kpa_get_width(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[0], (s32)*(f32*)((s32)marioGetPtr() + 0x1B8));
    return 2;
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

s32 evt_mobj_get_kindname(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    void* mobj = mobjNameToPtr(evtGetValue(evt, args[0]));
    evtSetValue(evt, args[1], (s32)((s32)mobj + 0x15));
    return 2;
}

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

void kpa_disp_pole_score(void) {
    u32 color;
    f32 trans[12];
    f32 scale[12];
    f32 z = float_300_80420248;
    PSMTXTrans(trans, *(f32*)((s32)psw + 8), *(f32*)((s32)psw + 0xC), z);
    PSMTXScale(scale, float_0p5_8042024c, float_0p5_8042024c, float_0p5_8042024c);
    PSMTXConcat(trans, scale, trans);
    FontDrawStart();
    color = dat_80420224;
    FontDrawColor(&color);
    FontDrawStringMtx(trans, psw);
}

