#include "action/star/sac_bakugame.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
extern char str_c_star_bom_802ff6c0[];
const char str_Z_1_80427c20[] = "Z_1";

void* memset(void* dst, int value, u32 size);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
s32 BtlUnit_CheckStatus(void* unit, s32 status);
s32 BtlUnit_CanActStatus(void* unit);
s32 BtlUnit_GetBodyPartsId(void* unit);
void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
void btlDispPoseAnime(void* part);
s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
s32 animPoseEntry(void* name, s32 mode);
void animPoseSetAnim(s32 poseId, const void* name, s32 force);
void BtlUnit_SetBodyAnimType(void* unit, s32 type);
void effDelete(void* effect);

typedef struct BakuGameWork {
    u8 pad[0xEC];
    s32 poseId;
    u8 padF0[0x204 - 0xF0];
    s32 starAppear;
} BakuGameWork;

void bakuGameDispStar(void) {
    ;
}

s32 bakuGameAudienceCanThrowPos(s32 pos) {
    s32 i;
    s32 base = 0;
    for (i = 0; i < 3; i++) {
        if (pos >= base + 8 && pos <= base + 11) {
            return 1;
        }
        base += 0x14;
    }
    return 0;
}

BakuGameWork* GetBakuGamePtr(void) {
    return (BakuGameWork*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(star_stone_appear) {
    BakuGameWork* wp;

    wp = GetBakuGamePtr();
    wp->starAppear = 1;
    return 2;
}

USER_FUNC(init_bakugame) {
    memset(GetBakuGamePtr(), 0, 0x2B8);
    return 2;
}

s32 bakuGamePartyExist(void* unit) {
    if (unit == 0) {
        return 0;
    }
    if (BtlUnit_CheckStatus(unit, 0x1B) != 0) {
        return 0;
    }
    return BtlUnit_CanActStatus(unit) != 0;
}

void bakuGameMarioSurpriseReset(void* mario, void* party) {
    btlDispPoseAnime(BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario)));
    if (bakuGamePartyExist(party) != 0) {
        btlDispPoseAnime(BtlUnit_GetPartsPtr(party, BtlUnit_GetBodyPartsId(party)));
    }
}

USER_FUNC(bakuGameBombEntry) {
    BakuGameWork* work = GetBakuGamePtr();
    if (animGroupBaseAsync(str_c_star_bom_802ff6c0, 2, 0) == 0) {
        return 0;
    }
    work->poseId = animPoseEntry(str_c_star_bom_802ff6c0, 2);
    animPoseSetAnim(work->poseId, str_Z_1_80427c20, 1);
    return 2;
}

void bakuGameEnemySurprise(void) {
    s32 i;
    void* battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1 && (*(u32*)((s32)unit + 0x104) & 0x20000) == 0
            && BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            BtlUnit_SetBodyAnimType(unit, 0x27);
        }
    }
}

USER_FUNC(end_bakugame) {
    BakuGameWork* work = GetBakuGamePtr();

    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x254) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x254));
    return 2;
}

void bakuGameAudienceSurprise(void) {
    extern void* BattleAudienceGetPtr(s32 id);
    extern s32 BattleAudience_GetWaiting(s32 id);
    extern void BattleAudience_SetAnim(s32 id, s32 anim, s32 pose);
    extern s32 bakuGameAudienceCanThrowPos(s32 pos);
    extern s32 irand(s32 max);
    extern void* memset(void* dst, int value, u32 size);
    extern f32 float_90_80427b7c;
    extern f32 float_270_80427b80;

    s32 i;
    void* audience;

    for (i = 0; i < 0xC8; i++) {
        audience = BattleAudienceGetPtr(i);
        if ((u8)BattleAudience_GetWaiting(i) != 0) {
            *(u32*)audience |= 0x10;
            memset((void*)((s32)audience + 0xF8), 0, 0x34);
            if (bakuGameAudienceCanThrowPos(i) == 0) {
                BattleAudience_SetAnim(i, 7, 0);
                if (irand(2) == 0) {
                    *(f32*)((s32)audience + 0xE8) = float_90_80427b7c;
                } else {
                    *(f32*)((s32)audience + 0xE8) = float_270_80427b80;
                }
            } else {
                BattleAudience_SetAnim(i, 3, 0);
            }
        }
    }
}

