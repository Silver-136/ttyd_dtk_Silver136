#include "driver/camdrv.h"
#include "mario/mario_party.h"
#include "motion/mot_stay.h"
#include "party/party.h"
#include "party/party_motion.h"

/* Target .sdata2 catalog: one-element arrays force named SDA loads under MWCC. */
const f32 float_20_80423090[1] = {20.0f};
const f32 float_800_80423094[1] = {800.0f};
const f32 float_neg100_80423098[1] = {-100.0f};
const f32 float_700_8042309c[1] = {700.0f};
const f32 float_650_804230a0[1] = {650.0f};
const f32 float_200_804230a8[1] = {200.0f};
const f32 float_300_804230ac[1] = {300.0f};
const f32 float_100_804230b0[1] = {100.0f};
const f32 float_74_804230b4[1] = {74.0f};
const f32 float_50_804230b8[1] = {50.0f};
const f32 float_0_804230bc[1] = {0.0f};
const f32 float_40_804230c0[1] = {40.0f};
const f32 float_1000_804230c4[1] = {1000.0f};
const f32 float_neg1_804230c8[1] = {-1.0f};
const f32 float_neg2000_804230cc[1] = {-2000.0f};
const f32 float_neg0p5_804230d4[1] = {-0.5f};
const f32 float_2_804230d8[1] = {2.0f};
const f32 float_3_804230dc[1] = {3.0f};
const f32 float_45_804230e0[1] = {45.0f};
const f32 float_0p75_804230e4[1] = {0.75f};
const f32 float_1_804230ec[1] = {1.0f};
const f32 float_18_804230f0[1] = {18.0f};
const f32 float_8_804230f4[1] = {8.0f};
const f32 float_neg6_804230fc[1] = {-6.0f};
const f32 float_6_80423100[1] = {6.0f};
const f32 float_270_80423104[1] = {270.0f};
const f32 float_180_80423108[1] = {180.0f};
const f32 float_7_8042310c[1] = {7.0f};
const f32 float_neg3_80423110[1] = {-3.0f};
const f32 float_0p1_80423114[1] = {0.1f};
const f32 float_90_80423118[1] = {90.0f};
const f32 float_280_8042311c[1] = {280.0f};
const f32 float_260_80423120[1] = {260.0f};
const f32 float_315_80423124[1] = {315.0f};
const f32 float_135_80423128[1] = {135.0f};
const f32 float_225_8042312c[1] = {225.0f};
const f32 float_15_80423130[1] = {15.0f};
const f32 float_neg300_80423134[1] = {-300.0f};
const f32 float_240_80423138[1] = {240.0f};
const f32 float_neg1p4_8042313c[1] = {-1.4f};
const f32 float_neg16_80423140[1] = {-16.0f};
const f32 float_neg0p2_80423144[1] = {-0.2f};
const f32 float_neg2001_80423148[1] = {-2001.0f};
const f32 float_37_8042314c[1] = {37.0f};
const f32 float_1p3_80423150[1] = {1.3f};
const f32 float_2p25_80423154[1] = {2.25f};
const f32 float_1p5_8042316c[1] = {1.5f};
const f32 float_3p6_80423170[1] = {3.6f};
const f32 float_0p05_80423174[1] = {0.05f};
const f32 float_28_80423178[1] = {28.0f};
const f32 float_neg5_80423180[1] = {-5.0f};
const f32 float_70_80423184[1] = {70.0f};
const f32 float_150_80423188[1] = {150.0f};
const f32 float_0p2_80423194[1] = {0.2f};
const f32 float_18p5_80423198[1] = {18.5f};
const f32 float_neg15_8042319c[1] = {-15.0f};
const f32 float_neg1p5_804231a0[1] = {-1.5f};
const f32 float_31_804231a4[1] = {31.0f};

/* Preserve scalar-looking source while forcing accesses through the named arrays above. */
#define float_20_80423090 float_20_80423090[0]
#define float_800_80423094 float_800_80423094[0]
#define float_neg100_80423098 float_neg100_80423098[0]
#define float_700_8042309c float_700_8042309c[0]
#define float_650_804230a0 float_650_804230a0[0]
#define float_10_804230a4 10.0f
#define float_200_804230a8 float_200_804230a8[0]
#define float_300_804230ac float_300_804230ac[0]
#define float_100_804230b0 float_100_804230b0[0]
#define float_74_804230b4 float_74_804230b4[0]
#define float_50_804230b8 float_50_804230b8[0]
#define float_0_804230bc float_0_804230bc[0]
#define float_40_804230c0 float_40_804230c0[0]
#define float_1000_804230c4 float_1000_804230c4[0]
#define float_neg1_804230c8 float_neg1_804230c8[0]
#define float_neg2000_804230cc float_neg2000_804230cc[0]
#define float_0p5_804230d0 0.5f
#define float_neg0p5_804230d4 float_neg0p5_804230d4[0]
#define float_2_804230d8 float_2_804230d8[0]
#define float_3_804230dc float_3_804230dc[0]
#define float_45_804230e0 float_45_804230e0[0]
#define float_0p75_804230e4 float_0p75_804230e4[0]
#define float_5_804230e8 5.0f
#define float_1_804230ec float_1_804230ec[0]
#define float_18_804230f0 float_18_804230f0[0]
#define float_8_804230f4 float_8_804230f4[0]
#define float_30_804230f8 30.0f
#define float_neg6_804230fc float_neg6_804230fc[0]
#define float_6_80423100 float_6_80423100[0]
#define float_270_80423104 float_270_80423104[0]
#define float_180_80423108 float_180_80423108[0]
#define float_7_8042310c float_7_8042310c[0]
#define float_neg3_80423110 float_neg3_80423110[0]
#define float_0p1_80423114 float_0p1_80423114[0]
#define float_90_80423118 float_90_80423118[0]
#define float_280_8042311c float_280_8042311c[0]
#define float_260_80423120 float_260_80423120[0]
#define float_315_80423124 float_315_80423124[0]
#define float_135_80423128 float_135_80423128[0]
#define float_225_8042312c float_225_8042312c[0]
#define float_15_80423130 float_15_80423130[0]
#define float_neg300_80423134 float_neg300_80423134[0]
#define float_240_80423138 float_240_80423138[0]
#define float_neg1p4_8042313c float_neg1p4_8042313c[0]
#define float_neg16_80423140 float_neg16_80423140[0]
#define float_neg0p2_80423144 float_neg0p2_80423144[0]
#define float_neg2001_80423148 float_neg2001_80423148[0]
#define float_37_8042314c float_37_8042314c[0]
#define float_1p3_80423150 float_1p3_80423150[0]
#define float_2p25_80423154 float_2p25_80423154[0]
#define float_1p5_8042316c float_1p5_8042316c[0]
#define float_3p6_80423170 float_3p6_80423170[0]
#define float_0p05_80423174 float_0p05_80423174[0]
#define float_28_80423178 float_28_80423178[0]
#define float_4_8042317c 4.0f
#define float_neg5_80423180 float_neg5_80423180[0]
#define float_70_80423184 float_70_80423184[0]
#define float_150_80423188 float_150_80423188[0]
#define float_11_80423190 11.0f
#define float_0p2_80423194 float_0p2_80423194[0]
#define float_18p5_80423198 float_18p5_80423198[0]
#define float_neg15_8042319c float_neg15_8042319c[0]
#define float_neg1p5_804231a0 float_neg1p5_804231a0[0]
#define float_31_804231a4 float_31_804231a4[0]


extern f32 float_60_8042318c;
extern f32 unk_8041ec08;
extern s32 unk_8041ec0c;
f32 touch_wall_ct;

f32 revise360(f32 angle);
void partyPaperOff(void* party);
void partyChgPoseId(void* party, s32 poseId);
void partyChgMot(void* party, s32 mot);
extern void partyClearFootmark(void);
extern void partyClearFootmark2(void);
extern s32 marioKeyOffChk(void);
extern void partyChgRunMode(void* party, s32 mode);
extern void* gp;
extern void* partyGetFootmarkPtr(s32 id);
extern void unk_800cbfbc(f32 speed, void* party);
extern f32 partyGetTargetDistY(s32 id);
extern f32 partyGetTargetMovespd2(void* party);
extern f32 vec3_802f4628[];
extern void partyPaperOn(void* party, char* name);
extern void partyChgPaper(void* party, char* name);

#define PARTY_CAN_FORCE_SLIT_OFF(party) \
    (((*(u32*)(party) & 0x1000) != 0) || ((*(u32*)((s32)(party) + 4) & 0x100) != 0))

#define PARTY_FORCE_SLIT_OFF_BODY(party) \
    do { \
        *(f32*)((s32)(party) + 0x10C) = \
            revise360(*(f32*)((s32)(party) + 0x100) - *(f32*)((s32)(party) + 0xF8)) >= float_180_80423108 \
                ? float_0_804230bc \
                : float_180_80423108; \
        { \
            f32 dir270 = float_270_80423104; \
        *(f32*)((s32)(party) + 0x110) = *(f32*)((s32)(party) + 0x10C); \
            *(f32*)((s32)(party) + 0xFC) = revise360(dir270 - *(f32*)((s32)(party) + 0x10C)); \
        } \
        *(f32*)((s32)(party) + 0x100) = *(f32*)((s32)(party) + 0xFC); \
        *(u32*)((s32)(party) + 4) &= ~0x3C0; \
        partyPaperOff(party); \
        *(u32*)(party) &= ~0x3000; \
        *(u8*)((s32)(party) + 0x37) = 0; \
        partyChgPoseId(party, 1); \
    } while (0)

void unk_80151d20(void) {
    touch_wall_ct = float_60_8042318c;
}

void N_partyFollowCloseOn(void* party) {
    *(u32*)((s32)party + 8) |= 0x20000;
}

void N_partyFollowCloseOff(void* party) {
    *(u32*)((s32)party + 8) &= ~0x20000;
}

u8 party_motion_stay(s32 pParty) {
    extern s32 partyGetFootmarkId(void*);
    extern void partyChgPoseId(void*, s32);
    extern void partyChgMot(void*, s32);
    extern void unk_800cbf84(f32, void*);
    extern s32 marioChkSlitThrouh(void);
    extern f32 distABf(f32, f32, f32, f32);
    extern void partyGetMoveDirSpd(void*, f32*, f32*);
    extern void* partySearchFrontWall(f32, f32, void*, void*);
    extern s32 party_slit(void*);
    extern s32 party_roll(void*);
    extern void unk_800cbc30(void*);
    extern void unk_800c27c0(f32, void*, void*);
    extern void unk_80150fac(void*);
    extern void unk_801512e0(void*);
    extern void party_force_reset_outofscreen(void*);
    extern void partyMoveBehindMario(void*, s32);
    extern s32 strcmp(const char*, const char*);
    extern s32 marioKeyOffChk(void);
    extern s32 chuchu_searchObject(void);
    extern void partyChgPose(void*, char*);
    extern void* gp;
    extern f32 vec3_802f4628[];
    void* party = (void*)pParty;
    void* player = *(void**)(pParty + 0x160);
    f32 pos[3];
    f32 dir;
    f32 speed;
    f32 follow = *(f32*)(pParty + 0x40);

    partyGetFootmarkId(party);
    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        partyChgPoseId(party, 1);
        if ((*(u32*)(pParty + 8) & 0x10000) == 0) {
            if ((*(u32*)pParty & 0x1000) == 0) partyClearFootmark();
            else partyClearFootmark2();
        }
        *(u32*)(pParty + 8) &= ~0x10000;
        *(s32*)(pParty + 0x168) = -1;
        unk_800cbf84(float_0_804230bc, party);
    }
    if (*(u16*)((s32)player + 0x2E) != 0xE &&
        (*(u32*)party & 0x80000) != 0) {
        *(u32*)party &= ~0x80000;
    }
    if (*(f32*)((s32)player + 0x1B0) == float_0_804230bc && *(f32*)((s32)player + 0x1B4) < float_1_804230ec)
        follow = float_100_804230b0;
    if (*(u8*)((s32)player + 0x2F) == 2) follow = float_70_80423184;
    if ((strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            follow = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5 || *(s8*)(pParty + 0x31) == 6) {
            follow = (*(u32*)(pParty + 8) & 0x00200000) ? float_8_804230f4 : float_30_804230f8;
        }
        *(u32*)(pParty + 8) |= 0x00020000;
    }
    if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            follow = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5 || *(s8*)(pParty + 0x31) == 6) {
            follow = (*(u32*)(pParty + 8) & 0x00200000) ? float_8_804230f4 : float_30_804230f8;
        }
    }
    party_slit(party);
    party_roll(party);
    unk_800cbc30(party);
    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }
    if (*(u8*)(pParty + 0x34) != 0 && marioChkSlitThrouh() != 0 && marioChkDeepSleep() == 0)
        partyChgMot(party, 1);
    if (*(u8*)(pParty + 0x34) != 0 &&
        distABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60),
                *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) >= follow)
        partyChgMot(party, 1);
    pos[0] = *(f32*)(pParty + 0x58);
    pos[1] = *(f32*)(pParty + 0x5C);
    pos[2] = *(f32*)(pParty + 0x60);
    partyGetMoveDirSpd(party, &dir, &speed);
    {
        void* wall = partySearchFrontWall(speed, dir, party, pos);
        if (wall != 0) {
            *(void**)(pParty + 0x13C) = wall;
            if (*(f32*)(pParty + 0x130) != float_0_804230bc) {
                *(void**)(pParty + 0x140) = wall;
            }
        }
    }
    *(f32*)(pParty + 0x58) = pos[0];
    *(f32*)(pParty + 0x60) = pos[2];
    partyGetMoveDirSpd(party, &dir, &speed);
    unk_800c27c0(speed, party, pos);
    *(f32*)(pParty + 0x58) = pos[0];
    *(f32*)(pParty + 0x60) = pos[2];
    if (*(s8*)(pParty + 0x4C) != 0) {
        unk_80150fac(party);
    } else {
        unk_801512e0(party);
        party_force_reset_outofscreen(party);
        if (*(u8*)(pParty + 0x31) == 7 &&
            (*(u32*)(pParty + 4) & 0x8000) == 0 &&
            marioKeyOffChk() == 0 && marioChkDeepSleep() != 1 &&
            chuchu_searchObject() != 0) {
            u8 motion = *(u8*)(pParty + 0x39);
            if (motion == 1) {
                s32 timer = *(s32*)(pParty + 0x24) - 1;
                *(s32*)(pParty + 0x24) = timer;
                if (timer < 1) {
                    partyChgPoseId(party, 1);
                    *(s32*)(pParty + 0x24) = 30;
                    *(u8*)(pParty + 0x39) = 2;
                }
            } else if (motion == 0 || motion > 2) {
                partyChgPose(party, (char*)vec3_802f4628 + 0xCC);
                *(u8*)(pParty + 0x39) = 1;
                *(s32*)(pParty + 0x24) = 30;
            } else {
                s32 timer = *(s32*)(pParty + 0x24) - 1;
                *(s32*)(pParty + 0x24) = timer;
                if (timer < 1) {
                    *(u8*)(pParty + 0x39) = 0;
                }
            }
        }
    }
    return 0;
}

