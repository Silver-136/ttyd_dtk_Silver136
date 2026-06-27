#include "party/party.h"

void* partyPtrTbl[2];
void animPoseSetMaterialLightFlagOff(void* pose, s32 flag);
s32 marioGetCamId(void);
void partyChgRunMode(void* party, s32 mode);
extern f32 float_18p5_804212d4;

void partyKeyOff(void) {
    ;
}

void partyKeyOn(void) {
    ;
}

void partySetCamId(void* party, s32 camId) {
    *(s32*)((s32)party + 0x164) = camId;
}

void partyShadowOff(void* party) {
    if (party == 0) {
        return;
    }

    *(u32*)((s32)party + 0x4) |= 0x100000;
}

void* partyGetPtr(s32 id) {
    if (id >= 0) {
        return partyPtrTbl[id];
    }

    return 0;
}

void* anotherPartyGetPtr(s32 id) {
    if (id < 0) {
        return 0;
    }

    return partyPtrTbl[(id + 1) & 1];
}

void partyPaperLightOff(void* party) {
    animPoseSetMaterialLightFlagOff(*(void**)((s32)party + 0xC), 1);
}

void partyInit(void) {
    memset(partyPtrTbl, 0, 8);
}

f32 partyGetHeight(void* party) {
    if ((s32)*(u8*)((s32)party + 0x31) == 4) {
        if ((*(u32*)((s32)party + 0x0) & 0x100) != 0) {
            return float_18p5_804212d4 + *(f32*)((s32)party + 0xF0);
        }
    }

    return *(f32*)((s32)party + 0xF0);
}

void partyInitCamId(void* party) {
    *(s32*)((s32)party + 0x164) = marioGetCamId();
}

void L_partyClearCont(void* party) {
    *(u8*)((s32)party + 0x158) = 0;
    *(u8*)((s32)party + 0x159) = 0;
    *(u8*)((s32)party + 0x15A) = 0;
    *(u8*)((s32)party + 0x15B) = 0;
    *(u8*)((s32)party + 0x15C) = 0;
    *(u8*)((s32)party + 0x15D) = 0;
    *(u8*)((s32)party + 0x15E) = 0;
    *(u8*)((s32)party + 0x15F) = 0;
    *(s16*)((s32)party + 0x150) = 0;
    *(s16*)((s32)party + 0x152) = 0;
    *(s16*)((s32)party + 0x154) = 0;
    *(s16*)((s32)party + 0x156) = 0;
}

void partyCtrlOff(void) {
    void* party;

    party = partyPtrTbl[0];
    if (party != 0) {
        *(u32*)((s32)party + 0x0) |= 0x20000000;
    }

    party = partyPtrTbl[1];
    if (party != 0) {
        *(u32*)((s32)party + 0x0) |= 0x20000000;
    }
}

void partyCtrlOn(void) {
    void* party;

    party = partyPtrTbl[0];
    if (party != 0) {
        *(u32*)((s32)party + 0x0) &= ~0x20000000;
    }

    party = partyPtrTbl[1];
    if (party != 0) {
        *(u32*)((s32)party + 0x0) &= ~0x20000000;
    }
}

void partyStop(void* party) {
    partyChgRunMode(party, 0);
}

void partyRun(void* party) {
    if (*(u8*)((s32)party + 0x34) == 0) {
        partyChgRunMode(party, 1);
    }
}

s32 partyRideChk(void* party) {
    s32 result;

    result = 0;

    if (party == 0) {
        return 0;
    }

    if (*(u8*)((s32)party + 0x34) == 0xB) {
        result = 1;

        if (*(u8*)((s32)party + 0x38) >= 2) {
            result = 2;
        }
    }

    return result;
}

s32 partyEntryMain(void);
s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
void partyChgMot(void* party, s32 mot);
void animPoseSetPaperAnimGroup(void* pose, s32 a, s32 b);
void animPaperPoseRelease(s32 id);
s32 strcmp(const char* a, const char* b);
extern u8 partyDataTbl[];

void partySetForceMove(void* party, s32 angle, f32 x, f32 z) {
    *(f32*)((s32)party + 0x100) = x;
    *(s16*)((s32)party + 0x28) = angle;
    *(f32*)((s32)party + 0x104) = z;

    partyChgRunMode(party, 9);
    partyChgMot(party, 1);
}

