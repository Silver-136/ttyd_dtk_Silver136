#include "party/party_cloud.h"

extern f32 float_0_804242e0;
extern f32 float_neg1_80424318;
extern f32 float_24_804242f8;
extern f32 float_38_80424348;
extern f32 float_4p25_8042434c;
extern f32 float_180_804242e8;
extern f32 float_18_804242fc;
extern f32 float_0p01_80424320;
extern f32 float_100_80424324;
extern f32 float_10_80424334;
extern f32 float_3p1416_80424350;
extern f32 float_3_80424354;
extern f32 float_0p5_80424358;
extern f32 float_30_8042435c;
extern f32 float_330_80424360;
extern char str_PWD_Z_1_802f89f8[];
extern char str_PWD_S_1_802f8a00[];
extern f32 cloud_at[3];

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
    f32 y;
    f32 x;
    s32 zero;

    *(u8*)((s32)party + 0x33) = 5;
    zero = 0;
    x = float_38_80424348;
    *(u8*)((s32)party + 0x32) = 1;
    y = float_24_804242f8;
    *(f32*)((s32)party + 0xF0) = x;
    *(f32*)((s32)party + 0xF4) = y;
    *(s32*)((s32)party + 0x174) = zero;
    *(s32*)((s32)party + 0x178) = zero;
    *(s32*)((s32)party + 0x17C) = zero;
    *(s32*)((s32)party + 0x180) = zero;
    *(s32*)((s32)party + 0x184) = zero;
    *(s32*)((s32)party + 0x170) = zero;
    *(s32*)((s32)party + 0x178) = zero;
}

f32 cloudGetBreathDist(void) {
    void* party = partyGetPtr(marioGetPartyId());
    if (party == 0) {
        return float_0_804242e0;
    }
    if (*(s8*)((s32)party + 0x31) != 5) {
        goto fail;
    }
    if ((*(u32*)party & 0x100) == 0) {
        goto fail;
    }
    if (*(u8*)((s32)party + 0x39) >= 10) {
        goto success;
    }
fail:
    return float_0_804242e0;
success:
    return *(f32*)(*(s32*)((s32)party + 0x170) + 8);
}

s32 cloudGetStatus(void) {
    void* party = partyGetPtr(marioGetPartyId());
    s32 ret = 2;
    u8 status;

    if (party == 0) {
        goto fail;
    }
    if (*(s8*)((s32)party + 0x31) != 5) {
        goto check_status;
    }
    if (*(u32*)party & 0x100) {
        goto check_status;
    }
fail:
    return 0;
check_status:
    status = *(u8*)((s32)party + 0x39);
    if (status == 10 && status < 20) {
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


f32 cloudGetBreathPower(void* pos, f32 radius) {
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 revise360(f32 angle);
    extern f64 cos(f64 x);
    extern f32 __fabsf(f32 value);
    void* party;
    void* work;
    f32 ret;
    f32 dir;
    f32 angle;

    party = partyGetPtr(0);
    ret = float_0_804242e0;
    if (party == 0) {
        return ret;
    }
    if (*(s8*)((s32)party + 0x31) == 5 &&
        (*(u32*)party & 0x100) &&
        *(u8*)((s32)party + 0x39) >= 10 &&
        __fabsf((*(f32*)((s32)party + 0x5C) - *(f32*)((s32)pos + 4)) - float_18_804242fc) <= float_10_80424334) {
        work = *(void**)((s32)party + 0x170);
        if (distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                    *(f32*)pos, *(f32*)((s32)pos + 8)) - float_0p5_80424358 * radius <= *(f32*)((s32)work + 8)) {
            dir = toMovedir(*(f32*)((s32)work + 4));
            angle = revise360(angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                                       *(f32*)pos, *(f32*)((s32)pos + 8)) - dir);
            if (angle <= float_30_8042435c || angle >= float_330_80424360) {
                ret = float_100_80424324 *
                      __fabsf((f32)cos((float_3p1416_80424350 * angle) / float_180_804242e8));
            }
        }
    }
    return ret;
}


s32 cloudGetHitBreathout(int param_1) {
    void* party;
    s32 ret;

    party = partyGetPtr(marioGetPartyId());
    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 5 || !(*(u32*)party & 0x100) ||
        *(u8*)((s32)party + 0x39) < 10) {
        return 0;
    }
    ret = 0;
    if (param_1 == 0) {
        ret = *(s32*)(*(s32*)((s32)party + 0x170) + 0x20);
    } else if (param_1 == 1) {
        ret = *(s32*)(*(s32*)((s32)party + 0x170) + 0x24);
    } else if (param_1 == 2) {
        ret = *(s32*)(*(s32*)((s32)party + 0x170) + 0x28);
    } else if (param_1 == 3) {
        ret = *(s32*)(*(s32*)((s32)party + 0x170) + 0x2C);
    }
    return ret;
}

