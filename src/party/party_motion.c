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
    extern f32 partyGetTargetDist3D(s32, void*);
    extern void partyGetMoveDirSpd(void*, f32*, f32*);
    extern void* partySearchFrontWall(f32, f32, void*, void*);
    extern s32 party_slit(void*);
    extern s32 party_roll(void*);
    extern void unk_800cbc30(void*);
    extern void unk_800c27c0(f32, f32, f32, f32, void*, void*);
    extern f32 partyGetTargetDist(f32, f32, s32);
    extern f32 angleABf(f32,f32,f32,f32);
    extern void movePos(f32,f32,f32*,f32*);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void* anotherPartyGetPtr(s32);
    extern s32 nokonokoGetStatus(void*);
    extern f32 PSVECDistance(void*,void*);
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
    typedef struct VecLocal {f32 x,y,z;} VecLocal;
    VecLocal pos, distancePos;
    f32 x,z;
    u8* base=(u8*)vec3_802f4628;
    void* another;
    f32 dir;
    f32 speed;
    f32 follow,override1,override2;
    f32 targetDistance;
    s32 footmarkId;
    u16 motion;
    s32 canMove;

    footmarkId = partyGetFootmarkId(party);
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
    follow=*(f32*)(pParty+0x40);
    if (*(f32*)((s32)player + 0x194) == float_0_804230bc &&
        *(f32*)((s32)player + 0x180) < float_1_804230ec)
        follow = float_100_804230b0;
    if ((s32)*(u8*)((s32)player + 0x3C) == 2) follow = float_70_80423184;
    if ((strcmp((char*)gp + 0x12C, (char*)base + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)base + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)base + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            override1 = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            override1 = (*(u32*)(pParty + 8) & 0x00200000) ? float_8_804230f4 : float_30_804230f8;
        } else if (*(s8*)(pParty + 0x31) == 6) {
            override1 = (*(u32*)(pParty + 8) & 0x00200000) ? float_8_804230f4 : float_30_804230f8;
        }
        follow=override1;
        *(u32*)(pParty + 8) |= 0x00020000;
    }
    if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            override2 = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            override2 = (*(u32*)(pParty + 8) & 0x00200000) ? float_8_804230f4 : float_30_804230f8;
        } else if (*(s8*)(pParty + 0x31) == 6) {
            override2 = (*(u32*)(pParty + 8) & 0x00200000) ? float_8_804230f4 : float_30_804230f8;
        }
        follow=override2;
    }
    party_slit(party);
    party_roll(party);
    unk_800cbc30(party);
    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
        goto done;
    }
    if (*(u8*)(pParty+0x34)!=0) {
        if (*(s8*)(pParty+0x2F)==0 || partyGetPtr(marioGetPartyId())==0) {
            void* current=*(void**)(pParty+0x160);
            if (*(s16*)((s32)current+0x50)!=0 && *(f32*)((s32)current+0x180)==float_0_804230bc) canMove=0;
            else {
                motion=*(u16*)((s32)current+0x2E);
                canMove=motion!=0x1F && motion!=0x20 && motion!=0x10 && motion!=0x11;
            }
            if (canMove) {
                if (*(u8*)(pParty+0x32)==0) {
                    distancePos=*(VecLocal*)(pParty+0x58);
                    targetDistance=partyGetTargetDist3D(footmarkId,&distancePos);
                    partyGetTargetDist(*(f32*)(pParty+0x58),*(f32*)(pParty+0x60),footmarkId);
                } else {
                    x=*(f32*)(pParty+0x58); z=*(f32*)(pParty+0x60);
                    dir=angleABf(*(f32*)(pParty+0x58),*(f32*)(pParty+0x60),*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94));
                    movePos(float_0p5_804230d0 * *(f32*)(pParty+0xF4),dir,&x,&z);
                    targetDistance=distABf(x,z,*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94));
                }
                if ((marioChkSlitThrouh()!=0 || targetDistance>=follow) && marioChkDeepSleep()==0) partyChgMot(party,1);
            }
        } else {
            another=anotherPartyGetPtr(*(s8*)(pParty+0x2F));
            if (another) {
                if (nokonokoGetStatus(another)==3 || PSVECDistance((u8*)player+0x8C,(u8*)another+0x58)>=float_150_80423188) {
                    if (partyGetTargetDist(*(f32*)(pParty+0x58),*(f32*)(pParty+0x60),footmarkId)>=follow) {partyChgMot(party,1);partyClearFootmark2();}
                } else if ((s32)*(u8*)((s32)another+0x31)==3 && *(u8*)((s32)another+0x34)==3) {
                    motion=*(u16*)((s32)player+0x2E);
                    if (motion!=0x1F && motion!=0x10 && motion!=0x11 && distABf(*(f32*)(pParty+0x58),*(f32*)(pParty+0x60),*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94))>=follow) partyChgMot(party,1);
                } else if ((*(u32*)another & 0x100)==0 || *(s8*)((s32)another+0x31)==4) {
                    if (distABf(*(f32*)(pParty+0x58),*(f32*)(pParty+0x60),*(f32*)((s32)another+0x58),*(f32*)((s32)another+0x60))>=follow) partyChgMot(party,1);
                }
                if (marioChkSlitThrouh()!=0) partyChgMot(party,1);
            }
        }
    }
    pos=*(VecLocal*)(pParty+0x58);
    partyGetMoveDirSpd(party, &dir, &speed);
    {
        void* wall = partySearchFrontWall(speed, dir, party, &pos);
        if (wall != 0) {
            *(void**)(pParty + 0x12C) = wall;
            if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
                *(void**)(pParty + 0x134) = wall;
            }
        }
    }
    *(f32*)(pParty + 0x58) = pos.x;
    *(f32*)(pParty + 0x60) = pos.z;
    partyGetMoveDirSpd(party, &dir, &speed);
    unk_800c27c0(speed,dir,*(f32*)(pParty+0xF4),*(f32*)(pParty+0xF0),party,&pos);
    *(f32*)(pParty + 0x58) = pos.x;
    *(f32*)(pParty + 0x60) = pos.z;
    if (*(u8*)(pParty + 0x32) == 1) {
        unk_80150fac(party);
    } else {
        unk_801512e0(party);
        party_force_reset_outofscreen(party);
        if ((s32)*(u8*)(pParty + 0x31) == 7 &&
            (*(u32*)(pParty + 4) & 0x8000) == 0 &&
            marioKeyOffChk() == 0 && marioChkDeepSleep() != 1 &&
            chuchu_searchObject() != 0) {
            switch ((s32)*(u8*)(pParty+0x39)) {
            case 1:
                if (--*(s32*)(pParty+0x24)<=0) {partyChgPoseId(party,1);*(s32*)(pParty+0x24)=30;*(u8*)(pParty+0x39)=2;}
                break;
            case 2:
                if (--*(s32*)(pParty+0x24)<=0) *(u8*)(pParty+0x39)=0;
                break;
            default:
                partyChgPose(party,(char*)base+0xCC);*(u8*)(pParty+0x39)=1;*(s32*)(pParty+0x24)=30;
            }
        }
    }
done:;
}