s32 partyEntryHello(void) {
    s32 id;
    void* party;

    id = partyEntryMain();

    if (id >= 0) {
        party = partyPtrTbl[id];
    } else {
        party = 0;
    }

    partyChgRunMode(party, 4);
    return id;
}

void L_allPartyRideOn(void) {
    s32 id;
    void* party;

    id = marioGetPartyId();
    if (id >= 0) {
        party = partyPtrTbl[id];
    } else {
        party = 0;
    }

    if (party != 0) {
        partyChgRunMode(party, 0xB);
    }

    id = marioGetExtraPartyId();
    if (id >= 0) {
        party = partyPtrTbl[id];
    } else {
        party = 0;
    }

    if (party != 0) {
        partyChgRunMode(party, 0xB);
    }
}

void partyUsePost(void* party) {
    void (*post)(void*);

    if (party == 0) {
        return;
    }

    if (*(u8*)((s32)party + 0x34) != 3) {
        return;
    }

    post = *(void (**)(void*))((s32)partyDataTbl + (s8)*(u8*)((s32)party + 0x31) * 0x28 + 0x10);
    if (post != 0) {
        post(party);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyChgPose(void* party, const char* pose) {
    const char* newPose = pose;
    const char* current;

    if ((*(u32*)((s32)party + 0x4) & 2) != 0) {
        return;
    }

    current = *(const char**)((s32)party + 0x18);
    if (current != 0 && strcmp(current, newPose) == 0) {
        return;
    }

    if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0) {
        return;
    }

    *(const char**)((s32)party + 0x18) = newPose;
    *(s16*)((s32)party + 0x20) = 0;
    *(u32*)((s32)party + 0x8) |= 0x2000;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void partyPaperOff(void* party) {
    s32 paperId;

    if ((*(u32*)((s32)party + 0x4) & 0x4000000) == 0) {
        return;
    }

    if (*(void**)((s32)party + 0x1C) == 0) {
        return;
    }

    animPoseSetPaperAnimGroup(*(void**)((s32)party + 0xC), 0, 0);
    *(void**)((s32)party + 0x1C) = 0;
    *(u32*)((s32)party + 0x4) &= ~0x4000000;

    paperId = *(s32*)((s32)party + 0x14);
    if (paperId >= 0) {
        animPaperPoseRelease(paperId);
        *(s32*)((s32)party + 0x14) = -1;
    }
}



u8 partyRideMain(s32 pParty) {
    return 0;
}


s32 partyDokanInit(void* pParty) {
    return 0;
}



u8 partyReInit(void) {
    return 0;
}


u8 partyHelloMain(void* pParty) {
    return 0;
}


u8 driveParty(void* party) {
    return 0;
}



u8 party_damage_return(void* pParty) {
    return 0;
}


s32 partyGoodbyeMain(void* party) {
    return 0;
}


s32 partyDokanMain(void* pParty) {
    return 0;
}



s32 partyRideYoshiMain(void* pParty) {
    return 0;
}


u8 partyDisp(s32 param_1, void* pParty) {
    return 0;
}


u8 partySetFamicomMode(void* pParty, int param_2) {
    return 0;
}


u8 partyEntry2Pos(void) {
    return 0;
}


u8 N__partyChgRunMode(void* pParty, int currRunModeId, int newRunModeId) {
    return 0;
}


s32 partyEntryMain(void) {
    return 0;
}


int partyEntry2(int partyMemberId) {
    return 0;
}


u8 partyMakeDispDir(void* pParty) {
    return 0;
}


u8 partyPreDisp(s32 pParty) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void getRidePos(void* pParty, void* out) {
    extern f32 toMovedir(f32 angle);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern s32 yoshiGetStatus(void);
    extern s32 marioGetPartyId(void);
    extern f32 toMovedir2(f32 angle, f32 add);
    extern void sincosf(f32 angle, f32* outSin, f32* outCos);
    extern u32 N_marioShipGetDir(void);
    extern f32 toMovedirSimple(f32 angle);
    extern void* partyPtrTbl[2];
    extern f32 float_180_80421214;
    extern f32 float_0_80421218;
    extern f32 float_10_8042123c;
    extern f32 float_20_80421258;
    extern f32 float_16_80421294;
    extern f32 float_3p1416_804212a0;
    extern f32 float_12_804212a4;
    extern f32 float_22_804212a8;
    extern f32 float_40_804212ac;

    void* player = *(void**)((s32)pParty + 0x160);
    void* party;
    f32 angle;
    f32 s;
    f32 c;
    s32 status;
    s32 id;

    if (*(u16*)((s32)player + 0x2E) == 0x1A) {
        angle = (float_3p1416_804212a0 * toMovedir(*(f32*)((s32)player + 0x1B0)) / float_180_80421214);
        status = yoshiGetStatus();
        if (status != 2) {
            *(f32*)((s32)out + 0) = -(float_10_8042123c * (f32)sin(angle) - *(f32*)((s32)player + 0x8C));
            *(f32*)((s32)out + 8) = -(float_10_8042123c * (f32)cos(angle) - *(f32*)((s32)player + 0x94));
            *(f32*)((s32)out + 4) = float_20_80421258 + *(f32*)((s32)player + 0x90);
        } else {
            *(f32*)((s32)out + 0) = -(float_20_80421258 * (f32)sin(angle) - *(f32*)((s32)player + 0x8C));
            *(f32*)((s32)out + 8) = -(float_20_80421258 * (f32)cos(angle) - *(f32*)((s32)player + 0x94));
            *(f32*)((s32)out + 4) = float_16_80421294 + *(f32*)((s32)player + 0x90);
        }
        if (*(s8*)((s32)pParty + 0x2F) != 0) {
            id = marioGetPartyId();
            party = id >= 0 ? partyPtrTbl[id] : 0;
            angle = toMovedir2(*(f32*)((s32)player + 0x1B0), float_10_8042123c);
            sincosf(angle, &s, &c);
            status = yoshiGetStatus();
            if (status != 2) {
                f32 dist = float_12_804212a4;
                f32 yoff = float_20_80421258;
                *(f32*)((s32)out + 0) = -(dist * s - *(f32*)((s32)party + 0x58));
                *(f32*)((s32)out + 8) = -(dist * c - *(f32*)((s32)party + 0x60));
                *(f32*)((s32)out + 4) = yoff + *(f32*)((s32)party + 0x5C);
            } else {
                f32 dist = float_22_804212a8;
                f32 yoff = float_16_80421294;
                *(f32*)((s32)out + 0) = -(dist * s - *(f32*)((s32)party + 0x58));
                *(f32*)((s32)out + 8) = -(dist * c - *(f32*)((s32)party + 0x60));
                *(f32*)((s32)out + 4) = yoff + *(f32*)((s32)party + 0x5C);
            }
        }
    } else if (*(u16*)((s32)player + 0x2E) == 0x18) {
        *(f32*)((s32)out + 0) = *(f32*)((s32)player + 0x8C);
        *(f32*)((s32)out + 8) = *(f32*)((s32)player + 0x94);
        *(f32*)((s32)out + 4) = float_40_804212ac + *(f32*)((s32)player + 0x90);
    } else if (*(u16*)((s32)player + 0x2E) == 0x19) {
        if (N_marioShipGetDir() != 0) {
            angle = float_0_80421218;
        } else {
            angle = float_180_80421214;
        }
        sincosf(toMovedirSimple(angle), &s, &c);
        *(f32*)((s32)out + 0) = float_22_804212a8 * s + *(f32*)((s32)player + 0x8C);
        *(f32*)((s32)out + 8) = float_22_804212a8 * c + *(f32*)((s32)player + 0x94);
        *(f32*)((s32)out + 4) = *(f32*)((s32)player + 0x90);
    } else {
        *(f32*)((s32)out + 0) = *(f32*)((s32)player + 0x8C);
        *(f32*)((s32)out + 8) = *(f32*)((s32)player + 0x94);
        *(f32*)((s32)out + 4) = float_20_80421258 + *(f32*)((s32)player + 0x90);
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntryPos(double xIn, double yIn, double zIn, s32 partyMemberId) {
    extern s32 partyEntryMain(s32 partyMemberId);
    extern void* partyPtrTbl[2];
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern u32 vec3_802cb178[];

    u32 pos[3];
    u32* base;
    s32 pose;
    s32 id;
    void* party;
    void* player;
    s32 weak;
    s32 member;

    base = vec3_802cb178;
    id = partyEntryMain(partyMemberId);
    if (id < 0) {
        party = 0;
    } else {
        party = partyPtrTbl[id];
    }
    *(u32*)((s32)party + 0x70) = base[3];
    pose = 1;
    *(u32*)((s32)party + 0x74) = base[4];
    *(u32*)((s32)party + 0x78) = base[5];
    pos[0] = base[6];
    pos[1] = base[7];
    pos[2] = base[8];
    *(f32*)&pos[0] = xIn;
    *(f32*)&pos[1] = yIn;
    *(f32*)&pos[2] = zIn;
    *(u32*)((s32)party + 0x58) = pos[0];
    *(u32*)((s32)party + 0x5C) = pos[1];
    *(u32*)((s32)party + 0x60) = pos[2];
    player = *(void**)((s32)party + 0x160);
    *(u32*)((s32)party + 0x70) = base[9];
    *(u32*)((s32)party + 0x74) = base[10];
    *(u32*)((s32)party + 0x78) = base[11];
    *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1AC);
    *(f32*)((s32)party + 0x10C) = *(f32*)((s32)player + 0x1AC);
    if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
        weak = 0;
        member = (s8)*(u8*)((s32)party + 0x31);
        if (member < 8 && partyGetHp(member) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            pose = 0xC;
        }
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
            pose = (s32)partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
        } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
            pose = (s32)partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
        } else {
            pose = (s32)yoshiDotPoseTbl[pouchGetPartyColor(4)][pose];
        }
        if (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), (char*)pose) != 0) {
            *(char**)((s32)party + 0x18) = (char*)pose;
            *(s16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 0x8) |= 0x2000;
        }
    }
    *(u8*)((s32)party + 0x34) = 0;
    return id;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntry(s32 partyMemberId) {
    extern s32 partyEntryMain(s32 partyMemberId);
    extern void partyGetAppearPos2(void* party, void* pos);
    extern void* partyPtrTbl[2];
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern u32 vec3_802cb178[];

    u32* base;
    s32 id;
    void* party;
    s32 pose;
    s32 weak;
    s32 member;

    base = vec3_802cb178;
    id = partyEntryMain(partyMemberId);
    if (id < 0) {
        party = 0;
    } else {
        party = partyPtrTbl[id];
    }
    *(u32*)((s32)party + 0x70) = base[0];
    *(u32*)((s32)party + 0x74) = base[1];
    *(u32*)((s32)party + 0x78) = base[2];
    *(u8*)((s32)party + 0x38) = 0xA;
    *(f32*)((s32)party + 0x110) = *(f32*)((s32)*(void**)((s32)party + 0x160) + 0x1AC);
    *(f32*)((s32)party + 0x10C) = *(f32*)((s32)*(void**)((s32)party + 0x160) + 0x1AC);
    partyGetAppearPos2(party, (void*)((s32)party + 0x58));
    pose = 1;
    *(u32*)((s32)party + 0xB8) = *(u32*)((s32)party + 0x58);
    *(u32*)((s32)party + 0xBC) = *(u32*)((s32)party + 0x5C);
    *(u32*)((s32)party + 0xC0) = *(u32*)((s32)party + 0x60);
    if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
        weak = 0;
        member = (s8)*(u8*)((s32)party + 0x31);
        if (member < 8 && partyGetHp(member) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            pose = 0xC;
        }
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
            pose = (s32)partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
        } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
            pose = (s32)partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
        } else {
            pose = (s32)yoshiDotPoseTbl[pouchGetPartyColor(4)][pose];
        }
        if (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), (char*)pose) != 0) {
            *(char**)((s32)party + 0x18) = (char*)pose;
            *(s16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 0x8) |= 0x2000;
        }
    }
    *(u8*)((s32)party + 0x34) = 1;
    return id;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_force_ride_yoshi(void* pParty) {
    extern void N_partyForceChgRunMode(void* party, s32 newRunMode, s32 subMotionId);
    extern void getRidePos(void* party, void* pos);
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern f32 toMovedir(f32 angle);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern f32 float_0_80421218;

    u32 pos[3];
    u32 x;
    u32 y;
    s32 pose;
    s32 weak;
    s32 member;

    N_partyForceChgRunMode(pParty, 0xC, 0xA);
    *(u32*)((s32)pParty + 0x0) &= ~0x100;
    getRidePos(pParty, pos);
    x = pos[0];
    pose = 1;
    y = pos[1];
    *(u32*)((s32)pParty + 0x58) = x;
    *(u32*)((s32)pParty + 0x5C) = y;
    *(u32*)((s32)pParty + 0x60) = pos[2];
    if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
        weak = 0;
        member = (s8)*(u8*)((s32)pParty + 0x31);
        if (member < 8 && partyGetHp(member) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            pose = 0xC;
        }
        if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
            pose = (s32)partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][pose];
        } else if ((s8)*(u8*)((s32)pParty + 0x31) != 4) {
            pose = (s32)partyDotPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][pose];
        } else {
            pose = (s32)yoshiDotPoseTbl[pouchGetPartyColor(4)][pose];
        }
        if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), (char*)pose) != 0) {
            *(char**)((s32)pParty + 0x18) = (char*)pose;
            *(s16*)((s32)pParty + 0x20) = 0;
            *(u32*)((s32)pParty + 0x8) |= 0x2000;
        }
    }
    *(f32*)((s32)pParty + 0x114) = float_0_80421218;
    if (pParty != 0) {
        *(u32*)((s32)pParty + 0x4) |= 0x100000;
    }
    *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)*(void**)((s32)pParty + 0x160) + 0x1B0);
    *(f32*)((s32)pParty + 0xFC) = toMovedir(*(f32*)((s32)pParty + 0x110));
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