u8 cloudGetAt(void* param_1) {
    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32 cameraId);
    void* mario;
    void* party;
    void* cam;

    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party == 0) {
        return;
    }
    if (*(s8*)((s32)party + 0x31) == 5) {
        if (*(u32*)party & 0x100) {
            goto check_mario;
        }
        goto done;
    }
check_mario:
    if (*(u32*)((s32)mario + 4) & 0x100000) {
        goto done;
    }
    cam = camGetPtr(4);
    if (*(u16*)((s32)cam + 4) == 0) {
        *(s32*)&cloud_at[0] = *(s32*)param_1;
        *(s32*)&cloud_at[1] = *(s32*)((s32)param_1 + 4);
        *(s32*)&cloud_at[2] = *(s32*)((s32)param_1 + 8);
    } else {
        *(s32*)param_1 = *(s32*)&cloud_at[0];
        *(s32*)((s32)param_1 + 4) = *(s32*)&cloud_at[1];
        *(s32*)((s32)param_1 + 8) = *(s32*)&cloud_at[2];
    }
done:
    ;
}


u8 cloudResetAt(void) {
    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32 cameraId);
    extern void marioCamZoomOffReq2(s32 frames);
    extern void marioAdjustMoveDir(void);
    void* mario;
    void* party;
    void* cam;
    f32 small;
    f32 zero;

    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (party != 0 && *(s8*)((s32)party + 0x31) == 5) {
        cam = camGetPtr(4);
        cloud_at[0] = *(f32*)((s32)cam + 0x18);
        cam = camGetPtr(4);
        cloud_at[1] = *(f32*)((s32)cam + 0x1C);
        cam = camGetPtr(4);
        cloud_at[2] = *(f32*)((s32)cam + 0x20);
        marioCamZoomOffReq2(0x258);
    }
    *(u32*)((s32)mario + 4) &= ~0x100;
    *(u32*)((s32)mario + 4) &= ~4;
    marioAdjustMoveDir();
    small = float_0p01_80424320;
    zero = float_0_804242e0;
    *(f32*)((s32)mario + 0x150) = small;
    *(f32*)((s32)mario + 0x138) = zero;
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
    extern void psndSFXOff(s32 soundId);
    extern void effSoftDelete(void* effect);
    extern void __memFree(s32 heap, void* ptr);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMot(s32 mot);
    void* work;
    void* mario;
    s32 soundId;
    f32 small;
    f32 zero;

    work = *(void**)((s32)pParty + 0x170);
    mario = *(void**)((s32)pParty + 0x160);
    if (work != 0) {
        soundId = *(s32*)((s32)work + 0x34);
        if ((u32)(soundId + 0x10000) != 0xFFFF) {
            psndSFXOff(soundId);
            *(s32*)((s32)*(void**)((s32)pParty + 0x170) + 0x34) = -1;
        }
    }
    if (*(void**)((s32)pParty + 0x178) != 0) {
        effSoftDelete(*(void**)((s32)pParty + 0x178));
        *(s32*)((s32)pParty + 0x178) = 0;
    }
    if (*(void**)((s32)pParty + 0x170) != 0) {
        __memFree(0, *(void**)((s32)pParty + 0x170));
        *(s32*)((s32)pParty + 0x170) = 0;
    }
    *(u32*)((s32)mario + 4) &= ~0x100;
    *(u32*)((s32)mario + 4) &= ~4;
    marioAdjustMoveDir();
    marioChgMot(0);
    small = float_0p01_80424320;
    zero = float_0_804242e0;
    *(f32*)((s32)mario + 0x150) = small;
    *(f32*)((s32)mario + 0x138) = zero;
}


u8 cloud_move(void* pParty) {
    extern void partyMoveFlyInit(void* party, s32 param);
    extern void partyFlyMain(void* party);
    extern f32 revise360(f32 angle);
    extern f64 sin(f64 x);
    f32 angle;
    f32 value;
    f32 zero;
    f32 dist;

    if (*(u32*)((s32)pParty + 8) & 8) {
        *(u32*)((s32)pParty + 8) &= ~8;
        partyMoveFlyInit(pParty, 1);
        zero = float_0_804242e0;
        dist = float_4p25_8042434c;
        *(f32*)((s32)pParty + 0x17C) = zero;
        *(f32*)((s32)pParty + 0x180) = dist;
    }
    if (*(s32*)((s32)pParty + 0x184) == 0) {
        partyFlyMain(pParty);
        angle = revise360(*(f32*)((s32)pParty + 0x17C) - *(f32*)((s32)pParty + 0x180));
        *(f32*)((s32)pParty + 0x17C) = angle;
        value = (f32)sin((float_3p1416_80424350 * *(f32*)((s32)pParty + 0x17C)) / float_180_804242e8);
        *(f32*)((s32)pParty + 0x80) = float_3_80424354 * value + float_3_80424354;
        if (*(u8*)((s32)pParty + 0x3B) != 2) {
            *(u32*)((s32)pParty + 8) &= ~0x40000;
        }
    }
}