void bakuGameAudienceSurpriseReset(void) {
    extern void* BattleAudienceGetPtr(s32 id);
    extern s32 BattleAudience_GetExist(s32 id);
    extern s32 bakuGameAudienceCanThrowPos(s32 pos);
    extern f32 float_90_80427b7c;

    s32 i;
    void* audience;
    f32 angle;

    i = 0;
    angle = float_90_80427b7c;
    while (i < 0xC8) {
        if ((u8)BattleAudience_GetExist(i) != 0) {
            audience = BattleAudienceGetPtr(i);
            *(f32*)((s32)audience + 0xE8) = angle;
            if (bakuGameAudienceCanThrowPos(i) == 0) {
                *(u32*)audience &= ~0x10;
            }
        }
        i++;
    }
}

void bakuGameEnemySurpriseReset(void) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void btlDispPoseAnime(void* part);

    s32 i;
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1 && (*(u32*)((s32)unit + 0x104) & 0x20000) == 0
            && BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            btlDispPoseAnime(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)));
        }
    }
}

void bakuGameMarioSurprise(void* mario, void* party) {
    extern void BtlUnit_SetBodyAnim(void* unit, const char* name);
    extern s32 bakuGamePartyExist(void* unit);
    extern const char str_btl_wn_sac_bakugame_802ff3a0[];
    extern const char str_M_N_7_80427b84[6];

    const char* base;

    base = str_btl_wn_sac_bakugame_802ff3a0;
    BtlUnit_SetBodyAnim(mario, str_M_N_7_80427b84);
    if (bakuGamePartyExist(party) != 0) {
        switch (*(s32*)((s32)party + 4) - 0xE0) {
            case 0:
                BtlUnit_SetBodyAnim(party, base + 0x24C);
                break;
            case 1:
                BtlUnit_SetBodyAnim(party, base + 0x254);
                break;
            case 2:
                BtlUnit_SetBodyAnim(party, base + 0x25C);
                break;
            case 3:
                BtlUnit_SetBodyAnim(party, base + 0x264);
                break;
            case 4:
                BtlUnit_SetBodyAnim(party, base + 0x26C);
                break;
            case 5:
                BtlUnit_SetBodyAnim(party, base + 0x274);
                break;
            case 6:
                BtlUnit_SetBodyAnim(party, base + 0x27C);
                break;
        }
    }
}

void bakuGameDecideButton(s32 flag) {
    typedef struct ButtonTable {
        s32 values[9];
    } ButtonTable;
    typedef struct PatternTable {
        s32 values[18];
    } PatternTable;
    extern void* GetBakuGamePtr(void);
    extern const ButtonTable dat_802ff504;
    extern const PatternTable dat_802ff528;
    extern s32 irand(s32 max);

    void* work;
    ButtonTable buttons;
    PatternTable patterns;
    s32 row;
    s32* rowBase;
    s32* buttonsBase;
    s32 workOffset;
    s32 rowOffset;
    s32 i;
    s32 entryOffset;
    s32 buttonOffset;
    s32* src;
    s32* dst;

    work = GetBakuGamePtr();
    buttons = dat_802ff504;
    patterns = dat_802ff528;
    row = irand(6) * 0xC;
    rowBase = (s32*)((s32)&patterns.values[0] + row);
    buttonsBase = buttons.values;
    workOffset = 0;
    rowOffset = 0;
    for (i = 0; i < 3; i++) {
        entryOffset = workOffset + 0xC;
        dst = (s32*)((s32)work + entryOffset);
        workOffset += 0x24;
        *(s32*)((s32)dst + 0xC) = *(s32*)((s32)rowBase + rowOffset);
        buttonOffset = *(s32*)((s32)rowBase + rowOffset) * 0xC;
        rowOffset += 4;
        src = (s32*)((s32)buttonsBase + buttonOffset);
        *(s32*)((s32)dst + 0x14) = src[0];
        *(s32*)((s32)dst + 0x18) = src[1];
        *(s32*)((s32)dst + 0x1C) = src[2];
    }
    if (flag == 0) {
        *(s32*)((s32)work + 0xF4) = 0x3C;
    }
}