void partyHello(void* pParty) {
    extern void partyHelloMain(void* party);
    extern u32 vec3_802cb178[];

    u32* base;
    void* player;
    u16 motion;
    u8 mode;
    u32 x;
    u32 y;

    base = vec3_802cb178;
    if ((*(u32*)((s32)pParty + 0x8) & 8) != 0) {
        *(u32*)((s32)pParty + 0x8) &= ~8;
        player = *(void**)((s32)pParty + 0x160);
        motion = *(u16*)((s32)player + 0x2E);
        if (motion == 0x16 || motion == 0x18 || motion == 0x19) {
            *(u8*)((s32)pParty + 0x38) = 0x1E;
        } else {
            mode = *(u8*)((s32)pParty + 0x34);
            if (mode == 4 || mode == 5) {
                *(u32*)((s32)pParty + 0x4) |= 0x10;
                *(u32*)((s32)pParty + 0x70) = base[0x36];
                *(u32*)((s32)pParty + 0x74) = base[0x37];
                *(u32*)((s32)pParty + 0x78) = base[0x38];
                *(u8*)((s32)pParty + 0x38) = 0;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
                x = *(u32*)((s32)player + 0x8C);
                y = *(u32*)((s32)player + 0x90);
                *(u32*)((s32)pParty + 0x58) = x;
                *(u32*)((s32)pParty + 0x5C) = y;
                *(u32*)((s32)pParty + 0x60) = *(u32*)((s32)player + 0x94);
                *(u32*)((s32)pParty + 0xB8) = *(u32*)((s32)pParty + 0x58);
                *(u32*)((s32)pParty + 0xBC) = *(u32*)((s32)pParty + 0x5C);
                *(u32*)((s32)pParty + 0xC0) = *(u32*)((s32)pParty + 0x60);
            } else if (mode == 7) {
                *(u32*)((s32)pParty + 0x70) = base[0x39];
                *(u32*)((s32)pParty + 0x74) = base[0x3A];
                *(u32*)((s32)pParty + 0x78) = base[0x3B];
                *(u8*)((s32)pParty + 0x38) = 0x14;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
            } else {
                *(u32*)((s32)pParty + 0x70) = base[0x3C];
                *(u32*)((s32)pParty + 0x74) = base[0x3D];
                *(u32*)((s32)pParty + 0x78) = base[0x3E];
                *(u8*)((s32)pParty + 0x38) = 0xA;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
                x = *(u32*)((s32)player + 0x8C);
                y = *(u32*)((s32)player + 0x90);
                *(u32*)((s32)pParty + 0x58) = x;
                *(u32*)((s32)pParty + 0x5C) = y;
                *(u32*)((s32)pParty + 0x60) = *(u32*)((s32)player + 0x94);
                *(u32*)((s32)pParty + 0xB8) = *(u32*)((s32)pParty + 0x58);
                *(u32*)((s32)pParty + 0xBC) = *(u32*)((s32)pParty + 0x5C);
                *(u32*)((s32)pParty + 0xC0) = *(u32*)((s32)pParty + 0x60);
            }
        }
        *(u32*)((s32)pParty + 0x0) &= ~0x100;
    }
    partyHelloMain(pParty);
}

