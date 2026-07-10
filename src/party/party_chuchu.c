#include "party/party_chuchu.h"

extern f32 float_20_80424374;
extern f32 float_24_80424378;
extern f32 float_0_80424370;
extern f32 float_18_80424388;
extern char str_PCH_Y_1_802f8ca0[];

s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);

void chuchu_exit(void) {
    ;
}

void chuchu_reinit(void) {
    ;
}

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

s32 post_msg(void* party) {
    void* current;

    current = partyGetPtr(marioGetPartyId());
    *(s32*)((s32)party + 0x9C) = 0;
    if (current != 0) {
        (*(u8*)((s32)current + 0x39))++;
    }
    return 2;
}


u8 set_msg(void* pEvt) {
    return 0;
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

u8 chuchu_use(void* pParty) {
    return 0;
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