/* fallback stub-fill: map=unk_8015146c addr=0x8015146c size=0x000001e0 */
void unk_8015146c(void* party) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void party_slit(void* party);
    extern void party_roll(void* party);
    extern void unk_800cbc30(void* party);
    extern void partyChkWall(void* party);
    extern void partyChkGnd(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyMoveBehindMario(void* party, s32 arg);
    extern void unk_80150fac(void* party);
    extern void unk_801512e0(void* party);
    extern s32 chuchu_searchObject(void);
    extern void partyChgPose(void* party, char* name);
    extern char str_PCH_A2_1_802f46f4[];
    void* player;
    s32 timer;
    u8 state;

    player = *(void**)((s32)party + 0x160);
    if ((*(u32*)((s32)party + 8) & 2) != 0) {
        *(u32*)((s32)party + 8) &= ~2;
        partyChgPoseId(party, 1);
        if ((*(u32*)party & 8) == 0) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
        *(s32*)((s32)party + 0x168) = -1;
    }
    party_slit(party);
    party_roll(party);
    unk_800cbc30(party);
    if ((*(u32*)party & 0x01000000) == 0) {
        partyChkWall(party);
    }
    partyChkGnd(party);
    if (distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) > float_31_804231a4) {
        partyChgRunMode(party, 2);
    }
    if (*(u16*)((s32)player + 0x2E) == 0xE && (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
    } else if (*(u8*)((s32)party + 0x32) == 1) {
        unk_80150fac(party);
    } else {
        unk_801512e0(party);
        if (*(u8*)((s32)party + 0x31) == 7 && (*(u32*)((s32)party + 4) & 0x8000) == 0 && marioKeyOffChk() == 0 && marioChkDeepSleep() != 1 && chuchu_searchObject() != 0) {
            state = *(u8*)((s32)party + 0x39);
            if (state == 1) {
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    partyChgPoseId(party, 1);
                    *(s32*)((s32)party + 0x24) = 0x1E;
                    *(u8*)((s32)party + 0x39) = 2;
                }
            } else if (state < 3 && state != 0) {
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    *(u8*)((s32)party + 0x39) = 0;
                }
            } else {
                partyChgPose(party, str_PCH_A2_1_802f46f4);
                *(u8*)((s32)party + 0x39) = 1;
                *(s32*)((s32)party + 0x24) = 0x1E;
            }
        }
    }
}
/* fallback stub-fill: map=unk_801512e0 addr=0x801512e0 size=0x0000018c */
void unk_801512e0(void* pParty) {
    extern void* partySearchHead(void* party, void* pos, f32* height, f32 dir);
    extern void* partySearchGround(void* party, f32 yOffHi, f32 yOffLo);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern s32 unk_800cb9bc(void* hit);
    extern void partyGetAppearPos5(void* party, void* hit, void* pos);
    f32 pos[3];
    f32 height;
    void* hit;
    u32 attr;

    pos[0] = *(f32*)((s32)pParty + 0x58);
    pos[1] = *(f32*)((s32)pParty + 0x5C);
    pos[2] = *(f32*)((s32)pParty + 0x60);
    height = *(f32*)((s32)pParty + 0xF0);
    hit = partySearchHead(pParty, pos, &height, *(f32*)((s32)pParty + 0x10C));
    if (hit != 0) {
        *(void**)((s32)pParty + 0x140) = hit;
    }
    if ((*(u32*)pParty & 0x04000000) == 0) {
        *(u32*)pParty &= ~0x100;
        hit = partySearchGround(pParty, float_15_80423130, float_neg15_8042319c);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if ((attr & 0x200) != 0) {
                hit = 0;
                if (*(s8*)((s32)pParty + 0x4C) == 0) {
                    *(u32*)((s32)pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
            }
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            *(u32*)pParty &= ~4;
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1p5_804231a0;
        } else {
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
            *(u32*)pParty |= 2;
            if ((attr & 0x100) != 0) {
                *(u32*)pParty |= 0x80;
            } else {
                *(u32*)pParty &= ~0x100;
            }
            if ((attr & 0x40000000) != 0) {
                *(void**)((s32)pParty + 0x14C) = hit;
            }
            if (unk_800cb9bc(hit) != 0) {
                partyGetAppearPos5(pParty, hit, (void*)((s32)pParty + 0x94));
                *(u32*)((s32)pParty + 8) |= 0x00400000;
                *(f32*)((s32)pParty + 0x98) = *(f32*)((s32)pParty + 0x5C);
                partyChgMot(pParty, 2);
            }
        }
    }
}


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_motion 20260624_191429 */

/* fallback stub-fill: map=unk_80150fac addr=0x80150fac size=0x00000334 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_80150fac(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 strcmp(const char* s1, const char* s2);
    extern void* partySearchHead(f32 dir, void* party, VecLocal* pos, f32* height);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern char str_las_10_802f47c8[];

    void* player = *(void**)((s32)pParty + 0x160);
    void* hit;
    u32 attr;
    s32 adjust;
    VecLocal pos;
    f32 height;
    f32 playerY;
    f32 groundY;
    f32 baseY;
    f32 partyY;
    f32 rate;
    f32 offset;
    f32 diff;

    if ((*(u32*)pParty & 0x04000000) != 0) {
        return;
    }

    if (strcmp((const char*)((s32)gp + 0x12C), str_las_10_802f47c8) == 0) {
        pos.x = *(f32*)((s32)pParty + 0x58);
        pos.y = *(f32*)((s32)pParty + 0x5C);
        pos.z = *(f32*)((s32)pParty + 0x60);
        height = *(f32*)((s32)pParty + 0xF0);
        hit = partySearchHead(*(f32*)((s32)pParty + 0x10C), pParty, &pos, &height);
        if (hit != 0) {
            *(void**)((s32)pParty + 0x140) = hit;
            diff = (pos.y - *(f32*)((s32)pParty + 0xF0)) - *(f32*)((s32)pParty + 0x5C);
            if (diff < float_neg5_80423180 || diff >= float_0_804230bc) {
                *(f32*)((s32)pParty + 0x5C) += diff * float_0p1_80423114;
            }
        }
    }

    hit = partySearchGround(float_10_804230a4, float_0_804230bc, pParty);
    if (hit != 0) {
        attr = hitGetAttr(hit);
        if ((attr & 0x800) != 0) {
            partyChgRunMode(pParty, 0xD);
            return;
        }
        if ((attr & 0x100) != 0) {
            *(u32*)pParty |= 0x80;
        } else {
            *(u32*)pParty &= ~0x80;
        }
    }

    if ((*(s16*)((s32)player + 0x50) == 0) ||
        (*(f32*)((s32)player + 0x180) != float_0_804230bc)) {
        switch (*(u16*)((s32)player + 0x2E)) {
            case 0x1F:
            case 0x20:
            case 0x10:
            case 0x11:
                adjust = 0;
                break;
            default:
                adjust = 1;
                break;
        }
    } else {
        adjust = 0;
    }

    if (adjust != 0) {
        if (hit == 0 && *(f32*)((s32)pParty + 0xE8) < float_0_804230bc) {
            return;
        }
        playerY = *(f32*)((s32)player + 0x90);
        groundY = *(f32*)((s32)pParty + 0xE4);
        baseY = groundY;
        if (playerY > groundY) {
            baseY = playerY;
        }
        partyY = *(f32*)((s32)pParty + 0x5C);
        rate = float_0p1_80423114;
        if (partyY < groundY) {
            baseY = groundY;
            rate = float_0p2_80423194;
        }
        offset = float_5_804230e8;
        if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
            offset = float_1_804230ec;
        }
        diff = (baseY + offset) - partyY;
        if (diff < float_neg5_80423180 || diff >= float_0_804230bc) {
            *(f32*)((s32)pParty + 0x5C) += diff * rate;
        }
    } else if (hit != 0) {
        playerY = *(f32*)((s32)player + 0x90);
        groundY = *(f32*)((s32)pParty + 0xE4);
        baseY = groundY;
        if (playerY > groundY) {
            baseY = playerY;
        }
        partyY = *(f32*)((s32)pParty + 0x5C);
        rate = float_0p05_80423174;
        if (partyY < groundY) {
            baseY = groundY + float_18p5_80423198;
            rate = float_0p2_80423194;
        }
        offset = float_5_804230e8;
        if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
            offset = float_1_804230ec;
        }
        diff = (baseY + offset) - partyY;
        if (diff < float_neg5_80423180 || diff >= float_0_804230bc) {
            *(f32*)((s32)pParty + 0x5C) += diff * rate;
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset



u8 party_motion_homing_walk(s32 pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct FootmarkLocal {
        u8 flags;
        u8 pad[3];
        VecLocal position;
    } FootmarkLocal;
    typedef struct HitWorkLocal {
        u8 pad[0xC];
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWorkLocal;

    extern s32 partyGetFootmarkId(void* party);
    extern s32 party_slit(void* party);
    extern s32 party_roll(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyMoveBehindMario(void* party, s32 mode);
    extern void partyChgMotSlitFloor(void* party);
    extern s32 strcmp(const char*, const char*);
    extern f32 PSVECDistance(void* a, void* b);
    extern s32 nokonokoGetStatus(void* party);
    extern void partyNextReadId(s32 id);
    extern void partyGetReadId2(s32 id, void* outPos);
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* speed);
    extern void* partySearchFrontWall(f32 speed, f32 dir, void* party, VecLocal* pos);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern void* partySearchHead(f32 dir, void* party, VecLocal* pos, f32* height);
    extern u32 getFrontFloor(f32 angle, void* party, f32* outPos);
    extern f32 revise360(f32 angle);
    extern void partyGetAppearPos4(void* party, VecLocal* pos);
    extern void partyGetAppearPos5(void* party, void* hit, void* pos);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 unk_800ccb34(f32 y, s32 footmarkId);
    extern f32 partyGetTargetDir(f32 x, f32 z, s32 footmarkId);
    extern void partyChkFrontStep(f32 height, void* party,
                                  f32* stepY, f32* out2, f32* out3);
    extern s32 unk_800cb9bc(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void party_force_reset_outofscreen(void* party);
    extern f32 partyGetTargetDist3D(s32 id, void* pos);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void sincosf(f32 angle, f32* outSin, f32* outCos);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern u32 marioGetSlitFloorPos(void* outPos);
    extern s32 partyGetFootmarkWriteId(s32 id);
    extern s32 partyGetFootmarkReadId(s32 id);

    void* party;
    void* player;
    void* leadParty;
    void* hit;
    FootmarkLocal* footmark;
    HitWorkLocal probe;
    VecLocal target;
    VecLocal current;
    VecLocal slitPos;
    f32 closeDist;
    f32 targetDist;
    f32 speed;
    f32 distance;
    f32 distance3D;
    f32 angle;
    f32 extraSpeed;
    f32 radius;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 sinValue;
    f32 cosValue;
    f32 moveDir;
    f32 moveSpeed;
    f32 frontDiff;
    f32 wallMetric;
    f32 headHeight;
    f32 groundY;
    f32 stepY;
    f32 stepOut2;
    f32 stepOut3;
    VecLocal moveVec;
    VecLocal frontFloor;
    VecLocal headPos;
    void* ground;
    void* wall;
    void* headHit;
    u32 attr;
    s32 footmarkId;
    s32 leadId;
    s32 status;
    s32 i;
    s32 readId;
    s32 writeId;
    u16 motion;

    party = (void*)pParty;
    footmarkId = partyGetFootmarkId(party);
    player = *(void**)(pParty + 0x160);

    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        *(u32*)pParty &= ~0x80000;
    }

    if (*(s16*)(pParty + 0x28) != 0) {
        closeDist = float_28_80423178;
    } else {
        closeDist = float_45_804230e0;
    }

    if ((strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            closeDist = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                closeDist = float_30_804230f8;
            } else {
                closeDist = float_8_804230f4;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                closeDist = float_30_804230f8;
            } else {
                closeDist = float_8_804230f4;
            }
        }
        *(u32*)(pParty + 8) |= 0x00020000;
    }

    targetDist = float_45_804230e0;
    if (partyGetTargetDistY(footmarkId) != float_0_804230bc) {
        targetDist = float_50_804230b8;
    }
    if (*(s8*)((s32)player + 0x3C) == 2) {
        targetDist = float_70_80423184;
    }

    if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            targetDist = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                targetDist = float_30_804230f8;
            } else {
                targetDist = float_8_804230f4;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                targetDist = float_30_804230f8;
            } else {
                targetDist = float_8_804230f4;
            }
        }
    }

    if (party_slit(party) == 0 || party_roll(party) == 0) {
        return 0;
    }

    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }

    if (*(s16*)((s32)player + 0x50) != 0 &&
        *(f32*)((s32)player + 0x180) == float_0_804230bc) {
        partyChgMot(party, 0);
        return 0;
    }

    motion = *(u16*)((s32)player + 0x2E);
    if (motion == 0x1F || motion == 0x20 ||
        motion == 0x10 || motion == 0x11) {
        partyChgMot(party, 0);
        return 0;
    }

    speed = partyGetTargetMovespd2(party);

    if ((*(u32*)party & 0x8) == 0) {
        distance = PSVECDistance(
            (void*)((s32)player + 0x8C),
            (void*)(pParty + 0x58));

        if (distance >= float_45_804230e0) {
            if (distance >= float_50_804230b8) {
                speed = float_3_804230dc;
            }
        } else {
            speed = float_1_804230ec;
        }

        if ((*(u32*)player & 0x00008000) != 0) {
            speed = float_1_804230ec;
            closeDist = float_100_804230b0;
        }

        unk_800cbfbc(speed, party);
    } else {
        leadId = marioGetPartyId();
        leadParty = partyGetPtr(leadId);

        if (leadParty != 0) {
            status = nokonokoGetStatus(leadParty);

            if (status == 3 ||
                PSVECDistance(
                    (void*)((s32)player + 0x8C),
                    (void*)((s32)leadParty + 0x58)) >= float_150_80423188) {
                distance = PSVECDistance(
                    (void*)((s32)player + 0x8C),
                    (void*)(pParty + 0x58));
            } else {
                distance = PSVECDistance(
                    (void*)((s32)leadParty + 0x58),
                    (void*)(pParty + 0x58));
            }

            if (distance >= float_45_804230e0) {
                if (distance >= float_50_804230b8) {
                    speed = float_3_804230dc;
                }
            } else {
                speed = float_1_804230ec;
            }

            if ((*(u32*)player & 0x00008000) != 0) {
                speed = float_1_804230ec;
            }

            unk_800cbfbc(speed, party);
        }
    }

    startX = *(f32*)(pParty + 0x58);
    startY = *(f32*)(pParty + 0x5C);
    startZ = *(f32*)(pParty + 0x60);

    footmark = (FootmarkLocal*)partyGetFootmarkPtr(footmarkId);
    target.x = footmark->position.x;
    target.y = footmark->position.y;
    target.z = footmark->position.z;

    /*
     * The target checks up to eleven footmarks, skipping candidates whose
     * short vertical probe lands on a 0x800 floor attribute.
     */
    for (i = 0; i < 11; i++) {
        angle = angleABf(startX, startZ, target.x, target.z);
        sincosf(angle, &sinValue, &cosValue);

        probe.start.x = *(f32*)(pParty + 0x58) + sinValue * float_20_80423090;
        probe.start.y = *(f32*)(pParty + 0x5C) + float_20_80423090;
        probe.start.z = *(f32*)(pParty + 0x60) + cosValue * float_20_80423090;
        probe.dir.x = float_0_804230bc;
        probe.dir.y = float_neg1_804230c8;
        probe.dir.z = float_0_804230bc;
        probe.dist = float_40_804230c0;

        hit = (void*)hitCheckVecFilter(&probe, 0);
        if (hit == 0 || (hitGetAttr(hit) & 0x800) == 0) {
            break;
        }

        partyNextReadId(footmarkId);
        partyGetReadId2(footmarkId, &target);
        footmark = (FootmarkLocal*)partyGetFootmarkPtr(footmarkId);
    }

    current.x = *(f32*)(pParty + 0x58);
    current.y = *(f32*)(pParty + 0x5C);
    current.z = *(f32*)(pParty + 0x60);

    distance3D = partyGetTargetDist3D(footmarkId, &current);
    if (distance3D >= closeDist) {
        unk_800cbfbc(speed, party);
    }
    if (targetDist < float_20_80423090) {
        unk_800cbfbc(speed, party);
    }

    for (i = 0; i < 12; i++) {
        distance = distABf(startX, startZ, target.x, target.z);
        angle = angleABf(startX, startZ, target.x, target.z);

        if ((*(u32*)player & 0x00008000) == 0) {
            radius = *(f32*)(pParty + 0xF4) * float_0p5_804230d0 +
                     *(f32*)(pParty + 0x104);
        } else {
            /*
             * The target adds the player's collision width here.  Width alone
             * is a conservative first reconstruction until the Player field is
             * promoted from typed context.
             */
            radius = *(f32*)(pParty + 0xF4);
        }

        if (distance > radius) {
            current.x = *(f32*)(pParty + 0x58);
            current.y = *(f32*)(pParty + 0x5C);
            current.z = *(f32*)(pParty + 0x60);

            distance3D = partyGetTargetDist3D(footmarkId, &current);

            if (distance3D >= closeDist) {
                if ((*(u32*)(pParty + 8) & 0x00020000) == 0) {
                    extraSpeed = (distance3D - closeDist) * float_0p1_80423114;
                } else {
                    extraSpeed = (distance3D - targetDist) * float_0p1_80423114;
                }

                if (extraSpeed > float_4_8042317c) {
                    extraSpeed = float_4_8042317c;
                }
                if (extraSpeed < float_0p5_804230d0) {
                    extraSpeed = float_0_804230bc;
                }

                unk_800cbf84(
                    *(f32*)((s32)player + 0x180) + extraSpeed,
                    party);
                goto common_motion;
            }

            if (marioGetSlitFloorPos(&slitPos) != 0) {
                unk_800cbfbc(float_1p5_8042316c, party);
                partyNextReadId(footmarkId);

                if (marioGetSlitFloorPos(&slitPos) != 0) {
                    target = slitPos;
                    closeDist = distABf(
                        startX, startZ, slitPos.x, slitPos.z);
                    angle = angleABf(
                        startX, startZ, target.x, target.z);

                    if (closeDist <= float_5_804230e8) {
                        partyChgMotSlitFloor(party);
                    }
                }

                goto common_motion;
            }

            if (distance > targetDist) {
                if ((*(u32*)(pParty + 8) & 0x00020000) == 0) {
                    extraSpeed = distance - targetDist;

                    if (extraSpeed >= float_1_804230ec) {
                        extraSpeed = float_1_804230ec;
                        if (targetDist < float_20_80423090) {
                            extraSpeed = speed;
                        }
                    }

                    unk_800cbfbc(extraSpeed, party);
                } else {
                    extraSpeed = (distance - targetDist) * 0.1f;

                    if (extraSpeed > 4.0f) {
                        extraSpeed = 4.0f;
                    }
                    if (extraSpeed < 0.5f) {
                        extraSpeed = 0.0f;
                    }

                    unk_800cbf84(
                        *(f32*)((s32)player + 0x180) + extraSpeed,
                        party);
                }

                goto common_motion;
            }

            if (*(f32*)(pParty + 0xA0) == 0.0f &&
                *(f32*)(pParty + 0xA8) == 0.0f) {
                unk_800cbfbc(
                    *(f32*)((s32)player + 0x180),
                    party);
            } else {
                unk_800cbf84(0.0f, party);
                *(f32*)(pParty + 0x100) =
                    angleABf(
                        *(f32*)(pParty + 0x58),
                        *(f32*)(pParty + 0x60),
                        *(f32*)((s32)player + 0x8C),
                        *(f32*)((s32)player + 0x94));
                partyChgPoseId(party, 1);
            }
        }

        writeId = partyGetFootmarkWriteId(footmarkId);
        readId = partyGetFootmarkReadId(footmarkId);

        if (readId == writeId) {
            unk_800cbfbc(
                *(f32*)((s32)player + 0x180),
                party);

            if (*(f32*)((s32)player + 0x180) == 0.0f ||
                *(f32*)(pParty + 0xA0) != 0.0f ||
                *(f32*)(pParty + 0xA8) != 0.0f) {
                angle = *(f32*)(pParty + 0x100);
                unk_800cbfbc(0.0f, party);
                *(f32*)(pParty + 0x11C) = float_0_804230bc;

                if ((*(u32*)pParty & 0x200) == 0) {
                    partyChgPoseId(party, 1);
                }

                partyChgMot(party, 0);
            }

            goto common_motion;
        }

        partyNextReadId(footmarkId);
        partyGetReadId2(footmarkId, &target);

        ground = 0;
        if ((*(u32*)pParty & 0x04000000) == 0) {
            *(u32*)pParty &= ~0x80;
            ground = partySearchGround(10.0f, 0.0f, party);

            if (ground != 0 && (hitGetAttr(ground) & 0x200) != 0) {
                ground = 0;
                if (*(s8*)(pParty + 0x4C) == 0) {
                    *(u32*)(pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
            }

            *(void**)(pParty + 0x138) = ground;

            if (ground == 0) {
                partyChgMot(party, 3);
                *(f32*)(pParty + 0x11C) = float_neg1_804230c8;
            } else {
                attr = hitGetAttr(ground);

                if ((attr & 0x100) != 0) {
                    *(u32*)pParty |= 0x80;
                } else {
                    *(u32*)pParty &= ~0x80;
                }

                if ((attr & 0x800) != 0) {
                    partyChgMot(party, 0);
                    goto common_motion;
                }
            }
        }

        if (unk_800cb9bc(ground) != 0) {
            partyGetAppearPos5(
                party, ground, (void*)(pParty + 0x94));
            *(u32*)(pParty + 8) |= 0x00400000;
            partyChgMot(party, 2);
            goto common_motion;
        }

        footmark = (FootmarkLocal*)partyGetFootmarkPtr(footmarkId);

        if ((*(u32*)pParty & 0x20) != 0 &&
            (footmark->flags & 4) != 0) {
            distance = distABf(
                startX, startZ, target.x, target.z);
            if (distance <= float_5_804230e8) {
                partyChgMotSlitFloor(party);
            }
            goto common_motion;
        }
    }

common_motion:
    if (*(f32*)(pParty + 0x104) == float_0_804230bc) {
        if ((footmark->flags & 4) == 0) {
            partyChgMot(party, 0);
        }
    } else {
        unk_800cbb10(*(f32*)(pParty + 0x104), party);
    }

    *(f32*)(pParty + 0x100) = angle;

    moveVec.x = *(f32*)(pParty + 0x58);
    moveVec.y = *(f32*)(pParty + 0x5C);
    moveVec.z = *(f32*)(pParty + 0x60);

    partyGetMoveDirSpd(party, &moveDir, &moveSpeed);
    wall = partySearchFrontWall(
        moveSpeed, moveDir, party, &moveVec);

    if ((*(u32*)pParty & 0x01000000) != 0) {
        wall = 0;
    }

    if (wall == 0) {
        *(u8*)(pParty + 0x4D) = 0;

        if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
            getFrontFloor(
                *(f32*)(pParty + 0x100),
                party,
                (f32*)&frontFloor);
        } else {
            *(f32*)(pParty + 0x54) =
                revise360(
                    float_270_80423104 +
                    *(f32*)((s32)player + 0x19C));
            partyGetAppearPos4(party, &frontFloor);
        }

        frontDiff = frontFloor.y - *(f32*)(pParty + 0x5C);

        if (frontFloor.y > *(f32*)(pParty + 0x5C)) {
            if ((double)frontDiff > 1.001 &&
                frontDiff < 60.0f) {
                *(f32*)(pParty + 0x94) = frontFloor.x;
                *(f32*)(pParty + 0x98) = frontFloor.y;
                *(f32*)(pParty + 0x9C) = frontFloor.z;
                partyChgMot(party, 2);
                return 0;
            }

            if (frontFloor.y > *(f32*)(pParty + 0x5C)) {
                *(f32*)(pParty + 0x5C) = frontFloor.y;
            }
        }

        if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
            sincosf(
                *(f32*)(pParty + 0x100),
                &sinValue,
                &cosValue);

            probe.start.x =
                *(f32*)(pParty + 0x58) +
                sinValue * float_40_804230c0;
            probe.start.y =
                *(f32*)(pParty + 0x5C) + float_40_804230c0;
            probe.start.z =
                *(f32*)(pParty + 0x60) +
                cosValue * float_40_804230c0;

            probe.dir.x = float_0_804230bc;
            probe.dir.y = float_neg1_804230c8;
            probe.dir.z = float_0_804230bc;
            probe.dist = float_100_804230b0;

            hit = (void*)hitCheckVecFilter(&probe, 0);

            if (hit == 0 ||
                (hitGetAttr(hit) & 0x200) == 0) {
                moveVec.x = *(f32*)(pParty + 0x58);
                moveVec.z = *(f32*)(pParty + 0x60);

                movePos(
                    *(f32*)(pParty + 0x104),
                    *(f32*)(pParty + 0x100),
                    &moveVec.x,
                    &moveVec.z);
            }
        }
    } else {
        *(void**)(pParty + 0x12C) = wall;

        if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
            *(void**)(pParty + 0x134) = wall;
            attr = hitGetAttr(wall);

            if ((attr & 0x2000) != 0) {
                if (*(u8*)(pParty + 0x37) == 1) {
                    if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
                        moveVec.x = *(f32*)(pParty + 0x58);
                        moveVec.z = *(f32*)(pParty + 0x60);
                        movePos(
                            *(f32*)(pParty + 0x104),
                            *(f32*)(pParty + 0x100),
                            &moveVec.x,
                            &moveVec.z);
                    }
                } else if ((*(u32*)((s32)player + 0xC) &
                            0x80000000) == 0) {
                    *(u32*)(pParty + 8) |= 1;
                    *(u8*)(pParty + 0x37) = 1;
                }
            } else {
                *(u8*)(pParty + 0x4D) =
                    *(u8*)(pParty + 0x4D) + 1;

                if (*(s8*)(pParty + 0x4D) > 8) {
                    *(u8*)(pParty + 0x4D) = 0;

                    wallMetric = unk_800ccb34(
                        *(f32*)(pParty + 0x5C),
                        footmarkId);

                    if (wallMetric < float_1_804230ec) {
                        angle = partyGetTargetDir(
                            *(f32*)(pParty + 0x58),
                            *(f32*)(pParty + 0x60),
                            footmarkId);

                        getFrontFloor(
                            angle, party, (f32*)&frontFloor);

                        frontDiff =
                            frontFloor.y -
                            *(f32*)(pParty + 0x5C);

                        if (frontDiff > float_100_804230b0) {
                            *(f32*)(pParty + 0x58) =
                                frontFloor.x;
                            *(f32*)(pParty + 0x5C) =
                                frontFloor.y;
                            *(f32*)(pParty + 0x60) =
                                frontFloor.z;
                            partyChgMot(party, 1);
                        } else if (frontDiff >= float_0_804230bc) {
                            *(f32*)(pParty + 0x94) =
                                frontFloor.x;
                            *(f32*)(pParty + 0x98) =
                                frontFloor.y;
                            *(f32*)(pParty + 0x9C) =
                                frontFloor.z;
                            partyChgMot(party, 2);
                        } else {
                            partyChgMot(party, 0);
                        }
                    } else {
                        getFrontFloor(
                            *(f32*)(pParty + 0x100),
                            party,
                            (f32*)&frontFloor);

                        frontDiff =
                            frontFloor.y -
                            *(f32*)(pParty + 0x5C);

                        if (frontDiff > float_5_804230e8) {
                            *(f32*)(pParty + 0x94) =
                                frontFloor.x;
                            *(f32*)(pParty + 0x98) =
                                frontFloor.y;
                            *(f32*)(pParty + 0x9C) =
                                frontFloor.z;
                            partyChgMot(party, 2);
                            return 0;
                        }

                        if ((*(u32*)pParty & 0x30) == 0) {
                            partyChkFrontStep(
                                11.0f, party,
                                &stepY, &stepOut2, &stepOut3);

                            if (stepY -
                                    *(f32*)(pParty + 0x5C) >=
                                    5.0f &&
                                *(f32*)(pParty + 0xE0) <=
                                    30.0f &&
                                *(f32*)(pParty + 0x104) !=
                                    0.0f) {
                                *(f32*)(pParty + 0x120) =
                                    stepY;
                                partyChgMot(party, 6);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    *(f32*)(pParty + 0x58) = moveVec.x;
    *(f32*)(pParty + 0x60) = moveVec.z;

    headPos.x = *(f32*)(pParty + 0x58);
    headPos.y = *(f32*)(pParty + 0x5C);
    headPos.z = *(f32*)(pParty + 0x60);
    headHeight = *(f32*)(pParty + 0xF0);

    headHit = partySearchHead(
        *(f32*)(pParty + 0x10C),
        party,
        &headPos,
        &headHeight);

    if (headHit != 0) {
        *(void**)(pParty + 0x140) = headHit;
    }

    *(u32*)pParty &= ~0x80;

    ground = partySearchGround(
        10.0f, 10.0f, party);
    groundY = *(f32*)(pParty + 0xE4);

    if (ground != 0) {
        attr = hitGetAttr(ground);

        if ((attr & 0x200) != 0) {
            ground = 0;

            if (*(s8*)(pParty + 0x4C) == 0) {
                *(u32*)(pParty + 8) |=
                    0x01000000;
                *(u32*)pParty |=
                    0x02000000;
            }
        } else if ((attr & 0x800) != 0) {
            partyChgRunMode(party, 0xD);
            return 0;
        }
    }

    *(void**)(pParty + 0x138) = ground;

    if (ground == 0) {
        partyChgMot(party, 3);
        *(f32*)(pParty + 0x11C) = float_neg1_804230c8;
    } else {
        attr = hitGetAttr(ground);

        if ((attr & 0x100) != 0) {
            *(u32*)pParty |= 0x80;
        } else {
            *(u32*)pParty &= ~0x80;
        }

        if ((hitGetAttr(ground) & 0x40000000) != 0) {
            *(void**)(pParty + 0x14C) = ground;
        }
    }

    *(f32*)(pParty + 0x5C) = groundY;
    party_force_reset_outofscreen(party);

    return 0;
}

u8 party_motion_homing_fly(s32 pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    typedef struct FootmarkLocal {
        u32 flags;
        VecLocal position;
    } FootmarkLocal;

    typedef union IntToDoubleLocal {
        f64 value;
        struct {
            u32 hi;
            u32 lo;
        } words;
    } IntToDoubleLocal;

    extern void* gp;
    extern f32 vec3_802f4628[];
    extern void partyChgPoseId(void* party, s32 pose);
    extern void partyChgMot(void* party, s32 motion);
    extern s32 marioKeyOffChk(void);
    extern void* partyGetFootmarkPtr(s32 id);
    extern f32 partyGetTargetDistY(s32 id);
    extern f32 partyGetTargetMovespd2(void* party);
    extern void unk_800cbfbc(f32 speed, void* party);

    extern f32 touch_wall_ct;

    extern s32 partyGetFootmarkId(void* party);
    extern s32 party_slit(void* party);
    extern s32 party_roll(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyMoveBehindMario(void* party, s32 mode);
    extern void partyChgMotSlitFloor(void* party);
    extern s32 strcmp(const char*, const char*);
    extern f32 PSVECDistance(void* a, void* b);
    extern void partyNextReadId(s32 id);
    extern void partyGetReadId(s32 id, f32* x, f32* y, f32* z);
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* speed);
    extern void* partySearchFrontWall(f32 speed, f32 dir, void* party, VecLocal* pos);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern void* partySearchHead(f32 dir, void* party, VecLocal* pos, f32* height);
    extern u32 getFrontFloor(f32 angle, void* party, f32* outPos);
    extern f32 revise360(f32 angle);
    extern void partyGetAppearPos3(void* party, VecLocal* pos);
    extern void partyGetAppearPos4(void* party, VecLocal* pos);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 unk_800ccb34(f32 y, s32 footmarkId);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void party_force_reset_outofscreen(void* party);
    extern f32 partyGetTargetDist3D(s32 id, void* pos);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void unk_800cbb10(f32 speed, void* party);
    extern u32 hitGetAttr(void* hit);
    extern u32 marioGetSlitFloorPos(void* outPos);
    extern s32 partyGetFootmarkWriteId(s32 id);
    extern s32 partyGetFootmarkReadId(s32 id);
    extern void* hitCheckFilter(f32 x, f32 y, f32 z,
                                f32 dx, f32 dy, f32 dz, s32 flags,
                                void* out0, f32* outY, void* out2,
                                f32* outDist, void* out4, void* out5,
                                void* out6);


    void* party;
    void* player;
    FootmarkLocal* footmark;
    void* ground;
    void* wall;
    void* hit;
    void* headHit;

    VecLocal target;
    VecLocal current;
    VecLocal slitPos;
    VecLocal appearPos;
    VecLocal movePosLocal;
    VecLocal frontFloor;
    VecLocal wallProbe;
    VecLocal headPos;

    f32 closeDist;
    f32 targetDist;
    f32 overrideDist;
    f32 speed;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 distance;
    f32 distance3D;
    f32 direction;
    f32 moveSpeed;
    f32 extraSpeed;
    f32 radius;
    f32 groundY;
    f32 headHeight;
    f32 wallHeight;
    f32 deltaY;
    f32 hitOutY;
    f32 hitMaxDist;
    f32 hitOut0;
    f32 hitOut2;
    f32 hitOut4;
    f32 hitOut5;
    f32 hitOut6;
    f32 absDelta;

    IntToDoubleLocal convert;
    s32 quantized;
    s32 footmarkId;
    s32 writeId;
    s32 readId;
    u16 motion;
    u32 attr;
    s32 farFollow;

    party = (void*)pParty;
    player = *(void**)(pParty + 0x160);
    footmarkId = partyGetFootmarkId(party);

    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        *(u32*)pParty &= ~0x00080000;
        *(u8*)(pParty + 0x4E) = 0;
    }

    if (*(s16*)(pParty + 0x28) != 0) {
        closeDist = float_28_80423178;
    } else {
        closeDist = float_45_804230e0;
    }

    if ((strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            overrideDist = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) != 0) {
                overrideDist = float_8_804230f4;
            } else {
                overrideDist = float_30_804230f8;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) != 0) {
                overrideDist = float_8_804230f4;
            } else {
                overrideDist = float_30_804230f8;
            }
        }

        closeDist = overrideDist;
        *(u32*)(pParty + 8) |= 0x00020000;
    }

    if (partyGetTargetDistY(footmarkId) != float_0_804230bc) {
        targetDist = float_50_804230b8;
    } else {
        targetDist = float_45_804230e0;
    }

    if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            overrideDist = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) != 0) {
                overrideDist = float_8_804230f4;
            } else {
                overrideDist = float_30_804230f8;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) != 0) {
                overrideDist = float_8_804230f4;
            } else {
                overrideDist = float_30_804230f8;
            }
        }

        targetDist = overrideDist;
    }

    if (party_slit(party) == 0 || party_roll(party) == 0) {
        return 0;
    }

    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x00080000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }

    if (*(s16*)((s32)player + 0x50) != 0 &&
        *(f32*)((s32)player + 0x180) == float_0_804230bc) {
        partyChgMot(party, 0);
        return 0;
    }

    motion = *(u16*)((s32)player + 0x2E);
    if (motion == 0x1F || motion == 0x20 ||
        motion == 0x10 || motion == 0x11) {
        partyChgMot(party, 0);
        return 0;
    }

    speed = partyGetTargetMovespd2(party);

    distance = PSVECDistance(
        (void*)((s32)player + 0x8C),
        (void*)(pParty + 0x58));

    if (distance < float_45_804230e0) {
        speed = float_1_804230ec;
    } else if (distance >= float_50_804230b8) {
        speed = float_3_804230dc;
    }

    if ((*(u32*)player & 0x00008000) != 0) {
        speed = float_1_804230ec;
        closeDist = float_100_804230b0;
    }

    unk_800cbfbc(speed, party);

    startX = *(f32*)(pParty + 0x58);
    startY = *(f32*)(pParty + 0x5C);
    startZ = *(f32*)(pParty + 0x60);

    footmark = (FootmarkLocal*)partyGetFootmarkPtr(footmarkId);
    target.x = footmark->position.x;

    if ((*(u32*)(pParty + 4) & 0x00008000) != 0) {
        target.y = footmark->position.y + float_1_804230ec;
    } else {
        target.y = footmark->position.y + float_5_804230e8;
    }

    target.z = footmark->position.z;

    current = *(VecLocal*)(pParty + 0x58);
    distance3D = partyGetTargetDist3D(footmarkId, &current);

    if (distance3D >= closeDist) {
        unk_800cbfbc(speed, party);
    }

    if (targetDist < float_20_80423090) {
        unk_800cbfbc(speed, party);
    }

    farFollow = 0;

    for (;;) {
        direction = angleABf(startX, startZ, target.x, target.z);
        distance = distABf(startX, startZ, target.x, target.z);

        if ((*(u32*)player & 0x00008000) != 0) {
            radius = *(f32*)(pParty + 0xF4) +
                     *(f32*)((s32)player + 0x1B8);
        } else {
            radius = float_0p5_804230d0 * *(f32*)(pParty + 0xF4) +
                     *(f32*)(pParty + 0x104);
        }

        if (distance > radius) {
            current = *(VecLocal*)(pParty + 0x58);
            distance3D = partyGetTargetDist3D(footmarkId, &current);

            if (distance3D >= closeDist) {
                if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
                    extraSpeed =
                        float_0p1_80423114 *
                        (distance3D - targetDist);
                } else {
                    extraSpeed =
                        float_0p1_80423114 *
                        (distance3D - closeDist);
                }

                if (extraSpeed > float_4_8042317c) {
                    extraSpeed = float_4_8042317c;
                }
                if (extraSpeed < float_0p5_804230d0) {
                    extraSpeed = float_0_804230bc;
                }

                unk_800cbf84(
                    *(f32*)((s32)player + 0x180) + extraSpeed,
                    party);
                farFollow = 1;
                goto common_motion;
            }

            if (marioGetSlitFloorPos(&slitPos) != 0) {
                unk_800cbfbc(float_1p5_8042316c, party);
                partyNextReadId(footmarkId);

                if (marioGetSlitFloorPos(&slitPos) != 0) {
                    target = slitPos;
                    distance = distABf(
                        startX, startZ,
                        slitPos.x, slitPos.z);
                    direction = angleABf(
                        startX, startZ,
                        target.x, target.z);

                    if (distance <= float_5_804230e8) {
                        partyChgMotSlitFloor(party);
                    }
                }

                goto common_motion;
            }

            if (distance > targetDist) {
                if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
                    extraSpeed =
                        float_0p1_80423114 *
                        (distance - targetDist);

                    if (extraSpeed > float_4_8042317c) {
                        extraSpeed = float_4_8042317c;
                    }
                    if (extraSpeed < float_0p5_804230d0) {
                        extraSpeed = float_0_804230bc;
                    }

                    unk_800cbf84(
                        *(f32*)((s32)player + 0x180) + extraSpeed,
                        party);
                } else {
                    extraSpeed = distance - targetDist;

                    if (extraSpeed >= float_1_804230ec) {
                        extraSpeed = float_1_804230ec;

                        if (targetDist < float_20_80423090) {
                            extraSpeed = speed;
                        }
                    }

                    unk_800cbfbc(extraSpeed, party);
                }

                goto common_motion;
            }

            if (*(f32*)(pParty + 0xA0) == float_0_804230bc &&
                *(f32*)(pParty + 0xA8) == float_0_804230bc) {
                unk_800cbfbc(
                    *(f32*)((s32)player + 0x180),
                    party);
            } else {
                unk_800cbf84(float_0_804230bc, party);

                *(f32*)(pParty + 0x100) =
                    angleABf(
                        *(f32*)(pParty + 0x58),
                        *(f32*)(pParty + 0x60),
                        *(f32*)((s32)player + 0x8C),
                        *(f32*)((s32)player + 0x94));

                partyChgPoseId(party, 1);
            }
        }

        writeId = partyGetFootmarkWriteId(footmarkId);
        readId = partyGetFootmarkReadId(footmarkId);

        if (readId == writeId) {
            unk_800cbfbc(
                *(f32*)((s32)player + 0x180),
                party);

            if (*(f32*)((s32)player + 0x180) == float_0_804230bc ||
                *(f32*)(pParty + 0xA0) != float_0_804230bc ||
                *(f32*)(pParty + 0xA8) != float_0_804230bc) {
                direction = *(f32*)(pParty + 0x100);
                unk_800cbfbc(float_0_804230bc, party);
                *(f32*)(pParty + 0x11C) = float_0_804230bc;

                if ((*(u32*)pParty & 0x00000200) == 0) {
                    partyChgPoseId(party, 1);
                }

                partyChgMot(party, 0);
            }

            goto common_motion;
        }

        partyNextReadId(footmarkId);
        partyGetReadId(
            footmarkId,
            &target.x,
            &target.y,
            &target.z);

        if ((*(u32*)pParty & 0x04000000) != 0) {
            continue;
        }

        *(u32*)pParty &= ~0x00000080;

        ground = partySearchGround(
            float_100_804230b0,
            float_100_804230b0,
            party);

        if (ground != 0 &&
            (hitGetAttr(ground) & 0x00000200) != 0) {
            ground = 0;
        }

        *(void**)(pParty + 0x138) = ground;

        if (ground == 0) {
            continue;
        }

        attr = hitGetAttr(ground);

        if ((attr & 0x00000100) != 0) {
            *(u32*)pParty |= 0x00000080;
        } else {
            *(u32*)pParty &= ~0x00000080;
        }

        if ((attr & 0x00000800) != 0) {
            partyChgMot(party, 0);
            goto common_motion;
        }
    }