void partyChgPoseId(void* party, s32 poseId) {
    ;
}


s32 partyPaperOn(void* pParty, char* param_2) {
    return 0;
}


void partyUpdateKeyData(void* party) {
    extern void* gp;
    s32 port;
    s32 offset;

    port = (s8)*(u8*)((s32)party + 0x30);
    if ((*(u32*)party & 0x60000000) != 0) {
        L_partyClearCont(party);
        return;
    }
    *(u8*)((s32)party + 0x158) = *(u8*)((s32)gp + port + 0x13B8);
    *(u8*)((s32)party + 0x159) = -*(u8*)((s32)gp + port + 0x13BC);
    *(u8*)((s32)party + 0x15A) = *(u8*)((s32)gp + port + 0x13C0);
    *(u8*)((s32)party + 0x15B) = -*(u8*)((s32)gp + port + 0x13C4);
    *(u8*)((s32)party + 0x15C) = *(u8*)((s32)gp + port + 0x13C8);
    *(u8*)((s32)party + 0x15D) = *(u8*)((s32)gp + port + 0x13CC);
    *(u8*)((s32)party + 0x15E) = 0;
    *(u8*)((s32)party + 0x15F) = 0;
    offset = port << 2;
    *(u16*)((s32)party + 0x150) = *(u32*)((s32)gp + offset + 0x1328);
    *(u16*)((s32)party + 0x152) = *(u32*)((s32)gp + offset + 0x1338);
    *(u16*)((s32)party + 0x154) = *(u32*)((s32)gp + offset + 0x1348);
    *(u16*)((s32)party + 0x156) = *(u32*)((s32)gp + offset + 0x1368);
}

