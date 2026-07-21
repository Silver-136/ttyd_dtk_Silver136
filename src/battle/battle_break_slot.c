#include "battle/battle_break_slot.h"

void item_appear_force(void* reel, s32 item);

extern void* _battleWorkPointer;
extern f32 float_490_804268ac;

void BattleBreakSlotDispReel(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);
void BattleFree(void* ptr);
s32 evtCheckID(s32 id);
void btl_camera_set_mode(s32 cameraId, s32 mode);
void btl_camera_set_moveSpeedLv(s32 cameraId, s32 level);
void BattleAudienceNumToTarget(void);

void* BattleBreakSlotGetPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1616C);
}

void* BattleBreakSlotReelGetPtr(s32 index) {
    s32 offset = index * 0x5C;
    void* work = BattleBreakSlotGetPtr();

    return (void*)((s32)work + offset + 0x18);
}

/* stub-fill: BattleBreakSlot_Init | missing_definition | header_prototype */
void BattleBreakSlot_Init(void) {
    extern void* BattleAlloc(u32 size);
    extern void* memset(void* dst, int value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern const s32 R_slot_1_cards[];
    extern const s32 R_slot_2_cards[];
    extern const s32 R_slot_3_cards[];
    void* work = BattleBreakSlotGetPtr();
    void* reel;
    s32 i;

    memset(work, 0, 0x134);
    *(s32*)((s32)work + 4) = 0;

    reel = BattleBreakSlotReelGetPtr(0);
    for (i = 0; i < 3; i++) {
        *(s32*)reel = i;
        *(s32*)((s32)reel + 8) = 0;
        reel = (void*)((s32)reel + 0x5C);
    }

    reel = BattleBreakSlotReelGetPtr(0);
    *(s32*)((s32)reel + 0x18) = 8;
    *(void**)((s32)reel + 0x14) = BattleAlloc(*(s32*)((s32)reel + 0x18) * 4);
    memcpy(*(void**)((s32)reel + 0x14), R_slot_1_cards, *(s32*)((s32)reel + 0x18) * 4);

    reel = BattleBreakSlotReelGetPtr(1);
    *(s32*)((s32)reel + 0x18) = 8;
    *(void**)((s32)reel + 0x14) = BattleAlloc(*(s32*)((s32)reel + 0x18) * 4);
    memcpy(*(void**)((s32)reel + 0x14), R_slot_2_cards, *(s32*)((s32)reel + 0x18) * 4);

    reel = BattleBreakSlotReelGetPtr(2);
    *(s32*)((s32)reel + 0x18) = 5;
    *(void**)((s32)reel + 0x14) = BattleAlloc(*(s32*)((s32)reel + 0x18) * 4);
    memcpy(*(void**)((s32)reel + 0x14), R_slot_3_cards, *(s32*)((s32)reel + 0x18) * 4);
}

/* CHATGPT STUB FILL: main/battle/battle_break_slot 20260624_185954 */

/* stub-fill: BattleBreakSlot_Main | missing_definition | header_prototype */
void BattleBreakSlot_Main(void) {
    extern void* BattleBreakSlotGetPtr(void);
    extern void* BattleBreakSlotReelGetPtr(s32);
    extern s32 keyGetButtonTrg(s32);
    extern s32 rand(void);
    extern s32 psndSFXOn(void*);
    extern void psndSFXOff(s32);
    extern f32 intplGetValue(f64, f64, s32, s32, s32);
    extern f64 sin(f64);
    u8* work = BattleBreakSlotGetPtr();
    u8* reel;
    s32 state = *(s32*)(work + 4);
    s32 count;
    s32 i;

    switch (state) {
        case 0:
            count = 0;
            for (i = 0; i < 3; i++) {
                reel = BattleBreakSlotReelGetPtr(i);
                if (*(s32*)(reel + 8) == 2) count++;
            }
            if (count == 2 && *(s32*)((u8*)BattleBreakSlotReelGetPtr(0) + 0x1C) !=
                              *(s32*)((u8*)BattleBreakSlotReelGetPtr(1) + 0x1C)) {
                *(s32*)(work + 4) = 1;
                *(s32*)(work + 8) = 0;
            }
            break;
        case 1:
            if (*(s32*)(work + 8) == 60) {
                for (i = 0; i < 2; i++) {
                    reel = BattleBreakSlotReelGetPtr(i);
                    *(s32*)(reel + 8) = 3;
                    *(s32*)(reel + 0xC) = 0;
                }
            }
            count = 0;
            for (i = 0; i < 3; i++) {
                if (*(s32*)((u8*)BattleBreakSlotReelGetPtr(i) + 8) == 0) count++;
            }
            if (count == 3) {
                *(s32*)(work + 4) = 8;
                *(s32*)(work + 0xC) = 0;
            }
            (*(s32*)(work + 8))++;
            break;
        case 2:
            count = 0;
            for (i = 0; i < 3; i++) {
                s32 reelState = *(s32*)((u8*)BattleBreakSlotReelGetPtr(i) + 8);
                if (reelState == 6 || reelState == 10) count++;
            }
            if (count == 3) {
                *(s32*)(work + 4) = 3;
                *(s32*)(work + 8) = 0;
                for (i = 0; i < 3; i++) {
                    reel = BattleBreakSlotReelGetPtr(i);
                    if (*(s32*)(reel + 8) == 6) *(s32*)(reel + 8) = 7;
                }
                *(s32*)(work + 0x14) = psndSFXOn((void*)"SFX_BTL_SLOT_START1");
            }
            break;
        case 3:
            count = 0;
            for (i = 0; i < 3; i++) {
                if (*(s32*)((u8*)BattleBreakSlotReelGetPtr(i) + 8) == 10) count++;
            }
            if (count == 3) {
                *(s32*)(work + 4) = 4;
                *(s32*)(work + 8) = 0;
                psndSFXOff(*(s32*)(work + 0x14));
            }
            if ((keyGetButtonTrg(0) & 0x100) && *(s32*)(work + 8) > 14) {
                for (i = 0; i < 3; i++) {
                    reel = BattleBreakSlotReelGetPtr(i);
                    if (*(s32*)(reel + 8) == 8) {
                        *(s32*)(reel + 8) = 9;
                        *(f32*)(reel + 0x54) = -1.0f;
                        break;
                    }
                }
            }
            (*(s32*)(work + 8))++;
            break;
        case 4:
            if (*(s32*)(work + 8) == 20) {
                count = 0;
                for (i = 0; i < 2; i++) {
                    reel = BattleBreakSlotReelGetPtr(i);
                    if (*(s32*)(reel + 0x1C) == *(s32*)(reel + 0x78)) count++;
                }
                *(s32*)(work + 4) = count == 2 ? 5 : 7;
                for (i = 0; i < 3; i++) {
                    reel = BattleBreakSlotReelGetPtr(i);
                    *(s32*)(reel + 8) = count == 2 ? 11 : 12;
                    *(s32*)(reel + 0xC) = 0;
                }
            }
            (*(s32*)(work + 8))++;
            break;
        case 5:
        case 7:
            count = 0;
            for (i = 0; i < 3; i++) {
                if (*(s32*)((u8*)BattleBreakSlotReelGetPtr(i) + 8) == 0) count++;
            }
            if (count == 3) {
                *(s32*)(work + 4) = state == 5 ? 6 : 8;
                *(s32*)(work + 8) = 0;
            }
            break;
        case 8:
            *(s32*)(work + 4) = 0;
            break;
    }

    for (i = 0; i < 3; i++) {
        reel = BattleBreakSlotReelGetPtr(i);
        switch (*(s32*)(reel + 8)) {
            case 0:
                *(u32*)reel &= ~1;
                *(s32*)(reel + 0x58) = 0xFF;
                break;

            case 1:
                if (i != 2) *(u32*)reel |= 1;
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) < 6) {
                    *(f32*)(reel + 0x20) = 226.0f + 42.0f * (f32)i;
                    *(f32*)(reel + 0x24) = intplGetValue(110.0, 142.0, 0,
                                                         *(s32*)(reel + 0xC), 5);
                    *(f32*)(reel + 0x28) = 0.0f;
                    *(f32*)(reel + 0x2C) = 1.0f;
                    *(f32*)(reel + 0x30) = 1.0f;
                    *(f32*)(reel + 0x34) = 1.0f;
                    *(f32*)(reel + 0x38) = 0.0f;
                    *(f32*)(reel + 0x3C) = 0.0f;
                    *(f32*)(reel + 0x40) = 0.0f;
                    *(s32*)(reel + 0x58) = 0xFF;
                } else {
                    *(f32*)(reel + 0x20) = 226.0f + 42.0f * (f32)i;
                    *(f32*)(reel + 0x24) = intplGetValue(142.0, 174.0, 4,
                                                         *(s32*)(reel + 0xC) - 5, 20);
                    *(f32*)(reel + 0x28) = 0.0f;
                    *(s32*)(reel + 0x58) = 0xFF;
                }
                if (*(s32*)(reel + 0xC) > 24) {
                    *(s32*)(reel + 8) = 2;
                }
                break;

            case 2:
                *(f32*)(reel + 0x20) = 226.0f + 42.0f * (f32)i;
                *(f32*)(reel + 0x24) = 174.0f;
                *(f32*)(reel + 0x28) = 0.0f;
                *(f32*)(reel + 0x2C) = 1.0f;
                *(f32*)(reel + 0x30) = 1.0f;
                *(f32*)(reel + 0x34) = 1.0f;
                if (*(u32*)reel & 2) {
                    *(u32*)reel &= ~2;
                    *(s32*)(reel + 8) = 3;
                    *(s32*)(reel + 0xC) = 0;
                }
                break;

            case 3:
                *(f32*)(reel + 0x20) = 226.0f + 42.0f * (f32)i;
                *(f32*)(reel + 0x24) = intplGetValue(142.0, 174.0, 0,
                                                     *(s32*)(reel + 0xC), 20);
                *(f32*)(reel + 0x28) = 0.0f;
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) > 20) {
                    *(s32*)(reel + 8) = 0;
                    *(s32*)(reel + 0xC) = 0;
                    *(u32*)reel &= ~1;
                }
                break;

            case 4:
                *(f32*)(reel + 0x20) = 226.0f + 42.0f * (f32)i;
                *(f32*)(reel + 0x24) = intplGetValue(142.0, 174.0, 0,
                                                     *(s32*)(reel + 0xC), 20);
                *(f32*)(reel + 0x28) = 0.0f;
                *(f32*)(reel + 0x2C) = 1.0f;
                *(f32*)(reel + 0x30) = 1.0f;
                *(f32*)(reel + 0x34) = 1.0f;
                *(f32*)(reel + 0x38) = 0.0f;
                *(f32*)(reel + 0x3C) = 0.0f;
                *(f32*)(reel + 0x40) = 0.0f;
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) > 20) {
                    *(s32*)(reel + 8) = 5;
                    *(f32*)(reel + 0x20) = -128.0f + 128.0f * (f32)i;
                    *(f32*)(reel + 0x24) = 0.0f;
                    *(f32*)(reel + 0x28) = 0.0f;
                    *(s32*)(reel + 0xC) = 0;
                }
                break;

            case 5:
                *(u32*)reel |= 1;
                *(f32*)(reel + 0x20) = -128.0f + 128.0f * (f32)i;
                *(f32*)(reel + 0x24) = intplGetValue(-128.0, 0.0, 0,
                                                     *(s32*)(reel + 0xC), 40);
                *(f32*)(reel + 0x28) = 0.0f;
                *(f32*)(reel + 0x2C) = 1.0f;
                *(f32*)(reel + 0x30) = 1.0f;
                *(f32*)(reel + 0x34) = 1.0f;
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) > 40) {
                    *(s32*)(reel + 8) = (i == 2) ? 6 : 10;
                    *(s32*)(reel + 0xC) = 0;
                }
                break;

            case 7:
                *(f32*)(reel + 0x50) = intplGetValue(0.0, 22.5, 0,
                                                     *(s32*)(reel + 0xC), 20);
                *(f32*)(reel + 0x20) = -128.0f + 128.0f * (f32)i;
                *(f32*)(reel + 0x24) += *(f32*)(reel + 0x50);
                *(f32*)(reel + 0x28) = 0.0f;
                if (*(f32*)(reel + 0x24) >= 128.0f) {
                    *(f32*)(reel + 0x24) -= 128.0f;
                }
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) > 20) {
                    *(s32*)(reel + 8) = 8;
                    *(s32*)(reel + 0xC) = 0;
                }
                break;

            case 8:
                *(f32*)(reel + 0x50) = 22.5f;
                *(f32*)(reel + 0x20) = -128.0f + 128.0f * (f32)i;
                *(f32*)(reel + 0x24) += *(f32*)(reel + 0x50);
                *(f32*)(reel + 0x28) = 0.0f;
                if (*(f32*)(reel + 0x24) >= 128.0f) {
                    *(f32*)(reel + 0x24) -= 128.0f;
                }
                break;

            case 9:
                *(f32*)(reel + 0x50) += *(f32*)(reel + 0x54);
                if (*(f32*)(reel + 0x50) < 20.0f) *(f32*)(reel + 0x50) = 20.0f;
                *(f32*)(reel + 0x20) = -128.0f + 128.0f * (f32)i;
                *(f32*)(reel + 0x24) += *(f32*)(reel + 0x50);
                *(f32*)(reel + 0x28) = 0.0f;
                if (*(f32*)(reel + 0x50) >= 0.0f && *(f32*)(reel + 0x24) >= 128.0f) {
                    *(f32*)(reel + 0x24) -= 128.0f;
                    *(s32*)(reel + 8) = 10;
                    *(s32*)(reel + 0xC) = 0;
                    *(f32*)(reel + 0x54) = *(f32*)(reel + 0x50);
                }
                break;

            case 10:
                if (i == 2) {
                    *(s32*)(reel + 0xC) += 1;
                    *(f32*)(reel + 0x50) = intplGetValue(*(f32*)(reel + 0x50), 0.0, 0,
                                                         *(s32*)(reel + 0xC), 20);
                    *(f32*)(reel + 0x24) = *(f32*)(reel + 0x50) *
                        (f32)sin(3.1416 * 2.0 * (f64)*(s32*)(reel + 0xC) * 0.125);
                }
                break;

            case 11:
                *(f32*)(reel + 0x50) = 0.0f;
                *(f32*)(reel + 0x24) = 0.0f;
                *(f32*)(reel + 0x2C) = intplGetValue(1.0, 2.0, 0,
                                                     *(s32*)(reel + 0xC), 60);
                *(f32*)(reel + 0x30) = *(f32*)(reel + 0x2C);
                *(s32*)(reel + 0x58) = (s32)intplGetValue(255.0, 0.0, 0,
                                                         *(s32*)(reel + 0xC), 60);
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) > 60) {
                    *(s32*)(reel + 8) = 13;
                    *(u32*)reel &= ~1;
                }
                break;

            case 12:
                *(f32*)(reel + 0x50) = 0.0f;
                *(f32*)(reel + 0x24) = intplGetValue(0.0, 128.0, 0,
                                                     *(s32*)(reel + 0xC), 40);
                *(f32*)(reel + 0x20) = -128.0f + 128.0f * (f32)i;
                *(f32*)(reel + 0x28) = 0.0f;
                *(s32*)(reel + 0xC) += 1;
                if (*(s32*)(reel + 0xC) > 40) {
                    *(s32*)(reel + 8) = 13;
                    *(u32*)reel &= ~1;
                }
                break;

            case 13:
                *(s32*)(reel + 8) = 0;
                break;
        }
    }
}

