#include "event/evt_audience.h"
extern s32 evtGetValue(void* evt, s32 value);
extern f32 evtGetFloat(void* evt, s32 value);
extern void evtSetFloat(void* evt, s32 value, f32 f);

extern void BattleAudience_Case_AcrobatNoTry(void);
extern void BattleAudienceJoyEnding(void);
extern void BattleAudienceAddTargetNum(f32 a, f32 b);
extern void BattleAudience_Delete(s32 id);
extern s32 BattleAudience_Entry(s32 id, u8 type, s32 arg);
extern void BattleAudience_SetAnim(s32 id, s32 anim, s32 pose);
extern void* BattleAudienceGetPtr(s32 id);
extern void* BattleAudienceBaseGetPtr(void);

extern const f32 float_0_80422b50;

s32 evt_audience_ap_recovery(void* evt) {
    return 2;
}

s32 evt_audience_joy_ending(void* evt) {
    BattleAudienceJoyEnding();
    return 2;
}

s32 evt_audience_acrobat_notry(void* evt) {
    BattleAudience_Case_AcrobatNoTry();
    return 2;
}

s32 evt_audience_num_updown_in_event(void* evt) {
    f32 value;

    value = evtGetFloat(evt, **(s32**)((s32)evt + 0x18));
    BattleAudienceAddTargetNum(float_0_80422b50, value);

    return 2;
}

s32 evt_audience_delete(void* evt) {
    s32 value;

    value = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    BattleAudience_Delete(value);

    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_entry(void* evt) {
    s32* args;
    s32 id;

    args = *(s32**)((s32)evt + 0x18);

    id = evtGetValue(evt, args[0]);

    if (BattleAudience_Entry(id, (u8)evtGetValue(evt, args[1]), 0) == 1) {
        BattleAudience_SetAnim(id, 1, 1);
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_set_animpose(void* evt) {
    s32* args;
    s32 id;
    s32 anim;
    s32 pose;

    args = *(s32**)((s32)evt + 0x18);

    id = evtGetValue(evt, args[0]);
    anim = evtGetValue(evt, args[1]);
    pose = evtGetValue(evt, args[2]);

    BattleAudience_SetAnim(id, anim, pose);

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_get_position(void* evt) {
    s32* args;
    void* audience;

    args = *(s32**)((s32)evt + 0x18);

    audience = BattleAudienceGetPtr(evtGetValue(evt, args[0]));

    evtSetFloat(evt, args[1], *(f32*)((s32)audience + 0x48));
    evtSetFloat(evt, args[2], *(f32*)((s32)audience + 0x4C));
    evtSetFloat(evt, args[3], *(f32*)((s32)audience + 0x50));

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_flag_off(void* evt) {
    s32* args;
    s32 id;
    s32 flag;
    void* audience;

    args = *(s32**)((s32)evt + 0x18);

    id = evtGetValue(evt, args[0]);
    flag = evtGetValue(evt, args[1]);

    audience = BattleAudienceGetPtr(id);
    *(s32*)audience = *(s32*)audience & ~flag;

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_flag_on(void* evt) {
    s32* args;
    s32 id;
    s32 flag;
    void* audience;

    args = *(s32**)((s32)evt + 0x18);

    id = evtGetValue(evt, args[0]);
    flag = evtGetValue(evt, args[1]);

    audience = BattleAudienceGetPtr(id);
    *(u32*)audience |= flag;

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_audience_base_flag_on(void* evt) {
    s32 flag;
    void* base;

    flag = evtGetValue(evt, **(s32**)((s32)evt + 0x18));

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= flag;

    return 2;
}


u8 evt_audience_move_position_speed(void) {
    return 0;
}


u8 evt_audience_jump_position_firstsp(void) {
    return 0;
}


u8 evt_audience_jump_position_gravity(void) {
    return 0;
}


u8 evt_audience_move_position_frame(void) {
    return 0;
}


s32 evt_audience_abnormal_natural_all(int param_1) {
    return 0;
}


s32 evt_audience_reflesh_natural_all(int param_1) {
    return 0;
}


s32 evt_audience_set_rotate_offset(int param_1) {
    return 0;
}


s32 evt_audience_set_rotate(int param_1) {
    return 0;
}


s32 evt_audience_set_position(int param_1) {
    return 0;
}


s32 evt_audience_flag_check(int param_1) {
    return 0;
}


s32 evt_audience_sound_cheer(int param_1) {
    return 0;
}
