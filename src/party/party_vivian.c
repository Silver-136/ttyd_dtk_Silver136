#include "party/party_vivian.h"

extern s32 vivian_cancel_req;
extern f32 float_35_804242d8;
extern f32 float_24_804242bc;
extern f32 float_10_80424270;

void L_partyVivianTailStart(void* party) {
    if (*(s8*)((s32)party + 0x31) == 6) {
        s32 flags = *(u8*)((s32)party + 0x4F);
        *(u8*)((s32)party + 0x4F) = flags & ~1;
    }
}

void L_partyVivianTailStop(void* party) {
    if (*(s8*)((s32)party + 0x31) == 6) {
        *(u8*)((s32)party + 0x4F) |= 1;
    }
}

void vivianUnhold(void* party) {
    if (party == 0) {
        return;
    }
    if (*(s8*)((s32)party + 0x31) != 6) {
        return;
    }
    if (*(u32*)party & 0x100) {
        if (*(u8*)((s32)party + 0x39) == 10) {
            *(u8*)((s32)party + 0x39) = 0x14;
        }
    }
}

void vivian_init(void* party) {
    f32 value;
    f32 offset;
    f32 scale;
    s32 six = 6;
    s32 one = 1;
    s32 zero = 0;
    s32 pos1;
    s32 pos0;

    *(u8*)((s32)party + 0x33) = six;
    scale = float_35_804242d8;
    *(u8*)((s32)party + 0x32) = one;
    offset = float_24_804242bc;
    *(f32*)((s32)party + 0xF0) = scale;
    value = float_10_80424270;
    *(f32*)((s32)party + 0xF4) = offset;
    *(f32*)((s32)party + 0x174) = value;
    *(u8*)((s32)party + 0x4F) = zero;
    pos1 = *(s32*)((s32)party + 0x5C);
    pos0 = *(s32*)((s32)party + 0x58);
    *(s32*)((s32)party + 0xB8) = pos0;
    *(s32*)((s32)party + 0xBC) = pos1;
    *(s32*)((s32)party + 0xC0) = *(s32*)((s32)party + 0x60);
    *(s32*)((s32)party + 0x184) = zero;
    vivian_cancel_req = zero;
}


u8 vivian_use(void* pParty) {
    return 0;
}


u8 mot_vivian_post(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void partyPaperOff(void* party);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void partyChgPoseId(void* party, s32 pose);
    extern s32 vec3_802f8948[];
    extern s32 vec3_802f8954[];
    void* party;
    void* mario;

    party = partyGetPtr(marioGetPartyId());
    mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x20 || *(u16*)((s32)mario + 0x2E) == 0x1F) {
        *(u32*)((s32)mario + 4) &= ~0x1000;
        marioPaperOff();
        *(s32*)((s32)mario + 0xA4) = vec3_802f8948[0];
        *(s32*)((s32)mario + 0xA8) = vec3_802f8948[1];
        *(s32*)((s32)mario + 0xAC) = vec3_802f8948[2];
        *(u32*)mario &= ~0x400;
        if (party != 0) {
            partyPaperOff(party);
            *(u32*)party &= ~0x100;
            *(u32*)party &= 0x7FFFFFFF;
            *(u32*)((s32)party + 4) &= ~0x200000;
            *(f32*)((s32)party + 0x174) = float_10_80424270;
            *(s32*)((s32)party + 0x7C) = vec3_802f8954[0];
            *(s32*)((s32)party + 0x80) = vec3_802f8954[1];
            *(s32*)((s32)party + 0x84) = vec3_802f8954[2];
            partyChgRunMode(party, 2);
            *(u8*)((s32)party + 0x3B) = 0;
            partyChgPoseId(party, 1);
            if (*(s8*)((s32)party + 0x31) == 6) {
                *(u8*)((s32)party + 0x4F) &= ~1;
            }
        }
    } else if (party != 0 && *(s8*)((s32)party + 0x31) == 6 && (*(u32*)party & 0x100)) {
        vivian_cancel_req = 1;
    }
}


u8 vivian_move(void* pParty) {
    extern void partyMoveFlyInit(void* party, s32 param);
    extern void partyFlyMain(void* party);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void movePos(f32* x, f32* z, f32 dist, f32 angle);
    extern f32 float_0p1_804242cc;
    extern f32 float_30_804242d0;
    f32 step;
    f32 dx;
    f32 dy;
    f32 dz;

    if (*(u32*)((s32)pParty + 8) & 8) {
        *(u32*)((s32)pParty + 8) &= ~8;
        partyMoveFlyInit(pParty, 1);
    }
    partyFlyMain(pParty);
    if (pParty != 0 && *(s8*)((s32)pParty + 0x31) == 6) {
        step = float_0p1_804242cc;
        dx = *(f32*)((s32)pParty + 0x58) - *(f32*)((s32)pParty + 0xB8);
        dy = *(f32*)((s32)pParty + 0x5C) - *(f32*)((s32)pParty + 0xBC);
        dz = *(f32*)((s32)pParty + 0x60) - *(f32*)((s32)pParty + 0xC0);
        dx = step * dx;
        dy = step * dy;
        dz = step * dz;
        *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0xB8) + dx;
        *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0xBC) + dy;
        *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0xC0) + dz;
        if (distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                    *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)) >= float_30_804242d0) {
            *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
            *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
            *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);
            movePos((f32*)((s32)pParty + 0xB8), (f32*)((s32)pParty + 0xC0),
                    float_30_804242d0,
                    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)));
        }
    }
}