u8 partyMain(void) {
    return 0;
}


void allPartyForceRideOn(void) {
    extern u8 N_partyForceChgRunMode(void* party, int newRunMode, u8 subMotionId);
    s32 id;
    void* party;

    id = marioGetPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        N_partyForceChgRunMode(party, 0xB, 0xB);
        *(u32*)((s32)party + 4) |= 1;
        if (party != 0) {
            *(u32*)((s32)party + 4) |= 0x100000;
        }
    }
    id = marioGetExtraPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        N_partyForceChgRunMode(party, 0xB, 0xB);
        *(u32*)((s32)party + 4) |= 1;
        if (party != 0) {
            *(u32*)((s32)party + 4) |= 0x100000;
        }
    }
}

void partyGoodbyeInit(void* party) {
    ;
}


void allPartyRideOff2(void) {
    s32 id;
    void* party;

    id = marioGetPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0 && *(u8*)((s32)party + 0x34) == 0xB) {
        if ((*(u32*)((s32)party + 8) & 8) == 0) {
            *(u8*)((s32)party + 0x38) = 0x65;
        } else {
            *(u8*)((s32)party + 0x34) = 1;
        }
    }
    id = marioGetExtraPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0 && *(u8*)((s32)party + 0x34) == 0xB) {
        if ((*(u32*)((s32)party + 8) & 8) == 0) {
            *(u8*)((s32)party + 0x38) = 0x65;
        } else {
            *(u8*)((s32)party + 0x34) = 1;
        }
    }
}

