#include "party/party.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "party/party_vivian.h"


void* partyPtrTbl[2];
void animPoseSetMaterialLightFlagOff(void* pose, s32 flag);
s32 marioGetCamId(void);
void partyChgRunMode(void* party, s32 mode);
s32 partyEntryMain(s32 partyMemberId);
s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
void partyChgMot(void* party, s32 mot);
void animPoseSetPaperAnimGroup(void* pose, s32 a, s32 b);
void animPaperPoseRelease(s32 id);
s32 strcmp(const char* a, const char* b);
extern u8 partyDataTbl[];
extern char* partyPoseTbl[][15];
extern char* partyDotPoseTbl[][15];
extern char* yoshiDotPoseTbl[][15];
void animPoseRelease(s32 poseId);
void animPosePeraOff(s32 poseId);
s32 yoshiGetStatus(void);
extern char* yoshigroup[];
s32 marioAnimeId(void);
f32 revise360(f32 angle);
void* __memAlloc(s32 heap, u32 size);
void __memFree(s32 heap, void* ptr);
void getRidePos(void* party, void* pos);
f32 toMovedir(f32 angle);
s32 animPoseEntry(char* group, s32 flags);
u32 animPoseGetMaterialFlag(s32 poseId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
u32 animPoseGetMaterialLightFlag(s32 poseId);
void animPoseSetMaterialLightFlagOn(s32 poseId, u32 flags);
s32 animPaperPoseEntry(char* group, s32 flags);
void partyClearJumpPara(void* party);
f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
void movePos(f32 dist, f32 dir, f32* x, f32* z);

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyPaperOn(void* pParty, char* paperGroup) {
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern char str_p_roll_802cb36c[];
    extern char str_p_plane_802cb374[];
    extern char str_p_dokan_x_802cb37c[];
    extern char str_p_dokan_y_802cb360[];
    extern char str_p_slit_802cb388[];
    s32 special;
    s32 result;
    extern u32 vec3_802cb178[];
    char* base = (char*)vec3_802cb178;

    if (*(s32*)((s32)pParty + 0x14) >= 0) {
        animPaperPoseRelease(*(s32*)((s32)pParty + 0x14));
        *(s32*)((s32)pParty + 0x14) = -1;
    }
    *(u32*)((s32)pParty + 4) |= 0x4000000;
    *(s32*)((s32)pParty + 0x14) = animPaperPoseEntry(paperGroup, 2);
    special = 0;
    if (strcmp(paperGroup, base + 0x1F4) == 0 ||
        strcmp(paperGroup, base + 0x1FC) == 0 ||
        strcmp(paperGroup, base + 0x204) == 0 ||
        strcmp(paperGroup, base + 0x1E8) == 0) special = 1;
    result = !special;
    if (strcmp(paperGroup, base + 0x210) == 0) {
        result = *(s8*)((s32)pParty + 0x31) == 5;
    }
    animPoseSetPaperAnimGroup(*(s32*)((s32)pParty + 0xC), paperGroup, result);
    return 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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

void partyPaperLightOff(void* party) {
    animPoseSetMaterialLightFlagOff(*(void**)((s32)party + 0xC), 1);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
char* partyChgPoseId(void* party, s32 poseId) {
    s32 weak;
    char* pose;

    if ((*(u32*)((s32)party + 4) & 2) != 0) return *(char**)((s32)party + 0x18);
    weak = 0;
    if (*(s8*)((s32)party + 0x31) < 8 && partyGetHp(*(s8*)((s32)party + 0x31)) <= 5) weak = 1;
    if (weak == 1) {
        if (poseId == 1) poseId = 0xC;
        else if (poseId == 2) poseId = 0xD;
        else if (poseId == 3) poseId = 0xE;
    }
    if ((*(u32*)((s32)party + 4) & 0x8000) == 0) {
        pose = partyPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
    } else if (*(s8*)((s32)party + 0x31) != 4) {
        pose = partyDotPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
    } else {
        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
    }
    if (*(char**)((s32)party + 0x18) == 0 ||
        strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
        *(char**)((s32)party + 0x18) = pose;
        *(u16*)((s32)party + 0x20) = 0;
        *(u32*)((s32)party + 8) |= 0x2000;
    }
    return pose;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void partyChgPaper(void* pParty, char* paperPose) {
    if ((*(u32*)((s32)pParty + 4) & 2) == 0 &&
        (*(char**)((s32)pParty + 0x1C) == 0 ||
         strcmp(*(char**)((s32)pParty + 0x1C), paperPose) != 0)) {
        *(char**)((s32)pParty + 0x1C) = paperPose;
        *(u16*)((s32)pParty + 0x20) = 0;
        *(u32*)((s32)pParty + 8) |= 0x4000;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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

void partyRun(void* party) {
    if (*(u8*)((s32)party + 0x34) == 0) {
        partyChgRunMode(party, 1);
    }
}

void partyStop(void* party) {
    partyChgRunMode(party, 0);
}

void partySetForceMove(void* party, s32 angle, f32 x, f32 z) {
    *(f32*)((s32)party + 0x100) = x;
    *(s16*)((s32)party + 0x28) = angle;
    *(f32*)((s32)party + 0x104) = z;

    partyChgRunMode(party, 9);
    partyChgMot(party, 1);
}

s32 partyEntryHello(s32 partyMemberId) {
    s32 id;
    void* party;

    id = partyEntryMain(partyMemberId);

    if (id >= 0) {
        party = partyPtrTbl[id];
    } else {
        party = 0;
    }

    partyChgRunMode(party, 4);
    return id;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntry(s32 partyMemberId) {
    extern void partyGetAppearPos2(void* party, void* pos);
    extern u32 vec3_802cb178[];

    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    s32 id;
    void* party;
    s32 pose;
    s32 weak;
    s32 member;

    id = partyEntryMain(partyMemberId);
    if (id >= 0) {
        party = partyPtrTbl[id];
    } else {
        party = 0;
    }
    *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)vec3_802cb178;
    *(u8*)((s32)party + 0x38) = 0xA;
    *(f32*)((s32)party + 0x110) = *(f32*)((s32)*(void**)((s32)party + 0x160) + 0x1AC);
    *(f32*)((s32)party + 0x10C) = *(f32*)((s32)*(void**)((s32)party + 0x160) + 0x1AC);
    partyGetAppearPos2(party, (void*)((s32)party + 0x58));
    pose = 1;
    *(VecLocal*)((s32)party + 0xB8) = *(VecLocal*)((s32)party + 0x58);
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
s32 partyEntryPos(double xIn, double yIn, double zIn, s32 partyMemberId) {
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyKill(s32 id) {
    extern void* party_dp;
    void* party;
    void* current;
    char* slotData;
    void* player;
    s32 slot;
    s32 member;
    void (*callback)(void*);

    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party == 0) return 0;
    player = *(void**)((s32)party + 0x160);
    slotData = (char*)player + id;
    slot = *(s8*)(slotData + 0x245);
    current = slot >= 0 ? partyPtrTbl[slot] : 0;
    member = *(s8*)((s32)current + 0x31);
    party_dp = partyDataTbl + member * 0x28;
    callback = *(void (**)(void*))((s32)party_dp + 0x24);
    if (callback != 0 && *(u8*)((s32)party + 0x34) != 0xB) callback(party);
    slot = *(s8*)(slotData + 0x245);
    current = slot >= 0 ? partyPtrTbl[slot] : 0;
    if (current != 0) {
        current = slot >= 0 ? partyPtrTbl[slot] : 0;
        *(s8*)((s32)player + 0x247 + id) = *(s8*)((s32)current + 0x31);
    } else {
        *(s8*)((s32)player + 0x247 + id) = 0;
    }
    *(s8*)(slotData + 0x245) = -1;
    partyPtrTbl[id] = 0;
    animPoseRelease(*(s32*)((s32)party + 0xC));
    __memFree(0, party);
    return 0;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyKill2(s32 id) {
    extern void* party_dp;
    void* party;
    void* current;
    void* player;
    char* slotData;
    s32 slot;
    s32 member;
    void (*callback)(void*);

    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party == 0) return 0;
    player = *(void**)((s32)party + 0x160);
    slotData = (char*)player + id;
    slot = *(s8*)(slotData + 0x245);
    current = slot >= 0 ? partyPtrTbl[slot] : 0;
    member = *(s8*)((s32)current + 0x31);
    party_dp = partyDataTbl + member * 0x28;
    callback = *(void (**)(void*))((char*)party_dp + 0x24);
    if (callback != 0 && *(u8*)((s32)party + 0x34) != 0xB) callback(party);
    *(s8*)((s32)player + 0x247 + id) = 0;
    *(s8*)(slotData + 0x245) = -1;
    partyPtrTbl[id] = 0;
    animPoseRelease(*(s32*)((s32)party + 0xC));
    __memFree(0, party);
    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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

f32 partyGetHeight(void* party) {
    extern f32 float_18p5_804212d4;

    if ((s32)*(u8*)((s32)party + 0x31) == 4) {
        if ((*(u32*)((s32)party + 0x0) & 0x100) != 0) {
            return float_18p5_804212d4 + *(f32*)((s32)party + 0xF0);
        }
    }

    return *(f32*)((s32)party + 0xF0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyEntryMain(s32 memberId) {
    extern void* party_dp;
    extern char str_d_mario_802cb340[];

    void* player;
    void* party;
    char* group;
    char* pose;
    s32 slotId;
    void** slots;
    s32 poseId;
    s32 weak;
    s32 animId;
    u32 flags;
    void (*initFunc)(void*);

    player = marioGetPtr();
    slots = partyPtrTbl;
    for (slotId = 0; slotId < 2; slotId++, slots++) {
        if (*slots == 0) {
            partyPtrTbl[slotId] = __memAlloc(0, 0x188);
            memset(partyPtrTbl[slotId], 0, 0x188);
            goto allocated;
        }
    }
    slotId = -1;
allocated:
    if (slotId < 0) {
        return -1;
    }

    party = slotId >= 0 ? partyPtrTbl[slotId] : 0;
    memset(party, 0, 0x188);
    *(s32*)((s32)party + 0x164) = marioGetCamId();
    *(s32*)((s32)party + 0xC) = -1;
    *(s32*)((s32)party + 0x10) = -1;
    *(s32*)((s32)party + 0x14) = -1;
    *(u32*)((s32)party + 0x0) = 1;
    *(u8*)((s32)party + 0x2F) = slotId;
    *(void**)((s32)party + 0x160) = player;
    *(u8*)((s32)party + 0x31) = memberId;
    if (*(s8*)((s32)player + 0x280) == -1) {
        *(u8*)((s32)player + 0x280) = *(u8*)((s32)party + 0x31);
    }
    if (slotId == 1) {
        *(u32*)((s32)party + 0x0) |= 4;
    }

    party_dp = partyDataTbl + memberId * 0x28;
    if ((*(u32*)((s32)player + 0x0) & 0x100) != 0) {
        *(u32*)((s32)party + 0x4) |= 0x8000;
    }
    if (*(u8*)((s32)party + 0x31) != 4) {
        if ((*(u32*)((s32)party + 4) & 0x8000) == 0) group = *(char**)party_dp;
        else group = str_d_mario_802cb340;
    } else {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 4) & 0x8000) != 0) group = str_d_mario_802cb340;
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
        } else if (*(s8*)((s32)party + 0x31) != 4) {
            pose = partyDotPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
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
    return slotId;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
#pragma opt_common_subs off
s32 partyEntry2(s32 memberId) {
    extern void* party_dp;
    extern char str_d_mario_802cb340[];

    extern u32 vec3_802cb178[];
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    void* originalPlayer;
    void** slots;
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

    originalPlayer = marioGetPtr();
    player = originalPlayer;
    slots = partyPtrTbl;
    if (slots[1] == 0) {
        slots[1] = __memAlloc(0, 0x188);
        memset(slots[1], 0, 0x188);
        slotId = 1;
    } else {
        slotId = -1;
    }
    if (slotId < 0) {
        return -1;
    }

    party = slotId >= 0 ? slots[slotId] : 0;
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
    if (*(u8*)((s32)party + 0x31) != 4) {
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0)
            group = *(char**)party_dp;
        else
            group = str_d_mario_802cb340;
    } else {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0)
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
        } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
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
    *(VecLocal*)((s32)party + 0x58) = *(VecLocal*)((s32)originalPlayer + 0x8C);
    *(VecLocal*)((s32)party + 0xB8) = *(VecLocal*)((s32)party + 0x58);
    *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)&vec3_802cb178[0x27];
    *(u8*)((s32)party + 0x34) = 0;
    return slotId;
}
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
#pragma opt_common_subs off
s32 partyEntry2Hello(s32 memberId) {
    extern void* party_dp;
    extern char str_d_mario_802cb340[];

    void** slots;
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
    slots = partyPtrTbl;
    if (slots[1] == 0) {
        slots[1] = __memAlloc(0, 0x188);
        memset(slots[1], 0, 0x188);
        slotId = 1;
    } else {
        slotId = -1;
    }
    if (slotId < 0) {
        return -1;
    }

    party = slotId >= 0 ? slots[slotId] : 0;
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
    if (*(u8*)((s32)party + 0x31) != 4) {
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0)
            group = *(char**)party_dp;
        else
            group = str_d_mario_802cb340;
    } else {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0)
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
        } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
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
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
s32 partyEntry2Pos(s32 memberId, f32 x, f32 y, f32 z) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void* party_dp;
    extern char str_d_mario_802cb340[];
    extern f32 float_1_80421200;
    extern VecLocal vec3_802cb220[];

    void** slots;
    f32 scale;
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
    slots = partyPtrTbl;
    if (slots[1] == 0) {
        slots[1] = __memAlloc(0, 0x188);
        memset(slots[1], 0, 0x188);
        slotId = 1;
    } else {
        slotId = -1;
    }

    if (slotId < 0) {
        return -1;
    }

    party = slotId >= 0 ? slots[slotId] : 0;
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
    if (*(u8*)((s32)party + 0x31) != 4) {
        if ((*(u32*)((s32)party + 0x4) & 0x8000) == 0)
            group = *(char**)party_dp;
        else
            group = str_d_mario_802cb340;
    } else {
        group = yoshigroup[pouchGetPartyColor(4)];
        if ((*(u32*)((s32)party + 0x4) & 0x8000) != 0)
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
        } else if ((s8)*(u8*)((s32)party + 0x31) != 4) {
            pose = partyDotPoseTbl[(s8)*(u8*)((s32)party + 0x31)][poseId];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
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

    scale = float_1_80421200;
    pos = vec3_802cb220[0];
    pos.x = x;
    pos.y = y;
    pos.z = z;
    *(VecLocal*)((s32)party + 0x58) = pos;
    *(VecLocal*)((s32)party + 0xB8) = *(VecLocal*)((s32)party + 0x58);
    *(f32*)((s32)party + 0x70) = scale;
    *(f32*)((s32)party + 0x74) = scale;
    *(f32*)((s32)party + 0x78) = scale;
    return slotId;
}
#pragma no_register_save_helpers reset


void partyInit(void) {
    memset(partyPtrTbl, 0, 8);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyReInit(void) {
    extern void animPoseSetMaterialFlagOff(s32, u32);
    extern void animPoseSetMaterialLightFlagOn(s32, u32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern void partyChgRunMode(void*, s32);
    extern void partyChgMot(void*, s32);
    extern void partyClearFootmark(void);
    extern void chuchu_reinit(void);
    extern void unk_JP_US_EU_36_80151d20(void);
    extern char** party_dp;
    extern char str_d_mario_802cb340[];
    extern u32 vec3_802cb178[];
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    u32* base = vec3_802cb178;
    void* party;
    void* player;
    char* pose;
    char* group;
    s32 id;
    s32 member;
    s32 poseId;
    s32 weak;
    s32 animId;
    u32 oldDispFlags;

    id = marioGetExtraPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        *(u32*)party = 0xD;
        *(u32*)((s32)party + 4) = 0;
        animId = *(s32*)((s32)party + 0xC);
        if (animId >= 0) {
            animPoseSetMaterialFlagOff(animId, 0xFFFFFFFF);
            animPoseSetMaterialLightFlagOn(animId, 2);
        }
        *(u8*)((s32)party + 0x37) = 0;
        if (*(char**)((s32)party + 0x1C) != 0) {
            animPoseSetPaperAnimGroup(*(s32*)((s32)party + 0xC), 0, 0);
            *(char**)((s32)party + 0x1C) = 0;
            *(u32*)((s32)party + 4) &= ~0x4000000;
            if (*(s32*)((s32)party + 0x14) >= 0) {
                animPaperPoseRelease(*(s32*)((s32)party + 0x14));
                *(s32*)((s32)party + 0x14) = -1;
            }
        }
        *(void**)((s32)party + 0x130) = 0;
        *(void**)((s32)party + 0x12C) = 0;
        *(void**)((s32)party + 0x148) = 0;
        *(void**)((s32)party + 0x138) = 0;
        *(void**)((s32)party + 0x13C) = 0;
        *(void**)((s32)party + 0x134) = 0;
        *(void**)((s32)party + 0x140) = 0;
        *(void**)((s32)party + 0x144) = 0;
        weak = 0;
        poseId = 1;
        if ((*(u32*)((s32)party + 4) & 2) == 0) {
            member = *(s8*)((s32)party + 0x31);
            if (member < 8 && partyGetHp(member) <= 5) weak = 1;
            if (weak == 1) poseId = 0xC;
            if ((*(u32*)((s32)party + 4) & 0x8000) == 0) {
                pose = partyPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
            } else if (*(s8*)((s32)party + 0x31) != 4) {
                pose = partyDotPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
            } else {
                pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
            }
            if (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
                *(char**)((s32)party + 0x18) = pose;
                *(u16*)((s32)party + 0x20) = 0;
                *(u32*)((s32)party + 8) |= 0x2000;
            }
        } else {
            pose = *(char**)((s32)party + 0x18);
        }
        if ((*(u32*)((s32)party + 4) & 2) == 0 &&
            (*(char**)((s32)party + 0x18) == 0 || strcmp(*(char**)((s32)party + 0x18), pose) != 0) &&
            ((*(u32*)((s32)party + 4) & 0x8000) == 0)) {
            *(char**)((s32)party + 0x18) = pose;
            *(u16*)((s32)party + 0x20) = 0;
            *(u32*)((s32)party + 8) |= 0x2000;
        }
        partyChgRunMode(party, 2);
        partyChgMot(party, 0);
        *(s32*)((s32)party + 0x168) = -1;
        *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)&base[0x2D];
    }

    id = marioGetPartyId();
    party = id >= 0 ? partyPtrTbl[id] : 0;
    if (party != 0) {
        oldDispFlags = *(u32*)((s32)party + 4) & 0x8000;
        *(u32*)party = 1;
        *(u32*)((s32)party + 4) = 0;
        *(u8*)((s32)party + 0x37) = 0;
        *(s32*)((s32)party + 0x164) = marioGetCamId();
        if (party != 0) {
        animId = *(s32*)((s32)party + 0xC);
        if (animId >= 0) {
            animPoseSetMaterialFlagOff(animId, 0xFFFFFFFF);
            animPoseSetMaterialLightFlagOn(animId, 2);
        }
        if (*(char**)((s32)party + 0x1C) != 0) {
            animPoseSetPaperAnimGroup(*(s32*)((s32)party + 0xC), 0, 0);
            *(char**)((s32)party + 0x1C) = 0;
            *(u32*)((s32)party + 4) &= ~0x4000000;
            if (*(s32*)((s32)party + 0x14) >= 0) {
                animPaperPoseRelease(*(s32*)((s32)party + 0x14));
                *(s32*)((s32)party + 0x14) = -1;
            }
        }
        *(void**)((s32)party + 0x130) = 0;
        *(void**)((s32)party + 0x12C) = 0;
        *(void**)((s32)party + 0x148) = 0;
        *(void**)((s32)party + 0x138) = 0;
        *(void**)((s32)party + 0x13C) = 0;
        *(void**)((s32)party + 0x134) = 0;
        *(void**)((s32)party + 0x140) = 0;
        *(void**)((s32)party + 0x144) = 0;
        *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)&base[0x30];
        }
        partyClearFootmark();
        if (oldDispFlags != 0) {
            if (*(u8*)((s32)party + 0x31) != 4) {
                if ((*(u32*)((s32)party + 4) & 0x8000) == 0)
                    group = *(char**)party_dp;
                else
                    group = (char*)base + 0x1C8;
            } else {
                group = yoshigroup[pouchGetPartyColor(4)];
                if (*(u32*)((s32)party + 4) & 0x8000)
                    group = (char*)base + 0x1C8;
            }
            if (*(s32*)((s32)party + 0xC) >= 0) {
                animPoseRelease(*(s32*)((s32)party + 0xC));
            }
            *(s32*)((s32)party + 0xC) = animPoseEntry(group, 2);
        }
        if (*(u8*)((s32)party + 0x31) != 4 || *(u8*)((s32)party + 0x34) != 3) {
            player = marioGetPtr();
            if ((*(u32*)player & 0x00400000) == 0) {
                poseId = 1;
                if ((*(u32*)((s32)party + 4) & 2) == 0) {
                    weak = 0;
                    member = *(s8*)((s32)party + 0x31);
                    if (member < 8 && partyGetHp(member) <= 5) weak = 1;
                    if (weak == 1) poseId = 0xC;
                    if ((*(u32*)((s32)party + 4) & 0x8000) == 0) {
                        pose = partyPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
                    } else if (*(s8*)((s32)party + 0x31) != 4) {
                        pose = partyDotPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
                    } else {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                    }
                    if (*(char**)((s32)party + 0x18) == 0 ||
                        strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
                        *(char**)((s32)party + 0x18) = pose;
                        *(u16*)((s32)party + 0x20) = 0;
                        *(u32*)((s32)party + 8) |= 0x2000;
                    }
                } else {
                    pose = *(char**)((s32)party + 0x18);
                }
                if ((*(u32*)((s32)party + 4) & 2) == 0 &&
                    (*(char**)((s32)party + 0x18) == 0 ||
                     strcmp(*(char**)((s32)party + 0x18), pose) != 0) &&
                    ((*(u32*)((s32)party + 4) & 0x8000) == 0)) {
                    *(char**)((s32)party + 0x18) = pose;
                    *(u16*)((s32)party + 0x20) = 0;
                    *(u32*)((s32)party + 8) |= 0x2000;
                }
                partyChgRunMode(party, 2);
                partyChgMot(party, 0);
            } else {
                poseId = 1;
                if ((*(u32*)((s32)party + 4) & 2) == 0) {
                    weak = 0;
                    member = *(s8*)((s32)party + 0x31);
                    if (member < 8 && partyGetHp(member) <= 5) weak = 1;
                    if (weak == 1) poseId = 0xC;
                    if ((*(u32*)((s32)party + 4) & 0x8000) == 0) {
                        pose = partyPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
                    } else if (*(s8*)((s32)party + 0x31) != 4) {
                        pose = partyDotPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
                    } else {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                    }
                    if (*(char**)((s32)party + 0x18) == 0 ||
                        strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
                        *(char**)((s32)party + 0x18) = pose;
                        *(u16*)((s32)party + 0x20) = 0;
                        *(u32*)((s32)party + 8) |= 0x2000;
                    }
                } else {
                    pose = *(char**)((s32)party + 0x18);
                }
                if ((*(u32*)((s32)party + 4) & 2) == 0 &&
                    (*(char**)((s32)party + 0x18) == 0 ||
                     strcmp(*(char**)((s32)party + 0x18), pose) != 0) &&
                    ((*(u32*)((s32)party + 4) & 0x8000) == 0)) {
                    *(char**)((s32)party + 0x18) = pose;
                    *(u16*)((s32)party + 0x20) = 0;
                    *(u32*)((s32)party + 8) |= 0x2000;
                }
                partyChgRunMode(party, 2);
                *(u8*)((s32)party + 0x3B) = 6;
                *(u8*)((s32)party + 0x38) = 0;
                *(u32*)party |= 0x8000;
            }
            if (*(u8*)((s32)party + 0x31) == 6) {
                chuchu_reinit();
            }
            unk_JP_US_EU_36_80151d20();
        }
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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
void N__partyChgRunMode(void* pParty, int currRunModeId, int newRunModeId) {
    extern u32 vec3_802cb178[];
    extern void L_partyForceSlitOff(void*);
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    u32* base = vec3_802cb178;
    void* player;
    char* pose;
    s32 member;
    s32 poseId;
    s32 weak;
    void (*callback)(void);

    switch (currRunModeId) {
        case 0xB:
            if (newRunModeId == 8) break;
            player = *(void**)((s32)pParty + 0x160);
            *(VecLocal*)((s32)pParty + 0x70) = *(VecLocal*)&base[0x66];
            *(VecLocal*)((s32)pParty + 0x7C) = *(VecLocal*)&base[0x69];
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)player + 0x1A0);
            *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)player + 0x1A4);
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(u32*)pParty &= ~0x70000;
            partyClearJumpPara(pParty);
            poseId = 1;
            if ((*(u32*)((s32)pParty + 4) & 2) == 0) {
                weak = 0;
                member = *(s8*)((s32)pParty + 0x31);
                if (member < 8 && partyGetHp(member) <= 5) weak = 1;
                if (weak == 1) poseId = 0xC;
                if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 8) |= 0x2000;
                }
            }
            *(u32*)((s32)pParty + 4) &= ~0x10;
            *(u32*)((s32)pParty + 4) &= ~1;
            if (pParty != 0) *(u32*)((s32)pParty + 4) &= ~0x100000;
            break;
        case 4:
            player = *(void**)((s32)pParty + 0x160);
            *(VecLocal*)((s32)pParty + 0x70) = *(VecLocal*)&base[0x45];
            *(u32*)((s32)pParty + 4) &= ~0x10;
            *(f32*)((s32)pParty + 0xFC) = *(f32*)((s32)player + 0x1A0);
            *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)player + 0x1A4);
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
            partyClearJumpPara(pParty);
            poseId = 1;
            if ((*(u32*)((s32)pParty + 4) & 2) == 0) {
                weak = 0;
                member = *(s8*)((s32)pParty + 0x31);
                if (member < 8 && partyGetHp(member) <= 5) weak = 1;
                if (weak == 1) poseId = 0xC;
                if ((*(u32*)((s32)pParty + 4) & 0x8000) == 0) {
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 || strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 8) |= 0x2000;
                }
            }
            *(u32*)((s32)pParty + 4) &= ~0x10;
            *(u8*)((s32)pParty + 0x34) = 1;
            break;
        case 0xC:
            if (pParty != 0) *(u32*)((s32)pParty + 4) &= ~0x100000;
            break;
        case 3:
            member = *(s8*)((s32)pParty + 0x31);
            callback = *(void (**)(void))(partyDataTbl + member * 0x28 + 0x10);
            if (callback != 0) callback();
            break;
        case 1:
        case 2:
            L_partyForceSlitOff(pParty);
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800bc660(void* party) {
    extern u32 vec3_802cb178[];
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyMain(void) {
    extern void unk_8014140c(void);
    extern s32 marioBgmodeChk(void);
    extern void driveParty(void* party);

    s32 active;
    s32 i;
    void* party;
    void** ptr;

    marioGetPtr();
    ptr = partyPtrTbl;
    active = 0;
    party = ptr[0];
    if (party != 0 && (*(u32*)(active >= 0 ? party : (void*)active) & 1) != 0) {
        active = 1;
    }
    i = 1;
    party = ptr[i];
    if (party != 0 && (*(u32*)(i >= 0 ? party : 0) & 1) != 0) {
        active++;
    }
    if (active != 0) {
        unk_8014140c();
        for (i = 0; i < 2; i++, ptr++) {
            party = *ptr;
            if (party != 0 && (*(u32*)party & 1) != 0 && marioBgmodeChk() != 1) {
                driveParty(party);
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 driveParty(void* party) {
    extern void* party_dp;
    extern void unk_JP_US_EU_13_800ca884(void*);
    extern void partyHelloMain(void*);
    extern void partyDokanInit(void*);
    extern void partyDokanMain(void*);
    extern void partyRideMain(void*);
    extern void partyRideYoshiMain(void*);
    extern void party_damage_return(void*);
    extern void partyForceWalk(void*);
    extern void partyPreDisp(void*);
    extern void party_kemuri(void*);
    extern void partyChgMot(void*, s32);
    extern void L_partyForceSlitOff(void*);
    extern s32 vivianGetStatus(void);
    extern void vivianShadow(void*);
    extern void marioGetScreenPos(void*, f32*, f32*, f32*);
    extern s32 marioChkInScreen(s32, s32);
    extern u32 vec3_802cb244[];
    extern u32 vec3_802cb250[];
    extern u32 vec3_802cb25c[];
    extern u32 vec3_802cb268[];

    extern u32 vec3_802cb178[];
    u32* base = vec3_802cb178;
    void* player = *(void**)((s32)party + 0x160);
    s32 mode;
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    VecLocal screenPos;
    f32 sx, sy, sz;
    u32 flags;
    f32 x, y;
    s32 member;
    s32 poseId;
    s32 weak;
    char* pose;
    void (*callback)(void*);

    *(f32*)((s32)party + 0xF8) = *(f32*)((s32)player + 0x19C);
    if (*(s16*)((s32)party + 0x2A) > 0) {
        (*(s16*)((s32)party + 0x2A))--;
    }
    mode = *(u8*)((s32)party + 0x34);
    if (mode != 0xE && (mode >= 0xE || mode >= 9 || mode < 4) && (*(u32*)party & 0x20000000) != 0) {
        if (vivianGetStatus() != 2) {
            vivianShadow(party);
        }
        partyPreDisp(party);
        return 0;
    } else {
        if ((*(u32*)player & 0x02000000) == 0) {
            *(u32*)party &= ~0x00800000;
        } else {
            *(u32*)party |= 0x00800000;
        }
        *(void**)((s32)party + 0x138) = 0;
        *(void**)((s32)party + 0x12C) = 0;
        *(void**)((s32)party + 0x140) = 0;
        *(void**)((s32)party + 0x134) = 0;
        *(void**)((s32)party + 0x130) = 0;
        switch (*(u8*)((s32)party + 0x34)) {
            case 0:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    if ((*(u32*)((s32)party + 8) & 0x8) != 0) {
                        *(u32*)((s32)party + 8) &= ~0x8;
                        if ((*(u32*)((s32)party + 4) & 2) == 0) {
                            poseId = 1;
                            weak = 0;
                            member = (s8)*(u8*)((s32)party + 0x31);
                            if (member < 8 && partyGetHp(member) <= 5) {
                                weak = 1;
                            }
                            if (weak == 1) {
                                poseId = 0xC;
                            }
                            if ((*(u32*)((s32)party + 4) & 0x8000) == 0) {
                                pose = partyPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
                            } else if (*(s8*)((s32)party + 0x31) != 4) {
                                pose = partyDotPoseTbl[*(s8*)((s32)party + 0x31)][poseId];
                            } else {
                                pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                            }
                            if (*(char**)((s32)party + 0x18) == 0 ||
                                strcmp(*(char**)((s32)party + 0x18), pose) != 0) {
                                *(char**)((s32)party + 0x18) = pose;
                                *(u16*)((s32)party + 0x20) = 0;
                                *(u32*)((s32)party + 8) |= 0x2000;
                            }
                        }
                        *(f32*)((s32)party + 0x104) = 0.0f;
                        *(u32*)party &= ~0x100;
                        partyChgMot(party, 0);
                    }
                    member = (s8)*(u8*)((s32)party + 0x31);
                    party_dp = partyDataTbl + member * 0x28;
                    callback = *(void (**)(void*))((u8*)party_dp + 8);
                    callback(party);
                }
                break;
            case 1:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    unk_JP_US_EU_13_800ca884(party);
                }
                break;
            case 2:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    member = (s8)*(u8*)((s32)party + 0x31);
                    party_dp = partyDataTbl + member * 0x28;
                    callback = *(void (**)(void*))((u8*)party_dp + 8);
                    callback(party);
                }
                break;
            case 3:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    member = (s8)*(u8*)((s32)party + 0x31);
                    party_dp = partyDataTbl + member * 0x28;
                    callback = *(void (**)(void*))((u8*)party_dp + 0xC);
                    *(u32*)party |= 0x100;
                    callback(party);
                }
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                if ((*(u32*)((s32)party + 8) & 0x8) != 0) {
                    *(u32*)((s32)party + 8) &= ~0x8;
                    player = *(void**)((s32)party + 0x160);
                    if (*(u16*)((s32)player + 0x2E) == 0x16 ||
                        *(u16*)((s32)player + 0x2E) == 0x18 ||
                        *(u16*)((s32)player + 0x2E) == 0x19) {
                        *(u8*)((s32)party + 0x38) = 0x1E;
                    } else if (*(u8*)((s32)party + 0x34) == 4 || *(u8*)((s32)party + 0x34) == 5) {
                        *(u32*)((s32)party + 4) |= 0x10;
                        *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)&base[0x36];
                        *(u8*)((s32)party + 0x38) = 0;
                        *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1AC);
                        *(f32*)((s32)party + 0x10C) = *(f32*)((s32)player + 0x1AC);
                        *(VecLocal*)((s32)party + 0x58) = *(VecLocal*)((s32)player + 0x8C);
                        *(VecLocal*)((s32)party + 0xB8) = *(VecLocal*)((s32)party + 0x58);
                    } else if (*(u8*)((s32)party + 0x34) == 7) {
                        *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)&base[0x39];
                        *(u8*)((s32)party + 0x38) = 0x14;
                        *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1AC);
                        *(f32*)((s32)party + 0x10C) = *(f32*)((s32)player + 0x1AC);
                    } else {
                        *(VecLocal*)((s32)party + 0x70) = *(VecLocal*)&base[0x3C];
                        *(u8*)((s32)party + 0x38) = 0xA;
                        *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1AC);
                        *(f32*)((s32)party + 0x10C) = *(f32*)((s32)player + 0x1AC);
                        *(VecLocal*)((s32)party + 0x58) = *(VecLocal*)((s32)player + 0x8C);
                        *(VecLocal*)((s32)party + 0xB8) = *(VecLocal*)((s32)party + 0x58);
                    }
                    *(u32*)party &= ~0x100;
                }
                partyHelloMain(party);
                break;
            case 8:
                member = (s8)*(u8*)((s32)party + 0x31);
                party_dp = partyDataTbl + member * 0x28;
                callback = *(void (**)(void*))((u8*)party_dp + 0x14);
                callback(party);
                break;
            case 9:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    partyForceWalk(party);
                }
                break;
            case 10:
                if ((*(u32*)((s32)party + 8) & 0x8) != 0) {
                    *(u32*)((s32)party + 8) &= ~0x8;
                    partyDokanInit(party);
                    *(u32*)party &= ~0x100;
                }
                partyDokanMain(party);
                break;
            case 11:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    if ((*(u32*)((s32)party + 8) & 0x8) != 0) {
                        *(u32*)((s32)party + 8) &= ~0x8;
                        *(u8*)((s32)party + 0x38) = 0;
                        *(u32*)party &= ~0x100;
                        L_partyForceSlitOff(party);
                    }
                    partyRideMain(party);
                }
                break;
            case 12:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    if ((*(u32*)((s32)party + 8) & 0x8) != 0) {
                        *(u32*)((s32)party + 8) &= ~0x8;
                        *(u8*)((s32)party + 0x38) = 0;
                        *(u32*)party &= ~0x100;
                    }
                    partyRideYoshiMain(party);
                }
                break;
            case 13:
                if (((*(u32*)party & 0x20000000) == 0) || ((*(u32*)party & 0x10000000) != 0)) {
                    party_damage_return(party);
                }
                break;
            case 14:
                player = *(void**)((s32)party + 0x160);
                if ((*(u32*)((s32)party + 8) & 0x8) != 0) {
                    *(u32*)((s32)party + 8) &= ~0x8;
                }
                member = *(u8*)((s32)party + 0x2F);
                *(u8*)((s32)player + 0x247 + member) = *(u8*)((s32)party + 0x31);
                *(u8*)((s32)player + 0x245 + member) = -1;
                partyPtrTbl[member] = 0;
                animPoseRelease(*(s32*)((s32)party + 0xC));
                __memFree(0, party);
                return 0;
        }
        party_kemuri(party);
        screenPos = *(VecLocal*)((s32)party + 0x58);
        marioGetScreenPos(&screenPos, &sx, &sy, &sz);
        if (marioChkInScreen((s32)sx, (s32)sy) != 0) {
            *(u32*)((s32)party + 4) &= ~0x00800000;
        } else {
            *(u32*)((s32)party + 4) |= 0x00800000;
        }
        *(VecLocal*)((s32)party + 0xAC) = *(VecLocal*)&base[0x33];
        partyPreDisp(party);
    }
    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers off

