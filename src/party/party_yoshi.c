#include "party/party_yoshi.h"

extern s32 evt_yoshi_frm;
extern f32 evt_yoshi_spd;
extern f32 float_26_804211e0;
extern f32 float_20_804211e4;

s32 sysMsec2Frame(s32 msec);

s32 evt_get_yoshi_frm(void) {
    return evt_yoshi_frm;
}

void evt_set_yoshi_spd(f32 speed) {
    evt_yoshi_spd = speed;
}

void evt_set_yoshi_frm(s32 msec) {
    evt_yoshi_frm = sysMsec2Frame(msec);
}

void yoshi_init(void* party) {
    f32 offset;
    f32 height;

    *(u8*)((s32)party + 0x33) = 4;
    height = float_26_804211e0;
    *(u8*)((s32)party + 0x32) = 0;
    offset = float_20_804211e4;
    *(f32*)((s32)party + 0xF0) = height;
    *(f32*)((s32)party + 0xF4) = offset;
    *(s32*)((s32)party + 0x168) = -1;
}


u8 yoshi_use(s32 pParty) {
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void __memFree(s32, void*);
    extern void partyUpdateKeyData(void*);
    extern void L_partyClearCont(void*);
    extern s32 marioKeyOffChk(void);
    extern void partyChgRunMode(void*, s32);
    extern f64 angleABf(f32, f32, f32, f32);
    extern f64 distABf(f32, f32, f32, f32);
    extern void searchGround(void*);
    extern void marioChgSmallJumpMotion(void);
    extern void partyChkGnd(void*);
    extern void inertia(void*);
    void* party = (void*)pParty;
    void* player = *(void**)(pParty + 0x160);
    void* work;
    u8 state;

    if (*(u8*)((s32)player + 0x2A) == 0) partyUpdateKeyData(party);
    else L_partyClearCont(party);
    if ((*(u32*)(pParty + 8) & 0x1000) != 0) {
        *(u32*)(pParty + 8) &= ~0x1000;
        *(u32*)pParty &= ~0xC78000;
        work = __memAlloc(0, 0x18);
        *(void**)(pParty + 0x164) = work;
        memset(work, 0, 0x18);
        *(u32*)((s32)work + 0x14) = 0;
        *(u8*)(pParty + 0x38) = (*(u32*)pParty & 0x10000000) ? 4 : 0;
        *(s32*)(pParty + 0x9C) = 3;
        *(f32*)((s32)player + 0x180) = 0.0f;
        *(f32*)((s32)player + 0x178) = 0.0f;
    }
    work = *(void**)(pParty + 0x164);
    state = *(u8*)(pParty + 0x38);
    if (state == 0) {
        if (--*(s32*)(pParty + 0x9C) > 0) {
            if (marioKeyOffChk() == 0 && *(void**)((s32)player + 0x1F4) != 0) return 0;
            if ((*(u32*)pParty & 0x200000) != 0) {
                *(u32*)pParty &= ~0x200000;
                if (work != 0) { __memFree(0, work); *(void**)(pParty + 0x164) = 0; }
            }
            if (*(u8*)(pParty + 0x34) != 8 && *(u8*)(pParty + 0x34) != 13) partyChgRunMode(party, 2);
            return 0;
        }
        *(u8*)(pParty + 0x38) = 4;
        state = 4;
    }
    if (state == 4) {
        *(f32*)((s32)player + 0x1A4) = (*(f32*)((s32)player + 0x1A4) <= 180.0f) ? 90.0f : 270.0f;
        *(f32*)(pParty + 0xB8) = *(f32*)((s32)player + 0x8C);
        *(f32*)(pParty + 0xBC) = *(f32*)((s32)player + 0x90);
        *(f32*)(pParty + 0xC0) = *(f32*)((s32)player + 0x94);
        *(s32*)(pParty + 0x9C) = 4;
        *(f32*)(pParty + 0x104) = (f32)angleABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60),
                                                *(f32*)(pParty + 0xB8), *(f32*)(pParty + 0xC0));
        *(f32*)(pParty + 0x100) = (f32)distABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60),
                                               *(f32*)(pParty + 0xB8), *(f32*)(pParty + 0xC0)) / 4.0f;
        *(u8*)(pParty + 0x38) = 5;
        state = 5;
    }
    if (state == 5) {
        f32 frames = (f32)*(s32*)(pParty + 0x9C);
        *(f32*)(pParty + 0x58) += (*(f32*)(pParty + 0xB8) - *(f32*)(pParty + 0x58)) / frames;
        *(f32*)(pParty + 0x5C) += (*(f32*)(pParty + 0xBC) - *(f32*)(pParty + 0x5C)) / frames;
        *(f32*)(pParty + 0x60) += (*(f32*)(pParty + 0xC0) - *(f32*)(pParty + 0x60)) / frames;
        if (--*(s32*)(pParty + 0x9C) == 0) {
            marioChgSmallJumpMotion();
            *(f32*)((s32)player + 0x180) = 0.0f;
            *(f32*)(pParty + 0x100) = 0.0f;
            *(u8*)(pParty + 0x38) = 7;
            partyChkGnd(party);
        } else searchGround(party);
        return 0;
    }
    if (state >= 12) inertia(party);
    return 0;
}

