#include "party/party_yoshi.h"

extern s32 evt_yoshi_frm;
extern f32 evt_yoshi_spd;
extern f32 float_26_804211e0;
extern f32 float_20_804211e4;

s32 sysMsec2Frame(s32 msec);

s32 evt_get_yoshi_frm(void) {
    return evt_yoshi_frm;
}

void evt_set_yoshi_spd(f32 speed) {
    evt_yoshi_spd = speed;
}

void evt_set_yoshi_frm(s32 msec) {
    evt_yoshi_frm = sysMsec2Frame(msec);
}

void yoshi_init(void* party) {
    f32 offset;
    f32 height;

    *(u8*)((s32)party + 0x33) = 4;
    height = float_26_804211e0;
    *(u8*)((s32)party + 0x32) = 0;
    offset = float_20_804211e4;
    *(f32*)((s32)party + 0xF0) = height;
    *(f32*)((s32)party + 0xF4) = offset;
    *(s32*)((s32)party + 0x168) = -1;
}


u8 yoshi_use(s32 pParty) {
    return 0;
}


u8 searchGround(s32 pParty) {
    return 0;
}


u8 checkWall(s64 speed, s64 direction, s32 pParty) {
    return 0;
}


u8 searchGround2(void* pParty) {
    return 0;
}


void inertia(void* pParty) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern f32 float_0_80421138;
    extern f32 float_neg0p07_80421174;
    extern f32 float_neg0p5_80421178;
    extern f32 float_0p01_8042117c;
    extern f32 float_256_80421180;
    extern f32 float_16_80421184;

    void* player = *(void**)((s32)pParty + 0x160);
    f32 speedX = *(f32*)((s32)player + 0x2C8);
    f32 speedZ = *(f32*)((s32)player + 0x2CC);
    f32 stickX = (f32)*(s8*)((s32)pParty + 0x158);
    f32 x;
    f32 z;
    void* party;
    s32 status;
    u8 motion;
    f32 drag;
    f32 limit;
    s32 dir;

    if (stickX == float_0_80421138) {
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            motion = *(u8*)((s32)party + 0x39);
            if (motion >= 0xA && motion <= 0xD) {
                status = 1;
            } else if (motion >= 0x14 && motion <= 0x16) {
                status = 2;
            } else if (motion >= 0x64 && motion <= 0x65) {
                status = 5;
            } else if (motion >= 0x3C && motion <= 0x41) {
                status = 3;
            } else if (motion >= 0x32 && motion <= 0x34) {
                status = 4;
            }
        }
        if (status != 2) {
            speedX = float_0_80421138;
            speedZ = float_0_80421138;
        } else {
            drag = float_neg0p07_80421174;
            if (*(void**)((s32)player + 0x1F4) != 0) {
                drag = float_neg0p5_80421178;
            }
            speedX = speedX * drag + speedX;
            speedZ = speedZ * drag + speedZ;
            if (__fabsf(speedX) < float_0p01_8042117c) {
                speedX = float_0_80421138;
            }
            if (__fabsf(speedZ) < float_0p01_8042117c) {
                speedZ = float_0_80421138;
            }
        }
    } else if (speedX * speedX + speedZ * speedZ <= float_256_80421180) {
        dir = *(s32*)(*(s32*)((s32)pParty + 0x170) + 4);
        speedX = stickX * float_0p01_8042117c + speedX;
        if (dir > 0 && speedX <= float_0_80421138) {
            speedX = float_0_80421138;
        }
        if (dir < 0 && speedX >= float_0_80421138) {
            speedX = float_0_80421138;
        }
        speedZ = speedZ - float_0_80421138;
    } else {
        speedX = stickX * float_0p01_8042117c + speedX;
        limit = -float_16_80421184;
        if (speedX < limit) {
            speedX = limit;
        }
        if (speedX > float_16_80421184) {
            speedX = float_16_80421184;
        }
        speedZ = speedZ - float_0_80421138;
        if (speedZ < limit) {
            speedZ = limit;
        }
        if (speedZ > float_16_80421184) {
            speedZ = float_16_80421184;
        }
    }
    *(f32*)((s32)player + 0x2C8) = speedX;
    *(f32*)((s32)player + 0x2CC) = speedZ;
}