s32 partyDoWork(void) {
    extern f32 float_0_80421218;

    void* mario = marioGetPtr();
    void* party;
    s32 slot = *(s8*)((s32)mario + 0x245);
    f32 speed;

    if (slot >= 0) {
        party = partyPtrTbl[slot];
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
    speed = float_0_80421218;
    mario = marioGetPtr();
    *(f32*)((s32)mario + 0x180) = speed;
    partyChgRunMode(party, 3);
    return 1;
}
void partyHello(void* pParty) {
    extern void partyHelloMain(void* party);
    extern u32 vec3_802cb178[];

    typedef struct VecLocal { f32 x, y, z; } VecLocal;
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
                *(VecLocal*)((s32)pParty + 0x70) = *(VecLocal*)&base[0x36];
                *(u8*)((s32)pParty + 0x38) = 0;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
                *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
                *(VecLocal*)((s32)pParty + 0xB8) = *(VecLocal*)((s32)pParty + 0x58);
            } else if (mode == 7) {
                *(VecLocal*)((s32)pParty + 0x70) = *(VecLocal*)&base[0x39];
                *(u8*)((s32)pParty + 0x38) = 0x14;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
            } else {
                *(VecLocal*)((s32)pParty + 0x70) = *(VecLocal*)&base[0x3C];
                *(u8*)((s32)pParty + 0x38) = 0xA;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
                *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
                *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
                *(VecLocal*)((s32)pParty + 0xB8) = *(VecLocal*)((s32)pParty + 0x58);
            }
        }
        *(u32*)((s32)pParty + 0x0) &= ~0x100;
    }
    partyHelloMain(pParty);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyHelloMain(void* pParty) {
    extern void partyGetAppearPos(void*, void*);
    extern void partyGetAppearPos2(void*, void*);
    extern f32 distABf(f32, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern void partyMoveNoHosei(void*);
    extern f32 partyToMovedir(f32, void*);
    extern void allPartyForceRideOn(void);
    extern u32 psndSFXOn_3D(s32, void*);
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

    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern VecLocal vec3_802cb274[];
    extern VecLocal vec3_802cb280[];
    VecLocal airScale;
    VecLocal landScale;
    u8 result;
    void* player;
    char* pose;
    s32 member;
    s32 poseId;
    s32 weak;
    s32 timer;
    f32 frames;
    f32 delta;
    f32 peak;
    f32 scale;
    f32 originX;
    f32 originZ;

    result = 0;
    player = *(void**)((s32)pParty + 0x160);

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            originX = *(f32*)((s32)player + 0x8C);
            originZ = *(f32*)((s32)player + 0x94);
            *(f32*)((s32)pParty + 0x58) = originX;
            *(f32*)((s32)pParty + 0x5C) =
                float_0p5_80421288 * *(f32*)((s32)player + 0x1BC) +
                *(f32*)((s32)player + 0x90);
            *(f32*)((s32)pParty + 0x60) = originZ;
            partyGetAppearPos(pParty, (void*)((s32)pParty + 0x94));
            *(s32*)((s32)pParty + 0x24) = 16;
            *(f32*)((s32)pParty + 0x128) =
                distABf(originX, originZ,
                        *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) =
                angleABf(originX, originZ,
                         *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / (f32)*(s32*)((s32)pParty + 0x24);
            delta = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
            frames = float_0p5_80421288 * (f32)*(s32*)((s32)pParty + 0x24);
            if (__fabsf(delta) < float_1_80421200) {
                peak = float_10_8042123c;
            } else if (delta >= float_0_80421218) {
                peak = float_0p5_80421288 * *(f32*)((s32)pParty + 0xF0) + delta;
            } else {
                peak = float_10_8042123c;
            }
            delta = (peak + peak) / (frames * frames - frames);
            *(f32*)((s32)pParty + 0x114) = delta * frames;
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
                goto done;
            }
            if (*(void**)((s32)pParty + 0x138) == 0) {
                timer = *(s32*)((s32)pParty + 0x24);
                scale = (f32)(16 - timer) * float_0p0625_8042129c;
                airScale = vec3_802cb274[0];
                airScale.x = scale;
                airScale.y = scale;
                airScale.z = scale;
                *(VecLocal*)((s32)pParty + 0x70) = airScale;
                timer--;
                *(s32*)((s32)pParty + 0x24) = timer;
                if (timer > 0) {
                    goto done;
                }
                *(s32*)((s32)pParty + 0x24) = 0;
                goto done;
            }
            *(u8*)((s32)pParty + 0x38) = 2;
            /* fallthrough */

        case 2:
            timer = *(s32*)((s32)pParty + 0x24);
            scale = (f32)(16 - timer) * float_0p0625_8042129c;
            landScale = vec3_802cb280[0];
            landScale.x = scale;
            landScale.y = scale;
            landScale.z = scale;
            *(VecLocal*)((s32)pParty + 0x70) = landScale;
            timer--;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer >= 1) {
                break;
            }

            *(s32*)((s32)pParty + 0x24) = 0;
            if (*(f32*)((s32)pParty + 0x110) < float_90_80421208 ||
                *(f32*)((s32)pParty + 0x110) > float_270_8042120c) {
                *(f32*)((s32)pParty + 0xFC) = partyToMovedir(float_0_80421218, pParty);
                *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)pParty + 0xFC);
                *(f32*)((s32)pParty + 0x10C) = float_0_80421218;
                *(f32*)((s32)pParty + 0x110) = float_0_80421218;
            } else {
                *(f32*)((s32)pParty + 0xFC) = partyToMovedir(float_180_80421214, pParty);
                *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)pParty + 0xFC);
                *(f32*)((s32)pParty + 0x10C) = float_180_80421214;
                *(f32*)((s32)pParty + 0x110) = float_180_80421214;
            }
            partyClearJumpPara(pParty);

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
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 ||
                    strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }

            *(u32*)((s32)pParty + 0x4) &= ~0x10;
            *(u8*)((s32)pParty + 0x34) = 1;
            psndSFXOn_3D(0x20920, (void*)((s32)player + 0x8C));
            result = 1;
            break;

        case 10:
            partyGetAppearPos2(pParty, (void*)((s32)pParty + 0x58));
            *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0x58);
            *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0x60);

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
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 ||
                    strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }

            *(u8*)((s32)pParty + 0x34) = 1;
            result = 1;
            break;

        case 20:
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
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
                if (*(char**)((s32)pParty + 0x18) == 0 ||
                    strcmp(*(char**)((s32)pParty + 0x18), pose) != 0) {
                    *(char**)((s32)pParty + 0x18) = pose;
                    *(s16*)((s32)pParty + 0x20) = 0;
                    *(u32*)((s32)pParty + 0x8) |= 0x2000;
                }
            }

            *(u8*)((s32)pParty + 0x34) = 0;
            result = 1;
            break;

        case 30:
            allPartyForceRideOn();
            result = 1;
            break;
    }

