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
    extern void getRidePos(void*, void*);
    extern f32 distABf(f32, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern void movePos(f32, f32, f32*, f32*);
    extern void partyMoveNoHosei(void*);
    extern void partyChgRunMode(void*, s32);
    extern void partyGetAppearPos(void*, void*);
    extern void partyGetAppearPos2(void*, void*);
    extern void partyClearJumpPara(void*);
    void* party = (void*)pParty;
    void* player = *(void**)(pParty + 0x160);
    f32 ride[3];
    u8 state = *(u8*)(pParty + 0x38);

    if (state == 0) {
        getRidePos(party, ride);
        *(f32*)(pParty + 0xB8) = ride[0];
        *(f32*)(pParty + 0xBC) = ride[1];
        *(f32*)(pParty + 0xC0) = ride[2];
        *(f32*)(pParty + 0x100) = (f32)distABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60), ride[0], ride[2]);
        *(f32*)(pParty + 0x104) = (f32)angleABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60), ride[0], ride[2]);
        *(s32*)(pParty + 0x9C) = 30;
        *(f32*)(pParty + 0x100) /= 30.0f;
        *(f32*)(pParty + 0x108) = 0.4f;
        *(f32*)(pParty + 0xFC) = (0.5f * 30.0f * 0.4f * 30.0f + (ride[1] - *(f32*)(pParty + 0x5C))) / 30.0f;
        *(f32*)(pParty + 0x124) = 1.0f;
        *(u32*)(pParty + 4) |= 0x400;
        *(u8*)(pParty + 0x38) = 1;
    } else if (state == 1) {
        *(f32*)(pParty + 0xFC) -= *(f32*)(pParty + 0x108);
        *(f32*)(pParty + 0x5C) += *(f32*)(pParty + 0xFC);
        movePos(*(f32*)(pParty + 0x100), *(f32*)(pParty + 0x104), (f32*)(pParty + 0x58), (f32*)(pParty + 0x60));
        *(f32*)(pParty + 0x124) += 2.0f;
        if (*(f32*)(pParty + 0x124) > 30.0f) *(f32*)(pParty + 0x124) = 30.0f;
        if (--*(s32*)(pParty + 0x9C) < 0) *(u8*)(pParty + 0x38) = 2;
    } else if (state == 2) {
        *(f32*)(pParty + 0xFC) = 0.0f;
        *(f32*)(pParty + 0x5C) = *(f32*)(pParty + 0xBC);
        *(u32*)(pParty + 4) |= 0x1000000;
        *(u8*)(pParty + 0x38) = 10;
    } else if (state == 10) {
        *(f32*)(pParty + 0x58) = *(f32*)((s32)player + 0x8C);
        *(f32*)(pParty + 0x5C) = *(f32*)((s32)player + 0x90);
        *(f32*)(pParty + 0x60) = *(f32*)((s32)player + 0x94);
    } else if (state == 20) {
        *(u8*)(pParty + 0x38) = 21;
        *(s32*)(pParty + 0x9C) = 16;
        *(f32*)(pParty + 0xFC) = 16.0f;
        *(f32*)(pParty + 0x110) = 0.5f;
    } else if (state == 21) {
        *(f32*)(pParty + 0x5C) += *(f32*)(pParty + 0xFC);
        *(f32*)(pParty + 0xFC) += *(f32*)(pParty + 0x110);
        if (--*(s32*)(pParty + 0x9C) < 1) {
            *(u8*)(pParty + 0x38) = 22;
            *(u32*)pParty |= 0x40000;
        }
    } else if (state == 100 || state == 101) {
        *(u32*)(pParty + 4) &= ~0x1000000;
        *(f32*)(pParty + 0x58) = *(f32*)((s32)player + 0x8C);
        *(f32*)(pParty + 0x5C) = *(f32*)((s32)player + 0x90) + 0.5f * *(f32*)((s32)player + 0xE4);
        *(f32*)(pParty + 0x60) = *(f32*)((s32)player + 0x94);
        if (state == 100) partyGetAppearPos(party, (void*)(pParty + 0xB8));
        else partyGetAppearPos2(party, (void*)(pParty + 0xB8));
        *(s32*)(pParty + 0x9C) = 16;
        *(u8*)(pParty + 0x38) = 110;
    } else if (state == 110) {
        partyMoveNoHosei(party);
        if (*(f32*)(pParty + 0xFC) <= 0.0f) *(u32*)pParty &= ~0x40000;
    } else if (state == 111 || state == 200) {
        partyClearJumpPara(party);
        *(u32*)pParty &= ~0x70000;
        *(f32*)(pParty + 0x118) = 0.0f;
        *(f32*)(pParty + 0xA4) = 1.0f;
        *(f32*)(pParty + 0xA8) = 1.0f;
        *(f32*)(pParty + 0xAC) = 1.0f;
        partyChgRunMode(party, 1);
    }
    *(f32*)(pParty + 0xE0) = *(f32*)(pParty + 0x58);
    *(f32*)(pParty + 0xE4) = *(f32*)(pParty + 0x5C);
    *(f32*)(pParty + 0xE8) = *(f32*)(pParty + 0x60);
    return 0;
}

s32 partyDokanInit(void* party) {
    extern f64 revise360(f64);
    extern void animPaperPoseRelease(s32);
    extern s32 animPaperPoseEntry(char*, s32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern void animPoseSetMaterialLightFlagOff(s32, s32);
    extern char str_p_dokan_x_802cb37c[];
    extern char str_p_dokan_y_802cb360[];
    extern char str_PM_D_1A_802cb390[];
    void* player = *(void**)((s32)party + 0x160);
    u8 dokan = *(u8*)((s32)player + 0x249);
    char* paper = (dokan == 4 || dokan == 5) ? str_p_dokan_x_802cb37c : str_p_dokan_y_802cb360;
    s32 poseId = *(s32*)((s32)party + 0x14);

    *(f32*)((s32)party + 0x10C) = (f32)revise360(*(f32*)((s32)player + 0x1AC) + 180.0f);
    *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1AC);
    *(f32*)((s32)party + 0xFC) = *(f32*)((s32)player + 0x1A4);
    *(f32*)((s32)party + 0x58) = *(f32*)((s32)player + 0x8C);
    *(f32*)((s32)party + 0x5C) = *(f32*)((s32)player + 0x90);
    *(f32*)((s32)party + 0x60) = *(f32*)((s32)player + 0x94);
    *(u32*)((s32)party + 4) |= 0x400;
    if (poseId >= 0) animPaperPoseRelease(poseId);
    *(s32*)((s32)party + 0x14) = animPaperPoseEntry(paper, 2);
    *(u32*)((s32)party + 4) |= 0x1000;
    animPoseSetPaperAnimGroup(*(s32*)((s32)party + 0x10), paper, 0);
    *(char**)((s32)party + 0x18) = str_PM_D_1A_802cb390;
    *(s32*)((s32)party + 0x24) = 0;
    *(u32*)((s32)party + 8) |= 0x2000;
    if (dokan == 4) {
        *(u8*)((s32)party + 0x38) = 10;
        *(f32*)((s32)party + 0x8C) = 10.0f;
        *(f32*)((s32)party + 0x124) = (*(f32*)((s32)player + 0x1B0) < 90.0f ||
                                      *(f32*)((s32)player + 0x1B0) > 270.0f) ? -30.0f : 30.0f;
        *(f32*)((s32)party + 0x70) = -4.0f;
    } else {
        *(u8*)((s32)party + 0x38) = 0;
        *(f32*)((s32)party + 0x124) = 30.0f;
        *(f32*)((s32)party + 0xA4) = 1.0f;
        *(f32*)((s32)party + 0xA8) = 1.0f;
        *(f32*)((s32)party + 0xAC) = 1.0f;
    }
    animPoseSetMaterialLightFlagOff(*(s32*)((s32)party + 0x10), 1);
    return 1;
}