USER_FUNC(bakuGameDecideWeapon) {
    extern void* GetBakuGamePtr(void);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern u8 weapon[0xC0];
    extern f32 float_100_80427b78;

    s32* args;
    u8* weaponWork;
    s32 value;
    void* work;

    args = event->args;
    evtGetFloat(event, args[0]);
    value = evtGetValue(event, args[1]);
    work = GetBakuGamePtr();
    weaponWork = (u8*)((s32)work + 0x178);
    memcpy((void*)((s32)work + 0xF8), weapon, 0xC0);
    weaponWork[2] = 0x64;
    if (value != -1) {
        if (*(f32*)((s32)work + 0xD0) >= float_100_80427b78) {
            weaponWork[2] = 0x7F;
        } else {
            weaponWork[2] = 0x64;
        }
        weaponWork[3] = value - 1;
    } else {
        weaponWork[2] = 0;
        weaponWork[3] = 0;
    }
    evtSetValue(event, args[2], (s32)work + 0xF8);
    return 2;
}

void bakuGameDisp3D(void) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* c);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 rot, f32 scale);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 texgen, s32 type, s32 src, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void* BattleAudienceBaseGetPtr(void);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void GXLoadTexObj(void* texObj, s32 mapid);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
    extern u32 dat_80427b58;
    extern u32 dat_802ff570[16];
    extern f32 float_neg1_80427b5c;
    extern f32 float_1_80427b60;
    extern f32 float_180_80427b64;
    extern f32 float_neg20_80427b68;
    extern f32 float_40_80427b6c;
    extern f32 float_0_80427b70;
    extern f32 float_20_80427b74;

    u8* work;
    void* cam;
    Mtx modelMtx;
    Mtx transMtx;
    Mtx scaleMtx;
    u32 color;
    u32 texObj[8];
    u32 uvWords[16];
    void* audience;
    void* tpl;
    s32 i;
    s32 base;
    volatile f32* fifo;
    f32* uv;

    work = (u8*)GetBakuGamePtr();
    cam = camGetPtr(4);

    PSMTXScale(scaleMtx, float_neg1_80427b5c, float_1_80427b60, float_1_80427b60);
    PSMTXTrans(transMtx, *(f32*)(work + 0x88), *(f32*)(work + 0x8C), *(f32*)(work + 0x90));
    PSMTXConcat(transMtx, scaleMtx, modelMtx);

    if (*(s32*)(work + 0xEC) != -1) {
        animPoseDrawMtx(*(s32*)(work + 0xEC), modelMtx, 2, float_180_80427b64, *(f32*)(work + 0xC4));
    }

    if ((*(u32*)work & 1) != 0) {
        GXSetCullMode(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xD, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 1, 0, 0, 2, 2);
        color = dat_80427b58;
        GXSetChanMatColor(4, &color);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetNumTevStages(1);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xC, 8, 0xA);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevOrder(0, 0, 0, 4);

        audience = BattleAudienceBaseGetPtr();
        tpl = *(void**)(*(s32*)(*(s32*)((s32)audience + 0xC) + 0xA0) + 0);
        TEXGetGXTexObjFromPalette(tpl, texObj, *(s32*)(work + 0x200));
        GXLoadTexObj(texObj, 0);

        PSMTXTrans(transMtx, *(f32*)(work + 0x1D0), *(f32*)(work + 0x1D4), *(f32*)(work + 0x1D8));
        PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, modelMtx);
        GXLoadPosMtxImm(modelMtx, 0);

        for (i = 0; i < 16; i++) {
            uvWords[i] = dat_802ff570[i];
        }
        uv = (f32*)uvWords;
        if (*(s8*)(work + 0x1CC) == 1) {
            base = 4;
        } else {
            base = 0;
        }

        GXBegin(0x80, 0, 4);
        fifo = (volatile f32*)0xCC008000;
        *fifo = float_neg20_80427b68;
        *fifo = float_40_80427b6c;
        *fifo = float_0_80427b70;
        *fifo = uv[base * 2];
        *fifo = uv[base * 2 + 1];
        *fifo = float_20_80427b74;
        *fifo = float_40_80427b6c;
        *fifo = float_0_80427b70;
        *fifo = uv[(base + 1) * 2];
        *fifo = uv[(base + 1) * 2 + 1];
        *fifo = float_20_80427b74;
        *fifo = float_0_80427b70;
        *fifo = float_0_80427b70;
        *fifo = uv[(base + 2) * 2];
        *fifo = uv[(base + 2) * 2 + 1];
        *fifo = float_neg20_80427b68;
        *fifo = float_0_80427b70;
        *fifo = float_0_80427b70;
        *fifo = uv[(base + 3) * 2];
        *fifo = uv[(base + 3) * 2 + 1];
    }
}