done:
    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 partyGoodbyeInit(void* party) {
    extern void* party_dp;
    extern void vivianUnhold(void*);
    extern u32 marioGetoffYoshi(void);
    void (*callback)(void);
    void* player;
    s32 member;

    *(u8*)((s32)party + 0x38) = 10;
    member = *(s8*)((s32)party + 0x31);
    party_dp = partyDataTbl + member * 0x28;
    callback = *(void (**)(void))((u8*)party_dp + 4);
    if (callback != 0) {
        callback();
    }
    if ((*(u32*)party & 0x100) != 0) {
        if ((s32)*(u8*)((s32)party + 0x31) == 6) {
            vivianUnhold(party);
            *(u8*)((s32)party + 0x38) = 0;
        }
        if ((s32)*(u8*)((s32)party + 0x31) == 4) {
            marioGetoffYoshi();
            *(u8*)((s32)party + 0x38) = 0;
        }
    }
    player = *(void**)((s32)party + 0x160);
    if (*(u16*)((s32)player + 0x2E) == 0x16 ||
        *(u16*)((s32)player + 0x2E) == 0x18 ||
        *(u16*)((s32)player + 0x2E) == 0x19) {
        *(u8*)((s32)party + 0x38) = 0x14;
    }
    return 0;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyGoodbyeMain(void* pParty) {
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void* party_dp;
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
            if (*(s8*)((s32)pParty + 0x31) == 6) {
                if (vivianGetStatus() == 0) {
                    *(u8*)((s32)pParty + 0x38) = 0xA;
                    *(u8*)((s32)pParty + 0x34) = 8;
                }
            } else if (*(s8*)((s32)pParty + 0x31) == 4) {
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
            *(f32*)((s32)pParty + 0x94) = destX;
            destY = float_0p5_80421288 * *(f32*)((s32)player + 0x1BC) + *(f32*)((s32)player + 0x90);
            *(f32*)((s32)pParty + 0x98) = destY;
            destZ = *(f32*)((s32)player + 0x94);
            *(f32*)((s32)pParty + 0x9C) = destZ;
            posX = *(f32*)((s32)pParty + 0x58);
            posZ = *(f32*)((s32)pParty + 0x60);
            posY = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0x104) = float_4_80421224;
            *(f32*)((s32)pParty + 0x11C) = float_1p6_80421284;
            *(f32*)((s32)pParty + 0x128) = distABf(posX, posZ, destX, destZ);
            *(f32*)((s32)pParty + 0x100) = angleABf(posX, posZ, destX, destZ);
            *(s32*)((s32)pParty + 0x24) = 0xF;
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x114) =
                ((float_0p5_80421288 * ((f32)*(s32*)((s32)pParty + 0x24) * *(f32*)((s32)pParty + 0x11C) * (f32)*(s32*)((s32)pParty + 0x24))) +
                 (destY - posY)) / (f32)*(s32*)((s32)pParty + 0x24);
            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8) {
                    partyGetHp(member);
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][4];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][4];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][4];
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
            *(f32*)((s32)pParty + 0x74) = *(f32*)((s32)pParty + 0x70);
            *(f32*)((s32)pParty + 0x78) = *(f32*)((s32)pParty + 0x70);
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
                    pose = partyPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else if (*(s8*)((s32)pParty + 0x31) != 4) {
                    pose = partyDotPoseTbl[*(s8*)((s32)pParty + 0x31)][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
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
s32 partyDokanInit(void* party) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern f32 revise360(f32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern void animPoseSetMaterialLightFlagOff(s32, s32);
    extern void L_partyVivianTailStop(void*);
    extern VecLocal vec3_802cb178[];

    extern f32 float_180_80421214;
    extern f32 float_30_80421290;
    extern f32 float_26_804212cc;
    extern f32 float_10_8042123c;
    extern f32 float_90_80421208;
    extern f32 float_270_8042120c;
    extern f32 float_neg30_80421260;
    extern f32 float_neg4_804212bc;

    s32 p = (s32)party;
    s32 player = *(s32*)(p + 0x160);
    VecLocal* base = vec3_802cb178;
    s32 dokan;

    *(f32*)(p + 0x110) =
        revise360((f64)(float_180_80421214 + *(f32*)(player + 0x1AC)));
    *(f32*)(p + 0x10C) = *(f32*)(player + 0x1AC);
    *(f32*)(p + 0xFC) = *(f32*)(player + 0x1A0);
    *(VecLocal*)(p + 0x58) = *(VecLocal*)(player + 0x8C);
    *(u32*)(p + 0x4) |= 0x10;

    dokan = *(s8*)(player + 0x3E);

    /*
     * Keep the low side as the fallthrough path.  This mirrors the target
     * compare tree and keeps the physical case blocks below in target order.
     */
    if (dokan == 4) {
        goto case_dokan4;
    }
    if (dokan < 4) {
        if (dokan == 1) {
            goto case_dokan1;
        }
        if (dokan < 1) {
            if (dokan >= 0) {
                goto case_dokan0;
            }
            goto done;
        }
        goto case_dokan23;
    }
    if (dokan >= 6) {
        goto done;
    }
    goto case_dokan5;

case_dokan23:
    {
        s32 paperFlag = 0;

        if (*(s32*)(p + 0x14) > -1) {
            animPaperPoseRelease(*(s32*)(p + 0x14));
            *(s32*)(p + 0x14) = -1;
        }
        *(u32*)(p + 0x4) |= 0x04000000;
        *(s32*)(p + 0x14) =
            animPaperPoseEntry((char*)((u8*)base + 0x1E8), 2);

        if (strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x1F4)) == 0 ||
            strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x1FC)) == 0 ||
            strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x204)) == 0 ||
            strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x1E8)) == 0) {
            paperFlag = 1;
        }
        paperFlag = !paperFlag;
        if (strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x210)) == 0) {
            paperFlag = (*(s8*)(p + 0x31) == 5);
        }

        animPoseSetPaperAnimGroup(
            *(s32*)(p + 0xC), (char*)((u8*)base + 0x1E8), paperFlag);
    }

    if ((*(u32*)(p + 0x4) & 2) == 0 &&
        (*(char**)(p + 0x1C) == 0 ||
         strcmp(*(char**)(p + 0x1C),
                (char*)((u8*)base + 0x218)) != 0)) {
        *(char**)(p + 0x1C) = (char*)((u8*)base + 0x218);
        *(u16*)(p + 0x20) = 0;
        *(u32*)(p + 0x8) |= 0x4000;
    }

    if ((*(u32*)(p + 0x4) & 2) == 0) {
        char* pose;

        if (*(s8*)(p + 0x31) < 8) {
            partyGetHp(*(s8*)(p + 0x31));
        }

        if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[*(s8*)(p + 0x31)][0];
        } else if (*(s8*)(p + 0x31) != 4) {
            pose = partyDotPoseTbl[*(s8*)(p + 0x31)][0];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][0];
        }

        if (*(char**)(p + 0x18) == 0 ||
            strcmp(*(char**)(p + 0x18), pose) != 0) {
            *(char**)(p + 0x18) = pose;
            *(u16*)(p + 0x20) = 0;
            *(u32*)(p + 0x8) |= 0x2000;
        }
    }

    *(f32*)(p + 0x124) = float_30_80421290;
    *(u8*)(p + 0x38) = 0;
    *(u32*)(p + 0x4) |= 0x8;
    *(u32*)(p + 0x4) |= 0x10;
    animPoseSetMaterialLightFlagOff(*(s32*)(p + 0xC), 1);
    goto done;

