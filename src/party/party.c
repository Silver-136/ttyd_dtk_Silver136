#include "party/party.h"

void* partyPtrTbl[2];
void animPoseSetMaterialLightFlagOff(void* pose, s32 flag);
s32 marioGetCamId(void);
void partyChgRunMode(void* party, s32 mode);
extern f32 float_18p5_804212d4;

void partyKeyOff(void) {
}

void partyKeyOn(void) {
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


u8 getRidePos(void* pParty, void* param_2) {
    return 0;
}


u8 partyEntryPos(double param_1, double param_2, double param_3, s32 param_4) {
    return 0;
}


int partyEntry(int partyMemberId) {
    return 0;
}


u8 party_force_ride_yoshi(void* pParty) {
    return 0;
}


u8 partyHello(void* pParty) {
    return 0;
}


void partyChgPoseId(void* party, s32 poseId) {
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
