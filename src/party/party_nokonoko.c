#include "party/party_nokonoko.h"

s32 nokoSe;
u32 gap_08_8041EAFC_sbss;
extern f32 float_37_80421678;
extern f32 float_24_80421708;
extern const f32 float_0_80421644;
extern f32 float_neg1_80421648;
extern f32 float_10_8042165c;
extern f32 float_180_8042163c;
extern f32 float_3p1416_80421638;
extern f32 float_90_80421654;
extern f32 float_0p5_80421658;
extern double sin(double x);
extern double cos(double x);

void partyGoodbyeInit(void* party);
s32 partyGoodbyeMain(void* party);
void partyChgRunMode(void* party, s32 mode);
s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);
extern void* marioGetPtr(void);
extern void psndSFXOff(s32 soundId);
extern void itemNokoForceGet(s32 item);

s32 nokonokoGetStatus(void* party) {
    u8 state;
    s32 ret;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 2) {
        return 0;
    }
    if ((*(u32*)party & 0x100) == 0) {
        return 0;
    }

    state = *(u8*)((s32)party + 0x39);
    ret = 7;
    if (state <= 2) {
        ret = 1;
    } else if (state >= 0xA && state <= 0xD) {
        ret = 2;
    } else if (state >= 0x14 && state <= 0x15) {
        ret = 3;
    } else if (state >= 0x32 && state <= 0x47) {
        ret = 4;
    } else if (state >= 0x50 && state <= 0x64) {
        ret = 5;
    } else if (state >= 0x6E && state <= 0x72) {
        ret = 6;
    }
    return ret;
}

s32 nokonoko_holdItem(void) {
    void* party = partyGetPtr(marioGetPartyId());
    s32 item;

    if (party == 0) {
        return 0;
    }
    if (*(s8*)((s32)party + 0x31) != 2 || !(*(u32*)party & 0x100)) {
        return 0;
    }

    item = *(s32*)((s32)party + 0x174);
    return ((u32)(-item) | (u32)item) >> 31;
}

void nokonoko_bye(void* party) {
    if (*(u32*)((s32)party + 8) & 8) {
        *(u32*)((s32)party + 8) &= ~8;
        partyGoodbyeInit(party);
    }
    if (partyGoodbyeMain(party) != 0) {
        partyChgRunMode(party, 0xE);
    }
}

void nokonoko_init(void* party) {
    f32 offset;
    f32 height;

    *(u8*)((s32)party + 0x33) = 2;
    height = float_37_80421678;
    *(u8*)((s32)party + 0x32) = 0;
    offset = float_24_80421708;
    *(f32*)((s32)party + 0xF0) = height;
    *(f32*)((s32)party + 0xF4) = offset;
    nokoSe = -1;
}

void nokonoko_move(void* party) {
    extern void partyWalkInit(void* party, s32 param);
    extern void unk_800cbeb0(void* party);
    extern void partyRecordFootmark(void* party);
    extern void party_move_mode_stay(void* party);
    extern void party_move_mode_walk(void* party);
    extern void party_move_mode_behind_mario(void* party);
    extern void party_move_mode_beside_mario(void* party);

    marioGetPtr();
    if (*(u32*)((s32)party + 8) & 8) {
        *(u32*)((s32)party + 8) &= ~8;
        partyWalkInit(party, 1);
    }
    unk_800cbeb0(party);
    partyRecordFootmark(party);
    switch (*(u8*)((s32)party + 0x3B)) {
        case 0:
            party_move_mode_stay(party);
            break;
        case 1:
            party_move_mode_walk(party);
            break;
        case 2:
            party_move_mode_behind_mario(party);
            break;
        case 3:
            party_move_mode_beside_mario(party);
            break;
        case 4:
        default:
            break;
    }
}


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_nokonoko 20260624_191429 */

/* fallback stub-fill: map=unk_800d058c addr=0x800d058c size=0x000001a8 */
void unk_800d058c(void) {

    void* party;
    unsigned char state;
    int status;
    float height;

    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    status = 0;
    if (party != 0) {
        if (*(signed char*)((int)party + 0x31) == 2 && (*(unsigned int*)party & 0x100) != 0) {
            state = *(unsigned char*)((int)party + 0x39);
            status = 7;
            if (state < 3) {
                status = 1;
            } else if (state >= 0xA && state <= 0xD) {
                status = 2;
            } else if (state >= 0x14 && state <= 0x15) {
                status = 3;
            } else if (state >= 0x32 && state <= 0x47) {
                status = 4;
            } else if (state >= 0x50 && state <= 0x64) {
                status = 5;
            } else if (state >= 0x6E && state <= 0x72) {
                status = 6;
            }
        }
    }
    if (status != 0) {
        if ((*(unsigned int*)party & 0x100) != 0) {
            marioGetPtr();
            if ((unsigned int)(nokoSe + 0x10000) != 0xFFFF) {
                psndSFXOff(nokoSe);
                nokoSe = -1;
            }
            height = float_37_80421678;
            *(unsigned int*)party &= ~0x100;
            *(unsigned int*)party &= ~0x04000000;
            *(unsigned int*)party &= ~0x01000000;
            *(unsigned int*)party &= ~0x02000000;
            *(unsigned int*)((int)party + 4) &= ~0x20;
            *(unsigned int*)((int)party + 4) &= ~0x10;
            *(float*)((int)party + 0xF0) = height;
            *(int*)((int)party + 0x178) = 0;
            *(int*)((int)party + 0x17C) = 0;
            *(int*)((int)party + 0x180) = 0;
        }
        if (*(unsigned char*)((int)party + 0x34) != 8 && *(unsigned char*)((int)party + 0x34) != 0xD) {
            partyChgRunMode(party, 2);
            *(unsigned char*)((int)party + 0x3B) = 0;
        }
    }
}


void nokonoko_use_post(void* party) {
    u32 item;
    f32 height;

    item = *(u32*)((s32)party + 0x174);
    if (item != 0) {
        itemNokoForceGet(item);
        *(s32*)((s32)party + 0x174) = 0;
    }
    marioGetPtr();
    if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
        psndSFXOff(nokoSe);
        nokoSe = -1;
    }
    height = float_37_80421678;
    *(u32*)party &= ~0x100;
    *(u32*)party &= ~0x04000000;
    *(u32*)party &= ~0x01000000;
    *(u32*)party &= ~0x02000000;
    *(u32*)((s32)party + 4) &= ~0x20;
    *(u32*)((s32)party + 4) &= ~0x10;
    *(f32*)((s32)party + 0xF0) = height;
    *(s32*)((s32)party + 0x178) = 0;
    *(s32*)((s32)party + 0x17C) = 0;
    *(s32*)((s32)party + 0x180) = 0;
}