case_dokan0:
    *(VecLocal*)(p + 0x70) = base[24];
    *(u32*)(p + 0x4) |= 0x8;
    *(u32*)(p + 0x4) |= 0x10;
    animPoseSetMaterialLightFlagOff(*(s32*)(p + 0xC), 1);
    /* Intentional target fallthrough into the full case-1 setup. */

case_dokan1:
    {
        s32 paperFlag = 0;

        if (*(s32*)(p + 0x14) > -1) {
            animPaperPoseRelease(*(s32*)(p + 0x14));
            *(s32*)(p + 0x14) = -1;
        }
        *(u32*)(p + 0x4) |= 0x04000000;
        *(s32*)(p + 0x14) =
            animPaperPoseEntry((char*)((u8*)base + 0x1E8), 2);

        if (strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x1F4)) == 0 ||
            strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x1FC)) == 0 ||
            strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x204)) == 0 ||
            strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x1E8)) == 0) {
            paperFlag = 1;
        }
        paperFlag = !paperFlag;
        if (strcmp((char*)((u8*)base + 0x1E8),
                   (char*)((u8*)base + 0x210)) == 0) {
            paperFlag = (*(s8*)(p + 0x31) == 5);
        }

        animPoseSetPaperAnimGroup(
            *(s32*)(p + 0xC), (char*)((u8*)base + 0x1E8), paperFlag);
    }

    if ((*(u32*)(p + 0x4) & 2) == 0 &&
        (*(char**)(p + 0x1C) == 0 ||
         strcmp(*(char**)(p + 0x1C),
                (char*)((u8*)base + 0x218)) != 0)) {
        *(char**)(p + 0x1C) = (char*)((u8*)base + 0x218);
        *(u16*)(p + 0x20) = 0;
        *(u32*)(p + 0x8) |= 0x4000;
    }

    if ((*(u32*)(p + 0x4) & 2) == 0) {
        char* pose;

        if (*(s8*)(p + 0x31) < 8) {
            partyGetHp(*(s8*)(p + 0x31));
        }

        if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[*(s8*)(p + 0x31)][0];
        } else if (*(s8*)(p + 0x31) != 4) {
            pose = partyDotPoseTbl[*(s8*)(p + 0x31)][0];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][0];
        }

        if (*(char**)(p + 0x18) == 0 ||
            strcmp(*(char**)(p + 0x18), pose) != 0) {
            *(char**)(p + 0x18) = pose;
            *(u16*)(p + 0x20) = 0;
            *(u32*)(p + 0x8) |= 0x2000;
        }
    }

    *(f32*)(p + 0x124) = float_30_80421290;
    *(u8*)(p + 0x38) = 0;
    *(VecLocal*)(p + 0x58) = *(VecLocal*)(player + 0x8C);
    *(VecLocal*)(p + 0x70) = base[25];
    *(u32*)(p + 0x4) |= 0x8;
    *(u32*)(p + 0x4) |= 0x10;
    animPoseSetMaterialLightFlagOff(*(s32*)(p + 0xC), 1);
    goto done;