u8 searchGround(s32 pParty) {
    extern void* partySearchGround(f64,f64,void*);
    extern u32 hitGetAttr(void*);
    extern void psndSFXOff(s32);
    extern void marioSetSplash(s32,void*);
    extern void marioChgMot(s32);
    extern void partyChgPose(void*,char*);
    extern void marioChgPose(char*);
    extern void partyChgRunMode(void*,s32);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void* marioGetPtr(void);
    extern f64 fabs(f64);
    extern f32 float_0_80421138;
    extern f32 float_1_80421160;
    extern f32 float_neg0p4_80421158;
    extern f32 float_neg15_8042115c;
    extern f32 float_neg6_80421168;
    extern char str_PYS_S_2_802cb148[];
    extern char str_M_Y_2_80421148;
    void* party=(void*)pParty;void* player=*(void**)(pParty+0x160);void* hit=0;void* extra;u32 attr;f32 distance;u32 pos[3];
    if((*(u32*)party&0x02000000)==0){distance=(f32)fabs(*(f32*)(pParty+0x11C));if(distance<float_1_80421160)distance=float_1_80421160;hit=partySearchGround(distance,distance,party);}
    if(hit!=0){attr=hitGetAttr(hit);if(*(s32*)(pParty+0x168)!=-1){psndSFXOff(*(s32*)(pParty+0x168));*(s32*)(pParty+0x168)=-1;}
        if((attr&0x200)!=0){if(*(u8*)(pParty+0x39)==5)return 0;*(u8*)(pParty+0x39)=0x46;pos[0]=*(u32*)(pParty+0x58);pos[1]=*(u32*)(pParty+0x5C);pos[2]=*(u32*)(pParty+0x60);marioSetSplash(0,pos);*(void**)(pParty+0x138)=0;
            if((*(u32*)party&0x10)==0){*(u32*)party|=0x10;*(f32*)(pParty+0x11C)=float_neg0p4_80421158;*(f32*)(pParty+0x174)=float_neg0p4_80421158;*(f32*)(pParty+0x178)=float_0_80421138;*(f32*)(pParty+0x118)=float_0_80421138;}
            *(f32*)(pParty+0x178)+=*(f32*)(pParty+0x118);*(f32*)(pParty+0x174)+=*(f32*)(pParty+0x178);*(f32*)(pParty+0x11C)+=*(f32*)(pParty+0x174);if(*(f32*)(pParty+0x11C)<float_neg15_8042115c)*(f32*)(pParty+0x11C)=float_neg15_8042115c;*(f32*)(pParty+0x5C)+=*(f32*)(pParty+0x11C);*(u32*)party|=0x02000000;if(*(f32*)(pParty+0x11C)<=float_neg6_80421168){*(f32*)(pParty+0x11C)=float_neg6_80421168;*(f32*)(pParty+0x174)=float_0_80421138;*(f32*)(pParty+0x178)=float_0_80421138;*(f32*)(pParty+0x118)=float_0_80421138;}*(f32*)((s32)player+0x7C)=*(f32*)(pParty+0x11C);*(f32*)((s32)player+0x80)=*(f32*)(pParty+0x174);*(f32*)((s32)player+0x84)=*(f32*)(pParty+0x178);*(f32*)((s32)player+0x88)=*(f32*)(pParty+0x118);marioChgMot(0x1F);return 0;}
        if((attr&0xC00)!=0){*(void**)(pParty+0x138)=0;marioChgMot(0x20);*(u8*)(pParty+0x39)=10;partyChgPose(party,str_PYS_S_2_802cb148);marioChgPose(&str_M_Y_2_80421148);*(f32*)((s32)player+0x2C8)=float_0_80421138;*(f32*)((s32)player+0x2CC)=float_0_80421138;partyChgRunMode(party,0xD);extra=partyGetPtr(marioGetExtraPartyId());if(extra!=0)partyChgRunMode(extra,0xD);return 0;}
    }
    *(void**)(pParty+0x138)=hit;if(hit==0){*(u32*)party&=~2;if((*(u32*)party&0x10)==0){*(u32*)party|=0x10;*(f32*)(pParty+0x11C)=float_neg0p4_80421158;*(f32*)(pParty+0x174)=float_neg0p4_80421158;*(f32*)(pParty+0x178)=float_0_80421138;*(f32*)(pParty+0x118)=float_0_80421138;}*(f32*)(pParty+0x178)+=*(f32*)(pParty+0x118);*(f32*)(pParty+0x174)+=*(f32*)(pParty+0x178);*(f32*)(pParty+0x11C)+=*(f32*)(pParty+0x174);if(*(f32*)(pParty+0x11C)<float_neg15_8042115c)*(f32*)(pParty+0x11C)=float_neg15_8042115c;*(f32*)(pParty+0x5C)+=*(f32*)(pParty+0x11C);}else{*(u32*)party|=2;*(f32*)(pParty+0x114)=float_0_80421138;*(f32*)(pParty+0x5C)=*(f32*)(pParty+0xE4);}return 0;
}