void allPartyRideOff(void) {
    s32 id;
    void* party;

    id = marioGetPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0 && *(u8*)((s32)party + 0x34) == 0xB) {
        if ((*(u32*)((s32)party + 8) & 8) == 0) {
            *(u8*)((s32)party + 0x38) = 0x64;
        } else {
            *(u8*)((s32)party + 0x34) = 1;
        }
    }
    id = marioGetExtraPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0 && *(u8*)((s32)party + 0x34) == 0xB) {
        if ((*(u32*)((s32)party + 8) & 8) == 0) {
            *(u8*)((s32)party + 0x38) = 0x64;
        } else {
            *(u8*)((s32)party + 0x34) = 1;
        }
    }
}

u8 partyDokanEnd(int partyId, u8 param_2) {
    return 0;
}


u8 partyDoWork(void) {
    return 0;
}


void yoshiSetColor(void) {
    ;
}


void allPartyRideShip(void) {
    extern void bomhei_use_cancel(void);
    s32 id;
    void* party;

    id = marioGetPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        bomhei_use_cancel();
        *(u32*)party |= 0x20000;
        partyChgRunMode(party, 0xB);
    }
    party = partyPtrTbl[1];
    if (party != 0) {
        *(u32*)party |= 0x20000;
        partyChgRunMode(party, 0xB);
    }
}