case_dokan5:
    {
        s32 paperFlag = 0;

        if (*(s32*)(p + 0x14) > -1) {
            animPaperPoseRelease(*(s32*)(p + 0x14));
            *(s32*)(p + 0x14) = -1;
        }
        *(u32*)(p + 0x4) |= 0x04000000;
        *(s32*)(p + 0x14) =
            animPaperPoseEntry((char*)((u8*)base + 0x204), 2);

        if (strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x1F4)) == 0 ||
            strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x1FC)) == 0 ||
            strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x204)) == 0 ||
            strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x1E8)) == 0) {
            paperFlag = 1;
        }
        paperFlag = !paperFlag;
        if (strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x210)) == 0) {
            paperFlag = (*(s8*)(p + 0x31) == 5);
        }

        animPoseSetPaperAnimGroup(
            *(s32*)(p + 0xC), (char*)((u8*)base + 0x204), paperFlag);
    }

    if ((*(u32*)(p + 0x4) & 2) == 0 &&
        (*(char**)(p + 0x1C) == 0 ||
         strcmp(*(char**)(p + 0x1C),
                (char*)((u8*)base + 0x218)) != 0)) {
        *(char**)(p + 0x1C) = (char*)((u8*)base + 0x218);
        *(u16*)(p + 0x20) = 0;
        *(u32*)(p + 0x8) |= 0x4000;
    }

    if ((*(u32*)(p + 0x4) & 2) == 0) {
        char* pose;

        if (*(s8*)(p + 0x31) < 8) {
            partyGetHp(*(s8*)(p + 0x31));
        }

        if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[*(s8*)(p + 0x31)][0];
        } else if (*(s8*)(p + 0x31) != 4) {
            pose = partyDotPoseTbl[*(s8*)(p + 0x31)][0];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][0];
        }

        if (*(char**)(p + 0x18) == 0 ||
            strcmp(*(char**)(p + 0x18), pose) != 0) {
            *(char**)(p + 0x18) = pose;
            *(u16*)(p + 0x20) = 0;
            *(u32*)(p + 0x8) |= 0x2000;
        }
    }

    *(f32*)(p + 0x124) = float_26_804212cc;
    *(u8*)(p + 0x38) = 10;
    *(f32*)(p + 0x8C) = float_10_8042123c;
    *(u32*)(p + 0x4) |= 0x8;
    *(u32*)(p + 0x4) |= 0x10;
    animPoseSetMaterialLightFlagOff(*(s32*)(p + 0xC), 1);
    goto done;