/* fallback stub-fill: map=unk_8015146c addr=0x8015146c size=0x000001e0 */
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
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
    s32 state;

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
        if ((s32)*(u8*)((s32)party + 0x31) == 7 && (*(u32*)((s32)party + 4) & 0x8000) == 0 && marioKeyOffChk() == 0 && marioChkDeepSleep() != 1 && chuchu_searchObject() != 0) {
            state = *(u8*)((s32)party + 0x39);
            switch (state) {
            case 1:
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    partyChgPoseId(party, 1);
                    *(s32*)((s32)party + 0x24) = 0x1E;
                    *(u8*)((s32)party + 0x39) = 2;
                }
                break;
            case 2:
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    *(u8*)((s32)party + 0x39) = 0;
                }
                break;
            default:
                partyChgPose(party, str_PCH_A2_1_802f46f4);
                *(u8*)((s32)party + 0x39) = 1;
                *(s32*)((s32)party + 0x24) = 0x1E;
            }
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
/* fallback stub-fill: map=unk_801512e0 addr=0x801512e0 size=0x0000018c */
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
void unk_801512e0(void* pParty) {
    extern void* partySearchHead(f32 dir, void* party, void* pos, f32* height);
    extern void* partySearchGround(f32 yOffHi, f32 yOffLo, void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern s32 unk_800cb9bc(void* hit);
    extern void partyGetAppearPos5(void* party, void* hit, void* pos);
    typedef struct VecLocal { f32 x,y,z; } VecLocal;
    VecLocal pos;
    f32 height;
    void* hit;
    u32 attr;

    pos=*(VecLocal*)((s32)pParty+0x58);
    height = *(f32*)((s32)pParty + 0xF0);
    hit = partySearchHead(*(f32*)((s32)pParty+0x10C),pParty,&pos,&height);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x140) = hit;
    }
    if ((*(u32*)pParty & 0x04000000) == 0) {
        *(u32*)pParty &= ~0x80;
        hit = partySearchGround(float_15_80423130,float_neg15_8042319c,pParty);
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
            *(u32*)pParty &= ~2;
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1p5_804231a0;
        } else {
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
            *(u32*)pParty |= 2;
            if ((attr & 0x100) != 0) {
                *(u32*)pParty |= 0x80;
            } else {
                *(u32*)pParty &= ~0x80;
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset



/* CHATGPT FALLBACK MISSING STUBS: main/party/party_motion 20260624_191429 */

/* fallback stub-fill: map=unk_80150fac addr=0x80150fac size=0x00000334 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#undef float_10_804230a4
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

    extern const f32 float_10_804230a4[1];
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
        pos=*(VecLocal*)((s32)pParty+0x58);
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

    hit = partySearchGround(float_10_804230a4[0], float_0_804230bc, pParty);
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

    {
        void* active=*(void**)((s32)pParty+0x160);
        u16 motion;
        if(*(s16*)((s32)active+0x50)!=0 && float_0_804230bc==*(f32*)((s32)active+0x180)) adjust=0;
        else {
            motion=*(u16*)((s32)active+0x2E);
            if(motion==0x1F || motion==0x20 || motion==0x10 || motion==0x11) adjust=0;
            else adjust=1;
        }
    }
    if(adjust!=0) {
        if(hit!=0) {
            playerY=*(f32*)((s32)player+0x90); groundY=*(f32*)((s32)pParty+0xE4);
            if(playerY>groundY) {baseY=playerY;rate=float_0p1_80423114;}
            else {baseY=groundY;rate=float_0p1_80423114;}
            partyY=*(f32*)((s32)pParty+0x5C);
            if(partyY<groundY) {baseY=groundY;rate=float_0p2_80423194;}
            offset=float_5_804230e8;
            if((*(u32*)((s32)pParty+4)&0x8000)!=0) offset=float_1_804230ec;
            diff=(baseY+offset)-partyY;
            if(float_neg5_80423180>diff || diff>=float_0_804230bc)
                *(f32*)((s32)pParty+0x5C)=diff*rate+*(f32*)((s32)pParty+0x5C);
        } else if(*(f32*)((s32)pParty+0xE8)>=float_0_804230bc) {
            playerY=*(f32*)((s32)player+0x90); groundY=*(f32*)((s32)pParty+0xE4);
            if(playerY>groundY) rate=float_0p1_80423114;
            else {playerY=groundY;rate=float_0p1_80423114;}
            partyY=*(f32*)((s32)pParty+0x5C);
            if(partyY<groundY) {playerY=groundY;rate=float_0p2_80423194;}
            offset=float_5_804230e8;
            if((*(u32*)((s32)pParty+4)&0x8000)!=0) offset=float_1_804230ec;
            diff=(playerY+offset)-partyY;
            if(float_neg5_80423180>diff || diff>=float_0_804230bc)
                *(f32*)((s32)pParty+0x5C)=diff*rate+*(f32*)((s32)pParty+0x5C);
        }
    } else {
        rate=float_0p05_80423174;
        if(hit!=0) {
            playerY=*(f32*)((s32)player+0x90); groundY=*(f32*)((s32)pParty+0xE4);
            if(playerY>groundY) {} else playerY=groundY;
            partyY=*(f32*)((s32)pParty+0x5C);
            if(partyY<groundY) {playerY=float_18p5_80423198+groundY;rate=float_0p2_80423194;}
            offset=float_5_804230e8;
            if((*(u32*)((s32)pParty+4)&0x8000)!=0) offset=float_1_804230ec;
            diff=(playerY+offset)-partyY;
            if(float_neg5_80423180>diff || diff>=float_0_804230bc)
                *(f32*)((s32)pParty+0x5C)=diff*rate+*(f32*)((s32)pParty+0x5C);
        }
    }
}
#define float_10_804230a4 10.0f

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

    void* player;
    void* leadParty;
    void* hit;
    FootmarkLocal* footmark;
    u8* base;
    extern void* marioGetPtr(void);
    extern void L_getTargetPos(s32,VecLocal*);
    extern void partyGetReadId(s32,f32*,f32*,f32*);
    extern f32 toMovedir(f32);
    extern f32 partyToMovedir(f32,void*);
    extern char* hitGetName(void*);
    HitWorkLocal probe, jumpProbe, frontProbe;
    VecLocal moveVec, target, jumpTarget, headPos, current, loopCurrent, loopInitial, jumpStart;
    f32 moveSpeed,angle,secondSin,secondCos;
    f32 jumpX,jumpY,jumpZ;
    f32 stepY,stepOut2,stepOut3,headHeight;
    f32 cosValue,sinValue,jumpCos,jumpSin,frontCos,frontSin;
    f32 firstOverride,secondOverride;
    f32 closeDist,targetDist,speed,distance,distance3D,extraSpeed,radius;
    f32 startX,startY,startZ,frontDiff,wallMetric,groundY,jumpDir,jumpHeight;
    void *ground,*wall,*headHit;
    u32 attr;
    s32 footmarkId;
    s32 leadId;
    s32 status;
    s32 i;
    s32 readId;
    s32 writeId;
    s32 canMove;
    u16 motion;

    base=(u8*)vec3_802f4628;
    ground=0;
    footmarkId = partyGetFootmarkId((void*)pParty);
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

    if ((strcmp((char*)gp + 0x12C, (char*)base + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)base + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)base + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            firstOverride = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                firstOverride = float_30_804230f8;
            } else {
                firstOverride = float_8_804230f4;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                firstOverride = float_30_804230f8;
            } else {
                firstOverride = float_8_804230f4;
            }
        }
        closeDist=firstOverride;
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
            secondOverride = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                secondOverride = float_30_804230f8;
            } else {
                secondOverride = float_8_804230f4;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
                secondOverride = float_30_804230f8;
            } else {
                secondOverride = float_8_804230f4;
            }
        }
        targetDist=secondOverride;
    }

    if (party_slit((void*)pParty) == 0 || party_roll((void*)pParty) == 0) {
        return;
    }

    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)(void*)pParty & 0x80000) == 0) {
        partyMoveBehindMario((void*)pParty, 0);
        return;
    }

    if (*(s16*)((s32)player + 0x50) == 0 ||
        *(f32*)((s32)player + 0x180) != float_0_804230bc) {
        motion = *(u16*)((s32)player + 0x2E);
        if (motion == 0x1F || motion == 0x20 ||
            motion == 0x10 || motion == 0x11) {
            canMove = 0;
        } else {
            canMove = 1;
        }
    } else {
        canMove = 0;
    }

    if (!canMove) {
        partyChgMot((void*)pParty, 0);
        return;
    }

    speed = partyGetTargetMovespd2((void*)pParty);

    if ((*(u32*)(void*)pParty & 0x8) == 0) {
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

        unk_800cbfbc(speed, (void*)pParty);
    } else {
        leadId = marioGetPartyId();
        leadParty = partyGetPtr(leadId);

        if (leadParty != 0) {
            status = nokonokoGetStatus(leadParty);

            moveVec=*(VecLocal*)((s32)leadParty+0x58);
            if(status==3 || PSVECDistance((void*)((s32)player+0x8C),(void*)((s32)leadParty+0x58))>=float_150_80423188) {
                distance=PSVECDistance((void*)((s32)player+0x8C),(void*)(pParty+0x58));
                if(distance<float_45_804230e0) speed=float_1_804230ec;
                else if(distance>=float_50_804230b8) speed=float_3_804230dc;
                if((*(u32*)*(void**)(pParty+0x160)&0x8000)!=0) speed=float_1_804230ec;
                unk_800cbfbc(speed,(void*)pParty);
            } else {
                distance=PSVECDistance((void*)((s32)leadParty+0x58),(void*)(pParty+0x58));
                if(distance<float_45_804230e0) speed=float_1_804230ec;
                else if(distance>=float_50_804230b8) speed=float_3_804230dc;
                if((*(u32*)*(void**)(pParty+0x160)&0x8000)!=0) speed=float_1_804230ec;
                unk_800cbfbc(speed,(void*)pParty);
            }

        }
    }

    startX = *(f32*)(pParty + 0x58);
    startY = *(f32*)(pParty + 0x5C);
    startZ = *(f32*)(pParty + 0x60);


    /*
     * The target checks up to eleven footmarks, skipping candidates whose
     * short vertical probe lands on a 0x800 floor attribute.
     */
    for (i = 0; i <= 10; i++) {
        footmark = (FootmarkLocal*)partyGetFootmarkPtr(footmarkId);
    target=footmark->position;

        angle = angleABf(startX, startZ, target.x, target.z);
        sincosf(angle, &sinValue, &cosValue);

        probe.start.x = float_20_80423090 * sinValue + *(f32*)(pParty + 0x58);
        probe.start.z = float_20_80423090 * cosValue + *(f32*)(pParty + 0x60);
        probe.start.y = float_20_80423090 + *(f32*)(pParty + 0x5C);
        probe.dir=*(VecLocal*)(base+0x90);
        probe.dist = float_40_804230c0;

        hit = (void*)hitCheckVecFilter(&probe, 0);
        if (hit == 0 || (hitGetAttr(hit) & 0x800) == 0) {
            break;
        }

        partyNextReadId(footmarkId);
        partyGetReadId2(footmarkId, &target);
    }
    if(i>10) target=*(VecLocal*)((s32)marioGetPtr()+0x8C);

    current=*(VecLocal*)(pParty+0x58);

    distance3D = partyGetTargetDist3D(footmarkId, &current);
    if (distance3D >= closeDist) {
        unk_800cbfbc(speed, (void*)pParty);
    }
    if (targetDist < float_20_80423090) {
        unk_800cbfbc(speed, (void*)pParty);
    }

    for (;;) {
        distance = distABf(startX, startZ, target.x, target.z);
        angle = angleABf(startX, startZ, target.x, target.z);

        if ((*(u32*)player & 0x8000) != 0) {
            radius = *(f32*)(pParty + 0xF4) +
                     *(f32*)((s32)player + 0x1B8);
        } else {
            radius = *(f32*)(pParty + 0xF4) * float_0p5_804230d0 +
                     *(f32*)(pParty + 0x104);
        }

        if (distance > radius) {
            loopInitial=*(VecLocal*)base;
            loopInitial.x=startX; loopInitial.y=startY; loopInitial.z=startZ;
            loopCurrent=loopInitial;
            distance3D = partyGetTargetDist3D(footmarkId, &loopCurrent);

            if (distance3D >= closeDist) {
                if((*(u32*)(pParty+8)&0x20000)!=0) {
                    extraSpeed=(distance3D-targetDist)*float_0p1_80423114;
                    if(extraSpeed>float_4_8042317c) extraSpeed=float_4_8042317c;
                    if(extraSpeed<float_0p5_804230d0) extraSpeed=float_0_804230bc;
                    unk_800cbf84(*(f32*)((s32)player+0x180)+extraSpeed,(void*)pParty);
                } else {
                    extraSpeed=(distance3D-closeDist)*float_0p1_80423114;
                    if(extraSpeed>float_4_8042317c) extraSpeed=float_4_8042317c;
                    if(extraSpeed<float_0p5_804230d0) extraSpeed=float_0_804230bc;
                    unk_800cbf84(*(f32*)((s32)player+0x180)+extraSpeed,(void*)pParty);
                }

                goto common_motion;
            }

            if (marioGetSlitFloorPos(&moveVec) != 0) {
                unk_800cbfbc(float_1p5_8042316c, (void*)pParty);
                partyNextReadId(footmarkId);

                if (marioGetSlitFloorPos(&moveVec) != 0) {
                    jumpTarget = moveVec;
                    closeDist = distABf(
                        startX, startZ, moveVec.x, moveVec.z);
                    angle = angleABf(
                        startX, startZ, jumpTarget.x, jumpTarget.z);

                    if (closeDist <= float_5_804230e8) {
                        partyChgMotSlitFloor((void*)pParty);
                    }
                }

                goto common_motion;
            }

            if (distance3D > targetDist) {
                if((*(u32*)(pParty+8)&0x20000)!=0) {
                    extraSpeed = (distance3D - targetDist) * 0.1f;

                    if (extraSpeed > 4.0f) {
                        extraSpeed = 4.0f;
                    }
                    if (extraSpeed < 0.5f) {
                        extraSpeed = 0.0f;
                    }

                    unk_800cbf84(
                        *(f32*)((s32)player + 0x180) + extraSpeed,
                        (void*)pParty);
                } else {
                    extraSpeed = distance3D - targetDist;

                    if (extraSpeed >= float_1_804230ec) {
                        extraSpeed = float_1_804230ec;
                        if (targetDist < float_20_80423090) {
                            extraSpeed = speed;
                        }
                    }

                    unk_800cbfbc(extraSpeed, (void*)pParty);
                }

                goto common_motion;
            }

            if (*(f32*)(pParty + 0xA0) == 0.0f &&
                *(f32*)(pParty + 0xA8) == 0.0f) {
                unk_800cbfbc(
                    *(f32*)((s32)player + 0x180),
                    (void*)pParty);
            } else {
                unk_800cbf84(0.0f, (void*)pParty);
                *(f32*)(pParty + 0x100) =
                    angleABf(
                        *(f32*)(pParty + 0x58),
                        *(f32*)(pParty + 0x60),
                        *(f32*)((s32)player + 0x8C),
                        *(f32*)((s32)player + 0x94));
                partyChgPoseId((void*)pParty, 1);
            }
        }

        writeId = partyGetFootmarkWriteId(footmarkId);
        readId = partyGetFootmarkReadId(footmarkId);

        if (readId == writeId) {
            unk_800cbfbc(
                *(f32*)((s32)*(void**)(pParty+0x160) + 0x180),
                (void*)pParty);

            if (*(f32*)((s32)*(void**)(pParty+0x160) + 0x180) == 0.0f ||
                *(f32*)(pParty + 0xA0) != 0.0f ||
                *(f32*)(pParty + 0xA8) != 0.0f) {
                angle = *(f32*)(pParty + 0x100);
                unk_800cbfbc(0.0f, (void*)pParty);
                *(f32*)(pParty + 0x11C) = float_0_804230bc;

                if ((*(u32*)pParty & 0x200) == 0) {
                    partyChgPoseId((void*)pParty, 1);
                }

                partyChgMot((void*)pParty, 0);
            }

            goto common_motion;
        }

        partyNextReadId(footmarkId);
        partyGetReadId2(footmarkId, &target);

        if ((*(u32*)pParty & 0x04000000) == 0) {
            *(u32*)pParty &= ~0x80;
            ground = partySearchGround(10.0f, 0.0f, (void*)pParty);

            if (ground != 0 && (hitGetAttr(ground) & 0x200) != 0) {
                ground = 0;
                if (*(s8*)(pParty + 0x4C) == 0) {
                    *(u32*)(pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
            }

            *(void**)(pParty + 0x138) = ground;

            if (ground == 0) {
                partyChgMot((void*)pParty, 3);
                *(f32*)(pParty + 0x11C) = float_neg1_804230c8;
            } else {
                attr = hitGetAttr(ground);

                if ((attr & 0x100) != 0) {
                    *(u32*)pParty |= 0x80;
                } else {
                    *(u32*)pParty &= ~0x80;
                }

                if ((attr & 0x800) != 0) {
                    partyChgMot((void*)pParty, 0);
                    goto common_motion;
                }
            }
        }

        if ((*(u32*)pParty&2)!=0 && (footmark->flags&1)!=0) {
            jumpDir=toMovedir(*(f32*)(pParty+0x110));
            partyGetReadId(footmarkId,&jumpX,&jumpY,&jumpZ);
            L_getTargetPos(footmarkId,&jumpTarget);
            if(jumpTarget.y<jumpY) jumpY=jumpTarget.y;
            *(f32*)(pParty+0x94)=jumpX; *(f32*)(pParty+0x9C)=jumpZ; *(f32*)(pParty+0x98)=jumpY;
            jumpStart=*(VecLocal*)(pParty+0x94);
            wallMetric=touch_wall_ct;
            sincosf(jumpDir,&jumpSin,&jumpCos);
            jumpProbe.start=jumpStart;
            jumpProbe.start.x+=jumpSin*float_40_804230c0;
            jumpProbe.start.y+=wallMetric;
            jumpProbe.start.z+=jumpCos*float_40_804230c0;
            jumpProbe.dist=float_1000_804230c4;
            jumpProbe.dir=*(VecLocal*)(base+0x84);
            hit=(void*)hitCheckVecFilter(&jumpProbe,0);
            if(hit!=0) {
                f32 rounding;
                if(jumpProbe.hitPos.y>=float_0_804230bc) rounding=float_0p5_804230d0;
                else rounding=float_neg0p5_804230d4;
                jumpHeight=(f32)(s32)(float_1000_804230c4*jumpProbe.hitPos.y+rounding)/float_1000_804230c4;
                hitGetAttr(hit);
            } else jumpHeight=float_neg2000_804230cc;
            if(jumpHeight>*(f32*)(pParty+0x5C)) {
                angle=partyToMovedir(*(f32*)(pParty+0x110),(void*)pParty);
                sincosf(angle,&secondSin,&secondCos);
                *(f32*)(pParty+0x94)=secondSin*float_40_804230c0;
                *(f32*)(pParty+0x9C)=secondCos*float_40_804230c0;
                *(f32*)(pParty+0x98)=jumpHeight;
                partyChgMot((void*)pParty,2); goto common_motion;
            }
        } else if (unk_800cb9bc(ground) != 0) {
            partyGetAppearPos5((void*)pParty,ground,(void*)(pParty+0x94));
            *(u32*)(pParty+8)|=0x00400000;
            partyChgMot((void*)pParty,2); goto common_motion;
        }
        if ((*(u32*)pParty&2)!=0 && (footmark->flags&4)!=0) {
            distance=distABf(startX,startZ,target.x,target.z);
            if(distance<=float_5_804230e8) partyChgMotSlitFloor((void*)pParty);
            goto common_motion;
        }

    }

common_motion:
    if(*(f32*)(pParty+0x104)!=float_0_804230bc) unk_800cbb10(*(f32*)(pParty+0x104),(void*)pParty);
    else if((footmark->flags&4)==0) partyChgMot((void*)pParty,0);

    *(f32*)(pParty + 0x100) = angle;

    moveVec=*(VecLocal*)(pParty+0x58);

    partyGetMoveDirSpd((void*)pParty, &angle, &moveSpeed);
    wall = partySearchFrontWall(
        moveSpeed, angle, (void*)pParty, &moveVec);

    if ((*(u32*)pParty & 0x01000000) != 0) {
        wall = 0;
    }

    if (wall == 0) {
        *(u8*)(pParty + 0x4D) = 0;

        if ((*(u32*)(pParty + 8) & 0x00200000) == 0) {
            getFrontFloor(
                *(f32*)(pParty + 0x100),
                (void*)pParty,
                (f32*)&target);
        } else {
            *(f32*)(pParty + 0x54) =
                revise360(
                    float_270_80423104 +
                    *(f32*)((s32)player + 0x19C));
            partyGetAppearPos4((void*)pParty, &target);
        }

        frontDiff = target.y - *(f32*)(pParty + 0x5C);

        if (target.y > *(f32*)(pParty + 0x5C)) {
            if ((double)frontDiff > 1.001 &&
                frontDiff < 60.0f) {
                *(f32*)(pParty + 0x94) = target.x;
                *(f32*)(pParty + 0x98) = target.y;
                *(f32*)(pParty + 0x9C) = target.z;
                partyChgMot((void*)pParty, 2);
                return;
            }

            if (target.y > *(f32*)(pParty + 0x5C)) {
                *(f32*)(pParty + 0x5C) = target.y;
            }
        }

        if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
            sincosf(
                *(f32*)(pParty + 0x100),
                &frontSin,
                &frontCos);

            frontProbe.start.x =
                frontSin * float_40_804230c0 +
                *(f32*)(pParty + 0x58);
            frontProbe.start.z =
                frontCos * float_40_804230c0 +
                *(f32*)(pParty + 0x60);

            frontProbe.start.y =
                *(f32*)(pParty + 0x5C) + float_40_804230c0;
            frontProbe.dir=*(VecLocal*)(base+0x9C);
            frontProbe.dist = float_100_804230b0;

            hit = (void*)hitCheckVecFilter(&frontProbe, 0);

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

                    if (wallMetric >= float_1_804230ec) {
                        getFrontFloor(
                            *(f32*)(pParty + 0x100),
                            (void*)pParty,
                            (f32*)&target);

                        frontDiff =
                            target.y -
                            *(f32*)(pParty + 0x5C);

                        if (frontDiff > float_5_804230e8) {
                            *(f32*)(pParty + 0x94) =
                                target.x;
                            *(f32*)(pParty + 0x98) =
                                target.y;
                            *(f32*)(pParty + 0x9C) =
                                target.z;
                            partyChgMot((void*)pParty, 2);
                            return;
                        }

                        if ((*(u32*)pParty & 0x30) == 0) {
                            partyChkFrontStep(
                                11.0f, (void*)pParty,
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
                                partyChgMot((void*)pParty, 6);
                                return;
                            }
                        }
                    } else {
                        angle = partyGetTargetDir(
                            *(f32*)(pParty + 0x58),
                            *(f32*)(pParty + 0x60),
                            footmarkId);

                        getFrontFloor(
                            angle, (void*)pParty, (f32*)&target);

                        frontDiff =
                            target.y -
                            *(f32*)(pParty + 0x5C);

                        if (frontDiff > float_100_804230b0) {
                            *(f32*)(pParty + 0x58) =
                                target.x;
                            *(f32*)(pParty + 0x5C) =
                                target.y;
                            *(f32*)(pParty + 0x60) =
                                target.z;
                            partyChgMot((void*)pParty, 1);
                        } else if (frontDiff >= float_0_804230bc) {
                            *(f32*)(pParty + 0x94) =
                                target.x;
                            *(f32*)(pParty + 0x98) =
                                target.y;
                            *(f32*)(pParty + 0x9C) =
                                target.z;
                            partyChgMot((void*)pParty, 2);
                        } else {
                            partyChgMot((void*)pParty, 0);
                        }

                    }
                }
            }
        }
    }

    *(f32*)(pParty + 0x58) = moveVec.x;
    *(f32*)(pParty + 0x60) = moveVec.z;

    headPos=*(VecLocal*)(pParty+0x58);
    headHeight = *(f32*)(pParty + 0xF0);

    headHit = partySearchHead(
        *(f32*)(pParty + 0x10C),
        (void*)pParty,
        &headPos,
        &headHeight);

    if (headHit != 0) {
        *(void**)(pParty + 0x140) = headHit;
    }

    *(u32*)pParty &= ~0x80;

    ground = partySearchGround(
        10.0f, 10.0f, (void*)pParty);
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
            partyChgRunMode((void*)pParty, 0xD);
            return;
        }
    }

    *(void**)(pParty + 0x138) = ground;

    if (ground == 0) {
        partyChgMot((void*)pParty, 3);
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
    party_force_reset_outofscreen((void*)pParty);
    {
        void* finalPlayer=*(void**)(pParty+0x160);
        if(strcmp((char*)gp+0x12C,(char*)base+0xD8)==0 &&
           *(void**)((s32)finalPlayer+0x1E8)!=0 && *(void**)(pParty+0x138)!=0 &&
           strcmp(hitGetName(*(void**)((s32)finalPlayer+0x1E8)),(char*)base+0xE0)==0 &&
           strcmp(hitGetName(*(void**)(pParty+0x138)),(char*)base+0xF4)==0) {
            *(VecLocal*)(pParty+0x58)=*(VecLocal*)(base+0xC0);
        }
    }


    return;
}