s32 main_star(void) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 order);
    extern void* _battleWorkPointer;
    extern f32 float_0_80427b70;
    extern f32 float_neg1000_80427bd8;
    extern f32 float_1_80427b60;
    extern f32 float_37_80427c24;
    extern f32 float_50_80427bd4;
    extern f32 float_neg1_80427b5c;
    extern f32 float_2_80427bb4;
    extern f32 float_2160_80427c28;
    extern f32 float_300_80427b9c;
    extern f32 float_1p5_80427ba0;
    extern u32 vec3_802ff3d0[3];
    extern u32 vec3_802ff3dc[3];

    u8* work;
    u8* mario;
    u8* starWork;
    s32 state;
    s32 timer;
    f32 value;
    f32 start;
    f32 end;
    u32* srcA;
    u32* srcB;

    work = (u8*)GetBakuGamePtr();
    mario = (u8*)BattleGetMarioPtr(_battleWorkPointer);
    state = *(s32*)(work + 0x204);

    switch (state) {
        case 0:
            break;
        case 1:
            value = float_0_80427b70;
            *(s32*)(work + 0x204) = 2;
            *(s32*)(work + 0x208) = 0;
            *(void**)(work + 0x254) = effStarStoneEntry(1, value, float_neg1000_80427bd8, value, float_1_80427b60);

            BtlUnit_GetPos(mario, (f32*)(work + 0x20C), (f32*)(work + 0x210), (f32*)(work + 0x214));
            *(f32*)(work + 0x210) += (*(f32*)(mario + 0x114) * (f32)*(s16*)(mario + 0xCE)) + float_37_80427c24;

            srcA = vec3_802ff3d0;
            srcB = vec3_802ff3dc;

            *(u32*)(work + 0x218) = *(u32*)(work + 0x20C);
            *(u32*)(work + 0x21C) = *(u32*)(work + 0x210);
            *(u32*)(work + 0x220) = *(u32*)(work + 0x214);

            *(u32*)(work + 0x224) = *(u32*)(work + 0x20C);
            *(u32*)(work + 0x228) = *(u32*)(work + 0x210);
            *(u32*)(work + 0x22C) = *(u32*)(work + 0x214);
            *(f32*)(work + 0x228) += float_50_80427bd4;
            *(f32*)(work + 0x22C) += float_neg1_80427b5c;

            *(u32*)(work + 0x23C) = srcA[0];
            *(u32*)(work + 0x240) = srcA[1];
            *(u32*)(work + 0x244) = srcA[2];
            *(u32*)(work + 0x248) = srcB[0];
            *(u32*)(work + 0x24C) = srcB[1];
            *(u32*)(work + 0x250) = srcB[2];
            /* fall through */
        case 2:
            timer = *(s32*)(work + 0x208) + 1;
            *(s32*)(work + 0x208) = timer;
            if (timer <= 100) {
                start = *(f32*)(work + 0x218);
                end = *(f32*)(work + 0x224);
                timer = *(s32*)(work + 0x208);
                *(f32*)(work + 0x20C) = intplGetValue(0, timer, 100, start, end);

                start = *(f32*)(work + 0x21C);
                end = *(f32*)(work + 0x228);
                timer = *(s32*)(work + 0x208);
                *(f32*)(work + 0x210) = intplGetValue(0, timer, 100, start, end);

                start = *(f32*)(work + 0x220);
                end = *(f32*)(work + 0x22C);
                timer = *(s32*)(work + 0x208);
                *(f32*)(work + 0x214) = intplGetValue(0, timer, 100, start, end);

                start = float_0_80427b70;
                end = float_2_80427bb4;
                timer = *(s32*)(work + 0x208);
                value = intplGetValue(0, timer, 100, start, end);
                *(f32*)(work + 0x244) = value;
                *(f32*)(work + 0x240) = value;
                *(f32*)(work + 0x23C) = value;
            } else {
                value = float_2_80427bb4;
                *(f32*)(work + 0x20C) = *(f32*)(work + 0x224);
                *(f32*)(work + 0x210) = *(f32*)(work + 0x228);
                *(f32*)(work + 0x214) = *(f32*)(work + 0x22C);
                *(f32*)(work + 0x244) = value;
                *(f32*)(work + 0x240) = value;
                *(f32*)(work + 0x23C) = value;
            }

            start = float_0_80427b70;
            end = float_2160_80427c28;
            timer = *(s32*)(work + 0x208);
            *(f32*)(work + 0x24C) = intplGetValue(4, timer, 0x78, start, end);
            if (*(s32*)(work + 0x208) >= 0x78) {
                *(s32*)(work + 0x204) = 3;
                *(s32*)(work + 0x208) = 0;
                value = float_300_80427b9c;
                *(u32*)(work + 0x218) = *(u32*)(work + 0x20C);
                *(u32*)(work + 0x21C) = *(u32*)(work + 0x210);
                *(u32*)(work + 0x220) = *(u32*)(work + 0x214);
                *(f32*)(work + 0x228) = value;
            }
            break;
        case 3:
            timer = *(s32*)(work + 0x208);
            timer++;
            *(s32*)(work + 0x208) = timer;
            start = *(f32*)(work + 0x21C);
            end = *(f32*)(work + 0x228);
            timer = *(s32*)(work + 0x208);
            *(f32*)(work + 0x210) = intplGetValue(1, timer, 0x3C, start, end);
            if (*(s32*)(work + 0x208) >= 0x3C) {
                *(s32*)(work + 0x204) = 4;
                *(s32*)(work + 0x208) = 0;
            }
            break;
        case 4:
        default:
            break;
    }

    if (*(void**)(work + 0x254) != 0) {
        starWork = *(u8**)((s32)*(void**)(work + 0x254) + 0xC);
        value = float_1p5_80427ba0;
        *(f32*)(starWork + 0x8) = *(f32*)(work + 0x20C);
        *(f32*)(starWork + 0xC) = *(f32*)(work + 0x210);
        *(f32*)(starWork + 0x10) = *(f32*)(work + 0x214);
        *(f32*)(starWork + 0x18) = *(f32*)(work + 0x248);
        *(f32*)(starWork + 0x1C) = *(f32*)(work + 0x24C);
        *(f32*)(starWork + 0x20) = *(f32*)(work + 0x250);
        *(f32*)(starWork + 0x14) = value * *(f32*)(work + 0x23C);
    }

    dispEntry(4, 2, bakuGameDispStar, 0, float_0_80427b70);
    return 0;
}