u8 partyReInit(void) {
    extern void* partyPtrTbl[2];
    extern s32 marioGetExtraPartyId(void);
    extern s32 marioGetPartyId(void);
    extern void* marioGetPtr(void);
    extern s32 marioGetCamId(void);
    extern void animPoseSetMaterialFlagOff(s32, u32);
    extern void animPoseSetMaterialLightFlagOn(s32, u32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern void animPaperPoseRelease(s32);
    extern void partyChgRunMode(void*, s32);
    extern void partyChgMot(void*, s32);
    extern void partyClearFootmark(void);
    extern void chuchu_reinit(void);
    extern void unk_JP_US_EU_36_80151d20(void);
    extern s32 partyGetHp(s32);
    extern s32 pouchGetPartyColor(s32);
    extern s32 strcmp(const char*, const char*);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    void* party;
    void* player;
    char* pose;
    s32 id;
    s32 member;
    s32 poseId;

    id = marioGetExtraPartyId();
    party = id < 0 ? 0 : partyPtrTbl[id];
    if (party != 0) {
        *(u32*)party = 0xD;
        *(u32*)((s32)party + 4) = 0;
        if (*(s32*)((s32)party + 0xC) >= 0) {
            animPoseSetMaterialFlagOff(*(s32*)((s32)party + 0xC), 0xFFFFFFFF);
            animPoseSetMaterialLightFlagOn(*(s32*)((s32)party + 0xC), 2);
        }
        *(u8*)((s32)party + 0x37) = 0;
        if (*(char**)((s32)party + 0x1C) != 0) {
            animPoseSetPaperAnimGroup(*(s32*)((s32)party + 0xC), 0, 0);
            *(char**)((s32)party + 0x1C) = 0;
            *(u32*)((s32)party + 4) &= ~0x4000000;
            if (*(s32*)((s32)party + 0x14) >= 0) {
                animPaperPoseRelease(*(s32*)((s32)party + 0x14));
            }
            *(s32*)((s32)party + 0x14) = -1;
        }
        *(void**)((s32)party + 0x130) = 0;
        *(void**)((s32)party + 0x12C) = 0;
        *(void**)((s32)party + 0x148) = 0;
        *(void**)((s32)party + 0x138) = 0;
        *(void**)((s32)party + 0x13C) = 0;
        *(void**)((s32)party + 0x134) = 0;
        *(void**)((s32)party + 0x140) = 0;
        *(void**)((s32)party + 0x144) = 0;
        member = *(s8*)((s32)party + 0x31);
        poseId = (member < 8 && partyGetHp(member) < 6) ? 0xC : 1;
        if ((*(u32*)((s32)party + 4) & 0x10000) == 0) {
            pose = partyPoseTbl[member][poseId];
        } else if (member == 4) {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
        } else {
            pose = partyDotPoseTbl[member][poseId];
        }
        if ((*(u32*)((s32)party + 4) & 2) == 0 &&
            (*(char**)((s32)party + 0x18) == 0 ||
             strcmp(*(char**)((s32)party + 0x18), pose) != 0)) {
            *(char**)((s32)party + 0x18) = pose;
            *(u16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 8) |= 0x1000;
        }
        partyChgRunMode(party, 2);
        partyChgMot(party, 0);
        *(s32*)((s32)party + 0x168) = -1;
        *(f32*)((s32)party + 0x70) = 1.0f;
        *(f32*)((s32)party + 0x74) = 1.0f;
        *(f32*)((s32)party + 0x78) = 1.0f;
    }

    id = marioGetPartyId();
    party = id < 0 ? 0 : partyPtrTbl[id];
    if (party != 0) {
        *(u32*)party = 1;
        *(u32*)((s32)party + 4) = 0;
        *(u8*)((s32)party + 0x37) = 0;
        *(s32*)((s32)party + 0x164) = marioGetCamId();
        if (*(s32*)((s32)party + 0xC) >= 0) {
            animPoseSetMaterialFlagOff(*(s32*)((s32)party + 0xC), 0xFFFFFFFF);
            animPoseSetMaterialLightFlagOn(*(s32*)((s32)party + 0xC), 2);
        }
        if (*(char**)((s32)party + 0x1C) != 0) {
            animPoseSetPaperAnimGroup(*(s32*)((s32)party + 0xC), 0, 0);
            *(char**)((s32)party + 0x1C) = 0;
            *(u32*)((s32)party + 4) &= ~0x4000000;
            if (*(s32*)((s32)party + 0x14) >= 0) {
                animPaperPoseRelease(*(s32*)((s32)party + 0x14));
            }
            *(s32*)((s32)party + 0x14) = -1;
        }
        *(void**)((s32)party + 0x130) = 0;
        *(void**)((s32)party + 0x12C) = 0;
        *(void**)((s32)party + 0x148) = 0;
        *(void**)((s32)party + 0x138) = 0;
        *(void**)((s32)party + 0x13C) = 0;
        *(void**)((s32)party + 0x134) = 0;
        *(void**)((s32)party + 0x140) = 0;
        *(void**)((s32)party + 0x144) = 0;
        partyClearFootmark();
        *(f32*)((s32)party + 0x70) = 1.0f;
        *(f32*)((s32)party + 0x74) = 1.0f;
        *(f32*)((s32)party + 0x78) = 1.0f;
        player = marioGetPtr();
        partyChgRunMode(party, 2);
        if ((*(u32*)player & 0x2000000) == 0) {
            partyChgMot(party, 0);
        } else {
            *(u8*)((s32)party + 0x3B) = 6;
            *(u8*)((s32)party + 0x38) = 0;
            *(u32*)party |= 0x8000;
        }
        if (*(s8*)((s32)party + 0x31) == 6) {
            chuchu_reinit();
        }
        unk_JP_US_EU_36_80151d20();
    }
    return 0;
}

u8 partyHelloMain(void* pParty) {
    extern void partyGetAppearPos(void*, void*);
    extern void partyGetAppearPos2(void*, void*);
    extern f32 distABf(f32,f32,f32,f32);
    extern f32 angleABf(f32,f32,f32,f32);
    extern void partyMoveNoHosei(void*);
    extern f32 partyToMovedir(f32,void*);
    extern void partyClearJumpPara(void*);
    extern void allPartyForceRideOn(void);
    extern u32 psndSFXOn_3D(s32,void*);
    extern f32 float_0p5_80421288;
    extern f32 float_0_80421218;
    extern f32 float_1_80421200;
    extern f32 float_10_8042123c;
    extern f32 float_neg1p6_80421298;
    extern f32 float_neg2000_8042127c;
    extern f32 float_0p0625_8042129c;
    extern f32 float_90_80421208;
    extern f32 float_270_8042120c;
    extern f32 float_180_80421214;
    void* player = *(void**)((s32)pParty + 0x160);
    s32 timer;
    f32 frames;
    f32 delta;
    f32 peak;
    f32 scale;

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)pParty + 0x5C) =
                float_0p5_80421288 * *(f32*)((s32)player + 0x1BC) +
                *(f32*)((s32)player + 0x90);
            *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
            partyGetAppearPos(pParty, (void*)((s32)pParty + 0x94));
            *(s32*)((s32)pParty + 0x24) = 16;
            *(f32*)((s32)pParty + 0x128) =
                distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                        *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) =
                angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                         *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
            frames = 16.0f;
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / frames;
            delta = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
            peak = float_10_8042123c;
            if (delta >= float_0_80421218 && delta >= float_1_80421200) {
                peak = float_0p5_80421288 * *(f32*)((s32)pParty + 0xF0) + delta;
            }
            delta = (peak + peak) / (8.0f * 8.0f - 8.0f);
            *(f32*)((s32)pParty + 0x114) = delta * 8.0f;
            *(f32*)((s32)pParty + 0x118) = -delta;
            *(u32*)pParty |= 0x10;
            *(u8*)((s32)pParty + 0x38) = 1;
            break;
        case 1:
            partyMoveNoHosei(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_0_80421218) {
                *(u32*)pParty &= ~0x10;
                *(f32*)((s32)pParty + 0x118) = float_neg1p6_80421298;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_8042127c) {
                *(s32*)((s32)pParty + 0x24) = 0;
                *(u8*)((s32)pParty + 0x38) = 2;
                return 0;
            }
            if (*(void**)((s32)pParty + 0x138) == 0) {
                timer = *(s32*)((s32)pParty + 0x24);
                scale = (16.0f - (f32)timer) * float_0p0625_8042129c;
                *(f32*)((s32)pParty + 0x70) = scale;
                *(f32*)((s32)pParty + 0x74) = scale;
                *(f32*)((s32)pParty + 0x78) = scale;
                if (--timer > 0) {
                    *(s32*)((s32)pParty + 0x24) = timer;
                    return 0;
                }
                *(s32*)((s32)pParty + 0x24) = 0;
                return 0;
            }
            *(u8*)((s32)pParty + 0x38) = 2;
        case 2:
            timer = *(s32*)((s32)pParty + 0x24);
            scale = (16.0f - (f32)timer) * float_0p0625_8042129c;
            *(f32*)((s32)pParty + 0x70) = scale;
            *(f32*)((s32)pParty + 0x74) = scale;
            *(f32*)((s32)pParty + 0x78) = scale;
            timer--;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                *(s32*)((s32)pParty + 0x24) = 0;
                if (*(f32*)((s32)pParty + 0x108) < float_90_80421208 ||
                    *(f32*)((s32)pParty + 0x108) > float_270_8042120c) {
                    *(f32*)((s32)pParty + 0xFC) = partyToMovedir(float_0_80421218, pParty);
                    *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)pParty + 0xFC);
                    *(f32*)((s32)pParty + 0x104) = float_0_80421218;
                    *(f32*)((s32)pParty + 0x108) = float_0_80421218;
                } else {
                    *(f32*)((s32)pParty + 0xFC) = partyToMovedir(float_180_80421214, pParty);
                    *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)pParty + 0xFC);
                    *(f32*)((s32)pParty + 0x104) = float_180_80421214;
                    *(f32*)((s32)pParty + 0x108) = float_180_80421214;
                }
                partyClearJumpPara(pParty);
                *(u32*)((s32)pParty + 4) &= ~0x8000;
                *(u8*)((s32)pParty + 0x34) = 1;
                psndSFXOn_3D(0x20920, (void*)((s32)player + 0x8C));
                return 1;
            }
            break;
        case 10:
            partyGetAppearPos2(pParty, (void*)((s32)pParty + 0x58));
            *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0x58);
            *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0x60);
            *(u8*)((s32)pParty + 0x34) = 1;
            return 1;
        case 20:
            *(u8*)((s32)pParty + 0x34) = 0;
            return 1;
        case 30:
            allPartyForceRideOn();
            return 1;
    }
    return 0;
}