void BattleBreakSlot_Disp(void) {
    volatile f32 priority = float_490_804268ac;
    dispEntry(8, 0, BattleBreakSlotDispReel, 0, priority);
}

void BattleBreakSlotDispReel(void) {
    typedef f32 Mtx[3][4];

    extern f64 double_to_int_802fe248;
    extern f32 float_226_80426890;
    extern f32 float_42_80426894;
    extern f32 float_122_80426898;
    extern f32 float_0_8042689c;
    extern f32 float_deg2rad_804268a0;
    extern f32 float_neg32_804268a4;
    extern f32 float_neg128_804268a8;
    extern u32 dat_80426884;
    extern u32 dat_80426888;
    extern u32 dat_8042688c;
    extern u16 tex_id_list_small[];
    extern u16 tex_id_list_big[];
    extern void* gp;

    extern void* BattleBreakSlotReelGetPtr(s32 index);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void iconDispGxCol(Mtx m, s16 flags, u16 iconId, void* color);
    extern void btlDispTexPlane2(Mtx m, u16 texId, void* color);
    extern void GXSetScissor(s32 x, s32 y, s32 w, s32 h);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 compareEnable, s32 func, s32 updateEnable);

    Mtx mtxA;
    Mtx mtxB;
    Mtx mtxC;
    Mtx mtxD;
    Mtx mtxE;
    Mtx mtxF;
    Mtx mtxG;
    Mtx mtxH;
    Mtx mtxI;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    u32 colorD;
    s32 i;
    s32 reel;
    s32 state;
    s32 nextIndex;
    u16* texList;
    u16 texId;

    i = 0;
    reel = (s32)BattleBreakSlotReelGetPtr(0);
    do {
        if ((*(u32*)(reel + 4) & 1) != 0 &&
            *(s32*)(reel + 8) >= 1 &&
            *(s32*)(reel + 8) <= 4) {
            PSMTXTrans(
                mtxA,
                (double)(float_226_80426890 + float_42_80426894 * (f32)i),
                (double)float_122_80426898,
                (double)float_0_8042689c);

            colorA = dat_80426884;
            iconDispGxCol(mtxA, 0x10, 0x1DC, &colorA);
        }

        i++;
        reel += 0x5C;
    } while (i < 3);

    reel = (s32)BattleBreakSlotReelGetPtr(0);
    state = *(s32*)(reel + 8);
    if (state >= 1 && state <= 4) {
        GXSetScissor(0, 0x52, 0x260, 0x20);
        texList = tex_id_list_small;
    } else {
        texList = tex_id_list_big;
        if (state != 0xB) {
            GXSetScissor(0, 0xB0, 0x260, 0x80);
        }
    }

    i = 0;
    reel = (s32)BattleBreakSlotReelGetPtr(0);
    do {
        if ((*(u32*)(reel + 4) & 1) != 0) {
            GXSetBlendMode(1, 4, 5, 0);
            GXSetZCompLoc(1);
            GXSetAlphaCompare(7, 0, 0, 7, 0);
            GXSetZMode(1, 3, 0);

            PSMTXTrans(mtxA,
                (double)*(f32*)(reel + 0x20),
                (double)*(f32*)(reel + 0x24),
                (double)*(f32*)(reel + 0x28));

            PSMTXScale(mtxB,
                *(f32*)(reel + 0x38),
                *(f32*)(reel + 0x3C),
                *(f32*)(reel + 0x40));

            PSMTXRotRad(mtxE, (double)(float_deg2rad_804268a0 * *(f32*)(reel + 0x2C)), 'x');
            PSMTXRotRad(mtxF, (double)(float_deg2rad_804268a0 * *(f32*)(reel + 0x30)), 'y');
            PSMTXRotRad(mtxG, (double)(float_deg2rad_804268a0 * *(f32*)(reel + 0x34)), 'z');

            PSMTXConcat(mtxE, mtxG, mtxD);
            PSMTXConcat(mtxF, mtxD, mtxD);

            PSMTXTrans(mtxC,
                (double)*(f32*)(reel + 0x44),
                (double)*(f32*)(reel + 0x48),
                (double)*(f32*)(reel + 0x4C));
            PSMTXTrans(mtxH,
                (double)-*(f32*)(reel + 0x44),
                (double)-*(f32*)(reel + 0x48),
                (double)-*(f32*)(reel + 0x4C));

            PSMTXConcat(mtxD, mtxC, mtxI);
            PSMTXConcat(mtxH, mtxI, mtxI);
            PSMTXConcat(mtxB, mtxI, mtxI);
            PSMTXConcat(mtxA, mtxI, mtxI);

            colorB = (dat_80426888 & 0xFFFFFF00) | *(u8*)(reel + 0x58);
            texId = texList[*(s32*)(reel + 0x10)];
            btlDispTexPlane2(mtxI, texId, &colorB);

            state = *(s32*)(reel + 8);
            if (state <= 10 && state != 3 && state != 4) {
                nextIndex = *(s32*)(reel + 0x1C) + 1;
                if (*(s32*)(reel + 0x18) <= nextIndex) {
                    nextIndex = 0;
                }

                texId = texList[((s32*)*(s32*)(reel + 0x14))[nextIndex]];

                if (state >= 1 && state <= 4) {
                    PSMTXTrans(mtxA,
                        (double)float_0_8042689c,
                        (double)float_neg32_804268a4,
                        (double)float_0_8042689c);
                } else {
                    PSMTXTrans(mtxA,
                        (double)float_0_8042689c,
                        (double)float_neg128_804268a8,
                        (double)float_0_8042689c);
                }

                PSMTXConcat(mtxA, mtxI, mtxI);

                colorC = (dat_8042688c & 0xFFFFFF00) | *(u8*)(reel + 0x58);
                btlDispTexPlane2(mtxI, texId, &colorC);
            }
        }

        i++;
        reel += 0x5C;
    } while (i < 3);

    GXSetScissor(
        0,
        0,
        *(u16*)((s32)gp + 0x170),
        *(u16*)((s32)gp + 0x172));
}