u8 checkWall(void* pParty, f32 speed, f32 direction) {
    extern void* marioGetPtr(void);
    extern void* partySearchWallFront(f32, f32, void*, void*);
    extern void* dou10_yoko_yari2(void*);
    extern u32 hitGetAttr(void*);
    extern u32 marioChkBero(void*);
    extern void marioChgMot(s32);
    extern void partyChgPose(void*, char*);
    extern void marioChgPose(char*);
    extern void partyChgRunMode(void*, s32);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void movePos(f32, f32, f32*, f32*);
    extern void unk_US_EU_04_800c27c0(f32, f32, f32, void*, void*);
    extern f32 float_0_80421138;
    extern char str_PYS_S_2_802cb148[];
    extern char str_M_Y_2_80421148;
    void* player = marioGetPtr();
    void* hit;
    void* yoshi;
    void* extra;
    u32 attr;
    s32 status = 5;
    s32 out;
    u8 state;
    f32 pos[3];

    pos[0] = *(f32*)((s32)pParty + 0x58);
    pos[1] = *(f32*)((s32)pParty + 0x5C);
    pos[2] = *(f32*)((s32)pParty + 0x60);
    hit = partySearchWallFront(speed, direction, pParty, pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x12C) = hit;
        *(void**)((s32)player + 0x1E4) = hit;
        if (speed != float_0_80421138) {
            if ((*(u32*)pParty & 0x100) != 0 && marioChkBero(hit) == 1) {
                *(void**)((s32)pParty + 0x134) = hit;
                *(void**)((s32)player + 0x1F0) = hit;
                *(void**)((s32)player + 0x1F4) = hit;
            } else {
                *(void**)((s32)pParty + 0x134) = 0;
                *(void**)((s32)player + 0x1F0) = 0;
                *(void**)((s32)player + 0x1F4) = 0;
            }
        }
        attr = hitGetAttr(hit);
        if ((attr & 0x800) != 0) goto spike;
        if ((attr & 0x800004) != 0 && (attr & 0x80080) == 0) {
            movePos(speed, direction, (f32*)((s32)pParty + 0x58),
                    (f32*)((s32)pParty + 0x60));
            return 0;
        }
        *(f32*)((s32)pParty + 0x58) = pos[0];
        *(f32*)((s32)pParty + 0x5C) = pos[1];
        *(f32*)((s32)pParty + 0x60) = pos[2];
    } else {
        if (speed == float_0_80421138) {
            *(void**)((s32)pParty + 0x12C) = 0;
            *(void**)((s32)player + 0x1E4) = 0;
        }
        hit = dou10_yoko_yari2(&out);
        if (hit != 0 && (hitGetAttr(hit) & 0x800) != 0) goto spike;
    }
    unk_US_EU_04_800c27c0(speed, direction, direction, pParty, pos);
    *(f32*)((s32)pParty + 0x58) = pos[0];
    *(f32*)((s32)pParty + 0x60) = pos[2];
    return 0;