common_motion:
    if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
        unk_800cbb10(*(f32*)(pParty + 0x104), party);
    } else {
        partyChgMot(party, 0);
    }

    if (*(s8*)(pParty + 0x4E) > 10) {
        direction =
            angleABf(
                *(f32*)(pParty + 0x58),
                *(f32*)(pParty + 0x60),
                *(f32*)((s32)player + 0x8C),
                *(f32*)((s32)player + 0x94));
    }

    if (farFollow != 0) {
        player = *(void**)(pParty + 0x160);
        partyGetAppearPos3(party, &appearPos);

        direction =
            angleABf(
                *(f32*)(pParty + 0x58),
                *(f32*)(pParty + 0x60),
                appearPos.x,
                appearPos.z);

        ground = partySearchGround(
            float_10_804230a4,
            float_0_804230bc,
            party);

        groundY = *(f32*)(pParty + 0xE4);

        if (ground != 0 &&
            (hitGetAttr(ground) & 0x00000200) != 0) {
            ground = 0;
        }

        if (ground != 0) {
            *(f32*)(pParty + 0x5C) = groundY;
        } else {
            if ((*(u32*)(pParty + 4) & 0x00008000) != 0) {
                deltaY =
                    (*(f32*)((s32)player + 0x90) +
                     float_1_804230ec) -
                    *(f32*)(pParty + 0x5C);
            } else {
                deltaY =
                    (*(f32*)((s32)player + 0x90) +
                     float_5_804230e8) -
                    *(f32*)(pParty + 0x5C);
            }

            *(f32*)(pParty + 0x5C) =
                float_0p05_80423174 * deltaY +
                *(f32*)(pParty + 0x5C);
        }
    }

    *(f32*)(pParty + 0x100) = direction;

    movePosLocal = *(VecLocal*)(pParty + 0x58);

    partyGetMoveDirSpd(
        party,
        &direction,
        &moveSpeed);

    wall = partySearchFrontWall(
        moveSpeed,
        direction,
        party,
        &movePosLocal);

    if ((*(u32*)pParty & 0x01000000) != 0) {
        wall = 0;
    }

    if (wall == 0) {
        if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
            getFrontFloor(
                *(f32*)(pParty + 0x100),
                party,
                (f32*)&frontFloor);
        } else {
            *(f32*)(pParty + 0x54) =
                revise360(
                    float_270_80423104 +
                    *(f32*)((s32)player + 0x19C));

            partyGetAppearPos4(
                party,
                &frontFloor);
        }

        if (frontFloor.y >= *(f32*)((s32)player + 0x90)) {
            if ((*(u32*)(pParty + 4) & 0x00008000) != 0) {
                deltaY =
                    (frontFloor.y + float_1_804230ec) -
                    *(f32*)(pParty + 0x5C);
            } else {
                deltaY =
                    (frontFloor.y + float_5_804230e8) -
                    *(f32*)(pParty + 0x5C);
            }

            *(f32*)(pParty + 0x5C) =
                float_0p05_80423174 * deltaY +
                *(f32*)(pParty + 0x5C);
        }

        *(u8*)(pParty + 0x4E) = 0;

        if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
            movePosLocal.x = *(f32*)(pParty + 0x58);
            movePosLocal.z = *(f32*)(pParty + 0x60);

            movePos(
                *(f32*)(pParty + 0x104),
                *(f32*)(pParty + 0x100),
                &movePosLocal.x,
                &movePosLocal.z);
        }
    } else {
        *(void**)(pParty + 0x12C) = wall;

        if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
            *(void**)(pParty + 0x134) = wall;
            attr = hitGetAttr(wall);

            if ((attr & 0x00002000) != 0) {
                if (*(u8*)(pParty + 0x37) == 1) {
                    if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
                        movePosLocal.x = *(f32*)(pParty + 0x58);
                        movePosLocal.z = *(f32*)(pParty + 0x60);

                        movePos(
                            *(f32*)(pParty + 0x104),
                            *(f32*)(pParty + 0x100),
                            &movePosLocal.x,
                            &movePosLocal.z);
                    }
                } else if ((*(u32*)((s32)player + 0xC) &
                            0x80000000) == 0) {
                    *(u32*)(pParty + 8) |= 1;
                    *(u8*)(pParty + 0x37) = 1;
                }
            } else {
                wallProbe.x = *(f32*)(pParty + 0x58);
                wallProbe.z = *(f32*)(pParty + 0x60);
                groundY = *(f32*)(pParty + 0x5C);

                movePos(
                    *(f32*)(pParty + 0xF4),
                    direction,
                    &wallProbe.x,
                    &wallProbe.z);

                hitMaxDist = float_1000_804230c4;

                hit = hitCheckFilter(
                    wallProbe.x,
                    groundY + touch_wall_ct,
                    wallProbe.z,
                    float_0_804230bc,
                    float_neg1_804230c8,
                    float_0_804230bc,
                    0,
                    &hitOut0,
                    &hitOutY,
                    &hitOut2,
                    &hitMaxDist,
                    &hitOut4,
                    &hitOut5,
                    &hitOut6);

                if (hit == 0) {
                    wallHeight = float_neg2000_804230cc;
                } else {
                    quantized =
                        (s32)(float_100_804230b0 * hitOutY +
                              float_0p5_804230d0);

                    convert.words.hi = 0x43300000;
                    convert.words.lo = ((u32)quantized) ^ 0x80000000;

                    wallHeight =
                        (f32)(convert.value -
                              *(f64*)((u8*)vec3_802f4628 + 0x108)) /
                        float_100_804230b0;

                    hitGetAttr(hit);
                }

                if (wallHeight > float_neg2000_804230cc) {
                    deltaY =
                        (wallHeight + *(f32*)(pParty + 0xF0)) -
                        *(f32*)(pParty + 0x5C);

                    *(f32*)(pParty + 0x5C) =
                        float_0p05_80423174 * deltaY +
                        *(f32*)(pParty + 0x5C);
                } else {
                    *(u8*)(pParty + 0x4E) =
                        *(u8*)(pParty + 0x4E) + 1;

                    if (unk_800ccb34(
                            *(f32*)(pParty + 0x5C),
                            footmarkId) >= float_1_804230ec) {
                        absDelta =
                            *(f32*)((s32)player + 0x90) -
                            *(f32*)(pParty + 0x5C);

                        if (absDelta < float_0_804230bc) {
                            absDelta = -absDelta;
                        }

                        if (absDelta >= float_10_804230a4) {
                            deltaY =
                                (float_10_804230a4 +
                                 *(f32*)((s32)player + 0x90)) -
                                *(f32*)(pParty + 0x5C);

                            *(f32*)(pParty + 0x5C) =
                                float_0p1_80423114 * deltaY +
                                *(f32*)(pParty + 0x5C);

                            *(f32*)(pParty + 0x58) =
                                movePosLocal.x;
                            *(f32*)(pParty + 0x60) =
                                movePosLocal.z;

                            return 0;
                        }
                    }
                }
            }
        }
    }

    *(f32*)(pParty + 0x58) = movePosLocal.x;
    *(f32*)(pParty + 0x60) = movePosLocal.z;

    if (strcmp(
            (char*)gp + 0x12C,
            (char*)vec3_802f4628 + 0x1A0) == 0) {
        headPos = *(VecLocal*)(pParty + 0x58);
        headHeight = *(f32*)(pParty + 0xF0);

        headHit = partySearchHead(
            *(f32*)(pParty + 0x10C),
            party,
            &headPos,
            &headHeight);

        if (headHit != 0) {
            *(void**)(pParty + 0x140) = headHit;

            deltaY =
                (headPos.y - *(f32*)(pParty + 0xF0)) -
                *(f32*)(pParty + 0x5C);

            if (deltaY < float_neg5_80423180 ||
                deltaY >= float_0_804230bc) {
                *(f32*)(pParty + 0x5C) =
                    float_0p1_80423114 * deltaY +
                    *(f32*)(pParty + 0x5C);
            }
        }
    }

    *(u32*)pParty &= ~0x00000080;

    ground = partySearchGround(
        float_10_804230a4,
        float_10_804230a4,
        party);

    if (ground != 0) {
        attr = hitGetAttr(ground);

        if ((attr & 0x00000800) != 0) {
            partyChgRunMode(party, 0xD);
            return 0;
        }

        if ((attr & 0x00000100) != 0) {
            *(u32*)pParty |= 0x00000080;
        } else {
            *(u32*)pParty &= ~0x00000080;
        }
    }

    party_force_reset_outofscreen(party);

    return 0;
}