void BattleBreakSlot_End(void) {
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(0) + 0x14));
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(1) + 0x14));
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(2) + 0x14));
}


/* CHATGPT STUB FILL: main/battle/battle_break_slot 20260624_184008 */

/* stub-fill: BattleBreakSlot_PointInc | missing_definition | ghidra_signature */
void BattleBreakSlot_PointInc(void) {
    extern void* pouchGetPtr(void);
    extern s32 irand(s32 range);
    void* battle = _battleWorkPointer;
    void* work = BattleBreakSlotGetPtr();
    void* reel;
    void* reel1;
    s32 index;

    if (*(u16*)((s32)pouchGetPtr() + 0x8C) == 0) {
        return;
    }

    reel = BattleBreakSlotReelGetPtr(*(s32*)((s32)work + 0xC));
    *(s32*)((s32)reel + 8) = 1;
    *(s32*)((s32)reel + 0xC) = 0;
    *(s32*)((s32)work + 0xC) = *(s32*)((s32)work + 0xC) + 1;

    if (*(s32*)((s32)work + 0xC) > 3) {
        *(s32*)((s32)work + 0xC) = 3;
        return;
    }

    index = *(s32*)reel;
    switch (index) {
        case 0:
        case 1:
            index = irand(*(s32*)((s32)reel + 0x18));
            *(s32*)((s32)reel + 0x1C) = index - 1;
            if (*(s32*)((s32)reel + 0x1C) == -1) {
                *(s32*)((s32)reel + 0x1C) = *(s32*)((s32)reel + 0x18) - 1;
            }
            *(s32*)((s32)reel + 0x10) =
                ((s32*)*(void**)((s32)reel + 0x14))[*(s32*)((s32)reel + 0x1C)];
            if ((*(u32*)((s32)battle + 0x19078) & 0x80000) != 0) {
                item_appear_force(reel, *(s32*)((s32)battle + 0x1908C));
            }
            if ((*(u32*)work & 1) != 0) {
                item_appear_force(reel, 3);
            }
            if ((*(u32*)work & 4) != 0) {
                item_appear_force(reel, 3);
            }
            break;
        case 2:
            do {
                reel1 = BattleBreakSlotReelGetPtr(1);
                index = irand(*(s32*)((s32)reel + 0x18));
                *(s32*)((s32)reel + 0x1C) = index;
                *(s32*)((s32)reel + 0x10) =
                    ((s32*)*(void**)((s32)reel + 0x14))[*(s32*)((s32)reel + 0x1C)];
            } while (*(s32*)((s32)reel + 0x10) == *(s32*)((s32)reel1 + 0x10));
            break;
    }
}

