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

void partyChgPose(void* party, const char* pose) {
    const char* current;

    if ((*(u32*)((s32)party + 0x4) & 2) != 0) {
        return;
    }

    current = *(const char**)((s32)party + 0x18);
    if (current != 0 && strcmp(current, pose) == 0) {
        return;
    }

    if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0) {
        return;
    }

    *(const char**)((s32)party + 0x18) = pose;
    *(s16*)((s32)party + 0x20) = 0;
    *(u32*)((s32)party + 0x8) |= 0x2000;
}

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
