#include "party/party_bomhei.h"

s32 bomheiGetStatus(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);

    void* party = partyGetPtr(marioGetPartyId());
    s32 status = 2;

    if (party == NULL || ((s32)*(u8*)((s32)party + 0x31) == 3 && !(*(u32*)party & 0x100))) {
        return 0;
    }

    if (*(u8*)((s32)party + 0x39) == 0x3C) {
        status = 1;
    }
    return status;
}

void bomhei_init(void* party) {
    extern f32 float_28_804213d8;

    *(u8*)((s32)party + 0x33) = 3;
    {
        f32 temp = float_28_804213d8;

        *(u8*)((s32)party + 0x32) = 0;
        *(f32*)((s32)party + 0xF0) = temp;
        *(f32*)((s32)party + 0xF4) = temp;
    }
    *(u8*)((s32)party + 0x17A) = 100;
    *(s32*)((s32)party + 0x17C) = -1;
    *(s32*)((s32)party + 0x180) = -1;
    *(s32*)((s32)party + 0x184) = -1;
}


u8 bomhei_use_main(void* party) {
    return 0;
}


u8 partyGetReturnPos(void) {
    return 0;
}


u8 bomhei_use(void* pParty) {
    return 0;
}


u8 check_front(void* pParty) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mario_controll(void* pParty) {
    extern void marioAdjustMoveDir(void);
    extern void marioChgPose(const char* pose);
    extern int strcmp(const char* s1, const char* s2);
    extern int marioAnimeId(void);
    extern void animPoseSetLocalTime(float time, int poseId);
    extern void marioChgMot(int mot);
    extern const char str_M_U_2_80421318[6];
    extern const char str_M_W_7_80421320[6];
    extern const char str_M_C_1_80421328[6];

    void* player;
    unsigned char state;
    int frame;
    int timer;

    player = *(void**)((int)pParty + 0x160);
    state = *(unsigned char*)((int)pParty + 0x17A);

    switch (state) {
        case 0xA:
            marioAdjustMoveDir();
            marioChgPose(str_M_U_2_80421318);
            *(unsigned int*)player |= 0x2000;
            *(unsigned char*)((int)pParty + 0x17A) = 0x65;
            break;

        case 0x14:
            *(unsigned int*)player |= 0x2000;
            *(unsigned char*)((int)pParty + 0x17A) = 0x15;
            break;

        case 0x15:
            if (strcmp(*(const char**)((int)player + 0x18), str_M_W_7_80421320) == 0) {
                animPoseSetLocalTime((float)*(unsigned char*)((int)pParty + 0x17B),
                                      *(int*)((int)player + 0x22C + (marioAnimeId() << 2)));
                frame = *(unsigned char*)((int)pParty + 0x17B) + 1;
                *(unsigned char*)((int)pParty + 0x17B) = frame;
                if ((unsigned char)frame >= 0x28) {
                    *(unsigned char*)((int)pParty + 0x17B) = 0;
                }
            } else {
                *(unsigned char*)((int)pParty + 0x17B) = 0;
            }
            break;

        case 0x28:
            marioChgMot(0);
            *(unsigned char*)((int)pParty + 0x17A) = 0x29;
            *(short*)((int)pParty + 0x178) = 0x28;
            break;

        case 0x29:
            timer = *(short*)((int)pParty + 0x178) - 1;
            *(short*)((int)pParty + 0x178) = timer;
            if ((short)timer <= 0) {
                marioChgMot(0);
                *(unsigned char*)((int)pParty + 0x17A) = 0x64;
            }
            break;

        case 0x1E:
            marioChgMot(0);
            *(unsigned char*)((int)pParty + 0x17A) = 0x1F;
            break;

        case 0x1F:
            marioChgPose(str_M_C_1_80421328);
            *(unsigned int*)player &= ~0x2000;
            *(short*)((int)pParty + 0x178) = 0x32;
            *(unsigned char*)((int)pParty + 0x17A) = 0x20;
            break;

        case 0x20:
            timer = *(short*)((int)pParty + 0x178) - 1;
            *(short*)((int)pParty + 0x178) = timer;
            if ((short)timer <= 0) {
                *(unsigned char*)((int)pParty + 0x17A) = 0x24;
                *(short*)((int)pParty + 0x178) = 10;
            }
            break;

        case 0x24:
            animPoseSetLocalTime((float)*(short*)((int)pParty + 0x178),
                                  *(int*)((int)player + 0x22C + (marioAnimeId() << 2)));
            timer = *(short*)((int)pParty + 0x178) - 1;
            *(short*)((int)pParty + 0x178) = timer;
            if ((short)timer < 0) {
                marioChgMot(0);
                *(unsigned char*)((int)pParty + 0x17A) = 0x64;
            }
            break;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 bomhei_use_post(void* party) {
    return 0;
}


u8 bomhei_use_cancel2(void) {
    return 0;
}


u8 bomhei_exit(void* pParty) {
    return 0;
}


u8 bomhei_use_cancel(void) {
    return 0;
}


s32 mario_bomhei_keychk(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* party;
    u8 state;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) return 1;
    if (*(s8*)((s32)party + 0x31) != 3) return 1;
    if ((*(u32*)party & 0x100) == 0) return 1;
    state = *(u8*)((s32)party + 0x17A);
    if (state == 0xA || (state >= 0x14 && state <= 0x15) || state == 0x65 || (state >= 0x28 && state <= 0x29) || (state >= 0x1E && state <= 0x24)) return 0;
    state = *(u8*)((s32)party + 0x39);
    if ((state >= 0x32 && state <= 0x33) || state == 0x3C || (state >= 0x46 && state <= 0x47)) return 0;
    return 1;
}

u8 bomhei_bye(void* pParty) {
    return 0;
}


s32 mario_bomhei_keychk2(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* party;
    u8 state;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) return 1;
    if (*(s8*)((s32)party + 0x31) != 3) return 1;
    if ((*(u32*)party & 0x100) == 0) return 1;
    state = *(u8*)((s32)party + 0x17A);
    if (state == 0xA || state == 0x65 || (state >= 0x1E && state <= 0x24)) return 0;
    return 1;
}