#undef float_10_804230a4
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


    extern const f32 float_10_804230a4[1];
    void* party;
    void* player;
    FootmarkLocal* footmark;
    void* ground;
    void* wall;
    void* hit;
    void* headHit;

    VecLocal movePosLocal;
    VecLocal slitTarget;
    VecLocal appearPos;
    VecLocal frontFloor;
    VecLocal headPos;
    VecLocal initialCurrent;
    VecLocal loopCurrent;
    VecLocal initialPosition;
    f32 direction, moveSpeed;
    f32 targetX, targetY, targetZ;
    f32 probeX, probeZ;
    f32 headHeight, unusedHeight;
    f32 hitOut6, hitOut5, hitOut4, hitMaxDist, hitOut2, hitOutY, hitOut0;

    f32 closeDist;
    f32 targetDist;
    f32 overrideDist;
    f32 secondOverride;
    f32 speed;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 distance;
    f32 distance3D;
    f32 extraSpeed;
    f32 radius;
    f32 groundY;
    f32 wallHeight;
    f32 deltaY;
    f32 absDelta;

    u8* base;
    s32 quantized;
    s32 footmarkId;
    s32 writeId;
    s32 readId;
    u16 motion;
    u32 attr;
    s32 farFollow;

    party = (void*)pParty;
    base=(u8*)vec3_802f4628;
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

    if ((strcmp((char*)gp + 0x12C, (char*)base + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)base + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)base + 0x198) == 0) &&
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
            secondOverride = float_18_804230f0;
        } else if (*(s8*)(pParty + 0x31) == 5) {
            if ((*(u32*)(pParty + 8) & 0x00200000) != 0) {
                secondOverride = float_8_804230f4;
            } else {
                secondOverride = float_30_804230f8;
            }
        } else if (*(s8*)(pParty + 0x31) == 6) {
            if ((*(u32*)(pParty + 8) & 0x00200000) != 0) {
                secondOverride = float_8_804230f4;
            } else {
                secondOverride = float_30_804230f8;
            }
        }

        targetDist = secondOverride;
    }

    if (party_slit(party) == 0 || party_roll(party) == 0) {
        return 0;
    }

    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x00080000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }

    {
        void* movementPlayer=*(void**)(pParty+0x160);
        s32 canMove;
        if (*(s16*)((s32)movementPlayer+0x50)!=0 &&
            float_0_804230bc==*(f32*)((s32)movementPlayer+0x180)) {
            canMove=0;
        } else {
            motion=*(u16*)((s32)movementPlayer+0x2E);
            if(motion==0x1F || motion==0x20 || motion==0x10 || motion==0x11) canMove=0;
            else canMove=1;
        }
        if(!canMove) { partyChgMot(party,0); return 0; }
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

    if ((*(u32*)*(void**)(pParty+0x160) & 0x00008000) != 0) {
        speed = float_1_804230ec;
        closeDist = float_100_804230b0;
    }

    unk_800cbfbc(speed, party);

    startX = *(f32*)(pParty + 0x58);
    startY = *(f32*)(pParty + 0x5C);
    startZ = *(f32*)(pParty + 0x60);

    footmark = (FootmarkLocal*)partyGetFootmarkPtr(footmarkId);
    targetX = footmark->position.x;

    {
        f32 lift=float_5_804230e8;
        if((*(u32*)(pParty+4)&0x8000)!=0) lift=float_1_804230ec;
        targetY=footmark->position.y+lift;
    }

    targetZ = footmark->position.z;

    initialCurrent = *(VecLocal*)(pParty + 0x58);
    distance3D = partyGetTargetDist3D(footmarkId, &initialCurrent);

    if (distance3D >= closeDist) {
        unk_800cbfbc(speed, party);
    }

    if (targetDist < float_20_80423090) {
        unk_800cbfbc(speed, party);
    }

    farFollow = 0;

    for (;;) {
        direction = angleABf(startX, startZ, targetX, targetZ);
        distance = distABf(startX, startZ, targetX, targetZ);

        if ((*(u32*)player & 0x00008000) != 0) {
            radius = *(f32*)(pParty + 0xF4) +
                     *(f32*)((s32)player + 0x1B8);
        } else {
            radius = float_0p5_804230d0 * *(f32*)(pParty + 0xF4) +
                     *(f32*)(pParty + 0x104);
        }

        if (distance > radius) {
            initialPosition = *(VecLocal*)(base + 0xC);
            initialPosition.x=startX; initialPosition.y=startY; initialPosition.z=startZ;
            loopCurrent=initialPosition;
            distance3D = partyGetTargetDist3D(footmarkId, &loopCurrent);

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

            if (marioGetSlitFloorPos(&movePosLocal) != 0) {
                unk_800cbfbc(float_1p5_8042316c, party);
                partyNextReadId(footmarkId);

                if (marioGetSlitFloorPos(&movePosLocal) != 0) {
                    slitTarget = movePosLocal;
                    distance = distABf(
                        startX, startZ,
                        movePosLocal.x, movePosLocal.z);
                    direction = angleABf(
                        startX, startZ,
                        slitTarget.x, slitTarget.z);

                    if (distance <= float_5_804230e8) {
                        partyChgMotSlitFloor(party);
                    }
                }

                goto common_motion;
            }

            if (distance3D > targetDist) {
                if ((*(u32*)(pParty + 8) & 0x00020000) != 0) {
                    extraSpeed =
                        float_0p1_80423114 *
                        (distance3D - targetDist);

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
                    extraSpeed = distance3D - targetDist;

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
                    *(f32*)((s32)*(void**)(pParty+0x160) + 0x180),
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
            &targetX,
            &targetY,
            &targetZ);

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
        void* verticalPlayer = *(void**)(pParty + 0x160);
        partyGetAppearPos3(party, &appearPos);

        direction =
            angleABf(
                *(f32*)(pParty + 0x58),
                *(f32*)(pParty + 0x60),
                appearPos.x,
                appearPos.z);

        ground = partySearchGround(
            float_10_804230a4[0],
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
            {
                f32 lift=float_5_804230e8;
                if((*(u32*)(pParty+4)&0x8000)!=0) lift=float_1_804230ec;
                deltaY=(*(f32*)((s32)verticalPlayer + 0x90)+lift)-*(f32*)(pParty+0x5C);
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
            {
                f32 lift=float_5_804230e8;
                if((*(u32*)(pParty+4)&0x8000)!=0) lift=float_1_804230ec;
                deltaY=(frontFloor.y+lift)-*(f32*)(pParty+0x5C);
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
                if (*(u8*)(pParty+0x37)!=1) {
                    void* wallPlayer=*(void**)(pParty+0x160);
                    if((*(u32*)((s32)wallPlayer+0xC)&0x80000000)==0) {
                        *(u32*)(pParty+8)|=1; *(u8*)(pParty+0x37)=1;
                    }
                } else if(*(f32*)(pParty+0x104)!=float_0_804230bc) {
                    movePosLocal.x=*(f32*)(pParty+0x58);
                    movePosLocal.z=*(f32*)(pParty+0x60);
                    movePos(*(f32*)(pParty+0x104),*(f32*)(pParty+0x100),&movePosLocal.x,&movePosLocal.z);
                }

            } else {
                probeX = *(f32*)(pParty + 0x58);
                probeZ = *(f32*)(pParty + 0x60);
                groundY = *(f32*)(pParty + 0x5C);

                movePos(
                    *(f32*)(pParty + 0xF4),
                    direction,
                    &probeX,
                    &probeZ);

                hitMaxDist = float_1000_804230c4;

                hit = hitCheckFilter(
                    probeX,
                    groundY + touch_wall_ct,
                    probeZ,
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

                    wallHeight=(f32)quantized / float_100_804230b0;

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

                        absDelta = __fabsf(absDelta);

                        if (absDelta >= float_10_804230a4[0]) {
                            deltaY =
                                (float_10_804230a4[0] +
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
            (char*)base + 0x1A0) == 0) {
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
        float_10_804230a4[0],
        float_10_804230a4[0],
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

#define float_10_804230a4 10.0f

void party_set_behind_dir(void* party, f32 dir) {
    void* cam = camGetPtr(4);

    *(f32*)((s32)party + 0x54) = revise360(dir + *(f32*)((s32)cam + 0x114));
    *(u32*)((s32)party + 8) |= 0x100000;
}

#undef float_10_804230a4
extern f32 __fabsf(f32);
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
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
    extern const f32 float_10_804230a4[1];
    void *party, *player, *heading, *lead, *hit;
    Vec target;
    f32 dir, targetY, distance, limit, speed, ground;
    f32 x,z;
    f32 a6,a5,a4,aDist,a2,aY,a0;
    f32 b6,b5,b4,bDist,b2,bY,b0;
    f32 c6,c5,c4,cDist,c2,cY,c0;
    party=(void*)pParty;
    player=*(void**)((s32)party+0x160);
    if((*(u32*)((s32)party+8)&2)!=0){
        *(u32*)((s32)party+8)&=~2;
        *(u32*)((s32)party+8)|=0x00040000;
        if((*(u32*)((s32)party+8)&0x00100000)==0){
            if((*(u32*)((s32)party+8)&0x00010000)==0){
                heading=*(void**)((s32)party+0x160);
                heading=*(void**)((s32)party+0x160);
        if((*(u32*)party&0x8)==0){
                    dir=toMovedir(*(f32*)((s32)heading+0x1AC));
                }else{
                    lead=partyGetPtr(marioGetPartyId());
                    if(lead!=0) dir=partyToMovedir(*(f32*)((s32)lead+0x10C),party);
                    else dir=toMovedir(*(f32*)((s32)heading+0x1AC));
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
        if(*(s32*)((s32)party+0x24)<=0) *(s32*)((s32)party+0x24)=1;
    }

    if(((*(u32*)((s32)party+8)&0x00100000)==0)&&(marioKeyOffChk()!=0)){
        if((*(u32*)party&0x8)==0) dir=toMovedir(*(f32*)((s32)heading+0x1AC));
        else{
            lead=partyGetPtr(marioGetPartyId());
            if(lead!=0) dir=partyToMovedir(*(f32*)((s32)lead+0x10C),party);
            else dir=toMovedir(*(f32*)((s32)heading+0x1AC));
        }
        *(f32*)((s32)party+0x54)=revise360(dir+float_180_80423108);
    }

    partyGetAppearPos4(party,&target);
    x=target.x; z=target.z; targetY=target.y;
    dir=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),x,z);
        aDist=float_1000_804230c4;
        hit=hitCheckFilter(x,targetY+touch_wall_ct,z,float_0_804230bc,float_neg1_804230c8,float_0_804230bc,
            0,&a0,&aY,&a2,&aDist,&a4,&a5,&a6);
        if(hit==0) { ground=float_neg2000_804230cc; } else {
            ground=(f32)(s32)(float_100_804230b0*aY+float_0p5_804230d0)/float_100_804230b0;
            hitGetAttr(hit);
        }
    if(ground<=float_neg2000_804230cc){
        speed=float_1p5_8042316c * *(f32*)((s32)player+0x1B8);
        movePos(speed,toMovedir(*(f32*)((s32)player+0x1AC)),&x,&z);
    }
    limit=float_2_804230d8;
    if(*(f32*)((s32)player+0x180)>float_1_804230ec) limit=float_10_804230a4[0];
    distance=distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),x,z);
    --*(s32*)((s32)party+0x24);
    if(distance>=limit && *(s32*)((s32)party+0x24)>0){
        *(f32*)((s32)party+0x100)=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),x,z);
        distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),x,z);
        if((*(u32*)((s32)party+8)&0x100000)!=0) speed=float_3p6_80423170;
        else speed=float_2_804230d8;
        unk_800cbf84(speed,party);
        partyMoveCheckWall(party);
        unk_800cbb10(*(f32*)((s32)party+0x104),party);
        if(*(void**)((s32)party+0x12C)!=0){
            x=*(f32*)((s32)party+0x58); z=*(f32*)((s32)party+0x60);
            movePos(distance,dir,&x,&z);
            angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),x,z);
        bDist=float_1000_804230c4;
        hit=hitCheckFilter(x,targetY+touch_wall_ct,z,float_0_804230bc,float_neg1_804230c8,float_0_804230bc,
            0,&b0,&bY,&b2,&bDist,&b4,&b5,&b6);
        if(hit==0) { ground=float_neg2000_804230cc; } else {
            ground=(f32)(s32)(float_100_804230b0*bY+float_0p5_804230d0)/float_100_804230b0;
            hitGetAttr(hit);
        }
            if(__fabsf(*(f32*)((s32)party+0x5C)-ground)>float_1_804230ec){
                *(f32*)((s32)party+0x94)=x; *(f32*)((s32)party+0x98)=ground; *(f32*)((s32)party+0x9C)=z;
                partyChgMot(party,2); goto finished;
            }
        } else if(targetY>*(f32*)((s32)party+0x5C)){
            x=*(f32*)((s32)party+0x58); z=*(f32*)((s32)party+0x60);
            movePos(distance,dir,&x,&z);
            angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),x,z);
        cDist=float_1000_804230c4;
        hit=hitCheckFilter(x,targetY+touch_wall_ct,z,float_0_804230bc,float_neg1_804230c8,float_0_804230bc,
            0,&c0,&cY,&c2,&cDist,&c4,&c5,&c6);
        if(hit==0) { ground=float_neg2000_804230cc; } else {
            ground=(f32)(s32)(float_100_804230b0*cY+float_0p5_804230d0)/float_100_804230b0;
            hitGetAttr(hit);
        }
            if(__fabsf(*(f32*)((s32)party+0x5C)-ground)>float_1_804230ec){
                *(f32*)((s32)party+0x94)=x; *(f32*)((s32)party+0x98)=ground; *(f32*)((s32)party+0x9C)=z;
                partyChgMot(party,2); goto finished;
            }
        }
        hit=partySearchGround(float_10_804230a4[0],float_0_804230bc,party);
        if(hit!=0 && (hitGetAttr(hit)&0x200)!=0){
            hit=0;
            if(*(s8*)((s32)party+0x4C)==0){
                *(u32*)((s32)party+8)|=0x01000000;
                *(u32*)party|=0x02000000;
            }
        }
        *(void**)((s32)party+0x138)=hit;
        if(hit==0){ partyChgMot(party,3); *(f32*)((s32)party+0x11C)=float_neg1_804230c8; }
        *(f32*)((s32)party+0x5C)=*(f32*)((s32)party+0xE4);
    }else{
        *(s32*)((s32)party+0x24)=0;
        if((*(u32*)((s32)party+8)&0x200000)!=0) *(u32*)((s32)party+8)&=~0x20000;
        partyChgMoveMode(party,1); partyChgMot(party,0);
        partySetDir(*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94),party);
        *(f32*)((s32)party+0x40)=float_50_804230b8;
        *(u32*)((s32)party+4)&=~0x10;
        *(u32*)party|=0x80000;
        *(u32*)((s32)party+8)&=~0x100000;
        *(u32*)((s32)party+8)&=~0x40000;
        *(u32*)((s32)party+8)&=~0x200000;
    }
    if(*(void**)((s32)party+0x134)!=0){
        *(u32*)party|=0x80000;
        partyChgMoveMode(party,1); partyChgMot(party,0);
        partySetDir(x,z,party);
        *(f32*)((s32)party+0x40)=float_50_804230b8;
        *(u32*)((s32)party+4)&=~0x10;
        *(u32*)((s32)party+8)&=~0x40000;
        *(u32*)((s32)party+8)&=~0x200000;
    }