void party_set_behind_dir(void* party, f32 dir) {
    void* cam = camGetPtr(4);

    *(f32*)((s32)party + 0x54) = revise360(dir + *(f32*)((s32)cam + 0x114));
    *(u32*)((s32)party + 8) |= 0x100000;
}

u8 party_motion_behind_mario(s32 pParty) {
    typedef struct Vec { f32 x,y,z; } Vec;
    extern f32 toMovedir(f32);
    extern f32 revise360(f32);
    extern void* partyGetPtr(s32);
    extern f32 partyToMovedir(f32,void*);
    extern void partyGetAppearPos4(void*,Vec*);
    extern f32 distABf(f32,f32,f32,f32);
    extern f32 angleABf(f32,f32,f32,f32);
    extern void partyChgMoveMode(void*,s32);
    extern void partyChgMot(void*,s32);
    extern void partySetDir(f32,f32,void*);
    extern void unk_800cbf84(f32,void*);
    extern void partyMoveCheckWall(void*);
    extern void unk_800cbb10(f32,void*);
    extern void* partySearchGround(f32,f32,void*);
    extern u32 hitGetAttr(void*);
    extern void movePos(f32,f32,f32*,f32*);
    extern void* hitCheckFilter(f32,f32,f32,f32,f32,f32,s32,void*,f32*,void*,f32*,void*,void*,void*);
    void* party;
    void* player;
    void* lead;
    void* hit;
    Vec target;
    f32 dir;
    f32 distance;
    f32 speed;
    f32 groundY;
    f32 maxDist;
    f32 out0, out2, out4, out5, out6;

    party=(void*)pParty;
    player=*(void**)((s32)party+0x160);
    if((*(u32*)((s32)party+8)&2)!=0){
        *(u32*)((s32)party+8)&=~2;
        *(u32*)((s32)party+8)|=0x00040000;
        if((*(u32*)((s32)party+8)&0x00100000)==0){
            if((*(u32*)((s32)party+8)&0x00010000)==0){
                if((*(u32*)party&0x1000)==0){
                    dir=toMovedir(*(f32*)((s32)player+0x1AC));
                }else{
                    lead=partyGetPtr(marioGetPartyId());
                    if(lead==0) dir=toMovedir(*(f32*)((s32)player+0x1AC));
                    else dir=partyToMovedir(*(f32*)((s32)lead+0x10C),party);
                }
                dir=revise360(dir+float_180_80423108);
            }else{
                *(u32*)((s32)party+8)&=~0x00010000;
                dir=revise360(float_180_80423108+angleABf(*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94),
                    *(f32*)((s32)party+0x94),*(f32*)((s32)party+0x9C)));
            }
            *(f32*)((s32)party+0x54)=dir;
        }
        partyGetAppearPos4(party,&target);
        *(s32*)((s32)party+0x24)=(s32)distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),target.x,target.z);
        if(*(s32*)((s32)party+0x24)<1) *(s32*)((s32)party+0x24)=1;
    }

    if(((*(u32*)((s32)party+8)&0x00100000)==0)&&(marioKeyOffChk()!=0)){
        if((*(u32*)party&0x1000)==0) dir=toMovedir(*(f32*)((s32)player+0x1AC));
        else{
            lead=partyGetPtr(marioGetPartyId());
            if(lead==0) dir=toMovedir(*(f32*)((s32)player+0x1AC));
            else dir=partyToMovedir(*(f32*)((s32)lead+0x10C),party);
        }
        *(f32*)((s32)party+0x54)=revise360(dir+float_180_80423108);
    }

    partyGetAppearPos4(party,&target);
    dir=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),target.x,target.z);
    maxDist=float_1000_804230c4;
    hit=hitCheckFilter(target.x,target.y+touch_wall_ct,target.z,
        float_0_804230bc,float_neg1_804230c8,float_0_804230bc,0,
        &out0,&groundY,&out2,&maxDist,&out4,&out5,&out6);
    speed=float_neg2000_804230cc;
    if(hit!=0){
        speed=(f32)(s32)(float_100_804230b0*groundY+float_0p5_804230d0)/float_100_804230b0;
        hitGetAttr(hit);
    }
    if(speed<=float_neg2000_804230cc){
        speed=float_1p5_8042316c * *(f32*)((s32)player+0x1B8);
        movePos(speed,toMovedir(*(f32*)((s32)player+0x1AC)),&target.x,&target.z);
    }
    distance=distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),target.x,target.z);
    speed=(*(f32*)((s32)player+0x1B0)>float_1_804230ec)?float_10_804230a4:float_2_804230d8;
    --*(s32*)((s32)party+0x24);
    if((distance<speed)||(*(s32*)((s32)party+0x24)<1)){
        *(s32*)((s32)party+0x24)=0;
        if((*(u32*)((s32)party+8)&0x8000)!=0) *(u32*)((s32)party+8)&=~0x20000;
        partyChgMoveMode(party,1);
        partyChgMot(party,0);
        partySetDir(*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94),party);
        *(f32*)((s32)party+0x40)=float_50_804230b8;
        *(u32*)((s32)party+4)&=~0x100000;
        *(u32*)party|=0x100000;
        *(u32*)((s32)party+8)&=~(0x00100000|0x00040000|0x00008000);
        hit=partySearchGround(float_100_804230b0,float_100_804230b0,party);
        if((hit!=0)&&((hitGetAttr(hit)&0x200)!=0)&&(*(s8*)((s32)party+0x4C)==0)){
            hit=0;
            *(u32*)((s32)party+8)|=0x01000000;
            *(u32*)party|=0x02000000;
        }
        *(void**)((s32)party+0x138)=hit;
        return 0;
    }

    *(f32*)((s32)party+0x100)=dir;
    speed=((*(u32*)((s32)party+8)&0x00100000)!=0)?float_3p6_80423170:float_2_804230d8;
    unk_800cbf84(speed,party);
    partyMoveCheckWall(party);
    unk_800cbb10(*(f32*)((s32)party+0x104),party);
    if((*(void**)((s32)party+0x13C)!=0)||(*(f32*)((s32)party+0x5C)<target.y)){
        target.x=*(f32*)((s32)party+0x58);
        target.z=*(f32*)((s32)party+0x60);
        movePos(distance,dir,&target.x,&target.z);
        maxDist=float_1000_804230c4;
        hit=hitCheckFilter(target.x,target.y+touch_wall_ct,target.z,
            float_0_804230bc,float_neg1_804230c8,float_0_804230bc,0,
            &out0,&groundY,&out2,&maxDist,&out4,&out5,&out6);
        speed=float_neg2000_804230cc;
        if(hit!=0){
            speed=(f32)(s32)(float_100_804230b0*groundY+float_0p5_804230d0)/float_100_804230b0;
            hitGetAttr(hit);
        }
        maxDist=*(f32*)((s32)party+0x5C)-speed;
        if(maxDist<float_0_804230bc) maxDist=-maxDist;
        if(maxDist>float_1_804230ec){
            *(f32*)((s32)party+0x94)=target.x;
            *(f32*)((s32)party+0x98)=speed;
            *(f32*)((s32)party+0x9C)=target.z;
            partyChgMot(party,2);
            return 0;
        }
    }
    hit=partySearchGround(float_10_804230a4,float_0_804230bc,party);
    if((hit!=0)&&((hitGetAttr(hit)&0x200)!=0)&&(*(s8*)((s32)party+0x4C)==0)){
        hit=0;
        *(u32*)((s32)party+8)|=0x01000000;
        *(u32*)party|=0x02000000;
    }
    *(void**)((s32)party+0x138)=hit;
    if(hit==0){
        partyChgMot(party,3);
        *(f32*)((s32)party+0x11C)=float_neg1_804230c8;
    }
    *(f32*)((s32)party+0x5C)=*(f32*)((s32)party+0x118);
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void party_motion_behind_mario_continue(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern void* anotherPartyGetPtr(s32 slot);
    extern f32 partyToMovedir(f32 dir, void* party);
    extern f32 revise360(f32 dir);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void* hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags,
                                void* out0, f32* outY, void* out2, f32* dist,
                                void* out4, void* out5, void* out6);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partyChgMot(void* party, s32 mot);
    extern void partySetDir(f32 x, f32 z, void* party);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void partyMoveCheckWall(void* party);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void* partySearchGround(f32 height, f32 vel, void* party);

    void* player;
    void* keyPlayer;
    void* party;
    void* hit;
    f32 dir;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 hitY;
    f32 speed;
    f32 dist;
    f32 out0;
    f32 out2;
    f32 out4;
    f32 out5;
    f32 out6;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)pParty & 0x8) == 0) {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            dir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
        } else {
            dir = toMovedir(*(f32*)((s32)player + 0x1AC));
        }
    }
    *(f32*)((s32)pParty + 0x54) = revise360(dir + float_180_80423108);

    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
                *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             targetX, targetZ);
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) <= 0) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if (marioKeyOffChk() != 0) {
        keyPlayer = *(void**)((s32)pParty + 0x160);
        if ((*(u32*)pParty & 0x8) == 0) {
            dir = toMovedir(*(f32*)((s32)keyPlayer + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                dir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            } else {
                dir = toMovedir(*(f32*)((s32)keyPlayer + 0x1AC));
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(dir + float_180_80423108);
    }

    if ((*(u32*)pParty & 0x8) == 0) {
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        targetY = *(f32*)((s32)player + 0x90);
    } else {
        party = anotherPartyGetPtr(*(s8*)((s32)pParty + 0x2F));
        if (party != 0) {
            targetX = *(f32*)((s32)party + 0x58);
            targetZ = *(f32*)((s32)party + 0x60);
            targetY = *(f32*)((s32)party + 0x5C);
        } else {
            targetX = *(f32*)((s32)player + 0x8C);
            targetZ = *(f32*)((s32)player + 0x94);
            targetY = *(f32*)((s32)player + 0x90);
        }
    }

    movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
            *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);

    dist = float_1000_804230c4;
    hit = hitCheckFilter(targetX, targetY + touch_wall_ct, targetZ,
                         float_0_804230bc, float_neg1_804230c8, float_0_804230bc,
                         0, &out0, &hitY, &out2, &dist, &out4, &out5, &out6);
    speed = float_neg2000_804230cc;
    if (hit != 0) {
        speed = (f32)(s32)(float_100_804230b0 * hitY + float_0p5_804230d0) /
                float_100_804230b0;
        hitGetAttr(hit);
    }
    if (speed <= float_neg2000_804230cc) {
        speed = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
        movePos(speed, toMovedir(*(f32*)((s32)player + 0x1AC)), &targetX, &targetZ);
    }

    speed = float_2_804230d8;
    if (*(f32*)((s32)player + 0x180) > float_1_804230ec) {
        speed = float_10_804230a4;
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
    *(s32*)((s32)pParty + 0x24) -= 1;
    if (dist < speed) {
        *(s32*)((s32)pParty + 0x24) = 0;
        if (*(f32*)((s32)player + 0x180) == float_0_804230bc) {
            *(u32*)pParty |= 0x80000;
            partyChgMoveMode(pParty, 1);
            partyChgMot(pParty, 0);
            partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
            *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
            *(u32*)((s32)pParty + 4) &= ~0x10;
        }
    } else {
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        speed = dist / float_10_804230a4;
        if (speed > float_2_804230d8) {
            speed = float_2_804230d8;
        }
        if (speed < float_1_804230ec) {
            speed = float_1_804230ec;
        }
        unk_800cbf84(speed, pParty);
        partyMoveCheckWall(pParty);
        unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
        hit = partySearchGround(float_10_804230a4, float_0_804230bc, pParty);
        if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
            hit = 0;
            *(u32*)((s32)pParty + 8) |= 0x01000000;
            *(u32*)pParty |= 0x02000000;
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
    }

    if (*(void**)((s32)pParty + 0x13C) != 0) {
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(targetX, targetZ, pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x10;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw off
void party_motion_behind_mario_fly(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern f32 partyToMovedir(f32 dir, void* party);
    extern f32 revise360(f32 dir);
    extern void partyGetAppearPos4(void* party, VecLocal* pos);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partyChgMot(void* party, s32 mot);
    extern void partySetDir(f32 x, f32 z, void* party);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void unk_800cba60(void* party);
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* speed);
    extern void* partySearchFrontWall(f32 speed, f32 dir, void* party, VecLocal* pos);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);

    void* player;
    void* directionPlayer;
    void* party;
    void* hit;
    VecLocal pos;
    f32 facingDir;
    f32 dir;
    f32 speed;
    f32 moveSpeed;
    f32 dist;
    f32 limit;
    f32 y;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(u32*)((s32)pParty + 8) |= 0x00040000;
        if ((*(u32*)((s32)pParty + 8) & 0x00100000) == 0) {
            if ((*(u32*)((s32)pParty + 8) & 0x00010000) == 0) {
                directionPlayer = *(void**)((s32)pParty + 0x160);
                if ((*(u32*)pParty & 0x8) == 0) {
                    facingDir = toMovedir(*(f32*)((s32)directionPlayer + 0x1AC));
                } else {
                    party = partyGetPtr(marioGetPartyId());
                    if (party != 0) {
                        facingDir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
                    } else {
                        facingDir = toMovedir(*(f32*)((s32)directionPlayer + 0x1AC));
                    }
                }
                facingDir = revise360(facingDir + float_180_80423108);
            } else {
                *(u32*)((s32)pParty + 8) &= ~0x00010000;
                facingDir = revise360(float_180_80423108 +
                                      angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                               *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C)));
            }
            *(f32*)((s32)pParty + 0x54) = facingDir;
        }
        partyGetAppearPos4(pParty, (VecLocal*)((s32)pParty + 0x94));
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) < 1) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if ((*(u32*)((s32)pParty + 8) & 0x00100000) == 0 && marioKeyOffChk() != 0) {
        directionPlayer = *(void**)((s32)pParty + 0x160);
        if ((*(u32*)pParty & 0x8) == 0) {
            facingDir = toMovedir(*(f32*)((s32)directionPlayer + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                facingDir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            } else {
                facingDir = toMovedir(*(f32*)((s32)directionPlayer + 0x1AC));
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(facingDir + float_180_80423108);
    }

    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
             *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    if (*(f32*)((s32)player + 0x180) > float_1_804230ec) {
        limit = float_6_80423100;
    } else {
        limit = float_2_804230d8;
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                   *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    *(s32*)((s32)pParty + 0x24) -= 1;
    y = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
    if ((dist < limit && (y <= float_2_804230d8 && -y <= float_2_804230d8)) ||
        *(s32*)((s32)pParty + 0x24) < 1) {
        *(s32*)((s32)pParty + 0x24) = 0;
        if (((*(u32*)((s32)pParty + 8) & 0x00008000) != 0) && *(u8*)((s32)pParty + 0x32) != 0) {
            *(u32*)((s32)pParty + 8) &= ~0x20000;
            *(u32*)((s32)pParty + 4) &= ~0x100000;
        }
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
        unk_800cbf84(float_0_804230bc, pParty);
        *(u32*)pParty |= 0x100000;
        *(u32*)((s32)pParty + 8) &= ~0x00100000;
        *(u32*)((s32)pParty + 8) &= ~0x00040000;
        *(u32*)((s32)pParty + 8) &= ~0x00008000;
        unk_800cba60(pParty);
        if (*(void**)((s32)pParty + 0x13C) != 0) {
            *(u32*)pParty |= 0x100000;
            partyChgMoveMode(pParty, 1);
            partyChgMot(pParty, 0);
            partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
            *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
            *(u32*)((s32)pParty + 4) &= ~0x100000;
            *(u32*)((s32)pParty + 8) &= ~0x00040000;
        }
        hit = partySearchGround(float_100_804230b0, float_100_804230b0, pParty);
        if (hit != 0) {
            if ((hitGetAttr(hit) & 0x200) != 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
                hit = 0;
                *(u32*)((s32)pParty + 8) |= 0x01000000;
                *(u32*)pParty |= 0x02000000;
            }
            if (*(f32*)((s32)pParty + 0xF0) < dist) {
                hit = 0;
            }
            y = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0x5C) =
                float_0p05_80423174 * (*(f32*)((s32)pParty + 0x118) - y) + y;
        }
        *(void**)((s32)pParty + 0x138) = hit;
        return;
    }

    *(f32*)((s32)pParty + 0x100) =
        angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                 *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
            *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    moveSpeed = float_2_804230d8;
    if ((*(u32*)((s32)pParty + 8) & 0x00100000) != 0) {
        moveSpeed = float_3p6_80423170;
    }
    if (dist < limit) {
        moveSpeed = float_0_804230bc;
        unk_800cbf84(moveSpeed, pParty);
    }
    unk_800cbfbc(moveSpeed, pParty);
    pos.x = *(f32*)((s32)pParty + 0x58);
    pos.y = *(f32*)((s32)pParty + 0x5C);
    pos.z = *(f32*)((s32)pParty + 0x60);
    partyGetMoveDirSpd(pParty, &dir, &speed);
    hit = partySearchFrontWall(speed, dir, pParty, &pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x12C) = hit;
        if (*(f32*)((s32)pParty + 0x104) != float_0_804230bc) {
            *(void**)((s32)pParty + 0x134) = hit;
        }
    }
    *(f32*)((s32)pParty + 0x58) = pos.x;
    *(f32*)((s32)pParty + 0x60) = pos.z;
    y = *(f32*)((s32)pParty + 0x5C);
    *(f32*)((s32)pParty + 0x5C) = float_0p05_80423174 * (*(f32*)((s32)pParty + 0x98) - y) + y;
    unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
}
#pragma use_lmw_stmw reset

void party_motion_beside_mario(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern void* partyGetPtr(s32 id);
    extern f32 partyToMovedir(f32 dir, void* party);
    extern f32 revise360(f32 dir);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void* hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags,
                                void* out0, f32* outY, void* out2, f32* dist,
                                void* out4, void* out5, void* out6);
    extern u32 hitGetAttr(void* hit);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void partyMoveCheckWall(void* party);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern void partyChgMot(void* party, s32 mot);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partySetDir(f32 x, f32 z, void* party);

    void* player;
    void* party;
    void* hit;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    f32 targetX;
    f32 targetZ;
    f32 speed;
    u32 attr;
    s32 timer;
    f32 out0;
    f32 out2;
    f32 out4;
    f32 out5;
    f32 out6;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        if ((*(u32*)pParty & 0x1000) == 0) {
            speed = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                speed = toMovedir(*(f32*)((s32)player + 0x1AC));
            } else {
                speed = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(speed + float_90_80423118);
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
                *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             targetX, targetZ);
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) < 1) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if (marioKeyOffChk() != 0) {
        if ((*(u32*)pParty & 0x1000) == 0) {
            speed = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                speed = toMovedir(*(f32*)((s32)player + 0x1AC));
            } else {
                speed = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(speed + float_90_80423118);
    }

    targetX = *(f32*)((s32)player + 0x8C);
    targetZ = *(f32*)((s32)player + 0x94);
    y = *(f32*)((s32)player + 0x90);
    movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
            *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);

    dist = float_1000_804230c4;
    hit = hitCheckFilter(targetX, y + touch_wall_ct, targetZ,
                         float_0_804230bc, float_neg1_804230c8, float_0_804230bc,
                         0, &out0, &y, &out2, &dist, &out4, &out5, &out6);
    z = float_neg2000_804230cc;
    if (hit != 0) {
        z = (f32)(s32)(float_100_804230b0 * y + float_0p5_804230d0) / float_100_804230b0;
        hitGetAttr(hit);
    }
    if (z <= float_neg2000_804230cc) {
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
                toMovedir(*(f32*)((s32)player + 0x1AC)), &targetX, &targetZ);
    }

    speed = float_2_804230d8;
    if (*(f32*)((s32)player + 0x1B0) > float_1_804230ec) {
        speed = float_10_804230a4;
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
    *(s32*)((s32)pParty + 0x24) -= 1;
    if (dist < speed || *(s32*)((s32)pParty + 0x24) < 1) {
        *(s32*)((s32)pParty + 0x24) = 0;
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
        *(u32*)pParty |= 0x100000;
    } else {
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        speed = dist / float_10_804230a4;
        if (speed > float_2_804230d8) {
            speed = float_2_804230d8;
        }
        if (speed < float_1_804230ec) {
            speed = float_1_804230ec;
        }
        unk_800cbf84(speed, pParty);
        partyMoveCheckWall(pParty);
        unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
        hit = partySearchGround(float_10_804230a4, float_0_804230bc, pParty);
        if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
            hit = 0;
            *(u32*)((s32)pParty + 8) |= 0x01000000;
            *(u32*)pParty |= 0x02000000;
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
    }

    if (*(void**)((s32)pParty + 0x13C) != 0) {
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
    }
}