spike:
    player = marioGetPtr();
    *(void**)((s32)pParty + 0x138) = 0;
    yoshi = partyGetPtr(marioGetPartyId());
    if (yoshi == 0 || *(s8*)((s32)yoshi + 0x31) != 4 ||
        (*(u32*)yoshi & 0x100) == 0) {
        status = 0;
    } else {
        state = *(u8*)((s32)yoshi + 0x39);
        if (state >= 0xA && state <= 0xD) status = 1;
        else if (state >= 0x14 && state <= 0x16) status = 2;
        else if (state >= 0x64 && state <= 0x65) status = 5;
        else if (state >= 0x3C && state <= 0x41) status = 3;
        else if (state >= 0x32 && state <= 0x34) status = 4;
    }
    if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
        *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = 0;
    }
    marioChgMot(0x20);
    *(u8*)((s32)pParty + 0x39) = 10;
    partyChgPose(pParty, str_PYS_S_2_802cb148);
    marioChgPose(&str_M_Y_2_80421148);
    *(f32*)((s32)player + 0x2C8) = float_0_80421138;
    *(f32*)((s32)player + 0x2CC) = float_0_80421138;
    partyChgRunMode(pParty, 0xD);
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra != 0) partyChgRunMode(extra, 0xD);
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void searchGround2(void* pParty) {
    extern void* partySearchGround(f32 height, f32 y, void* party);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern u32 hitGetAttr(void* hit);
    extern void marioSetSplash(s32 type, void* pos);
    extern void* marioGetPtr(void);
    extern void marioChgMot(s32 motion);
    extern void partyChgPose(void* party, char* pose);
    extern void marioChgPose(char* pose);
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 marioGetExtraPartyId(void);
    extern f32 float_11_80421154;
    extern f32 float_0_80421138;
    extern f32 float_neg0p4_80421158;
    extern f32 float_neg15_8042115c;
    extern char str_PYS_S_2_802cb148[];
    extern char str_M_Y_2_80421148;

    void* hit = 0;
    void* player = *(void**)((s32)pParty + 0x160);
    void* party;
    s32 status;
    u8 motion;
    u32 attr;
    u32 pos[3];
    void* mario;

    if ((*(u32*)pParty & 0x02000000) == 0) {
        hit = partySearchGround(float_11_80421154, float_0_80421138, pParty);
    }

    if (hit == 0) {
        *(void**)((s32)pParty + 0x138) = hit;
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            motion = *(u8*)((s32)party + 0x39);
            if (motion >= 0xA && motion <= 0xD) {
                status = 1;
            } else if (motion >= 0x14 && motion <= 0x16) {
                status = 2;
            } else if (motion >= 0x64 && motion <= 0x65) {
                status = 5;
            } else if (motion >= 0x3C && motion <= 0x41) {
                status = 3;
            } else if (motion >= 0x32 && motion <= 0x34) {
                status = 4;
            }
        }
        if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
            *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = hit;
        }
        return;
    }

    attr = hitGetAttr(hit);
    if ((attr & 0x200) != 0) {
        pos[0] = *(u32*)((s32)pParty + 0x58);
        pos[1] = *(u32*)((s32)pParty + 0x5C);
        pos[2] = *(u32*)((s32)pParty + 0x60);
        marioSetSplash(0, pos);
        *(void**)((s32)pParty + 0x138) = 0;
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            motion = *(u8*)((s32)party + 0x39);
            if (motion >= 0xA && motion <= 0xD) {
                status = 1;
            } else if (motion >= 0x14 && motion <= 0x16) {
                status = 2;
            } else if (motion >= 0x64 && motion <= 0x65) {
                status = 5;
            } else if (motion >= 0x3C && motion <= 0x41) {
                status = 3;
            } else if (motion >= 0x32 && motion <= 0x34) {
                status = 4;
            }
        }
        if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
            *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = 0;
        }

        {
            f32 fall = float_neg0p4_80421158;
            f32 zero = float_0_80421138;
            *(f32*)((s32)pParty + 0x11C) = fall;
            *(f32*)((s32)pParty + 0x174) = fall;
            *(f32*)((s32)pParty + 0x178) = zero;
            *(f32*)((s32)pParty + 0x118) = zero;
        }
        *(f32*)((s32)pParty + 0x178) += *(f32*)((s32)pParty + 0x118);
        *(f32*)((s32)pParty + 0x174) += *(f32*)((s32)pParty + 0x178);
        *(f32*)((s32)pParty + 0x11C) += *(f32*)((s32)pParty + 0x174);
        if (*(f32*)((s32)pParty + 0x11C) < float_neg15_8042115c) {
            *(f32*)((s32)pParty + 0x11C) = float_neg15_8042115c;
        }
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x11C);
        *(u8*)((s32)pParty + 0x39) = 0x46;
        return;
    }

    if ((attr & 0x800) != 0) {
        mario = marioGetPtr();
        *(void**)((s32)pParty + 0x138) = 0;
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            motion = *(u8*)((s32)party + 0x39);
            if (motion >= 0xA && motion <= 0xD) {
                status = 1;
            } else if (motion >= 0x14 && motion <= 0x16) {
                status = 2;
            } else if (motion >= 0x64 && motion <= 0x65) {
                status = 5;
            } else if (motion >= 0x3C && motion <= 0x41) {
                status = 3;
            } else if (motion >= 0x32 && motion <= 0x34) {
                status = 4;
            }
        }
        if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
            *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = 0;
        }
        marioChgMot(0x20);
        *(u8*)((s32)pParty + 0x39) = 0xA;
        partyChgPose(pParty, str_PYS_S_2_802cb148);
        marioChgPose(&str_M_Y_2_80421148);
        {
            f32 zero = float_0_80421138;
            *(f32*)((s32)mario + 0x2C8) = zero;
            *(f32*)((s32)mario + 0x2CC) = zero;
        }
        partyChgRunMode(pParty, 0xD);
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            partyChgRunMode(party, 0xD);
        }
    }

    {
        f32 zero = float_0_80421138;
        *(u32*)pParty |= 2;
        *(f32*)((s32)pParty + 0x114) = zero;
    }
    *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
    *(u32*)pParty &= ~0x70;
    *(u32*)player &= ~0xF0000;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void inertia(void* pParty) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern f32 float_0_80421138;
    extern f32 float_neg0p07_80421174;
    extern f32 float_neg0p5_80421178;
    extern f32 float_0p01_8042117c;
    extern f32 float_256_80421180;
    extern f32 float_16_80421184;

    void* player = *(void**)((s32)pParty + 0x160);
    f32 speedX = *(f32*)((s32)player + 0x2C8);
    f32 speedZ = *(f32*)((s32)player + 0x2CC);
    f32 stickX = (f32)*(s8*)((s32)pParty + 0x158);
    f32 x;
    f32 z;
    void* party;
    s32 status;
    u8 motion;
    f32 drag;
    f32 limit;
    s32 dir;

    if (stickX == float_0_80421138) {
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            motion = *(u8*)((s32)party + 0x39);
            if (motion >= 0xA && motion <= 0xD) {
                status = 1;
            } else if (motion >= 0x14 && motion <= 0x16) {
                status = 2;
            } else if (motion >= 0x64 && motion <= 0x65) {
                status = 5;
            } else if (motion >= 0x3C && motion <= 0x41) {
                status = 3;
            } else if (motion >= 0x32 && motion <= 0x34) {
                status = 4;
            }
        }
        if (status != 2) {
            speedX = float_0_80421138;
            speedZ = float_0_80421138;
        } else {
            drag = float_neg0p07_80421174;
            if (*(void**)((s32)player + 0x1F4) != 0) {
                drag = float_neg0p5_80421178;
            }
            speedX = speedX * drag + speedX;
            speedZ = speedZ * drag + speedZ;
            if (__fabsf(speedX) < float_0p01_8042117c) {
                speedX = float_0_80421138;
            }
            if (__fabsf(speedZ) < float_0p01_8042117c) {
                speedZ = float_0_80421138;
            }
        }
    } else if (speedX * speedX + speedZ * speedZ <= float_256_80421180) {
        dir = *(s32*)(*(s32*)((s32)pParty + 0x170) + 4);
        speedX = stickX * float_0p01_8042117c + speedX;
        if (dir > 0 && speedX <= float_0_80421138) {
            speedX = float_0_80421138;
        }
        if (dir < 0 && speedX >= float_0_80421138) {
            speedX = float_0_80421138;
        }
        speedZ = speedZ - float_0_80421138;
    } else {
        speedX = stickX * float_0p01_8042117c + speedX;
        limit = -float_16_80421184;
        if (speedX < limit) {
            speedX = limit;
        }
        if (speedX > float_16_80421184) {
            speedX = float_16_80421184;
        }
        speedZ = speedZ - float_0_80421138;
        if (speedZ < limit) {
            speedZ = limit;
        }
        if (speedZ > float_16_80421184) {
            speedZ = float_16_80421184;
        }
    }
    *(f32*)((s32)player + 0x2C8) = speedX;
    *(f32*)((s32)player + 0x2CC) = speedZ;
}