finished:;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#define float_10_804230a4 10.0f
#pragma no_register_save_helpers on

#pragma use_lmw_stmw off
#undef float_10_804230a4
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

    extern const f32 float_10_804230a4[1];
    void* player;
    void* keyPlayer;
    void* party;
    void* hit;
    f32 dir;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 speed;
    f32 dist;
    f32 followDist;
    f32 out6, out5, out4, hitDist, out2, hitY, out0;
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
        followDist = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(followDist,
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
        followDist = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        targetY = *(f32*)((s32)player + 0x90);
    } else {
        followDist = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
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

    movePos(followDist,
            *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);

    hitDist = float_1000_804230c4;
    hit = hitCheckFilter(targetX, targetY + touch_wall_ct, targetZ,
                         float_0_804230bc, float_neg1_804230c8, float_0_804230bc,
                         0, &out0, &hitY, &out2, &hitDist, &out4, &out5, &out6);
    if (hit == 0) {
        speed = float_neg2000_804230cc;
    } else {
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
        speed = float_10_804230a4[0];
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
    *(s32*)((s32)pParty + 0x24) -= 1;
    if (dist >= speed) {
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        speed = dist / float_10_804230a4[0];
        if (speed > float_2_804230d8) {
            speed = float_2_804230d8;
        }
        if (speed < float_1_804230ec) {
            speed = float_1_804230ec;
        }
        unk_800cbf84(speed, pParty);
        partyMoveCheckWall(pParty);
        unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
        hit = partySearchGround(float_10_804230a4[0], float_0_804230bc, pParty);
        if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
            hit = 0;
            if (*(s8*)((s32)pParty + 0x4C) == 0) {
                *(u32*)((s32)pParty + 8) |= 0x01000000;
                *(u32*)pParty |= 0x02000000;
            }
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
    } else {
        *(s32*)((s32)pParty + 0x24) = 0;
        if (*(f32*)((s32)player + 0x180) == float_0_804230bc) {
            *(u32*)pParty |= 0x80000;
            partyChgMoveMode(pParty, 1);
            partyChgMot(pParty, 0);
            partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
            *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
            *(u32*)((s32)pParty + 4) &= ~0x10;
        }
    }

    if (*(void**)((s32)pParty + 0x134) != 0) {
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(targetX, targetZ, pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x10;
    }
}
#define float_10_804230a4 10.0f

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
extern f32 __fabsf(f32);
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
        if (*(s32*)((s32)pParty + 0x24) <= 0) {
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
    if ((dist >= limit || __fabsf(*(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C)) > float_2_804230d8) &&
        *(s32*)((s32)pParty + 0x24) > 0) {


    *(f32*)((s32)pParty + 0x100) =
        angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                 *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
            *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    if ((*(u32*)((s32)pParty + 8) & 0x00100000) != 0) {
        moveSpeed = float_3p6_80423170;
    } else {
        moveSpeed = float_2_804230d8;
    }
    if (dist < limit) {
        moveSpeed = float_0_804230bc;
        unk_800cbf84(moveSpeed, pParty);
    }
    unk_800cbfbc(moveSpeed, pParty);
    pos = *(VecLocal*)((s32)pParty + 0x58);
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
    } else {
        *(s32*)((s32)pParty + 0x24) = 0;
        if ((*(u32*)((s32)pParty + 8) & 0x00200000) != 0) {
            *(u32*)((s32)pParty + 8) &= ~0x20000;
            if (*(u8*)((s32)pParty + 0x32) == 1) {
                *(u32*)((s32)pParty + 4) &= ~0x10;
            }
        }
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x10;
        unk_800cbf84(float_0_804230bc, pParty);
        *(u32*)pParty |= 0x80000;
        *(u32*)((s32)pParty + 8) &= ~0x00100000;
        *(u32*)((s32)pParty + 8) &= ~0x00040000;
        *(u32*)((s32)pParty + 8) &= ~0x00200000;
        unk_800cba60(pParty);
        if (*(void**)((s32)pParty + 0x134) != 0) {
            *(u32*)pParty |= 0x80000;
            partyChgMoveMode(pParty, 1);
            partyChgMot(pParty, 0);
            partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
            *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
            *(u32*)((s32)pParty + 4) &= ~0x10;
            *(u32*)((s32)pParty + 8) &= ~0x00040000;
        }
        hit = partySearchGround(float_100_804230b0, float_100_804230b0, pParty);
        if (hit != 0) {
            if ((hitGetAttr(hit) & 0x200) != 0) {
                hit = 0;
                if (*(s8*)((s32)pParty + 0x4C) == 0) {
                    *(u32*)((s32)pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
            }
            if (dist > *(f32*)((s32)pParty + 0xF0)) {
                hit = 0;
            }
            y = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0x5C) =
                float_0p05_80423174 * (*(f32*)((s32)pParty + 0xE4) - y) + y;
        }
        *(void**)((s32)pParty + 0x138) = hit;
    }
}
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#undef float_10_804230a4
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

    typedef struct GroundOut { f32 x,y,z,dist,nx,ny,nz; } GroundOut;
    extern const f32 float_10_804230a4[1];
    void* headingPlayer;
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
    f32 outNz, outNy, outNx, outDist, outZ, outY, outX;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        headingPlayer = *(void**)((s32)pParty + 0x160);
        if ((*(u32*)pParty & 0x8) == 0) {
            speed = toMovedir(*(f32*)((s32)headingPlayer + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                speed = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            } else {
                speed = toMovedir(*(f32*)((s32)headingPlayer + 0x1AC));
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(speed + float_90_80423118);
        speed = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
    targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(speed,
                *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             targetX, targetZ);
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) <= 0) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if (marioKeyOffChk() != 0) {
        headingPlayer = *(void**)((s32)pParty + 0x160);
        if ((*(u32*)pParty & 0x8) == 0) {
            speed = toMovedir(*(f32*)((s32)headingPlayer + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party != 0) {
                speed = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            } else {
                speed = toMovedir(*(f32*)((s32)headingPlayer + 0x1AC));
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(speed + float_90_80423118);
    }

    speed = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
    targetX = *(f32*)((s32)player + 0x8C);
    targetZ = *(f32*)((s32)player + 0x94);
    y = *(f32*)((s32)player + 0x90);
    movePos(speed,
            *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);

    outDist = float_1000_804230c4;
    hit = hitCheckFilter(targetX, y + touch_wall_ct, targetZ,
                         float_0_804230bc, float_neg1_804230c8, float_0_804230bc,
                         0, &outX, &outY, &outZ, &outDist, &outNx, &outNy, &outNz);
    if (hit == 0) {
        z = float_neg2000_804230cc;
    } else {
        z = (f32)(s32)(float_100_804230b0 * outY + float_0p5_804230d0) / float_100_804230b0;
        hitGetAttr(hit);
    }
    if (z <= float_neg2000_804230cc) {
        speed = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
    targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(speed,
                toMovedir(*(f32*)((s32)player + 0x1AC)), &targetX, &targetZ);
    }

    speed = float_2_804230d8;
    if (*(f32*)((s32)player + 0x180) > float_1_804230ec) {
        speed = float_10_804230a4[0];
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
    *(s32*)((s32)pParty + 0x24) -= 1;
    if (dist >= speed && *(s32*)((s32)pParty + 0x24) > 0) {
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        speed = dist / float_10_804230a4[0];
        if (speed > float_2_804230d8) {
            speed = float_2_804230d8;
        }
        if (speed < float_1_804230ec) {
            speed = float_1_804230ec;
        }
        unk_800cbf84(speed, pParty);
        partyMoveCheckWall(pParty);
        unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
        hit = partySearchGround(float_10_804230a4[0], float_0_804230bc, pParty);
        if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
            hit = 0;
            if (*(s8*)((s32)pParty + 0x4C) == 0) {
                *(u32*)((s32)pParty + 8) |= 0x01000000;
                *(u32*)pParty |= 0x02000000;
            }
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
    } else {
        *(s32*)((s32)pParty + 0x24) = 0;
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x10;
        *(u32*)pParty |= 0x80000;

    }

    if (*(void**)((s32)pParty + 0x134) != 0) {
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x10;
    }
}
#define float_10_804230a4 10.0f

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

extern f32 __fabsf(f32);
u8 party_motion_jump(s32 pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern s32 partyGetFootmarkId(void* party);
    extern s32 partyGetFootmarkWriteId(s32 id);
    extern s32 partyGetFootmarkReadId(s32 id);
    extern f32 partyGetTargetDist(f32 x, f32 z, s32 id);
    extern f32 partyGetTargetDir(f32 x, f32 z, s32 id);
    extern f32 partyGetTargetDistY(s32 id);
    extern void partyGetReadId(s32 id, f32* x, f32* y, f32* z);
    extern void partyNextReadId(s32 id);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void sincosf(f32 angle, f32* outSin, f32* outCos);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern u32 getFrontFloor(f32 angle, void* party, f32* outPos);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern void* partySearchGround(f32 rise, f32 fall, void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern void N_partyPlaySfxPartyLanding1(void* party);
    extern void* partySearchFrontWall(f32 speed, f32 dir, void* party, VecLocal* pos);
    extern f32 vec3_802f4628[];

    void* party = (void*)pParty;
    f32* base = vec3_802f4628;
    void* player = *(void**)(pParty + 0x160);
    void* hit;
    VecLocal frontFloor;
    VecLocal copy0;
    VecLocal copy1;
    VecLocal copy2;
    VecLocal copy3;
    VecLocal wallPos;
    f32 sinValue;
    f32 cosValue;
    f32 z;
    f32 y;
    f32 x;
    f32 distance;
    f32 maxDistance;
    f32 yDelta;
    f32 frames;
    f32 accel;
    f32 velAbs;
    s32 footmarkId;
    s32 writeId;
    s32 readId;
    s32 timer;

    footmarkId = partyGetFootmarkId(party);

    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        *(u8*)(pParty + 0x38) = 0;
    }

    switch (*(u8*)(pParty + 0x38)) {
        case 0:
            x = *(f32*)(pParty + 0x94);
            y = *(f32*)(pParty + 0x98);
            z = *(f32*)(pParty + 0x9C);

            if (*(s16*)(pParty + 0x28) != 0 || *(u8*)(pParty + 0x34) == 9) {
                x = *(f32*)(pParty + 0x58);
                y = *(f32*)(pParty + 0x5C);
                z = *(f32*)(pParty + 0x60);
                movePos(*(f32*)(pParty + 0x104), *(f32*)(pParty + 0x100), &x, &z);

                copy0 = ((VecLocal*)base)[2];
                copy0.x = x;
                copy0.y = y;
                copy0.z = z;
                *(VecLocal*)(pParty + 0x94) = copy0;
            } else if ((*(u32*)(pParty + 8) & 0x00400000) == 0) {
                writeId = partyGetFootmarkWriteId(footmarkId);
                readId = partyGetFootmarkReadId(footmarkId);

                if (readId == writeId) {
                    distance = partyGetTargetDist(
                        *(f32*)(pParty + 0x58),
                        *(f32*)(pParty + 0x60),
                        footmarkId);

                    if (distance > float_2_804230d8 * *(f32*)((s32)player + 0x1B8)) {
                        f32 dir = partyGetTargetDir(
                            *(f32*)(pParty + 0x58),
                            *(f32*)(pParty + 0x60),
                            footmarkId);

                        getFrontFloor(dir, party, (f32*)&frontFloor);
                        y = frontFloor.y + float_10_804230a4;
                        x = *(f32*)(pParty + 0x58);
                        z = *(f32*)(pParty + 0x60);
                        frontFloor.y = y;
                        movePos((float_3_804230dc * float_0p2_80423194) * *(f32*)(pParty + 0xF4), dir, &x, &z);
                    }

                    copy1 = ((VecLocal*)base)[3];
                    copy1.x = x;
                    copy1.y = y;
                    copy1.z = z;
                    *(VecLocal*)(pParty + 0x94) = copy1;
                } else {
                    partyGetReadId(footmarkId, &x, &y, &z);

                    maxDistance = float_50_804230b8;
                    distance = distABf(
                        *(f32*)(pParty + 0x58),
                        *(f32*)(pParty + 0x60),
                        x,
                        z);

                    yDelta = y - *(f32*)(pParty + 0x5C);
                    if (yDelta < 0.0f) {
                        yDelta = -yDelta;
                    }
                    yDelta *= -float_neg1p4_8042313c;
                    if (yDelta < maxDistance) {
                        maxDistance = yDelta;
                    }

                    sincosf(
                        angleABf(
                            *(f32*)(pParty + 0x58),
                            *(f32*)(pParty + 0x60),
                            x,
                            z),
                        &sinValue,
                        &cosValue);

                    if (distance > maxDistance) {
                        distance = maxDistance;
                        if (partyGetTargetDistY(footmarkId) == float_0_804230bc) {
                            distance = float_0_804230bc;
                        }

                        x = *(f32*)(pParty + 0x58) + distance * sinValue;
                        z = *(f32*)(pParty + 0x60) + distance * cosValue;
                        y += float_10_804230a4;

                        copy2 = ((VecLocal*)base)[4];
                        copy2.x = x;
                        copy2.y = y;
                        copy2.z = z;
                        *(VecLocal*)(pParty + 0x94) = copy2;
                    } else {
                        if (partyGetTargetDistY(footmarkId) == float_0_804230bc) {
                            distance = float_0_804230bc;
                        }

                        x += distance * sinValue;
                        z += distance * cosValue;

                        copy3 = ((VecLocal*)base)[5];
                        copy3.x = x;
                        copy3.y = y;
                        copy3.z = z;
                        *(VecLocal*)(pParty + 0x94) = copy3;
                    }

                    if (distance < maxDistance) {
                        partyNextReadId(footmarkId);
                    }
                }
            }

            distance = distABf(
                *(f32*)(pParty + 0x58),
                *(f32*)(pParty + 0x60),
                *(f32*)(pParty + 0x94),
                *(f32*)(pParty + 0x9C));
            *(f32*)(pParty + 0x128) = distance;

            if (*(s16*)(pParty + 0x28) == 0) {
                *(f32*)(pParty + 0x100) = angleABf(
                    *(f32*)(pParty + 0x58),
                    *(f32*)(pParty + 0x60),
                    *(f32*)(pParty + 0x94),
                    *(f32*)(pParty + 0x9C));
                *(f32*)(pParty + 0x104) =
                    float_2p25_80423154 * *(f32*)((s32)player + 0x228);
            }

            timer = (s32)((float_10_804230a4 *
                           (9.0f * float_0p1_80423114 *
                            (*(f32*)(pParty + 0x128) / *(f32*)(pParty + 0x104))) +
                           float_0p5_804230d0) /
                          float_10_804230a4);
            *(s32*)(pParty + 0x24) = timer;

            if (*(s32*)(pParty + 0x24) < 12) {
                *(s32*)(pParty + 0x24) = 12;
            }
            if (*(s32*)(pParty + 0x24) > 40) {
                *(s32*)(pParty + 0x24) = 40;
            }

            frames = float_0p5_804230d0 * (f32)*(s32*)(pParty + 0x24);
            yDelta = *(f32*)(pParty + 0x98) - *(f32*)(pParty + 0x5C);

            velAbs = __fabsf(yDelta);

            if (velAbs < float_1_804230ec) {
                yDelta = (-float_neg5_80423180) * (-float_neg5_80423180);
            } else if (yDelta < float_0_804230bc) {
                yDelta = float_10_804230a4;
            }

            accel = (yDelta + yDelta) / (frames * frames - frames);
            *(f32*)(pParty + 0x114) =
                *(f32*)((s32)player + 0x228) * (accel * frames);

            if (*(f32*)(pParty + 0x114) > 13.0f) {
                *(f32*)(pParty + 0x114) = 13.0f;
            }
            if (*(f32*)(pParty + 0x114) < 3.0f) {
                *(f32*)(pParty + 0x114) = 3.0f;
            }

            *(f32*)(pParty + 0x11C) = -accel * *(f32*)((s32)player + 0x228);
            *(u32*)pParty |= 0x10;
            *(u32*)pParty &= ~0x2;
            partyChgPoseId(party, 4);
            psndSFXOn_3D(0x927, (void*)(pParty + 0x58));
            *(u8*)(pParty + 0x38) = 10;

            if ((*(u32*)pParty & 0x1000) == 0) {
                partyNextReadId(partyGetFootmarkId(party));
            }
            break;

        case 10:
            if ((*(u32*)pParty & 0x1000) == 0) {
                partyNextReadId(partyGetFootmarkId(party));
            }

            *(f32*)(pParty + 0x114) += *(f32*)(pParty + 0x11C);
            *(f32*)(pParty + 0x5C) += *(f32*)(pParty + 0x114);

            if (*(f32*)(pParty + 0x5C) < float_neg2000_804230cc) {
                *(u32*)pParty &= ~0x10;
                *(f32*)(pParty + 0x114) = float_0_804230bc;
                *(f32*)(pParty + 0x100) = angleABf(
                    *(f32*)(pParty + 0x58),
                    *(f32*)(pParty + 0x60),
                    *(f32*)((s32)player + 0x8C),
                    *(f32*)((s32)player + 0x94));
                *(VecLocal*)(pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
                partyChgMot(party, 0);
                break;
            }

            if (*(f32*)(pParty + 0x114) <= float_0_804230bc) {
                *(u32*)pParty &= ~0x10;
                *(u32*)pParty |= 0x20;

                velAbs = __fabsf(*(f32*)(pParty + 0x114));

                hit = partySearchGround(velAbs, float_0_804230bc, party);
                if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
                    if (*(s8*)(pParty + 0x4C) == 0) {
                        *(u32*)(pParty + 8) |= 0x01000000;
                        *(u32*)pParty |= 0x02000000;
                    }
                    *(void**)(pParty + 0x138) = 0;
                    partyChgMot(party, 3);
                    *(f32*)(pParty + 0x11C) = float_neg1_804230c8;
                    break;
                }

                *(void**)(pParty + 0x138) = hit;
                *(f32*)(pParty + 0x5C) = *(f32*)(pParty + 0xE4);

                if (hit != 0) {
                    *(u32*)pParty |= 0x2;
                    *(u32*)(pParty + 8) &= ~0x00400000;
                    *(u32*)pParty &= ~0x70;
                    N_partyPlaySfxPartyLanding1(party);
                    *(f32*)(pParty + 0x114) = float_0_804230bc;
                    partyChgMot(party, 0);
                    *(u32*)(pParty + 8) |= 0x00800000;
                    break;
                }
            }

            if ((*(u32*)pParty & 0x01000000) != 0) {
                movePos(
                    *(f32*)(pParty + 0x104),
                    *(f32*)(pParty + 0x100),
                    &wallPos.x,
                    &wallPos.z);
            } else {
                wallPos = *(VecLocal*)(pParty + 0x58);

                hit = partySearchFrontWall(
                    *(f32*)(pParty + 0x104),
                    *(f32*)(pParty + 0x100),
                    party,
                    &wallPos);

                if (hit != 0) {
                    *(void**)(pParty + 0x12C) = hit;
                    if (*(f32*)(pParty + 0x104) != float_0_804230bc) {
                        *(void**)(pParty + 0x134) = hit;
                    }
                }

                *(f32*)(pParty + 0x58) = wallPos.x;
                *(f32*)(pParty + 0x60) = wallPos.z;
            }
            break;
    }
}

#undef float_10_804230a4
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
extern f32 __fabsf(f32 value);
#pragma opt_common_subs off
void party_motion_jump2(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern const f32 float_10_804230a4[1];
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
        timer = (s32)((float_10_804230a4[0] * (dist / *(f32*)((s32)pParty + 0x104)) +
                       float_0p5_804230d0) / float_10_804230a4[0]);
        if (timer < 10) {
            timer = 10;
        }
        yDelta = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
        *(s32*)((s32)pParty + 0x24) = timer;
        frames = (f32)timer;
        *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)player + 0x228) * (*(f32*)((s32)pParty + 0x128) / frames);
        *(f32*)((s32)pParty + 0x114) = (float_0p5_804230d0 * (frames * (frames * __fabsf(*(f32*)((s32)pParty + 0x11C)))) + yDelta) / frames;
        *(f32*)((s32)pParty + 0x114) *= *(f32*)((s32)player + 0x228);
        if (*(f32*)((s32)pParty + 0x114) > float_20_80423090) {
            *(f32*)((s32)pParty + 0x114) = float_20_80423090;
        }
        if (*(f32*)((s32)pParty + 0x114) < float_0_804230bc) {
            *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        }
        partyChgPoseId(pParty, 4);
        *(u32*)pParty |= 0x10;
        *(u32*)pParty &= ~0x2;
        psndSFXOn_3D(0x927, (void*)((s32)pParty + 0x58));
        *(u8*)((s32)pParty + 0x38) = 0;
    }

    switch (*(u8*)((s32)pParty + 0x38)) {
    case 0:
        *(u8*)((s32)pParty + 0x38) = 1;
        break;
    case 1:
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x11C);
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
        pos = *(VecLocal*)((s32)pParty + 0x58);
        partySearchFrontWall(*(f32*)((s32)pParty + 0x104), *(f32*)((s32)pParty + 0x100),
                             pParty, &pos);
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x60) = pos.z;
        if (*(f32*)((s32)pParty + 0x114) <= float_0_804230bc) {
            hit = partySearchGround(__fabsf(*(f32*)((s32)pParty + 0x114)), pParty);
            pos.y = *(f32*)((s32)pParty + 0xE4);
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
            *(f32*)((s32)pParty + 0x5C) = pos.y;
            *(void**)((s32)pParty + 0x138) = hit;
            if (hit != 0) {
                N_partyPlaySfxPartyLanding1(pParty);
                *(u32*)pParty &= ~0x70;
                *(u32*)pParty |= 0x2;
                *(u32*)((s32)pParty + 8) &= ~0x400000;
                *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                               *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
                if (dist < float_5_804230e8) timer = 0;
                else timer = 1;
                partyChgMot(pParty, timer);
            }
        }
        break;
    }

    if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
        *(u32*)pParty &= ~0x10;
        *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                     *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
        partyChgMot(pParty, 0);
    }
}

#pragma opt_common_subs reset

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#define float_10_804230a4 10.0f

#undef float_10_804230a4
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
s32 party_motion_fall(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 __fabsf(f32 value);
    extern const f32 float_10_804230a4[1];
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
    if ((*(u32*)pParty & 0x8) == 0) {
        partyNextReadId(partyGetFootmarkId(pParty));
    }

    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        partyChgPoseId(pParty, 4);
        *(u32*)pParty &= ~0x80000;
    }

    hit = 0;
    if ((*(u32*)pParty & 0x02000000) == 0) {
        hit = partySearchGround(float_10_804230a4[0], __fabsf(*(f32*)((s32)pParty + 0x114)), pParty);
    }

    if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
        hit = 0;
        if (*(u8*)((s32)pParty + 0x32) == 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
            *(u32*)((s32)pParty + 8) |= 0x01000000;
            *(u32*)pParty |= 0x02000000;
        }
    }
    *(void**)((s32)pParty + 0x138) = hit;

    if (hit == 0) {
        *(u32*)pParty &= ~0x2;
        if ((*(u32*)pParty & 0x20) == 0) {
            *(u32*)pParty |= 0x20;
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
            *(u32*)pParty &= ~0x70;
            *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x11C) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x100) =
                angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                         *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
            *(f32*)((s32)pParty + 0x54) =
                revise360(float_180_80423108 + toMovedir(*(f32*)((s32)player + 0x1B0)));
            partyGetAppearPos4(pParty, (VecLocal*)((s32)pParty + 0x58));
            *(f32*)((s32)pParty + 0x5C) += float_37_8042314c;
            partyChgMot(pParty, 0);
            *(u32*)pParty &= ~0x02000000;
        }

        if (*(f32*)((s32)pParty + 0x104) != float_0_804230bc) {
            pos = *(VecLocal*)((s32)pParty + 0x58);
            partySearchFrontWall(float_1p3_80423150 * *(f32*)((s32)pParty + 0x104),
                                 *(f32*)((s32)pParty + 0x100), pParty, &pos);
            *(f32*)((s32)pParty + 0x58) = pos.x;
            *(f32*)((s32)pParty + 0x60) = pos.z;
        }
        return 2;
    }

    if ((*(u32*)pParty & 0x20) != 0) {
        *(u32*)pParty &= ~0x70;
        *(u32*)pParty |= 0x2;
        *(u32*)((s32)pParty + 8) &= ~0x400000;
        *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
        if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
            partyChgMot(pParty, 0);
        } else {
            partyChgMot(pParty, 1);
        }
        N_partyPlaySfxPartyLanding1(pParty);
        return 3;
    }

    *(u32*)pParty &= ~0x3000;
    *(u32*)pParty &= ~0x70;
    *(u32*)pParty |= 0x2;
    *(u32*)((s32)pParty + 8) &= ~0x400000;
    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
    *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
    if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
        partyChgMot(pParty, 0);
    } else {
        partyChgMot(pParty, 1);
    }
    N_partyPlaySfxPartyLanding1(pParty);
        if ((*(u32*)pParty & 0x8) == 0) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
    return 3;
}

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
#define float_10_804230a4 10.0f

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