u8 party_motion_jump(s32 pParty) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern s32 partyGetFootmarkId(void*);
    extern void partyNextReadId(s32);
    extern f32 distABf(f32,f32,f32,f32);
    extern f32 angleABf(f32,f32,f32,f32);
    extern void movePos(f32,f32,f32*,f32*);
    extern void partyChgPoseId(void*,s32);
    extern void psndSFXOn_3D(s32,void*);
    extern void* partySearchGround(f32,void*);
    extern u32 hitGetAttr(void*);
    extern void partyChgMot(void*,s32);
    extern void N_partyPlaySfxPartyLanding1(void*);
    extern void* partySearchFrontWall(f32,f32,void*,Vec*);
    void* party;
    void* player;
    void* hit;
    Vec pos;
    f32 distance;
    f32 frames;
    f32 yDelta;
    f32 accel;
    f32 vel;
    s32 id;
    s32 timer;

    party=(void*)pParty;
    player=*(void**)((s32)party+0x160);
    id=partyGetFootmarkId(party);
    if((*(u32*)((s32)party+8)&2)!=0){
        *(u32*)((s32)party+8)&=~2;
        *(u8*)((s32)party+0x38)=0;
    }

    if(*(u8*)((s32)party+0x38)==10){
        if((*(u32*)party&0x1000)==0) partyNextReadId(id);
        *(f32*)((s32)party+0x114)+=*(f32*)((s32)party+0x11C);
        *(f32*)((s32)party+0x5C)+=*(f32*)((s32)party+0x114);
        if(*(f32*)((s32)party+0x5C)<float_neg2000_804230cc){
            *(u32*)party&=~0x80;
            *(f32*)((s32)party+0x114)=float_0_804230bc;
            *(f32*)((s32)party+0x100)=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),
                *(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94));
            *(f32*)((s32)party+0x58)=*(f32*)((s32)player+0x8C);
            *(f32*)((s32)party+0x5C)=*(f32*)((s32)player+0x90);
            *(f32*)((s32)party+0x60)=*(f32*)((s32)player+0x94);
            partyChgMot(party,0);
            return;
        }
        if(*(f32*)((s32)party+0x114)<=float_0_804230bc){
            *(u32*)party&=~0x80;
            *(u32*)party|=0x20;
            vel=*(f32*)((s32)party+0x114);
            if(vel<float_0_804230bc) vel=-vel;
            hit=partySearchGround(vel,party);
            if((hit!=0)&&((hitGetAttr(hit)&0x200)!=0)){
                if(*(s8*)((s32)party+0x4C)==0){
                    *(u32*)((s32)party+8)|=0x01000000;
                    *(u32*)party|=0x02000000;
                }
                *(void**)((s32)party+0x138)=0;
                partyChgMot(party,3);
                *(f32*)((s32)party+0x11C)=float_neg1_804230c8;
                return;
            }
            *(void**)((s32)party+0x138)=hit;
            *(f32*)((s32)party+0x5C)=*(f32*)((s32)party+0x118);
            if(hit!=0){
                *(u32*)party|=0x20;
                *(u32*)((s32)party+8)&=~0x200000;
                *(u32*)party&=~0xE0;
                N_partyPlaySfxPartyLanding1(party);
                *(f32*)((s32)party+0x114)=float_0_804230bc;
                partyChgMot(party,0);
                *(u32*)((s32)party+8)|=0x400000;
                return;
            }
        }
        pos.x=*(f32*)((s32)party+0x58);
        pos.y=*(f32*)((s32)party+0x5C);
        pos.z=*(f32*)((s32)party+0x60);
        if((*(u32*)party&0x200000)==0){
            hit=partySearchFrontWall(*(f32*)((s32)party+0x104),*(f32*)((s32)party+0x100),party,&pos);
            *(void**)((s32)party+0x13C)=hit;
            if((hit!=0)&&(*(f32*)((s32)party+0x104)!=0.0f)) *(void**)((s32)party+0x140)=hit;
            *(f32*)((s32)party+0x58)=pos.x;
            *(f32*)((s32)party+0x60)=pos.z;
        }else{
            movePos(*(f32*)((s32)party+0x104),*(f32*)((s32)party+0x100),
                (f32*)((s32)party+0x58),(f32*)((s32)party+0x60));
        }
        return;
    }

    if(*(u8*)((s32)party+0x38)==0){
        if((*(s16*)((s32)party+0x28)!=0)||(*(u8*)((s32)party+0x34)==9)){
            pos.x=*(f32*)((s32)party+0x58);
            pos.y=*(f32*)((s32)party+0x5C);
            pos.z=*(f32*)((s32)party+0x60);
            movePos(*(f32*)((s32)party+0x104),*(f32*)((s32)party+0x100),&pos.x,&pos.z);
            *(f32*)((s32)party+0x94)=pos.x;
            *(f32*)((s32)party+0x98)=pos.y;
            *(f32*)((s32)party+0x9C)=pos.z;
        }
        distance=distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),
            *(f32*)((s32)party+0x94),*(f32*)((s32)party+0x9C));
        *(f32*)((s32)party+0x128)=distance;
        if(*(s16*)((s32)party+0x28)==0){
            *(f32*)((s32)party+0x100)=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),
                *(f32*)((s32)party+0x94),*(f32*)((s32)party+0x9C));
            *(f32*)((s32)party+0x104)=float_2p25_80423154**(f32*)((s32)player+0x228);
        }
        timer=(s32)((9.0f*(distance/ *(f32*)((s32)party+0x104))+float_0p5_804230d0)/float_10_804230a4);
        if(timer<12) timer=12;
        if(timer>40) timer=40;
        *(s32*)((s32)party+0x24)=timer;
        frames=float_0p5_804230d0*(f32)timer;
        yDelta=*(f32*)((s32)party+0x98)-*(f32*)((s32)party+0x5C);
        accel=((yDelta>=float_1_804230ec)?yDelta:float_10_804230a4)*float_2_804230d8/(frames*frames-frames);
        *(f32*)((s32)party+0x114)=*(f32*)((s32)player+0x228)*accel*frames;
        if(*(f32*)((s32)party+0x114)>13.0f) *(f32*)((s32)party+0x114)=13.0f;
        if(*(f32*)((s32)party+0x114)<3.0f) *(f32*)((s32)party+0x114)=3.0f;
        *(f32*)((s32)party+0x11C)=-accel**(f32*)((s32)player+0x228);
        *(u32*)party|=0x80;
        *(u32*)party&=~0x20;
        partyChgPoseId(party,4);
        psndSFXOn_3D(0x927,(void*)((s32)party+0x58));
        *(u8*)((s32)party+0x38)=10;
        if((*(u32*)party&0x1000)==0) partyNextReadId(id);
    }
    return;
}