u8 vivianShadow(void* pParty) {
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void movePos(f32* x, f32* z, f32 dist, f32 angle);
    extern f32 float_0p1_804242cc;
    extern f32 float_30_804242d0;
    f32 step;
    f32 dx;
    f32 dy;
    f32 dz;

    if (pParty != 0 && *(s8*)((s32)pParty + 0x31) == 6) {
        step = float_0p1_804242cc;
        dx = *(f32*)((s32)pParty + 0x58) - *(f32*)((s32)pParty + 0xB8);
        dy = *(f32*)((s32)pParty + 0x5C) - *(f32*)((s32)pParty + 0xBC);
        dz = *(f32*)((s32)pParty + 0x60) - *(f32*)((s32)pParty + 0xC0);
        dx = step * dx;
        dy = step * dy;
        dz = step * dz;
        *(f32*)((s32)pParty + 0xB8) = *(f32*)((s32)pParty + 0xB8) + dx;
        *(f32*)((s32)pParty + 0xBC) = *(f32*)((s32)pParty + 0xBC) + dy;
        *(f32*)((s32)pParty + 0xC0) = *(f32*)((s32)pParty + 0xC0) + dz;
        if (distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                    *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)) >= float_30_804242d0) {
            *(s32*)((s32)pParty + 0xB8) = *(s32*)((s32)pParty + 0x58);
            *(s32*)((s32)pParty + 0xBC) = *(s32*)((s32)pParty + 0x5C);
            *(s32*)((s32)pParty + 0xC0) = *(s32*)((s32)pParty + 0x60);
            movePos((f32*)((s32)pParty + 0xB8), (f32*)((s32)pParty + 0xC0),
                    float_30_804242d0,
                    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             *(f32*)((s32)pParty + 0xB8), *(f32*)((s32)pParty + 0xC0)));
        }
    }
}


s32 N_vivianDisplayTailCheck(void* pParty) {
    extern void* gp;
    extern s32 strcmp(char* a, char* b);
    extern char str_bom_01_802f89b0[];
    extern f32 float_neg1000_804242d4;

    if (*(s8*)((s32)pParty + 0x31) != 6) {
        return 0;
    }
    if (*(u8*)((s32)pParty + 0x4F) & 1) {
        return 0;
    }
    if (*(u32*)((s32)pParty + 4) & 0x00800000) {
        return 0;
    }
    if (strcmp((char*)((s32)gp + 0x12C), str_bom_01_802f89b0) == 0 &&
        float_neg1000_804242d4 == *(f32*)((s32)pParty + 0x5C)) {
        return 0;
    }
    return 1;
}

u8 N_marioForceVivianAnime(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioChgPaper(void* name);
    extern void marioPaperOn(void* name);
    extern void marioChgPose(void* pose);
    extern s32 marioAnimeId(void);
    extern void animPoseSetLocalTime(void* pose, f32 time);
    extern char str_p_bibi_802f898c[];
    extern char str_PM_B_1_802f8994[];
    extern char str_M_B_3_804242a4;

    void* mario = marioGetPtr();
    s32 animeId;
    u16 frame;

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(str_p_bibi_802f898c);
    marioChgPose(&str_M_B_3_804242a4);
    marioChgPaper(str_PM_B_1_802f8994);
    *(u16*)((s32)mario + 0x28) = 0x18;
    animeId = marioAnimeId();
    frame = *(u16*)((s32)mario + 0x28);
    animPoseSetLocalTime(*(void**)((s32)mario + 0x22C + animeId * 4), (f32)frame);
}


s32 vivianGetStatus(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);

    void* party = partyGetPtr(marioGetPartyId());
    u8 status;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 6 || !(*(u32*)party & 0x100)) {
        return 0;
    }
    status = *(u8*)((s32)party + 0x39);
    if (status < 10) {
        return 1;
    }
    if (status == 10) {
        return 2;
    }
    if (status >= 20 && status <= 27) {
        return 3;
    }
    return 4;
}

u8 mot_vivian(void) {
    return 0;
}
