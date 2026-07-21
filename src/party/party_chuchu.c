#include "party/party_chuchu.h"

extern f32 float_20_80424374;
extern f32 float_24_80424378;
extern f32 float_0_80424370;
extern f32 float_18_80424388;
extern char str_PCH_Y_1_802f8ca0[];

s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 search_result(void* pEvt, int param_2) {
    extern s32 sysMsec2Frame(s32 msec);
    extern void effFukidashiEntry(s32 type, s32 a, s32 b, s32 frames, f32 x, f32 y, f32 z, f32 w, f32 v);
    extern void psndSFXOn_3D(s32 soundId, void* pos);
    extern void partyChgPose(void* party, void* pose);
    extern void partyChkGnd(void* party);
    void* party;
    s32 frames;

    party = partyGetPtr(marioGetPartyId());
    if (param_2 != 0) {
        if (*(u32*)((s32)pEvt + 0x9C) == 0 || *(s32*)((s32)pEvt + 0xA0) == 0) {
            return 2;
        }
        if (*(s32*)((s32)pEvt + 0xA0) == 1) {
            if (party != 0) {
                frames = sysMsec2Frame(0x5DC);
                effFukidashiEntry(6, 0, -2, frames, float_0_80424370, float_18_80424388,
                                  float_0_80424370, float_0_80424370, float_0_80424370);
            }
            return 2;
        }
        psndSFXOn_3D(0x945, (void*)((s32)party + 0x58));
        *(s32*)((s32)pEvt + 0x84) = 0x2A;
        partyChgPose(party, str_PCH_Y_1_802f8ca0);
    }
    partyChkGnd(party);
    if (--(*(s32*)((s32)pEvt + 0x84)) > 0) {
        return 0;
    }
    if (party != 0) {
        frames = sysMsec2Frame(0x5DC);
        effFukidashiEntry(6, 0, -2, frames, float_0_80424370, float_18_80424388,
                          float_0_80424370, float_0_80424370, float_0_80424370);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 set_msg(void* pEvt) {
    extern void* gp;
    extern u8 mobj_list[];
    extern u32 unk_8038e994[];
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* itemNearDistCheck(f32 x, f32 y, f32 z, f32 dist);
    extern void* mobjNearDistCheck2(void* list, f32 x, f32 y, f32 z, f32 dist);
    extern s32 strcmp(char* a, char* b);
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 PSVECDistance(f32* a, f32* b);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 revise360(f32 angle);
    extern f32 partyToMovedir(f32 angle, void* party);
    extern char str_MOBJ_TreasureBox_802f8a08[];
    extern f32 float_0_80424370;
    extern f32 float_150_8042437c;
    extern f32 float_10000_80424380;
    extern f32 float_180_80424384;

    void* party;
    void* mario;
    void* item;
    void* mobj;
    u32* entry;
    f32 range;
    f32 best;
    f32 angle;
    f32 dist;
    f32 objPos[3];
    f32 bestMapPos[3];
    f32 itemPos[3];
    f32 mobjPos[3];
    s32 foundType;
    s32 foundMap;
    s32 pass;
    s32 i;

    party = partyGetPtr(marioGetPartyId());
    *(s32*)((s32)pEvt + 0x9C) = 0;
    *(s32*)((s32)pEvt + 0xA0) = 0;
    if (party == 0) {
        return 2;
    }

    mario = *(void**)((s32)party + 0x160);
    foundType = -1;
    angle = float_0_80424370;

    for (pass = 0; pass < 2 && foundType < 0; pass++) {
        if (pass == 0) {
            range = float_150_8042437c;
        } else {
            range = float_10000_80424380;
        }

        item = itemNearDistCheck(*(f32*)((s32)mario + 0x8C),
                                 *(f32*)((s32)mario + 0x90),
                                 *(f32*)((s32)mario + 0x94),
                                 range);
        mobj = mobjNearDistCheck2(mobj_list,
                                  *(f32*)((s32)mario + 0x8C),
                                  *(f32*)((s32)mario + 0x90),
                                  *(f32*)((s32)mario + 0x94),
                                  range);

        foundMap = 0;
        best = float_10000_80424380;
        entry = unk_8038e994;
        for (i = 0; i < 0x28; i++, entry += 5) {
            if (strcmp((char*)gp + 0x12C, (char*)entry[0]) == 0 && evtGetValue(0, entry[1]) == 0) {
                objPos[0] = (f32)(s32)entry[2];
                objPos[1] = (f32)(s32)entry[3];
                objPos[2] = (f32)(s32)entry[4];
                dist = PSVECDistance((f32*)((s32)mario + 0x8C), objPos);
                if (dist <= range && dist < best) {
                    best = dist;
                    foundMap = 1;
                    bestMapPos[0] = objPos[0];
                    bestMapPos[1] = objPos[1];
                    bestMapPos[2] = objPos[2];
                }
            }
        }

        best = float_10000_80424380;
        if (item != 0) {
            itemPos[0] = *(f32*)((s32)item + 0x3C);
            itemPos[1] = *(f32*)((s32)item + 0x40);
            itemPos[2] = *(f32*)((s32)item + 0x44);
            dist = PSVECDistance(itemPos, (f32*)((s32)mario + 0x8C));
            if (dist < best) {
                best = dist;
                angle = revise360(angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                                           itemPos[0], itemPos[2]) - *(f32*)((s32)party + 0xF8));
                foundType = 0;
            }
        }

        if (mobj != 0) {
            mobjPos[0] = *(f32*)((s32)mobj + 0x3C);
            mobjPos[1] = *(f32*)((s32)mobj + 0x40);
            mobjPos[2] = *(f32*)((s32)mobj + 0x44);
            dist = PSVECDistance(mobjPos, (f32*)((s32)mario + 0x8C));
            if (dist < best) {
                best = dist;
                angle = revise360(angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                                           mobjPos[0], mobjPos[2]) - *(f32*)((s32)party + 0xF8));
                foundType = 1;
            }
        }

        if (foundMap != 0) {
            dist = PSVECDistance(bestMapPos, (f32*)((s32)mario + 0x8C));
            if (dist < best) {
                angle = revise360(angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                                           bestMapPos[0], bestMapPos[2]) - *(f32*)((s32)party + 0xF8));
                foundType = 2;
            }
        }
    }

    if (foundType < 0) {
        *(char**)((s32)pEvt + 0x9C) = str_MOBJ_TreasureBox_802f8a08 + 0x288;
        *(s32*)((s32)pEvt + 0xA0) = 0;
    } else {
        if (range == float_150_8042437c) {
            *(char**)((s32)pEvt + 0x9C) = str_MOBJ_TreasureBox_802f8a08 + 0x268;
            *(s32*)((s32)pEvt + 0xA0) = 2;
        } else {
            *(char**)((s32)pEvt + 0x9C) = str_MOBJ_TreasureBox_802f8a08 + 0x278;
            *(s32*)((s32)pEvt + 0xA0) = 1;
        }
        if (angle <= float_180_80424384) {
            *(f32*)((s32)party + 0x110) = float_180_80424384;
        } else {
            *(f32*)((s32)party + 0x110) = float_0_80424370;
        }
        *(f32*)((s32)party + 0xFC) = partyToMovedir(*(f32*)((s32)party + 0x110), party);
        *(f32*)((s32)party + 0x100) = *(f32*)((s32)party + 0xFC);
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 post_msg(void* party) {
    void* current;

    current = partyGetPtr(marioGetPartyId());
    *(s32*)((s32)party + 0x9C) = 0;
    if (current != 0) {
        (*(u8*)((s32)current + 0x39))++;
    }
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 chuchu_searchObject(void) {
    extern void* gp;
    extern u8 mobj_list[];
    extern s32 itemNearDistCheck(f32 x, f32 y, f32 z, f32 dist);
    extern s32 mobjNearDistCheck2(void* list, f32 x, f32 y, f32 z, f32 dist);
    extern s32 strcmp(char* a, char* b);
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 PSVECDistance(f32* a, f32* b);
    extern u32 unk_8038e994[];
    extern f32 vec3_802f8c4c[3];
    extern f32 float_150_8042437c;
    extern f32 float_10000_80424380;
    void* party;
    void* mario;
    u32* entry;
    f32 best;
    f32 pos[3];
    f32 obj[3];
    f32 dist;
    s32 found;
    s32 i;

    party = partyGetPtr(marioGetPartyId());
    mario = *(void**)((s32)party + 0x160);
    if (itemNearDistCheck(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90), *(f32*)((s32)mario + 0x94), float_150_8042437c) != 0) {
        return 1;
    }
    if (mobjNearDistCheck2(mobj_list, *(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90), *(f32*)((s32)mario + 0x94), float_150_8042437c) != 0) {
        return 1;
    }

    entry = unk_8038e994;
    found = 0;
    best = float_10000_80424380;
    for (i = 0; i < 0x28; i++, entry += 5) {
        if (strcmp((char*)gp + 0x12C, (char*)entry[0]) == 0 && evtGetValue(0, entry[1]) == 0) {
            obj[0] = (f32)(s32)entry[2];
            obj[1] = (f32)(s32)entry[3];
            obj[2] = (f32)(s32)entry[4];
            pos[0] = vec3_802f8c4c[0];
            pos[1] = vec3_802f8c4c[1];
            pos[2] = vec3_802f8c4c[2];
            pos[0] = obj[0];
            pos[1] = obj[1];
            pos[2] = obj[2];
            dist = PSVECDistance((f32*)((s32)mario + 0x8C), pos);
            if (dist <= float_150_8042437c && dist < best) {
                best = dist;
                found = 1;
            }
        }
    }
    return found != 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void chuchu_init(void* party) {
    *(u8*)((s32)party + 0x33) = 7;
    {
        f32 f24;
        f32 f20 = float_20_80424374;
        *(u8*)((s32)party + 0x32) = 0;
        f24 = float_24_80424378;
        *(f32*)((s32)party + 0xF0) = f20;
        *(f32*)((s32)party + 0xF4) = f24;
    }
}

void chuchu_reinit(void) {
    ;
}

void chuchu_exit(void) {
    ;
}
u8 chuchu_move(void* pParty) {
    extern void partyWalkInit(void* party, s32 param);
    extern void unk_800cbeb0(void* party);
    extern void partyWalkMain(void* party);

    if (*(u32*)((s32)pParty + 8) & 8) {
        *(u32*)((s32)pParty + 8) &= ~8;
        *(u32*)pParty &= ~0x100;
        partyWalkInit(pParty, 1);
    }
    unk_800cbeb0(pParty);
    partyWalkMain(pParty);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void chuchu_use(void* pParty) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void marioStSystemLevel(s32 level);
    extern void partyGetAppearPos3(void* party, Vec3* out);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern char* partyChgPoseId(void* party, s32 poseId);
    extern f32 toMovedir(f32 angle);
    extern void psndSFXOn_3D(s32 soundId, void* pos);
    extern void partyChgPose(void* party, char* pose);
    extern void* evtEntryType(void* evtCode, s8 priority, u32 flags, u32 types);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void marioChgMot(s32 mot);
    extern void partyChkGnd(void* party);
    extern void msg_evt;
    extern char str_PCH_A2_1_802f8c58[];
    extern f32 float_0_80424370;
    extern double double_to_int_802f8c68;

    void* player;
    Vec3 pos;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if (*(u32*)((s32)pParty + 8) & 8) {
        *(u32*)((s32)pParty + 8) &= ~8;
        *(s32*)((s32)pParty + 0x39) = 0;
        marioStSystemLevel(1);
    }

    switch (*(u8*)((s32)pParty + 0x39)) {
        case 0:
            partyGetAppearPos3(pParty, &pos);
            *(f32*)((s32)pParty + 0x94) = pos.x;
            *(f32*)((s32)pParty + 0x98) = pos.y;
            *(f32*)((s32)pParty + 0x9C) = pos.z;
            *(s32*)((s32)pParty + 0x24) = 4;
            *(f32*)((s32)pParty + 0x1B0) = angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                                                    *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x1B4) = *(f32*)((s32)pParty + 0x1B0);
            *(s8*)((s32)pParty + 0x174) = *(s8*)((s32)player + 0x280);
            *(s8*)((s32)pParty + 0x175) = *(s8*)((s32)player + 0x280);
            *(f32*)((s32)pParty + 0x1A8) =
                distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                        *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C)) /
                (f32)(*(s32*)((s32)pParty + 0x24));
            *(f32*)((s32)pParty + 0x1AC) = float_0_80424370;
            *(u8*)((s32)pParty + 0x39) = 1;
            break;
        case 1:
            partyGetAppearPos3(pParty, &pos);
            *(f32*)((s32)pParty + 0x94) = pos.x;
            *(f32*)((s32)pParty + 0x98) = pos.y;
            *(f32*)((s32)pParty + 0x9C) = pos.z;
            *(f32*)((s32)pParty + 0x58) += (*(f32*)((s32)pParty + 0x94) - *(f32*)((s32)pParty + 0x58)) /
                                           (f32)(*(s32*)((s32)pParty + 0x24));
            *(f32*)((s32)pParty + 0x5C) += (*(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C)) /
                                           (f32)(*(s32*)((s32)pParty + 0x24));
            *(f32*)((s32)pParty + 0x60) += (*(f32*)((s32)pParty + 0x9C) - *(f32*)((s32)pParty + 0x60)) /
                                           (f32)(*(s32*)((s32)pParty + 0x24));
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer > 0) {
                return;
            }
            partyChgPoseId(pParty, 1);
            *(f32*)((s32)pParty + 0x1A8) = float_0_80424370;
            *(u8*)((s32)pParty + 0x39) = 0xA;
            break;
        case 0xA:
            *(s8*)((s32)pParty + 0x175) = *(s8*)((s32)player + 0x281);
            *(f32*)((s32)pParty + 0x1B0) = toMovedir((f32)*(s8*)((s32)pParty + 0x175));
            *(f32*)((s32)pParty + 0x1B4) = *(f32*)((s32)pParty + 0x1B0);
            *(u8*)((s32)pParty + 0x39) = 0x14;
            *(s32*)((s32)pParty + 0x24) = 0x19;
            break;
        case 0x14:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer > 0) {
                break;
            }
            psndSFXOn_3D(0x944, (void*)((s32)pParty + 0x58));
            partyChgPose(pParty, str_PCH_A2_1_802f8c58);
            *(u8*)((s32)pParty + 0x39) = 0x15;
            *(s32*)((s32)pParty + 0x24) = 0x1E;
        case 0x15:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                partyChgPoseId(pParty, 1);
                evtEntryType(&msg_evt, 0, 0, 0x1A);
                *(u8*)((s32)pParty + 0x39) = 0x16;
            }
            break;
        case 0x17:
            partyChgPoseId(pParty, 1);
            *(u8*)((s32)pParty + 0x39) = 0x50;
            break;
    }

    if (*(u8*)((s32)pParty + 0x39) == 0x50) {
        *(u32*)pParty &= ~0x100;
        partyChgRunMode(pParty, 2);
        *(u8*)((s32)pParty + 0x3B) = 0;
        marioChgMot(0);
        marioStSystemLevel(0);
    }
    partyChkGnd(pParty);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

