#include "event/evt_party.h"
#include "event/evt_cmd.h"

extern s32 partyCtrlNo;
extern f32 float_0_80421d4c;

s32 evtGetValue(EventEntry* event, s32 value);
f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
void* partyGetPtr(s32 id);
void partyInitCamId(void* party);
void partySetCamId(void* party, s32 camId);
void partyChgPoseId(void* party, s32 poseId);
void partyCtrlOff(void);
void partyCtrlOn(void);

static void* evt_party_get_arg_party(EventEntry* event) {
    s32* args = event->args;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    return partyGetPtr(partyId);
}

USER_FUNC(evt_party_wait_landon) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    return ((*(u32*)party >> 1) & 1) != 0 ? 2 : 0;
}

USER_FUNC(evt_party_sleep_off) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        partyChgPoseId(party, 1);
        *(u32*)party &= ~0x400000;
    }
    return 2;
}

USER_FUNC(evt_party_sleep_on) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        partyChgPoseId(party, 6);
        *(u32*)party |= 0x400000;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_set_breed_pose) {
    void* party;
    s32* args = event->args;
    s32 poseId;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        poseId = evtGetValue(event, args[1]);
        partyChgPoseId(party, poseId);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_get_dispdir) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        evtSetFloat(event, args[1], float_0_80421d4c);
    } else {
        evtSetFloat(event, args[1], *(f32*)((s32)party + 0x10C));
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_set_camid) {
    void* party;
    s32* args = event->args;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    partySetCamId(party, evtGetValue(event, args[1]));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_party_init_camid) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    partyInitCamId(party);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_cont_onoff) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (evtGetValue(event, args[1]) == 0) {
        partyCtrlOff();
    } else {
        partyCtrlOn();
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_party_jump_pos(EventEntry* event, s32 first) {
    extern s32 partyCtrlNo;
    extern void* partyGetPtr(s32 id);
    extern void partyChgMot(void* party, s32 motion);
    extern void partyChgRunMode(void* party, s32 mode);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern s32 searchUnder2(f32 x, f32 y, f32 z, f32* out);
    extern void movePos(f32* x, f32* z, f32 distance, f32 angle);
    extern void* partySearchGround(f64 a, f64 b, void* party);
    extern void unk_800bc660(void* party);
    extern const f32 float_neg1000_80421d54;
    extern const f32 float_20_80421d48;
    extern const f32 float_0p5_80421d44;
    extern const f32 float_1_80421d40;
    extern f32 float_0_80421d4c;
    extern const f32 float_37_80421d50;

    s32* args;
    void* party;
    s32 partyId;
    f32 x;
    f32 y;
    f32 z;
    s32 frames;
    s32 keepDir;
    f32 apex;
    f32 framef;
    s32 state;

    args = event->args;
    if (first != 0) {
        partyId = 1;
        if (args[0] == 0) {
            partyId = partyCtrlNo;
        }
        *(s32*)((s32)event + 0x78) = partyId;
        party = partyGetPtr(*(s32*)((s32)event + 0x78));
        if (party == NULL) {
            return 2;
        }
        if (*(u8*)((s32)party + 0x34) == 0xB) {
            return 2;
        }
        *(s32*)((s32)event + 0x84) = 0;
        partyChgMot(party, 8);
        partyChgRunMode(party, 9);
    }

    args++;
    party = partyGetPtr(*(s32*)((s32)event + 0x78));
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    frames = sysMsec2Frame(evtGetValue(event, args[3]));
    keepDir = evtGetValue(event, args[4]);
    apex = evtGetFloat(event, args[5]);
    if (apex <= float_neg1000_80421d54) {
        apex = float_20_80421d48;
    }
    framef = (f32)frames;
    state = *(s32*)((s32)event + 0x84);

    if (state != 1) {
        if (state > 0) {
            if (state > 2) {
                return 0;
            }
            movePos((f32*)((s32)party + 0x58), (f32*)((s32)party + 0x60),
                    *(f32*)((s32)party + 0x104), *(f32*)((s32)party + 0x100));
            *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x174);
            {
                f32 vy = *(f32*)((s32)party + 0x114);
                f32 avy = vy < float_0_80421d4c ? -vy : vy;
                void* hit = partySearchGround(avy, avy, party);
                s32 landed = 0;
                if (hit != NULL && (*(f32*)((s32)party + 0x5C) + vy) <= *(f32*)((s32)party + 0xE4)) {
                    *(void**)((s32)party + 0x138) = hit;
                    landed = 1;
                    *(f32*)((s32)party + 0x5C) = *(f32*)((s32)party + 0xE4);
                    *(u32*)party &= ~(0x2 | 0x4 | 0x8);
                }
                if (landed != 0) {
                    *(s32*)((s32)event + 0x80) = 0;
                } else {
                    *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
                }
            }
            *(s32*)((s32)event + 0x80) -= 1;
            if (*(s32*)((s32)event + 0x80) > 0) {
                return 0;
            }
            partyChgPoseId(party, 1);
            *(f32*)((s32)party + 0x11C) = float_0_80421d4c;
            *(f32*)((s32)party + 0x174) = float_0_80421d4c;
            *(f32*)((s32)party + 0x104) = float_0_80421d4c;
            unk_800bc660(party);
            return 1;
        }
        if (state < 0) {
            return 0;
        }

        *(void**)((s32)party + 0x13C) = *(void**)((s32)party + 0x138);
        *(s32*)((s32)party + 0x138) = 0;
        *(f32*)((s32)party + 0x104) =
            distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z) / framef;
        *(f32*)((s32)party + 0x100) =
            angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z);
        if (keepDir == 0) {
            *(f32*)((s32)party + 0xFC) = *(f32*)((s32)party + 0x100);
        }
        partyChgPoseId(party, 4);
        {
            f32 height = y - *(f32*)((s32)party + 0x5C);
            f32 half = float_0p5_80421d44 * framef;
            f32 accelBase = apex;
            f32 accel;
            f32 under[2];
            if (height >= float_1_80421d40 || height <= -float_1_80421d40) {
                if (height >= float_0_80421d4c) {
                    accelBase = height + float_37_80421d50;
                } else {
                    accelBase = height;
                    if (searchUnder2(x, y, z, under) != 0) {
                        accelBase = apex;
                    }
                }
            }
            accel = (accelBase + accelBase) / (half * half - half);
            *(f32*)((s32)party + 0x114) = accel * half;
            *(f32*)((s32)party + 0x174) = -accel;
            *(s32*)((s32)event + 0x80) = (s32)(float_0p5_80421d44 + half);
            *(s32*)((s32)event + 0x84) = 1;
        }
    }

    movePos((f32*)((s32)party + 0x58), (f32*)((s32)party + 0x60),
            *(f32*)((s32)party + 0x104), *(f32*)((s32)party + 0x100));
    *(f32*)((s32)party + 0x5C) += *(f32*)((s32)party + 0x114);
    *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x174);
    *(s32*)((s32)event + 0x80) -= 1;
    if (*(s32*)((s32)event + 0x80) < 1) {
        f32 half = float_0p5_80421d44 * framef;
        f32 height = y - *(f32*)((s32)party + 0x5C);
        *(f32*)((s32)party + 0x114) = float_0_80421d4c;
        *(f32*)((s32)party + 0x174) = (height + height) / (half * half - half);
        *(f32*)((s32)party + 0x11C) = float_0_80421d4c;
        *(s32*)((s32)event + 0x80) = (s32)(float_0p5_80421d44 + half);
        *(s32*)((s32)event + 0x84) = 2;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_force_reset_outofscreen(EventEntry* event) {
    extern void marioGetScreenPos(float* pos, float* x, float* y, float* z);
    extern s32 marioChkInScreen(s32 x, s32 y);
    extern void partyClearFootmark(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void movePos(f32* x, f32* z, f32 distance, f32 angle);
    extern void* partyHitCheck(void* party, float* pos, float* dir, float* hitPos, float* out, float* dist);
    extern f32 vec3_802e7bf0[3];
    extern f32 float_1_80421d40;
    extern f32 float_100_80421d64;
    extern f32 float_50_80421d68;

    s32* args = event->args;
    void* party;
    void* work;
    f32 pos[3];
    f32 dir[3];
    f32 hitPos[3];
    f32 out[3];
    f32 dist;
    f32 sx;
    f32 sy;
    f32 sz;
    s32 partyId = 1;

    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == NULL) {
        return 2;
    }
    pos[0] = *(f32*)((s32)party + 0x58);
    pos[1] = *(f32*)((s32)party + 0x5C);
    pos[2] = *(f32*)((s32)party + 0x60);
    marioGetScreenPos(pos, &sx, &sy, &sz);
    if (marioChkInScreen((s32)sy, (s32)sx) != 0) {
        return 2;
    }
    partyClearFootmark();
    work = *(void**)((s32)party + 0x160);
    dir[0] = vec3_802e7bf0[0];
    dir[1] = vec3_802e7bf0[1];
    dir[2] = vec3_802e7bf0[2];
    do {
        pos[0] = *(f32*)((s32)party + 0x58);
        pos[1] = *(f32*)((s32)party + 0x5C);
        pos[2] = *(f32*)((s32)party + 0x60);
        marioGetScreenPos(pos, &sx, &sy, &sz);
        if (marioChkInScreen((s32)sy, (s32)sx) != 0) {
            break;
        }
        movePos((f32*)((s32)party + 0x58), (f32*)((s32)party + 0x60), float_1_80421d40,
                angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                         *(f32*)((s32)work + 0x8C), *(f32*)((s32)work + 0x94)));
        dist = float_100_80421d64;
        hitPos[0] = *(f32*)((s32)party + 0x58);
        hitPos[1] = *(f32*)((s32)party + 0x5C) + float_50_80421d68;
        hitPos[2] = *(f32*)((s32)party + 0x60);
        if (partyHitCheck(party, hitPos, dir, out, pos, &dist) != NULL) {
            *(f32*)((s32)party + 0x5C) = out[1];
        } else {
            *(f32*)((s32)party + 0x5C) = *(f32*)((s32)work + 0x90);
        }
    } while (1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_set_dir(EventEntry* event, s32 first) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 revise360(f32 angle);
    extern f32 toMovedirSimple(f32 angle);
    extern f32 float_270_80421d5c;

    void* party;
    s32* args = event->args;
    f32 dir;
    s32 frames;
    s32 partyId;

    if (first != 0) {
        partyId = 1;
        if (args[0] == 0) {
            partyId = partyCtrlNo;
        }
        args++;
        *(s32*)((s32)event + 0x78) = partyId;
        party = partyGetPtr(*(s32*)((s32)event + 0x78));
        if (party == NULL) {
            return 2;
        }
        *(s32*)((s32)event + 0x80) = 0;
        *(f32*)((s32)event + 0x7C) = *(f32*)((s32)party + 0x10C);
    } else {
        args++;
    }
    dir = evtGetFloat(event, args[0]);
    frames = evtGetValue(event, args[1]);
    if (frames >= 0) {
        frames = sysMsec2Frame(frames);
    }
    party = partyGetPtr(*(s32*)((s32)event + 0x78));
    if (party == NULL) {
        return 2;
    }
    if (frames > 0) {
        f32 target = revise360(float_270_80421d5c - dir);
        f32 start = *(f32*)((s32)event + 0x7C);
        f32 delta = target - start;
        f32 ratio = (f32)*(s32*)((s32)event + 0x80) / (f32)frames;
        *(f32*)((s32)party + 0x10C) = revise360(start + (delta * ratio));
        *(f32*)((s32)party + 0x110) = *(f32*)((s32)party + 0x10C);
        *(f32*)((s32)party + 0x100) = toMovedirSimple(*(f32*)((s32)party + 0x110));
        *(s32*)((s32)event + 0x80) = *(s32*)((s32)event + 0x80) + 1;
        return *(s32*)((s32)event + 0x80) == frames;
    }
    if (frames < 0) {
        *(f32*)((s32)party + 0x110) = revise360(float_270_80421d5c - dir);
        *(f32*)((s32)party + 0x100) = toMovedirSimple(*(f32*)((s32)party + 0x110));
        return 2;
    }
    *(f32*)((s32)party + 0x10C) = revise360(float_270_80421d5c - dir);
    *(f32*)((s32)party + 0x110) = *(f32*)((s32)party + 0x10C);
    *(f32*)((s32)party + 0x100) = toMovedirSimple(*(f32*)((s32)party + 0x110));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_move_pos2(EventEntry* event, s32 first) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void partySetForceMove(void* party, f32 angle, s32 frames, f32 speed);
    extern void* gp;

    void* party;
    s32* args = event->args;
    f32 x;
    f32 z;
    f32 dist;
    f32 speed;
    s32 partyId;

    if (first != 0) {
        *(s32*)((s32)event + 0x80) = 0;
    }
    partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    *(s32*)((s32)event + 0x78) = partyId;
    party = partyGetPtr(*(s32*)((s32)event + 0x78));
    if (party == NULL) {
        return 2;
    }
    if (*(s32*)((s32)event + 0x80) == 0) {
        u8 status = *(u8*)((s32)party + 0x34);
        if (status != 0 && (u8)(status - 1) > 1) {
            return 0;
        }
        *(s32*)((s32)event + 0x80) = 1;
        x = evtGetFloat(event, args[1]);
        z = evtGetFloat(event, args[2]);
        speed = evtGetFloat(event, args[3]);
        dist = distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z);
        *(f32*)((s32)party + 0x100) = angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z);
        partySetForceMove(
            party,
            *(f32*)((s32)party + 0x100),
            (s32)((f32)*(s32*)((s32)gp + 4) * (dist / speed)),
            speed / (f32)*(s32*)((s32)gp + 4));
    }
    if (*(s16*)((s32)party + 0x28) == 0) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_move_pos(EventEntry* event, s32 first) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void partySetForceMove(void* party, f32 angle, s32 frames, f32 speed);

    void* party;
    s32* args = event->args;
    f32 x;
    f32 z;
    f32 speed;
    s32 partyId;
    s32 timer;

    if (first != 0) {
        *(s32*)((s32)event + 0x80) = 0;
    }
    partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    *(s32*)((s32)event + 0x78) = partyId;
    party = partyGetPtr(*(s32*)((s32)event + 0x78));
    if (party == NULL) {
        return 2;
    }
    if (*(s32*)((s32)event + 0x80) == 0) {
        u8 status = *(u8*)((s32)party + 0x34);
        if (status != 0 && (u8)(status - 1) > 1) {
            return 0;
        }
        *(s32*)((s32)event + 0x80) = 1;
        x = evtGetFloat(event, args[1]);
        z = evtGetFloat(event, args[2]);
        *(s32*)((s32)event + 0x7C) = sysMsec2Frame(evtGetValue(event, args[3]));
        *(f32*)((s32)party + 0x100) = angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z);
        if (*(s32*)((s32)event + 0x7C) == 0) {
            timer = (s32)(distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z) / *(f32*)((s32)party + 0x104));
            *(s32*)((s32)event + 0x7C) = timer;
            speed = *(f32*)((s32)party + 0x104);
        } else {
            speed = distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z) / (f32)*(s32*)((s32)event + 0x7C);
        }
        partySetForceMove(party, *(f32*)((s32)party + 0x100), *(s32*)((s32)event + 0x7C), speed);
    }
    timer = *(s32*)((s32)event + 0x7C) - 1;
    *(s32*)((s32)event + 0x7C) = timer;
    if (timer < 0) {
        return 1;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_nokotaro_hold_cancel(EventEntry* event, s32 first) {
    extern s32 nokonokoGetStatus(void* party);
    extern void nokotaro_hold_cancel(void* party);

    void* party;
    s32 ret = 0;
    s32* args = event->args;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if ((s32)*(u8*)((s32)party + 0x31) != 2) {
            return 2;
        }
        if (nokonokoGetStatus(party) == 0) {
            return 2;
        }
        *(s32*)((s32)event + 0x78) = 0;
    }
    switch (*(s32*)((s32)event + 0x78)) {
        case 0:
            if (nokonokoGetStatus(party) != 3) {
                ret = 0;
            } else {
                nokotaro_hold_cancel(party);
                *(s32*)((s32)event + 0x78) = 1;
            }
            break;
        case 1:
            if (nokonokoGetStatus(party) != 0) {
                ret = 0;
            } else {
                *(u32*)party &= 0x7FFFFFFF;
                ret = 2;
            }
            break;
    }
    return ret;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_set_dir_pos) {
    extern void* camGetPtr(s32 id);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 revise360(f32 angle);
    extern f32 float_180_80421d58;

    s32* args = event->args;
    void* party = partyGetPtr(args[0]);
    f32 x;
    f32 z;
    f32 angle;
    f32 bucket;
    if (party == 0) {
        return 2;
    }
    x = (f32)evtGetValue(event, args[1]);
    z = (f32)evtGetValue(event, args[2]);
    angle = revise360(angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z) - *(f32*)((s32)camGetPtr(4) + 0x114));
    if (angle < float_180_80421d58) {
        bucket = float_180_80421d58;
    } else {
        bucket = float_0_80421d4c;
    }
    *(f32*)((s32)party + 0x110) = bucket;
    *(f32*)((s32)party + 0x100) = angle;
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
s32 evt_party_yoshi_ride(EventEntry* event, s32 first) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 yoshiGetStatus(void);
    extern u32 marioGetoffYoshi(void);

    s32* args = event->args;
    void* party;
    s32 ride;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    ride = args[1];
    if (first != 0) {
        if (ride != 0) {
            if (yoshiGetStatus() != 0) {
                return 2;
            }
            *(u32*)party |= 0x80000000;
            *(u32*)party |= 0x100;
            partyChgRunMode(party, 3);
        } else {
            marioGetoffYoshi();
        }
    }
    if (ride != 0) {
        if (yoshiGetStatus() == 1) {
            *(u32*)party &= 0x7FFFFFFF;
            return 2;
        }
    } else if (yoshiGetStatus() == 0) {
        *(u32*)party &= 0x7FFFFFFF;
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_set_dir_npc(EventEntry* event, s32 first) {
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void* camGetPtr(s32 id);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 revise360(f32 angle);
    extern f32 float_180_80421d58;

    s32* args = event->args;
    void* party = partyGetPtr(args[0]);
    void* npc;
    s32 name;
    f32 x;
    f32 z;
    f32 angle;
    f32 bucket;
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        name = evtGetValue(event, args[1]);
        npc = evtNpcNameToPtr(event, name);
        if (npc == 0) {
            return 2;
        }
        x = *(f32*)((s32)npc + 0x8C);
        z = *(f32*)((s32)npc + 0x94);
        angle = revise360(angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), x, z) - *(f32*)((s32)camGetPtr(4) + 0x114));
        if (angle < float_180_80421d58) {
            bucket = float_180_80421d58;
        } else {
            bucket = float_0_80421d4c;
        }
        *(f32*)((s32)party + 0x110) = bucket;
        *(f32*)((s32)party + 0x100) = angle;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 evt_party_nokotaro_kick_hold(EventEntry* event, s32 first) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 nokonokoGetStatus(void* party);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    s32 status;

    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }

    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }

    if (first != 0) {
        if ((s32)*(u8*)((s32)party + 0x31) != 2) {
            return 2;
        }
        if ((*(u32*)party & 0x100) != 0) {
            return 2;
        }

        *(s32*)((s32)party + 0x16C) = 0x1388;
        *(u32*)party |= 0x80000000;
        *(u32*)party |= 0x100;
        partyChgRunMode(party, 3);
    }

    if (nokonokoGetStatus(party) == 0) {
        *(u32*)party &= 0x7FFFFFFF;
        return 2;
    }

    status = nokonokoGetStatus(party);
    return 2 & (~((status - 3) | (3 - status)) >> 31);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_cloud_breathout(EventEntry* event, s32 first) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 cloudGetStatus(void);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if ((s32)*(u8*)((s32)party + 0x31) != 5) {
            return 2;
        }
        if ((*(u32*)party & 0x100) != 0) {
            return 2;
        }
        *(s32*)((s32)party + 0x16C) = args[1];
        *(u32*)party |= 0x80000000;
        *(u32*)party |= 0x100;
        partyChgRunMode(party, 3);
    }
    if (cloudGetStatus() == 0) {
        *(u32*)party &= 0x7FFFFFFF;
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_yoshi_fly(EventEntry* event, s32 first) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern void evt_set_yoshi_spd(f32 speed);
    extern void evt_set_yoshi_frm(s32 frame);
    extern s32 evt_get_yoshi_frm(void);
    extern s32 yoshiGetStatus(void);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if (yoshiGetStatus() != 1) {
            return 2;
        }
        *(u32*)party |= 0x80000000;
        evt_set_yoshi_spd(evtGetFloat(event, args[1]));
        evt_set_yoshi_frm(sysMsec2Frame(evtGetValue(event, args[2])));
    }
    if (evt_get_yoshi_frm() >= 0) {
        return 0;
    }
    *(u32*)party &= 0x7FFFFFFF;
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
s32 evt_party_nokotaro_kick(EventEntry* event, s32 first) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 nokonokoGetStatus(void* party);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if ((s32)*(u8*)((s32)party + 0x31) != 2) {
            return 2;
        }
        if ((*(u32*)party & 0x100) != 0) {
            return 2;
        }
        *(s32*)((s32)party + 0x16C) = 0;
        *(u32*)party |= 0x80000000;
        *(u32*)party |= 0x100;
        partyChgRunMode(party, 3);
    }
    if (nokonokoGetStatus(party) == 0) {
        *(u32*)party &= 0x7FFFFFFF;
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_get_pos) {
    extern f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    extern f32 float_neg5000_80421d60;

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        evtSetFloat(event, args[1], float_neg5000_80421d60);
        evtSetFloat(event, args[2], float_neg5000_80421d60);
        evtSetFloat(event, args[3], float_neg5000_80421d60);
        return 2;
    }
    evtSetFloat(event, args[1], *(f32*)((s32)party + 0x58));
    evtSetFloat(event, args[2], *(f32*)((s32)party + 0x5C));
    evtSetFloat(event, args[3], *(f32*)((s32)party + 0x60));
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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_vivian_hold(EventEntry* event, s32 first) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 vivianGetStatus(void);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if (vivianGetStatus() != 0) {
            return 2;
        }
        *(u32*)party |= 0x80000000;
        *(u32*)party |= 0x100;
        partyChgRunMode(party, 3);
    }
    if (vivianGetStatus() != 2) {
        return 0;
    }
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_party_thunders_use(EventEntry* event, s32 first) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 bomheiGetStatus(void);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if (bomheiGetStatus() != 0) {
            return 2;
        }
        *(u32*)party |= 0x80000000;
        *(u32*)party |= 0x100;
        partyChgRunMode(party, 3);
    }
    if (bomheiGetStatus() != 0) {
        return 0;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_evt_party_cloud_lock_animations_on_off(EventEntry* event, s32 first) {
    extern void N_cloudLockAnimationsOn(void* party);
    extern void N_cloudLockAnimationsOff(void* party);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0) {
        if ((s32)*(u8*)((s32)party + 0x31) != 5) {
            return 2;
        }
        if ((*(u32*)party & 0x100) != 0) {
            return 2;
        }
        if (args[1] != 0) {
            N_cloudLockAnimationsOn(party);
        } else {
            N_cloudLockAnimationsOff(party);
        }
    }
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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_dispflg_onoff) {
    s32 mask;
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    mask = args[1];
    if (evtGetValue(event, args[2]) != 0) {
        *(u32*)((s32)party + 4) |= mask;
    } else {
        *(u32*)((s32)party + 4) &= ~mask;
    }
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_flg_onoff) {
    s32 mask;
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    mask = args[1];
    if (evtGetValue(event, args[2]) != 0) {
        *(u32*)party |= mask;
    } else {
        *(u32*)party &= ~mask;
    }
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_set_homing_dist) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    *(f32*)((s32)party + 0x40) = (f32)evtGetValue(event, args[1]);
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
USER_FUNC(evt_party_set_hosei_xyz) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    *(f32*)((s32)party + 0xAC) = evtGetFloat(event, args[1]);
    *(f32*)((s32)party + 0xB0) = evtGetFloat(event, args[2]);
    *(f32*)((s32)party + 0xB4) = evtGetFloat(event, args[3]);
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
USER_FUNC(evt_party_set_pos) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    *(f32*)((s32)party + 0x58) = evtGetFloat(event, args[1]);
    *(f32*)((s32)party + 0x5C) = evtGetFloat(event, args[2]);
    *(f32*)((s32)party + 0x60) = evtGetFloat(event, args[3]);
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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_outofscreen) {
    extern f32 evtSetFloat(EventEntry* event, s32 target, f32 value);

    s32* args = event->args;
    void* party;
    s32 out;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    out = (s32)(((*(u32*)((s32)party + 4) >> 23) & 1) ^ 1);
    evtSetFloat(event, args[1], (f32)out);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_get_name_hitobj_ride) {
    extern s32 hitGetName(void* hit);

    s32* args = event->args;
    s32 name = 0;
    void* party;
    void* hit;
    s32 dst;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    hit = *(void**)((s32)party + 0x138);
    dst = args[1];
    if (hit != 0) {
        name = hitGetName(hit);
    }
    evtSetValue(event, dst, name);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_get_name_hitobj_head) {
    extern s32 hitGetName(void* hit);

    s32* args = event->args;
    s32 name = 0;
    void* party;
    void* hit;
    s32 dst;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    hit = *(void**)((s32)party + 0x140);
    dst = args[1];
    if (hit != 0) {
        name = hitGetName(hit);
    }
    evtSetValue(event, dst, name);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 evt_party_vivian_unhold(EventEntry* event, s32 first) {
    extern s32 vivianGetStatus(void);
    extern void vivianUnhold(void* party);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (first != 0 && vivianGetStatus() == 2) {
        vivianUnhold(party);
    }
    return vivianGetStatus() == 0 ? 2 : 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_nokotaro_get_status) {
    extern s32 nokonokoGetStatus(void* party);

    s32* args = event->args;
    void* party;
    s32 dst;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if ((s32)*(u8*)((s32)party + 0x31) != 2) {
        return 2;
    }
    dst = args[1];
    evtSetValue(event, dst, nokonokoGetStatus(party));
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(L_evt_party_dokan) {
    s32* args = event->args;
    void* party;
    s32 dst;
    s32 value;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    dst = args[1];
    if (*(u8*)((s32)party + 0x34) == 0xB || *(u8*)((s32)party + 0x34) == 0xC) {
        value = 0;
    } else {
        value = 1;
    }
    evtSetValue(event, dst, value);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_get_status) {
    extern s32 vivianGetStatus(void);
    extern s32 bomheiGetStatus(void);

    s32* args = event->args;
    s32 kind = args[0];
    s32 dst = args[1];
    s32 status;

    switch (kind) {
        case 6:
            status = vivianGetStatus();
            evtSetValue(event, dst, status);
            break;
        case 3:
            evtSetValue(event, dst, bomheiGetStatus());
            break;
    }

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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_set_pose) {
    extern void partyChgPose(void* party, void* pose);

    void* party;
    s32* args = event->args;
    s32 pose;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    pose = evtGetValue(event, args[1]);
    partyChgPose(party, (void*)pose);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_move_beside_mario) {
    extern void partyMoveBesideMario(void* party, s32 mode);

    void* party;
    s32* args = event->args;
    s32 mode;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    mode = evtGetValue(event, args[1]);
    partyMoveBesideMario(party, mode);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_move_behind_mario) {
    extern void partyMoveBehindMario(void* party, s32 mode);

    void* party;
    s32* args = event->args;
    s32 mode;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    mode = evtGetValue(event, args[1]);
    partyMoveBehindMario(party, mode);
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_party_set_dispdir) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern f32 reviseAngle(f32 angle);

    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    *(f32*)((s32)party + 0x110) = reviseAngle(evtGetFloat(event, args[1]));
    *(f32*)((s32)party + 0x10C) = *(f32*)((s32)party + 0x110);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 L_evt_party_vivian_tail(EventEntry* event) {
    extern s32 partyCtrlNo;
    extern void* partyGetPtr(s32 partyId);
    extern void L_partyVivianTailStop(void* party);
    extern void L_partyVivianTailStart(void* party);
    s32* args;
    s32 partyId;
    void* party;

    args = event->args;
    partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (args[1] == 0) {
        L_partyVivianTailStop(party);
    } else {
        L_partyVivianTailStart(party);
    }
    return 2;
}

s32 evt_party_run(EventEntry* event) {
    extern s32 partyCtrlNo;
    extern void* partyGetPtr(s32 partyId);
    extern void partyRun(void* party);
    s32* args;
    s32 partyId;
    void* party;

    args = event->args;
    partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        partyRun(party);
    }
    return 2;
}

s32 evt_party_stop(EventEntry* event) {
    extern s32 partyCtrlNo;
    extern void* partyGetPtr(s32 partyId);
    extern void partyStop(void* party);
    s32* args;
    s32 partyId;
    void* party;

    args = event->args;
    partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        partyStop(party);
    }
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
USER_FUNC(unk_800eb9cc) {
    s32 mask;
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    mask = args[1];
    if (evtGetValue(event, args[2]) != 0) {
        *(u32*)((s32)party + 8) |= mask;
    } else {
        *(u32*)((s32)party + 8) &= ~mask;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