/* CHATGPT STUB FILL: main/action/star/sac_bakugame 20260624_184823 */

/* stub-fill: bakuGameDisp2D | missing_definition | ghidra_signature */
void bakuGameDisp2D(void) {
    extern void* GetBakuGamePtr(void);
    extern void* _battleWorkPointer;
    extern void btlGetScreenPoint(void* pos, void* out);
    extern void iconDispGx(void* pos, s32 size, s32 iconId, f32 scale);
    extern void BattleAcDrawGauge(s32 x, s32 y, s32 icon, s32 unk1, s32 unk2, s32 unk3, s32 unk4, f32 value);
    extern f32 float_100_80427b78;

    void* work = GetBakuGamePtr();
    void* battle = _battleWorkPointer;
    f32 screen[2];
    s32 i;
    s32 offset;
    s32 icon;

    for (i = 0, offset = 0; i < 3; i++, offset += 0x24) {
        void* item = (void*)((s32)work + offset + 0xC);
        switch (*(s32*)((s32)item + 8)) {
            case 0x100:
                icon = 0x6C;
                break;
            case 0x200:
                icon = 0x6E;
                break;
            case 0x400:
                icon = 0x70;
                break;
            default:
                icon = 0;
                break;
        }
        if ((u16)icon != 0) {
            btlGetScreenPoint((void*)((s32)item + 0x14), screen);
            iconDispGx(screen, 0x10, icon, *(f32*)((s32)item + 0x20));
        }
    }

    if (*(f32*)((s32)work + 0xD0) >= float_100_80427b78) {
        *(u8*)((s32)battle + 0x1F2C) = 0;
        *(u8*)((s32)battle + 0x1F2F) = 0xFF;
        *(u8*)((s32)battle + 0x1F2D) += *(u8*)((s32)battle + 0x1F31);
        *(u8*)((s32)battle + 0x1F2E) += *(u8*)((s32)battle + 0x1F32);
        if (*(u8*)((s32)battle + 0x1F2D) <= 0x80 && *(s8*)((s32)battle + 0x1F31) <= 0) {
            *(u8*)((s32)battle + 0x1F31) = 0x10;
            *(u8*)((s32)battle + 0x1F32) = 0x10;
        }
        if (*(u8*)((s32)battle + 0x1F2D) >= 0xF5 && *(s8*)((s32)battle + 0x1F31) >= 0) {
            *(u8*)((s32)battle + 0x1F31) = -0x10;
            *(u8*)((s32)battle + 0x1F32) = -0x10;
        }
    }

    BattleAcDrawGauge((s32)*(f32*)((s32)work + 0xDC), (s32)*(f32*)((s32)work + 0xE0),
                      0xB1, 3, 0x21, 0x42, 0x64, *(f32*)((s32)work + 0xE8));
}