#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
void party_motion_damage(void* pParty) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void marioGetScreenPos(VecLocal pos, f32* x, f32* y, f32* z);
    extern void partyGetReturnPos(void* pos);
    extern void unk_800cbf84(f32 value, void* party);
    extern void partyMove(void* party);
    extern u32 hitGetAttr(void* hit);
    void* player;
    VecLocal posB;
    f32 z;
    f32 y;
    f32 x;
    s32 noGo;
    void* hit;
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
            marioGetScreenPos(*(VecLocal*)((s32)pParty + 0x58), &x, &y, &z);
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
                    partyGetReturnPos(&posB);
                    *(f32*)((s32)pParty + 0x58) = posB.x;
                    *(f32*)((s32)pParty + 0x5C) = float_200_804230a8 + posB.y;
                    *(f32*)((s32)pParty + 0x60) = posB.z;
                    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                    *(f32*)((s32)pParty + 0x118) = float_neg1_804230c8;
                    unk_800cbf84(float_0_804230bc, pParty);
                    *(u32*)pParty |= 0x20;
                    *(u32*)pParty |= 0x02000000;
                    *(u8*)((s32)pParty + 0x38) = 0xB;
                }
            }
            break;
        case 11:
            marioGetScreenPos(*(VecLocal*)((s32)pParty + 0x58), &x, &y, &z);
            if (y > float_240_80423138) {
                *(u32*)pParty &= ~0x02000000;
            }
            partyMove(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_neg6_804230fc) {
                *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
                *(f32*)((s32)pParty + 0x114) = float_neg6_804230fc;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
                *(VecLocal*)((s32)pParty + 0x58) = *(VecLocal*)((s32)player + 0x8C);
                *(f32*)((s32)pParty + 0x5C) += float_200_804230a8;
                *(u32*)pParty &= ~0x70;
            }
            hit = *(void**)((s32)pParty + 0x138);
            if (hit != 0) {
                noGo = hit == 0 ? 0 : (hitGetAttr(hit) & 0x200) ? 1 : 0;
                if (noGo == 0) {
                *(u8*)((s32)pParty + 0x38) = 0xC;
                *(s32*)((s32)pParty + 0x24) = 0x1E;
                }
            }
            break;
        case 12:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer <= 0) {
                partyChgRunMode(pParty, 2);
                *(u8*)((s32)pParty + 0x3B) = 0;
                *(u32*)((s32)pParty + 4) &= ~0x10;
            }
            break;
    }
}