void nokonoko_use(void* party) {
    extern s32 marioKeyOffChk(void);
    extern void marioGetScreenPos(void*, f32*, f32*, f32*);
    extern s32 marioChkInScreen(s32, s32);
    extern f32 angleABf(f64, f64, f64, f64);
    extern f32 distABf(f64, f64, f64, f64);
    extern void movePos(f64, f64, f32*, f32*);
    extern f64 toMovedirSimple(f64);
    extern f64 revise360(f64);
    extern void marioAdjustMoveDir(void);
    extern void partyClearJumpPara(void*);
    extern void partyChgPose(void*, const char*);
    extern void marioChgSmallJumpMotion(void);
    extern void marioChgMot(s32);
    extern f64 toMovedir(f64);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void partyMove(void*);
    extern void partyMoveNoHosei(void*);
    extern const char str_PNK_K_2_802cc5f0[];
    extern const char str_PNK_A_1_802cc5f8[];
    extern f32 float_18p5_80421680;
    extern f32 float_2_80421684;
    extern f32 float_45_80421688;
    extern f32 float_6p2832_8042168c;
    extern f32 float_360_80421690;
    extern f32 float_50_80421694;
    extern void effSmallStarN64Entry(f64, f64, f64, f64, f64, f64, s32, s32);
    extern void* effKemuTestEntry(f32, f32, f32, f32, s32);
    extern void effKemuTestDrawCam(void*, s32);
    extern void* itemNearDistCheck(f32, f32, f32, f32);
    extern void itemPickUp(void*);
    extern void* ccwall(void*, s32);
    extern u32 marioChkKey(void);
    extern void* gp;
    extern f32 float_1_80421670;
    extern f32 float_0p25_80421698;
    extern f32 float_0p3_8042169c;
    extern f32 float_12_804216a0;
    extern f32 float_120_804216a4;
    extern f32 float_0p15_804216a8;
    extern f32 float_0p1_80421640;
    extern f32 float_20_804216ac;
    extern f32 float_2p2_804216b0;
    extern f32 float_neg2_804216b4;
    extern f32 float_0p2_804216b8;
    extern f32 float_135_804216bc;
    extern f32 float_315_804216c0;
    extern f32 float_225_804216c4;
    extern f32 float_270_804216c8;
    extern f32 float_4_804216cc;
    extern f32 float_5_804216d0;
    extern f32 float_neg90_804216d4;
    extern f64 double_180_802cc600;
    extern void partyGetMoveDirSpd(void*, f32*, f32*);
    extern void unk_800c27c0(void*, void*, f32, f32, f32, f32);
    extern u32 lookupSafetyPos(f32, f32*, f32*);
    extern void lookupSafetyPos2(f32, f32*, f32*);
    extern void lookupSafetyPos3(f32, f32*, f32*);
    extern void lookupSafetyPosSub(f64, f64, f32*, f32*);
    extern f32 float_75_804216d8;
    extern f32 float_3_804216dc;
    extern f32 float_18_804216e0;
    extern f32 float_0p35_804216e4;
    extern f32 float_30_80421660;
    extern f32 float_15_804216e8;
    extern f32 float_neg300_804216ec;
    extern f32 float_240_804216f8;
    extern f32 float_neg6_804216fc;
    extern f32 float_neg2000_80421700;
    extern f32 float_200_80421704;
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 sysMsec2Frame(s32);
    void* player = *(void**)((s32)party + 0x160);
    u8 state;
    f32 sx, sy, sz;
    f32 pos[3];
    f32 x;
    f32 z;
    typedef struct { u32 x, y, z; } NokonokoRawVec3;

    if ((*(u32*)party & 0x80000000) == 0 && marioKeyOffChk() != 0) {
        *(NokonokoRawVec3*)pos =
            *(NokonokoRawVec3*)((s32)party + 0x58);
        marioGetScreenPos(pos, &sx, &sy, &sz);
        if (marioChkInScreen((s32)sx, (s32)sy) == 0) {
            x = *(f32*)((s32)player + 0x8C);
            z = *(f32*)((s32)player + 0x94);
            movePos(160.0f,
                    angleABf(x, z, *(f32*)((s32)party + 0x58),
                             *(f32*)((s32)party + 0x60)),
                    &x, &z);
            *(f32*)((s32)party + 0x58) = x;
            *(f32*)((s32)party + 0x60) = z;
            nokonoko_finish(party);
            return;
        }
    }
    if ((*(u32*)((s32)party + 8) & 8) != 0) {
        *(u32*)((s32)party + 8) &= ~8;
        *(s32*)((s32)party + 0x174) = 0;
        *(s32*)((s32)party + 0x178) = 0;
        *(s32*)((s32)party + 0x17C) = 0;
        *(s32*)((s32)party + 0x180) = 0;
        if ((*(u32*)party & 0x80000000) != 0) {
            if (*(s32*)((s32)party + 0x16C) > 0) {
                *(u16*)((s32)party + 0x17E) |= 1;
            }
        } else {
            *(u16*)((s32)party + 0x17E) &= ~1;
        }
        *(u8*)((s32)party + 0x39) = 0;
    }
    state = *(u8*)((s32)party + 0x39);

    switch (state) {
    case 0:
        if (*(u16*)((s32)player + 0x2E) == 0x20 ||
            *(u16*)((s32)player + 0x2E) == 0x1F) {
            *(u8*)((s32)party + 0x39) = 0x46;
            break;
        }
        partyClearJumpPara(party);
        *(u32*)party |= 0x04000000;
        *(u32*)((s32)party + 4) |= 0x10;
        partyChgPose(party, str_PNK_K_2_802cc5f0);
        *(f32*)((s32)party + 0xF0) = float_18p5_80421680;
        *(u8*)((s32)party + 0x39) = 1;
        *(s32*)((s32)party + 0x24) = 100;
        *(s32*)((s32)party + 0x174) = 0;
        *(u16*)((s32)party + 0x17C) = 0;
        marioAdjustMoveDir();
        *(f32*)((s32)player + 0x180) = float_0_80421644;
        *(NokonokoRawVec3*)((s32)party + 0x94) =
            *(NokonokoRawVec3*)((s32)player + 0x8C);
        movePos(float_2_80421684,
                toMovedirSimple(*(f32*)((s32)player + 0x1B0)),
                (f32*)((s32)party + 0x94), (f32*)((s32)party + 0x9C));
        *(s32*)((s32)party + 0x24) = 4;
        *(f32*)((s32)party + 0x114) = float_0_80421644;
        *(f32*)((s32)party + 0x100) =
            angleABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                     *(f32*)((s32)party + 0x94), *(f32*)((s32)party + 0x9C));
        *(f32*)((s32)party + 0x104) =
            distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60),
                    *(f32*)((s32)party + 0x94), *(f32*)((s32)party + 0x9C)) /
            *(s32*)((s32)party + 0x24);
        *(f32*)((s32)party + 0x10C) =
            revise360(float_45_80421688 + *(f32*)((s32)party + 0x10C));
        marioChgSmallJumpMotion();
        *(u32*)((s32)player + 0xC) |= 0x20000;
        *(f32*)((s32)player + 0x180) = float_0_80421644;
    case 1:
        if (*(u16*)((s32)player + 0x2E) == 0x20 ||
            *(u16*)((s32)player + 0x2E) == 0x1F) {
            *(u8*)((s32)party + 0x39) = 0x46;
            break;
        }
        *(f32*)((s32)party + 0x10C) =
            revise360(float_45_80421688 + *(f32*)((s32)party + 0x10C));
        *(f32*)((s32)party + 0x58) +=
            (*(f32*)((s32)party + 0x94) - *(f32*)((s32)party + 0x58)) /
            *(s32*)((s32)party + 0x24);
        *(f32*)((s32)party + 0x5C) +=
            (*(f32*)((s32)party + 0x98) - *(f32*)((s32)party + 0x5C)) /
            *(s32*)((s32)party + 0x24);
        *(f32*)((s32)party + 0x60) +=
            (*(f32*)((s32)party + 0x9C) - *(f32*)((s32)party + 0x60)) /
            *(s32*)((s32)party + 0x24);
        if (--*(s32*)((s32)party + 0x24) < 1) {
            *(NokonokoRawVec3*)((s32)party + 0x58) =
                *(NokonokoRawVec3*)((s32)party + 0x94);
            *(f32*)((s32)party + 0x104) = float_0_80421644;
            *(f32*)((s32)party + 0x110) = *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)party + 0x10C) = *(f32*)((s32)player + 0x1B0);
            *(u8*)((s32)party + 0x39) = 2;
            partyChgPose(party, str_PNK_A_1_802cc5f8);
        }
        break;
    case 2:
        if (*(u16*)((s32)player + 0x2E) == 0x20 ||
            *(u16*)((s32)player + 0x2E) == 0x1F) {
            *(u8*)((s32)party + 0x39) = 0x46;
        } else if (*(void**)((s32)player + 0x1E8) != 0) {
            marioChgMot(3);
            *(u32*)player |= 0x80;
            *(f32*)((s32)player + 0x180) = float_0_80421644;
            *(u8*)((s32)party + 0x39) = 10;
            *(s32*)((s32)party + 0x24) = 15;
            *(f32*)((s32)party + 0x104) = float_0_80421644;
            *(f32*)((s32)party + 0x128) = float_0_80421644;
            *(f32*)((s32)party + 0x100) =
                toMovedir(*(f32*)((s32)player + 0x1B0));
            psndSFXOn_3D(0x92D, (void*)((s32)party + 0x58));
        }
        break;
    case 3:
        marioGetScreenPos((void*)((s32)party + 0x58), &sx, &sy, &sz);
        if (marioChkInScreen((s32)sy, (s32)sz) == 0) nokonoko_finish(party);
        break;
    case 10:
        psndSFXOn_3D(0x92A, (void*)((s32)party + 0x58));
        {
            f64 radians =
                (float_6p2832_8042168c * *(f32*)((s32)party + 0x100)) /
                float_360_80421690;
            effSmallStarN64Entry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C) + *(f32*)((s32)party + 0xF0),
                *(f32*)((s32)party + 0x60), sin(radians),
                float_neg1_80421648, -cos(radians), 3, 3);
        }
        *(u32*)party |= 0x04000000;
        *(u32*)party |= 0x01000000;
        *(u8*)((s32)party + 0x39) = 11;
    case 11:
        *(f32*)((s32)party + 0x104) += float_2_80421684;
        if (*(f32*)((s32)party + 0x104) > float_10_8042165c) {
            *(f32*)((s32)party + 0x104) = float_10_8042165c;
        }
        {
            f32 turn = float_neg1_80421648;
            if (revise360(*(f32*)((s32)party + 0x100) -
                          *(f32*)((s32)party + 0xF8)) >
                float_180_8042163c) {
                turn = 1.0f;
            }
            *(f32*)((s32)party + 0x10C) =
                revise360(float_50_80421694 * turn +
                          *(f32*)((s32)party + 0x10C));
        }
        *(f32*)((s32)party + 0x114) += *(f32*)((s32)party + 0x104);
        partyMoveNoHosei(party);
        break;
    }


    state = *(u8*)((s32)party + 0x39);

    if (state == 11 || state == 12) {
        f32 turn;
        f32 radius;
        void* eff;
        void* item;
        f64 radians;

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            revise360(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        if ((*(u32*)((s32)gp + 0x1C) & 1) != 0) {
            eff = effKemuTestEntry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C) +
                    *(f32*)((s32)party + 0xF0) *
                        float_0p25_80421698,
                *(f32*)((s32)party + 0x60),
                float_0p3_8042169c, 0);
            effKemuTestDrawCam(
                eff, *(s32*)((s32)party + 0x164));
        }

        *(f32*)((s32)party + 0x128) +=
            *(f32*)((s32)party + 0x104);

        if (*(void**)((s32)party + 0x174) == 0) {
            radius =
                float_0p5_80421658 *
                    *(f32*)((s32)party + 0xF4) +
                float_12_804216a0;
            item = itemNearDistCheck(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C),
                *(f32*)((s32)party + 0x60),
                radius);
            *(void**)((s32)party + 0x174) = item;

            if (item != 0) {
                itemPickUp(item);
                *(u8*)((s32)party + 0x39) = 0x32;
                psndSFXOn_3D(
                    0x92C, (void*)((s32)party + 0x58));

                radians =
                    (float_6p2832_8042168c *
                     *(f32*)((s32)party + 0x100)) /
                    float_360_80421690;
                effSmallStarN64Entry(
                    *(f32*)((s32)party + 0x58),
                    *(f32*)((s32)party + 0x5C) +
                        *(f32*)((s32)party + 0xF0),
                    *(f32*)((s32)party + 0x60),
                    sin(radians), float_neg1_80421648,
                    -cos(radians), 3, 1);
                goto second_state_machine;
            }
        }

        if (ccwall(party, 0) != 0) {
            *(u8*)((s32)party + 0x39) = 0x32;
            psndSFXOn_3D(
                0x92C, (void*)((s32)party + 0x58));

            radians =
                (float_6p2832_8042168c *
                 *(f32*)((s32)party + 0x100)) /
                float_360_80421690;
            effSmallStarN64Entry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C) +
                    *(f32*)((s32)party + 0xF0),
                *(f32*)((s32)party + 0x60),
                sin(radians), float_neg1_80421648,
                -cos(radians), 3, 1);
            goto second_state_machine;
        }

        if (*(f32*)((s32)party + 0x128) >=
            float_120_804216a4) {
            *(u8*)((s32)party + 0x39) = 12;
            *(f32*)((s32)party + 0x104) =
                float_0p15_804216a8 *
                    (float_neg1_80421648 -
                     *(f32*)((s32)party + 0x104)) +
                *(f32*)((s32)party + 0x104);

            if (*(f32*)((s32)party + 0x104) <=
                float_0_80421644) {
                *(f32*)((s32)party + 0x104) =
                    float_0_80421644;

                if ((*(u32*)party & 0x80000000) == 0) {
                    if ((*(u16*)((s32)player + 0x24A) &
                         0x400) != 0) {
                        *(u8*)((s32)party + 0x39) = 13;
                        *(s32*)((s32)party + 0x24) = 0;
                    } else {
                        *(u8*)((s32)party + 0x39) = 0x32;
                    }
                } else if ((*(u16*)((s32)party + 0x17E) &
                            1) != 0) {
                    *(s32*)((s32)party + 0x24) = 5;
                    *(u8*)((s32)party + 0x39) = 0x15;
                    *(s16*)((s32)party + 0x17C) = 1;
                    if (nokoSe == -1) {
                        nokoSe = psndSFXOn_3D(
                            0x92E,
                            (void*)((s32)party + 0x58));
                    }
                } else {
                    *(u8*)((s32)party + 0x39) = 0x32;
                }
            }
        }
    }

    state = *(u8*)((s32)party + 0x39);
    if (state == 13) {
        s32 timer;

        timer = *(s32*)((s32)party + 0x24) + 1;
        *(s32*)((s32)party + 0x24) = timer;

        if (timer >= 5) {
            *(s32*)((s32)party + 0x24) = 5;
            *(u8*)((s32)party + 0x39) = 0x14;
            *(s16*)((s32)party + 0x17C) = 1;
            if (nokoSe == -1) {
                nokoSe = psndSFXOn_3D(
                    0x92E, (void*)((s32)party + 0x58));
            }
        }
    }

    state = *(u8*)((s32)party + 0x39);
    if (state == 13 || state == 0x14) {
        f32 turn;
        f32 radius;
        void* eff;
        void* item;
        f64 radians;
        s32 motion;

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            revise360(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        if ((*(u32*)((s32)gp + 0x1C) % 30) == 0) {
            eff = effKemuTestEntry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C),
                *(f32*)((s32)party + 0x60),
                float_1_80421670, 5);
            effKemuTestDrawCam(
                eff, *(s32*)((s32)party + 0x164));
        }

        if (ccwall(party, 0) != 0) {
            *(u8*)((s32)party + 0x39) = 0x32;
            psndSFXOn_3D(
                0x92C, (void*)((s32)party + 0x58));

            radians =
                (float_6p2832_8042168c *
                 *(f32*)((s32)party + 0x100)) /
                float_360_80421690;
            effSmallStarN64Entry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C) +
                    *(f32*)((s32)party + 0xF0),
                *(f32*)((s32)party + 0x60),
                sin(radians), float_neg1_80421648,
                -cos(radians), 3, 1);
            marioGetPtr();
            if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
                psndSFXOff(nokoSe);
                nokoSe = -1;
            }
            goto second_state_machine;
        }

        if (*(void**)((s32)party + 0x174) == 0) {
            radius =
                float_0p5_80421658 *
                    *(f32*)((s32)party + 0xF4) +
                float_12_804216a0;
            item = itemNearDistCheck(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C),
                *(f32*)((s32)party + 0x60),
                radius);
            *(void**)((s32)party + 0x174) = item;

            if (item != 0) {
                itemPickUp(item);
                *(u8*)((s32)party + 0x39) = 0x32;
                psndSFXOn_3D(
                    0x92C, (void*)((s32)party + 0x58));

                radians =
                    (float_6p2832_8042168c *
                     *(f32*)((s32)party + 0x100)) /
                    float_360_80421690;
                effSmallStarN64Entry(
                    *(f32*)((s32)party + 0x58),
                    *(f32*)((s32)party + 0x5C) +
                        *(f32*)((s32)party + 0xF0),
                    *(f32*)((s32)party + 0x60),
                    sin(radians), float_neg1_80421648,
                    -cos(radians), 3, 1);
                marioGetPtr();
                if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
                    psndSFXOff(nokoSe);
                    nokoSe = -1;
                }
                goto second_state_machine;
            }
        }

        if ((*(u32*)party & 0x80000000) == 0) {
            motion = *(u16*)((s32)player + 0x2E);
            if (motion == 0x19 || motion == 0x14 ||
                motion == 0x18 || motion == 0x16 ||
                motion == 0x1F || motion == 0x20 ||
                ((*(u32*)player & 0x20) != 0) ||
                marioChkKey() == 0) {
                nokonoko_finish(party);
                marioGetPtr();
                if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
                    psndSFXOff(nokoSe);
                    nokoSe = -1;
                }
                goto second_state_machine;
            }

            if ((*(u16*)((s32)player + 0x24A) &
                 0x400) == 0) {
                if (*(s16*)((s32)party + 0x17C) != 0) {
                    *(u8*)((s32)party + 0x39) = 0x50;
                } else {
                    *(u8*)((s32)party + 0x39) = 0x32;
                }

                marioGetPtr();
                if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
                    psndSFXOff(nokoSe);
                    nokoSe = -1;
                }
            }
        } else {
            if ((*(u16*)((s32)party + 0x17E) & 1) != 0) {
                *(u8*)((s32)party + 0x39) = 0x50;
            } else {
                *(u8*)((s32)party + 0x39) = 0x32;
            }

            marioGetPtr();
            if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
                psndSFXOff(nokoSe);
                nokoSe = -1;
            }
        }
    }

    state = *(u8*)((s32)party + 0x39);
    if (state == 0x15) {
        f32 turn;
        void* eff;

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            revise360(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        if ((*(u32*)((s32)gp + 0x1C) % 30) == 0) {
            eff = effKemuTestEntry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C),
                *(f32*)((s32)party + 0x60),
                float_1_80421670, 5);
            effKemuTestDrawCam(
                eff, *(s32*)((s32)party + 0x164));
        }
    }