u8 yoshi_exit(void* pParty) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void marioChgPose(char*);
    extern void marioChgMot(s32);
    extern void partyChgRunMode(void*, s32);
    extern void __memFree(s32, void*);
    extern char str_M_S_1_804211e8;
    void* player;
    void* party;
    void* check;
    void* extra;
    s32 status = 5;
    u8 state;

    *(u32*)*(void**)((s32)pParty + 0x160) &= ~0x4000000;
    player = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    check = partyGetPtr(marioGetPartyId());
    if (check == 0 || *(s8*)((s32)check + 0x31) != 4 ||
        (*(u32*)check & 0x100) == 0) {
        status = 0;
    } else {
        state = *(u8*)((s32)check + 0x39);
        if (state >= 0xA && state <= 0xD) status = 1;
        else if (state >= 0x14 && state <= 0x16) status = 2;
        else if (state >= 0x64 && state <= 0x65) status = 5;
        else if (state >= 0x3C && state <= 0x41) status = 3;
        else if (state >= 0x32 && state <= 0x34) status = 4;
    }
    if (status != 0) {
        void* partyPlayer = *(void**)((s32)party + 0x160);
        *(f32*)((s32)partyPlayer + 0xA0) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xA4) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xA8) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xB8) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xBC) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xC0) = 0.0f;
        extra = partyGetPtr(marioGetExtraPartyId());
        if (extra != 0) {
            *(f32*)((s32)extra + 0xA0) = 0.0f;
            *(f32*)((s32)extra + 0xA4) = 0.0f;
            *(f32*)((s32)extra + 0xA8) = 0.0f;
        }
        marioChgPose(&str_M_S_1_804211e8);
        if ((*(u32*)party & 0x100) != 0) {
            *(u32*)party &= ~0x100;
            if (*(void**)((s32)party + 0x164) != 0) {
                __memFree(0, *(void**)((s32)party + 0x164));
                *(void**)((s32)party + 0x164) = 0;
            }
            if (*(s32*)((s32)player + 0x28) == 0x20) {
                partyChgRunMode(party, 0xD);
                extra = partyGetPtr(marioGetExtraPartyId());
                if (extra != 0) partyChgRunMode(extra, 0xD);
                marioChgMot(0);
                return 0;
            }
        }
        if (*(u8*)((s32)party + 0x34) != 8 && *(u8*)((s32)party + 0x34) != 0xD) {
            partyChgRunMode(party, 2);
            *(u8*)((s32)party + 0x39) = 0;
        }
    }
    marioChgMot(0);
    return 0;
}