void allPartyRidePlane(void) {
    extern void bomhei_use_cancel(void);
    s32 id;
    void* party;

    id = marioGetPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        bomhei_use_cancel();
        *(u32*)party |= 0x10000;
        partyChgRunMode(party, 0xB);
    }
    party = partyPtrTbl[1];
    if (party != 0) {
        *(u32*)party |= 0x10000;
        partyChgRunMode(party, 0xB);
    }
}

void party_ride_yoshi_force_move(void) {
    extern void getRidePos(void* party, void* pos);
    extern f32 toMovedir(f32 angle);
    s32 id;
    void* party;
    u32 pos[3];

    id = marioGetExtraPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        getRidePos(party, pos);
        *(u32*)((s32)party + 0x58) = pos[0];
        *(u32*)((s32)party + 0x5C) = pos[1];
        *(u32*)((s32)party + 0x60) = pos[2];
        *(f32*)((s32)party + 0x110) = *(f32*)((s32)*(void**)((s32)party + 0x160) + 0x1B0);
        *(f32*)((s32)party + 0xFC) = toMovedir(*(f32*)((s32)party + 0x110));
    }
}

u8 N_partyForceChgRunMode(void* pParty, int newRunMode, u8 subMotionId) {
    return 0;
}


u8 partyChgPaper(void* pParty, char* param_2) {
    return 0;
}


void partyChgRunMode(void* party, s32 mode) {
    ;
}


s32 partyGoodbye(void* pParty) {
    return 0;
}


s32 partyKill(s32 id) {
    return 0;
}


s32 partyEntry2Hello(s32 id) {
    return 0;
}