case_dokan4:
    *(f32*)(p + 0x110) = *(f32*)(player + 0x1AC);

    {
        s32 paperFlag = 0;

        if (*(s32*)(p + 0x14) > -1) {
            animPaperPoseRelease(*(s32*)(p + 0x14));
            *(s32*)(p + 0x14) = -1;
        }
        *(u32*)(p + 0x4) |= 0x04000000;
        *(s32*)(p + 0x14) =
            animPaperPoseEntry((char*)((u8*)base + 0x204), 2);

        if (strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x1F4)) == 0 ||
            strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x1FC)) == 0 ||
            strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x204)) == 0 ||
            strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x1E8)) == 0) {
            paperFlag = 1;
        }
        paperFlag = !paperFlag;
        if (strcmp((char*)((u8*)base + 0x204),
                   (char*)((u8*)base + 0x210)) == 0) {
            paperFlag = (*(s8*)(p + 0x31) == 5);
        }

        animPoseSetPaperAnimGroup(
            *(s32*)(p + 0xC), (char*)((u8*)base + 0x204), paperFlag);
    }

    if ((*(u32*)(p + 0x4) & 2) == 0 &&
        (*(char**)(p + 0x1C) == 0 ||
         strcmp(*(char**)(p + 0x1C),
                (char*)((u8*)base + 0x218)) != 0)) {
        *(char**)(p + 0x1C) = (char*)((u8*)base + 0x218);
        *(u16*)(p + 0x20) = 0;
        *(u32*)(p + 0x8) |= 0x4000;
    }

    if ((*(u32*)(p + 0x4) & 2) == 0) {
        char* pose;

        if (*(s8*)(p + 0x31) < 8) {
            partyGetHp(*(s8*)(p + 0x31));
        }

        if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
            pose = partyPoseTbl[*(s8*)(p + 0x31)][0];
        } else if (*(s8*)(p + 0x31) != 4) {
            pose = partyDotPoseTbl[*(s8*)(p + 0x31)][0];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][0];
        }

        if (*(char**)(p + 0x18) == 0 ||
            strcmp(*(char**)(p + 0x18), pose) != 0) {
            *(char**)(p + 0x18) = pose;
            *(u16*)(p + 0x20) = 0;
            *(u32*)(p + 0x8) |= 0x2000;
        }
    }

    *(u8*)(p + 0x38) = 10;
    *(f32*)(p + 0x8C) = float_10_8042123c;

    if (*(f32*)(player + 0x1B0) >= float_90_80421208 &&
        *(f32*)(player + 0x1B0) <= float_270_8042120c) {
        *(f32*)(p + 0x64) =
            revise360((f64)(float_180_80421214 + *(f32*)(player + 0xBC)));
        *(f32*)(p + 0x110) = *(f32*)(player + 0x1B0);
        *(f32*)(p + 0x10C) = *(f32*)(player + 0x1AC);
        *(f32*)(p + 0x124) = float_30_80421290;
    } else {
        *(f32*)(p + 0x64) =
            revise360((f64)(*(f32*)(player + 0xBC) - float_180_80421214));
        *(f32*)(p + 0x110) = *(f32*)(player + 0x1B0);
        *(f32*)(p + 0x10C) = *(f32*)(player + 0x1AC);
        *(u32*)(p + 0x4) |= 0x8;
        *(f32*)(p + 0x124) = float_neg30_80421260;
        *(VecLocal*)(p + 0x58) = *(VecLocal*)(player + 0x8C);
        *(f32*)(p + 0x84) = float_neg4_804212bc;
    }

    *(u32*)(p + 0x4) |= 0x10;
    animPoseSetMaterialLightFlagOff(*(s32*)(p + 0xC), 1);