#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

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

#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
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

    const u8* data = (const u8*)vec3_802f4628;
    s32 ret;
    void* player;
    VecLocal pos;
    s32 timer;
    f32 dirDiff;

    ret = 1;
    player = *(void**)((s32)pParty + 0x160);
    if (*(u8*)((s32)pParty + 0x37) != 1) {
        if ((*(u32*)player & 0x00100000) != 0) {
            if ((*(u32*)((s32)player + 0xC) & 0x80000000) != 0) return 1;
            *(u32*)((s32)pParty + 8) |= 1;
            *(u8*)((s32)pParty + 0x37) = 1;
        } else return 1;
    }

    if ((*(u32*)((s32)pParty + 8) & 1) != 0) {
        *(u32*)((s32)pParty + 8) &= ~1;
        *(u32*)((s32)pParty + 4) |= 0x100;
        partyPaperOn(pParty, (char*)(data + 0x148));
        partyChgPaper(pParty, (char*)(data + 0x150));
        *(s32*)((s32)pParty + 0x24) = 0x1E;
        unk_800cbf84(float_0_804230bc, pParty);
        dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
        if (dirDiff <= float_180_80423108) {
            *(u32*)((s32)pParty + 4) &= ~0x200;
            *(u8*)((s32)pParty + 0x3A) = 0;
        } else {
            *(u32*)((s32)pParty + 4) |= 0x200;
            *(u8*)((s32)pParty + 0x3A) = 2;
        }
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 0:
            if (--*(s32*)((s32)pParty + 0x24) < 0) {
                *(u32*)pParty |= 0x1000;
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
                partyChgPaper(pParty, (char*)(data + 0x158));
                if ((*(u32*)pParty & 0x2000) == 0) {
                    ret = 0;
                    *(u8*)((s32)pParty + 0x3A) = 10;
                } else {
                    *(u8*)((s32)pParty + 0x3A) = 20;
                }
            }
            break;
        case 2:
            if (--*(s32*)((s32)pParty + 0x24) < 0) {
                *(u32*)pParty |= 0x1000;
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
                partyChgPaper(pParty, (char*)(data + 0x158));
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
                partyChgPaper(pParty, (char*)(data + 0x158));
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
            } else {
                dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
                if (dirDiff >= *(const f64*)(data + 0x160)) {
                    *(u32*)((s32)pParty + 4) |= 0x200;
                } else {
                    *(u32*)((s32)pParty + 4) &= ~0x200;
                }
                *(f32*)((s32)pParty + 0x10C) = ((*(u32*)((s32)pParty + 4) & 0x200) != 0) ? float_280_8042311c : float_260_80423120;
                dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
                if (dirDiff >= float_315_80423124 || dirDiff <= float_45_804230e0 ||
                    (dirDiff >= float_135_80423128 && dirDiff <= float_225_8042312c)) {
                    partyChgPaper(pParty, (char*)(data + 0x168));
                } else {
                    partyChgPaper(pParty, (char*)(data + 0x170));
                }
            }
            pos = *(VecLocal*)((s32)pParty + 0x58);
            if (unk_800c2010(pParty, &pos) == 0 && (*(u32*)player & 0x00100000) == 0 &&
                (hitGetAttr(*(void**)((s32)pParty + 0x12C)) & 0x2000) == 0 &&
                (hitGetAttr(*(void**)((s32)pParty + 0x134)) & 0x2000) == 0) {
                *(u8*)((s32)pParty + 0x3A) = 40;
            }
            break;
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 20:
            partyChgPaper(pParty, (char*)(data + 0x178));
            *(u32*)pParty |= 0x02000000;
            *(u32*)pParty |= 0x20;
            *(s32*)((s32)pParty + 0x24) = 0x14;
            unk_800cbf84(float_0_804230bc, pParty);
            *(u8*)((s32)pParty + 0x3A) = 21;
            break;
        case 21:
            dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)pParty + 0xF8));
            *(f32*)((s32)pParty + 0x10C) = (dirDiff <= float_180_80423108) ? float_260_80423120 : float_280_8042311c;
            *(f32*)((s32)pParty + 0x5C) += float_neg3_80423110;
            if (--*(s32*)((s32)pParty + 0x24) <= 0) {
                *(u8*)((s32)pParty + 0x3A) = 22;
            }
            break;
        case 22:
            *(u8*)((s32)pParty + 0x3A) = 10;
            *(u32*)pParty &= ~0x02000000;
            marioClearSlitFloor();
            break;
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 40:
            partyChgPaper(pParty, (char*)(data + 0x180));
            *(s32*)((s32)pParty + 0x24) = 0x1E;
            *(u8*)((s32)pParty + 0x3A) = 41;
            dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
            *(f32*)((s32)pParty + 0x10C) = (dirDiff >= float_180_80423108) ? float_0_804230bc : float_180_80423108;
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
        case 41:
            if (--*(s32*)((s32)pParty + 0x24) <= 0) {
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

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

extern f64 fabs(f64 value);
extern f32 __fabsf(f32 value);
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
s32 party_roll(void* pParty) {
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void unk_800cbf84(f32 value, void* party);
    extern f32 revise360(f32 angle);
    extern void partyPaperLightOff(void* party);
    extern char str_p_roll_802f4750[];
    extern char str_PM_R_1A_802f4758[];
    extern char str_PM_R_1B_802f4760[];
    extern char str_PM_R_1C_802f4768[];
    extern f32 vec3_802f4628[];
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    const u8* data = (const u8*)vec3_802f4628;
    f32 value;
    u8 state;
    s32 result = 1;

    if (*(u8*)((s32)pParty + 0x37) != 2) {
        return result;
    }

    if ((*(u32*)((s32)pParty + 8) & 1) != 0) {
        *(u32*)((s32)pParty + 8) &= ~1;
        partyPaperOn(pParty, (char*)(data + 0x128));
        partyChgPaper(pParty, (char*)(data + 0x130));
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
                partyChgPaper(pParty, (char*)(data + 0x138));
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

#pragma fp_contract off
    switch (*(u8*)((s32)pParty + 0x3A)) {
    case 100:
        *(f32*)((s32)pParty + 0x104) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x64) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x6C) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x88) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x8C) = float_0_804230bc;
        if (*(f32*)((s32)pParty + 0x100) < float_180_80423108) value = float_3_804230dc;
        else value = float_neg3_80423110;
        *(f32*)((s32)pParty + 0x7C) = value;
        *(f32*)((s32)pParty + 0x80) = float_0_804230bc;
        *(s32*)((s32)pParty + 0x24) = 0x41;
        *(u8*)((s32)pParty + 0x3A) = 101;
        partyChgPaper(pParty, (char*)(data + 0x140));
        partyPaperLightOff(pParty);
        /* fall through */
    case 101:

    *(s32*)((s32)pParty + 0x24) -= 1;
    result = 0;
    if (*(s32*)((s32)pParty + 0x24) > 0) {
        if (*(f32*)((s32)pParty + 0x100) < float_180_80423108) {
        value = float_0p1_80423114 * (float_90_80423118 - *(f32*)((s32)pParty + 0x10C));
        value += *(f32*)((s32)pParty + 0x10C);
        if (__fabsf((f32)(s32)((float_90_80423118 - value) < float_1_804230ec)) !=
            float_0_804230bc) {
            value = float_90_80423118;
        }
        *(f32*)((s32)pParty + 0x10C) = value;
        *(f32*)((s32)pParty + 0x110) = value;
        } else {
        value = float_0p1_80423114 * (float_270_80423104 - *(f32*)((s32)pParty + 0x10C));
        value += *(f32*)((s32)pParty + 0x10C);
        if (__fabsf((f32)(s32)((float_270_80423104 - value) < float_1_804230ec)) !=
            float_0_804230bc) {
            value = float_270_80423104;
        }
        *(f32*)((s32)pParty + 0x10C) = value;
        *(f32*)((s32)pParty + 0x110) = value;
        }
    } else {
        result = 1;
        if (*(f32*)((s32)pParty + 0x100) >= float_180_80423108) {
            *(f32*)((s32)pParty + 0x10C) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x110) = float_0_804230bc;
            *(f32*)((s32)pParty + 0xFC) = float_270_80423104;
            *(f32*)((s32)pParty + 0x100) = float_270_80423104;
        } else {
            *(f32*)((s32)pParty + 0x10C) = float_180_80423108;
            *(f32*)((s32)pParty + 0x110) = float_180_80423108;
            *(f32*)((s32)pParty + 0xFC) = float_90_80423118;
            *(f32*)((s32)pParty + 0x100) = float_90_80423118;
        }
        *(VecLocal*)((s32)pParty + 0x88) = *(const VecLocal*)(data + 0x48);
        *(VecLocal*)((s32)pParty + 0x7C) = *(const VecLocal*)(data + 0x54);
        *(u8*)((s32)pParty + 0x37) = 0;
        *(u32*)pParty &= ~0x1000;
        *(u32*)((s32)pParty + 4) &= ~0x100;
        *(u32*)pParty &= ~0x200;
        *(u32*)((s32)pParty + 4) &= ~0x10;
        *(u32*)((s32)pParty + 4) &= ~0x8;
        *(u8*)((s32)pParty + 0x3A) = 100;
        partyPaperOff(pParty);
    }
    break;
    }
    return result;
}

