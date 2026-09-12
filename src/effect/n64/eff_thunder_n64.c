#include "effect/n64/eff_thunder_n64.h"
#include "effect/n64/eff_akari_charge_n64.h"
#include "effect/n64/eff_shock_n64.h"

extern f32 float_0_804263c0;
extern f32 float_1_804263c4;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effThunderN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effThunderMain(void*);
    extern void* effThunderflareN64Entry(s32 type, s32 unk, f32 x, f32 y, f32 z, f32 scale);
    extern char str_ThunderN64_802fc1a8[];
    extern void* gp;
    extern f32 float_48_804263d8;
    extern f32 float_32_804263dc;
    extern f32 float_8_804263e0;
    extern f32 float_neg118_804263e4;
    extern f32 float_184_804263e8;
    extern f32 float_79_804263ec;
    extern f32 float_neg120_804263f0;
    extern f32 float_189_804263f4;
    extern f32 float_72_804263f8;
    void* entry;
    u8* work;
    s32 shockType;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ThunderN64_802fc1a8;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x4C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effThunderMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x18) = 0;
    if (type >= 3) {
        *(s32*)(work + 0x14) = 0x1E;
    } else {
        *(s32*)(work + 0x14) = 0x64;
    }
    *(s32*)(work + 0x10) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x20) = float_0_804263c0;
    *(f32*)(work + 0x1C) = float_0_804263c0;
    *(f32*)(work + 0x2C) = float_48_804263d8;
    *(f32*)(work + 0x30) = float_32_804263dc;
    *(f32*)(work + 0x28) = float_0_804263c0;
    *(f32*)(work + 0x24) = float_0_804263c0;
    *(f32*)(work + 0x34) = float_8_804263e0;
    *(f32*)(work + 0x38) = float_32_804263dc;
    *(s32*)(work + 0x3C) = 4;
    *(s32*)(work + 0x40) = 2;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x44) = 0;
    if (type == 1 || type == 2) {
        if (type == 1) {
            *(void**)(work + 0x44) = effThunderflareN64Entry(0, -1, float_neg118_804263e4, float_184_804263e8, float_79_804263ec, float_1_804263c4);
        }
        if (type != 1) {
            *(void**)(work + 0x48) = effAkariChargeN64Entry(0, 0, float_neg120_804263f0, float_189_804263f4, float_72_804263f8, float_1_804263c4);
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x28) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x2C) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x30) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x18) = 0xFF;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x1C) = 0;
            *(s32*)((s32)*(void**)((s32)*(void**)(work + 0x48) + 0xC) + 0x20) = 0xFF;
        }
    }
    if (type >= 3) {
        shockType = 1;
        if (*(s32*)((s32)gp + 0x14) == 1) {
            shockType = 3;
        }
        effShockN64Entry(shockType, x, y, z, sx, sy);
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset


void effThunderMain(void* effect) {
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effThunderDisp(void);
    extern f32 float_256_804263d0;
    extern f32 float_64_804263d4;
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 position[3];
    s32 type;
    s32 timer;
    s32 frame;

    position[0] = *(f32*)(work + 4);
    position[1] = *(f32*)(work + 8);
    position[2] = *(f32*)(work + 0xC);
    if (*(u32*)effect & 4) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x14) = 0;
    }
    type = *(s32*)work;
    if (*(s32*)(work + 0x14) < 100) {
        *(s32*)(work + 0x14) -= 1;
    }
    *(s32*)(work + 0x18) += 1;
    timer = *(s32*)(work + 0x14);
    if (timer < 0) {
        if (*(void**)(work + 0x48) != 0) {
            effDelete(*(void**)(work + 0x48));
            *(void**)(work + 0x48) = 0;
        }
        if (*(void**)(work + 0x44) != 0) {
            effDelete(*(void**)(work + 0x44));
            *(void**)(work + 0x44) = 0;
        }
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x18);
    if (*(void**)(work + 0x44) != 0 && frame > 0x11) {
        effDelete(*(void**)(work + 0x44));
        *(void**)(work + 0x44) = 0;
    }

    if (type < 3 || frame < 7) {
        *(f32*)(work + 0x1C) += *(f32*)(work + 0x2C);
        if (*(f32*)(work + 0x1C) >= float_0_804263c0) {
            if (*(f32*)(work + 0x1C) > float_256_804263d0) {
                *(f32*)(work + 0x1C) -= float_256_804263d0;
            }
        } else {
            *(f32*)(work + 0x1C) += float_256_804263d0;
        }

        if (frame % *(s32*)(work + 0x3C) == 0) {
            *(f32*)(work + 0x20) += *(f32*)(work + 0x30);
            if (*(f32*)(work + 0x20) >= float_0_804263c0) {
                if (*(f32*)(work + 0x20) > float_64_804263d4) {
                    *(f32*)(work + 0x20) -= float_64_804263d4;
                }
            } else {
                *(f32*)(work + 0x20) += float_64_804263d4;
            }
        }
        if (frame % *(s32*)(work + 0x40) == 0) {
            *(f32*)(work + 0x24) += *(f32*)(work + 0x34);
            if (*(f32*)(work + 0x24) >= float_0_804263c0) {
                if (*(f32*)(work + 0x24) > float_256_804263d0) {
                    *(f32*)(work + 0x24) -= float_256_804263d0;
                }
            } else {
                *(f32*)(work + 0x24) += float_256_804263d0;
            }
        }
        *(f32*)(work + 0x28) += *(f32*)(work + 0x38);
        if (*(f32*)(work + 0x28) >= float_0_804263c0) {
            if (*(f32*)(work + 0x28) > float_64_804263d4) {
                *(f32*)(work + 0x28) -= float_64_804263d4;
            }
        } else {
            *(f32*)(work + 0x28) += float_64_804263d4;
        }
    }

    if (type < 3) {
        if (timer < 10) {
            *(s32*)(work + 0x10) -= 0x19;
            if (*(s32*)(work + 0x10) < 0) {
                *(s32*)(work + 0x10) = 0;
            }
        }
    } else if (frame > 6) {
        *(s32*)(work + 0x10) -= 0xD;
        if (*(s32*)(work + 0x10) < 0) {
            *(s32*)(work + 0x10) = 0;
        }
    }
    dispEntry(4, 2, effThunderDisp, effect, dispCalcZ(position));
}


void effThunderDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2();
    extern f32 float_4_804263b8;
    extern f32 float_deg2rad_804263bc;
    extern f32 float_0p0078125_804263c8;
    extern f32 float_0p03125_804263cc;
    extern u32 dat_804263a8;
    extern u32 dat_804263ac;
    extern u32 dat_804263b0;
    extern u32 dat_804263b4;
    u8* work = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    Mtx trans, rot, model, scaleMtx;
    u8 texObj[0x20];
    u32 color;
    s32 type = *(s32*)work;
    f32 scroll0 = float_4_804263b8 * *(f32*)(work + 0x1C);
    f32 scroll1 = float_4_804263b8 * *(f32*)(work + 0x20);
    f32 scroll2 = float_4_804263b8 * *(f32*)(work + 0x24);
    f32 scroll3 = float_4_804263b8 * *(f32*)(work + 0x28);

    if (type == 1 && *(s32*)(work + 0x18) <= 0x11) {
        return;
    }
    if (type < 3) {
        color = dat_804263a8;
        GXSetTevColor(2, &color);
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
        GXSetNumTevStages(2);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 8, 12, 2, 0);
        GXSetTevAlphaIn(0, 0, 0, 0, 4);
        GXSetTevOrder(1, 1, 1, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 0, 0, 0, 15);
        GXSetTevAlphaIn(1, 0, 4, 4, 0);
    } else {
        color = dat_804263ac;
        GXSetTevColor(2, &color);
        GXSetNumChans(0);
        GXSetNumTevStages(4);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 3);
        GXSetTevColorIn(0, 0, 8, 8, 0);
        GXSetTevAlphaIn(0, 0, 0, 0, 4);
        GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 15, 12, 2, 0);
        GXSetTevAlphaIn(1, 6, 0, 5, 7);
        GXSetTevOrder(2, 1, 1, 0xFF);
        GXSetTevColorOp(2, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
        GXSetTevColorIn(2, 0, 0, 0, 15);
        GXSetTevAlphaIn(2, 0, 4, 6, 0);
        GXSetTevOrder(3, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(3, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
        GXSetTevColorIn(3, 0, 0, 0, 15);
        GXSetTevAlphaIn(3, 0, 7, 1, 7);
    }

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_804263bc * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(trans, rot, model);
    PSMTXTrans(trans, float_0_804263c0, float_0_804263c0, float_1_804263c4);
    PSMTXConcat(model, trans, model);
    PSMTXConcat((void*)((s32)camera + 0x11C), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    color = type < 3 ? dat_804263b0 : ((dat_804263b4 & 0xFFFFFF00) | (*(s32*)(work + 0x10) & 0xFF));
    GXSetTevColor(1, &color);
    effGetTexObjN64(0x54, texObj);
    GXLoadTexObj(texObj, 0);
    GXLoadTexObj(texObj, 1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p0078125_804263c8, float_0p03125_804263cc, float_0_804263c0);
    PSMTXTrans(trans, scroll0, scroll1, float_0_804263c0);
    PSMTXConcat(scaleMtx, trans, trans);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    PSMTXScale(scaleMtx, float_0p0078125_804263c8, float_0p03125_804263cc, float_0_804263c0);
    PSMTXTrans(trans, scroll2, scroll3, float_0_804263c0);
    PSMTXConcat(scaleMtx, trans, trans);
    GXLoadTexMtxImm(trans, 0x21, 1);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A9680);

    if (type == 3) {
        GXBegin(0x90, 0, 0x30);
        tri2(0x1BC,0x1BD,0x1BE,0,0x1BC,0x1BE,0x1BF);
        tri2(0x1C0,0x1C1,0x1C2,0,0x1C0,0x1C2,0x1C3);
        tri2(0x1C4,0x1C5,0x1C6,0,0x1C4,0x1C6,0x1C7);
        tri2(0x1C8,0x1C9,0x1CA,0,0x1C8,0x1CA,0x1CB);
        tri2(0x1CC,0x1CD,0x1CE,0,0x1CC,0x1CE,0x1CF);
        tri2(0x1D0,0x1D1,0x1D2,0,0x1D0,0x1D2,0x1D3);
        tri2(0x1D4,0x1D5,0x1D6,0,0x1D4,0x1D6,0x1D7);
        tri2(0x1D8,0x1D9,0x1DA,0,0x1BE,0x1DA,0x1DB);
    } else if (type == 5) {
        GXBegin(0x90, 0, 0x36);
        tri2(0x204,0x205,0x206,0,0x204,0x206,0x207);
        tri2(0x208,0x209,0x20A,0,0x208,0x20A,0x20B);
        tri2(0x20C,0x20D,0x20E,0,0x20C,0x20E,0x20F);
        tri2(0x210,0x211,0x212,0,0x210,0x212,0x213);
        tri2(0x214,0x215,0x216,0,0x214,0x216,0x217);
        tri2(0x218,0x219,0x21A,0,0x218,0x21A,0x21B);
        tri2(0x21C,0x21D,0x21E,0,0x21C,0x21E,0x21F);
        tri2(0x220,0x221,0x222,0,0x220,0x222,0x223);
        tri2(0x224,0x225,0x226,0,0x224,0x226,0x227);
    } else if (type == 4) {
        GXBegin(0x90, 0, 0x3C);
        tri2(0x1DC,0x1DD,0x1DE,0,0x1DC,0x1DE,0x1DF);
        tri2(0x1E0,0x1E1,0x1E2,0,0x1E0,0x1E2,0x1E3);
        tri2(0x1E4,0x1E5,0x1E6,0,0x1E4,0x1E6,0x1E7);
        tri2(0x1E8,0x1E9,0x1EA,0,0x1E8,0x1EA,0x1EB);
        tri2(0x1EC,0x1ED,0x1EE,0,0x1EC,0x1EE,0x1EF);
        tri2(0x1F0,0x1F1,0x1F2,0,0x1F0,0x1F2,0x1F3);
        tri2(0x1F4,0x1F5,0x1F6,0,0x1F4,0x1F6,0x1F7);
        tri2(0x1F8,0x1F9,0x1FA,0,0x1F8,0x1FA,0x1FB);
        tri2(0x1FC,0x1FD,0x1FE,0,0x1FC,0x1FE,0x1FF);
        tri2(0x200,0x201,0x202,0,0x200,0x202,0x203);
    } else if (type == 0) {
        GXBegin(0x90, 0, 0x9C);
        tri2(0,1,2,0,0,2,3,0);
        tri2(4,5,5,0,4,5,4,0);
        tri2(6,7,8,0,6,8,9,0);
        tri2(10,11,12,0,10,12,13,0);
        tri2(14,15,16,0,14,16,17,0);
        tri2(18,19,20,0,18,20,21,0);
        tri2(22,23,24,0,22,24,25,0);
        tri2(26,27,28,0,26,28,29,0);
        tri2(30,35,36,0,30,36,37,0);
        tri2(31,38,39,0,31,39,40,0);
        tri2(32,41,42,0,32,42,43,0);
        tri2(33,44,45,0,33,45,46,0);
        tri2(34,47,48,0,34,48,49,0);
        tri2(50,51,52,0,50,52,53,0);
        tri2(54,55,56,0,54,56,57,0);
        tri2(58,59,60,0,58,60,61,0);
        tri2(62,63,64,0,62,64,65,0);
        tri2(66,67,68,0,66,68,69,0);
        tri2(70,71,72,0,70,72,73,0);
        tri2(74,75,76,0,74,76,77,0);
        tri2(78,79,80,0,78,80,81,0);
        tri2(82,87,88,0,82,88,89,0);
        tri2(83,90,91,0,83,91,92,0);
        tri2(84,93,93,0,84,93,84,0);
        tri2(85,94,95,0,85,95,96,0);
        tri2(86,97,98,0,86,98,99,0);
    } else if (type == 1) {
        GXBegin(0x90, 0, 0x120);
        tri2(100,101,102,0,100,102,103,0);
        tri2(104,105,106,0,104,106,107,0);
        tri2(108,109,110,0,108,110,111,0);
        tri2(112,113,114,0,112,114,115,0);
        tri2(116,117,118,0,116,118,119,0);
        tri2(120,121,122,0,120,122,123,0);
        tri2(124,125,126,0,124,126,127,0);
        tri2(128,129,130,0,128,130,131,0);
        tri2(132,172,173,0,132,173,174,0);
        tri2(133,175,176,0,133,176,177,0);
        tri2(134,178,179,0,134,179,180,0);
        tri2(135,181,182,0,135,182,183,0);
        tri2(136,184,185,0,136,185,186,0);
        tri2(137,187,188,0,137,188,189,0);
        tri2(138,190,191,0,138,191,192,0);
        tri2(139,193,194,0,139,194,195,0);
        tri2(140,194,195,0,140,195,196,0);
        tri2(141,197,198,0,141,198,199,0);
        tri2(142,200,201,0,142,201,202,0);
        tri2(143,203,204,0,143,204,205,0);
        tri2(144,206,207,0,144,207,208,0);
        tri2(145,209,210,0,145,210,211,0);
        tri2(146,212,213,0,146,213,214,0);
        tri2(147,215,216,0,147,216,217,0);
        tri2(148,220,221,0,148,221,222,0);
        tri2(149,223,224,0,149,224,225,0);
        tri2(150,226,227,0,150,227,228,0);
        tri2(151,229,230,0,151,230,231,0);
        tri2(152,232,233,0,152,233,234,0);
        tri2(153,235,236,0,153,236,237,0);
        tri2(154,238,239,0,154,239,240,0);
        tri2(155,241,242,0,155,242,243,0);
        tri2(156,244,245,0,156,245,246,0);
        tri2(157,247,248,0,157,248,249,0);
        tri2(158,250,251,0,158,251,252,0);
        tri2(159,253,254,0,159,254,255,0);
        tri2(160,256,257,0,160,257,258,0);
        tri2(161,259,260,0,161,260,261,0);
        tri2(162,262,263,0,162,263,264,0);
        tri2(163,265,266,0,163,266,267,0);
        tri2(164,268,269,0,164,269,270,0);
        tri2(165,271,272,0,165,272,273,0);
        tri2(166,274,275,0,166,275,276,0);
        tri2(167,277,278,0,167,278,279,0);
        tri2(168,280,281,0,168,281,282,0);
        tri2(169,283,284,0,169,284,285,0);
        tri2(170,286,287,0,170,287,288,0);
        tri2(171,289,290,0,171,290,291,0);
    } else {
        GXBegin(0x90, 0, 0xE4);
        tri2(0x124,0x125,0x126,0,0x124,0x126,0x127);
        tri2(0x128,0x129,0x12A,0,0x128,0x12A,0x12B);
        tri2(0x12C,0x12D,0x12E,0,0x12C,0x12E,0x12F);
        tri2(0x130,0x131,0x132,0,0x130,0x132,0x133);
        tri2(0x134,0x135,0x136,0,0x134,0x136,0x137);
        tri2(0x138,0x139,0x13A,0,0x138,0x13A,0x13B);
        tri2(0x13C,0x13D,0x13E,0,0x13C,0x13E,0x13F);
        tri2(0x140,0x141,0x142,0,0x140,0x142,0x143);
        tri2(0x144,0x14F,0x150,0,0x144,0x150,0x151);
        tri2(0x145,0x152,0x153,0,0x145,0x153,0x154);
        tri2(0x146,0x155,0x156,0,0x146,0x156,0x157);
        tri2(0x147,0x158,0x159,0,0x147,0x159,0x15A);
        tri2(0x148,0x15B,0x15C,0,0x148,0x15C,0x15D);
        tri2(0x149,0x15E,0x15F,0,0x149,0x15F,0x160);
        tri2(0x14A,0x161,0x162,0,0x14A,0x162,0x163);
        tri2(0x14B,0x164,0x165,0,0x14B,0x165,0x166);
        tri2(0x14C,0x167,0x168,0,0x14C,0x168,0x169);
        tri2(0x14D,0x16A,0x16B,0,0x14D,0x16B,0x16C);
        tri2(0x14E,0x16D,0x16E,0,0x14E,0x16E,0x16F);
        tri2(0x170,0x171,0x172,0,0x170,0x172,0x173);
        tri2(0x174,0x175,0x176,0,0x174,0x176,0x177);
        tri2(0x178,0x179,0x17A,0,0x178,0x17A,0x17B);
        tri2(0x17C,0x17D,0x17E,0,0x17C,0x17E,0x17F);
        tri2(0x180,0x181,0x182,0,0x180,0x182,0x183);
        tri2(0x184,0x185,0x186,0,0x184,0x186,0x187);
        tri2(0x188,0x189,0x18A,0,0x188,0x18A,0x18B);
        tri2(0x18C,0x18D,0x18E,0,0x18C,0x18E,0x18F);
        tri2(0x190,0x19B,0x19C,0,0x190,0x19C,0x19D);
        tri2(0x191,0x19E,0x19F,0,0x191,0x19F,0x1A0);
        tri2(0x192,0x1A1,0x1A2,0,0x192,0x1A2,0x1A3);
        tri2(0x193,0x1A4,0x1A5,0,0x193,0x1A5,0x1A6);
        tri2(0x194,0x1A7,0x1A8,0,0x194,0x1A8,0x1A9);
        tri2(0x195,0x1AA,0x1AB,0,0x195,0x1AB,0x1AC);
        tri2(0x196,0x1AD,0x1AE,0,0x196,0x1AE,0x1AF);
        tri2(0x197,0x1B0,0x1B1,0,0x197,0x1B1,0x1B2);
        tri2(0x198,0x1B3,0x1B4,0,0x198,0x1B4,0x1B5);
        tri2(0x199,0x1B6,0x1B7,0,0x199,0x1B7,0x1B8);
        tri2(0x19A,0x1B9,0x1BA,0,0x19A,0x1BA,0x1BB);
    }
}

const f32 vec3_802fc190[3] = { 0.0f, 0.0f, 0.0f };
const char str_ThunderN64_802fc1a8[] = "ThunderN64";
