#include "party/party_nokonoko.h"

extern s32 nokoSe;
extern f32 float_37_80421678;
extern f32 float_24_80421708;

void partyGoodbyeInit(void* party);
s32 partyGoodbyeMain(void* party);
void partyChgRunMode(void* party, s32 mode);
s32 marioGetPartyId(void);
void* partyGetPtr(s32 id);

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
    extern void* marioGetPtr(void);
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
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);

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
    extern void itemNokoForceGet(s32 item);
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);
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


u8 nokonoko_use(void* party) {
    extern s32 marioKeyOffChk(void);
    extern void marioGetScreenPos(void*, f32*, f32*, f32*);
    extern s32 marioChkInScreen(s32, s32);
    extern f64 angleABf(f64, f64, f64, f64);
    extern f64 distABf(f64, f64, f64, f64);
    extern void movePos(f64, f64, f32*, f32*);
    extern f64 toMovedirSimple(f64);
    extern f64 revise360(f64);
    extern void marioAdjustMoveDir(void);
    extern void nokonoko_finish(void*);
    extern void partyClearJumpPara(void*);
    extern void partyChgPose(void*, char*);
    extern void marioChgSmallJumpMotion(void);
    extern void marioChgMot(s32);
    extern f64 toMovedir(f64);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void partyMove(void*);
    extern void partyMoveNoHosei(void*);
    extern char str_PNK_K_2_802cc5f0[];
    extern char str_PNK_A_1_802cc5f8[];
    extern f32 float_18p5_80421680;
    extern f32 float_2_80421684;
    extern f32 float_45_80421688;
    extern f32 float_0_80421644;
    extern f32 float_6p2832_8042168c;
    extern f32 float_360_80421690;
    extern f32 float_neg1_80421648;
    extern f32 float_10_8042165c;
    extern f32 float_50_80421694;
    extern f32 float_180_8042163c;
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void effSmallStarN64Entry(f64, f64, f64, f64, f64, f64, s32, s32);
    void* player = *(void**)((s32)party + 0x160);
    u8 state;
    f32 sx, sy, sz;
    f32 pos[3];
    f32 x;
    f32 z;

    if ((*(u32*)party & 0x80000000) == 0 && marioKeyOffChk() != 0) {
        pos[0] = *(f32*)((s32)party + 0x58);
        pos[1] = *(f32*)((s32)party + 0x5C);
        pos[2] = *(f32*)((s32)party + 0x60);
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
            return 0;
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
        *(f32*)((s32)party + 0x94) = *(f32*)((s32)player + 0x8C);
        *(f32*)((s32)party + 0x98) = *(f32*)((s32)player + 0x90);
        *(f32*)((s32)party + 0x9C) = *(f32*)((s32)player + 0x94);
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
            *(f32*)((s32)party + 0x58) = *(f32*)((s32)party + 0x94);
            *(f32*)((s32)party + 0x5C) = *(f32*)((s32)party + 0x98);
            *(f32*)((s32)party + 0x60) = *(f32*)((s32)party + 0x9C);
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
            marioChgMot(6);
            *(u32*)player |= 0x8000;
            *(f32*)((s32)player + 0x180) = float_0_80421644;
            *(u8*)((s32)party + 0x39) = 10;
            *(s32*)((s32)party + 0x24) = 15;
            *(f32*)((s32)party + 0x104) = float_0_80421644;
            *(f32*)((s32)party + 0x114) = float_0_80421644;
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
        *(u32*)party |= 0x02000000;
        *(u8*)((s32)party + 0x39) = 11;
    case 11:
        *(f32*)((s32)party + 0x104) += float_2_80421684;
        if (*(f32*)((s32)party + 0x104) > float_10_8042165c) {
            *(f32*)((s32)party + 0x104) = float_10_8042165c;
        }
        {
            f32 turn = float_neg1_80421648;
            if (revise360(*(f32*)((s32)party + 0x100) -
                          *(f32*)((s32)party + 0x110)) >
                float_180_8042163c) {
                turn = 1.0f;
            }
            *(f32*)((s32)party + 0x10C) =
                revise360(float_50_80421694 * turn +
                          *(f32*)((s32)party + 0x10C));
        }
        partyMoveNoHosei(party);
        break;
    }
    return 0;
}

