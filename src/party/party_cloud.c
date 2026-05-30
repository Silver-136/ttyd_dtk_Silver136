#include "party/party_cloud.h"

extern f32 float_0_804242e0;
extern f32 float_neg1_80424318;
extern f32 float_24_804242f8;
extern f32 float_38_80424348;
extern f32 float_4p25_8042434c;
extern char str_PWD_Z_1_802f89f8[];
extern char str_PWD_S_1_802f8a00[];

void* partyGetPtr(s32 id);
s32 marioGetPartyId(void);
void partyChgPose(void* party, void* pose);
f32 toMovedir(f32 angle);

void unk_80187d74(void* party) {
    f32 zero = float_0_804242e0;
    f32 dist = float_4p25_8042434c;
    *(f32*)((s32)party + 0x17C) = zero;
    *(f32*)((s32)party + 0x180) = dist;
}

void N_cloudLockAnimationsOff(void* party) {
    *(s32*)((s32)party + 0x184) = 0;
    partyChgPose(party, str_PWD_S_1_802f8a00);
}

void N_cloudLockAnimationsOn(void* party) {
    *(s32*)((s32)party + 0x184) = 1;
    partyChgPose(party, str_PWD_Z_1_802f89f8);
}

void cloud_init(void* party) {
    *(u8*)((s32)party + 0x33) = 5;
    *(u8*)((s32)party + 0x32) = 1;
    *(f32*)((s32)party + 0xF0) = float_38_80424348;
    *(f32*)((s32)party + 0xF4) = float_24_804242f8;
    *(s32*)((s32)party + 0x174) = 0;
    *(s32*)((s32)party + 0x178) = 0;
    *(s32*)((s32)party + 0x17C) = 0;
    *(s32*)((s32)party + 0x180) = 0;
    *(s32*)((s32)party + 0x184) = 0;
    *(s32*)((s32)party + 0x170) = 0;
    *(s32*)((s32)party + 0x178) = 0;
}

f32 cloudGetBreathDist(void) {
    void* party = partyGetPtr(marioGetPartyId());
    if (party == 0) {
        return float_0_804242e0;
    }
    if (*(u8*)((s32)party + 0x31) == 5 && (*(u32*)party & 0x100) != 0) {
        if (*(u8*)((s32)party + 0x39) < 10) {
            return float_0_804242e0;
        }
        return *(f32*)(*(s32*)((s32)party + 0x170) + 8);
    }
    return float_0_804242e0;
}

s32 cloudGetStatus(void) {
    s32 ret = 2;
    void* party = partyGetPtr(marioGetPartyId());
    if (party == 0 || (*(u8*)((s32)party + 0x31) == 5 && (*(u32*)party & 0x100) == 0)) {
        return 0;
    }
    if (*(u8*)((s32)party + 0x39) == 10) {
        ret = 1;
    }
    return ret;
}

f32 cloudGetBreathDir(void) {
    void* party = partyGetPtr(0);
    if (party == 0) {
        return float_0_804242e0;
    }
    if ((*(u32*)party & 0x100) != 0 && *(u8*)((s32)party + 0x39) < 10) {
        return float_neg1_80424318;
    }
    party = *(void**)((s32)party + 0x170);
    if (party == 0) {
        return float_0_804242e0;
    }
    return toMovedir(*(f32*)((s32)party + 4));
}


u8 cloudGetBreathPower(void) {
    return 0;
}


s32 cloudGetHitBreathout(int param_1) {
    return 0;
}


u8 cloudGetAt(void* param_1) {
    return 0;
}


u8 cloudResetAt(void) {
    return 0;
}


u8 mot_cloud(void) {
    return 0;
}


u8 getHitBreatheout2(s64 param_1, void* pParty) {
    return 0;
}


u8 cloud_use(void* pParty) {
    return 0;
}


u8 cloud_exit(void* pParty) {
    return 0;
}


u8 cloud_move(void* pParty) {
    return 0;
}