s32 partyKill2(s32 id) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800bc660(void* party) {
    extern u32 vec3_802cb178[];
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern u8 partyDataTbl[];
    extern void partyClearJumpPara(void* party);
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern void partyChgMot(void* party, s32 mot);

    u32* base;
    u32 mode;
    u32 previousMode;
    void* player;
    s32 pose;
    s32 weak;
    s32 member;
    void (*callback)(void*);

    base = vec3_802cb178;
    mode = *(u8*)((s32)party + 0x34);
    previousMode = *(u8*)((s32)party + 0x35);

    if (mode == previousMode) {
        return;
    }

    if (mode == 4) {
        player = *(void**)((s32)party + 0x160);

        *(u32*)((s32)party + 0x70) = base[0x45];
        *(u32*)((s32)party + 0x74) = base[0x46];
        *(u32*)((s32)party + 0x78) = base[0x47];

        *(u32*)((s32)party + 0x4) &= ~0x10;

        *(f32*)((s32)party + 0xFC) = *(f32*)((s32)player + 0x1A0);
        *(f32*)((s32)party + 0x100) = *(f32*)((s32)player + 0x1A4);
        *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1B0);
        *(f32*)((s32)party + 0x10C) = *(f32*)((s32)player + 0x1AC);

        partyClearJumpPara(party);

        pose = 1;
        if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
            weak = 0;
            member = (s8)*(u8*)((s32)party + 0x31);
            if (member < 8 && partyGetHp(member) <= 5) {
                weak = 1;
            }
            if (weak == 1) {
                pose = 0xC;
            }

            if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
                pose = (s32)partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
            } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
                pose = (s32)partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
            } else {
                pose = (s32)yoshiDotPoseTbl[pouchGetPartyColor(4)][pose];
            }

            if (*(char**)((s32)party + 0x18) == 0 ||
                strcmp(*(char**)((s32)party + 0x18), (char*)pose) != 0) {
                *(char**)((s32)party + 0x18) = (char*)pose;
                *(s16*)((s32)party + 0x20) = 0;
                *(u32*)((s32)party + 0x8) |= 0x2000;
            }
        }

        *(u32*)((s32)party + 0x4) &= ~0x10;
        *(u8*)((s32)party + 0x34) = 1;

        if (party != 0 && *(u8*)((s32)party + 0x34) == 3) {
            member = (s8)*(u8*)((s32)party + 0x31);
            callback = *(void (**)(void*))(partyDataTbl + member * 0x28 + 0x10);
            if (callback != 0) {
                callback(party);
            }
        }

        partyChgMot(party, 0);
        return;
    }

    if (mode == 0xB) {
        player = *(void**)((s32)party + 0x160);

        *(u32*)((s32)party + 0x70) = base[0x66];
        *(u32*)((s32)party + 0x74) = base[0x67];
        *(u32*)((s32)party + 0x78) = base[0x68];
        *(u32*)((s32)party + 0x7C) = base[0x69];
        *(u32*)((s32)party + 0x80) = base[0x6A];
        *(u32*)((s32)party + 0x84) = base[0x6B];

        *(f32*)((s32)party + 0xFC) = *(f32*)((s32)player + 0x1A0);
        *(f32*)((s32)party + 0x100) = *(f32*)((s32)player + 0x1A4);
        *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1B0);

        *(u32*)party &= ~0x70000;

        partyClearJumpPara(party);

        pose = 1;
        if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
            weak = 0;
            member = (s8)*(u8*)((s32)party + 0x31);
            if (member < 8 && partyGetHp(member) <= 5) {
                weak = 1;
            }
            if (weak == 1) {
                pose = 0xC;
            }

            if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
                pose = (s32)partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
            } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
                pose = (s32)partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][pose];
            } else {
                pose = (s32)yoshiDotPoseTbl[pouchGetPartyColor(4)][pose];
            }

            if (*(char**)((s32)party + 0x18) == 0 ||
                strcmp(*(char**)((s32)party + 0x18), (char*)pose) != 0) {
                *(char**)((s32)party + 0x18) = (char*)pose;
                *(s16*)((s32)party + 0x20) = 0;
                *(u32*)((s32)party + 0x8) |= 0x2000;
            }
        }

        *(u32*)((s32)party + 0x4) &= ~0x10;
        *(u32*)((s32)party + 0x4) &= ~1;

        if (party != 0) {
            *(u32*)((s32)party + 0x4) &= ~0x100000;
        }

        if (party != 0 && *(u8*)((s32)party + 0x34) == 3) {
            member = (s8)*(u8*)((s32)party + 0x31);
            callback = *(void (**)(void*))(partyDataTbl + member * 0x28 + 0x10);
            if (callback != 0) {
                callback(party);
            }
        }

        partyChgMot(party, 0);
        return;
    }

    if (mode == 0xC) {
        if (party != 0) {
            *(u32*)((s32)party + 0x4) &= ~0x100000;
        }
    }

    if (party != 0 && *(u8*)((s32)party + 0x34) == 3) {
        member = (s8)*(u8*)((s32)party + 0x31);
        callback = *(void (**)(void*))(partyDataTbl + member * 0x28 + 0x10);
        if (callback != 0) {
            callback(party);
        }
    }

    *(u8*)((s32)party + 0x34) = *(u8*)((s32)party + 0x35);
    *(u32*)((s32)party + 0x8) |= 8;
    partyChgMot(party, 0);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/party/party 20260624_183901 */

/* stub-fill: partyGetAppearPos5 | missing_definition | ghidra_signature */
u8 partyGetAppearPos5(void* pParty, s32 param_2, s32 param_3) {
    return 0;
}

/* stub-fill: partyGetAppearPos4 | missing_definition | ghidra_signature */
u8 partyGetAppearPos4(void) {
    return 0;
}

/* stub-fill: partyGetAppearPos3 | missing_definition | ghidra_signature */
u8 partyGetAppearPos3(void* party, s32 param_2) {
    return 0;
}

/* stub-fill: partyGetAppearPos2 | prototype_only | source_prototype */
void partyGetAppearPos2(void* party, void* pos) {
    return;
}

/* stub-fill: partyGetAppearPos | missing_definition | ghidra_signature */
u8 partyGetAppearPos(void* party, s32 param_2) {
    return 0;
}