u8 yoshi_exit(void* pParty) {
    return 0;
}


u8 marioYoshiForceCancel(void) {
    return 0;
}


u8 L_getStick(void* pParty, s32 param_2, s32 param_3) {
    return 0;
}


void yoshi_bye(void* pParty) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void partyGoodbyeInit(void* party);
    extern s32 partyGoodbyeMain(void* party);
    extern void partyChgRunMode(void* party, s32 mode);
    void* party;
    s32 status;
    u8 state;

    if ((*(u32*)((s32)pParty + 8) & 8) != 0) {
        *(u32*)((s32)pParty + 8) &= ~8;
        partyGoodbyeInit(pParty);
    }
    if ((*(u32*)pParty & 0x100) != 0) {
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            state = *(u8*)((s32)party + 0x39);
            if (state >= 0xA && state <= 0xD) {
                status = 1;
            } else if (state >= 0x14 && state <= 0x16) {
                status = 2;
            } else if (state >= 0x64 && state <= 0x65) {
                status = 5;
            } else if (state >= 0x3C && state <= 0x41) {
                status = 3;
            } else if (state >= 0x32 && state <= 0x34) {
                status = 4;
            }
        }
        if (status == 1) {
            *(u8*)((s32)pParty + 0x39) = 0x64;
        }
        yoshi_use((s32)pParty);
    } else if (partyGoodbyeMain(pParty) != 0) {
        partyChgRunMode(pParty, 0xE);
    }
}

u8 L_yoshiFlyMove(void* pParty) {
    extern void* marioGetPtr(void);
    extern f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f32 revise360(f32 angle);
    extern f32 float_0_80421138;
    extern f32 float_0p1_80421188;
    extern f32 float_1_80421160;
    extern f32 float_0p01_8042117c;
    void* mario;
    f32 x;
    f32 z;
    f32 speed;
    f32 angle;
    f32 max;

    mario = marioGetPtr();
    inertia(pParty);
    z = -*(f32*)((s32)mario + 0x2CC);
    x = *(f32*)((s32)mario + 0x2C8);
    speed = float_0p1_80421188 * distABf(x, float_0_80421138, float_0_80421138, z);
    max = *(f32*)((s32)mario + 0x188) * float_1_80421160;
    if (speed >= max) {
        speed = max;
    }
    angle = angleABf(x, float_0_80421138, float_0_80421138, z);
    if (speed < float_0p01_8042117c) {
        speed = float_0_80421138;
    }
    *(f32*)((s32)pParty + 0x104) = speed;
    if (speed != float_0_80421138) {
        *(f32*)((s32)pParty + 0x100) = revise360(angle + *(f32*)((s32)pParty + 0xF8));
        *(f32*)((s32)pParty + 0xFC) = revise360(angle + *(f32*)((s32)pParty + 0xF8));
    }
    *(f32*)((s32)mario + 0x18C) = speed;
    return 0;
}