second_state_machine:
    state = *(u8*)((s32)party + 0x39);

    if (state == 0x32) {
        partyClearJumpPara(party);
        *(u8*)((s32)party + 0x39) = 0x33;
        *(f32*)((s32)party + 0x104) = float_0_80421644;
        *(f32*)((s32)party + 0x100) =
            angleABf(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x60),
                *(f32*)((s32)party + 0x94),
                *(f32*)((s32)party + 0x9C));
        psndSFXOn_3D(
            0x92B, (void*)((s32)party + 0x58));
    } else if (state == 0x33 || state == 0x34) {
        if (state == 0x33) {
            f32 targetSpeed;

            targetSpeed = float_10_8042165c;
            if (*(s16*)((s32)party + 0x17C) != 0) {
                targetSpeed = float_20_804216ac;
            }

            *(f32*)((s32)party + 0x104) =
                float_0p3_8042169c *
                    (targetSpeed -
                     *(f32*)((s32)party + 0x104)) +
                *(f32*)((s32)party + 0x104);

            if (*(f32*)((s32)party + 0x104) >=
                targetSpeed) {
                *(f32*)((s32)party + 0x104) =
                    targetSpeed;
                if (*(f32*)((s32)party + 0x128) <
                    float_10_8042165c) {
                    *(u8*)((s32)party + 0x39) = 0x34;
                }
            }
        }

        if (ccwall(party, 1) != 0) {
            f32 moveDir;
            f32 moveSpeed;
            f64 radians;

            partyGetMoveDirSpd(
                party, &moveDir, &moveSpeed);
            unk_800c27c0(
                party, (void*)((s32)party + 0x58),
                moveSpeed, moveDir,
                *(f32*)((s32)party + 0xF4),
                *(f32*)((s32)party + 0xF0));

            *(u8*)((s32)party + 0x39) = 0x46;
            psndSFXOn_3D(
                0x92C, (void*)((s32)party + 0x58));

            radians =
                (float_6p2832_8042168c *
                 *(f32*)((s32)party + 0x100)) /
                float_360_80421690;
            effSmallStarN64Entry(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C) +
                    *(f32*)((s32)party + 0xF0),
                *(f32*)((s32)party + 0x60),
                sin(radians), float_neg1_80421648,
                -cos(radians), 3, 1);
            return;
        }

        {
            f32 turn;

            turn = float_neg1_80421648;
            if (revise360(
                    *(f32*)((s32)party + 0x100) -
                    *(f32*)((s32)party + 0xF8)) >
                float_180_8042163c) {
                turn = float_1_80421670;
            }
            *(f32*)((s32)party + 0x10C) =
                revise360(
                    float_50_80421694 * turn +
                    *(f32*)((s32)party + 0x10C));
        }

        *(f32*)((s32)party + 0x104) =
            float_0p1_80421640 *
                (float_neg1_80421648 -
                 *(f32*)((s32)party + 0x104)) +
            *(f32*)((s32)party + 0x104);
        *(f32*)((s32)party + 0x128) -=
            *(f32*)((s32)party + 0x104);

        if (*(f32*)((s32)party + 0x128) <=
            float_0_80421644) {
            *(f32*)((s32)party + 0x128) =
                float_0_80421644;
            *(u8*)((s32)party + 0x39) = 0x35;
            *(f32*)((s32)party + 0x104) =
                float_0p1_80421640 *
                    (float_neg1_80421648 -
                     *(f32*)((s32)party + 0x104)) +
                *(f32*)((s32)party + 0x104);
            *(f32*)((s32)party + 0x178) =
                float_50_80421694;
        }
    } else if (state == 0x35) {
        f32 turn;
        f32 factor;

        if (ccwall(party, 1) != 0) {
            *(u8*)((s32)party + 0x39) = 0x46;
            return;
        }

        *(f32*)((s32)party + 0x178) -=
            float_2p2_804216b0;
        if (*(f32*)((s32)party + 0x178) <=
            float_0_80421644) {
            *(f32*)((s32)party + 0x178) =
                float_0_80421644;
        }

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            revise360(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        factor = float_0p1_80421640;
        if (*(s16*)((s32)party + 0x17C) != 0) {
            factor = float_0p2_804216b8;
        }
        *(f32*)((s32)party + 0x104) =
            (float_neg2_804216b4 -
             *(f32*)((s32)party + 0x104)) *
                factor +
            *(f32*)((s32)party + 0x104);
        *(f32*)((s32)party + 0x128) -=
            *(f32*)((s32)party + 0x104);

        if (*(f32*)((s32)party + 0x104) <=
            float_0_80421644) {
            f32 safetyIn[3];
            f32 safetyOut[3];
            f32 safetyCase1[3];
            f32 safetyCase3[3];
            f32 safetyCase5[3];
            f32 safetyCase8[3];
            f32 safetyCase10[3];
            f32 safetyCase12[3];
            f32 safetyDefault[3];
            u32 safetyCode;

            *(f32*)((s32)party + 0x104) =
                float_0_80421644;

            *(NokonokoRawVec3*)safetyIn =
                    *(NokonokoRawVec3*)((s32)party + 0x94);

            safetyCode = lookupSafetyPos(
                *(f32*)((s32)party + 0xF0),
                safetyIn, safetyOut);

            if (*(void**)((s32)party + 0x138) != 0 &&
                ((safetyCode - 1) <= 1 ||
                 safetyCode == 4 ||
                 safetyCode == 8)) {
                *(u8*)((s32)party + 0x39) = 0x46;
            } else {
                if (safetyCode == 1) {
                    *(NokonokoRawVec3*)safetyCase1 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPosSub(
                        *(f32*)((s32)party + 0xF0),
                        float_270_804216c8,
                        safetyCase1, safetyOut);
                } else if (safetyCode == 3) {
                    *(NokonokoRawVec3*)safetyCase3 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPosSub(
                        *(f32*)((s32)party + 0xF0),
                        float_135_804216bc,
                        safetyCase3, safetyOut);
                } else if (safetyCode == 5) {
                    *(NokonokoRawVec3*)safetyCase5 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPosSub(
                        *(f32*)((s32)party + 0xF0),
                        float_315_804216c0,
                        safetyCase5, safetyOut);
                } else if (safetyCode == 8) {
                    *(NokonokoRawVec3*)safetyCase8 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPos3(
                        *(f32*)((s32)party + 0xF0),
                        safetyCase8, safetyOut);
                } else if (safetyCode == 10) {
                    *(NokonokoRawVec3*)safetyCase10 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPosSub(
                        *(f32*)((s32)party + 0xF0),
                        float_225_804216c4,
                        safetyCase10, safetyOut);
                } else if (safetyCode == 12) {
                    *(NokonokoRawVec3*)safetyCase12 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPosSub(
                        *(f32*)((s32)party + 0xF0),
                        float_315_804216c0,
                        safetyCase12, safetyOut);
                } else if (safetyCode == 7 ||
                           safetyCode == 14 ||
                           safetyCode == 15) {
                    *(u8*)((s32)party + 0x39) = 0x46;
                } else {
                    *(NokonokoRawVec3*)safetyDefault =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                    lookupSafetyPos2(
                        *(f32*)((s32)party + 0xF0),
                        safetyDefault, safetyOut);
                }

                *(u8*)((s32)party + 0x39) = 0x36;
                *(NokonokoRawVec3*)((s32)party + 0x94) =
                    *(NokonokoRawVec3*)safetyOut;
                *(f32*)((s32)party + 0x128) =
                    distABf(
                        *(f32*)((s32)party + 0x58),
                        *(f32*)((s32)party + 0x60),
                        *(f32*)((s32)party + 0x94),
                        *(f32*)((s32)party + 0x9C));
                *(f32*)((s32)party + 0x100) =
                    angleABf(
                        *(f32*)((s32)party + 0x58),
                        *(f32*)((s32)party + 0x60),
                        *(f32*)((s32)party + 0x94),
                        *(f32*)((s32)party + 0x9C));
            }
        }
    } else if (state == 0x36) {
        f32 turn;

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            revise360(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        *(f32*)((s32)party + 0x104) +=
            float_0p5_80421658;
        if (*(f32*)((s32)party + 0x104) >=
            float_4_804216cc) {
            *(f32*)((s32)party + 0x104) =
                float_4_804216cc;
        }

        *(f32*)((s32)party + 0x128) -=
            *(f32*)((s32)party + 0x104);

        if (*(f32*)((s32)party + 0x128) <=
            float_10_8042165c) {
            *(u8*)((s32)party + 0x39) = 0x46;
            *(f32*)((s32)party + 0x104) =
                float_0_80421644;
            return;
        }
    } else if (state == 0x37) {
        f32 turn;
        f32 factor;

        if (ccwall(party, 1) != 0) {
            *(u8*)((s32)party + 0x39) = 0x46;
            return;
        }

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            ((f32 (*)(f64))revise360)(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        *(f32*)((s32)party + 0x178) -=
            float_2p2_804216b0;
        if (*(f32*)((s32)party + 0x178) <=
            float_0_80421644) {
            *(f32*)((s32)party + 0x178) =
                float_0_80421644;
        }

        factor = float_0p1_80421640;
        if (*(s16*)((s32)party + 0x17C) != 0) {
            factor = float_0p2_804216b8;
        }
        *(f32*)((s32)party + 0x104) =
            (float_neg2_804216b4 -
             *(f32*)((s32)party + 0x104)) *
                factor +
            *(f32*)((s32)party + 0x104);

        if (*(f32*)((s32)party + 0x104) <=
            float_0_80421644) {
            *(u8*)((s32)party + 0x39) = 0x46;
            *(f32*)((s32)party + 0x104) =
                float_0_80421644;
            return;
        }
    } else if (state == 0x46) {
        f32 dx;
        f32 dy;
        f32 dz;
        f32 distance;

        dx =
            *(f32*)((s32)party + 0x58) -
            *(f32*)((s32)player + 0x8C);
        dy =
            *(f32*)((s32)party + 0x5C) -
            *(f32*)((s32)player + 0x90);
        dz =
            *(f32*)((s32)party + 0x60) -
            *(f32*)((s32)player + 0x94);
        distance = (f32)sqrt(
            (f64)(dx * dx + dy * dy + dz * dz));

        if (distance < float_5_804216d0) {
            f32 sideAngle;

            sideAngle = float_90_80421654;
            if ((f64)*(f32*)((s32)player + 0x1A4) >=
                double_180_802cc600) {
                sideAngle = float_neg90_804216d4;
            }

            movePos(
                float_0p5_80421658 *
                    *(f32*)((s32)player + 0x1B8),
                *(f32*)((s32)player + 0x1A4) +
                    sideAngle,
                (f32*)((s32)party + 0x58),
                (f32*)((s32)party + 0x60));
            ccwall(party, 1);
        }

        *(f32*)((s32)party + 0x104) =
            float_0_80421644;
        *(u32*)((s32)party + 4) &= ~0x10;
        *(u32*)((s32)party + 4) |= 0x20;
        *(u8*)((s32)party + 0x39) = 0x47;
    } else if (state == 0x47) {
        if (*(s32*)((s32)party + 0x174) != 0) {
            itemNokoForceGet(
                *(s32*)((s32)party + 0x174));
            *(s32*)((s32)party + 0x174) = 0;
        }
        nokonoko_finish(party);
        return;
    }

    state = *(u8*)((s32)party + 0x39);

    if (state == 0x50) {
        *(u8*)((s32)party + 0x39) = 0x51;
        *(f32*)((s32)party + 0x104) = float_0_80421644;
        *(f32*)((s32)party + 0x100) =
            angleABf(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x60),
                *(f32*)((s32)party + 0x94),
                *(f32*)((s32)party + 0x9C));
        *(f32*)((s32)party + 0x128) +=
            float_75_804216d8;
        psndSFXOn_3D(
            0x92B, (void*)((s32)party + 0x58));

        state = 0x51;
    }

    if (state == 0x51) {
        f32 turn;
        void* item;
        void* hit;
        f64 radians;

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            ((f32 (*)(f64))revise360)(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        *(f32*)((s32)party + 0x104) +=
            float_3_804216dc;
        if (*(f32*)((s32)party + 0x104) >=
            float_18_804216e0) {
            *(f32*)((s32)party + 0x104) =
                float_18_804216e0;
        }

        if (*(void**)((s32)party + 0x174) == 0) {
            item = itemNearDistCheck(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C),
                *(f32*)((s32)party + 0x60),
                float_0p5_80421658 *
                    *(f32*)((s32)party + 0xF4) +
                    float_12_804216a0);
            *(void**)((s32)party + 0x174) = item;

            if (item != 0) {
                itemPickUp(item);
                psndSFXOn_3D(
                    0x92C, (void*)((s32)party + 0x58));

                radians =
                    (float_6p2832_8042168c *
                     *(f32*)((s32)party + 0x100)) /
                    float_360_80421690;
                effSmallStarN64Entry(
                    *(f32*)((s32)party + 0x58),
                    *(f32*)((s32)party + 0x5C) +
                        *(f32*)((s32)party + 0xF0),
                    *(f32*)((s32)party + 0x60),
                    sin(radians), float_neg1_80421648,
                    -cos(radians), 3, 1);
            }
        }

        hit = ccwall(party, 1);
        if (hit != 0) {
            if ((hitGetAttr(hit) & 0x80000000) == 0) {
                *(u8*)((s32)party + 0x39) = 100;
                return;
            }

            mobjHitObjPtrToPtr((s32)hit);
            *(u8*)((s32)party + 0x39) = 0x6E;
            return;
        }

        *(f32*)((s32)party + 0x128) -=
            *(f32*)((s32)party + 0x104);
        if (*(f32*)((s32)party + 0x128) <=
            float_20_804216ac) {
            *(u8*)((s32)party + 0x39) = 0x52;
        }
    } else if (state == 0x52) {
        f32 turn;
        void* item;
        f64 radians;

        turn = float_neg1_80421648;
        if (revise360(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            ((f32 (*)(f64))revise360)(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        *(f32*)((s32)party + 0x104) =
            float_0p35_804216e4 *
                (float_neg1_80421648 -
                 *(f32*)((s32)party + 0x104)) +
            *(f32*)((s32)party + 0x104);

        if (*(void**)((s32)party + 0x174) == 0) {
            item = itemNearDistCheck(
                *(f32*)((s32)party + 0x58),
                *(f32*)((s32)party + 0x5C),
                *(f32*)((s32)party + 0x60),
                float_0p5_80421658 *
                    *(f32*)((s32)party + 0xF4) +
                    float_12_804216a0);
            *(void**)((s32)party + 0x174) = item;

            if (item != 0) {
                itemPickUp(item);
                psndSFXOn_3D(
                    0x92C, (void*)((s32)party + 0x58));

                radians =
                    (float_6p2832_8042168c *
                     *(f32*)((s32)party + 0x100)) /
                    float_360_80421690;
                effSmallStarN64Entry(
                    *(f32*)((s32)party + 0x58),
                    *(f32*)((s32)party + 0x5C) +
                        *(f32*)((s32)party + 0xF0),
                    *(f32*)((s32)party + 0x60),
                    sin(radians), float_neg1_80421648,
                    -cos(radians), 3, 1);
            }
        }

        if (ccwall(party, 1) == 0) {
            *(f32*)((s32)party + 0x128) -=
                *(f32*)((s32)party + 0x104);

            if (*(f32*)((s32)party + 0x128) <=
                float_0_80421644) {
                *(f32*)((s32)party + 0x128) =
                    distABf(
                        *(f32*)((s32)party + 0x94),
                        *(f32*)((s32)party + 0x9C),
                        *(f32*)((s32)party + 0x58),
                        *(f32*)((s32)party + 0x60));
                *(u8*)((s32)party + 0x39) = 0x54;
                *(f32*)((s32)party + 0x178) =
                    float_50_80421694;
                *(f32*)((s32)party + 0x100) =
                    angleABf(
                        *(f32*)((s32)party + 0x58),
                        *(f32*)((s32)party + 0x60),
                        *(f32*)((s32)party + 0x94),
                        *(f32*)((s32)party + 0x9C));
            }
        } else {
            *(u8*)((s32)party + 0x39) = 100;
        }
    } else if (state == 0x54) {
        f32 turn;

        turn = float_neg1_80421648;
        if (((f32 (*)(f64))revise360)(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            ((f32 (*)(f64))revise360)(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        if (ccwall(party, 1) == 0) {
            *(f32*)((s32)party + 0x128) -=
                *(f32*)((s32)party + 0x104);

            if (*(f32*)((s32)party + 0x128) <=
                float_30_80421660) {
                *(u8*)((s32)party + 0x39) = 0x56;
                *(f32*)((s32)party + 0x104) =
                    float_0_80421644;
                *(s32*)((s32)party + 0x24) =
                    sysMsec2Frame(500);
            }
        } else {
            *(u8*)((s32)party + 0x39) = 100;
        }
    } else if (state == 0x56) {
        f32 turn;
        s32 timer;

        turn = float_neg1_80421648;
        if (((f32 (*)(f64))revise360)(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            ((f32 (*)(f64))revise360)(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        timer = *(s32*)((s32)party + 0x24) - 1;
        *(s32*)((s32)party + 0x24) = timer;
        if (timer < 1) {
            *(u8*)((s32)party + 0x39) = 100;
        }

        if (ccwall(party, 1) != 0) {
            *(u8*)((s32)party + 0x39) = 100;
        }
    } else if (state == 100) {
        f32 dx;
        f32 dy;
        f32 dz;
        f32 distance;

        ccwall(party, 1);

        dx =
            *(f32*)((s32)party + 0x58) -
            *(f32*)((s32)player + 0x8C);
        dy =
            *(f32*)((s32)party + 0x5C) -
            *(f32*)((s32)player + 0x90);
        dz =
            *(f32*)((s32)party + 0x60) -
            *(f32*)((s32)player + 0x94);
        distance =
            (f32)sqrt((f64)(dx * dx + dy * dy + dz * dz));

        if (distance < float_5_804216d0) {
            f32 sideAngle;

            sideAngle = float_neg90_804216d4;
            if ((f64)*(f32*)((s32)player + 0x1A4) >=
                double_180_802cc600) {
                sideAngle = float_90_80421654;
            }

            movePos(
                float_0p5_80421658 *
                    *(f32*)((s32)player + 0x1B8),
                *(f32*)((s32)player + 0x1A4) +
                    sideAngle,
                (f32*)((s32)party + 0x58),
                (f32*)((s32)party + 0x60));
            ccwall(party, 1);
        }

        *(f32*)((s32)party + 0x104) =
            float_0_80421644;
        *(u32*)((s32)party + 4) &= ~0x10;

        if (*(void**)((s32)party + 0x174) != 0) {
            itemNokoForceGet(
                *(s32*)((s32)party + 0x174));
            *(s32*)((s32)party + 0x174) = 0;
        }

        if (*(void**)((s32)party + 0x138) != 0) {
            if (*(void**)((s32)party + 0x174) != 0) {
                itemNokoForceGet(
                    *(s32*)((s32)party + 0x174));
                *(s32*)((s32)party + 0x174) = 0;
            }

            nokonoko_finish(party);
            return;
        }

        *(u8*)((s32)party + 0x39) = 0x58;
    } else if (state == 0x58) {
        f32 safetyIn[3];
        f32 safetyOut[3];
        f32 safetyCase1[3];
        f32 safetyCase3[3];
        f32 safetyCase5[3];
        f32 safetyCase8[3];
        f32 safetyCase10[3];
        f32 safetyCase12[3];
        f32 safetyDefault[3];
        u32 safetyCode;

        *(f32*)((s32)party + 0x104) =
            float_0_80421644;

        *(NokonokoRawVec3*)safetyIn =
                    *(NokonokoRawVec3*)((s32)party + 0x94);

        safetyCode = lookupSafetyPos(
            *(f32*)((s32)party + 0xF0),
            safetyIn, safetyOut);

        if (*(void**)((s32)party + 0x138) != 0 &&
            ((safetyCode - 1) <= 1 ||
             safetyCode == 4 ||
             safetyCode == 8)) {
            *(u8*)((s32)party + 0x39) = 0x46;
        } else {
            if (safetyCode == 1) {
                *(NokonokoRawVec3*)safetyCase1 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPosSub(
                    *(f32*)((s32)party + 0xF0),
                    float_270_804216c8,
                    safetyCase1, safetyOut);
            } else if (safetyCode == 3) {
                *(NokonokoRawVec3*)safetyCase3 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPosSub(
                    *(f32*)((s32)party + 0xF0),
                    float_135_804216bc,
                    safetyCase3, safetyOut);
            } else if (safetyCode == 5) {
                *(NokonokoRawVec3*)safetyCase5 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPosSub(
                    *(f32*)((s32)party + 0xF0),
                    float_315_804216c0,
                    safetyCase5, safetyOut);
            } else if (safetyCode == 8) {
                *(NokonokoRawVec3*)safetyCase8 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPos3(
                    *(f32*)((s32)party + 0xF0),
                    safetyCase8, safetyOut);
            } else if (safetyCode == 10) {
                *(NokonokoRawVec3*)safetyCase10 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPosSub(
                    *(f32*)((s32)party + 0xF0),
                    float_225_804216c4,
                    safetyCase10, safetyOut);
            } else if (safetyCode == 12) {
                *(NokonokoRawVec3*)safetyCase12 =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPosSub(
                    *(f32*)((s32)party + 0xF0),
                    float_315_804216c0,
                    safetyCase12, safetyOut);
            } else if (safetyCode == 7 ||
                       safetyCode == 14 ||
                       safetyCode == 15) {
                *(u8*)((s32)party + 0x39) = 0x46;
            } else {
                *(NokonokoRawVec3*)safetyDefault =
                    *(NokonokoRawVec3*)((s32)party + 0x94);
                lookupSafetyPos2(
                    *(f32*)((s32)party + 0xF0),
                    safetyDefault, safetyOut);
            }

            *(u8*)((s32)party + 0x39) = 0x5A;
            *(NokonokoRawVec3*)((s32)party + 0x94) =
                    *(NokonokoRawVec3*)safetyOut;
            *(f32*)((s32)party + 0x128) =
                distABf(
                    *(f32*)((s32)party + 0x58),
                    *(f32*)((s32)party + 0x60),
                    *(f32*)((s32)party + 0x94),
                    *(f32*)((s32)party + 0x9C));
            *(f32*)((s32)party + 0x100) =
                angleABf(
                    *(f32*)((s32)party + 0x58),
                    *(f32*)((s32)party + 0x60),
                    *(f32*)((s32)party + 0x94),
                    *(f32*)((s32)party + 0x9C));
            *(f32*)((s32)party + 0x178) =
                float_50_80421694;
        }
    } else if (state == 0x5A) {
        f32 turn;

        turn = float_neg1_80421648;
        if (((f32 (*)(f64))revise360)(
                *(f32*)((s32)party + 0x100) -
                *(f32*)((s32)party + 0xF8)) >
            float_180_8042163c) {
            turn = float_1_80421670;
        }
        *(f32*)((s32)party + 0x10C) =
            ((f32 (*)(f64))revise360)(
                float_50_80421694 * turn +
                *(f32*)((s32)party + 0x10C));

        *(f32*)((s32)party + 0x104) +=
            float_0p5_80421658;
        if (*(f32*)((s32)party + 0x104) >=
            float_4_804216cc) {
            *(f32*)((s32)party + 0x104) =
                float_4_804216cc;
        }

        *(f32*)((s32)party + 0x128) -=
            *(f32*)((s32)party + 0x104);
        if (*(f32*)((s32)party + 0x128) <=
            float_10_8042165c) {
            *(u8*)((s32)party + 0x39) = 0x46;
            *(f32*)((s32)party + 0x104) =
                float_0_80421644;
        }
    } else if (state == 0x6E) {
        *(f32*)((s32)party + 0x104) =
            float_0_80421644;
        *(f32*)((s32)party + 0x114) =
            float_15_804216e8;
        *(f32*)((s32)party + 0x118) =
            float_0_80421644;
        *(s32*)((s32)party + 0x24) =
            sysMsec2Frame(1000);
        *(u8*)((s32)party + 0x39) = 0x6F;
        goto state_6f_body;
    } else if (state == 0x6F) {
state_6f_body:
        *(f32*)((s32)party + 0x5C) +=
            *(f32*)((s32)party + 0x114);

        *(NokonokoRawVec3*)pos =
            *(NokonokoRawVec3*)((s32)party + 0x58);
        marioGetScreenPos(pos, &sx, &sy, &sz);

        if (sy <= float_neg300_804216ec) {
            *(s32*)((s32)party + 0x24) =
                sysMsec2Frame(800);
            *(u8*)((s32)party + 0x39) = 0x70;
        }
    } else if (state == 0x71) {
        f32 pos71[3];
        f32 sx71;
        f32 sy71;
        f32 sz71;

        *(NokonokoRawVec3*)pos71 =
            *(NokonokoRawVec3*)((s32)party + 0x58);
        marioGetScreenPos(pos71, &sx71, &sy71, &sz71);

        if (sy71 > float_240_804216f8) {
            *(u32*)party &= ~0x02000000;
        }

        partyMove(party);

        if (*(f32*)((s32)party + 0x114) <=
            float_neg6_804216fc) {
            *(f32*)((s32)party + 0x118) =
                float_0_80421644;
            *(f32*)((s32)party + 0x114) =
                float_neg6_804216fc;
        }

        if (*(f32*)((s32)party + 0x5C) <
            float_neg2000_80421700) {
            *(NokonokoRawVec3*)((s32)party + 0x58) =
                *(NokonokoRawVec3*)((s32)player + 0x8C);
            *(f32*)((s32)party + 0x5C) +=
                float_200_80421704;
        }

        if (*(void**)((s32)party + 0x138) != 0) {
            *(f32*)((s32)party + 0x110) =
                *(f32*)((s32)player + 0x1B0);
            *(f32*)((s32)party + 0x10C) =
                *(f32*)((s32)party + 0x110);

            if ((hitGetAttr(
                     *(void**)((s32)party + 0x138)) &
                 0x200) == 0) {
                *(u8*)((s32)party + 0x39) = 0x72;
                *(s32*)((s32)party + 0x24) = 30;
            }
        }
    } else if (state == 0x72) {
        if (--*(s32*)((s32)party + 0x24) <= 0) {
            nokonoko_finish(party);
            return;
        }
    }

first_state_done:
    if (*(u8*)((s32)party + 0x39) != 100) {
        partyMoveNoHosei(party);
    }
    return;
}

void nokonoko_finish(void* party) {
    u32 item;
    f32 height;
    u32 flags;

    if (party == 0) {
        return;
    }
    if (*(s8*)((s32)party + 0x31) != 2) {
        return;
    }
    if (!(*(u32*)party & 0x100)) {
        return;
    }

    item = *(u32*)((s32)party + 0x174);
    if (item != 0) {
        itemNokoForceGet(item);
        *(s32*)((s32)party + 0x174) = 0;
    }
    marioGetPtr();
    if ((u32)(nokoSe + 0x10000) != 0xFFFF) {
        psndSFXOff(nokoSe);
        nokoSe = -1;
    }
    height = float_37_80421678;
    *(u32*)party &= ~0x100;
    *(u32*)party &= ~0x04000000;
    *(u32*)party &= ~0x01000000;
    *(u32*)party &= ~0x02000000;
    *(u32*)((s32)party + 4) &= ~0x20;
    *(u32*)((s32)party + 4) &= ~0x10;
    *(f32*)((s32)party + 0xF0) = height;
    *(s32*)((s32)party + 0x178) = 0;
    *(s32*)((s32)party + 0x17C) = 0;
    *(s32*)((s32)party + 0x180) = 0;

    if (*(u8*)((s32)party + 0x34) != 9) {
        flags = *(u32*)party;
        if (!(flags & 0x80000000)) {
            partyChgRunMode(party, 2);
            *(u8*)((s32)party + 0x3B) = 0;
        } else {
            *(u32*)party = flags & 0x7FFFFFFF;
            partyChgRunMode(party, 0);
        }
    }
}

void nokotaro_hold_cancel(void* party) {
    *(u8*)((s32)party + 0x39) = 0x14;
}

extern f32 __fabsf(f32 value);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* ccwall(void* party, s32 mode) {
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern void* hitCheckFilter(f64, f64, f64, f64, f64, f64, s32,
                                void*, void*, void*, void*, void*, void*, void*);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_0p4_80421674;
    extern f32 float_1_80421670;
    extern f32 float_6_8042166c;
    extern f32 float_11_80421668;
    extern f32 float_30_80421660;
    extern f32 float_100_80421664;

    f32 backCos3;
    f32 backSin3;
    f32 backCos2;
    f32 backSin2;
    f32 backCos1;
    f32 backSin1;
    f32 normalZ;
    f32 normalY;
    f32 normalX;
    f32 hitZ;
    f32 hitY;
    f32 hitX;
    f32 limit;
    f32 cosv;
    f32 sinv;
    f32 verticalOffset;
    f32 height;
    f32 currentY;
    f32 probeX;
    f32 probeY;
    f32 probeZ;
    f32 sideScale;
    f32 angle;
    f32 angleCos;
    f32 angleSin;
    f32 pushX;
    f32 pushZ;
    f32 dot;
    void* hit;

    verticalOffset = float_10_8042165c;
    height = *(f32*)((s32)party + 0xF0);

    limit = height + float_30_80421660;
    hit = hitCheckFilter(
        *(f32*)((s32)party + 0x58),
        verticalOffset + *(f32*)((s32)party + 0x5C) + height,
        *(f32*)((s32)party + 0x60),
        float_0_80421644,
        float_neg1_80421648,
        float_0_80421644,
        0,
        &hitX,
        &hitY,
        &hitZ,
        &limit,
        &normalX,
        &normalY,
        &normalZ);

    if (((hitGetAttr(hit) >> 9) & 1) == 1) {
        hit = 0;
    }

    *(void**)((s32)party + 0x138) = hit;
    if (hit != 0) {
        hitY =
            (f32)(s32)(float_100_80421664 * hitY +
                       float_0p5_80421658) /
            float_100_80421664;

        currentY =
            (f32)(s32)(float_100_80421664 *
                           *(f32*)((s32)party + 0x5C) +
                       float_0p5_80421658) /
            float_100_80421664;

        if (__fabsf(currentY - hitY) <= float_11_80421668) {
            if (hitY < currentY) {
                if (((*(s16*)((s32)party + 0x17E) & 2) != 0) &&
                    mode == 1) {
                    *(f32*)((s32)party + 0x5C) = hitY;
                }

                if (__fabsf(currentY - hitY) <= float_6_8042166c) {
                    *(f32*)((s32)party + 0x5C) = hitY;
                }
            } else if (mode == 0) {
                *(f32*)((s32)party + 0x5C) = hitY;
                if (float_1_80421670 <= __fabsf(currentY - hitY)) {
                    *(s16*)((s32)party + 0x17E) =
                        *(s16*)((s32)party + 0x17E) | 2;
                }
            }
        }
    }

    *(void**)((s32)party + 0x130) = 0;
    *(void**)((s32)party + 0x12C) = 0;

    verticalOffset += float_1_80421670;

    limit =
        float_0p5_80421658 * *(f32*)((s32)party + 0xF4) +
        *(f32*)((s32)party + 0x104);
    probeX = *(f32*)((s32)party + 0x58);
    probeZ = *(f32*)((s32)party + 0x60);
    probeY = *(f32*)((s32)party + 0x5C) + verticalOffset;

    sincosf(
        *(f32*)((s32)party + 0x100),
        &sinv,
        &cosv);

    hit = hitCheckFilter(
        probeX,
        probeY,
        probeZ,
        sinv,
        float_0_80421644,
        cosv,
        0,
        &hitX,
        &hitY,
        &hitZ,
        &limit,
        &normalX,
        &normalY,
        &normalZ);

    if (hit != 0 && (hitGetAttr(hit) & 5) == 0) {
        sincosf(
            float_180_8042163c + *(f32*)((s32)party + 0x100),
            &backSin1,
            &backCos1);

        *(f32*)((s32)party + 0x58) =
            float_0p5_80421658 * backSin1 *
                *(f32*)((s32)party + 0xF4) +
            hitX;
        *(f32*)((s32)party + 0x60) =
            float_0p5_80421658 * backCos1 *
                *(f32*)((s32)party + 0xF4) +
            hitZ;

        pushZ = cosv * *(f32*)((s32)party + 0x104);
        pushX = sinv * *(f32*)((s32)party + 0x104);
        dot = pushX * normalX + pushZ * normalZ;

        *(f32*)((s32)party + 0x58) =
            float_0p5_80421658 *
                -(dot * normalX - pushX) +
            *(f32*)((s32)party + 0x58);
        *(f32*)((s32)party + 0x60) =
            float_0p5_80421658 *
                -(dot * normalZ - pushZ) +
            *(f32*)((s32)party + 0x60);

        if (*(s16*)((s32)party + 0x17C) != 0 || mode == 0) {
            *(void**)((s32)party + 0x130) = hit;
        }
        *(void**)((s32)party + 0x12C) = hit;
        return hit;
    }

    sideScale =
        float_0p4_80421674 * *(f32*)((s32)party + 0xF4);
    angle =
        (float_3p1416_80421638 *
         (*(f32*)((s32)party + 0x100) - float_90_80421654)) /
        float_180_8042163c;

    angleCos = (f32)cos((f64)angle);
    probeZ = sideScale * angleCos + *(f32*)((s32)party + 0x60);

    angle =
        (float_3p1416_80421638 *
         (*(f32*)((s32)party + 0x100) - float_90_80421654)) /
        float_180_8042163c;
    angleSin = (f32)sin((f64)angle);
    probeX = sideScale * angleSin + *(f32*)((s32)party + 0x58);

    limit =
        float_0p4_80421674 * *(f32*)((s32)party + 0xF4) +
        *(f32*)((s32)party + 0x104);
    probeY = *(f32*)((s32)party + 0x5C) + verticalOffset;

    sincosf(
        *(f32*)((s32)party + 0x100),
        &sinv,
        &cosv);

    hit = hitCheckFilter(
        probeX,
        probeY,
        probeZ,
        sinv,
        float_0_80421644,
        cosv,
        0,
        &hitX,
        &hitY,
        &hitZ,
        &limit,
        &normalX,
        &normalY,
        &normalZ);

    if (hit != 0 && (hitGetAttr(hit) & 5) == 0) {
        sincosf(
            float_180_8042163c + *(f32*)((s32)party + 0x100),
            &backSin2,
            &backCos2);

        *(f32*)((s32)party + 0x58) =
            float_0p5_80421658 * backSin2 *
                *(f32*)((s32)party + 0xF4) +
            hitX;
        *(f32*)((s32)party + 0x60) =
            float_0p5_80421658 * backCos2 *
                *(f32*)((s32)party + 0xF4) +
            hitZ;

        angle =
            (float_3p1416_80421638 *
             (*(f32*)((s32)party + 0x100) - float_90_80421654)) /
            float_180_8042163c;
        angleCos = (f32)cos((f64)angle);
        *(f32*)((s32)party + 0x60) =
            -(sideScale * angleCos -
              *(f32*)((s32)party + 0x60));

        angle =
            (float_3p1416_80421638 *
             (*(f32*)((s32)party + 0x100) - float_90_80421654)) /
            float_180_8042163c;
        angleSin = (f32)sin((f64)angle);
        *(f32*)((s32)party + 0x58) =
            -(sideScale * angleSin -
              *(f32*)((s32)party + 0x58));

        pushZ = cosv * *(f32*)((s32)party + 0x104);
        pushX = sinv * *(f32*)((s32)party + 0x104);
        dot = pushX * normalX + pushZ * normalZ;

        *(f32*)((s32)party + 0x58) =
            float_0p5_80421658 *
                -(dot * normalX - pushX) +
            *(f32*)((s32)party + 0x58);
        *(f32*)((s32)party + 0x60) =
            float_0p5_80421658 *
                -(dot * normalZ - pushZ) +
            *(f32*)((s32)party + 0x60);

        if (*(s16*)((s32)party + 0x17C) != 0 || mode == 0) {
            *(void**)((s32)party + 0x130) = hit;
        }
        *(void**)((s32)party + 0x12C) = hit;

        *(f32*)((s32)party + 0x58) = hitX;
        return hit;
    }

    sideScale =
        float_0p4_80421674 * *(f32*)((s32)party + 0xF4);
    angle =
        (float_3p1416_80421638 *
         (float_90_80421654 + *(f32*)((s32)party + 0x100))) /
        float_180_8042163c;

    angleCos = (f32)cos((f64)angle);
    probeZ = sideScale * angleCos + *(f32*)((s32)party + 0x60);

    angle =
        (float_3p1416_80421638 *
         (float_90_80421654 + *(f32*)((s32)party + 0x100))) /
        float_180_8042163c;
    angleSin = (f32)sin((f64)angle);
    probeX = sideScale * angleSin + *(f32*)((s32)party + 0x58);

    limit =
        float_0p4_80421674 * *(f32*)((s32)party + 0xF4) +
        *(f32*)((s32)party + 0x104);
    probeY = *(f32*)((s32)party + 0x5C) + verticalOffset;

    sincosf(
        *(f32*)((s32)party + 0x100),
        &sinv,
        &cosv);

    hit = hitCheckFilter(
        probeX,
        probeY,
        probeZ,
        sinv,
        float_0_80421644,
        cosv,
        0,
        &hitX,
        &hitY,
        &hitZ,
        &limit,
        &normalX,
        &normalY,
        &normalZ);

    if (hit == 0 || (hitGetAttr(hit) & 5) != 0) {
        return 0;
    }

    sincosf(
        float_180_8042163c + *(f32*)((s32)party + 0x100),
        &backSin3,
        &backCos3);

    *(f32*)((s32)party + 0x58) =
        float_0p5_80421658 * backSin3 *
            *(f32*)((s32)party + 0xF4) +
        hitX;
    *(f32*)((s32)party + 0x60) =
        float_0p5_80421658 * backCos3 *
            *(f32*)((s32)party + 0xF4) +
        hitZ;

    angle =
        (float_3p1416_80421638 *
         (float_90_80421654 + *(f32*)((s32)party + 0x100))) /
        float_180_8042163c;
    angleCos = (f32)cos((f64)angle);
    *(f32*)((s32)party + 0x60) =
        -(sideScale * angleCos -
          *(f32*)((s32)party + 0x60));

    angle =
        (float_3p1416_80421638 *
         (float_90_80421654 + *(f32*)((s32)party + 0x100))) /
        float_180_8042163c;
    angleSin = (f32)sin((f64)angle);
    *(f32*)((s32)party + 0x58) =
        -(sideScale * angleSin -
          *(f32*)((s32)party + 0x58));

    pushZ = cosv * *(f32*)((s32)party + 0x104);
    pushX = sinv * *(f32*)((s32)party + 0x104);
    dot = pushX * normalX + pushZ * normalZ;

    *(f32*)((s32)party + 0x58) =
        float_0p5_80421658 *
            -(dot * normalX - pushX) +
        *(f32*)((s32)party + 0x58);
    *(f32*)((s32)party + 0x60) =
        float_0p5_80421658 *
            -(dot * normalZ - pushZ) +
        *(f32*)((s32)party + 0x60);

    if (*(s16*)((s32)party + 0x17C) != 0 || mode == 0) {
        *(void**)((s32)party + 0x130) = hit;
    }
    *(void**)((s32)party + 0x12C) = hit;

    *(f32*)((s32)party + 0x58) = hitX;
    return hit;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 lookupSafetyPos(f32 height, f32* inPos, f32* outPos) {
    extern f32 toMovedir(f32 dir);
    extern f32 revise360(f32 dir);
    extern s32 hitCheckFilter(double x, double y, double z,
                              double nx, double ny, double nz, s32 flags,
                              void* out1, void* out2, void* out3, void* out4,
                              void* out5, void* out6, void* out7);
    extern f32 float_0p1_80421640;
    void* player;
    f32 radius, dir, rad, limit;
    f32 x, y, z;
    volatile f32 p0[3], p1[3], p2[3], p3[3];
    s32 outA, outB, outC, outD, outE, outF;
    u32 h0, h1, h2, h3;
    u32 mask;

    player = marioGetPtr();
    radius = *(f32*)((s32)player + 0x1B8);
    limit = height + float_0p1_80421640;
    dir = revise360(toMovedir(*(f32*)((s32)player + 0x1B0)) - float_180_8042163c);
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    x = inPos[0] + radius * (f32)sin(rad);
    p0[0] = x;
    y = inPos[1];
    p0[1] = y;
    z = inPos[2] + radius * -(f32)cos(rad);
    p0[2] = z;
    h0 = hitCheckFilter(x, y + height, z,
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    dir = revise360(toMovedir(*(f32*)((s32)player + 0x1B0)) - float_90_80421654);
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    x = inPos[0] + radius * (f32)sin(rad);
    p1[0] = x;
    y = inPos[1];
    p1[1] = y;
    z = inPos[2] + radius * -(f32)cos(rad);
    p1[2] = z;
    limit = height + float_0p1_80421640;
    h1 = hitCheckFilter(x, y + height, z,
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    dir = revise360(float_90_80421654 + toMovedir(*(f32*)((s32)player + 0x1B0)));
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    x = inPos[0] + radius * (f32)sin(rad);
    p2[0] = x;
    y = inPos[1];
    p2[1] = y;
    z = inPos[2] + radius * -(f32)cos(rad);
    p2[2] = z;
    limit = height + float_0p1_80421640;
    h2 = hitCheckFilter(x, y + height, z,
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    dir = revise360(toMovedir(*(f32*)((s32)player + 0x1B0)));
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    x = inPos[0] + radius * (f32)sin(rad);
    p3[0] = x;
    y = inPos[1];
    p3[1] = y;
    z = inPos[2] + radius * -(f32)cos(rad);
    p3[2] = z;
    limit = height + float_0p1_80421640;
    h3 = hitCheckFilter(x, y + height, z,
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    mask = 0;
    if (h2 != 0) mask |= 4;
    if (h3 != 0) mask |= 8;
    if (h0 != 0) mask |= 1;
    if (h1 != 0) mask |= 2;
    if (h0 == 0) {
        if (h3 == 0) {
            if (h1 != 0) {
                outPos[0] = p1[0]; outPos[1] = p1[1]; outPos[2] = p1[2]; return mask;
            }
            if (h2 != 0) {
                outPos[0] = p2[0]; outPos[1] = p2[1]; outPos[2] = p2[2]; return mask;
            }
        } else {
            if (h1 != 0) {
                outPos[0] = float_0p5_80421658 * (p3[0] + p1[0]);
                outPos[1] = p3[1];
                outPos[2] = float_0p5_80421658 * (p3[2] + p1[2]); return mask;
            }
            if (h2 != 0) {
                outPos[0] = float_0p5_80421658 * (p2[0] + p1[0]);
                outPos[1] = p2[1];
                outPos[2] = float_0p5_80421658 * (p2[2] + p1[2]); return mask;
            }
            outPos[0] = p3[0]; outPos[1] = p3[1]; outPos[2] = p3[2];
        }
    } else {
        if ((h1 != 0) && (h2 != 0) && (h3 != 0)) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p2[0]);
            outPos[1] = p0[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p2[2]); return mask;
        }
        if (h1 != 0) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p1[0]);
            outPos[1] = inPos[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p1[2]); return mask;
        }
        if (h2 != 0) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p2[0]);
            outPos[1] = p0[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p2[2]); return mask;
        }
        if (h3 != 0) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p3[0]);
            outPos[1] = p0[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p3[2]); return mask;
        }
        outPos[0] = p0[0]; outPos[1] = p0[1]; outPos[2] = p0[2];
    }
    outPos[0] = inPos[0]; outPos[1] = inPos[1]; outPos[2] = inPos[2];
    return mask;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void lookupSafetyPos2(float height, float* inPos, float* outPos) {
    extern int hitCheckFilter(float x, float y, float z, float nx, float ny, float nz, int flags,
                              void* out1, void* out2, void* out3, void* out4, void* out5, void* out6, void* out7);

    int outA;
    int outB;
    int outC;
    int outD;
    int outE;
    int outF;
    float limit[2];
    void* player;
    float radius;
    float x;
    float y;
    float z;
    float probeLimit;
    u32 hit;
    extern const f32 float_0_80421644;
    extern f32 float_neg1_80421648;

    player = marioGetPtr();
    radius = *(float*)((int)player + 0x1B8);
    probeLimit = height + 0.1f;

    x = inPos[0] + radius * (float)sin(1.5708);
    y = inPos[1];
    z = inPos[2] + radius * -(float)cos(1.5708);
    limit[0] = probeLimit;
    hit = hitCheckFilter(x, y + height, z, float_0_80421644, float_neg1_80421648, float_0_80421644, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
    if (hit != 0) {
        outPos[0] = x; outPos[2] = z; outPos[1] = y;
    } else {
        x = inPos[0] + radius * (float)sin(4.7124);
        y = inPos[1];
        z = inPos[2] + radius * -(float)cos(4.7124);
        limit[0] = probeLimit;
        hit = hitCheckFilter(x, y + height, z, float_0_80421644, float_neg1_80421648, float_0_80421644, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
        if (hit != 0) {
            outPos[0] = x; outPos[2] = z; outPos[1] = y;
        } else {
            sin(3.1416);
            cos(3.1416);
            x = inPos[0];
            y = inPos[1];
            z = inPos[2];
            limit[0] = probeLimit;
            hit = hitCheckFilter(x, y + height, z, float_0_80421644, float_neg1_80421648, float_0_80421644, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
            if (hit != 0) {
                outPos[0] = x; outPos[2] = z; outPos[1] = y;
            } else {
                x = inPos[0] + radius * (float)sin(0.0);
                y = inPos[1];
                z = inPos[2] + radius * -(float)cos(0.0);
                limit[0] = probeLimit;
                hit = hitCheckFilter(x, y + height, z, float_0_80421644, float_neg1_80421648, float_0_80421644, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
                if (hit != 0) {
                    outPos[0] = x; outPos[2] = z; outPos[1] = y;
                } else {
                    outPos[0] = inPos[0]; outPos[1] = inPos[1]; outPos[2] = inPos[2];
                }
            }
        }
    }
}

void lookupSafetyPos3(float height, float* inPos, float* outPos) {
    extern int hitCheckFilter(float x, float y, float z, float nx, float ny, float nz, int flags,
                              void* out1, void* out2, void* out3, void* out4, void* out5, void* out6, void* out7);

    int outA;
    int outB;
    int outC;
    int outD;
    int outE;
    int outF;
    float limit[2];
    void* player;
    float radius;
    float x;
    float y;
    float z;
    u32 hit;
    extern f32 float_1p5708_8042164c;
    extern f32 float_4p7124_80421650;
    extern const f32 float_0_80421644;
    extern f32 float_neg1_80421648;

    player = marioGetPtr();
    radius = *(float*)((int)player + 0x1B8);
    x = inPos[0] + radius * (float)sin(float_1p5708_8042164c);
    y = inPos[1];
    z = inPos[2] + radius * -(float)cos(float_1p5708_8042164c);
    limit[0] = height + 0.1f;
    hit = hitCheckFilter(x, y + height, z, float_0_80421644, float_neg1_80421648, float_0_80421644, 0,
                         &outF, &outE, &outD, limit, &outC, &outB, &outA);
    if (hit != 0) {
        outPos[0] = x;
        outPos[2] = z;
        outPos[1] = y;
    } else {
        x = inPos[0] + radius * (float)sin(float_4p7124_80421650);
        y = inPos[1];
        z = inPos[2] + radius * -(float)cos(float_4p7124_80421650);
        limit[0] = height + 0.1f;
        hit = hitCheckFilter(x, y + height, z, float_0_80421644, float_neg1_80421648, float_0_80421644, 0,
                             &outF, &outE, &outD, limit, &outC, &outB, &outA);
        if (hit != 0) {
            outPos[0] = x;
            outPos[2] = z;
            outPos[1] = y;
        } else {
            outPos[0] = inPos[0];
            outPos[1] = inPos[1];
            outPos[2] = inPos[2];
        }
    }
}

void lookupSafetyPosSub(double height, double angle, float* inPos, float* outPos) {
    int outA;
    int outB;
    int outC;
    int outD;
    int outE;
    int outF;
    u32 hit;
    void* player;
    float radius;
    float probeX;
    float probeY;
    float probeZ;
    float limit;
    float localPos[3];
    float* sourcePos;

    extern int hitCheckFilter(double x, double y, double z, double nx, double ny, double nz, int flags,
                              void* out1, void* out2, void* out3, void* out4, void* out5, void* out6,
                              void* out7);
    extern const u32 vec3_802cc5d8[];

    player = marioGetPtr();
    radius = *(float*)((s32)player + 0x1B8);
    angle = angle * 3.14159265358979323846 / 180.0;

    probeX = inPos[0] + radius * (float)sin(angle);
    probeY = inPos[1] + (float)height;
    probeZ = inPos[2] + radius * -(float)cos(angle);
    limit = (float)height + 0.1f;

    hit = hitCheckFilter((double)probeX, (double)probeY, (double)probeZ, 0.0, -1.0, 0.0, 0,
                         &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    sourcePos = inPos;
    if (hit != 0) {
        ((u32*)localPos)[0] = vec3_802cc5d8[0];
        ((u32*)localPos)[1] = vec3_802cc5d8[1];
        ((u32*)localPos)[2] = vec3_802cc5d8[2];
        localPos[0] = probeX;
        localPos[1] = probeY;
        localPos[2] = probeZ;
        sourcePos = localPos;
    }
    ((u32*)outPos)[0] = ((u32*)sourcePos)[0];
    ((u32*)outPos)[1] = ((u32*)sourcePos)[1];
    ((u32*)outPos)[2] = ((u32*)sourcePos)[2];
}

const u32 vec3_802cc5d8[] = { 0, 0, 0 };
const char str_PNK_K_2_802cc5f0[] = "PNK_K_2";
const char str_PNK_A_1_802cc5f8[] = "PNK_A_1";

/* Target small-data definitions; keep scalar extern codegen unchanged. */
__declspec(section ".sdata2") f32 float_3p1416_80421638 = 3.1415927f;
__declspec(section ".sdata2") f32 float_180_8042163c = 180.0f;
__declspec(section ".sdata2") f32 float_0p1_80421640 = 0.1f;
__declspec(section ".sdata2") const f32 float_0_80421644 = 0.0f;
__declspec(section ".sdata2") f32 float_neg1_80421648 = -1.0f;
__declspec(section ".sdata2") f32 float_1p5708_8042164c = 1.5707964f;
__declspec(section ".sdata2") f32 float_4p7124_80421650 = 4.712389f;
__declspec(section ".sdata2") f32 float_90_80421654 = 90.0f;
__declspec(section ".sdata2") f32 float_0p5_80421658 = 0.5f;
__declspec(section ".sdata2") f32 float_10_8042165c = 10.0f;
__declspec(section ".sdata2") f32 float_30_80421660 = 30.0f;
__declspec(section ".sdata2") f32 float_100_80421664 = 100.0f;
__declspec(section ".sdata2") f32 float_11_80421668 = 11.0f;
__declspec(section ".sdata2") f32 float_6_8042166c = 6.0f;
__declspec(section ".sdata2") f32 float_1_80421670 = 1.0f;
__declspec(section ".sdata2") f32 float_0p4_80421674 = 0.4f;
__declspec(section ".sdata2") f32 float_37_80421678 = 37.0f;
__declspec(section ".sdata2") f32 float_160_8042167c = 160.0f;
__declspec(section ".sdata2") f32 float_18p5_80421680 = 18.5f;
__declspec(section ".sdata2") f32 float_2_80421684 = 2.0f;
__declspec(section ".sdata2") f32 float_45_80421688 = 45.0f;
__declspec(section ".sdata2") f32 float_6p2832_8042168c = 6.2831855f;
__declspec(section ".sdata2") f32 float_360_80421690 = 360.0f;
__declspec(section ".sdata2") f32 float_50_80421694 = 50.0f;
__declspec(section ".sdata2") f32 float_0p25_80421698 = 0.25f;
__declspec(section ".sdata2") f32 float_0p3_8042169c = 0.3f;
__declspec(section ".sdata2") f32 float_12_804216a0 = 12.0f;
__declspec(section ".sdata2") f32 float_120_804216a4 = 120.0f;
__declspec(section ".sdata2") f32 float_0p15_804216a8 = 0.15f;
__declspec(section ".sdata2") f32 float_20_804216ac = 20.0f;
__declspec(section ".sdata2") f32 float_2p2_804216b0 = 2.2f;
__declspec(section ".sdata2") f32 float_neg2_804216b4 = -2.0f;
__declspec(section ".sdata2") f32 float_0p2_804216b8 = 0.2f;
__declspec(section ".sdata2") f32 float_135_804216bc = 135.0f;
__declspec(section ".sdata2") f32 float_315_804216c0 = 315.0f;
__declspec(section ".sdata2") f32 float_225_804216c4 = 225.0f;
__declspec(section ".sdata2") f32 float_270_804216c8 = 270.0f;
__declspec(section ".sdata2") f32 float_4_804216cc = 4.0f;
__declspec(section ".sdata2") f32 float_5_804216d0 = 5.0f;
__declspec(section ".sdata2") f32 float_neg90_804216d4 = -90.0f;
__declspec(section ".sdata2") f32 float_75_804216d8 = 75.0f;
__declspec(section ".sdata2") f32 float_3_804216dc = 3.0f;
__declspec(section ".sdata2") f32 float_18_804216e0 = 18.0f;
__declspec(section ".sdata2") f32 float_0p35_804216e4 = 0.35f;
__declspec(section ".sdata2") f32 float_15_804216e8 = 15.0f;
__declspec(section ".sdata2") f32 float_neg300_804216ec = -300.0f;
__declspec(section ".sdata2") f32 float_neg30_804216f0 = -30.0f;
__declspec(section ".sdata2") f32 float_1000_804216f4 = 1000.0f;
__declspec(section ".sdata2") f32 float_240_804216f8 = 240.0f;
__declspec(section ".sdata2") f32 float_neg6_804216fc = -6.0f;
__declspec(section ".sdata2") f32 float_neg2000_80421700 = -2000.0f;
__declspec(section ".sdata2") f32 float_200_80421704 = 200.0f;