#pragma fp_contract reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

extern f64 __frsqrte(f64 value);
extern f32 __float_nan[];
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
    extern const f64 double_0p5_802f4738;
    extern const f64 double_3_802f4740;
    extern const f64 double_0_802f4748;
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);

    HitWork floor;
    HitWork probe;
    f32 c;
    f32 s;
    f32 s2;
    f32 c2;
    f32 step;
    f32 bestY;
    f32 xzLen;
    f32 maxY;
    f32 bias;
    f32 root;
    f32 wall;
    f64 inv;
    union {
        f32 f;
        u32 u;
    } rootValue;
    s32 exponent;
    s32 classify;
    s32 i;
    void* hit;
    const u8* data = (const u8*)vec3_802f4628;

    wall = touch_wall_ct;
    step = (float_2_804230d8 * *(f32*)((s32)pParty + 0xF4)) / float_3_804230dc;
    probe.dir = *(const VecLocal*)(data + 0x60);
    sincosf(angle, &s, &c);
    for (i = 0; i < 3; i++) {
        probe.start.x = *(f32*)((s32)pParty + 0x58) + (f32)i * (s * step);
        probe.start.z = *(f32*)((s32)pParty + 0x60) + (f32)i * (c * step);
        probe.start.y = *(f32*)((s32)pParty + 0x5C) + *(f32*)((s32)pParty + 0xF0);
        probe.dist = float_100_804230b0;
        hit = (void*)hitCheckVecFilter(&probe, 0);
        if (hit != 0) {
            xzLen = probe.normal.x * probe.normal.x;
            root = probe.normal.z * probe.normal.z;
            root = xzLen + root;
            if (root > float_0_804230bc) {
                inv = __frsqrte(root);
                inv = *(const f64*)(data + 0x110) * inv *
                      (*(const f64*)(data + 0x118) - root * (inv * inv));
                inv = *(const f64*)(data + 0x110) * inv *
                      (*(const f64*)(data + 0x118) - root * (inv * inv));
                inv = *(const f64*)(data + 0x110) * inv *
                      (*(const f64*)(data + 0x118) - root * (inv * inv));
                root = (f32)(root * inv);
            } else if (root < *(const f64*)(data + 0x120)) {
                root = __float_nan[0];
            } else {
                rootValue.f = root;
                exponent = rootValue.u & 0x7F800000;
                switch (exponent) {
                case 0x7F800000:
                    if ((rootValue.u & 0x007FFFFF) != 0) classify = 1;
                    else classify = 2;
                    break;
                case 0:
                    if ((rootValue.u & 0x007FFFFF) != 0) classify = 5;
                    else classify = 3;
                    break;
                default:
                    classify = 4;
                    break;
                }
                if (classify == 1) root = __float_nan[0];
            }
            if (root < float_45_804230e0) {
                bestY = probe.hitPos.y;
                goto found_floor;
            }
        }
    }
    bestY = float_neg2000_804230cc;
