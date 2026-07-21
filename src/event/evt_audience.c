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
extern void BattleAudience_SetPosition(s32 id, f32 x, f32 y, f32 z);
extern void BattleAudience_SetRotate(s32 id, f32 x, f32 y, f32 z);
extern void BattleAudience_SetRotateOffset(s32 id, f32 x, f32 y, f32 z);
extern u8 BattleAudience_GetSysCtrl(s32 id);
extern void BattleAudience_ChangeStatus(s32 id, s32 status);
extern void* BattleAudienceGetPtr(s32 id);
extern void* BattleAudienceBaseGetPtr(void);
extern void evtSetValue(void* evt, s32 value, s32 out);
extern s32 irand(s32 max);

extern const f32 float_0_80422b50;
s32 evt_audience_base_flag_on(void* evt) {
    s32 flag;
    void* base;

    flag = evtGetValue(evt, **(s32**)((s32)evt + 0x18));

    base = BattleAudienceBaseGetPtr();
    *(u32*)base |= flag;

    return 2;
}


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_flag_check(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    s32 flag = evtGetValue(evt, args[1]);
    void* audience = BattleAudienceGetPtr(id);

    if ((*(u32*)audience & flag) != 0) {
        evtSetValue(evt, args[2], 1);
    } else {
        evtSetValue(evt, args[2], 0);
    }
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
s32 evt_audience_set_position(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);

    BattleAudience_SetPosition(id, x, y, z);
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
s32 evt_audience_set_rotate(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);

    BattleAudience_SetRotate(id, x, y, z);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_set_rotate_offset(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);

    BattleAudience_SetRotateOffset(id, x, y, z);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_move_position_frame(void* evt, s32 isFirstCall) {
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern const f32 float_180_80422b5c;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 type = evtGetValue(evt, args[4]);
    s32 frames = evtGetValue(evt, args[5]);
    void* audience = BattleAudienceGetPtr(id);
    f32 diff;
    f32 outX;
    f32 outY;
    f32 outZ;

    if (isFirstCall != 0) {
        *(f32*)((s32)audience + 0x100) = *(f32*)((s32)audience + 0x48);
        *(f32*)((s32)audience + 0x108) = *(f32*)((s32)audience + 0x4C);
        *(f32*)((s32)audience + 0x110) = *(f32*)((s32)audience + 0x50);
        *(f32*)((s32)audience + 0x104) = x;
        *(f32*)((s32)audience + 0x10C) = y;
        *(f32*)((s32)audience + 0x114) = z;
        *(s32*)((s32)audience + 0x128) = type;
        *(s32*)((s32)audience + 0xF8) = frames;
        *(s32*)((s32)audience + 0xFC) = frames;
    }

    diff = *(f32*)((s32)audience + 0x104) - *(f32*)((s32)audience + 0x100);
    if (diff > float_0_80422b50) {
        *(f32*)((s32)audience + 0xE8) = float_180_80422b5c;
    } else if (diff < float_0_80422b50) {
        *(f32*)((s32)audience + 0xE8) = float_0_80422b50;
    }
    outX = intplGetValue(*(s32*)((s32)audience + 0x128),
        *(s32*)((s32)audience + 0xFC) - *(s32*)((s32)audience + 0xF8),
        *(s32*)((s32)audience + 0xFC),
        *(f32*)((s32)audience + 0x100),
        *(f32*)((s32)audience + 0x104));
    outY = intplGetValue(*(s32*)((s32)audience + 0x128),
        *(s32*)((s32)audience + 0xFC) - *(s32*)((s32)audience + 0xF8),
        *(s32*)((s32)audience + 0xFC),
        *(f32*)((s32)audience + 0x108),
        *(f32*)((s32)audience + 0x10C));
    outZ = intplGetValue(*(s32*)((s32)audience + 0x128),
        *(s32*)((s32)audience + 0xFC) - *(s32*)((s32)audience + 0xF8),
        *(s32*)((s32)audience + 0xFC),
        *(f32*)((s32)audience + 0x110),
        *(f32*)((s32)audience + 0x114));
    BattleAudience_SetPosition(id, outX, outY, outZ);
    *(s32*)((s32)audience + 0xF8) = *(s32*)((s32)audience + 0xF8) - 1;
    if (*(s32*)((s32)audience + 0xF8) < 1) {
        BattleAudience_SetPosition(id,
            *(f32*)((s32)audience + 0x104),
            *(f32*)((s32)audience + 0x10C),
            *(f32*)((s32)audience + 0x114));
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_move_position_speed(void* evt, s32 isFirstCall) {
    extern double sqrt(double x);
    extern const f32 float_180_80422b5c;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    f32 speed = evtGetFloat(evt, args[4]);
    void* audience = BattleAudienceGetPtr(id);
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;

    if (isFirstCall != 0) {
        *(f32*)((s32)audience + 0x104) = x;
        *(f32*)((s32)audience + 0x10C) = y;
        *(f32*)((s32)audience + 0x114) = z;
        dy = y - *(f32*)((s32)audience + 0x4C);
        dx = x - *(f32*)((s32)audience + 0x48);
        dz = z - *(f32*)((s32)audience + 0x50);
        dist = sqrt(dx * dx + dy * dy + dz * dz);
        if (float_0_80422b50 == dx) {
            *(f32*)((s32)audience + 0x118) = float_0_80422b50;
        } else {
            *(f32*)((s32)audience + 0x118) = speed * (dx / dist);
        }
        if (float_0_80422b50 == dy) {
            *(f32*)((s32)audience + 0x11C) = float_0_80422b50;
        } else {
            *(f32*)((s32)audience + 0x11C) = speed * (dy / dist);
        }
        if (float_0_80422b50 == dz) {
            *(f32*)((s32)audience + 0x120) = float_0_80422b50;
        } else {
            *(f32*)((s32)audience + 0x120) = speed * (dz / dist);
        }
        *(s32*)((s32)audience + 0xF8) = (s32)(dist / speed);
        *(s32*)((s32)audience + 0xFC) = (s32)(dist / speed);
    }

    dx = *(f32*)((s32)audience + 0x118);
    if (dx > float_0_80422b50) {
        *(f32*)((s32)audience + 0xE8) = float_180_80422b5c;
    } else if (dx < float_0_80422b50) {
        *(f32*)((s32)audience + 0xE8) = float_0_80422b50;
    }
    BattleAudience_SetPosition(id,
        *(f32*)((s32)audience + 0x48) + *(f32*)((s32)audience + 0x118),
        *(f32*)((s32)audience + 0x4C) + *(f32*)((s32)audience + 0x11C),
        *(f32*)((s32)audience + 0x50) + *(f32*)((s32)audience + 0x120));
    if (*(s32*)((s32)audience + 0xF8) > 0) {
        *(s32*)((s32)audience + 0xF8) = *(s32*)((s32)audience + 0xF8) - 1;
    }
    if (*(s32*)((s32)audience + 0xF8) < 0) {
        *(s32*)((s32)audience + 0xF8) = *(s32*)((s32)audience + 0xF8) + 1;
    }
    if (*(s32*)((s32)audience + 0xF8) == 0) {
        BattleAudience_SetPosition(id,
            *(f32*)((s32)audience + 0x104),
            *(f32*)((s32)audience + 0x10C),
            *(f32*)((s32)audience + 0x114));
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_jump_position_firstsp(void* evt, s32 isFirstCall) {
    extern const f32 float_2_80422b58;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 frames = evtGetValue(evt, args[4]);
    f32 firstSpeed = evtGetFloat(evt, args[5]);
    void* audience = BattleAudienceGetPtr(id);
    f32 framesF = (f32)frames;

    if (isFirstCall != 0) {
        *(f32*)((s32)audience + 0x104) = x;
        *(f32*)((s32)audience + 0x10C) = y;
        *(f32*)((s32)audience + 0x114) = z;
        *(f32*)((s32)audience + 0x118) = (x - *(f32*)((s32)audience + 0x48)) / framesF;
        *(f32*)((s32)audience + 0x11C) = firstSpeed;
        *(f32*)((s32)audience + 0x120) = (z - *(f32*)((s32)audience + 0x50)) / framesF;
        *(f32*)((s32)audience + 0x124) =
            (float_2_80422b58 * -(firstSpeed * framesF - (y - *(f32*)((s32)audience + 0x4C)))) /
            (f32)(frames * frames);
        *(s32*)((s32)audience + 0xF8) = frames;
        *(s32*)((s32)audience + 0xFC) = frames;
    }

    BattleAudience_SetPosition(id,
        *(f32*)((s32)audience + 0x48) + *(f32*)((s32)audience + 0x118),
        *(f32*)((s32)audience + 0x4C) + *(f32*)((s32)audience + 0x11C),
        *(f32*)((s32)audience + 0x50) + *(f32*)((s32)audience + 0x120));
    *(f32*)((s32)audience + 0x11C) = *(f32*)((s32)audience + 0x11C) + *(f32*)((s32)audience + 0x124);
    *(s32*)((s32)audience + 0xF8) = *(s32*)((s32)audience + 0xF8) - 1;
    if (*(s32*)((s32)audience + 0xF8) < 1) {
        BattleAudience_SetPosition(id,
            *(f32*)((s32)audience + 0x104),
            *(f32*)((s32)audience + 0x10C),
            *(f32*)((s32)audience + 0x114));
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_jump_position_gravity(void* evt, s32 isFirstCall) {
    extern const f32 float_0p5_80422b54;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    f32 x = evtGetFloat(evt, args[1]);
    f32 y = evtGetFloat(evt, args[2]);
    f32 z = evtGetFloat(evt, args[3]);
    s32 frames = evtGetValue(evt, args[4]);
    f32 gravity = evtGetFloat(evt, args[5]);
    void* audience = BattleAudienceGetPtr(id);
    f32 framesF = (f32)frames;

    if (isFirstCall != 0) {
        *(f32*)((s32)audience + 0x104) = x;
        *(f32*)((s32)audience + 0x10C) = y;
        *(f32*)((s32)audience + 0x114) = z;
        *(f32*)((s32)audience + 0x118) = (x - *(f32*)((s32)audience + 0x48)) / framesF;
        *(f32*)((s32)audience + 0x11C) =
            -((float_0p5_80422b54 * gravity) * framesF -
            ((y - *(f32*)((s32)audience + 0x4C)) / framesF));
        *(f32*)((s32)audience + 0x120) = (z - *(f32*)((s32)audience + 0x50)) / framesF;
        *(f32*)((s32)audience + 0x124) = gravity;
        *(s32*)((s32)audience + 0xF8) = frames;
        *(s32*)((s32)audience + 0xFC) = frames;
    }

    BattleAudience_SetPosition(id,
        *(f32*)((s32)audience + 0x48) + *(f32*)((s32)audience + 0x118),
        *(f32*)((s32)audience + 0x4C) + *(f32*)((s32)audience + 0x11C),
        *(f32*)((s32)audience + 0x50) + *(f32*)((s32)audience + 0x120));
    *(f32*)((s32)audience + 0x11C) = *(f32*)((s32)audience + 0x11C) + *(f32*)((s32)audience + 0x124);
    *(s32*)((s32)audience + 0xF8) = *(s32*)((s32)audience + 0xF8) - 1;
    if (*(s32*)((s32)audience + 0xF8) < 1) {
        BattleAudience_SetPosition(id,
            *(f32*)((s32)audience + 0x104),
            *(f32*)((s32)audience + 0x10C),
            *(f32*)((s32)audience + 0x114));
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
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


s32 evt_audience_delete(void* evt) {
    s32 value;

    value = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    BattleAudience_Delete(value);

    return 2;
}

s32 evt_audience_ap_recovery(void* evt) {
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_reflesh_natural_all(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 percent = evtGetValue(evt, args[0]);
    void* audience;
    s32 i;
    s32 natural = 2;

    evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    i = 0;
    audience = BattleAudienceGetPtr(0);
    while (i < 200) {
        if ((*(u32*)audience & 1) != 0 &&
            *(u8*)((s32)audience + 0x19) == 0xD &&
            *(s32*)((s32)audience + 0x4) == 1) {
            if (irand(100) < percent) {
                *(s32*)((s32)audience + 0x4) = natural;
            }
        }
        i++;
        audience = (void*)((s32)audience + 0x134);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_abnormal_natural_all(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 percent = evtGetValue(evt, args[0]);
    void* audience;
    s32 i;

    evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    i = 0;
    audience = BattleAudienceGetPtr(0);
    while (i < 200) {
        if (BattleAudience_GetSysCtrl(i) != 0) {
            u8 status = *(u8*)((s32)audience + 0x19);

            if (status <= 3 || (u8)(status - 4) <= 2 || status == 7) {
                if (irand(100) < percent) {
                    BattleAudience_ChangeStatus(i, 0xD);
                }
            }
        }
        i++;
        audience = (void*)((s32)audience + 0x134);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_audience_sound_cheer(void* evt) {
    extern void BattleAudienceSoundCheer(s32 vol, s32 frames);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 vol = evtGetValue(evt, args[0]);
    s32 frames = evtGetValue(evt, args[1]);

    BattleAudienceSoundCheer(vol, frames);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_audience_num_updown_in_event(void* evt) {
    f32 value;

    value = evtGetFloat(evt, **(s32**)((s32)evt + 0x18));
    BattleAudienceAddTargetNum(float_0_80422b50, value);

    return 2;
}

s32 evt_audience_acrobat_notry(void* evt) {
    BattleAudience_Case_AcrobatNoTry();
    return 2;
}

s32 evt_audience_joy_ending(void* evt) {
    BattleAudienceJoyEnding();
    return 2;
}