void party_motion_jump2(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void partySearchFrontWall(f32 distance, f32 angle, void* party, VecLocal* pos);
    extern void* partySearchGround(f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern void N_partyPlaySfxPartyLanding1(void* party);

    void* player;
    void* hit;
    VecLocal pos;
    f32 dist;
    f32 frames;
    f32 yDelta;
    f32 velAbs;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(s32*)((s32)pParty + 0x24) = 0;
        *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8 * *(f32*)((s32)player + 0x228);
        *(f32*)((s32)pParty + 0x104) = float_2p25_80423154 * *(f32*)((s32)player + 0x228);
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                     *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                       *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
        if (dist > float_50_804230b8) {
            dist = float_50_804230b8;
        }
        *(f32*)((s32)pParty + 0x128) = dist;
        timer = (s32)((float_10_804230a4 * (dist / *(f32*)((s32)pParty + 0x104)) +
                       float_0p5_804230d0) / float_10_804230a4);
        if (timer < 10) {
            timer = 10;
        }
        *(s32*)((s32)pParty + 0x24) = timer;
        frames = (f32)timer;
        *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)player + 0x228) * (dist / frames);
        yDelta = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
        velAbs = *(f32*)((s32)pParty + 0x11C);
        if (velAbs < float_0_804230bc) {
            velAbs = -velAbs;
        }
        *(f32*)((s32)pParty + 0x114) =
            (float_0p5_804230d0 * frames * frames * velAbs + yDelta) / frames;
        *(f32*)((s32)pParty + 0x114) *= *(f32*)((s32)player + 0x228);
        if (*(f32*)((s32)pParty + 0x114) > float_20_80423090) {
            *(f32*)((s32)pParty + 0x114) = float_20_80423090;
        }
        if (*(f32*)((s32)pParty + 0x114) < float_0_804230bc) {
            *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        }
        partyChgPoseId(pParty, 4);
        *(u32*)pParty |= 0x80;
        *(u32*)pParty &= ~0x20;
        psndSFXOn_3D(0x927, (void*)((s32)pParty + 0x58));
        *(u8*)((s32)pParty + 0x38) = 0;
    }

    if (*(u8*)((s32)pParty + 0x38) == 1) {
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x11C);
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
        pos.x = *(f32*)((s32)pParty + 0x58);
        pos.y = *(f32*)((s32)pParty + 0x5C);
        pos.z = *(f32*)((s32)pParty + 0x60);
        partySearchFrontWall(*(f32*)((s32)pParty + 0x104), *(f32*)((s32)pParty + 0x100),
                             pParty, &pos);
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x60) = pos.z;
        if (*(f32*)((s32)pParty + 0x114) <= float_0_804230bc) {
            velAbs = *(f32*)((s32)pParty + 0x114);
            if (velAbs < float_0_804230bc) {
                velAbs = -velAbs;
            }
            hit = partySearchGround(velAbs, pParty);
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
            if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
                if (*(s8*)((s32)pParty + 0x4C) == 0) {
                    *(u32*)((s32)pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
                *(void**)((s32)pParty + 0x138) = 0;
                partyChgMot(pParty, 3);
                *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
                return;
            }
            *(void**)((s32)pParty + 0x138) = hit;
            if (hit != 0) {
                N_partyPlaySfxPartyLanding1(pParty);
                *(u32*)pParty &= ~0xE0;
                *(u32*)pParty |= 0x20;
                *(u32*)((s32)pParty + 8) &= ~0x200000;
                *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                               *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
                partyChgMot(pParty, dist >= float_5_804230e8);
            }
        }
    } else if (*(u8*)((s32)pParty + 0x38) == 0) {
        *(u8*)((s32)pParty + 0x38) = 1;
    }

    if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
        *(u32*)pParty &= ~0x80;
        *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                     *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
        *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
        partyChgMot(pParty, 0);
    }
}