u8 marioYoshiForceCancel(void) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void marioChgPose(char*);
    extern void partyChgRunMode(void*, s32);
    extern void __memFree(s32, void*);
    extern char str_M_S_1_804211e8;
    void* player = marioGetPtr();
    void* party = partyGetPtr(marioGetPartyId());
    void* check = partyGetPtr(marioGetPartyId());
    void* extra;
    s32 status = 5;
    u8 state;

    if (check == 0 || *(s8*)((s32)check + 0x31) != 4 ||
        (*(u32*)check & 0x100) == 0) {
        status = 0;
    } else {
        state = *(u8*)((s32)check + 0x39);
        if (state >= 0xA && state <= 0xD) status = 1;
        else if (state >= 0x14 && state <= 0x16) status = 2;
        else if (state >= 0x64 && state <= 0x65) status = 5;
        else if (state >= 0x3C && state <= 0x41) status = 3;
        else if (state >= 0x32 && state <= 0x34) status = 4;
    }
    if (status != 0) {
        void* partyPlayer = *(void**)((s32)party + 0x160);
        *(f32*)((s32)partyPlayer + 0xA0) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xA4) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xA8) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xB8) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xBC) = 0.0f;
        *(f32*)((s32)partyPlayer + 0xC0) = 0.0f;
        extra = partyGetPtr(marioGetExtraPartyId());
        if (extra != 0) {
            *(f32*)((s32)extra + 0xA0) = 0.0f;
            *(f32*)((s32)extra + 0xA4) = 0.0f;
            *(f32*)((s32)extra + 0xA8) = 0.0f;
        }
        marioChgPose(&str_M_S_1_804211e8);
        if ((*(u32*)party & 0x100) != 0) {
            *(u32*)party &= ~0x100;
            if (*(void**)((s32)party + 0x164) != 0) {
                __memFree(0, *(void**)((s32)party + 0x164));
                *(void**)((s32)party + 0x164) = 0;
            }
            if (*(s32*)((s32)player + 0x28) == 0x20) {
                partyChgRunMode(party, 0xD);
                extra = partyGetPtr(marioGetExtraPartyId());
                if (extra != 0) partyChgRunMode(extra, 0xD);
                return 0;
            }
        }
        if (*(u8*)((s32)party + 0x34) != 8 && *(u8*)((s32)party + 0x34) != 0xD) {
            partyChgRunMode(party, 2);
            *(u8*)((s32)party + 0x39) = 0;
        }
    }
    return 0;
}

u8 L_getStick(void* pParty, f32* outDirection, f32* outSpeed) {
    extern f32 distABf(f32, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 revise360(f32);
    extern f32 marioGetMoveRate(void);
    extern u32 hitGetAttr(void*);
    extern f32 float_0_80421138;
    extern f32 float_70_8042113c;
    extern f32 float_1p5_80421140;
    extern f32 float_0p75_80421144;
    void* player = *(void**)((s32)pParty + 0x160);
    f32 distance;
    f32 direction;
    f32 speed;
    s32 stickX;
    s32 stickY;

    if (*(s8*)((s32)player + 0x39) != 0) {
        *outSpeed = float_0_80421138;
        *outDirection = *(f32*)((s32)pParty + 0xFC);
        return 0;
    }

    stickY = *(s8*)((s32)pParty + 0x159);
    stickX = *(s8*)((s32)pParty + 0x158);
    distance = distABf(float_0_80421138, float_0_80421138,
                       (f32)stickX, (f32)-stickY);
    if (distance >= float_70_8042113c) {
        distance = float_70_8042113c;
    }
    direction = angleABf(float_0_80421138, float_0_80421138,
                         (f32)stickX, (f32)-stickY);
    if (distance == float_0_80421138) {
        direction = *(f32*)((s32)pParty + 0xFC);
    }
    *outDirection = revise360(direction + *(f32*)((s32)pParty + 0xF8));
    if (distance > float_0_80421138) {
        if (stickX * stickX + stickY * stickY < 0xBD2) {
            speed = float_1p5_80421140 *
                    (*(f32*)((s32)player + 0x174) * marioGetMoveRate());
        } else {
            speed = float_1p5_80421140 *
                    (*(f32*)((s32)player + 0x178) * marioGetMoveRate());
        }
        if ((hitGetAttr(*(void**)((s32)player + 0x1E8)) & 0x100) == 0) {
            *(u32*)player &= ~0x200000;
        } else {
            speed *= float_0p75_80421144;
            *(u32*)player |= 0x200000;
        }
        distance = speed * *(f32*)((s32)player + 0x17C);
    }
    *outSpeed = distance;
    return 0;
}

void yoshi_bye(void* pParty) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void partyGoodbyeInit(void* party);
    extern s32 partyGoodbyeMain(void* party);
    extern void partyChgRunMode(void* party, s32 mode);
    void* party;
    s32 status;
    u8 state;

    if ((*(u32*)((s32)pParty + 8) & 8) != 0) {
        *(u32*)((s32)pParty + 8) &= ~8;
        partyGoodbyeInit(pParty);
    }
    if ((*(u32*)pParty & 0x100) != 0) {
        party = partyGetPtr(marioGetPartyId());
        status = 5;
        if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
            status = 0;
        } else {
            state = *(u8*)((s32)party + 0x39);
            if (state >= 0xA && state <= 0xD) {
                status = 1;
            } else if (state >= 0x14 && state <= 0x16) {
                status = 2;
            } else if (state >= 0x64 && state <= 0x65) {
                status = 5;
            } else if (state >= 0x3C && state <= 0x41) {
                status = 3;
            } else if (state >= 0x32 && state <= 0x34) {
                status = 4;
            }
        }
        if (status == 1) {
            *(u8*)((s32)pParty + 0x39) = 0x64;
        }
        yoshi_use((s32)pParty);
    } else if (partyGoodbyeMain(pParty) != 0) {
        partyChgRunMode(pParty, 0xE);
    }
}

