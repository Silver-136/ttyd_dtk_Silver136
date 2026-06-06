#include "party/party_chuchu.h"

extern f32 float_20_80424374;
extern f32 float_24_80424378;
extern f32 float_0_80424370;
extern f32 float_18_80424388;
extern char str_PCH_Y_1_802f8ca0[];

s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);

void chuchu_exit(void) {
}

void chuchu_reinit(void) {
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


u8 chuchu_searchObject(void) {
    return 0;
}


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


u8 chuchu_use(void* pParty) {
    return 0;
}


u8 chuchu_move(void* pParty) {
    return 0;
}