/* stub-fill: bakuGameHeihoReturn | missing_definition | ghidra_signature */
s32 bakuGameHeihoReturn(s32 unused, s32 reset) {
    extern void* GetBakuGamePtr(void);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void dispEntry(s32 cameraId, s32 layer, void* func, s32 param, f32 z);
    extern void bakuGameDisp3D(void);
    extern u32 vec3_802ff4f8[3];
    extern f32 float_0_80427b70;
    extern f32 float_neg2_80427b8c;

    void* work = GetBakuGamePtr();
    s32 phase;
    s32 frameMod;

    if ((*(u32*)work & 1) == 0) {
        return 2;
    }

    if (reset != 0) {
        *(s32*)((s32)work + 0x1C0) = 0;
    }

    switch (*(s32*)((s32)work + 0x1C0)) {
        case 0:
            if (*(f32*)((s32)work + 0x1D4) != float_0_80427b70) {
                *(f32*)((s32)work + 0x1D4) += *(f32*)((s32)work + 0x1F8);
                *(f32*)((s32)work + 0x1F8) += float_neg2_80427b8c;
            }
            if (*(f32*)((s32)work + 0x1D4) <= float_0_80427b70) {
                *(f32*)((s32)work + 0x1D4) = float_0_80427b70;
                *(s32*)((s32)work + 0x1C0) = 5;
            }
            break;
        case 5:
            *(s32*)((s32)work + 0x1C0) = 10;
            *(s8*)((s32)work + 0x1CC) = 1;
            *(s32*)((s32)work + 0x1DC) = *(s32*)((s32)work + 0x1D0);
            *(s32*)((s32)work + 0x1E0) = *(s32*)((s32)work + 0x1D4);
            *(s32*)((s32)work + 0x1E4) = *(s32*)((s32)work + 0x1D8);
            *(s32*)((s32)work + 0x1E8) = vec3_802ff4f8[0];
            *(s32*)((s32)work + 0x1EC) = vec3_802ff4f8[1];
            *(s32*)((s32)work + 0x1F0) = vec3_802ff4f8[2];
            *(f32*)((s32)work + 0x1F0) = *(f32*)((s32)work + 0x1D8);
            *(s32*)((s32)work + 0x1C4) = 0;
            *(s32*)((s32)work + 0x1C8) = 0x1E;
            break;
        case 10:
            *(f32*)((s32)work + 0x1D0) = intplGetValue(0, *(s32*)((s32)work + 0x1C4),
                                                        *(s32*)((s32)work + 0x1C8),
                                                        *(f32*)((s32)work + 0x1DC),
                                                        *(f32*)((s32)work + 0x1E8));
            frameMod = *(s32*)((s32)work + 0x1C4) % 12;
            if (frameMod >= 0 && frameMod <= 3) {
                *(s32*)((s32)work + 0x200) = 0x5A;
            }
            if (frameMod >= 4 && frameMod <= 7) {
                *(s32*)((s32)work + 0x200) = 0x5B;
            }
            if (frameMod >= 8 && frameMod <= 11) {
                *(s32*)((s32)work + 0x200) = 0x5C;
            }
            phase = *(s32*)((s32)work + 0x1C4) + 1;
            *(s32*)((s32)work + 0x1C4) = phase;
            if (phase > *(s32*)((s32)work + 0x1C8)) {
                return 2;
            }
            break;
    }

    dispEntry(4, 1, bakuGameDisp3D, 0, float_0_80427b70);
    return 0;
}