found_floor:
    if (bestY > float_neg2000_804230cc) {
        maxY = *(f32*)((s32)pParty + 0x5C) + wall;
        bestY -= float_2_804230d8;
        if (maxY < bestY) floor.start.y = maxY;
        else floor.start.y = bestY;
    } else {
        floor.start.y = *(f32*)((s32)pParty + 0x5C) + wall;
    }

    step = float_0p75_804230e4 * *(f32*)((s32)pParty + 0xF4);
    sincosf(angle, &s2, &c2);
    floor.start.x = *(f32*)((s32)pParty + 0x58) + s2 * step;
    floor.start.z = *(f32*)((s32)pParty + 0x60) + c2 * step;
    floor.dir = *(const VecLocal*)(data + 0x6C);
    floor.dist = float_300_804230ac;

    hit = (void*)hitCheckVecFilter(&floor, 0);
    if (hit == 0) {
        outPos[0] = *(f32*)((s32)pParty + 0x58);
        outPos[1] = *(f32*)((s32)pParty + 0x5C);
        outPos[2] = *(f32*)((s32)pParty + 0x60);
        return 0;
    }

    bias = floor.hitPos.x >= float_0_804230bc ? float_0p5_804230d0 : float_neg0p5_804230d4;
    outPos[0] = (f32)(s32)(floor.hitPos.x * float_1000_804230c4 + bias) / float_1000_804230c4;

    bias = floor.hitPos.y >= float_0_804230bc ? float_0p5_804230d0 : float_neg0p5_804230d4;
    outPos[1] = (f32)(s32)(floor.hitPos.y * float_1000_804230c4 + bias) / float_1000_804230c4;

    bias = floor.hitPos.z >= float_0_804230bc ? float_0p5_804230d0 : float_neg0p5_804230d4;
    outPos[2] = (f32)(s32)(floor.hitPos.z * float_1000_804230c4 + bias) / float_1000_804230c4;
    return hitGetAttr(hit);
}

void party_force_reset_outofscreen(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 marioCtrlOffChk(void);
    extern void marioGetScreenPos(VecLocal pos, f32* x, f32* y, f32* z);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 partyGetAppearPos3(void* party, VecLocal* pos);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern void* partyHitCheck(void* party, VecLocal* pos, VecLocal* dir, VecLocal* outPos, VecLocal* work, f32* dist);
    extern u32 hitGetAttr(void* hit);

    void* player;
    void* initialPlayer;
    s32 attrBlocked;
    VecLocal pos;
    VecLocal checkPos;
    VecLocal dir;
    VecLocal hitPos;
    VecLocal work;
    f32 screenZ;
    f32 screenY;
    f32 screenX;
    f32 dist;
    f32 step;
    f32 angle;
    f32 y;
    f32 height;
    extern const f32 vec3_802f46dc[3];
    void* hit;
    s32 xLimit;
    s32 outerCount;
    s32 innerCount;

    initialPlayer = *(void**)((s32)pParty + 0x160);
    if (marioCtrlOffChk() != 0) {
        return;
    }
    if (marioKeyOffChk() != 0) {
        return;
    }

    marioGetScreenPos(*(VecLocal*)((s32)pParty + 0x58), &screenX, &screenY, &screenZ);

    if (distABf(*(f32*)((s32)initialPlayer + 0x8C), *(f32*)((s32)initialPlayer + 0x94),
                *(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60)) < float_20_80423090 &&
        screenY > float_800_80423094) {
        partyGetAppearPos3(pParty, &pos);
        *(VecLocal*)((s32)pParty + 0x58) = pos;
        return;
    }

    xLimit = 100;
    if (float_neg100_80423098 <= screenX && screenX <= float_700_8042309c &&
        screenY <= float_650_804230a0) {
        return;
    }

    if ((*(u32*)pParty & 8) == 0) {
        partyClearFootmark();
    } else {
        partyClearFootmark();
    }
    player = *(void**)((s32)pParty + 0x160);
    pos = *(VecLocal*)((s32)pParty + 0x58);
    if (*(u8*)((s32)pParty + 0x32) == 0) {
        step = *(f32*)((s32)pParty + 0xF4);
    } else if (*(u8*)((s32)pParty + 0x31) == 5) {
        step = float_10_804230a4;
        xLimit = 70;
    } else {
        step = float_10_804230a4;
        xLimit = 50;
    }
    dir = *(const VecLocal*)vec3_802f46dc;
    for (outerCount = 0; ; outerCount++) {
        marioGetScreenPos(pos, &screenX, &screenY, &screenZ);
        if ((f32)-xLimit <= screenX && screenX <= (f32)(xLimit + 600)) {
            *(VecLocal*)((s32)pParty + 0x58) = pos;
            break;
        }
        if (outerCount > 100) break;
        angle = angleABf(pos.x, pos.z, *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        movePos(step, angle, &pos.x, &pos.z);
        height = float_200_804230a8;
        y = pos.y;
        for (innerCount = 0; ; ) {
            checkPos = pos;
            checkPos.y += height;
            dist = float_300_804230ac;
            hit = partyHitCheck(pParty, &checkPos, &dir, &hitPos, &work, &dist);
            if (hit != 0) {
                attrBlocked = hit != 0 && (hitGetAttr(hit) & 0x200) != 0;
                if (!attrBlocked) y = hitPos.y;
            } else {
                y = *(f32*)((s32)player + 0x90);
            }
            checkPos.y = y;
            marioGetScreenPos(checkPos, &screenX, &screenY, &screenZ);
            if (screenY > float_100_804230b0) {
                checkPos = pos;
                dist = float_100_804230b0;
                if (pos.y + float_74_804230b4 > y) {
                    checkPos.y += float_74_804230b4;
                    hit = partyHitCheck(pParty, &checkPos, &dir, &hitPos, &work, &dist);
                    if (hit != 0) {
                        attrBlocked = hit != 0 && (hitGetAttr(hit) & 0x200) != 0;
                        if (!attrBlocked) y = hitPos.y;
                    }
                }
                break;
            }
            if (++innerCount > 4) break;
            height -= float_50_804230b8;
            if (height < float_0_804230bc) height = float_0_804230bc;
        }
        pos.y = y;
    }
}