void BattleBreakSlot_Start(void) {
    void* work = BattleBreakSlotGetPtr();
    void* reel;
    s32 i;

    if (*(s32*)((s32)work + 0xC) >= 3) {
        *(s32*)((s32)work + 0xC) = 0;
        *(s32*)((s32)work + 4) = 2;
        reel = BattleBreakSlotReelGetPtr(0);
        for (i = 0; i < 3; i++) {
            *(s32*)((s32)reel + 8) = 4;
            *(s32*)((s32)reel + 0xC) = 0;
            reel = (void*)((s32)reel + 0x5C);
        }
    }
}

s32 BattleBreakSlot_CheckReaction(void) {
    void* work = BattleBreakSlotGetPtr();
    void* event;

    BattleBreakSlot_Start();
    if (*(s32*)((s32)work + 4) >= 1 && *(s32*)((s32)work + 4) <= 8) {
        return 1;
    }

    event = *(void**)((s32)work + 0x130);
    if (event != NULL && evtCheckID(*(s32*)((s32)event + 0x15C)) != 0) {
        return 1;
    }

    btl_camera_set_mode(0, 0);
    btl_camera_set_moveSpeedLv(0, 2);
    BattleAudienceNumToTarget();
    *(s32*)((s32)work + 0x130) = 0;
    return 0;
}