done:
    L_partyVivianTailStop(party);
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyDokanMain(void* pParty) {
    typedef struct VecLocal { f32 x; f32 y; f32 z; } VecLocal;
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void animPoseSetMaterialLightFlagOff(s32 pose, s32 flag);
    extern void animPoseSetPaperAnimGroup(s32 pose, char* anim, s32 flag);
    extern void animPoseSetMaterialLightFlagOn(s32 pose, s32 flag);
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
    f32 cosVal;
    f32 sinVal;

    data = vec3_802cb178;
    player = *(void**)((s32)pParty + 0x160);

    switch (*(u8*)((s32)pParty + 0x38)) {
        case 0:
            *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
            *(f32*)((s32)pParty + 0x10C) =
                revise360(*(f32*)((s32)pParty + 0x10C) + *(f32*)((s32)pParty + 0x124));
            break;

        case 0xA:
            *(f32*)((s32)pParty + 0x64) += *(f32*)((s32)pParty + 0x124);
            *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
            sincosf(*(f32*)((s32)player + 0x19C), &cosVal, &sinVal);
            *(f32*)((s32)pParty + 0x58) = float_neg5_804212b0 * cosVal + *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)pParty + 0x60) = float_5_804212b4 * sinVal + *(f32*)((s32)player + 0x94);
            break;

        case 0x64:
            dokanVal = *(u8*)((s32)pParty + 0x2E);
            switch (dokanVal) {
                case 0:
                case 1:
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
                    break;
                case 4:
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

                    break;
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
            *(VecLocal*)((s32)pParty + 0x70) = *(VecLocal*)(data + 0x138);
            *(VecLocal*)((s32)pParty + 0x64) = *(VecLocal*)(data + 0x144);
            *(VecLocal*)((s32)pParty + 0x88) = *(VecLocal*)(data + 0x150);
            *(VecLocal*)((s32)pParty + 0x7c) = *(VecLocal*)(data + 0x15c);
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
            *(VecLocal*)((s32)pParty + 0xB8) = *(VecLocal*)((s32)pParty + 0x58);
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

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void partyDokanEnd(int partyId, u8 param_2) {

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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void getRidePos(void* pParty, void* out) {
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 toMovedir2(f32 angle, f32 add);
    extern void sincosf(f32 angle, f32* outSin, f32* outCos);
    extern u32 N_marioShipGetDir(void);
    extern f32 toMovedirSimple(f32 angle);
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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyRideMain(void* pParty) {
    typedef struct VecLocal { f32 x; f32 y; f32 z; } VecLocal;

    extern void getRidePos(void* party, VecLocal* out);
    extern f32 distABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern void movePos(f32 dist, f32 dir, f32* x, f32* z);
    extern f32 revise360(f32 angle);
    extern f32 toMovedir(f32 angle);
    extern f32 toMovedirSimple(f32 angle);
    extern void partyMoveNoHosei(void* party);
    extern void partyGetAppearPos(void* party, VecLocal* out);
    extern void partyGetAppearPos2(void* party, VecLocal* out);
    extern u32 vec3_802cb178[];

    s32 p = (s32)pParty;
    s32 player = *(s32*)(p + 0x160);
    VecLocal ride;
    VecLocal scaleFade;
    VecLocal scaleEnd;
    char* pose;
    s32 member;
    s32 poseId;
    s32 hpWeak;
    s32 timer;
    f32 frame;
    f32 halfFrame;
    f32 dy;
    f32 height;
    f32 accel;
    f32 scale;
    f32 startX;
    f32 startZ;
    u32* vecBase = vec3_802cb178;
    u8 state = *(u8*)(p + 0x38);

    switch (state) {
    case 0:
        getRidePos(pParty, &ride);
        *(VecLocal*)(p + 0x94) = ride;
        *(f32*)(p + 0x128) = (f32)distABf(*(f32*)(p + 0x58), *(f32*)(p + 0x60), *(f32*)(p + 0x94), *(f32*)(p + 0x9C));
        *(f32*)(p + 0x100) = (f32)angleABf(*(f32*)(p + 0x58), *(f32*)(p + 0x60), *(f32*)(p + 0x94), *(f32*)(p + 0x9C));
        *(s32*)(p + 0x24) = 30;
        *(f32*)(p + 0x104) = *(f32*)(p + 0x128) / (f32)*(s32*)(p + 0x24);
        *(f32*)(p + 0x11C) = 0.4f;
        *(f32*)(p + 0x114) = (0.5f * ((f32)*(s32*)(p + 0x24) * (*(f32*)(p + 0x11C) * (f32)*(s32*)(p + 0x24))) + (*(f32*)(p + 0x98) - *(f32*)(p + 0x5C))) / (f32)*(s32*)(p + 0x24);
        *(f32*)(p + 0x124) = 1.0f;
        *(u32*)(p + 0x4) |= 0x10;

        if ((*(u32*)(p + 0x4) & 2) == 0) {
            member = (s8)*(u8*)(p + 0x31);
            if (member < 8) {
                partyGetHp(member);
            }
            if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
                pose = partyPoseTbl[(s8)*(u8*)(p + 0x31)][4];
            } else {
                member = (s8)*(u8*)(p + 0x31);
                if (member != 4) {
                    pose = partyDotPoseTbl[member][4];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][4];
                }
            }
            if (*(char**)(p + 0x18) == 0 || strcmp(*(char**)(p + 0x18), pose) != 0) {
                *(char**)(p + 0x18) = pose;
                *(s16*)(p + 0x20) = 0;
                *(u32*)(p + 0x8) |= 0x2000;
            }
        }
        *(u8*)(p + 0x38) = 1;
        break;
    case 1:
        *(f32*)(p + 0x114) -= *(f32*)(p + 0x11C);
        *(f32*)(p + 0x5C) += *(f32*)(p + 0x114);
        movePos(*(f32*)(p + 0x104), *(f32*)(p + 0x100), (f32*)(p + 0x58), (f32*)(p + 0x60));
        *(f32*)(p + 0x10C) = (f32)revise360(*(f32*)(p + 0x10C) + *(f32*)(p + 0x124));
        *(f32*)(p + 0x124) += 2.0f;
        if (*(f32*)(p + 0x124) >= 30.0f) {
            *(f32*)(p + 0x124) = 30.0f;
        }
        frame = (f32)*(s32*)(p + 0x24);
        if (frame > 10.0f) {
            frame = 10.0f;
        }
        scale = frame / 10.0f;
        *(f32*)(p + 0x70) = scale;
        *(f32*)(p + 0x74) = *(f32*)(p + 0x70);
        *(f32*)(p + 0x78) = *(f32*)(p + 0x70);
        *(s32*)(p + 0x24) -= 1;
        if (*(s32*)(p + 0x24) < 0) {
            *(u8*)(p + 0x38) = 2;
        }
        break;
    case 2:
        poseId = 1;
        if ((*(u32*)(p + 0x4) & 2) == 0) {
            hpWeak = 0;
            member = (s8)*(u8*)(p + 0x31);
            if (member < 8 && partyGetHp(member) < 6) {
                hpWeak = 1;
            }
            if (hpWeak == 1) {
                poseId = 12;
            }
            if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
                pose = partyPoseTbl[(s8)*(u8*)(p + 0x31)][poseId];
            } else {
                member = (s8)*(u8*)(p + 0x31);
                if (member != 4) {
                    pose = partyDotPoseTbl[member][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
            }
            if (*(char**)(p + 0x18) == 0 || strcmp(*(char**)(p + 0x18), pose) != 0) {
                *(char**)(p + 0x18) = pose;
                *(s16*)(p + 0x20) = 0;
                *(u32*)(p + 0x8) |= 0x2000;
            }
        }
        *(f32*)(p + 0x114) = 0.0f;
        *(f32*)(p + 0x5C) = *(f32*)(p + 0x98);
        *(VecLocal*)(p + 0x70) = *(VecLocal*)&vecBase[90];
        *(u32*)(p + 0x4) |= 1;
        if (pParty != 0) {
            *(u32*)(p + 0x4) |= 0x100000;
        }
        *(u8*)(p + 0x38) = 10;
        break;
    case 10:
        *(VecLocal*)(p + 0x58) = *(VecLocal*)(player + 0x8C);
        *(f32*)(p + 0x110) = *(f32*)(player + 0x1B0);
        *(f32*)(p + 0xFC) = (f32)toMovedir(*(f32*)(p + 0x110));
        break;
    case 11:
        poseId = 1;
        if ((*(u32*)(p + 0x4) & 2) == 0) {
            hpWeak = 0;
            member = (s8)*(u8*)(p + 0x31);
            if (member < 8 && partyGetHp(member) < 6) {
                hpWeak = 1;
            }
            if (hpWeak == 1) {
                poseId = 12;
            }
            if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
                pose = partyPoseTbl[(s8)*(u8*)(p + 0x31)][poseId];
            } else {
                member = (s8)*(u8*)(p + 0x31);
                if (member != 4) {
                    pose = partyDotPoseTbl[member][poseId];
                } else {
                    pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                }
            }
            if (*(char**)(p + 0x18) == 0 || strcmp(*(char**)(p + 0x18), pose) != 0) {
                *(char**)(p + 0x18) = pose;
                *(s16*)(p + 0x20) = 0;
                *(u32*)(p + 0x8) |= 0x2000;
            }
        }
        *(u32*)(p + 0x4) |= 1;
        if (pParty != 0) {
            *(u32*)(p + 0x4) |= 0x100000;
        }
        getRidePos(pParty, &ride);
        *(VecLocal*)(p + 0x70) = *(VecLocal*)&vecBase[93];
        *(VecLocal*)(p + 0x58) = ride;
        *(f32*)(p + 0x114) = 0.0f;
        *(f32*)(p + 0x110) = *(f32*)(*(s32*)(p + 0x160) + 0x1B0);
        *(f32*)(p + 0xFC) = (f32)toMovedir(*(f32*)(p + 0x110));
        break;
    case 20:
        *(u8*)(p + 0x38) = 21;
        *(s32*)(p + 0x24) = 16;
        *(f32*)(p + 0x114) = 16.0f;
        *(f32*)(p + 0x174) = 0.5f;
    case 21:
        *(f32*)(p + 0x5C) += *(f32*)(p + 0x114);
        *(f32*)(p + 0x114) += *(f32*)(p + 0x174);
        *(s32*)(p + 0x24) -= 1;
        if (*(s32*)(p + 0x24) < 1) {
            *(u8*)(p + 0x38) = 22;
            *(u32*)pParty |= 0x02000000;
        }
        break;
    case 200:
        *(u32*)pParty &= ~0x70000;
        *(f32*)(p + 0x80) = 0.0f;
        *(f32*)(p + 0x70) = 1.0f;
        *(f32*)(p + 0x74) = *(f32*)(p + 0x70);
        *(f32*)(p + 0x78) = *(f32*)(p + 0x70);
        if (pParty != 0) {
            *(u32*)(p + 0x4) &= ~0x100000;
        }
        partyChgRunMode(pParty, 1);
        break;
    case 100:
    case 101:
        *(u32*)(p + 0x4) &= ~1U;
        startX = *(f32*)(player + 0x8C);
        startZ = *(f32*)(player + 0x94);
        *(f32*)(p + 0x58) = startX;
        *(f32*)(p + 0x5C) = *(f32*)(player + 0x90) + 0.5f * *(f32*)(player + 0x1BC);
        *(f32*)(p + 0x60) = startZ;
        if (*(u8*)(p + 0x38) == 100) {
            partyGetAppearPos(pParty, (VecLocal*)(p + 0x94));
        } else {
            partyGetAppearPos2(pParty, (VecLocal*)(p + 0x94));
        }
        *(f32*)(p + 0x110) = *(f32*)(player + 0x1B0);
        *(s32*)(p + 0x24) = 16;
        *(f32*)(p + 0x128) = distABf(startX, startZ, *(f32*)(p + 0x94), *(f32*)(p + 0x9C));
        *(f32*)(p + 0x100) = angleABf(startX, startZ, *(f32*)(p + 0x94), *(f32*)(p + 0x9C));
        frame = (f32)*(s32*)(p + 0x24);
        *(f32*)(p + 0x104) = *(f32*)(p + 0x128) / frame;
        dy = *(f32*)(p + 0x98) - *(f32*)(p + 0x5C);
        halfFrame = 0.5f * (f32)*(s32*)(p + 0x24);
        height = 10.0f;
        if (__fabsf(dy) >= 1.0f) {
            if (dy >= 0.0f) {
                height = 0.5f * *(f32*)(p + 0xF0) + dy;
            } else {
                height = 10.0f;
            }
        }
        accel = (height + height) / (halfFrame * halfFrame - halfFrame);
        *(f32*)(p + 0x114) = accel * halfFrame;
        *(f32*)(p + 0x118) = -accel;
        *(u32*)pParty |= 0x10;
        *(u32*)(p + 0x4) &= ~1U;
        if (pParty != 0) {
            *(u32*)(p + 0x4) &= ~0x100000;
        }
        *(u8*)(p + 0x38) = 110;
        break;
    case 110:
        partyMoveNoHosei(pParty);
        if (*(f32*)(p + 0x114) <= 0.0f) {
            *(u32*)pParty &= ~0x10;
            *(f32*)(p + 0x118) = -1.6f;
        }
        if (*(f32*)(p + 0x5C) < -2000.0f) {
            *(s32*)(p + 0x24) = 0;
            *(u8*)(p + 0x38) = 111;
        } else if (*(void**)(p + 0x138) == 0) {
            scaleFade = *(VecLocal*)&vecBase[96];
            scale = (f32)(16 - *(s32*)(p + 0x24)) * 0.0625f;
            scaleFade.x = scale;
            scaleFade.y = scale;
            scaleFade.z = scale;
            *(VecLocal*)(p + 0x70) = scaleFade;
            *(s32*)(p + 0x24) -= 1;
            if (*(s32*)(p + 0x24) < 1) {
                *(s32*)(p + 0x24) = 0;
            }
        } else {
            *(u8*)(p + 0x38) = 111;
        }
        break;
    case 111:
        scaleEnd = *(VecLocal*)&vecBase[99];
        scale = (f32)(16 - *(s32*)(p + 0x24)) * 0.0625f;
        scaleEnd.x = scale;
        scaleEnd.y = scale;
        scaleEnd.z = scale;
        *(VecLocal*)(p + 0x70) = scaleEnd;
        *(s32*)(p + 0x24) -= 1;
        if (*(s32*)(p + 0x24) < 1) {
            *(s32*)(p + 0x24) = 0;
            *(f32*)(p + 0x110) = *(f32*)(player + 0x1B0);
            *(f32*)(p + 0x10C) = *(f32*)(player + 0x1B0);
            *(f32*)(p + 0xFC) = (f32)toMovedirSimple(*(f32*)(p + 0x10C));
            *(f32*)(p + 0x100) = *(f32*)(p + 0xFC);
            partyClearJumpPara(pParty);

            poseId = 1;
            if ((*(u32*)(p + 0x4) & 2) == 0) {
                hpWeak = 0;
                member = (s8)*(u8*)(p + 0x31);
                if (member < 8 && partyGetHp(member) < 6) {
                    hpWeak = 1;
                }
                if (hpWeak == 1) {
                    poseId = 12;
                }
                if ((*(u32*)(p + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[(s8)*(u8*)(p + 0x31)][poseId];
                } else {
                    member = (s8)*(u8*)(p + 0x31);
                    if (member != 4) {
                        pose = partyDotPoseTbl[member][poseId];
                    } else {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                    }
                }
                if (*(char**)(p + 0x18) == 0 || strcmp(*(char**)(p + 0x18), pose) != 0) {
                    *(char**)(p + 0x18) = pose;
                    *(s16*)(p + 0x20) = 0;
                    *(u32*)(p + 0x8) |= 0x2000;
                }
            }
            *(u32*)(p + 0x4) &= ~0x10U;
            *(u32*)pParty &= ~0x70000;
            *(f32*)(p + 0x80) = 0.0f;
            *(f32*)(p + 0x70) = 1.0f;
            *(f32*)(p + 0x74) = *(f32*)(p + 0x70);
            *(f32*)(p + 0x78) = *(f32*)(p + 0x70);
            *(u32*)(p + 0x4) &= ~1U;
            if (pParty != 0) {
                *(u32*)(p + 0x4) &= ~0x100000;
            }
            partyChgRunMode(pParty, 1);
        }
        break;
    }

    *(VecLocal*)(p + 0xB8) = *(VecLocal*)(p + 0x58);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_force_ride_yoshi(void* pParty) {
    extern void N_partyForceChgRunMode(void* party, s32 newRunMode, s32 subMotionId);
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


void party_ride_yoshi_force_move(void) {
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
s32 partyRideYoshiMain(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern u8 partyGetAppearPos(void* party, s32 out);
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
            *(VecLocal*)((s32)pParty + 0x94) = ridePos;
            *(f32*)((s32)pParty + 0x128) = distABf(*(f32*)((s32)pParty + 0x58),
                                                    *(f32*)((s32)pParty + 0x60),
                                                    *(f32*)((s32)pParty + 0x94),
                                                    *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) = angleABf(*(f32*)((s32)pParty + 0x58),
                                                     *(f32*)((s32)pParty + 0x60),
                                                     *(f32*)((s32)pParty + 0x94),
                                                     *(f32*)((s32)pParty + 0x9C));
            *(s32*)((s32)pParty + 0x24) = 0xF;
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x114) =
                ((float_0p5_80421288 * ((f32)*(s32*)((s32)pParty + 0x24) *
                  (*(f32*)((s32)pParty + 0x11C) * (f32)*(s32*)((s32)pParty + 0x24)))) +
                 (*(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C))) / (f32)*(s32*)((s32)pParty + 0x24);

            if ((*(u32*)((s32)pParty + 0x4) & 2) == 0) {
                member = (s8)*(u8*)((s32)pParty + 0x31);
                if (member < 8) {
                    partyGetHp(member);
                }
                if ((*(u32*)((s32)pParty + 0x4) & 0x8000) == 0) {
                    pose = partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][4];
                } else {
                    member = (s8)*(u8*)((s32)pParty + 0x31);
                    if (member != 4) {
                        pose = partyDotPoseTbl[member][4];
                    } else {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][4];
                    }
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
            *(VecLocal*)((s32)pParty + 0x94) = ridePos;
            *(f32*)((s32)pParty + 0x128) = distABf(*(f32*)((s32)pParty + 0x58),
                                                    *(f32*)((s32)pParty + 0x60),
                                                    *(f32*)((s32)pParty + 0x94),
                                                    *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) = angleABf(*(f32*)((s32)pParty + 0x58),
                                                     *(f32*)((s32)pParty + 0x60),
                                                     *(f32*)((s32)pParty + 0x94),
                                                     *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)pParty + 0x128) / (f32)*(s32*)((s32)pParty + 0x24);
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
                    pose = partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][poseId];
                } else {
                    member = (s8)*(u8*)((s32)pParty + 0x31);
                    if (member != 4) {
                        pose = partyDotPoseTbl[member][poseId];
                    } else {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][poseId];
                    }
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
            *(VecLocal*)((s32)pParty + 0x58) = ridePos;
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
                *(VecLocal*)((s32)pParty + 0x7C) = *(VecLocal*)vec3_802cb328;
            }
            break;
    }

    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 party_damage_return(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 strcmp(char* a, char* b);
    extern s32 sysMsec2Frame(s32 msec);
    extern void marioGetScreenPos(VecLocal* pos, f32* x, f32* y, f32* z);
    extern s32 marioWaitDamageReturnFall(void);
    extern s32 marioWaitDamageTogeReturnFall(void);
    extern u8 partyGetAppearPos(void* party, s32 out);
    extern void partyMove(void* party);
    extern u32 hitGetAttr(void* hit);
    extern void vivianShadow(void* party);
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
    VecLocal appearPos;
    VecLocal pos;
    VecLocal marioPos;
    VecLocal pos20;
    VecLocal pos22;
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
                    pose = partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][8];
                } else {
                    member = (s8)*(u8*)((s32)pParty + 0x31);
                    if (member != 4) {
                        pose = partyDotPoseTbl[member][8];
                    } else {
                        pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][8];
                    }
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
            pos = *(VecLocal*)((s32)pParty + 0x58);
            marioGetScreenPos(&pos, &sx, &sy, &sz);
            marioPos = *(VecLocal*)((s32)player + 0x8C);
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
            pos20 = *(VecLocal*)((s32)pParty + 0x58);
            marioGetScreenPos(&pos20, &sx, &sy, &sz);
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
            pos22 = *(VecLocal*)((s32)pParty + 0x58);
            marioGetScreenPos(&pos22, &sx, &sy, &sz);
            if (sy > float_240_80421274) {
                *(u32*)((s32)pParty + 0x0) &= ~0x02000000;
            }
            partyMove(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_neg6_80421278) {
                *(f32*)((s32)pParty + 0x118) = float_0_80421218;
                *(f32*)((s32)pParty + 0x114) = float_neg6_80421278;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_8042127c) {
                *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
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
                        pose = partyPoseTbl[(s8)*(u8*)((s32)pParty + 0x31)][9];
                    } else {
                        member = (s8)*(u8*)((s32)pParty + 0x31);
                        if (member != 4) {
                            pose = partyDotPoseTbl[member][9];
                        } else {
                            pose = yoshiDotPoseTbl[pouchGetPartyColor(4)][9];
                        }
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


void partyInitCamId(void* party) {
    *(s32*)((s32)party + 0x164) = marioGetCamId();
}

void partySetCamId(void* party, s32 camId) {
    *(s32*)((s32)party + 0x164) = camId;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 partyMakeDispDir(void* pParty) {
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
        projPos = *(VecLocal*)(pParty + 0x58);
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
            *(VecLocal*)(pParty + 0xB8) = *(VecLocal*)(pParty + 0x58);
        } else {
            if (*(u8*)(pParty + 0x34) == 4 ||
                (*(u8*)(pParty + 0x34) == 8 && (*(u32*)(pParty + 0x0) & 0x100) == 0)) {
                *(VecLocal*)(pParty + 0xB8) = *(VecLocal*)(pParty + 0x58);
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
            hitPos = *(VecLocal*)(pParty + 0x58);
            hitPos.y += float_10_8042123c;
            delta = *(VecLocal*)vec3_802cb334;
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
u8 partyDisp(s32 param_1, void* pParty) {
    typedef f32 Mtx34[3][4];
    extern void PSMTXScale(Mtx34 mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(Mtx34 mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx34 mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(Mtx34 a, Mtx34 b, Mtx34 out);
    extern void animPoseDrawMtx(s32 poseId, Mtx34 mtx, s32 mode, f32 rot, f32 scale);
    extern void animPoseSetLocalTimeRate(f32 rate, s32 poseId);
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
    s32 flipped;

    activeCount = 0;
    party = partyGetPtr(0);
    if (party != 0 && (*(u32*)((s32)party + 0x0) & 1) != 0) {
        activeCount = 1;
    }
    party = partyGetPtr(1);
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
            flipped = 0;
            if (angle >= float_90_80421208 && angle <= float_270_8042120c) {
                flipped = 1;
            }
            if (flipped) scale = float_neg1_80421204;
            else scale = float_1_80421200;
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
                PSMTXRotRad(workMtx, 0x79, float_deg2rad_80421210 * scale);
                PSMTXConcat(workMtx, scaleMtx, scaleMtx);
            } else {
                if ((*(u32*)((s32)pParty + 0x4) & 0x40) != 0) {
                    scale = float_0_80421218;
                } else {
                    scale = float_180_80421214;
                }
                PSMTXRotRad(workMtx, 0x79, float_deg2rad_80421210 * scale);
                PSMTXConcat(workMtx, scaleMtx, scaleMtx);
            }
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
            } else {
                yOffset = float_0_80421218;
            }
        }

        PSMTXTrans(workMtx,
                   *(f32*)((s32)pParty + 0x58) + *(f32*)((s32)pParty + 0x7C),
                   yOffset + *(f32*)((s32)pParty + 0x5C) + *(f32*)((s32)pParty + 0x80),
                   *(f32*)((s32)pParty + 0x60) + *(f32*)((s32)pParty + 0x84));
        PSMTXConcat(workMtx, scaleMtx, scaleMtx);

        animPoseDrawMtx(*(s32*)((s32)pParty + 0xC), scaleMtx, 2, float_0_80421218, float_1_80421200);
        animPoseDrawMtx(*(s32*)((s32)pParty + 0xC), scaleMtx, 1, float_0_80421218, float_1_80421200);

        if ((*(u32*)((s32)pParty + 0x0) & 0x80) != 0) {
            animPoseSetLocalTimeRate(float_0p75_80421228, *(s32*)((s32)pParty + 0xC));
        } else {
            animPoseSetLocalTimeRate(float_1_80421200, *(s32*)((s32)pParty + 0xC));
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void partyKeyOn(void) {
    ;
}

void partyKeyOff(void) {
    ;
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

void partyUpdateKeyData(void* party) {
    typedef struct PartyPadGlobalsLocal {
        u8 pad_0000[0x1328];
        u32 buttons[4];
        u32 buttonsPressed[4];
        u32 buttonsRep[4];
        u32 buttonsHeld[4];
        u32 buttonsReleased[4];
        u8 pad_1378[0x40];
        u8 stickX[4];
        u8 stickY[4];
        u8 subStickX[4];
        u8 subStickY[4];
        u8 triggerLeft[4];
        u8 triggerRight[4];
    } PartyPadGlobalsLocal;

    extern void* gp;
    s32 port = *(s8*)((s32)party + 0x30);

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

    *(u8*)((s32)party + 0x158) =
        ((PartyPadGlobalsLocal*)gp)->stickX[port];
    *(u8*)((s32)party + 0x159) =
        -((PartyPadGlobalsLocal*)gp)->stickY[port];
    *(u8*)((s32)party + 0x15A) =
        ((PartyPadGlobalsLocal*)gp)->subStickX[port];
    *(u8*)((s32)party + 0x15B) =
        -((PartyPadGlobalsLocal*)gp)->subStickY[port];
    *(u8*)((s32)party + 0x15C) =
        ((PartyPadGlobalsLocal*)gp)->triggerLeft[port];
    *(u8*)((s32)party + 0x15D) =
        ((PartyPadGlobalsLocal*)gp)->triggerRight[port];
    *(u8*)((s32)party + 0x15E) = 0;
    *(u8*)((s32)party + 0x15F) = 0;

    *(u16*)((s32)party + 0x150) =
        ((PartyPadGlobalsLocal*)gp)->buttons[port];
    *(u16*)((s32)party + 0x152) =
        ((PartyPadGlobalsLocal*)gp)->buttonsPressed[port];
    *(u16*)((s32)party + 0x154) =
        ((PartyPadGlobalsLocal*)gp)->buttonsRep[port];
    *(u16*)((s32)party + 0x156) =
        ((PartyPadGlobalsLocal*)gp)->buttonsReleased[port];
}

void partyShadowOff(void* party) {
    if (party == 0) {
        return;
    }

    *(u32*)((s32)party + 0x4) |= 0x100000;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partySetFamicomMode(void* pParty, int mode) {
    extern void L_partyForceSlitOff(void* party);
    extern s32 strcmp(char* a, char* b);
    extern char** party_dp;
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

const f32 float_neg6p8_804211f0 = -6.8f;
const f32 float_6p8_804211f4 = 6.8f;
const f32 float_1p2_804211f8 = 1.2f;
const f32 float_2_804211fc = 2.0f;
const f32 float_1_80421200 = 1.0f;
const f32 float_neg1_80421204 = -1.0f;
const f32 float_90_80421208 = 90.0f;
const f32 float_270_8042120c = 270.0f;
const f32 float_deg2rad_80421210 = 0.017453292f;
const f32 float_180_80421214 = 180.0f;
const f32 float_0_80421218 = 0.0f;
const f32 float_3_8042121c = 3.0f;
const f32 float_3p5_80421220 = 3.5f;
const f32 float_4_80421224 = 4.0f;
const f32 float_0p75_80421228 = 0.75f;
const f32 float_5000_8042122c = 5000.0f;
const f32 float_10000_80421230 = 10000.0f;
const f32 float_13p333_80421234 = 13.333333f;
const f32 float_1000_80421238 = 1000.0f;
const f32 float_10_8042123c = 10.0f;
const f32 float_290_80421240 = 290.0f;
const f32 float_250_80421244 = 250.0f;
const f32 float_190_80421248 = 190.0f;
const f32 float_350_8042124c = 350.0f;
const f32 float_170_80421250 = 170.0f;
const f32 float_0p125_80421254 = 0.125f;
const f32 float_20_80421258 = 20.0f;
const f32 float_neg40_8042125c = -40.0f;
const f32 float_neg30_80421260 = -30.0f;
const f32 float_220_80421264 = 220.0f;
const f32 float_210_80421268 = 210.0f;
const f32 float_15_8042126c = 15.0f;
const f32 float_neg100_80421270 = -100.0f;
const f32 float_240_80421274 = 240.0f;
const f32 float_neg6_80421278 = -6.0f;
const f32 float_neg2000_8042127c = -2000.0f;
const f32 float_200_80421280 = 200.0f;
const f32 float_1p6_80421284 = 1.6f;
const f32 float_0p5_80421288 = 0.5f;
const f32 float_0p4_8042128c = 0.4f;
const f32 float_30_80421290 = 30.0f;
const f32 float_16_80421294 = 16.0f;
const f32 float_neg1p6_80421298 = -1.6f;
const f32 float_0p0625_8042129c = 0.0625f;
const f32 float_3p1416_804212a0 = 3.1415927f;
const f32 float_12_804212a4 = 12.0f;
const f32 float_22_804212a8 = 22.0f;
const f32 float_40_804212ac = 40.0f;
const f32 float_neg5_804212b0 = -5.0f;
const f32 float_5_804212b4 = 5.0f;
const f32 float_0p2_804212b8 = 0.2f;
const f32 float_neg4_804212bc = -4.0f;
const f32 float_1p5_804212c0 = 1.5f;
const f32 float_neg3_804212c4 = -3.0f;
const f32 float_neg1p5_804212c8 = -1.5f;
const f32 float_26_804212cc = 26.0f;
const f32 float_50_804212d0 = 50.0f;
const f32 float_18p5_804212d4 = 18.5f;

const char str_c_babyyoshi_802cb1a8[] = "c_babyyoshi";
const char str_c_babyyoshi2_802cb1b4[] = "c_babyyoshi2";
const char str_c_babyyoshi3_802cb1c4[] = "c_babyyoshi3";
const char str_c_babyyoshi4_802cb1d4[] = "c_babyyoshi4";
const char str_c_babyyoshi5_802cb1e4[] = "c_babyyoshi5";
const char str_c_babyyoshi6_802cb1f4[] = "c_babyyoshi6";
const char str_c_babyyoshi7_802cb204[] = "c_babyyoshi7";
const char str_d_mario_802cb340[] = "d_mario";
const char str_PM_D_1B_802cb358[] = "PM_D_1B";
const char str_p_dokan_y_802cb360[] = "p_dokan_y";
const char str_p_roll_802cb36c[] = "p_roll";
const char str_p_plane_802cb374[] = "p_plane";
const char str_p_dokan_x_802cb37c[] = "p_dokan_x";
const char str_p_slit_802cb388[] = "p_slit";
const char str_PM_D_1A_802cb390[] = "PM_D_1A";

char* yoshigroup[] = {
    (char*)str_c_babyyoshi_802cb1a8,
    (char*)str_c_babyyoshi2_802cb1b4,
    (char*)str_c_babyyoshi3_802cb1c4,
    (char*)str_c_babyyoshi4_802cb1d4,
    (char*)str_c_babyyoshi5_802cb1e4,
    (char*)str_c_babyyoshi6_802cb1f4,
    (char*)str_c_babyyoshi7_802cb204,
};

void* party_dp;
s32 partyCtrlNo;