u8 driveParty(void* party) {
    extern void unk_JP_US_EU_13_800ca884(void*);
    extern void partyHelloMain(void*);extern void partyDokanInit(void*);extern void partyDokanMain(void*);extern void partyRideMain(void*);extern void partyRideYoshiMain(void*);extern void party_damage_return(void*);extern void partyForceWalk(void*);
    extern void partyPreDisp(void*);extern void party_kemuri(void*);extern void partyChgMot(void*,s32);extern void L_partyForceSlitOff(void*);
    extern s32 vivianGetStatus(void);extern void vivianShadow(void*);
    extern void marioGetScreenPos(void*,f32*,f32*,f32*);extern u32 marioChkInScreen(s32,s32);
    void* player=*(void**)((s32)party+0x160);u8 mode=*(u8*)((s32)party+0x34);f32 sx,sy,sz;u32 flags=*(u32*)party;
    *(f32*)((s32)party+0xF8)=*(f32*)((s32)player+0x19C);if(*(s16*)((s32)party+0x2A)>0)(*(u16*)((s32)party+0x2A))--;
    if(mode==0xE||((mode<9&&mode>3)||(flags&0x20000000)==0)){
        if((*(u32*)player&0x4000000)==0)*(u32*)party&=~0x4000000;else *(u32*)party|=0x4000000;
        *(void**)((s32)party+0x138)=0;*(void**)((s32)party+0x12C)=0;*(void**)((s32)party+0x13C)=0;*(void**)((s32)party+0x130)=0;*(void**)((s32)party+0x134)=0;
        switch(mode){
            case 0:if((*(u32*)((s32)party+8)&0x1000)!=0){*(u32*)((s32)party+8)&=~0x1000;*(f32*)((s32)party+0x104)=0.0f;*(u32*)party&=~0x100;partyChgMot(party,0);}break;
            case 1:unk_JP_US_EU_13_800ca884(party);break;
            case 4:case 5:case 6:case 7:if((*(u32*)((s32)party+8)&0x1000)!=0){*(u32*)((s32)party+8)&=~0x1000;if(*(u16*)((s32)player+0x2E)==0x12||*(u16*)((s32)player+0x2E)==0x13||*(u16*)((s32)player+0x2E)==0x14)*(u8*)((s32)party+0x38)=30;else if(mode==7)*(u8*)((s32)party+0x38)=20;else if(mode==6)*(u8*)((s32)party+0x38)=10;else *(u8*)((s32)party+0x38)=0;*(f32*)((s32)party+0x104)=*(f32*)((s32)player+0x1AC);*(f32*)((s32)party+0x108)=*(f32*)((s32)player+0x1AC);*(u32*)party&=~0x100;}partyHelloMain(party);break;
            case 9:partyForceWalk(party);break;
            case 10:if((*(u32*)((s32)party+8)&0x1000)!=0){*(u32*)((s32)party+8)&=~0x1000;partyDokanInit(party);*(u32*)party&=~0x100;}partyDokanMain(party);break;
            case 11:if((*(u32*)((s32)party+8)&0x1000)!=0){*(u32*)((s32)party+8)&=~0x1000;*(u8*)((s32)party+0x38)=0;*(u32*)party&=~0x100;L_partyForceSlitOff(party);}partyRideMain(party);break;
            case 12:if((*(u32*)((s32)party+8)&0x1000)!=0){*(u32*)((s32)party+8)&=~0x1000;*(u8*)((s32)party+0x38)=0;*(u32*)party&=~0x100;}partyRideYoshiMain(party);break;
            case 13:party_damage_return(party);break;
        }
        party_kemuri(party);marioGetScreenPos((void*)((s32)party+0x58),&sx,&sy,&sz);if(marioChkInScreen((s32)sy,(s32)sz)==0)*(u32*)((s32)party+4)|=0x200000;else *(u32*)((s32)party+4)&=~0x200000;*(f32*)((s32)party+0xAC)=0.0f;*(f32*)((s32)party+0xB0)=0.0f;*(f32*)((s32)party+0xB4)=0.0f;partyPreDisp(party);
    }else{if(vivianGetStatus()!=2)vivianShadow(party);partyPreDisp(party);}return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 party_damage_return(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(char* a, char* b);
    extern s32 sysMsec2Frame(s32 msec);
    extern void marioGetScreenPos(VecLocal* pos, f32* x, f32* y, f32* z);
    extern s32 marioWaitDamageReturnFall(void);
    extern s32 marioWaitDamageTogeReturnFall(void);
    extern u8 partyGetAppearPos(void* party, s32 out);
    extern void partyMove(void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 vivianGetStatus(void);
    extern void vivianShadow(void* party);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern f32 float_0_80421218;
    extern f32 float_15_8042126c;
    extern f32 float_neg100_80421270;
    extern f32 float_20_80421258;
    extern f32 float_neg1_80421204;
    extern f32 float_240_80421274;
    extern f32 float_neg6_80421278;
    extern f32 float_neg2000_8042127c;
    extern f32 float_200_80421280;

    void* player;
    char* pose;
    s32 member;
    VecLocal pos;
    VecLocal marioPos;
    VecLocal appearPos;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 mx;
    f32 my;
    f32 mz;
    f32 limit;

    player = *(void**)((s32)pParty + 0x160);

    if ((*(u32*)((s32)pParty + 0x8) & 8) != 0) {
        *(u32*)((s32)pParty + 0x8) &= ~8;
        *(u32*)((s32)pParty + 0x0) &= ~0x100;
        if ((*(u32*)((s32)pParty + 0x8) & 0x01000000) == 0) {
            *(u8*)((s32)pParty + 0x38) = 0xA;
        } else {
            *(u8*)((s32)pParty + 0x38) = 0;
            *(s32*)((s32)pParty + 0x24) = 100;
        }
    }

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) > 0) {
                break;
            }
            *(u8*)((s32)pParty + 0x38) = 0xA;
        case 0xA:
            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8) {
                    partyGetHp(member);
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[member][8];
                } else if (member == 4) {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][8];
                } else {
                    pose = partyDotPoseTbl[member][8];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }
            *(s32*)((s32)pParty + 0x24) = sysMsec2Frame(200);
            *(u8*)((s32)pParty + 0x38) = 0xB;
        case 0xB:
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) > 0) {
                break;
            }
            *(f32*)((s32)pParty + 0x114) = float_15_8042126c;
            *(f32*)((s32)pParty + 0x118) = float_0_80421218;
            *(u8*)((s32)pParty + 0x38) = 0xC;
        case 0xC:
            *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(&pos, &sx, &sy, &sz);
            marioPos.x = *(f32*)((s32)player + 0x8C);
            marioPos.y = *(f32*)((s32)player + 0x90);
            marioPos.z = *(f32*)((s32)player + 0x94);
            marioGetScreenPos(&marioPos, &mx, &my, &mz);
            limit = float_neg100_80421270;
            if (my < float_neg100_80421270) {
                limit = my;
            }
            if (sy <= limit) {
                *(s32*)((s32)pParty + 0x24) = 0x20;
                *(u8*)((s32)pParty + 0x38) = 0x14;
            }
            break;
        case 0x14:
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(&pos, &sx, &sy, &sz);
            if (sy > float_neg100_80421270) {
                *(f32*)((s32)pParty + 0x5C) += float_20_80421258;
            }
            if (*(u16*)((s32)player + 0x2E) == 0x1F) {
                if (marioWaitDamageReturnFall() == 0) {
                    break;
                }
            } else if (*(u16*)((s32)player + 0x2E) == 0x20) {
                if (marioWaitDamageTogeReturnFall() == 0) {
                    break;
                }
            }
            *(u8*)((s32)pParty + 0x38) = 0x15;
        case 0x15:
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) > 0) {
                break;
            }
            partyGetAppearPos(pParty, (s32)&appearPos);
            *(f32*)((s32)pParty + 0x58) = appearPos.x;
            *(f32*)((s32)pParty + 0x60) = appearPos.z;
            *(f32*)((s32)pParty + 0x114) = float_0_80421218;
            *(f32*)((s32)pParty + 0x118) = float_neg1_80421204;
            *(f32*)((s32)pParty + 0x104) = float_0_80421218;
            *(u32*)((s32)pParty + 0x0) |= 0x20;
            *(u32*)((s32)pParty + 0x0) |= 0x02000000;
            *(u8*)((s32)pParty + 0x38) = 0x16;
            break;
        case 0x16:
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(&pos, &sx, &sy, &sz);
            if (sy > float_240_80421274) {
                *(u32*)((s32)pParty + 0x0) &= ~0x02000000;
            }
            partyMove(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_neg6_80421278) {
                *(f32*)((s32)pParty + 0x118) = float_0_80421218;
                *(f32*)((s32)pParty + 0x114) = float_neg6_80421278;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_8042127c) {
                *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
                *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
                *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
                *(f32*)((s32)pParty + 0x5C) += float_200_80421280;
                *(u32*)((s32)pParty + 0x0) &= ~0x70;
            }
            if (*(void**)((s32)pParty + 0x138) != 0 && (hitGetAttr(*(void**)((s32)pParty + 0x138)) & 0x200) == 0) {
                if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                    member = (s8)*(u8*)((s32)pParty + 0x31);
                    if (member < 8) {
                        partyGetHp(member);
                    }
                    if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                        pose = partyPoseTbl[member][9];
                    } else if (member == 4) {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][9];
                    } else {
                        pose = partyDotPoseTbl[member][9];
                    }
                    if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                        *(char**)((s32)pParty + 0x18) = pose;
                        *(s16*)((s32)pParty + 0x20) = 0;
                        *(u32*)((s32)pParty + 0x8) |= 0x2000;
                    }
                }
                *(u8*)((s32)pParty + 0x38) = 0x17;
                *(s32*)((s32)pParty + 0x24) = 0x1E;
            }
            break;
        case 0x17:
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) <= 0) {
                partyChgRunMode(pParty, 2);
                *(u8*)((s32)pParty + 0x3B) = 0;
                *(u32*)((s32)pParty + 0x4) &= ~0x10;
            }
            break;
    }

    if (vivianGetStatus() != 2) {
        vivianShadow(pParty);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyGoodbyeMain(void* pParty) {
    extern void* marioGetPtr(void);
    extern s32 vivianGetStatus(void);
    extern s32 yoshiGetStatus(void);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void movePos(f32 dist, f32 dir, f32* x, f32* z);
    extern f32 revise360(f32 angle);
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern void partyChgRunMode(void* party, s32 mode);
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern f32 float_0_80421218;
    extern f32 float_0p5_80421288;
    extern f32 float_1p6_80421284;
    extern f32 float_4_80421224;
    extern f32 float_10_8042123c;
    extern f32 float_50_804212d0;

    void* player;
    void (*callback)(void*);
    char* pose;
    s32 member;
    s32 poseId;
    s32 weak;
    s32 timer;
    f32 frame;
    f32 destX;
    f32 destY;
    f32 destZ;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 scale;

    player = marioGetPtr();

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            member = (s8)*(u8*)((s32)pParty + 0x31);
            party_dp = partyDataTbl + member * 0x28;
            callback = *(void (**)(void*))(partyDataTbl + member * 0x28 + 0xC);
            callback(pParty);
            if (member == 6) {
                if (vivianGetStatus() == 0) {
                    *(u8*)((s32)pParty + 0x38) = 0xA;
                    *(u8*)((s32)pParty + 0x34) = 8;
                }
            } else if (member == 4) {
                if (yoshiGetStatus() == 0) {
                    *(u8*)((s32)pParty + 0x38) = 0xA;
                    *(u8*)((s32)pParty + 0x34) = 8;
                }
            } else {
                *(u8*)((s32)pParty + 0x38) = 0xA;
                *(u8*)((s32)pParty + 0x34) = 8;
            }
            break;

        case 0xA:
            psndSFXOn_3D(0x2091F, (void*)((s32)player + 0x8C));
            destX = *(f32*)((s32)player + 0x8C);
            destY = float_0p5_80421288 * *(f32*)((s32)player + 0x1BC) + *(f32*)((s32)player + 0x90);
            destZ = *(f32*)((s32)player + 0x94);
            posX = *(f32*)((s32)pParty + 0x58);
            posY = *(f32*)((s32)pParty + 0x5C);
            posZ = *(f32*)((s32)pParty + 0x60);
            *(f32*)((s32)pParty + 0x94) = destX;
            *(f32*)((s32)pParty + 0x98) = destY;
            *(f32*)((s32)pParty + 0x9C) = destZ;
            *(f32*)((s32)pParty + 0x104) = float_4_80421224;
            *(f32*)((s32)pParty + 0x11C) = float_1p6_80421284;
            *(f32*)((s32)pParty + 0x128) = distABf(posX, posZ, destX, destZ);
            *(f32*)((s32)pParty + 0x100) = angleABf(posX, posZ, destX, destZ);
            *(s32*)((s32)pParty + 0x24) = 0xF;
            frame = (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / frame;
            *(f32*)((s32)pParty + 0x114) =
                ((float_0p5_80421288 * frame * *(f32*)((s32)pParty + 0x11C) * frame) +
                 (destY - posY)) / frame;
            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8) {
                    partyGetHp(member);
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[member][4];
                } else if (member == 4) {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][4];
                } else {
                    pose = partyDotPoseTbl[member][4];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }
            *(u8*)((s32)pParty + 0x38) = 0xB;
            break;

        case 0xB:
            *(f32*)((s32)pParty + 0x114) -= *(f32*)((s32)pParty + 0x11C);
            *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
            movePos(*(f32*)((s32)pParty + 0x104),
                    *(f32*)((s32)pParty + 0x100),
                    (f32*)((s32)pParty + 0x58),
                    (f32*)((s32)pParty + 0x60));
            *(f32*)((s32)pParty + 0x10C) =
                revise360(float_50_804212d0 + *(f32*)((s32)pParty + 0x10C));
            frame = (f32)*(s32*)((s32)pParty + 0x24);
            if (frame > float_10_8042123c) {
                frame = float_10_8042123c;
            }
            scale = frame / float_10_8042123c;
            *(f32*)((s32)pParty + 0x70) = scale;
            *(f32*)((s32)pParty + 0x74) = scale;
            *(f32*)((s32)pParty + 0x78) = scale;
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 0) {
                *(u8*)((s32)pParty + 0x38) = 0xC;
            }
            break;

        case 0xC:
            poseId = 1;
            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                weak = 0;
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8 && partyGetHp(member) <= 5) {
                    weak = 1;
                }
                if (weak == 1) {
                    poseId = 0xC;
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[member][poseId];
                } else if (member == 4) {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                } else {
                    pose = partyDotPoseTbl[member][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }
            *(f32*)((s32)pParty + 0x114) = float_0_80421218;
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x98);
            partyChgRunMode(pParty, 0xE);
            return 1;

        case 0x14:
            partyChgRunMode(pParty, 0xE);
            return 1;
    }

    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyDokanMain(void* pParty) {
    extern f32 revise360(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern s32 strcmp(const char* a, const char* b);
    extern void animPoseSetMaterialLightFlagOff(s32 pose, s32 flag);
    extern void animPoseSetPaperAnimGroup(s32 pose, char* anim, s32 flag);
    extern void animPaperPoseRelease(s32 pose);
    extern void animPoseSetMaterialLightFlagOn(s32 pose, s32 flag);
    extern void L_partyVivianTailStart(void* party);
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyPtrTbl[2];
    extern u8 vec3_802cb178[];
    extern f32 float_0_80421218;
    extern f32 float_neg5_804212b0;
    extern f32 float_5_804212b4;
    extern f32 float_0p2_804212b8;
    extern f32 float_neg4_804212bc;
    extern f32 float_1p5_804212c0;
    extern f32 float_neg3_804212c4;
    extern f32 float_neg1p5_804212c8;

    void* player;
    void* party;
    u8* data;
    s8 dokanVal;
    s32 count;
    s32 id;
    f32 sinVal;
    f32 cosVal;

    data = vec3_802cb178;
    player = *(void**)((s32)pParty + 0x160);

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
            *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
            *(f32*)((s32)pParty + 0x10C) =
                revise360(*(f32*)((s32)pParty + 0x10C) + *(f32*)((s32)pParty + 0x124));
            break;

        case 0xA:
            *(f32*)((s32)pParty + 0x64) += *(f32*)((s32)pParty + 0x124);
            *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
            *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
            sincosf(*(f32*)((s32)player + 0x19C), &cosVal, &sinVal);
            *(f32*)((s32)pParty + 0x58) = float_neg5_804212b0 * cosVal + *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)pParty + 0x60) = float_5_804212b4 * sinVal + *(f32*)((s32)player + 0x94);
            break;

        case 0x64:
            dokanVal = *(u8*)((s32)pParty + 0x2E);
            if (dokanVal == 4) {
                *(f32*)((s32)pParty + 0x64) = float_0_80421218;
                *(u32*)((s32)pParty + 0x4) &= ~8;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
                if ((*(u32*)((s32)pParty + 0x4) & 2) == 0 &&
                    (*(char**)((s32)pParty + 0x1C) == 0 ||
                     strcmp(*(char**)((s32)pParty + 0x1C), (char*)(data + 0x1E0)) != 0)) {
                    *(char**)((s32)pParty + 0x1C) = (char*)(data + 0x1E0);
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x4000;
                }
                *(s32*)((s32)pParty + 0x24) = 0x12;
                *(u8*)((s32)pParty + 0x38) = 0x65;
            } else if (dokanVal < 4 && dokanVal < 2 && dokanVal >= 0) {
                if ((*(u32*)((s32)pParty + 0x4) & 2) == 0 &&
                    (*(char**)((s32)pParty + 0x1C) == 0 ||
                     strcmp(*(char**)((s32)pParty + 0x1C), (char*)(data + 0x1E0)) != 0)) {
                    *(char**)((s32)pParty + 0x1C) = (char*)(data + 0x1E0);
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x4000;
                }
                *(s32*)((s32)pParty + 0x24) = 0x12;
                *(u8*)((s32)pParty + 0x38) = 0x65;
                *(u32*)((s32)pParty + 0x4) |= 8;
                *(u32*)((s32)pParty + 0x4) |= 0x10;
                animPoseSetMaterialLightFlagOff(*(s32*)((s32)pParty + 0xC), 1);
            }
            break;

        case 0x65:
            dokanVal = *(u8*)((s32)pParty + 0x2E);
            if (dokanVal < 2 && dokanVal >= 0) {
                *(f32*)((s32)pParty + 0x60) -= float_0p2_804212b8;
            }
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) < 1) {
                *(u8*)((s32)pParty + 0x38) = 0x66;
            }
            break;

        case 0x66:
            *(u32*)((s32)pParty + 0x70) = *(u32*)(data + 0x138);
            *(u32*)((s32)pParty + 0x74) = *(u32*)(data + 0x13C);
            *(u32*)((s32)pParty + 0x78) = *(u32*)(data + 0x140);
            *(u32*)((s32)pParty + 0x64) = *(u32*)(data + 0x144);
            *(u32*)((s32)pParty + 0x68) = *(u32*)(data + 0x148);
            *(u32*)((s32)pParty + 0x6C) = *(u32*)(data + 0x14C);
            *(u32*)((s32)pParty + 0x88) = *(u32*)(data + 0x150);
            *(u32*)((s32)pParty + 0x8C) = *(u32*)(data + 0x154);
            *(u32*)((s32)pParty + 0x90) = *(u32*)(data + 0x158);
            *(u32*)((s32)pParty + 0x7C) = *(u32*)(data + 0x15C);
            *(u32*)((s32)pParty + 0x80) = *(u32*)(data + 0x160);
            *(u32*)((s32)pParty + 0x84) = *(u32*)(data + 0x164);
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)pParty + 0x110);
            *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)player + 0x1A4);
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)player + 0x1A4);
            if ((*(u32*)((s32)pParty + 0x4) & 0x4000000) != 0 && *(char**)((s32)pParty + 0x1C) != 0) {
                animPoseSetPaperAnimGroup(*(s32*)((s32)pParty + 0xC), 0, 0);
                *(char**)((s32)pParty + 0x1C) = 0;
                *(u32*)((s32)pParty + 0x4) &= ~0x4000000;
                if (*(s32*)((s32)pParty + 0x14) >= 0) {
                    animPaperPoseRelease(*(s32*)((s32)pParty + 0x14));
                    *(s32*)((s32)pParty + 0x14) = -1;
                }
            }
            animPoseSetMaterialLightFlagOn(*(s32*)((s32)pParty + 0xC), 1);
            *(u32*)((s32)pParty + 0x4) &= ~8;
            *(u32*)((s32)pParty + 0x4) &= ~0x10;
            *(u32*)((s32)pParty + 0x0) &= ~0x04000000;
            *(u32*)((s32)pParty + 0x0) &= ~0x02000000;
            if (pParty != 0) {
                *(u32*)((s32)pParty + 0x4) &= ~0x00100000;
            }
            L_partyVivianTailStart(pParty);
            *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0x58);
            *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0x60);
            partyChgRunMode(pParty, 1);
            count = 0;
            id = marioGetPartyId();
            if (id >= 0) {
                party = partyPtrTbl[id];
            } else {
                party = 0;
            }
            if (party != 0) {
                count = 1;
            }
            id = marioGetExtraPartyId();
            if (id >= 0) {
                party = partyPtrTbl[id];
            } else {
                party = 0;
            }
            if (party != 0) {
                count++;
            }
            if (*(u8*)((s32)pParty + 0x2E) == 4) {
                if (count == 1) {
                    *(f32*)((s32)pParty + 0x60) += float_neg4_804212bc;
                } else if ((*(u32*)((s32)pParty + 0x0) & 8) == 0) {
                    *(f32*)((s32)pParty + 0x60) += float_neg4_804212bc;
                    *(f32*)((s32)pParty + 0x58) += float_1p5_804212c0;
                } else {
                    *(f32*)((s32)pParty + 0x60) += float_neg3_804212c4;
                    *(f32*)((s32)pParty + 0x58) += float_neg1p5_804212c8;
                }
            }
            if (*(u8*)((s32)pParty + 0x2E) == 1) {
                if (count == 1) {
                    *(f32*)((s32)pParty + 0x60) += float_neg4_804212bc;
                } else if ((*(u32*)((s32)pParty + 0x0) & 8) == 0) {
                    *(f32*)((s32)pParty + 0x60) += float_neg4_804212bc;
                    *(f32*)((s32)pParty + 0x58) += float_1p5_804212c0;
                } else {
                    *(f32*)((s32)pParty + 0x60) += float_neg3_804212c4;
                    *(f32*)((s32)pParty + 0x58) += float_neg1p5_804212c8;
                }
            }
            break;
    }

    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyRideYoshiMain(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void getRidePos(void* party, void* pos);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern void movePos(f32 dist, f32 dir, f32* x, f32* z);
    extern f32 toMovedir(f32 dir);
    extern u8 partyGetAppearPos(void* party, s32 out);
    extern void partyChgRunMode(void* party, s32 mode);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern f32 float_0_80421218;
    extern f32 float_0p5_80421288;
    extern f32 float_1p6_80421284;
    extern u32 vec3_802cb328[];

    VecLocal ridePos;
    VecLocal appearPos;
    void* player;
    char* pose;
    s32 member;
    s32 poseId;
    s32 weak;
    f32 timer;

    player = *(void**)((s32)pParty + 0x160);

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            *(f32*)((s32)pParty + 0x11C) = float_1p6_80421284;
            getRidePos(pParty, &ridePos);
            *(f32*)((s32)pParty + 0x94) = ridePos.x;
            *(f32*)((s32)pParty + 0x98) = ridePos.y;
            *(f32*)((s32)pParty + 0x9C) = ridePos.z;
            *(f32*)((s32)pParty + 0x128) = distABf(*(f32*)((s32)pParty + 0x58),
                                                    *(f32*)((s32)pParty + 0x60),
                                                    *(f32*)((s32)pParty + 0x94),
                                                    *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) = angleABf(*(f32*)((s32)pParty + 0x58),
                                                     *(f32*)((s32)pParty + 0x60),
                                                     *(f32*)((s32)pParty + 0x94),
                                                     *(f32*)((s32)pParty + 0x9C));
            *(s32*)((s32)pParty + 0x24) = 0xF;
            timer = (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / timer;
            *(f32*)((s32)pParty + 0x114) =
                ((float_0p5_80421288 * timer * *(f32*)((s32)pParty + 0x11C) * timer) +
                 (*(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C))) / timer;

            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8) {
                    partyGetHp(member);
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[member][4];
                } else if (member == 4) {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][4];
                } else {
                    pose = partyDotPoseTbl[member][4];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }
            *(u8*)((s32)pParty + 0x38) = 1;
            break;

        case 1:
            getRidePos(pParty, &ridePos);
            *(f32*)((s32)pParty + 0x94) = ridePos.x;
            *(f32*)((s32)pParty + 0x98) = ridePos.y;
            *(f32*)((s32)pParty + 0x9C) = ridePos.z;
            *(f32*)((s32)pParty + 0x128) = distABf(*(f32*)((s32)pParty + 0x58),
                                                    *(f32*)((s32)pParty + 0x60),
                                                    *(f32*)((s32)pParty + 0x94),
                                                    *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) = angleABf(*(f32*)((s32)pParty + 0x58),
                                                     *(f32*)((s32)pParty + 0x60),
                                                     *(f32*)((s32)pParty + 0x94),
                                                     *(f32*)((s32)pParty + 0x9C));
            timer = (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / timer;
            *(f32*)((s32)pParty + 0x114) -= *(f32*)((s32)pParty + 0x11C);
            *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
            movePos(*(f32*)((s32)pParty + 0x104),
                    *(f32*)((s32)pParty + 0x100),
                    (f32*)((s32)pParty + 0x58),
                    (f32*)((s32)pParty + 0x60));
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) < 1) {
                *(u8*)((s32)pParty + 0x38) = 2;
            }
            break;

        case 2:
            poseId = 1;
            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                weak = 0;
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8 && partyGetHp(member) <= 5) {
                    weak = 1;
                }
                if (weak == 1) {
                    poseId = 0xC;
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[member][poseId];
                } else if (member == 4) {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                } else {
                    pose = partyDotPoseTbl[member][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }
            *(f32*)((s32)pParty + 0x114) = float_0_80421218;
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x98);
            *(u8*)((s32)pParty + 0x38) = 0xA;
            if (pParty != 0) {
                *(u32*)((s32)pParty + 0x4) |= 0x00100000;
            }
            break;

        case 0xA:
            getRidePos(pParty, &ridePos);
            *(f32*)((s32)pParty + 0x58) = ridePos.x;
            *(f32*)((s32)pParty + 0x5C) = ridePos.y;
            *(f32*)((s32)pParty + 0x60) = ridePos.z;
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)pParty + 0xFC) = toMovedir(*(f32*)((s32)pParty + 0x110));
            if (*(u16*)((s32)player + 0x2E) != 0x1A) {
                partyGetAppearPos(pParty, (s32)&appearPos);
                *(f32*)((s32)pParty + 0x58) = appearPos.x;
                *(f32*)((s32)pParty + 0x60) = appearPos.z;
                partyChgRunMode(pParty, 2);
                if (pParty != 0) {
                    *(u32*)((s32)pParty + 0x4) &= ~0x00100000;
                }
                *(u32*)((s32)pParty + 0x7C) = vec3_802cb328[0];
                *(u32*)((s32)pParty + 0x80) = vec3_802cb328[1];
                *(u32*)((s32)pParty + 0x84) = vec3_802cb328[2];
            }
            break;
    }

    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyDisp(s32 param_1, void* pParty) {
    typedef f32 Mtx34[3][4];
    extern void PSMTXScale(Mtx34 mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(Mtx34 mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34 mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(Mtx34 a, Mtx34 b, Mtx34 out);
    extern f32 revise360(f32 angle);
    extern void animPoseDrawMtx(s32 poseId, Mtx34 mtx, s32 mode, f32 rot, f32 scale);
    extern void animPoseSetLocalTimeRate(f32 rate, s32 poseId);
    extern void* partyPtrTbl[2];
    extern f32 float_neg6p8_804211f0;
    extern f32 float_6p8_804211f4;
    extern f32 float_1p2_804211f8;
    extern f32 float_2_804211fc;
    extern f32 float_1_80421200;
    extern f32 float_neg1_80421204;
    extern f32 float_90_80421208;
    extern f32 float_270_8042120c;
    extern f32 float_deg2rad_80421210;
    extern f32 float_180_80421214;
    extern f32 float_0_80421218;
    extern f32 float_3_8042121c;
    extern f32 float_3p5_80421220;
    extern f32 float_4_80421224;
    extern f32 float_0p75_80421228;

    Mtx34 scaleMtx;
    Mtx34 workMtx;
    Mtx34 tempMtx;
    s32 activeCount;
    void* party;
    f32 scale;
    f32 angle;
    f32 yOffset;
    f32 rate;
    s32 member;
    u32 flags;
    u32 dispFlags;

    activeCount = 0;
    party = partyPtrTbl[0];
    if (party != 0 && (*(u32*)((s32)party + 0x0) & 1) != 0) {
        activeCount = 1;
    }
    party = partyPtrTbl[1];
    if (party != 0 && (*(u32*)((s32)party + 0x0) & 1) != 0) {
        activeCount++;
    }

    if (activeCount != 0) {
        flags = *(u32*)((s32)pParty + 0x0);
        if ((flags & 0x200) != 0) {
            *(f32*)((s32)pParty + 0x88) = float_neg6p8_804211f0;
            *(f32*)((s32)pParty + 0x8C) = float_6p8_804211f4;
        }

        if ((*(u32*)((s32)pParty + 0x0) & 0x800000) != 0) {
            scale = float_1p2_804211f8;
        } else {
            scale = float_2_804211fc;
        }
        PSMTXScale(scaleMtx,
                   *(f32*)((s32)pParty + 0x70) * scale,
                   *(f32*)((s32)pParty + 0x74) * scale,
                   *(f32*)((s32)pParty + 0x78) * scale);

        dispFlags = *(u32*)((s32)pParty + 0x4);
        if ((dispFlags & 8) == 0) {
            angle = revise360(*(f32*)((s32)pParty + 0x10C));
            scale = float_1_80421200;
            if (angle >= float_90_80421208 && angle <= float_270_8042120c) {
                scale = float_neg1_80421204;
            }
            PSMTXScale(tempMtx, float_1_80421200, float_1_80421200, scale);
            PSMTXConcat(tempMtx, scaleMtx, scaleMtx);
        }

        PSMTXTrans(workMtx,
                   -*(f32*)((s32)pParty + 0x88),
                   -*(f32*)((s32)pParty + 0x8C),
                   -*(f32*)((s32)pParty + 0x90));
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        PSMTXRotRad(workMtx, 0x7A, float_deg2rad_80421210 * *(f32*)((s32)pParty + 0x6C));
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        PSMTXRotRad(workMtx, 0x78, float_deg2rad_80421210 * *(f32*)((s32)pParty + 0x64));
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        angle = revise360(*(f32*)((s32)pParty + 0x10C) - *(f32*)((s32)pParty + 0xF8));
        PSMTXRotRad(workMtx, 0x79, float_deg2rad_80421210 * angle);
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        dispFlags = *(u32*)((s32)pParty + 0x4);
        if ((dispFlags & 0xC0) != 0) {
            angle = revise360(*(f32*)((s32)pParty + 0x10C));
            if (angle >= float_90_80421208 && angle <= float_270_8042120c) {
                if ((*(u32*)((s32)pParty + 0x4) & 0x40) != 0) {
                    scale = float_180_80421214;
                } else {
                    scale = float_0_80421218;
                }
            } else {
                if ((*(u32*)((s32)pParty + 0x4) & 0x40) != 0) {
                    scale = float_0_80421218;
                } else {
                    scale = float_180_80421214;
                }
            }
            PSMTXRotRad(workMtx, 0x79, float_deg2rad_80421210 * scale);
            PSMTXConcat(workMtx, scaleMtx, scaleMtx);
        }

        PSMTXTrans(workMtx,
                   *(f32*)((s32)pParty + 0x88),
                   *(f32*)((s32)pParty + 0x8C),
                   *(f32*)((s32)pParty + 0x90));
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        yOffset = float_0_80421218;
        if (*(void**)((s32)pParty + 0x1C) != 0) {
            member = (s8)*(u8*)((s32)pParty + 0x31);
            if (member == 4) {
                yOffset = float_1_80421200;
            } else if (member == 3) {
                yOffset = float_2_804211fc;
            } else if (member == 7) {
                yOffset = float_2_804211fc;
            } else if (member == 1) {
                yOffset = float_2_804211fc;
            } else if (member == 0xB) {
                yOffset = float_3_8042121c;
            } else if (member == 8) {
                yOffset = float_3p5_80421220;
            } else if (member == 0xA) {
                yOffset = float_4_80421224;
            } else if (member == 9) {
                yOffset = float_neg1_80421204;
            }
        }

        PSMTXTrans(workMtx,
                   *(f32*)((s32)pParty + 0x58) + *(f32*)((s32)pParty + 0x7C),
                   yOffset + *(f32*)((s32)pParty + 0x5C) + *(f32*)((s32)pParty + 0x80),
                   *(f32*)((s32)pParty + 0x60) + *(f32*)((s32)pParty + 0x84));
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        animPoseDrawMtx(*(s32*)((s32)pParty + 0xC), scaleMtx, 2, float_0_80421218, float_1_80421200);
        animPoseDrawMtx(*(s32*)((s32)pParty + 0xC), scaleMtx, 1, float_0_80421218, float_1_80421200);

        rate = float_1_80421200;
        if ((*(u32*)((s32)pParty + 0x0) & 0x80) != 0) {
            rate = float_0p75_80421228;
        }
        animPoseSetLocalTimeRate(rate, *(s32*)((s32)pParty + 0xC));
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partySetFamicomMode(void* pParty, int mode) {
    extern void L_partyForceSlitOff(void* party);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(char* group, s32 flags);
    extern s32 partyGetHp(s32 partyId);
    extern s32 strcmp(char* a, char* b);
    extern char** party_dp;
    extern char* yoshigroup[];
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern char str_d_mario_802cb340[];

    char* group;
    char* pose;
    s32 poseId;
    s32 weak;
    s32 member;

    if (mode == 1) {
        if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
            return;
        }
        L_partyForceSlitOff(pParty);
        *(u32*)((s32)pParty + 4) |= 0x8000;
        member = (s8)*(u8*)((s32)pParty + 0x31);
        if (member != 4) {
            if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
                group = *party_dp;
            } else {
                group = str_d_mario_802cb340;
            }
        } else {
            group = yoshigroup[pouchGetPartyColor(4)];
            if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
                group = str_d_mario_802cb340;
            }
        }

        if (*(s32*)((s32)pParty + 0xC) >= 0) {
            animPoseRelease(*(s32*)((s32)pParty + 0xC));
        }
        *(s32*)((s32)pParty + 0xC) = animPoseEntry(group, 2);
        poseId = 1;
        if ((*(u32*)((s32)pParty + 4) & 2) == 0) {
            weak = 0;
            member = (s8)*(u8*)((s32)pParty + 0x31);
            if (member < 8 && partyGetHp(member) <= 5) {
                weak = 1;
            }
            if (weak == 1) {
                poseId = 0xC;
            }
            if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
                pose = partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][poseId];
            } else if ((s8)*(u8*)((s32)pParty + 0x31) == 4) {
                pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
            } else {
                pose = partyDotPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][poseId];
            }
            if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                *(char**)((s32)pParty + 0x18) = pose;
                *(s16*)((s32)pParty + 0x20) = 0;
                *(u32*)((s32)pParty + 8) |= 0x2000;
            }
        } else {
            pose = *(char**)((s32)pParty + 0x18);
        }
        if ((*(u32*)((s32)pParty + 4) & 2) == 0 &&
            (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) &&
            (*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
            *(char**)((s32)pParty + 0x18) = pose;
            *(s16*)((s32)pParty + 0x20) = 0;
            *(u32*)((s32)pParty + 8) |= 0x2000;
        }
        return;
    }

    if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
        return;
    }
    *(u32*)((s32)pParty + 4) &= ~0x8000;
    member = (s8)*(u8*)((s32)pParty + 0x31);
    if (member != 4) {
        if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
            group = *party_dp;
        } else {
            group = str_d_mario_802cb340;
        }
    } else {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
            group = str_d_mario_802cb340;
        }
    }

    if (*(s32*)((s32)pParty + 0xC) >= 0) {
        animPoseRelease(*(s32*)((s32)pParty + 0xC));
    }
    *(s32*)((s32)pParty + 0xC) = animPoseEntry(group, 2);
    poseId = 1;
    if ((*(u32*)((s32)pParty + 4) & 2) == 0) {
        weak = 0;
        member = (s8)*(u8*)((s32)pParty + 0x31);
        if (member < 8 && partyGetHp(member) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            poseId = 0xC;
        }
        if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
            pose = partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][poseId];
        } else if ((s8)*(u8*)((s32)pParty + 0x31) == 4) {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
        } else {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][poseId];
        }
        if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
            *(char**)((s32)pParty + 0x18) = pose;
            *(s16*)((s32)pParty + 0x20) = 0;
            *(u32*)((s32)pParty + 8) |= 0x2000;
        }
    } else {
        pose = *(char**)((s32)pParty + 0x18);
    }
    if ((*(u32*)((s32)pParty + 4) & 2) == 0 &&
        (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) &&
        (*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
        *(char**)((s32)pParty + 0x18) = pose;
        *(s16*)((s32)pParty + 0x20) = 0;
        *(u32*)((s32)pParty + 8) |= 0x2000;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntry2Pos(s32 memberId, f32 x, f32 y, f32 z) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern s32 marioGetCamId(void);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(char* group, s32 flags);
    extern void animPosePeraOff(s32 poseId);
    extern s32 marioAnimeId(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern u32 animPoseGetMaterialLightFlag(s32 poseId);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
    extern s32 partyGetHp(s32 partyId);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void* partyPtrTbl[2];
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern char* yoshigroup[];
    extern char str_d_mario_802cb340[];
    extern f32 float_1_80421200;

    void* player;
    void* party;
    char* group;
    char* pose;
    s32 slotId;
    s32 poseId;
    s32 weak;
    s32 animId;
    u32 flags;
    void (*initFunc)(void*);
    VecLocal pos;

    player = marioGetPtr();
    if (partyPtrTbl[1] == 0) {
        partyPtrTbl[1] = __memAlloc(0, 0x188);
        memset(partyPtrTbl[1], 0, 0x188);
        slotId = 1;
    } else {
        slotId = -1;
    }

    if (slotId < 0) {
        return -1;
    }

    party = partyPtrTbl[slotId];
    memset(party, 0, 0x188);
    *(s32*)((s32)party + 0x164) = marioGetCamId();
    *(s32*)((s32)party + 0xC) = -1;
    *(s32*)((s32)party + 0x10) = -1;
    *(s32*)((s32)party + 0x14) = -1;
    *(u32*)((s32)party + 0x0) = 9;
    *(u8*)((s32)party + 0x2F) = slotId;
    *(void**)((s32)party + 0x160) = player;
    *(u8*)((s32)party + 0x31) = memberId;
    if (slotId == 1) {
        *(u32*)((s32)party + 0x0) |= 4;
    }

    party_dp = partyDataTbl + memberId * 0x28;
    if ((s8)*(u8*)((s32)party + 0x31) == 4) {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0) {
            group = str_d_mario_802cb340;
        }
    } else if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
        group = *(char**)party_dp;
    } else {
        group = str_d_mario_802cb340;
    }

    if (*(s32*)((s32)party + 0xC) >= 0) {
        animPoseRelease(*(s32*)((s32)party + 0xC));
    }
    *(s32*)((s32)party + 0xC) = animPoseEntry(group, 2);
    animPosePeraOff(*(s32*)((s32)party + 0xC));
    player = *(void**)((s32)party + 0x160);
    animId = marioAnimeId();
    flags = animPoseGetMaterialFlag(*(s32*)((s32)player + 0x22C + animId * 4));
    animPoseSetMaterialFlagOn(*(s32*)((s32)party + 0xC), flags);
    animId = marioAnimeId();
    flags = animPoseGetMaterialLightFlag(*(s32*)((s32)player + 0x22C + animId * 4));
    animPoseSetMaterialLightFlagOn(*(s32*)((s32)party + 0xC), flags);

    poseId = 1;
    if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
        weak = 0;
        if ((s8)*(u8*)((s32)party + 0x31) < 8 &&
            partyGetHp((s8)*(u8*)((s32)party + 0x31)) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            poseId = 0xC;
        }
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        } else if ((s8)*(u8*)((s32)party + 0x31) == 4) {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
        } else {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        }
        if (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
            *(char**)((s32)party + 0x18) = pose;
            *(s16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 0x8) |= 0x2000;
        }
    } else {
        pose = *(char**)((s32)party + 0x18);
    }
    if ((*(u32*)((s32)party + 0x4) & 2) == 0 &&
        (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) &&
        ((*(u32*)((s32)party + 0x4) & 0x8000) == 0)) {
        *(char**)((s32)party + 0x18) = pose;
        *(s16*)((s32)party + 0x20) = 0;
        *(u32*)((s32)party + 0x8) |= 0x2000;
    }
    *(char**)((s32)party + 0x1C) = 0;
    initFunc = *(void (**)(void*))((s32)party_dp + 4);
    initFunc(party);
    partyChgRunMode(party, 2);

    pos.x = x;
    pos.y = y;
    pos.z = z;
    *(f32*)((s32)party + 0x58) = pos.x;
    *(f32*)((s32)party + 0x5C) = pos.y;
    *(f32*)((s32)party + 0x60) = pos.z;
    *(f32*)((s32)party + 0xB8) = *(f32*)((s32)party + 0x58);
    *(f32*)((s32)party + 0xBC) = *(f32*)((s32)party + 0x5C);
    *(f32*)((s32)party + 0xC0) = *(f32*)((s32)party + 0x60);
    *(f32*)((s32)party + 0x70) = float_1_80421200;
    *(f32*)((s32)party + 0x74) = float_1_80421200;
    *(f32*)((s32)party + 0x78) = float_1_80421200;
    return slotId;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 N__partyChgRunMode(void* pParty, int currRunModeId, int newRunModeId) {
    extern void partyClearJumpPara(void*);
    extern void L_partyForceSlitOff(void*);
    extern s32 partyGetHp(s32);
    extern s32 pouchGetPartyColor(s32);
    extern s32 strcmp(const char*, const char*);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern u8 partyDataTbl[];
    void* player;
    char* pose;
    s32 member;
    s32 poseId;
    void (*callback)(void);

    member = *(s8*)((s32)pParty + 0x31);
    if (currRunModeId == 4 || (currRunModeId == 0xB && newRunModeId != 8)) {
        player = *(void**)((s32)pParty + 0x160);
        *(f32*)((s32)pParty + 0x70) = 1.0f;
        *(f32*)((s32)pParty + 0x74) = 1.0f;
        *(f32*)((s32)pParty + 0x78) = 1.0f;
        *(u32*)((s32)pParty + 4) &= ~0x100;
        *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)player + 0x1A4);
        *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)player + 0x1A8);
        *(f32*)((s32)pParty + 0x108) = *(f32*)((s32)player + 0x1B0);
        partyClearJumpPara(pParty);
        poseId = 1;
        if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
            if (member < 8 && partyGetHp(member) < 6) poseId = 0xC;
            if ((*(u32*)((s32)pParty + 4) & 0x10000) == 0) {
                pose = partyPoseTbl[member][poseId];
            } else if (member == 4) {
                pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
            } else {
                pose = partyDotPoseTbl[member][poseId];
            }
            if (*(char**)((s32)pParty + 0x18) == 0 ||
                strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                *(char**)((s32)pParty + 0x18) = pose;
                *(u16*)((s32)pParty + 0x20) = 0;
                *(u32*)((s32)pParty + 8) |= 0x1000;
            }
        }
        *(u32*)((s32)pParty + 4) &= ~0x100;
        if (currRunModeId == 4) {
            *(u8*)((s32)pParty + 0x34) = 1;
        } else {
            *(u32*)((s32)pParty + 4) &= ~0x200;
            *(u32*)pParty &= ~0x70000;
        }
    } else if (currRunModeId < 3 && currRunModeId > 0) {
        L_partyForceSlitOff(pParty);
    } else if (currRunModeId == 3) {
        callback = *(void (**)(void))(partyDataTbl + member * 0x28 + 0x18);
        if (callback != 0) callback();
    } else if (currRunModeId == 0xC) {
        if (pParty != 0) *(u32*)((s32)pParty + 4) &= ~0x40;
    }
    return 0;
}