s32 BattleBreakSlot_GetBreakTurn(void) {
    return *(s32*)((s32)BattleBreakSlotGetPtr() + 0x10);
}

void BattleBreakSlot_DecBreakTurn(void) {
    void* work = BattleBreakSlotGetPtr();

    *(s32*)((s32)work + 0x10) = *(s32*)((s32)work + 0x10) - 1;
    if (*(s32*)((s32)work + 0x10) < 0) {
        *(s32*)((s32)work + 0x10) = 0;
    }
}

void BattleBreakSlot_HideReel(void) {
    void* work;
    void* reel;
    s32 i;

    work = BattleBreakSlotGetPtr();
    reel = BattleBreakSlotReelGetPtr(0);
    for (i = 0; i < 3; i++) {
        *(u32*)((s32)reel + 4) |= 2;
        reel = (void*)((s32)reel + 0x5C);
    }
    *(s32*)((s32)work + 0xC) = 0;
}

void item_appear_force(void* reel, s32 item) {
    s32 offset;
    s32 i;

    i = 0;
    offset = 0;
    while (i < *(s32*)((s32)reel + 0x18)) {
        if (item == *(s32*)(*(s32*)((s32)reel + 0x14) + offset)) {
            *(s32*)((s32)reel + 0x1C) = i - 1;
        }
        if (*(s32*)((s32)reel + 0x1C) == -1) {
            *(s32*)((s32)reel + 0x1C) = *(s32*)((s32)reel + 0x18) - 1;
        }
        i++;
        offset += 4;
    }
}