u8 L_yoshiFlyMove(void* pParty) {
    extern void* marioGetPtr(void);
    extern f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f32 revise360(f32 angle);
    extern f32 float_0_80421138;
    extern f32 float_0p1_80421188;
    extern f32 float_1_80421160;
    extern f32 float_0p01_8042117c;
    void* mario;
    f32 x;
    f32 z;
    f32 speed;
    f32 angle;
    f32 max;

    mario = marioGetPtr();
    inertia(pParty);
    z = -*(f32*)((s32)mario + 0x2CC);
    x = *(f32*)((s32)mario + 0x2C8);
    speed = float_0p1_80421188 * distABf(x, float_0_80421138, float_0_80421138, z);
    max = *(f32*)((s32)mario + 0x188) * float_1_80421160;
    if (speed >= max) {
        speed = max;
    }
    angle = angleABf(x, float_0_80421138, float_0_80421138, z);
    if (speed < float_0p01_8042117c) {
        speed = float_0_80421138;
    }
    *(f32*)((s32)pParty + 0x104) = speed;
    if (speed != float_0_80421138) {
        *(f32*)((s32)pParty + 0x100) = revise360(angle + *(f32*)((s32)pParty + 0xF8));
        *(f32*)((s32)pParty + 0xFC) = revise360(angle + *(f32*)((s32)pParty + 0xF8));
    }
    *(f32*)((s32)mario + 0x18C) = speed;
    return 0;
}

u32 marioGetoffYoshi(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* party;
    void* party2;
    s32 status;
    u8 state;

    party = partyGetPtr(marioGetPartyId());
    if ((*(u32*)party & 0x100) == 0) {
        return 0;
    }
    party2 = partyGetPtr(marioGetPartyId());
    status = 5;
    if (party2 == 0 || *(s8*)((s32)party2 + 0x31) != 4 || (*(u32*)party2 & 0x100) == 0) {
        status = 0;
    } else {
        state = *(u8*)((s32)party2 + 0x39);
        if (state >= 0xA && state <= 0xD) {
            status = 1;
        } else if (state >= 0x14 && state <= 0x16) {
            status = 2;
        } else if (state >= 0x64 && state <= 0x65) {
            status = 5;
        } else if (state >= 0x3C && state <= 0x41) {
            status = 3;
        } else if (state >= 0x32 && state <= 0x34) {
            status = 4;
        }
    }
    if (status == 1) {
        *(u8*)((s32)party + 0x39) = 0x64;
        return 1;
    }
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void L_sweatEntry(void* pParty, void* hitobjStandOn) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* party;
    s32 status;

    *(void**)((s32)pParty + 0x138) = hitobjStandOn;
    party = partyGetPtr(marioGetPartyId());
    status = 5;
    if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
        status = 0;
    } else if (*(u8*)((s32)party + 0x39) >= 0xA && *(u8*)((s32)party + 0x39) <= 0xD) {
        status = 1;
    } else if (*(u8*)((s32)party + 0x39) >= 0x14 && *(u8*)((s32)party + 0x39) <= 0x16) {
        status = 2;
    } else if (*(u8*)((s32)party + 0x39) >= 0x64 && *(u8*)((s32)party + 0x39) <= 0x65) {
        status = 5;
    } else if (*(u8*)((s32)party + 0x39) >= 0x3C && *(u8*)((s32)party + 0x39) <= 0x41) {
        status = 3;
    } else if (*(u8*)((s32)party + 0x39) >= 0x32 && *(u8*)((s32)party + 0x39) <= 0x34) {
        status = 4;
    }
    if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
        *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = hitobjStandOn;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 yoshiGetStatus(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    void* party;
    u8 state;
    s32 status;

    party = partyGetPtr(marioGetPartyId());
    status = 5;
    if (party == 0 || *(s8*)((s32)party + 0x31) != 4 || (*(u32*)party & 0x100) == 0) {
        return 0;
    }
    state = *(u8*)((s32)party + 0x39);
    if (state >= 0xA && state <= 0xD) {
        status = 1;
    } else if (state >= 0x14 && state <= 0x16) {
        status = 2;
    } else if (state >= 0x64 && state <= 0x65) {
        status = 5;
    } else if (state >= 0x3C && state <= 0x41) {
        status = 3;
    } else if (state >= 0x32 && state <= 0x34) {
        status = 4;
    }
    return status;
}

void marioRideYoshi(void) {
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    extern void party_force_ride_yoshi(void* party);
    void* party;

    party = partyGetPtr(marioGetPartyId());
    *(u32*)((s32)party + 8) &= ~8;
    *(u32*)((s32)party + 8) &= ~4;
    *(u32*)((s32)party + 8) &= ~2;
    *(u32*)party |= 0x100;
    *(u8*)((s32)party + 0x34) = 3;
    *(u8*)((s32)party + 0x39) = 0xB;
    if (*(void**)((s32)party + 0x170) == 0) {
        *(void**)((s32)party + 0x170) = __memAlloc(0, 0x18);
        memset(*(void**)((s32)party + 0x170), 0, 0x18);
        *(s32*)((s32)*(void**)((s32)party + 0x170) + 0x14) = 0;
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0) {
        party_force_ride_yoshi(party);
    }
}