u32 marioGetoffYoshi(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* party;
    void* party2;
    s32 status;
    u8 state;

    party = partyGetPtr(marioGetPartyId());
    if ((*(u32*)party & 0x100) == 0) {
        return 0;
    }
    party2 = partyGetPtr(marioGetPartyId());
    status = 5;
    if (party2 == 0 || *(s8*)((s32)party2 + 0x31) != 4 || (*(u32*)party2 & 0x100) == 0) {
        status = 0;
    } else {
        state = *(u8*)((s32)party2 + 0x39);
        if (state >= 0xA && state <= 0xD) {
            status = 1;
        } else if (state >= 0x14 && state <= 0x16) {
            status = 2;
        } else if (state >= 0x64 && state <= 0x65) {
            status = 5;
        } else if (state >= 0x3C && state <= 0x41) {
            status = 3;
        } else if (state >= 0x32 && state <= 0x34) {
            status = 4;
        }
    }
    if (status == 1) {
        *(u8*)((s32)party + 0x39) = 0x64;
        return 1;
    }
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void L_sweatEntry(void* pParty, void* hitobjStandOn) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* party;
    s32 status;

    *(void**)((s32)pParty + 0x138) = hitobjStandOn;
    party = partyGetPtr(marioGetPartyId());
    status = 5;
    if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
        status = 0;
    } else if (*(u8*)((s32)party + 0x39) >= 0xA && *(u8*)((s32)party + 0x39) <= 0xD) {
        status = 1;
    } else if (*(u8*)((s32)party + 0x39) >= 0x14 && *(u8*)((s32)party + 0x39) <= 0x16) {
        status = 2;
    } else if (*(u8*)((s32)party + 0x39) >= 0x64 && *(u8*)((s32)party + 0x39) <= 0x65) {
        status = 5;
    } else if (*(u8*)((s32)party + 0x39) >= 0x3C && *(u8*)((s32)party + 0x39) <= 0x41) {
        status = 3;
    } else if (*(u8*)((s32)party + 0x39) >= 0x32 && *(u8*)((s32)party + 0x39) <= 0x34) {
        status = 4;
    }
    if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
        *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = hitobjStandOn;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 yoshiGetStatus(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* party;
    u8 state;
    s32 status;

    party = partyGetPtr(marioGetPartyId());
    status = 5;
    if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
        return 0;
    }
    state = *(u8*)((s32)party + 0x39);
    if (state >= 0xA && state <= 0xD) {
        status = 1;
    } else if (state >= 0x14 && state <= 0x16) {
        status = 2;
    } else if (state >= 0x64 && state <= 0x65) {
        status = 5;
    } else if (state >= 0x3C && state <= 0x41) {
        status = 3;
    } else if (state >= 0x32 && state <= 0x34) {
        status = 4;
    }
    return status;
}

void marioRideYoshi(void) {
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    extern void party_force_ride_yoshi(void* party);
    void* party;

    party = partyGetPtr(marioGetPartyId());
    *(u32*)((s32)party + 8) &= ~8;
    *(u32*)((s32)party + 8) &= ~4;
    *(u32*)((s32)party + 8) &= ~2;
    *(u32*)party |= 0x100;
    *(u8*)((s32)party + 0x34) = 3;
    *(u8*)((s32)party + 0x39) = 0xB;
    if (*(void**)((s32)party + 0x170) == 0) {
        *(void**)((s32)party + 0x170) = __memAlloc(0, 0x18);
        memset(*(void**)((s32)party + 0x170), 0, 0x18);
        *(s32*)((s32)*(void**)((s32)party + 0x170) + 0x14) = 0;
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0) {
        party_force_ride_yoshi(party);
    }
}

void L_sweatMain(void* pParty) {
    extern void effDelete(void* eff);
    extern f32 float_18_804211c4;
    void* effect;

    if (*(void**)((s32)pParty + 0x170) != 0) {
        effect = *(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14);
        if (effect != 0) {
            *(f32*)((s32)*(void**)((s32)effect + 0xC) + 4) = *(f32*)((s32)pParty + 0x58);
            *(f32*)((s32)*(void**)((s32)*(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14) + 0xC) + 8) = float_18_804211c4 + *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)*(void**)((s32)*(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14) + 0xC) + 0xC) = *(f32*)((s32)pParty + 0x60);
            if (--*(s32*)((s32)*(void**)((s32)pParty + 0x170) + 0x10) <= 0) {
                effDelete(*(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14));
                *(s32*)((s32)*(void**)((s32)pParty + 0x170) + 0x14) = 0;
            }
        }
    }
}

void yoshi_jumpStand(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);

    void* party = partyGetPtr(marioGetPartyId());
    u8 state;

    if (party != 0) {
        if (*(s8*)((s32)party + 0x31) == 4) {
            if ((*(u32*)party & 0x100) == 0) {
                ;
            } else {
                state = *(u8*)((s32)party + 0x39);
                if (state < 0x3C || state > 0x42) {
                    *(u8*)((s32)party + 0x39) = 0x3C;
                }
            }
        }
    }
}

/* CHATGPT FALLBACK MISSING STUBS: main/party/party_yoshi 20260624_191429 */

/* fallback stub-fill: map=unk_800b6468 addr=0x800b6468 size=0x0000016c */
int unk_800b6468() {
    return 0;
}

/* fallback stub-fill: map=unk_800b65d4 addr=0x800b65d4 size=0x00000164 */
int unk_800b65d4() {
    return 0;
}