void nokonoko_finish(void* party) {
    extern void itemNokoForceGet(s32 item);
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* ccwall(void* party, s32 mode) {
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern void* hitCheckFilter(f64, f64, f64, f64, f64, f64, s32,
                                void*, void*, void*, void*, void*, void*, void*);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_3p1416_80421638;
    extern f32 float_180_8042163c;
    extern f32 float_0_80421644;
    extern f32 float_neg1_80421648;
    extern f32 float_90_80421654;
    extern f32 float_0p5_80421658;
    extern f32 float_0p4_80421674;
    extern f32 float_1_80421670;
    extern f32 float_6_8042166c;
    extern f32 float_10_8042165c;
    extern f32 float_11_80421668;
    extern f32 float_30_80421660;
    extern f32 float_100_80421664;

    f32* pos;
    f32 height;
    f32 width;
    f32 direction;
    f32 velocity;
    f32 limit;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 sinv;
    f32 cosv;
    f32 backSin;
    f32 backCos;
    f32 sideScale;
    f32 angle;
    f32 startX;
    f32 startZ;
    f32 ground;
    f32 current;
    f32 delta;
    f32 pushX;
    f32 pushZ;
    f32 dot;
    void* hit;

    pos = (f32*)((s32)party + 0x58);
    height = *(f32*)((s32)party + 0xF0);
    width = *(f32*)((s32)party + 0xF4);
    direction = *(f32*)((s32)party + 0x100);
    velocity = *(f32*)((s32)party + 0x104);

    limit = height + float_30_80421660;
    hit = hitCheckFilter(pos[0], pos[1] + height + float_10_8042165c, pos[2],
                         float_0_80421644, float_neg1_80421648, float_0_80421644,
                         0, &hitX, &hitY, &hitZ, &limit, &normalX, &normalY, &normalZ);
    if (((hitGetAttr(hit) >> 9) & 1) != 0) {
        hit = 0;
    }
    *(void**)((s32)party + 0x138) = hit;
    if (hit != 0) {
        ground = (f32)(s32)(hitY * float_100_80421664 + float_0p5_80421658) / float_100_80421664;
        current = (f32)(s32)(pos[1] * float_100_80421664 + float_0p5_80421658) / float_100_80421664;
        delta = current - ground;
        if (delta < float_0_80421644) {
            delta = -delta;
        }
        if (delta <= float_11_80421668) {
            if (ground < current) {
                if (((*(u16*)((s32)party + 0x12E) & 2) != 0) && mode == 1) {
                    pos[1] = ground;
                }
                if (delta <= float_6_8042166c) {
                    pos[1] = ground;
                }
            } else if (mode == 0) {
                pos[1] = ground;
                if (float_1_80421670 <= delta) {
                    *(u16*)((s32)party + 0x12E) = *(u16*)((s32)party + 0x12E) | 2;
                }
            }
        }
    }

    *(void**)((s32)party + 0x134) = 0;
    *(void**)((s32)party + 0x13C) = 0;

    sincosf(direction, &sinv, &cosv);
    limit = float_0p5_80421658 * width + velocity;
    hit = hitCheckFilter(pos[0], pos[1] + float_10_8042165c + float_1_80421670, pos[2],
                         sinv, float_0_80421644, cosv, 0,
                         &hitX, &hitY, &hitZ, &limit, &normalX, &normalY, &normalZ);
    if ((hit == 0) || ((hitGetAttr(hit) & 5) != 0)) {
        sideScale = float_0p4_80421674 * width;
        angle = (float_3p1416_80421638 * (direction - float_90_80421654)) / float_180_8042163c;
        startX = sideScale * (f32)sin(angle) + pos[0];
        startZ = sideScale * (f32)cos(angle) + pos[2];
        sincosf(direction, &sinv, &cosv);
        limit = float_0p4_80421674 * width + velocity;
        hit = hitCheckFilter(startX, pos[1] + float_10_8042165c + float_1_80421670, startZ,
                             sinv, float_0_80421644, cosv, 0,
                             &hitX, &hitY, &hitZ, &limit, &normalX, &normalY, &normalZ);
        if ((hit == 0) || ((hitGetAttr(hit) & 5) != 0)) {
            sideScale = float_0p4_80421674 * width;
            angle = (float_3p1416_80421638 * (float_90_80421654 + direction)) / float_180_8042163c;
            startX = sideScale * (f32)sin(angle) + pos[0];
            startZ = sideScale * (f32)cos(angle) + pos[2];
            sincosf(direction, &sinv, &cosv);
            limit = float_0p4_80421674 * width + velocity;
            hit = hitCheckFilter(startX, pos[1] + float_10_8042165c + float_1_80421670, startZ,
                                 sinv, float_0_80421644, cosv, 0,
                                 &hitX, &hitY, &hitZ, &limit, &normalX, &normalY, &normalZ);
            if ((hit == 0) || ((hitGetAttr(hit) & 5) != 0)) {
                hit = 0;
            } else {
                sincosf(float_180_8042163c + direction, &backSin, &backCos);
                pos[0] = float_0p5_80421658 * backSin * width + hitX;
                pos[2] = float_0p5_80421658 * backCos * width + hitZ;
                angle = (float_3p1416_80421638 * (float_90_80421654 + direction)) / float_180_8042163c;
                pos[2] = -(sideScale * (f32)cos(angle) - pos[2]);
                pos[0] = -(sideScale * (f32)sin(angle) - pos[0]);
                pushZ = cosv * velocity;
                pushX = sinv * velocity;
                dot = pushX * normalX + pushZ * normalZ;
                pos[0] = float_0p5_80421658 * -(dot * normalX - pushX) + pos[0];
                pos[2] = float_0p5_80421658 * -(dot * normalZ - pushZ) + pos[2];
                if (*(s16*)((s32)party + 0x12C) != 0 || mode == 0) {
                    *(void**)((s32)party + 0x134) = hit;
                }
                *(void**)((s32)party + 0x13C) = hit;
                pos[0] = hitX;
            }
        } else {
            sincosf(float_180_8042163c + direction, &backSin, &backCos);
            pos[0] = float_0p5_80421658 * backSin * width + hitX;
            pos[2] = float_0p5_80421658 * backCos * width + hitZ;
            angle = (float_3p1416_80421638 * (direction - float_90_80421654)) / float_180_8042163c;
            pos[2] = -(sideScale * (f32)cos(angle) - pos[2]);
            pos[0] = -(sideScale * (f32)sin(angle) - pos[0]);
            pushZ = cosv * velocity;
            pushX = sinv * velocity;
            dot = pushX * normalX + pushZ * normalZ;
            pos[0] = float_0p5_80421658 * -(dot * normalX - pushX) + pos[0];
            pos[2] = float_0p5_80421658 * -(dot * normalZ - pushZ) + pos[2];
            if (*(s16*)((s32)party + 0x12C) != 0 || mode == 0) {
                *(void**)((s32)party + 0x134) = hit;
            }
            *(void**)((s32)party + 0x13C) = hit;
        }
    } else {
        sincosf(float_180_8042163c + direction, &backSin, &backCos);
        pos[0] = float_0p5_80421658 * backSin * width + hitX;
        pos[2] = float_0p5_80421658 * backCos * width + hitZ;
        pushZ = cosv * velocity;
        pushX = sinv * velocity;
        dot = pushX * normalX + pushZ * normalZ;
        pos[0] = float_0p5_80421658 * -(dot * normalX - pushX) + pos[0];
        pos[2] = float_0p5_80421658 * -(dot * normalZ - pushZ) + pos[2];
        if (*(s16*)((s32)party + 0x12C) != 0 || mode == 0) {
            *(void**)((s32)party + 0x134) = hit;
        }
        *(void**)((s32)party + 0x13C) = hit;
    }
    return hit;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 lookupSafetyPos(f32 height, f32* inPos, f32* outPos) {
    extern void* marioGetPtr(void);
    extern f32 toMovedir(f32 dir);
    extern f32 revise360(f32 dir);
    extern double sin(double x);
    extern double cos(double x);
    extern s32 hitCheckFilter(double x, double y, double z,
                              double nx, double ny, double nz, s32 flags,
                              void* out1, void* out2, void* out3, void* out4,
                              void* out5, void* out6, void* out7);
    extern f32 float_180_8042163c;
    extern f32 float_3p1416_80421638;
    extern f32 float_0p1_80421640;
    extern f32 float_0_80421644;
    extern f32 float_neg1_80421648;
    extern f32 float_90_80421654;
    extern f32 float_0p5_80421658;

    void* player;
    f32 radius;
    f32 dir;
    f32 rad;
    f32 limit;
    f32 p0[3];
    f32 p1[3];
    f32 p2[3];
    f32 p3[3];
    s32 outA;
    s32 outB;
    s32 outC;
    s32 outD;
    s32 outE;
    s32 outF;
    s32 h0;
    s32 h1;
    s32 h2;
    s32 h3;
    u32 mask;

    player = marioGetPtr();
    radius = *(f32*)((s32)player + 0x1B8);
    limit = height + float_0p1_80421640;

    dir = revise360(toMovedir(*(f32*)((s32)player + 0x1B0)) - float_180_8042163c);
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    p0[0] = inPos[0] + radius * (f32)sin(rad);
    p0[1] = inPos[1];
    p0[2] = inPos[2] + radius * -(f32)cos(rad);
    h0 = hitCheckFilter(p0[0], p0[1] + height, p0[2],
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    dir = revise360(toMovedir(*(f32*)((s32)player + 0x1B0)) - float_90_80421654);
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    p1[0] = inPos[0] + radius * (f32)sin(rad);
    p1[1] = inPos[1];
    p1[2] = inPos[2] + radius * -(f32)cos(rad);
    limit = height + float_0p1_80421640;
    h1 = hitCheckFilter(p1[0], p1[1] + height, p1[2],
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    dir = revise360(float_90_80421654 + toMovedir(*(f32*)((s32)player + 0x1B0)));
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    p2[0] = inPos[0] + radius * (f32)sin(rad);
    p2[1] = inPos[1];
    p2[2] = inPos[2] + radius * -(f32)cos(rad);
    limit = height + float_0p1_80421640;
    h2 = hitCheckFilter(p2[0], p2[1] + height, p2[2],
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    dir = revise360(toMovedir(*(f32*)((s32)player + 0x1B0)));
    rad = float_3p1416_80421638 * dir / float_180_8042163c;
    p3[0] = inPos[0] + radius * (f32)sin(rad);
    p3[1] = inPos[1];
    p3[2] = inPos[2] + radius * -(f32)cos(rad);
    limit = height + float_0p1_80421640;
    h3 = hitCheckFilter(p3[0], p3[1] + height, p3[2],
                        float_0_80421644, float_neg1_80421648, float_0_80421644,
                        0, &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    mask = 0;
    if (h2 != 0) {
        mask |= 4;
    }
    if (h3 != 0) {
        mask |= 8;
    }
    if (h0 != 0) {
        mask |= 1;
    }
    if (h1 != 0) {
        mask |= 2;
    }

    if (h0 == 0) {
        if (h3 == 0) {
            if (h1 != 0) {
                outPos[0] = p1[0]; outPos[1] = p1[1]; outPos[2] = p1[2];
                return mask;
            }
            if (h2 != 0) {
                outPos[0] = p2[0]; outPos[1] = p2[1]; outPos[2] = p2[2];
                return mask;
            }
        } else {
            if (h1 != 0) {
                outPos[0] = float_0p5_80421658 * (p3[0] + p1[0]);
                outPos[1] = p3[1];
                outPos[2] = float_0p5_80421658 * (p3[2] + p1[2]);
                return mask;
            }
            if (h2 != 0) {
                outPos[0] = float_0p5_80421658 * (p2[0] + p1[0]);
                outPos[1] = p2[1];
                outPos[2] = float_0p5_80421658 * (p2[2] + p1[2]);
                return mask;
            }
            outPos[0] = p3[0]; outPos[1] = p3[1]; outPos[2] = p3[2];
        }
    } else {
        if ((h1 != 0) && (h2 != 0) && (h3 != 0)) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p2[0]);
            outPos[1] = p0[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p2[2]);
            return mask;
        }
        if (h1 != 0) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p1[0]);
            outPos[1] = inPos[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p1[2]);
            return mask;
        }
        if (h2 != 0) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p2[0]);
            outPos[1] = p0[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p2[2]);
            return mask;
        }
        if (h3 != 0) {
            outPos[0] = float_0p5_80421658 * (p0[0] + p3[0]);
            outPos[1] = p0[1];
            outPos[2] = float_0p5_80421658 * (p0[2] + p3[2]);
            return mask;
        }
        outPos[0] = p0[0]; outPos[1] = p0[1]; outPos[2] = p0[2];
    }
    outPos[0] = inPos[0];
    outPos[1] = inPos[1];
    outPos[2] = inPos[2];
    return mask;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void lookupSafetyPos2(float height, float* inPos, float* outPos) {
    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern int hitCheckFilter(double x, double y, double z, double nx, double ny, double nz, int flags,
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
    int hit;

    player = marioGetPtr();
    radius = *(float*)((int)player + 0x1B8);
    probeLimit = height + 0.1f;

    x = inPos[0] + radius * (float)sin(1.5708);
    y = inPos[1];
    z = inPos[2] + radius * -(float)cos(1.5708);
    limit[0] = probeLimit;
    hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
    if (hit != 0) {
        outPos[0] = x; outPos[1] = y; outPos[2] = z;
    } else {
        x = inPos[0] + radius * (float)sin(4.7124);
        y = inPos[1];
        z = inPos[2] + radius * -(float)cos(4.7124);
        limit[0] = probeLimit;
        hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
        if (hit != 0) {
            outPos[0] = x; outPos[1] = y; outPos[2] = z;
        } else {
            sin(3.1416);
            cos(3.1416);
            x = inPos[0];
            y = inPos[1];
            z = inPos[2];
            limit[0] = probeLimit;
            hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
            if (hit != 0) {
                outPos[0] = x; outPos[1] = y; outPos[2] = z;
            } else {
                x = inPos[0] + radius * (float)sin(0.0);
                y = inPos[1];
                z = inPos[2] + radius * -(float)cos(0.0);
                limit[0] = probeLimit;
                hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0, &outF, &outE, &outD, limit, &outC, &outB, &outA);
                if (hit != 0) {
                    outPos[0] = x; outPos[1] = y; outPos[2] = z;
                } else {
                    outPos[0] = inPos[0]; outPos[1] = inPos[1]; outPos[2] = inPos[2];
                }
            }
        }
    }
}

void lookupSafetyPos3(float height, float* inPos, float* outPos) {
    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern int hitCheckFilter(double x, double y, double z, double nx, double ny, double nz, int flags,
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
    int hit;

    player = marioGetPtr();
    radius = *(float*)((int)player + 0x1B8);
    x = inPos[0] + radius * (float)sin(1.5708);
    y = inPos[1];
    z = inPos[2] + radius * -(float)cos(1.5708);
    limit[0] = height + 0.1f;
    hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0,
                         &outF, &outE, &outD, limit, &outC, &outB, &outA);
    if (hit != 0) {
        outPos[0] = x;
        outPos[1] = y;
        outPos[2] = z;
    } else {
        x = inPos[0] + radius * (float)sin(4.7124);
        y = inPos[1];
        z = inPos[2] + radius * -(float)cos(4.7124);
        limit[0] = height + 0.1f;
        hit = hitCheckFilter(x, y + height, z, 0.0, -1.0, 0.0, 0,
                             &outF, &outE, &outD, limit, &outC, &outB, &outA);
        if (hit != 0) {
            outPos[0] = x;
            outPos[1] = y;
            outPos[2] = z;
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
    int hit;
    void* player;
    float radius;
    float probeX;
    float probeY;
    float probeZ;
    float limit;

    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern int hitCheckFilter(double x, double y, double z, double nx, double ny, double nz, int flags,
                              void* out1, void* out2, void* out3, void* out4, void* out5, void* out6,
                              void* out7);

    player = marioGetPtr();
    radius = *(float*)((s32)player + 0x1B8);
    angle = angle * 3.14159265358979323846 / 180.0;

    probeX = inPos[0] + radius * (float)sin(angle);
    probeY = inPos[1] + (float)height;
    probeZ = inPos[2] + radius * -(float)cos(angle);
    limit = (float)height + 0.1f;

    hit = hitCheckFilter((double)probeX, (double)probeY, (double)probeZ, 0.0, -1.0, 0.0, 0,
                         &outF, &outE, &outD, &limit, &outC, &outB, &outA);

    if (hit != 0) {
        outPos[0] = probeX;
        outPos[1] = probeY;
        outPos[2] = probeZ;
    } else {
        outPos[0] = inPos[0];
        outPos[1] = inPos[1];
        outPos[2] = inPos[2];
    }
}