s32 party_motion_fall(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 partyGetFootmarkId(void* party);
    extern void partyNextReadId(s32 id);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 toMovedir(f32 dir);
    extern f32 revise360(f32 dir);
    extern void partyGetAppearPos4(void* party, VecLocal* pos);
    extern void partyChgMot(void* party, s32 mot);
    extern void partySearchFrontWall(f32 distance, f32 angle, void* party, VecLocal* pos);
    extern void N_partyPlaySfxPartyLanding1(void* party);

    void* player;
    void* hit;
    VecLocal pos;
    f32 velAbs;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)pParty & 0x1000) == 0) {
        partyNextReadId(partyGetFootmarkId(pParty));
    }

    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        partyChgPoseId(pParty, 4);
        *(u32*)pParty &= ~0x100000;
    }

    hit = 0;
    if ((*(u32*)pParty & 0x02000000) == 0) {
        velAbs = *(f32*)((s32)pParty + 0x114);
        if (velAbs < float_0_804230bc) {
            velAbs = -velAbs;
        }
        hit = partySearchGround(float_10_804230a4, velAbs, pParty);
    }

    if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0 && *(u8*)((s32)pParty + 0x32) == 0 &&
        *(s8*)((s32)pParty + 0x4C) == 0) {
        hit = 0;
        *(u32*)((s32)pParty + 8) |= 0x01000000;
        *(u32*)pParty |= 0x02000000;
    }
    *(void**)((s32)pParty + 0x138) = hit;

    if (hit == 0) {
        *(u32*)pParty &= ~0x20;
        if ((*(u32*)pParty & 0x40) == 0) {
            *(u32*)pParty |= 0x40;
            if (*(f32*)((s32)pParty + 0x11C) == float_0_804230bc) {
                *(f32*)((s32)pParty + 0x11C) = float_neg1p4_8042313c;
            }
        }
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x11C);
        if (*(f32*)((s32)pParty + 0x114) < float_neg16_80423140) {
            *(f32*)((s32)pParty + 0x114) = float_neg16_80423140;
        }
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);

        if (marioKeyOffChk() == 0) {
            *(f32*)((s32)pParty + 0x104) += float_neg0p2_80423144;
            if (*(f32*)((s32)pParty + 0x104) < float_0_804230bc) {
                *(f32*)((s32)pParty + 0x104) = float_0_804230bc;
            }
        }

        if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
            *(f32*)((s32)pParty + 0x5C) = float_neg2001_80423148;
            if (*(u16*)((s32)player + 0x2E) == 0x1F || *(u16*)((s32)player + 0x2E) == 0x20) {
                return 2;
            }
            *(u32*)pParty &= ~0xE0;
            *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x11C) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x100) =
                angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                         *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
            *(f32*)((s32)pParty + 0x54) =
                revise360(float_180_80423108 + toMovedir(*(f32*)((s32)player + 0x1A4)));
            partyGetAppearPos4(pParty, (VecLocal*)((s32)pParty + 0x58));
            *(f32*)((s32)pParty + 0x5C) += float_37_8042314c;
            partyChgMot(pParty, 0);
            *(u32*)pParty &= ~0x02000000;
        }

        if (*(f32*)((s32)pParty + 0x104) != float_0_804230bc) {
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            partySearchFrontWall(float_1p3_80423150 * *(f32*)((s32)pParty + 0x104),
                                 *(f32*)((s32)pParty + 0x100), pParty, &pos);
            *(f32*)((s32)pParty + 0x58) = pos.x;
            *(f32*)((s32)pParty + 0x60) = pos.z;
        }
        return 2;
    }

    if ((*(u32*)pParty & 0x40) == 0) {
        *(u32*)pParty &= ~0x2000;
        *(u32*)pParty &= ~0xE0;
        *(u32*)pParty |= 0x20;
        *(u32*)((s32)pParty + 8) &= ~0x200000;
        *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
        if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
            partyChgMot(pParty, 0);
        } else {
            partyChgMot(pParty, 1);
        }
        N_partyPlaySfxPartyLanding1(pParty);
        if ((*(u32*)pParty & 0x1000) == 0) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
        return 3;
    }

    *(u32*)pParty &= ~0xE0;
    *(u32*)pParty |= 0x20;
    *(u32*)((s32)pParty + 8) &= ~0x200000;
    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
    *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
    if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
        partyChgMot(pParty, 0);
    } else {
        partyChgMot(pParty, 1);
    }
    N_partyPlaySfxPartyLanding1(pParty);
    return 3;
}


void party_motion_upstairs(void* party) {
    if ((*(u32*)((s32)party + 8) & 2) != 0) {
        f32 divisor;
        *(u32*)((s32)party + 8) &= ~2;
        divisor = float_3_804230dc;
        *(u32*)party |= 0x40;
        unk_8041ec0c = 3;
        unk_8041ec08 = (*(f32*)((s32)party + 0x120) - *(f32*)((s32)party + 0x5C)) / divisor;
    }
    *(f32*)((s32)party + 0x5C) += unk_8041ec08;
    unk_8041ec0c--;
    if (unk_8041ec0c <= 0) {
        *(u32*)party &= ~0x40;
        partyChgMot(party, 1);
    }
}

void party_motion_damage(void* pParty) {
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void marioGetScreenPos(void* pos, f32* x, f32* y, f32* z);
    extern void partyGetReturnPos(void* pos);
    extern void unk_800cbf84(void* party);
    extern void partyMove(void* party);
    extern u32 hitGetAttr(void* hit);
    void* player;
    f32 posA[3];
    f32 posB[3];
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    u8 state;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(u8*)((s32)pParty + 0x38) = 0;
    }

    state = *(u8*)((s32)pParty + 0x38);
    switch (state) {
        case 0:
            partyChgPoseId(pParty, 8);
            *(f32*)((s32)pParty + 0x114) = float_15_80423130;
            *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
            *(s32*)((s32)pParty + 0x24) = 0x3C;
            *(u8*)((s32)pParty + 0x38) = 1;
        case 1:
            *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
            posA[0] = *(f32*)((s32)pParty + 0x58);
            posA[1] = *(f32*)((s32)pParty + 0x5C);
            posA[2] = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(posA, &x, &y, &z);
            if (y <= float_neg300_80423134) {
                *(s32*)((s32)pParty + 0x24) = 0x14;
                *(u8*)((s32)pParty + 0x38) = 0xA;
            }
            break;
        case 10:
            if (*(u16*)((s32)player + 0x2E) != 0x1F && *(u16*)((s32)player + 0x2E) != 0x20) {
                timer = *(s32*)((s32)pParty + 0x24) - 1;
                *(s32*)((s32)pParty + 0x24) = timer;
                if (timer <= 0) {
                    partyGetReturnPos(posB);
                    *(f32*)((s32)pParty + 0x58) = posB[0];
                    *(f32*)((s32)pParty + 0x5C) = posB[1] + float_200_804230a8;
                    *(f32*)((s32)pParty + 0x60) = posB[2];
                    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                    *(f32*)((s32)pParty + 0x118) = float_neg1_804230c8;
                    unk_800cbf84(pParty);
                    *(u32*)pParty |= 0x20;
                    *(u32*)pParty |= 0x02000000;
                    *(u8*)((s32)pParty + 0x38) = 0xB;
                }
            }
            break;
        case 11:
            posB[0] = *(f32*)((s32)pParty + 0x58);
            posB[1] = *(f32*)((s32)pParty + 0x5C);
            posB[2] = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(posB, &x, &y, &z);
            if (y > float_240_80423138) {
                *(u32*)pParty &= ~0x02000000;
            }
            partyMove(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_neg6_804230fc) {
                *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
                *(f32*)((s32)pParty + 0x114) = float_neg6_804230fc;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
                *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
                *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
                *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
                *(f32*)((s32)pParty + 0x5C) += float_200_804230a8;
                *(u32*)pParty &= ~0xE0;
            }
            if (*(void**)((s32)pParty + 0x138) != 0 && (hitGetAttr(*(void**)((s32)pParty + 0x138)) & 0x200) == 0) {
                *(u8*)((s32)pParty + 0x38) = 0xC;
                *(s32*)((s32)pParty + 0x24) = 0x1E;
            }
            break;
        case 12:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer <= 0) {
                partyChgRunMode(pParty, 2);
                *(u8*)((s32)pParty + 0x3B) = 0;
                *(u32*)((s32)pParty + 4) &= ~0x18;
            }
            break;
    }
}


void allPartySlitOn(void) {
    void* party = partyGetPtr(marioGetPartyId());
    void* unk;

    if (party != 0) {
        unk = *(void**)((s32)party + 0x160);
        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0) {
        unk = *(void**)((s32)party + 0x160);
        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
    }
}

void allPartySlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }
}


void allPartyForceSlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }
}


void U_allPartySlitOffReq(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        *(u32*)((s32)party + 4) &= ~0x3C0;
        partyPaperOff(party);
        *(u32*)party &= ~0x3000;
        *(u8*)((s32)party + 0x37) = 0;
        partyChgPoseId(party, 1);
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        *(u32*)((s32)party + 4) &= ~0x3C0;
        partyPaperOff(party);
        *(u32*)party &= ~0x3000;
        *(u8*)((s32)party + 0x37) = 0;
        partyChgPoseId(party, 1);
    }
}


void L_partyForceSlitOff(void* party) {
    if (PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }
}

void partyChgMotSlitFloor(void* party) {
    if (!(*(u32*)party & 0x2000)) {
        void* unk = *(void**)((s32)party + 0x160);

        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
        *(u32*)party |= 0x2000;
    }
}

void unk_8014c330(void* party) {
    if (party != 0) {
        if (*(u8*)((s32)party + 0x37) == 1) {
            if (PARTY_CAN_FORCE_SLIT_OFF(party)) {
                PARTY_FORCE_SLIT_OFF_BODY(party);
            }
        }
        *(u8*)((s32)party + 0x37) = 0;
    }
}

s32 party_slit(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void unk_800cbf84(f32 value, void* party);
    extern f32 revise360(f32 angle);
    extern s32 unk_800c2010(void* party, VecLocal* pos);
    extern u32 hitGetAttr(void* hit);
    extern void marioClearSlitFloor(void);
    extern char str_p_slit_802f4770[];
    extern char str_PM_S_1A_802f4778[];
    extern char str_PM_S_1B_802f4780[];
    extern char str_PM_S_1E_802f4790[];
    extern char str_PM_S_1D_802f4798[];
    extern char str_PM_S_1H_802f47a0[];
    extern char str_PM_S_1C_802f47a8[];

    void* player;
    VecLocal pos;
    s32 ret;
    s32 timer;
    f32 dirDiff;

    ret = 1;
    player = *(void**)((s32)pParty + 0x160);
    if (*(u8*)((s32)pParty + 0x37) != 1) {
        if ((*(u32*)player & 0x00100000) == 0) {
            return 1;
        }
        if ((*(u32*)((s32)player + 0xC) & 1) != 0) {
            return 1;
        }
        *(u32*)((s32)pParty + 8) |= 1;
        *(u8*)((s32)pParty + 0x37) = 1;
    }

    if ((*(u32*)((s32)pParty + 8) & 1) != 0) {
        *(u32*)((s32)pParty + 8) &= ~1;
        *(u32*)((s32)pParty + 4) |= 0x100;
        partyPaperOn(pParty, str_p_slit_802f4770);
        partyChgPaper(pParty, str_PM_S_1A_802f4778);
        *(s32*)((s32)pParty + 0x24) = 0x1E;
        unk_800cbf84(float_0_804230bc, pParty);
        dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
        if (dirDiff > float_180_80423108) {
            *(u32*)((s32)pParty + 4) |= 0x200;
            *(u8*)((s32)pParty + 0x3A) = 2;
        } else {
            *(u32*)((s32)pParty + 4) &= ~0x200;
            *(u8*)((s32)pParty + 0x3A) = 0;
        }
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 0:
        case 2:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 0) {
                *(u32*)pParty |= 0x1000;
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
                partyChgPaper(pParty, str_PM_S_1B_802f4780);
                if ((*(u32*)pParty & 0x2000) == 0) {
                    ret = 0;
                    *(u8*)((s32)pParty + 0x3A) = 10;
                } else {
                    *(u8*)((s32)pParty + 0x3A) = 20;
                }
            }
            break;
        case 10:
            if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
                partyChgPaper(pParty, str_PM_S_1B_802f4780);
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
            } else {
                dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
                if (dirDiff < float_180_80423108) {
                    *(u32*)((s32)pParty + 4) &= ~0x200;
                } else {
                    *(u32*)((s32)pParty + 4) |= 0x200;
                }
                if ((*(u32*)((s32)pParty + 4) & 0x200) != 0) {
                    *(f32*)((s32)pParty + 0x10C) = float_280_8042311c;
                } else {
                    *(f32*)((s32)pParty + 0x10C) = float_260_80423120;
                }
                if (dirDiff >= float_315_80423124 || dirDiff <= float_45_804230e0 ||
                    (dirDiff >= float_135_80423128 && dirDiff <= float_225_8042312c)) {
                    partyChgPaper(pParty, str_PM_S_1E_802f4790);
                } else {
                    partyChgPaper(pParty, str_PM_S_1D_802f4798);
                }
            }
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            if (unk_800c2010(pParty, &pos) == 0 && (*(u32*)player & 0x00100000) == 0 &&
                (hitGetAttr(*(void**)((s32)pParty + 0x138)) & 0x2000) == 0 &&
                (hitGetAttr(*(void**)((s32)pParty + 0x13C)) & 0x2000) == 0) {
                *(u8*)((s32)pParty + 0x3A) = 40;
            }
            break;
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 20:
            partyChgPaper(pParty, str_PM_S_1H_802f47a0);
            *(u32*)pParty |= 0x80;
            *(u32*)pParty |= 0x40;
            *(s32*)((s32)pParty + 0x24) = 0x14;
            unk_800cbf84(float_0_804230bc, pParty);
            *(u8*)((s32)pParty + 0x3A) = 21;
            break;
        case 21:
            dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)pParty + 0xFC));
            if (dirDiff <= float_180_80423108) {
                *(f32*)((s32)pParty + 0x10C) = float_260_80423120;
            } else {
                *(f32*)((s32)pParty + 0x10C) = float_280_8042311c;
            }
            *(f32*)((s32)pParty + 0x5C) += float_neg3_80423110;
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                *(u8*)((s32)pParty + 0x3A) = 22;
            }
            break;
        case 22:
            *(u8*)((s32)pParty + 0x3A) = 10;
            *(u32*)pParty &= ~0x80;
            marioClearSlitFloor();
            break;
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 40:
            partyChgPaper(pParty, str_PM_S_1C_802f47a8);
            *(s32*)((s32)pParty + 0x24) = 0x1E;
            *(u8*)((s32)pParty + 0x3A) = 41;
            dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
            if (float_180_80423108 <= dirDiff) {
                *(f32*)((s32)pParty + 0x10C) = float_0_804230bc;
            } else {
                *(f32*)((s32)pParty + 0x10C) = float_180_80423108;
            }
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
        case 41:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                *(u8*)((s32)pParty + 0x3A) = 42;
            }
            break;
        case 42:
            *(u32*)((s32)pParty + 4) &= ~0x3C0;
            partyPaperOff(pParty);
            *(u32*)pParty &= ~0x3000;
            *(u8*)((s32)pParty + 0x37) = 0;
            break;
    }

    return ret;
}