s32 partyEntryMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntry2(s32 memberId) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern s32 marioGetCamId(void);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(char* group, s32 flags);
    extern void animPosePeraOff(s32 poseId);
    extern s32 marioAnimeId(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern u32 animPoseGetMaterialLightFlag(s32 poseId);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
    extern s32 partyGetHp(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void* partyPtrTbl[2];
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern char* yoshigroup[];
    extern char str_d_mario_802cb340[];

    void* player;
    void* party;
    char* group;
    char* pose;
    s32 slotId;
    s32 poseId;
    s32 weak;
    s32 animId;
    u32 flags;
    void (*initFunc)(void*);

    player = marioGetPtr();
    if (partyPtrTbl[1] == 0) {
        partyPtrTbl[1] = __memAlloc(0, 0x188);
        memset(partyPtrTbl[1], 0, 0x188);
        slotId = 1;
    } else {
        slotId = -1;
    }
    if (slotId < 0) {
        return -1;
    }

    party = partyPtrTbl[slotId];
    memset(party, 0, 0x188);
    *(s32*)((s32)party + 0x164) = marioGetCamId();
    *(s32*)((s32)party + 0xC) = -1;
    *(s32*)((s32)party + 0x10) = -1;
    *(s32*)((s32)party + 0x14) = -1;
    *(u32*)((s32)party + 0x0) = 9;
    *(u8*)((s32)party + 0x2F) = slotId;
    *(void**)((s32)party + 0x160) = player;
    *(u8*)((s32)party + 0x31) = memberId;
    if (slotId == 1) {
        *(u32*)((s32)party + 0x0) |= 4;
    }

    party_dp = partyDataTbl + memberId * 0x28;
    if ((s8)*(u8*)((s32)party + 0x31) == 4) {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0) {
            group = str_d_mario_802cb340;
        }
    } else if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
        group = *(char**)party_dp;
    } else {
        group = str_d_mario_802cb340;
    }
    if (*(s32*)((s32)party + 0xC) >= 0) {
        animPoseRelease(*(s32*)((s32)party + 0xC));
    }
    *(s32*)((s32)party + 0xC) = animPoseEntry(group, 2);
    animPosePeraOff(*(s32*)((s32)party + 0xC));
    player = *(void**)((s32)party + 0x160);
    animId = marioAnimeId();
    flags = animPoseGetMaterialFlag(*(s32*)((s32)player + 0x22C + animId * 4));
    animPoseSetMaterialFlagOn(*(s32*)((s32)party + 0xC), flags);
    animId = marioAnimeId();
    flags = animPoseGetMaterialLightFlag(*(s32*)((s32)player + 0x22C + animId * 4));
    animPoseSetMaterialLightFlagOn(*(s32*)((s32)party + 0xC), flags);

    poseId = 1;
    if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
        weak = 0;
        if ((s8)*(u8*)((s32)party + 0x31) < 8 &&
            partyGetHp((s8)*(u8*)((s32)party + 0x31)) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            poseId = 0xC;
        }
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        } else if ((s8)*(u8*)((s32)party + 0x31) == 4) {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
        } else {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        }
        if (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
            *(char**)((s32)party + 0x18) = pose;
            *(s16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 0x8) |= 0x2000;
        }
    } else {
        pose = *(char**)((s32)party + 0x18);
    }
    if ((*(u32*)((s32)party + 0x4) & 2) == 0 &&
        (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) &&
        ((*(u32*)((s32)party + 0x4) & 0x8000) == 0)) {
        *(char**)((s32)party + 0x18) = pose;
        *(s16*)((s32)party + 0x20) = 0;
        *(u32*)((s32)party + 0x8) |= 0x2000;
    }
    *(char**)((s32)party + 0x1C) = 0;
    initFunc = *(void (**)(void*))((s32)party_dp + 4);
    initFunc(party);
    partyChgRunMode(party, 2);
    return slotId;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyMakeDispDir(void* pParty) {
    extern f32 revise360(f32 angle);
    extern f32 float_0_80421218;
    extern f32 float_10_8042123c;
    extern f32 float_170_80421250;
    extern f32 float_180_80421214;
    extern f32 float_190_80421248;
    extern f32 float_250_80421244;
    extern f32 float_290_80421240;
    extern f32 float_350_8042124c;
    extern f32 float_0p125_80421254;
    extern f32 float_20_80421258;
    extern f32 float_neg40_8042125c;
    extern f32 float_neg30_80421260;
    extern f32 float_220_80421264;
    extern f32 float_210_80421268;

    f32 diff;
    f32 target;
    f32 current;
    f32 toward;
    u32 flags3;

    if ((*(u32*)((s32)pParty + 0x4) & 0x100) == 0) {
        if ((*(u32*)((s32)pParty + 0x4) & 0x10) == 0) {
            diff = revise360(*(f32*)((s32)pParty + 0xFC) - *(f32*)((s32)pParty + 0xF8));
            target = *(f32*)((s32)pParty + 0x110);
            if ((*(u32*)((s32)pParty + 0x4) & 0x100) == 0 &&
                (target == float_290_80421240 || target == float_250_80421244) &&
                diff >= float_180_80421214) {
                target = float_0_80421218;
            }

            if (*(f32*)((s32)pParty + 0x104) != float_0_80421218) {
                if (diff >= float_190_80421248 && diff <= float_350_8042124c) {
                    target = float_0_80421218;
                } else if (diff >= float_10_8042123c && diff <= float_170_80421250) {
                    target = float_180_80421214;
                }
            }
            *(f32*)((s32)pParty + 0x110) = target;

            if ((*(u32*)((s32)pParty + 0x4) & 0x20) != 0) {
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)pParty + 0x110);
            }
            if (*(f32*)((s32)pParty + 0x10C) == *(f32*)((s32)pParty + 0x110)) {
                return;
            }
        }

        current = *(f32*)((s32)pParty + 0x10C);
        if (current <= float_180_80421214) {
            target = *(f32*)((s32)pParty + 0x110);
            if (target == float_0_80421218) {
                if ((*(u32*)((s32)pParty + 0x8) & 0x1000) != 0) {
                    toward = float_20_80421258;
                } else {
                    toward = float_neg40_8042125c;
                }
                *(f32*)((s32)pParty + 0x10C) = current + (float_0p125_80421254 * (toward - current));
                flags3 = *(u32*)((s32)pParty + 0x8);
                if ((flags3 & 0x1000) == 0) {
                    if (*(f32*)((s32)pParty + 0x10C) < float_neg30_80421260) {
                        *(u32*)((s32)pParty + 0x8) = flags3 | 0x1000;
                    }
                } else if (*(f32*)((s32)pParty + 0x10C) >= float_0_80421218) {
                    *(f32*)((s32)pParty + 0x10C) = float_0_80421218;
                    *(u32*)((s32)pParty + 0x8) &= ~0x1000;
                }
            } else if (target == float_180_80421214) {
                if ((*(u32*)((s32)pParty + 0x8) & 0x1000) != 0) {
                    toward = float_170_80421250;
                } else {
                    toward = float_220_80421264;
                }
                *(f32*)((s32)pParty + 0x10C) = current + (float_0p125_80421254 * (toward - current));
                flags3 = *(u32*)((s32)pParty + 0x8);
                if ((flags3 & 0x1000) == 0) {
                    if (*(f32*)((s32)pParty + 0x10C) > float_210_80421268) {
                        *(u32*)((s32)pParty + 0x8) = flags3 | 0x1000;
                    }
                } else if (*(f32*)((s32)pParty + 0x10C) <= float_180_80421214) {
                    *(f32*)((s32)pParty + 0x10C) = float_180_80421214;
                    *(u32*)((s32)pParty + 0x8) &= ~0x1000;
                }
            }
        } else if (current > float_180_80421214) {
            target = *(f32*)((s32)pParty + 0x110);
            if (target == float_0_80421218) {
                if ((*(u32*)((s32)pParty + 0x8) & 0x1000) != 0) {
                    toward = float_20_80421258;
                } else {
                    toward = float_neg40_8042125c;
                }
                *(f32*)((s32)pParty + 0x10C) = current + (float_0p125_80421254 * (toward - current));
                flags3 = *(u32*)((s32)pParty + 0x8);
                if ((flags3 & 0x1000) == 0) {
                    if (*(f32*)((s32)pParty + 0x10C) < float_neg30_80421260) {
                        *(u32*)((s32)pParty + 0x8) = flags3 | 0x1000;
                    }
                } else if (*(f32*)((s32)pParty + 0x10C) >= float_0_80421218) {
                    *(f32*)((s32)pParty + 0x10C) = float_0_80421218;
                    *(u32*)((s32)pParty + 0x8) &= ~0x1000;
                }
            } else if (target == float_180_80421214) {
                if ((*(u32*)((s32)pParty + 0x8) & 0x1000) != 0) {
                    toward = float_170_80421250;
                } else {
                    toward = float_220_80421264;
                }
                *(f32*)((s32)pParty + 0x10C) = current + (float_0p125_80421254 * (toward - current));
                flags3 = *(u32*)((s32)pParty + 0x8);
                if ((flags3 & 0x1000) == 0) {
                    if (*(f32*)((s32)pParty + 0x10C) > float_210_80421268) {
                        *(u32*)((s32)pParty + 0x8) = flags3 | 0x1000;
                    } else if (*(f32*)((s32)pParty + 0x10C) <= float_180_80421214) {
                        *(f32*)((s32)pParty + 0x10C) = float_180_80421214;
                        *(u32*)((s32)pParty + 0x8) &= ~0x1000;
                    }
                }
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyPreDisp(s32 pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void partyMakeDispDir(void* party);
    extern void animPoseSetAnim(s32 poseId, void* animName, s32 force);
    extern void animPoseSetPaperAnim(s32 poseId, void* animName, s32 force);
    extern void animPoseSetLocalTime(s32 poseId, f32 time);
    extern void animPoseMain(s32 poseId);
    extern void* camGetPtr(s32 camId);
    extern void PSMTX44MultVec(void* mtx, VecLocal* src, VecLocal* dst);
    extern void dispEntry(s32 cameraId, s32 mode, void* callback, void* param, f32 z);
    extern void partyDisp(s32 param_1, void* pParty);
    extern s32 N_vivianDisplayTailCheck(void* party);
    extern s32 partyHitCheck(void* party, VecLocal* pos, VecLocal* delta, VecLocal* outPos, VecLocal* outOther, f32* radius);
    extern void animPoseVivianMain(s32 poseId, VecLocal* pos);
    extern s32 shadowEntryMode(s32 mode, f32 x, f32 y, f32 z, f32 size);
    extern void shadowSetCamId(s32 shadowId, s32 camId);
    extern void shadowFlagOn(s32 shadowId, s32 flag);
    extern u32 vec3_802cb334[];
    extern f32 float_0_80421218;
    extern f32 float_neg1_80421204;
    extern f32 float_5000_8042122c;
    extern f32 float_10000_80421230;
    extern f32 float_13p333_80421234;
    extern f32 float_1000_80421238;
    extern f32 float_10_8042123c;

    VecLocal projPos;
    VecLocal hitPos;
    VecLocal delta;
    VecLocal hitOut;
    VecLocal hitOther;
    f32 radius;
    f32 order;
    f32 shadowSize;
    s32 shadowMode;
    s32 shadowId;
    u32 flags3;
    void* party;
    void* cam;

    party = (void*)pParty;
    if ((*(u32*)(pParty + 0x4) & 0x10) == 0 && (*(u32*)(pParty + 0x0) & 0x200) == 0) {
        partyMakeDispDir(party);
    }

    flags3 = *(u32*)(pParty + 0x8);
    if ((flags3 & 0x6000) != 0) {
        if ((flags3 & 0x2000) != 0) {
            *(u32*)(pParty + 0x8) = flags3 & ~0x2000;
        }
        if ((*(u32*)(pParty + 0x8) & 0x4000) != 0) {
            *(u32*)(pParty + 0x8) &= ~0x4000;
        }
        animPoseSetAnim(*(s32*)(pParty + 0xC), *(void**)(pParty + 0x18), 1);
        if (*(void**)(pParty + 0x1C) != 0) {
            animPoseSetPaperAnim(*(s32*)(pParty + 0xC), *(void**)(pParty + 0x1C), 1);
        }
        animPoseSetLocalTime(*(s32*)(pParty + 0xC), (f32)*(u16*)(pParty + 0x20));
    }

    animPoseMain(*(s32*)(pParty + 0xC));

    if ((*(u32*)(pParty + 0x4) & 1) == 0) {
        projPos.x = *(f32*)(pParty + 0x58);
        projPos.y = *(f32*)(pParty + 0x5C);
        projPos.z = *(f32*)(pParty + 0x60);
        cam = camGetPtr(4);
        PSMTX44MultVec((void*)((s32)cam + 0x15C), &projPos, &projPos);

        order = (float_5000_8042122c * projPos.z) + float_5000_8042122c;
        if (order < float_0_80421218) {
            order = float_0_80421218;
        } else if (order > float_10000_80421230) {
            order = float_10000_80421230;
        }
        order *= float_neg1_80421204;

        if (*(s16*)(pParty + 0x2A) <= 0) {
            dispEntry(*(s32*)(pParty + 0x164), 1, partyDisp, party, order);
        } else if ((*(u16*)(pParty + 0x2A) & 2) == 0) {
            dispEntry(*(s32*)(pParty + 0x164), 1, partyDisp, party, order);
        }

        shadowMode = 2;
        shadowSize = float_13p333_80421234;
        if (*(u8*)(pParty + 0x31) != 6) {
            *(f32*)(pParty + 0xB8) = *(f32*)(pParty + 0x58);
            *(f32*)(pParty + 0xBC) = *(f32*)(pParty + 0x5C);
            *(f32*)(pParty + 0xC0) = *(f32*)(pParty + 0x60);
        } else {
            if (*(u8*)(pParty + 0x34) == 4 ||
                (*(u8*)(pParty + 0x34) == 8 && (*(u32*)(pParty + 0x0) & 0x100) == 0)) {
                *(f32*)(pParty + 0xB8) = *(f32*)(pParty + 0x58);
                *(f32*)(pParty + 0xBC) = *(f32*)(pParty + 0x5C);
                *(f32*)(pParty + 0xC0) = *(f32*)(pParty + 0x60);
            }
            if ((*(u32*)(pParty + 0x0) & 0x100) != 0) {
                shadowSize = *(f32*)(pParty + 0x174);
                if ((*(u32*)(pParty + 0x0) & 0x80000000) != 0) {
                    shadowMode = 1;
                }
            }
        }

        if (N_vivianDisplayTailCheck(party) != 0) {
            radius = float_1000_80421238;
            hitPos.x = *(f32*)(pParty + 0x58);
            hitPos.y = *(f32*)(pParty + 0x5C) + float_10_8042123c;
            hitPos.z = *(f32*)(pParty + 0x60);
            delta.x = *(f32*)&vec3_802cb334[0];
            delta.y = *(f32*)&vec3_802cb334[1];
            delta.z = *(f32*)&vec3_802cb334[2];
            if (partyHitCheck(party, &hitPos, &delta, &hitOut, &hitOther, &radius) != 0) {
                *(f32*)(pParty + 0xBC) = hitOther.y;
            } else {
                *(f32*)(pParty + 0xBC) = *(f32*)(pParty + 0x5C);
            }
            animPoseVivianMain(*(s32*)(pParty + 0xC), (VecLocal*)(pParty + 0xB8));
        }

        if ((*(u32*)(pParty + 0x4) & 0x100000) == 0) {
            shadowId = shadowEntryMode(shadowMode,
                                       *(f32*)(pParty + 0xB8),
                                       *(f32*)(pParty + 0xBC),
                                       *(f32*)(pParty + 0xC0),
                                       shadowSize);
            if (*(s32*)(pParty + 0x164) != 4) {
                shadowSetCamId(shadowId, *(s32*)(pParty + 0x164));
            }
            if ((*(u32*)(pParty + 0x4) & 0x200000) != 0) {
                shadowFlagOn(shadowId, 4);
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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

char* partyChgPoseId(void* party, s32 poseId) {
    extern s32 partyGetHp(s32);
    extern s32 pouchGetPartyColor(s32);
    extern s32 strcmp(const char*, const char*);
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    s32 member;
    char* pose;

    if ((*(u32*)((s32)party + 4) & 2) != 0) return *(char**)((s32)party + 0x18);
    member = *(s8*)((s32)party + 0x31);
    if (member < 8 && partyGetHp(member) < 6) {
        if (poseId == 1) poseId = 0xC;
        else if (poseId == 2) poseId = 0xD;
        else if (poseId == 3) poseId = 0xE;
    }
    if ((*(u32*)((s32)party + 4) & 0x10000) == 0) {
        pose = partyPoseTbl[member][poseId];
    } else if (member == 4) {
        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
    } else {
        pose = partyDotPoseTbl[member][poseId];
    }
    if (*(char**)((s32)party + 0x18) == 0 ||
        strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
        *(char**)((s32)party + 0x18) = pose;
        *(u16*)((s32)party + 0x20) = 0;
        *(u32*)((s32)party + 8) |= 0x1000;
    }
    return pose;
}

s32 partyPaperOn(void* pParty, char* paperGroup) {
    extern void animPaperPoseRelease(s32);
    extern s32 animPaperPoseEntry(char*, s32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern s32 strcmp(const char*, const char*);
    extern char str_p_roll_802cb36c[];
    extern char str_p_plane_802cb374[];
    extern char str_p_dokan_x_802cb37c[];
    extern char str_p_dokan_y_802cb360[];
    extern char str_p_slit_802cb388[];
    s32 special;

    if (*(s32*)((s32)pParty + 0x14) >= 0) {
        animPaperPoseRelease(*(s32*)((s32)pParty + 0x14));
        *(s32*)((s32)pParty + 0x14) = -1;
    }
    *(u32*)((s32)pParty + 4) |= 0x4000000;
    *(s32*)((s32)pParty + 0x14) = animPaperPoseEntry(paperGroup, 2);
    special = strcmp(paperGroup, str_p_roll_802cb36c) == 0 ||
              strcmp(paperGroup, str_p_plane_802cb374) == 0 ||
              strcmp(paperGroup, str_p_dokan_x_802cb37c) == 0 ||
              strcmp(paperGroup, str_p_dokan_y_802cb360) == 0;
    special = !special;
    if (strcmp(paperGroup, str_p_slit_802cb388) == 0) {
        special = *(s8*)((s32)pParty + 0x31) == 6;
    }
    animPoseSetPaperAnimGroup(*(s32*)((s32)pParty + 0xC), paperGroup, special);
    return 1;
}

void partyUpdateKeyData(void* party) {
    extern void* gp;
    s32 port = *(s8*)((s32)party + 0x30);
    s32 offset;

    if ((*(u32*)party & 0x60000000) != 0) {
        *(u8*)((s32)party + 0x158) = 0;
        *(u8*)((s32)party + 0x159) = 0;
        *(u8*)((s32)party + 0x15A) = 0;
        *(u8*)((s32)party + 0x15B) = 0;
        *(u8*)((s32)party + 0x15C) = 0;
        *(u8*)((s32)party + 0x15D) = 0;
        *(u8*)((s32)party + 0x15E) = 0;
        *(u8*)((s32)party + 0x15F) = 0;
        *(u16*)((s32)party + 0x150) = 0;
        *(u16*)((s32)party + 0x152) = 0;
        *(u16*)((s32)party + 0x154) = 0;
        *(u16*)((s32)party + 0x156) = 0;
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

void partyMain(void) {
    extern void* marioGetPtr(void);
    extern void unk_8014140c(void);
    extern s32 marioBgmodeChk(void);
    extern void driveParty(void* party);

    s32 active = 0;
    s32 i;
    void** ptr;
    void* party;

    marioGetPtr();
    ptr = partyPtrTbl;
    party = ptr[0];
    if (party != 0 && (*(u32*)party & 1) != 0) {
        active++;
    }
    party = ptr[1];
    if (party != 0 && (*(u32*)party & 1) != 0) {
        active++;
    }
    if (active != 0) {
        unk_8014140c();
        ptr = partyPtrTbl;
        for (i = 0; i < 2; i++, ptr++) {
            party = *ptr;
            if (party != 0 && (*(u32*)party & 1) != 0 && marioBgmodeChk() != 1) {
                driveParty(party);
            }
        }
    }
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
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern void vivianUnhold(void*);
    extern u32 marioGetoffYoshi(void);
    void (*callback)(void);
    void* player;
    s32 member;
    s32 motion;

    *(u8*)((s32)party + 0x38) = 10;
    member = *(s8*)((s32)party + 0x31);
    party_dp = partyDataTbl + member * 0x28;
    callback = *(void (**)(void))(partyDataTbl + member * 0x28 + 0xC);
    if (callback != 0) {
        callback();
    }
    if ((*(u32*)party & 0x100) != 0) {
        if (member == 3) {
            vivianUnhold(party);
            *(u8*)((s32)party + 0x38) = 0;
        }
        if (member == 4) {
            marioGetoffYoshi();
            *(u8*)((s32)party + 0x38) = 0;
        }
    }
    player = *(void**)((s32)party + 0x160);
    motion = *(s32*)((s32)player + 0x28);
    if (motion == 0x12 || motion == 0x13 || motion == 0x14) {
        *(u8*)((s32)party + 0x38) = 0x14;
    }
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

void partyDokanEnd(int partyId, u8 param_2) {
    extern void L_partyVivianTailStart(void* party);

    void* party;

    if (partyId >= 0) {
        party = partyPtrTbl[partyId];
    } else {
        party = 0;
    }

    if (party != 0 && *(u8*)((s32)party + 0x34) == 0xA) {
        *(u8*)((s32)party + 0x38) = 0x64;
        *(u8*)((s32)party + 0x2E) = param_2;
        *(u32*)((s32)party + 0x4) &= ~8;
        *(u32*)((s32)party + 0x4) &= ~0x10;
        *(u32*)((s32)party + 0x4) &= ~0x100;
        *(u32*)party &= ~0x2000000;
        *(u32*)party &= ~0x1000000;
        if (party != 0) {
            *(u32*)((s32)party + 0x4) &= ~0x100000;
        }
        *(u32*)((s32)party + 0xB8) = *(u32*)((s32)party + 0x58);
        *(u32*)((s32)party + 0xBC) = *(u32*)((s32)party + 0x5C);
        *(u32*)((s32)party + 0xC0) = *(u32*)((s32)party + 0x60);
        L_partyVivianTailStart(party);
    }
}

s32 partyDoWork(void) {
    extern void* marioGetPtr(void);
    extern void partyChgRunMode(void* party, s32 mode);
    extern f32 float_0_80421218;

    void* mario = marioGetPtr();
    void* party;

    if (*(s8*)((s32)mario + 0x245) >= 0) {
        party = partyPtrTbl[*(s8*)((s32)mario + 0x245)];
    } else {
        party = 0;
    }
    if (party == 0) {
        return 0;
    }
    if (*(u8*)((s32)party + 0x34) != 0 &&
        *(u8*)((s32)party + 0x34) != 1 &&
        *(u8*)((s32)party + 0x34) != 2) {
        return 0;
    }
    *(f32*)((s32)mario + 0x180) = float_0_80421218;
    partyChgRunMode(party, 3);
    return 1;
}

s32 yoshiSetColor(void) {
    extern s32 irand(s32 max);
    extern void pouchSetPartyColor(s32 partyId, s32 color);

    s32 value = irand(1000);
    s32 color;

    if (value < 150) {
        color = 0;
    } else if (value < 300) {
        color = 1;
    } else if (value < 450) {
        color = 2;
    } else if (value < 600) {
        color = 3;
    } else if (value < 750) {
        color = 4;
    } else if (value < 900) {
        color = 5;
    } else {
        color = 6;
    }
    pouchSetPartyColor(4, color);
    return color;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void N_partyForceChgRunMode(void* pParty, int newRunMode, u8 subMotionId) {
    extern void N__partyChgRunMode(void* party, s32 currRunModeId, s32 newRunModeId);

    u8 oldMode = *(u8*)((s32)pParty + 0x34);

    if (oldMode != 9 && oldMode != 3) {
        *(u8*)((s32)pParty + 0x35) = oldMode;
    }
    N__partyChgRunMode(pParty, oldMode, newRunMode);
    *(u8*)((s32)pParty + 0x34) = newRunMode;
    *(u32*)((s32)pParty + 0x8) &= ~8;
    *(u8*)((s32)pParty + 0x38) = subMotionId;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 partyChgPaper(void* pParty, char* paperPose) {
    extern s32 strcmp(const char*, const char*);
    if ((*(u32*)((s32)pParty + 4) & 2) == 0 &&
        (*(char**)((s32)pParty + 0x1C) == 0 ||
         strcmp(*(char**)((s32)pParty + 0x1C), paperPose) != 0)) {
        *(char**)((s32)pParty + 0x1C) = paperPose;
        *(u16*)((s32)pParty + 0x20) = 0;
        *(u32*)((s32)pParty + 8) |= 0x2000;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyChgRunMode(void* party, s32 mode) {
    extern void N__partyChgRunMode(void* party, s32 currRunModeId, s32 newRunModeId);

    u8 oldMode = *(u8*)((s32)party + 0x34);

    if (oldMode != 9 && oldMode != 3) {
        *(u8*)((s32)party + 0x35) = oldMode;
    }
    N__partyChgRunMode(party, oldMode, mode);
    *(u8*)((s32)party + 0x34) = mode;
    *(u32*)((s32)party + 0x8) |= 8;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 partyGoodbye(s32 id) {
    void* party;
    s32 ret = 0;

    if (id >= 0) {
        if (id >= 0) {
            party = partyPtrTbl[id];
        } else {
            party = 0;
        }
        partyChgRunMode(party, 8);
        ret = 1;
    }
    return ret;
}

s32 partyKill(s32 id) {
    extern void* partyPtrTbl[2];
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern void animPoseRelease(s32);
    extern void __memFree(s32, void*);
    void* party;
    void* current;
    void* player;
    s32 slot;
    s32 member;
    void (*callback)(void*);

    party = id < 0 ? 0 : partyPtrTbl[id];
    if (party == 0) return 0;
    player = *(void**)((s32)party + 0x160);
    slot = *(s8*)((s32)player + 0x245 + id);
    current = slot < 0 ? 0 : partyPtrTbl[slot];
    member = *(s8*)((s32)current + 0x31);
    party_dp = partyDataTbl + member * 0x28;
    callback = *(void (**)(void*))(partyDataTbl + member * 0x28 + 0x24);
    if (callback != 0 && *(u8*)((s32)party + 0x34) != 0xB) callback(party);
    slot = *(s8*)((s32)player + 0x245 + id);
    current = slot < 0 ? 0 : partyPtrTbl[slot];
    if (current == 0) {
        *(s8*)((s32)player + 0x247 + id) = 0;
    } else {
        slot = *(s8*)((s32)player + 0x245 + id);
        current = slot < 0 ? 0 : partyPtrTbl[slot];
        *(s8*)((s32)player + 0x247 + id) = *(s8*)((s32)current + 0x31);
    }
    *(s8*)((s32)player + 0x245 + id) = -1;
    partyPtrTbl[id] = 0;
    animPoseRelease(*(s32*)((s32)party + 0xC));
    __memFree(0, party);
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntry2Hello(s32 memberId) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern s32 marioGetCamId(void);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animPoseEntry(char* group, s32 flags);
    extern void animPosePeraOff(s32 poseId);
    extern s32 marioAnimeId(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern u32 animPoseGetMaterialLightFlag(s32 poseId);
    extern void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
    extern s32 partyGetHp(s32 partyId);
    extern s32 strcmp(const char* a, const char* b);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void* partyPtrTbl[2];
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern char* partyPoseTbl[][15];
    extern char* partyDotPoseTbl[][15];
    extern char* yoshiDotPoseTbl[][15];
    extern char* yoshigroup[];
    extern char str_d_mario_802cb340[];

    void* player;
    void* party;
    char* group;
    char* pose;
    s32 slotId;
    s32 poseId;
    s32 weak;
    s32 animId;
    u32 flags;
    void (*initFunc)(void*);

    player = marioGetPtr();
    if (partyPtrTbl[1] == 0) {
        partyPtrTbl[1] = __memAlloc(0, 0x188);
        memset(partyPtrTbl[1], 0, 0x188);
        slotId = 1;
    } else {
        slotId = -1;
    }
    if (slotId < 0) {
        return -1;
    }

    party = partyPtrTbl[slotId];
    memset(party, 0, 0x188);
    *(s32*)((s32)party + 0x164) = marioGetCamId();
    *(s32*)((s32)party + 0xC) = -1;
    *(s32*)((s32)party + 0x10) = -1;
    *(s32*)((s32)party + 0x14) = -1;
    *(u32*)((s32)party + 0x0) = 9;
    *(u8*)((s32)party + 0x2F) = slotId;
    *(void**)((s32)party + 0x160) = player;
    *(u8*)((s32)party + 0x31) = memberId;
    if (slotId == 1) {
        *(u32*)((s32)party + 0x0) |= 4;
    }

    party_dp = partyDataTbl + memberId * 0x28;
    if ((s8)*(u8*)((s32)party + 0x31) == 4) {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0) {
            group = str_d_mario_802cb340;
        }
    } else if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
        group = *(char**)party_dp;
    } else {
        group = str_d_mario_802cb340;
    }
    if (*(s32*)((s32)party + 0xC) >= 0) {
        animPoseRelease(*(s32*)((s32)party + 0xC));
    }
    *(s32*)((s32)party + 0xC) = animPoseEntry(group, 2);
    animPosePeraOff(*(s32*)((s32)party + 0xC));
    player = *(void**)((s32)party + 0x160);
    animId = marioAnimeId();
    flags = animPoseGetMaterialFlag(*(s32*)((s32)player + 0x22C + animId * 4));
    animPoseSetMaterialFlagOn(*(s32*)((s32)party + 0xC), flags);
    animId = marioAnimeId();
    flags = animPoseGetMaterialLightFlag(*(s32*)((s32)player + 0x22C + animId * 4));
    animPoseSetMaterialLightFlagOn(*(s32*)((s32)party + 0xC), flags);

    poseId = 1;
    if ((*(u32*)((s32)party + 0x4) & 2) == 0) {
        weak = 0;
        if ((s8)*(u8*)((s32)party + 0x31) < 8 &&
            partyGetHp((s8)*(u8*)((s32)party + 0x31)) <= 5) {
            weak = 1;
        }
        if (weak == 1) {
            poseId = 0xC;
        }
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        } else if ((s8)*(u8*)((s32)party + 0x31) == 4) {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
        } else {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        }
        if (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
            *(char**)((s32)party + 0x18) = pose;
            *(s16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 0x8) |= 0x2000;
        }
    } else {
        pose = *(char**)((s32)party + 0x18);
    }
    if ((*(u32*)((s32)party + 0x4) & 2) == 0 &&
        (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) &&
        ((*(u32*)((s32)party + 0x4) & 0x8000) == 0)) {
        *(char**)((s32)party + 0x18) = pose;
        *(s16*)((s32)party + 0x20) = 0;
        *(u32*)((s32)party + 0x8) |= 0x2000;
    }
    *(char**)((s32)party + 0x1C) = 0;
    initFunc = *(void (**)(void*))((s32)party_dp + 4);
    initFunc(party);
    partyChgRunMode(party, 4);
    return slotId;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

s32 partyKill2(s32 id) {
    extern void* partyPtrTbl[2];
    extern u8 partyDataTbl[];
    extern void* party_dp;
    extern void animPoseRelease(s32);
    extern void __memFree(s32, void*);
    void* party;
    void* current;
    void* player;
    s32 slot;
    s32 member;
    void (*callback)(void*);

    party = id < 0 ? 0 : partyPtrTbl[id];
    if (party == 0) return 0;
    player = *(void**)((s32)party + 0x160);
    slot = *(s8*)((s32)player + 0x245 + id);
    current = slot < 0 ? 0 : partyPtrTbl[slot];
    member = *(s8*)((s32)current + 0x31);
    party_dp = partyDataTbl + member * 0x28;
    callback = *(void (**)(void*))(partyDataTbl + member * 0x28 + 0x24);
    if (callback != 0 && *(u8*)((s32)party + 0x34) != 0xB) callback(party);
    *(s8*)((s32)player + 0x247 + id) = 0;
    *(s8*)((s32)player + 0x245 + id) = -1;
    partyPtrTbl[id] = 0;
    animPoseRelease(*(s32*)((s32)party + 0xC));
    __memFree(0, party);
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