/* stub-fill: bakuGameMain | missing_definition | ghidra_signature */
s32 bakuGameMain(void* event, s32 isFirstCall) {
    extern void* GetBakuGamePtr(void);
    extern void* g_BattleWork;
    extern s32 BattleTransID(void*, s32);
    extern void* BattleGetUnitPtr(void*, s32);
    extern void bakuGameDecideButton(s32);
    extern s32 bakuGameAudienceCanThrowPos(s32);
    extern s32 BattleAudience_GetWaiting(s32);
    extern s32 irand(s32);
    extern void memcpy(void*, void*, u32);
    extern u8 weapon[];
    u32* work = GetBakuGamePtr();
    s32 i;

    BattleGetUnitPtr(g_BattleWork, BattleTransID(event, -3));
    BattleGetUnitPtr(g_BattleWork, BattleTransID(event, -4));
    if (isFirstCall != 0) {
        s32 canThrow = 0;
        work[1] = 0;
        bakuGameDecideButton(1);
        for (i = 0; i < 3; i++) {
            u8* slot = (u8*)work + 0xC + i * 0x24;
            *(s32*)slot = 0;
            *(f32*)(slot + 0x10) = 100.0f;
            *(f32*)(slot + 0x20) = 0.0f;
        }
        memcpy(work + 0x3E, weapon, 0xC0);
        for (i = 0; i < 200; i++) {
            if (bakuGameAudienceCanThrowPos(i) == 1 && BattleAudience_GetWaiting(i)) {
                canThrow = 1;
            }
        }
        if (canThrow) *work &= ~2U;
        else *work |= 2;
    }
    switch (work[1]) {
        case 0:
            for (i = 0; i < 3; i++) {
                if (*(s32*)((u8*)work + 0xC + i * 0x24) != 5) break;
            }
            if (i == 3 && work[0x1F] == 8 && (((*work & 1) == 0) || work[0x70] == 5)) {
                work[1] = 5;
            }
            break;
        case 5:
            work[1] = 10;
            work[2] = 0;
            for (i = 0; i < 3; i++) {
                u8* slot = (u8*)work + 0xC + i * 0x24;
                *(s32*)slot = 10;
                *(s32*)(slot + 4) = 0;
            }
            work[0x1F] = 10;
            work[0x3C] = irand(0x78) + 0x21C;
        case 10:
            work[2]++;
            if ((s32)work[2] > 60 && irand(0x96) == 0) {
                for (i = 0; i < 3; i++) *(s32*)((u8*)work + 0xC + i * 0x24) = 10;
                bakuGameDecideButton(0);
                work[2] = 0;
            }
            if (work[2] == 180) {
                for (i = 0; i < 3; i++) *(s32*)((u8*)work + 0xC + i * 0x24) = 10;
                bakuGameDecideButton(0);
                work[2] = 0;
            }
            break;
    }
    return 0;
}