s32 party_roll(void* pParty) {
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void unk_800cbf84(f32 value, void* party);
    extern f32 revise360(f32 angle);
    extern void partyPaperLightOff(void* party);
    extern char str_p_roll_802f4750[];
    extern char str_PM_R_1A_802f4758[];
    extern char str_PM_R_1B_802f4760[];
    extern char str_PM_R_1C_802f4768[];
    extern f32 vec3_802f4670;
    extern f32 DAT_802f4674;
    extern f32 DAT_802f4678;
    extern f32 vec3_802f467c;
    extern f32 DAT_802f4680;
    extern f32 DAT_802f4684;
    f32 value;

    if (*(u8*)((s32)pParty + 0x37) != 2) {
        return 1;
    }

    if ((*(u32*)((s32)pParty + 8) & 1) != 0) {
        *(u32*)((s32)pParty + 8) &= ~1;
        partyPaperOn(pParty, str_p_roll_802f4750);
        partyChgPaper(pParty, str_PM_R_1A_802f4758);
        partyChgPoseId(pParty, 0);
        *(f32*)((s32)pParty + 0x88) = float_neg6_804230fc;
        *(f32*)((s32)pParty + 0x8C) = float_6_80423100;
        *(f32*)((s32)pParty + 0x7C) = float_6_80423100;
        *(f32*)((s32)pParty + 0x80) = float_2_804230d8;
        *(s32*)((s32)pParty + 0x24) = 0x41;
        *(u8*)((s32)pParty + 0x3A) = 0;
        unk_800cbf84(float_0_804230bc, pParty);
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 0:
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) <= 0) {
                partyChgPaper(pParty, str_PM_R_1B_802f4760);
                *(u8*)((s32)pParty + 0x3A) = 10;
            }
            break;
        case 10:
            *(f32*)((s32)pParty + 0x10C) =
                revise360(float_270_80423104 - *(f32*)((s32)pParty + 0x100));
            value = revise360(*(f32*)((s32)pParty + 0xFC) + *(f32*)((s32)pParty + 0x10C));
            if (value > float_180_80423108) {
                *(f32*)((s32)pParty + 0x6C) =
                    revise360(float_7_8042310c * *(f32*)((s32)pParty + 0x104) +
                              *(f32*)((s32)pParty + 0x6C));
            } else {
                *(f32*)((s32)pParty + 0x6C) =
                    revise360(*(f32*)((s32)pParty + 0x6C) -
                              float_7_8042310c * *(f32*)((s32)pParty + 0x104));
            }
            break;
    }

    if (*(u8*)((s32)pParty + 0x3A) == 100) {
        *(f32*)((s32)pParty + 0x104) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x64) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x6C) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x88) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x8C) = float_0_804230bc;
        value = float_neg3_80423110;
        if (*(f32*)((s32)pParty + 0x100) < float_180_80423108) {
            value = float_3_804230dc;
        }
        *(f32*)((s32)pParty + 0x7C) = value;
        *(f32*)((s32)pParty + 0x80) = float_0_804230bc;
        *(s32*)((s32)pParty + 0x24) = 0x41;
        *(u8*)((s32)pParty + 0x3A) = 101;
        partyChgPaper(pParty, str_PM_R_1C_802f4768);
        partyPaperLightOff(pParty);
    } else if (*(u8*)((s32)pParty + 0x3A) != 101) {
        return 1;
    }

    *(s32*)((s32)pParty + 0x24) -= 1;
    if (*(s32*)((s32)pParty + 0x24) <= 0) {
        if (*(f32*)((s32)pParty + 0x100) < float_180_80423108) {
            *(f32*)((s32)pParty + 0x10C) = float_180_80423108;
            *(f32*)((s32)pParty + 0x110) = float_180_80423108;
            *(f32*)((s32)pParty + 0xFC) = float_90_80423118;
            *(f32*)((s32)pParty + 0x100) = float_90_80423118;
        } else {
            *(f32*)((s32)pParty + 0x10C) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x110) = float_0_804230bc;
            *(f32*)((s32)pParty + 0xFC) = float_270_80423104;
            *(f32*)((s32)pParty + 0x100) = float_270_80423104;
        }
        *(f32*)((s32)pParty + 0x88) = vec3_802f4670;
        *(f32*)((s32)pParty + 0x8C) = DAT_802f4674;
        *(f32*)((s32)pParty + 0x90) = DAT_802f4678;
        *(f32*)((s32)pParty + 0x7C) = vec3_802f467c;
        *(f32*)((s32)pParty + 0x80) = DAT_802f4680;
        *(f32*)((s32)pParty + 0x84) = DAT_802f4684;
        *(u8*)((s32)pParty + 0x37) = 0;
        *(u32*)pParty &= ~0x1000;
        *(u32*)((s32)pParty + 4) &= ~0x1000;
        *(u32*)pParty &= ~0x200;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
        *(u32*)((s32)pParty + 4) &= ~0x200000;
        *(u8*)((s32)pParty + 0x3A) = 100;
        partyPaperOff(pParty);
        return 1;
    }

    if (*(f32*)((s32)pParty + 0x100) >= float_180_80423108) {
        value = float_0p1_80423114 * (float_270_80423104 - *(f32*)((s32)pParty + 0x10C)) +
                *(f32*)((s32)pParty + 0x10C);
        if ((value - float_270_80423104) < float_1_804230ec &&
            (float_270_80423104 - value) < float_1_804230ec) {
            value = float_270_80423104;
        }
    } else {
        value = float_0p1_80423114 * (float_90_80423118 - *(f32*)((s32)pParty + 0x10C)) +
                *(f32*)((s32)pParty + 0x10C);
        if ((value - float_90_80423118) < float_1_804230ec &&
            (float_90_80423118 - value) < float_1_804230ec) {
            value = float_90_80423118;
        }
    }
    *(f32*)((s32)pParty + 0x10C) = value;
    *(f32*)((s32)pParty + 0x110) = value;
    return 0;
}

u32 getFrontFloor(f32 angle, void* pParty, f32* outPos) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitWork {
        char pad[0xC];
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWork;
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f64 sqrt(f64 value);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);

    HitWork probe;
    HitWork floor;
    f32 s;
    f32 c;
    f32 step;
    f32 bestY;
    f32 xzLen;
    f32 maxY;
    f32 bias;
    s32 i;
    void* hit;

    step = (float_2_804230d8 * *(f32*)((s32)pParty + 0xF4)) / float_3_804230dc;
    probe.dir = *(VecLocal*)&vec3_802f4628[24];
    sincosf(angle, &s, &c);
    bestY = float_neg2000_804230cc;
    for (i = 0; i < 3; i++) {
        probe.start.x = *(f32*)((s32)pParty + 0x58) + (f32)i * s * step;
        probe.start.y = *(f32*)((s32)pParty + 0x5C) + *(f32*)((s32)pParty + 0xF0);
        probe.start.z = *(f32*)((s32)pParty + 0x60) + (f32)i * c * step;
        probe.dist = float_100_804230b0;
        hit = (void*)hitCheckVecFilter(&probe, 0);
        if (hit != 0) {
            xzLen = probe.normal.x * probe.normal.x + probe.normal.z * probe.normal.z;
            if ((f32)sqrt((f64)xzLen) < float_45_804230e0) {
                bestY = probe.hitPos.y;
                break;
            }
        }
    }

    maxY = *(f32*)((s32)pParty + 0x5C) + touch_wall_ct;
    if (bestY <= float_neg2000_804230cc) {
        bestY = maxY;
    } else {
        bestY -= float_2_804230d8;
        if (maxY < bestY) {
            bestY = maxY;
        }
    }

    step = float_0p75_804230e4 * *(f32*)((s32)pParty + 0xF4);
    sincosf(angle, &s, &c);
    floor.start.x = *(f32*)((s32)pParty + 0x58) + s * step;
    floor.start.y = bestY;
    floor.start.z = *(f32*)((s32)pParty + 0x60) + c * step;
    floor.dir = *(VecLocal*)&vec3_802f4628[27];
    floor.dist = float_300_804230ac;

    hit = (void*)hitCheckVecFilter(&floor, 0);
    if (hit == 0) {
        outPos[0] = *(f32*)((s32)pParty + 0x58);
        outPos[1] = *(f32*)((s32)pParty + 0x5C);
        outPos[2] = *(f32*)((s32)pParty + 0x60);
        return 0;
    }

    bias = float_neg0p5_804230d4;
    if (floor.hitPos.x >= float_0_804230bc) {
        bias = float_0p5_804230d0;
    }
    outPos[0] = (f32)(s32)(floor.hitPos.x * float_1000_804230c4 + bias) / float_1000_804230c4;

    bias = float_neg0p5_804230d4;
    if (floor.hitPos.y >= float_0_804230bc) {
        bias = float_0p5_804230d0;
    }
    outPos[1] = (f32)(s32)(floor.hitPos.y * float_1000_804230c4 + bias) / float_1000_804230c4;

    bias = float_neg0p5_804230d4;
    if (floor.hitPos.z >= float_0_804230bc) {
        bias = float_0p5_804230d0;
    }
    outPos[2] = (f32)(s32)(floor.hitPos.z * float_1000_804230c4 + bias) / float_1000_804230c4;
    return hitGetAttr(hit);
}

void party_force_reset_outofscreen(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitWork {
        char pad[0xC];
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWork;
    extern s32 marioCtrlOffChk(void);
    extern void marioGetScreenPos(VecLocal* pos, f32* x, f32* y, f32* z);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 partyGetAppearPos3(void* party, VecLocal* pos);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern void* partyHitCheck(void* party, VecLocal* pos, VecLocal* dir, VecLocal* outPos, HitWork* work, f32* dist);
    extern u32 hitGetAttr(void* hit);

    void* player;
    VecLocal screenPos;
    VecLocal pos;
    VecLocal checkPos;
    VecLocal dir;
    VecLocal hitPos;
    HitWork work;
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 dist;
    f32 step;
    f32 angle;
    f32 y;
    f32 oldY;
    void* hit;
    s32 xLimit;
    s32 outerCount;
    s32 innerCount;

    player = *(void**)((s32)pParty + 0x160);
    if (marioCtrlOffChk() != 0) {
        return;
    }
    if (marioKeyOffChk() != 0) {
        return;
    }

    screenPos.x = *(f32*)((s32)pParty + 0x58);
    screenPos.y = *(f32*)((s32)pParty + 0x5C);
    screenPos.z = *(f32*)((s32)pParty + 0x60);
    marioGetScreenPos(&screenPos, &screenX, &screenY, &screenZ);

    if (distABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                *(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60)) < float_20_80423090 &&
        screenY > float_800_80423094) {
        partyGetAppearPos3(pParty, &pos);
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x5C) = pos.y;
        *(f32*)((s32)pParty + 0x60) = pos.z;
        return;
    }

    if (screenX >= float_neg100_80423098 && screenX <= float_700_8042309c &&
        screenY <= float_650_804230a0) {
        return;
    }

    partyClearFootmark();
    player = *(void**)((s32)pParty + 0x160);
    xLimit = 100;
    if (*(u8*)((s32)pParty + 0x30) != 0) {
        if (*(s8*)((s32)pParty + 0x31) == 5) {
            xLimit = 70;
        } else {
            xLimit = 50;
        }
        step = float_10_804230a4;
    } else {
        step = *(f32*)((s32)pParty + 0xF4);
    }

    pos.x = *(f32*)((s32)pParty + 0x58);
    pos.y = *(f32*)((s32)pParty + 0x5C);
    pos.z = *(f32*)((s32)pParty + 0x60);
    dir.x = float_0_804230bc;
    dir.y = -float_300_804230ac;
    dir.z = float_0_804230bc;

    for (outerCount = 0; outerCount <= 100; outerCount++) {
        screenPos = pos;
        marioGetScreenPos(&screenPos, &screenX, &screenY, &screenZ);
        if (screenX >= (f32)-xLimit && screenX <= (f32)(xLimit + 600)) {
            break;
        }

        angle = angleABf(pos.x, pos.z, *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        movePos(step, angle, &pos.x, &pos.z);
        oldY = pos.y;
        y = oldY;
        for (innerCount = 0; innerCount <= 4; innerCount++) {
            dist = float_300_804230ac;
            checkPos.x = pos.x;
            checkPos.y = pos.y + float_200_804230a8 - (float_50_804230b8 * (f32)innerCount);
            checkPos.z = pos.z;
            hit = partyHitCheck(pParty, &checkPos, &dir, &hitPos, &work, &dist);
            if (hit != 0) {
                y = hitPos.y;
                if ((hitGetAttr(hit) & 0x200) != 0) {
                    y = oldY;
                }
            } else {
                y = *(f32*)((s32)player + 0x90);
            }
            screenPos.x = checkPos.x;
            screenPos.y = y;
            screenPos.z = checkPos.z;
            marioGetScreenPos(&screenPos, &screenX, &screenY, &screenZ);
            if (screenY > float_100_804230b0) {
                break;
            }
        }

        if (y < pos.y + float_74_804230b4) {
            dist = float_100_804230b0;
            checkPos.x = pos.x;
            checkPos.y = pos.y + float_74_804230b4;
            checkPos.z = pos.z;
            hit = partyHitCheck(pParty, &checkPos, &dir, &hitPos, &work, &dist);
            if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
                y = hitPos.y;
            }
        }
        pos.y = y;
    }

    if (outerCount <= 100) {
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x5C) = pos.y;
        *(f32*)((s32)pParty + 0x60) = pos.z;
    }
}