void L_sweatMain(void* pParty) {
    extern void effDelete(void* eff);
    extern f32 float_18_804211c4;
    void* effect;

    if (*(void**)((s32)pParty + 0x170) != 0) {
        effect = *(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14);
        if (effect != 0) {
            *(f32*)((s32)*(void**)((s32)effect + 0xC) + 4) = *(f32*)((s32)pParty + 0x58);
            *(f32*)((s32)*(void**)((s32)*(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14) + 0xC) + 8) = float_18_804211c4 + *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)*(void**)((s32)*(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14) + 0xC) + 0xC) = *(f32*)((s32)pParty + 0x60);
            if (--*(s32*)((s32)*(void**)((s32)pParty + 0x170) + 0x10) <= 0) {
                effDelete(*(void**)((s32)*(void**)((s32)pParty + 0x170) + 0x14));
                *(s32*)((s32)*(void**)((s32)pParty + 0x170) + 0x14) = 0;
            }
        }
    }
}

void yoshi_jumpStand(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);

    void* party = partyGetPtr(marioGetPartyId());
    u8 state;

    if (party != 0) {
        if (*(s8*)((s32)party + 0x31) == 4) {
            if ((*(u32*)party & 0x100) == 0) {
                ;
            } else {
                state = *(u8*)((s32)party + 0x39);
                if (state < 0x3C || state > 0x42) {
                    *(u8*)((s32)party + 0x39) = 0x3C;
                }
            }
        }
    }
}

/* CHATGPT FALLBACK MISSING STUBS: main/party/party_yoshi 20260624_191429 */

/* fallback stub-fill: map=unk_800b6468 addr=0x800b6468 size=0x0000016c */
int unk_800b6468(void* pParty) {
    extern s32 strcmp(const char*, const char*);
    extern void animPoseSetLocalTime(s32, f32);
    extern char str_PYS_S_2_802cb148[];
    extern char str_PYS_R_2_802cb158[];
    extern char str_PYS_F_1_802cb168[];
    void* player = *(void**)((s32)pParty + 0x160);
    u16 time;
    u16 limit;

    if (strcmp(*(char**)((s32)pParty + 0x18), str_PYS_S_2_802cb148) == 0) {
        limit = 0x33;
    } else if (strcmp(*(char**)((s32)pParty + 0x18), str_PYS_R_2_802cb158) == 0) {
        limit = 9;
    } else if (strcmp(*(char**)((s32)pParty + 0x18), str_PYS_F_1_802cb168) == 0) {
        limit = 9;
    } else {
        return 0;
    }

    time = *(u16*)((s32)player + 0x20) + 1;
    *(u16*)((s32)player + 0x20) = time;
    if (time > limit) {
        time = 0;
        *(u16*)((s32)player + 0x20) = 0;
    }
    *(u16*)((s32)pParty + 0x20) = time;
    animPoseSetLocalTime(*(s32*)((s32)pParty + 0xC), (f32)time);
    return 0;
}

/* fallback stub-fill: map=unk_800b65d4 addr=0x800b65d4 size=0x00000164 */
int unk_800b65d4(void* pParty) {
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern void marioChgMot(s32);
    extern void partyChgPose(void*, char*);
    extern void marioChgPose(char*);
    extern void partyChgRunMode(void*, s32);
    extern char str_PYS_S_2_802cb148[];
    extern char str_M_Y_2_80421148;
    void* player = marioGetPtr();
    void* yoshi;
    void* extra;
    s32 status = 5;
    u8 state;

    *(void**)((s32)pParty + 0x138) = 0;
    yoshi = partyGetPtr(marioGetPartyId());
    if (yoshi == 0 || *(s8*)((s32)yoshi + 0x31) != 4 ||
        (*(u32*)yoshi & 0x100) == 0) {
        status = 0;
    } else {
        state = *(u8*)((s32)yoshi + 0x39);
        if (state >= 0xA && state <= 0xD) status = 1;
        else if (state >= 0x14 && state <= 0x16) status = 2;
        else if (state >= 0x64 && state <= 0x65) status = 5;
        else if (state >= 0x3C && state <= 0x41) status = 3;
        else if (state >= 0x32 && state <= 0x34) status = 4;
    }
    if (status == 1 && (*(u32*)*(void**)((s32)pParty + 0x160) & 0xF0000) == 0) {
        *(void**)((s32)*(void**)((s32)pParty + 0x160) + 0x1E8) = 0;
    }
    marioChgMot(0x20);
    *(u8*)((s32)pParty + 0x39) = 10;
    partyChgPose(pParty, str_PYS_S_2_802cb148);
    marioChgPose(&str_M_Y_2_80421148);
    *(f32*)((s32)player + 0x2C8) = 0.0f;
    *(f32*)((s32)player + 0x2CC) = 0.0f;
    partyChgRunMode(pParty, 0xD);
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra != 0) {
        partyChgRunMode(extra, 0xD);
    }
    return 0;
}

