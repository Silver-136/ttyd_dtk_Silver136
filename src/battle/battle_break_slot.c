#include "battle/battle_break_slot.h"

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

void BattleBreakSlot_Disp(void) {
    f32 priority = float_490_804268ac;

    dispEntry(8, 0, BattleBreakSlotDispReel, 0, priority);
}

void* BattleBreakSlotReelGetPtr(s32 index) {
    s32 offset = index * 0x5C;
    void* work = BattleBreakSlotGetPtr();

    return (void*)((s32)work + offset + 0x18);
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

void BattleBreakSlot_End(void) {
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(0) + 0x14));
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(1) + 0x14));
    BattleFree(*(void**)((s32)BattleBreakSlotReelGetPtr(2) + 0x14));
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


/* CHATGPT STUB FILL: main/battle/battle_break_slot 20260624_184008 */

/* stub-fill: BattleBreakSlot_PointInc | missing_definition | ghidra_signature */
u8 BattleBreakSlot_PointInc(void) {
    return 0;
}


/* CHATGPT STUB FILL: main/battle/battle_break_slot 20260624_185954 */

/* stub-fill: BattleBreakSlot_Main | missing_definition | header_prototype */
void BattleBreakSlot_Main(void) {
    return;
}

/* stub-fill: BattleBreakSlot_Init | missing_definition | header_prototype */
void BattleBreakSlot_Init(void) {
    return;
}
