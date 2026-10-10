#include "effect/eff_fukidashi.h"

#pragma optimize_for_size off
#pragma use_lmw_stmw off
u8 calc_pos(void* work, s32 flag) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* camGetPtr(s32 cameraId);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern void PSMTXRotAxisRad(f32 mtx[3][4], Vec* axis, f32 rad);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern const Vec vec3_802f3648;
    extern f32 float_deg2rad_804228c4;
    extern f32 float_20_80422950;
    extern f32 float_16_804228e0;
    extern f32 float_12_80422954;
    extern f32 float_0p5_8042291c;

    void* cam;
    void* base;
    void* target;
    Vec rotAxis;
    Vec axis;
    f32 mtx[3][4];
    s32 offset;
    f32 heightBase;

    cam = camGetPtr(4);
    axis = vec3_802f3648;
    axis.x = (f32)sin(float_deg2rad_804228c4 * *(f32*)((s32)cam + 0x114));
    axis.z = -(f32)cos(float_deg2rad_804228c4 * *(f32*)((s32)cam + 0x114));
    offset = flag * 0x14;
    rotAxis = axis;
    PSMTXRotAxisRad(mtx, &rotAxis,
                    float_deg2rad_804228c4 *
                        -((*(f32*)((s32)work + 0x1C) - float_20_80422950) + (f32)offset));

    target = *(void**)((s32)work + 0x44);
    if ((s32)target == -1) {
        base = marioGetPtr();
        heightBase = float_16_804228e0;
        *(f32*)((s32)work + 4) = mtx[0][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                  (*(f32*)((s32)base + 0x8C) + *(f32*)((s32)work + 0x10));
        *(f32*)((s32)work + 8) = mtx[1][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                  (*(f32*)((s32)base + 0x90) + *(f32*)((s32)work + 0x14));
        *(f32*)((s32)work + 0xC) = mtx[2][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                    (*(f32*)((s32)base + 0x94) + *(f32*)((s32)work + 0x18));
    } else if ((s32)target == -2) {
        base = partyGetPtr(marioGetPartyId());
        if (base != 0) {
            heightBase = float_16_804228e0;
            *(f32*)((s32)work + 4) = mtx[0][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                      (*(f32*)((s32)base + 0x58) + *(f32*)((s32)work + 0x10));
            *(f32*)((s32)work + 8) = mtx[1][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                      (*(f32*)((s32)base + 0x5C) + *(f32*)((s32)work + 0x14));
            *(f32*)((s32)work + 0xC) = mtx[2][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                        (*(f32*)((s32)base + 0x60) + *(f32*)((s32)work + 0x18));
        }
    } else if ((s32)target == -3) {
        base = partyGetPtr(marioGetExtraPartyId());
        if (base != 0) {
            heightBase = float_16_804228e0;
            *(f32*)((s32)work + 4) = mtx[0][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                      (*(f32*)((s32)base + 0x58) + *(f32*)((s32)work + 0x10));
            *(f32*)((s32)work + 8) = mtx[1][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                      (*(f32*)((s32)base + 0x5C) + *(f32*)((s32)work + 0x14));
            *(f32*)((s32)work + 0xC) = mtx[2][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                        (*(f32*)((s32)base + 0x60) + *(f32*)((s32)work + 0x18));
        }
    } else if (target != 0) {
        heightBase = float_16_804228e0;
        *(f32*)((s32)work + 4) = mtx[0][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                  (*(f32*)((s32)target + 0x8C) + *(f32*)((s32)work + 0x10));
        *(f32*)((s32)work + 8) = mtx[1][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                  (*(f32*)((s32)target + 0x90) + *(f32*)((s32)work + 0x14));
        *(f32*)((s32)work + 0xC) = mtx[2][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                    (*(f32*)((s32)target + 0x94) + *(f32*)((s32)work + 0x18));
    } else {
        heightBase = float_16_804228e0;
        *(f32*)((s32)work + 4) = mtx[0][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                  *(f32*)((s32)work + 0x10);
        *(f32*)((s32)work + 8) = mtx[1][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                  *(f32*)((s32)work + 0x14);
        *(f32*)((s32)work + 0xC) = mtx[2][1] * (heightBase + *(f32*)((s32)work + 0x20)) +
                                    *(f32*)((s32)work + 0x18);
    }

    *(f32*)((s32)work + 0x28) = (*(f32*)((s32)work + 0x1C) - float_20_80422950) + (f32)offset;
    *(f32*)((s32)work + 0x2C) =
        float_0p5_8042291c * (*(f32*)((s32)work + 0x20) / float_12_80422954) + float_0p5_8042291c;
}
#pragma optimize_for_size reset
#pragma use_lmw_stmw reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void* effFukidashiEntry(f32 x, f32 y, f32 z, f32 scale, f32 angle, s32 kind, s32 field3c, s32 target, s32 timer) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effFukidashiMain(void* effect);
    extern u8 calc_pos(void* work, s32 flag);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern u32 psndSFXOn_3D(char* id, Vec* pos);
    extern const Vec vec3_802f3654;
    extern const Vec vec3_802f3660;
    extern const Vec vec3_802f366c;
    extern const Vec vec3_802f3678;
    extern char str_Fukidashi_802f36b0[];
    extern char str_SFX_EVT_NPC_BIKKURI__802f36bc[];
    extern char str_SFX_EVT_NPC_QUESTION_802f36d8[];
    extern f32 float_0_804228c0;
    extern f32 float_3_80422930;
    extern f32 float_neg450_8042294c;

    extern const u8 vec3_802f3648[];
    const u8* readonlyPool = vec3_802f3648;
    void* effect;
    void* work;
    void* base;
    void* targetPtr;
    Vec partyBikkuriPos;
    Vec partyQuestionPos;
    Vec npcBikkuriPos;
    Vec npcQuestionPos;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = (char*)(readonlyPool + 104);
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, 0x58);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effFukidashiMain;

    *(s32*)work = kind;
    *(s32*)((s32)work + 0x3C) = field3c;
    if (timer < 0) {
        timer = 10000;
    }
    *(s32*)((s32)work + 0x40) = timer;
    *(s32*)((s32)work + 0x30) = timer;
    *(s32*)((s32)work + 0x34) = 0;
    *(s32*)((s32)work + 0x44) = target;
    *(f32*)((s32)work + 0x24) = float_0_804228c0;
    *(s32*)((s32)work + 0x48) = 0xFF;
    *(s32*)((s32)work + 0x4C) = 4;
    *(f32*)((s32)work + 0x10) = x;
    *(f32*)((s32)work + 0x14) = y + float_3_80422930;
    *(f32*)((s32)work + 0x18) = z;
    *(f32*)((s32)work + 0x1C) = angle;
    *(f32*)((s32)work + 0x20) = scale;
    *(s32*)((s32)work + 0x54) = 0;

    calc_pos(work, 1);
    if (kind == 6) {
        *(f32*)((s32)work + 0x24) = float_neg450_8042294c;
    }

    targetPtr = *(void**)((s32)work + 0x44);
    if ((s32)targetPtr == -1) {
        base = marioGetPtr();
        x += *(f32*)((s32)base + 0x8C);
        y += *(f32*)((s32)base + 0x90);
        z += *(f32*)((s32)base + 0x94);
    } else if ((s32)targetPtr == -2) {
        base = partyGetPtr(marioGetPartyId());
        if (base != 0) {
            x += *(f32*)((s32)base + 0x58);
            y += *(f32*)((s32)base + 0x5C);
            z += *(f32*)((s32)base + 0x60);
        }
    } else if ((s32)targetPtr == -3) {
        base = partyGetPtr(marioGetExtraPartyId());
        if (base != 0) {
            x += *(f32*)((s32)base + 0x58);
            y += *(f32*)((s32)base + 0x5C);
            z += *(f32*)((s32)base + 0x60);
        }
    } else if (targetPtr != 0) {
        x = *(f32*)((s32)targetPtr + 0x8C);
        y = *(f32*)((s32)targetPtr + 0x90);
        z = *(f32*)((s32)targetPtr + 0x94);
    }

    if ((target == -2) || (target == -3)) {
        switch (kind) {
        case 0:
            partyBikkuriPos = *(const Vec*)(readonlyPool + 12);
            partyBikkuriPos.x = x;
            partyBikkuriPos.y = y;
            partyBikkuriPos.z = z;
            psndSFXOn_3D((char*)0x5D, &partyBikkuriPos);
            break;
        case 1:
            partyQuestionPos = *(const Vec*)(readonlyPool + 24);
            partyQuestionPos.x = x;
            partyQuestionPos.y = y;
            partyQuestionPos.z = z;
            psndSFXOn_3D((char*)0x5E, &partyQuestionPos);
            break;
        }
    } else if ((target != 0) && (target != -1)) {
        switch (kind) {
        case 0:
            npcBikkuriPos = *(const Vec*)(readonlyPool + 36);
            npcBikkuriPos.x = x;
            npcBikkuriPos.y = y;
            npcBikkuriPos.z = z;
            psndSFXOn_3D((char*)(readonlyPool + 116), &npcBikkuriPos);
            break;
        case 1:
            npcQuestionPos = *(const Vec*)(readonlyPool + 48);
            npcQuestionPos.x = x;
            npcQuestionPos.y = y;
            npcQuestionPos.z = z;
            psndSFXOn_3D((char*)(readonlyPool + 144), &npcQuestionPos);
            break;
        }
    }

    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void effFukidashiMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effFukidashiDisp(s32 camera, void* effect);
    extern u8 calc_pos(void* work, s32 flag);
    extern const LocalVec3 vec3_802f3684;
    extern s8* anim_data_table[7];
    extern s8* anim_data_table_2[7];
    extern f32 float_22p5_80422940;
    extern f32 float_0_804228c0;
    extern f32 float_9_80422944;
    extern f32 float_neg90_80422948;
    void* work;
    LocalVec3 dispPos;
    LocalVec3 pos;
    s32 value;
    s32 kind;

    work = *(void**)((s32)effect + 0xC);
    pos = vec3_802f3684;
    pos.x = *(f32*)((s32)work + 0x4);
    pos.y = *(f32*)((s32)work + 0x8);
    pos.z = *(f32*)((s32)work + 0xC);
    dispPos = pos;
    kind = *(s32*)work;

    if (*(s32*)((s32)work + 0x40) == 0) {
        value = anim_data_table[kind][*(s32*)((s32)work + 0x34) / 2];
    } else {
        value = anim_data_table_2[kind][*(s32*)((s32)work + 0x34) / 2];
    }
    *(s32*)((s32)work + 0x34) += 1;
    *(s32*)((s32)work + 0x38) = value;

    if (value < 0) {
        if (value == -1) {
            effDelete(effect);
            return;
        }
        *(s32*)((s32)work + 0x34) = (-10 - value) * 2;
        if (*(s32*)((s32)work + 0x40) == 0) {
            value = anim_data_table[kind][*(s32*)((s32)work + 0x34) / 2];
        } else {
            value = anim_data_table_2[kind][*(s32*)((s32)work + 0x34) / 2];
        }
        *(s32*)((s32)work + 0x34) += 1;
        *(s32*)((s32)work + 0x38) = value;
    }

    if (kind == 6) {
        switch (*(s32*)((s32)work + 0x54)) {
        case 0:
            *(f32*)((s32)work + 0x24) += float_22p5_80422940;
            if (*(f32*)((s32)work + 0x24) >= float_0_804228c0) {
                *(f32*)((s32)work + 0x24) = float_0_804228c0;
                *(s32*)((s32)work + 0x54) += 1;
            }
            break;
        case 0x78:
            *(f32*)((s32)work + 0x24) -= float_9_80422944;
            if (*(f32*)((s32)work + 0x24) <= float_neg90_80422948) {
                effDelete(effect);
                return;
            }
            break;
        default:
            *(s32*)((s32)work + 0x54) += 1;
            break;
        }
    } else if (*(s32*)((s32)work + 0x40) > 0) {
        if (*(s32*)((s32)work + 0x30) < 10000) {
            *(s32*)((s32)work + 0x30) -= 1;
        }
        if (*(s32*)((s32)work + 0x30) < 0) {
            effDelete(effect);
            return;
        }
    }

    if (*(void**)((s32)work + 0x44) != 0) {
        calc_pos(work, 1);
    }
    if (*(s32*)work == 4) {
        dispEntry(*(s32*)((s32)work + 0x4C), 2, effFukidashiDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(*(s32*)((s32)work + 0x4C), 8, effFukidashiDisp, effect, dispCalcZ(&dispPos));
    }
    return;
}
#pragma optimize_for_size reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



void effFukidashiDisp(s32 param_1, void* effect)

{
    extern f64 double_to_int_mask_802f36a8;
    union { f64 number; u32 words[2]; } unsignedConversion;
    #define FUKIDASHI_UNSIGNED_FLOAT(value) (unsignedConversion.words[0]=0x43300000, unsignedConversion.words[1]=(u32)(value), (f32)(unsignedConversion.number-double_to_int_mask_802f36a8))


    typedef f32 MTX34[3][4];
    typedef struct { f32 x,y,z; } Vec;
    typedef struct { u8 r,g,b,a; } GXColor;
    typedef struct { u32 data[8]; } GXTexObj;
    typedef struct { u32 data[16]; } GXLightObj;
    extern char* camGetPtr(s32);
    extern u8* gp;
    extern const Vec vec3_802f3690;
    extern const f32 chuchu_dt[];
    extern u8 moja_model_vertex_tbl[],moja_model_normal_tbl[],moja_model_color0_tbl[];
    extern void* moja_model_dl_0_tbl[];
    extern u8 moja_model_dl_0_size_tbl[];
    extern char str_PCTd_80422918[];
    extern f64 cos(f64),sin(f64);
    extern s32 sprintf(char*,const char*,...);
    extern u16 FontGetMessageWidth(char*);
    extern void FontDrawStart_alpha(u8);
    extern void FontDrawStringMtx(void*,char*);
    extern void iconDispGxCol(void*,u16,u16,void*);
    extern void mapSetMaterialFog(void);
    extern void effGetTexObj(s32,void*);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,f32,char);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXMultVec(void*,void*,void*);
    extern void PSMTXInvXpose(void*,void*);
    extern void PSMTXTransApply(void*,void*,f32,f32,f32);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevOp(s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32,void*);
    extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetZMode(s32,s32,s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetChanMatColor(s32,void*);
    extern void GXSetChanAmbColor(s32,void*);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXLoadNrmMtxImm(void*,s32);
    extern void GXBegin(s32,s32,s32);
    extern void GXInitLightPos(void*,f32,f32,f32);
    extern void GXInitLightColor(void*,void*);
    extern void GXInitLightAttn(void*,f32,f32,f32,f32,f32,f32);
    extern void GXLoadLightObjImm(void*,s32);
    extern void GXSetArray(s32,void*,u8);
    extern void GXCallDisplayList(void*,u32);
    extern f32 float_0_804228c0;
    extern f32 float_0p15_80422914;
    extern f32 float_0p16667_804228ec;
    extern f32 float_0p25_804228f0;
    extern f32 float_0p43633_80422910;
    extern f32 float_0p5_8042291c;
    extern f32 float_0p7_8042293c;
    extern f32 float_0p8_804228c8;
    extern f32 float_10_80422908;
    extern f32 float_13_804228dc;
    extern f32 float_16_804228e0;
    extern f32 float_1_804228cc;
    extern f32 float_1p3_804228d0;
    extern f32 float_22_80422924;
    extern f32 float_2_804228e4;
    extern f32 float_3_80422930;
    extern f32 float_4_80422900;
    extern f32 float_5_8042290c;
    extern f32 float_7_80422938;
    extern f32 float_8_804228fc;
    extern f32 float_deg2rad_804228c4;
    extern f32 float_neg10_80422920;
    extern f32 float_neg16_804228d8;
    extern f32 float_neg19_804228e8;
    extern f32 float_neg1_804228f4;
    extern f32 float_neg1p3_804228d4;
    extern f32 float_neg3_8042292c;
    extern f32 float_neg4_804228f8;
    extern f32 float_neg7_80422934;
    extern f32 float_neg8_80422904;
    extern u32 dat_80422898;
    extern u32 dat_8042289c;
    extern u32 dat_804228a0;
    extern u32 dat_804228a4;
    extern u32 dat_804228a8;
    extern u32 dat_804228ac;
    extern u32 dat_804228b0;
    extern u32 dat_804228b4;
    extern u32 dat_804228b8;
    extern u32 unk_804295a0;
    extern u32 unk_804295a4;
    extern u32 unk_804295a8;
    extern u32 unk_804295ac;
    extern u32 unk_804295b0;
  float fVar1;
  float fVar2;
  char *pcVar3;
  char *pcVar4;
  int iVar5;
  void **ppuVar6;
  u32 uVar7;
  u8 *pbVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  u8 bVar12;
  int *piVar13;
  u8 bVar14;
  char cVar15;
  double dVar16;
  u32 local_380;
  u32 local_37c;
  u32 local_378;
  u32 local_374;
  u32 local_370;
  u32 local_36c;
  u32 local_368;
  u32 local_364;
  u32 local_360;
  u32 local_35c;
  u32 local_358;
  u32 local_354;
  u32 local_350;
  u32 local_34c;
  GXColor local_348;
  GXColor local_344;
  u32 local_340;
  u32 local_33c;
  u32 local_338;
  u32 local_334;
  u32 local_330;
  u32 local_32c;
  GXColor local_328;
  GXColor local_324;
  GXColor local_320;
  GXColor local_31c;
  GXColor local_318;
  GXColor local_314;
  float local_310;
  float local_30c;
  float local_308;
  Vec local_304;
  char acStack_2f8 [32];
  GXTexObj GStack_2d8;
  MTX34 MStack_2b8;
  GXLightObj GStack_288;
  u8 auStack_248 [48];
  MTX34 MStack_218;
  MTX34 MStack_1e8;
  MTX34 MStack_1b8;
  MTX34 MStack_188;
  MTX34 MStack_158;
  MTX34 MStack_128;
  MTX34 MStack_f8;
  MTX34 MStack_c8;
  u32 uStack_94;
  int iStack_8c;
  u32 uStack_84;
  
  piVar13 = *(int **)((char*)effect + 0xc);
  pcVar3 = camGetPtr(param_1);
  iVar11 = piVar13[0x12];
  iVar9 = 1;
  mapSetMaterialFog();
  fVar2 = float_deg2rad_804228c4;
  if (float_0_804228c0 != *(f32*)(piVar13+9)) {
    iVar9 = 2;
  }
  if (*piVar13 == 2) {
    iVar9 = 2;
  }
  iVar5 = (iVar11 * 0xb0) / 0xff + (iVar11 * 0xb0 >> 0x1f);
  iVar10 = (iVar11 * 0xff) / 0xff + (iVar11 * 0xff >> 0x1f);
  cVar15 = (char)iVar5 - (char)(iVar5 >> 0x1f);
  bVar14 = (char)iVar10 - (char)(iVar10 >> 0x1f);
  for (iVar10 = 0; iVar10 < iVar9; iVar10 = iVar10 + 1) {
    PSMTXTrans(&MStack_128, (double)*(f32*)(piVar13+1), (double)*(f32*)(piVar13+2),
                    (double)*(f32*)(piVar13+3));
    pcVar4 = camGetPtr(param_1);
    PSMTXRotRad(&MStack_188, (double)(fVar2 * -*(f32*)(pcVar4+0x114)), 'y');
    PSMTXConcat(&MStack_128, &MStack_188, &MStack_1e8);
    PSMTXScale(&MStack_f8, float_0p8_804228c8, float_0p8_804228c8, float_0p8_804228c8);
    PSMTXConcat(&MStack_1e8, &MStack_f8, &MStack_1e8);
    PSMTXScale(&MStack_f8, float_1_804228cc, float_0p8_804228c8, float_1_804228cc);
    PSMTXRotRad(&MStack_1b8, (double)(fVar2 * *(f32*)(piVar13+10)), 'z');
    PSMTXConcat(&MStack_1e8, &MStack_f8, &MStack_f8);
    PSMTXConcat(&MStack_f8, &MStack_1b8, &MStack_c8);
    PSMTXConcat(pcVar3+0x48, &MStack_c8, &MStack_c8);
    GXSetCurrentMtx(0);
    if (iVar10 == 0) {
      GXSetCullMode(2);
    }
    else {
      GXSetCullMode(1);
    }
    PSMTXIdentity(&MStack_128);
    GXLoadTexMtxImm(&MStack_128, 0x1e, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    iVar5 = 0;
    do {
      bVar12 = (u8)iVar11;
      if (iVar5 == 0) {
        effGetTexObj(5, (u32 *)&GStack_2d8);
        GXLoadTexObj(&GStack_2d8, 0);
        *(u32*)&local_318 = unk_804295a0;
        local_318.a = bVar12;
        local_314 = local_318;
        GXSetChanMatColor(4, &local_314);
        PSMTXTrans(&MStack_128, (double)float_1p3_804228d0, (double)float_neg1p3_804228d4,
                        (double)float_0_804228c0);
        PSMTXConcat(&MStack_c8, &MStack_128, &MStack_128);
        PSMTXRotRad(&MStack_158, (double)(float_deg2rad_804228c4 * *(f32*)(piVar13+9)), 'y');
        PSMTXConcat(&MStack_128, &MStack_158, &MStack_128);
        GXLoadPosMtxImm(&MStack_128, 0);
        GXSetCurrentMtx(0);
      }
      else {
        effGetTexObj(4, (u32 *)&GStack_2d8);
        GXLoadTexObj(&GStack_2d8, 0);
        if (iVar10 == 0) {
          *(u32*)&local_320 = dat_80422898;
          local_320.a = bVar12;
          local_31c = local_320;
          GXSetChanMatColor(4, &local_31c);
        }
        else {
          *(u32*)&local_328 = dat_8042289c;
          local_328.a = bVar12;
          local_324 = local_328;
          GXSetChanMatColor(4, &local_324);
        }
        dVar16 = (double)float_0_804228c0;
        PSMTXTrans(&MStack_128, dVar16, dVar16, dVar16);
        PSMTXConcat(&MStack_c8, &MStack_128, &MStack_128);
        PSMTXRotRad(&MStack_158, (double)(float_deg2rad_804228c4 * *(f32*)(piVar13+9)), 'y');
        PSMTXConcat(&MStack_128, &MStack_158, &MStack_128);
        GXLoadPosMtxImm(&MStack_128, 0);
        GXSetCurrentMtx(0);
      }
      GXBegin(128, 0, 4);
      iVar5 = iVar5 + 1;
      *(volatile f32*)0xCC008000 = float_neg16_804228d8;
      *(volatile f32*)0xCC008000 = float_13_804228dc;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_16_804228e0;
      *(volatile f32*)0xCC008000 = float_13_804228dc;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_2_804228e4;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_16_804228e0;
      *(volatile f32*)0xCC008000 = float_neg19_804228e8;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_2_804228e4;
      *(volatile f32*)0xCC008000 = float_1_804228cc;
      *(volatile f32*)0xCC008000 = float_neg16_804228d8;
      *(volatile f32*)0xCC008000 = float_neg19_804228e8;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_1_804228cc;
    } while (iVar5 < 2);
    PSMTXScale(&MStack_218, float_1p3_804228d0, float_1p3_804228d0, float_1p3_804228d0);
    PSMTXConcat(&MStack_c8, &MStack_218, &MStack_c8);
    iVar5 = *piVar13;
    switch (*piVar13) {
case 0:
case 1:

          GXSetNumTevStages(1);
          GXSetTevOrder(0, 0, 0, 0xFF);
          GXSetTevColorOp(0, 0, 0, 0, 1, 0);
          GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
          GXSetTevColorIn(0, 15, 15, 15, 2);
          GXSetTevAlphaIn(0, 7, 1, 4, 7);
          iVar5 = *piVar13;
          if (iVar5 == 1) {
            effGetTexObj(7, (u32 *)&GStack_2d8);
            GXLoadTexObj(&GStack_2d8, 0);
            PSMTXScale(&MStack_f8, float_0p25_804228f0, float_1_804228cc, float_1_804228cc);
            uStack_94 = piVar13[0xe] ^ 0x80000000;

            PSMTXTrans(&MStack_128,
                            (double)(f32)(s32)(uStack_94 ^ 0x80000000), (double)float_0_804228c0,
                            (double)float_0_804228c0);
            PSMTXConcat(&MStack_f8, &MStack_128, &MStack_128);
            GXLoadTexMtxImm(&MStack_128, 0x1e, 1);
          }
          else if ((iVar5 < 1) && (iVar5 > -1)) {
            effGetTexObj(6, (u32 *)&GStack_2d8);
            GXLoadTexObj(&GStack_2d8, 0);
            PSMTXScale(&MStack_f8, float_0p16667_804228ec, float_1_804228cc, float_1_804228cc);
            uStack_94 = piVar13[0xe] ^ 0x80000000;

            PSMTXTrans(&MStack_128,
                            (double)(f32)(s32)(uStack_94 ^ 0x80000000), (double)float_0_804228c0,
                            (double)float_0_804228c0);
            PSMTXConcat(&MStack_f8, &MStack_128, &MStack_128);
            GXLoadTexMtxImm(&MStack_128, 0x1e, 1);
          }
          local_330 = ((unk_804295a4 & 0xFFFFFF00) | (u8)cVar15);
          local_32c = local_330;
          GXSetTevColor(1, &local_32c);
          PSMTXTransApply(&MStack_c8, &MStack_218, float_1_804228cc, float_neg1_804228f4, float_0_804228c0);
          GXLoadPosMtxImm(&MStack_218, 0);
          if (*piVar13 == 0) {
            GXBegin(128, 0, 4);
            *(volatile f32*)0xCC008000 = float_neg4_804228f8;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_4_80422900;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_4_80422900;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_neg4_804228f8;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
          }
          else {
            GXBegin(128, 0, 4);
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_16_804228e0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_16_804228e0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
          }
          if (iVar10 == 0) {
            local_338 = ((dat_804228a0 & 0xFFFFFF00) | (u8)bVar14);
            local_334 = local_338;
            GXSetTevColor(1, &local_334);
          }
          else {
            local_340 = ((dat_804228a4 & 0xFFFFFF00) | (u8)bVar14);
            local_33c = local_340;
            GXSetTevColor(1, &local_33c);
          }
          GXLoadPosMtxImm(&MStack_c8, 0);
          if (*piVar13 == 0) {
            GXBegin(128, 0, 4);
            *(volatile f32*)0xCC008000 = float_neg4_804228f8;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_4_80422900;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_4_80422900;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_neg4_804228f8;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
          }
          else {
            GXBegin(128, 0, 4);
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_16_804228e0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_16_804228e0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_8_804228fc;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_neg8_80422904;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_0_804228c0;
            *(volatile f32*)0xCC008000 = float_1_804228cc;
          }
        
break;
case 2:

        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0xff, 0xff, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 15, 15, 15, 10);
        GXSetTevAlphaIn(0, 7, 7, 7, 5);
        GXSetNumChans(1);
        GXSetChanCtrl(4, 1, 0, 1, 1, 2, 2);
        *(u32*)&local_348 = dat_804228a8;
        local_348.a = bVar14;
        local_344 = local_348;
        GXSetChanAmbColor(4, &local_344);
        uStack_94 = *(u32*)(gp+0x1C);

        local_310 = vec3_802f3690.x;
        local_30c = vec3_802f3690.y;
        local_308 = vec3_802f3690.z;
        dVar16 = cos
                           ((double)(FUKIDASHI_UNSIGNED_FLOAT(uStack_94) / float_10_80422908));
        local_30c = *(f32*)(piVar13+2);
        iStack_8c = *(u32*)(gp+0x1C);

        local_310 = float_10_80422908 * (float)dVar16 + *(f32*)(piVar13+1);
        dVar16 = sin
                           ((double)(FUKIDASHI_UNSIGNED_FLOAT(iStack_8c) / float_10_80422908));
        local_308 = -(float_10_80422908 * (float)dVar16 - *(f32*)(piVar13+3));
        local_304.x = local_310;
        local_304.y = local_30c;
        local_304.z = local_308;
        pcVar4 = camGetPtr(param_1);
        PSMTXMultVec(pcVar4+0x48, &local_304, &local_304);
        GXInitLightPos(&GStack_288, local_304.x, local_304.y, local_304.z);
        local_350 = ((dat_804228ac & 0xFFFFFF00) | (u8)bVar14);
        local_34c = local_350;
        GXInitLightColor(&GStack_288, &local_34c);
        dVar16 = (double)float_0_804228c0;
        GXInitLightAttn(&GStack_288, (double)float_1_804228cc, dVar16, dVar16,
                            (double)float_1_804228cc, dVar16, dVar16);
        GXLoadLightObjImm(&GStack_288, 1);
        PSMTXTrans(&MStack_2b8, (double)float_5_8042290c, (double)float_neg8_80422904,
                        (double)float_0_804228c0);
        PSMTXConcat(&MStack_c8, &MStack_2b8, &MStack_218);
        PSMTXRotRad(&MStack_2b8, (double)float_0p43633_80422910, 'z');
        PSMTXConcat(&MStack_218, &MStack_2b8, &MStack_218);

        uStack_84 = *(u32*)(gp+0x1C) * 10;
        PSMTXRotRad(&MStack_2b8,
                         (double)(float_deg2rad_804228c4 *
                                 FUKIDASHI_UNSIGNED_FLOAT(uStack_84)), 'y');
        PSMTXConcat(&MStack_218, &MStack_2b8, &MStack_218);
        PSMTXScale(&MStack_2b8, float_0p15_80422914, float_0p15_80422914, float_0p15_80422914);
        PSMTXConcat(&MStack_218, &MStack_2b8, &MStack_218);
        GXLoadPosMtxImm(&MStack_218, 0);
        PSMTXInvXpose(&MStack_218, auStack_248);
        GXLoadNrmMtxImm(auStack_248, 0);
        GXSetZMode(0, 3, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 8);
        GXSetArray(9, moja_model_vertex_tbl, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, moja_model_normal_tbl, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetArray(11, moja_model_color0_tbl, 4);
        ppuVar6 = moja_model_dl_0_tbl;
        uVar7 = 0;
        pbVar8 = moja_model_dl_0_size_tbl;
        do {
          GXCallDisplayList(*ppuVar6, (u32)*pbVar8 << 5);
          uVar7 = uVar7 + 1;
          ppuVar6 = ppuVar6 + 1;
          pbVar8 = pbVar8 + 1;
        } while (uVar7 < 0x15);
      
break;
case 3:
if (piVar13[0xF]>=0 && piVar13[0xF]<100) {
        sprintf(acStack_2f8, str_PCTd_80422918, piVar13[0xF]);
        uStack_84 = FontGetMessageWidth(acStack_2f8);
        uStack_84 = uStack_84 & 0xffff;

        fVar1 = float_10_80422908 +
                FUKIDASHI_UNSIGNED_FLOAT(uStack_84);
        PSMTXRotRad(&MStack_158, (double)(float_deg2rad_804228c4 * *(f32*)(piVar13+9)), 'y');
        PSMTXScale(&MStack_f8, float_0p5_8042291c, float_0p5_8042291c, float_0p5_8042291c);
        PSMTXTrans(&MStack_128, (double)(-fVar1 * float_0p5_8042291c),
                        (double)float_neg10_80422920, (double)float_0_804228c0);
        PSMTXConcat(&MStack_1e8, &MStack_158, &MStack_218);
        PSMTXConcat(&MStack_218, &MStack_f8, &MStack_218);
        PSMTXConcat(&MStack_218, &MStack_128, &MStack_218);
        if (iVar10 == 0) {
          local_358 = ((dat_804228b0 & 0xFFFFFF00) | (u8)bVar12);
          local_354 = local_358;
          iconDispGxCol(&MStack_218, 0x400, 0x193, &local_354);
        }
        else {
          local_360 = ((dat_804228b4 & 0xFFFFFF00) | (u8)bVar12);
          local_35c = local_360;
          iconDispGxCol(&MStack_218, 0x800, 0x193, &local_35c);
        }
        PSMTXTrans(&MStack_128, (double)float_10_80422908, (double)float_22_80422924,
                        (double)float_0_804228c0);
        PSMTXConcat(&MStack_218, &MStack_128, &MStack_218);
        FontDrawStart_alpha(bVar12);
        GXSetZMode(1, 3, 0);
        if (iVar10 == 0) {
          GXSetCullMode(2);
        }
        else {
          GXSetCullMode(1);
        }
        FontDrawStringMtx(&MStack_218, acStack_2f8);
      }
break;
case 4:

      PSMTXIdentity(&MStack_128);
      GXLoadTexMtxImm(&MStack_128, 0x1e, 1);
      GXSetNumTevStages(1);
      GXSetTevOrder(0, 0, 0, 0xFF);
      GXSetTevColorOp(0, 0, 0, 0, 1, 0);
      GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
      GXSetTevColorIn(0, 15, 15, 15, 15);
      GXSetTevAlphaIn(0, 7, 1, 4, 7);
      local_368 = ((unk_804295a8 & 0xFFFFFF00) | (u8)bVar14);
      local_364 = local_368;
      GXSetTevColor(1, &local_364);
      effGetTexObj(8, (u32 *)&GStack_2d8);
      GXLoadTexObj(&GStack_2d8, 0);
      uVar7 = 0;
      while ((int)uVar7 < piVar13[0xe]) {
        uStack_84 = uVar7 ^ 0x80000000;

        PSMTXTransApply(&MStack_c8, &MStack_218, float_neg3_8042292c * (f32)(s32)uVar7 + float_3_80422930, float_0_804228c0, float_0_804228c0);
        GXLoadPosMtxImm(&MStack_218, 0);
        GXBegin(128, 0, 4);
        uVar7 = uVar7 + 1;
        *(volatile f32*)0xCC008000 = float_neg3_8042292c;
        *(volatile f32*)0xCC008000 = float_3_80422930;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_3_80422930;
        *(volatile f32*)0xCC008000 = float_3_80422930;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_1_804228cc;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_3_80422930;
        *(volatile f32*)0xCC008000 = float_neg3_8042292c;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_1_804228cc;
        *(volatile f32*)0xCC008000 = float_1_804228cc;
        *(volatile f32*)0xCC008000 = float_neg3_8042292c;
        *(volatile f32*)0xCC008000 = float_neg3_8042292c;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_1_804228cc;
      }
    
break;
case 5:

      GXSetNumTevStages(1);
      GXSetTevOrder(0, 0, 0, 0xFF);
      GXSetTevColorOp(0, 0, 0, 0, 1, 0);
      GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
      GXSetTevColorIn(0, 15, 15, 15, 15);
      GXSetTevAlphaIn(0, 7, 1, 4, 7);
      effGetTexObj(9, (u32 *)&GStack_2d8);
      GXLoadTexObj(&GStack_2d8, 0);
      PSMTXScale(&MStack_f8, float_0p25_804228f0, float_1_804228cc, float_1_804228cc);
      uStack_84 = piVar13[0xe] ^ 0x80000000;

      PSMTXTrans(&MStack_128,
                      (double)(f32)(s32)(uStack_84 ^ 0x80000000), (double)float_0_804228c0,
                      (double)float_0_804228c0);
      PSMTXConcat(&MStack_f8, &MStack_128, &MStack_f8);
      GXLoadTexMtxImm(&MStack_f8, 0x1e, 1);
      local_370 = ((unk_804295ac & 0xFFFFFF00) | (u8)bVar14);
      local_36c = local_370;
      GXSetTevColor(1, &local_36c);
      GXLoadPosMtxImm(&MStack_c8, 0);
      GXBegin(128, 0, 4);
      *(volatile f32*)0xCC008000 = float_neg7_80422934;
      *(volatile f32*)0xCC008000 = float_7_80422938;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_7_80422938;
      *(volatile f32*)0xCC008000 = float_7_80422938;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_1_804228cc;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_7_80422938;
      *(volatile f32*)0xCC008000 = float_neg7_80422934;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_1_804228cc;
      *(volatile f32*)0xCC008000 = float_1_804228cc;
      *(volatile f32*)0xCC008000 = float_neg7_80422934;
      *(volatile f32*)0xCC008000 = float_neg7_80422934;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_0_804228c0;
      *(volatile f32*)0xCC008000 = float_1_804228cc;
    
break;
case 6:

      uVar7 = (u32)*(u32*)(gp+0x1C) % 0xf;
      PSMTXIdentity(&MStack_128);
      GXLoadTexMtxImm(&MStack_128, 0x1e, 1);
      PSMTXTrans(&MStack_128, (double)float_0_804228c0, (double)(float)chuchu_dt[uVar7 * 3],
                      (double)float_0_804228c0);
      PSMTXScale(&MStack_f8, float_0p7_8042293c * (float)chuchu_dt[uVar7 * 3+1],
                      float_0p7_8042293c * (float)chuchu_dt[uVar7 * 3+2], float_0p7_8042293c);
      PSMTXRotRad(&MStack_158, (double)(float_deg2rad_804228c4 * *(f32*)(piVar13+9)), 'y');
      PSMTXConcat(&MStack_128, &MStack_158, &MStack_128);
      PSMTXConcat(&MStack_128, &MStack_f8, &MStack_128);
      GXSetNumTevStages(1);
      GXSetTevOrder(0, 0, 0, 0xFF);
      GXSetTevColorOp(0, 0, 0, 0, 1, 0);
      GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
      GXSetTevColorIn(0, 15, 2, 8, 15);
      GXSetTevAlphaIn(0, 7, 1, 4, 7);
      effGetTexObj(10, (u32 *)&GStack_2d8);
      GXLoadTexObj(&GStack_2d8, 0);
      if (iVar10 == 0) {
        local_378 = ((unk_804295b0 & 0xFFFFFF00) | (u8)cVar15);
        local_374 = local_378;
        GXSetTevColor(1, &local_374);
        PSMTXTransApply(&MStack_c8, &MStack_218, float_1_804228cc, float_neg1_804228f4, float_0_804228c0);
        PSMTXConcat(&MStack_218, &MStack_128, &MStack_218);
        GXLoadPosMtxImm(&MStack_218, 0);
        GXBegin(128, 0, 4);
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_16_804228e0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_8_804228fc;
        *(volatile f32*)0xCC008000 = float_16_804228e0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_8_804228fc;
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        local_380 = ((dat_804228b8 & 0xFFFFFF00) | (u8)bVar14);
        local_37c = local_380;
        GXSetTevColor(1, &local_37c);
        PSMTXConcat(&MStack_c8, &MStack_128, &MStack_218);
        GXLoadPosMtxImm(&MStack_218, 0);
        GXBegin(128, 0, 4);
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_16_804228e0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_8_804228fc;
        *(volatile f32*)0xCC008000 = float_16_804228e0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_8_804228fc;
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_neg8_80422904;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_0_804228c0;
        *(volatile f32*)0xCC008000 = float_2_804228e4;
      }
    
break;
}
  }
  return;
}

/* Exact object-owned target scalar/color catalog. */
__declspec(section ".sdata2") u32 dat_80422898 = 0xffffff00;
__declspec(section ".sdata2") u32 dat_8042289c = 0xa0a0a000;
__declspec(section ".sdata2") u32 dat_804228a0 = 0xff000000;
__declspec(section ".sdata2") u32 dat_804228a4 = 0xa0000000;
__declspec(section ".sdata2") u32 dat_804228a8 = 0xe0e0e000;
__declspec(section ".sdata2") u32 dat_804228ac = 0xffffff00;
__declspec(section ".sdata2") u32 dat_804228b0 = 0xffffff00;
__declspec(section ".sdata2") u32 dat_804228b4 = 0xa0a0a000;
__declspec(section ".sdata2") u32 dat_804228b8 = 0xffcd2800;
__declspec(section ".sdata2") u32 dat_804228bc = 0x7f661400;
__declspec(section ".sdata2") const f32 float_0_804228c0 = 0.0f;
__declspec(section ".sdata2") f32 float_deg2rad_804228c4 = 0.01745329238474369f;
__declspec(section ".sdata2") f32 float_0p8_804228c8 = 0.800000011920929f;
__declspec(section ".sdata2") f32 float_1_804228cc = 1.0f;
__declspec(section ".sdata2") f32 float_1p3_804228d0 = 1.2999999523162842f;
__declspec(section ".sdata2") f32 float_neg1p3_804228d4 = -1.2999999523162842f;
__declspec(section ".sdata2") f32 float_neg16_804228d8 = -16.0f;
__declspec(section ".sdata2") f32 float_13_804228dc = 13.0f;
__declspec(section ".sdata2") f32 float_16_804228e0 = 16.0f;
__declspec(section ".sdata2") f32 float_2_804228e4 = 2.0f;
__declspec(section ".sdata2") f32 float_neg19_804228e8 = -19.0f;
__declspec(section ".sdata2") f32 float_0p16667_804228ec = 0.1666666716337204f;
__declspec(section ".sdata2") f32 float_0p25_804228f0 = 0.25f;
__declspec(section ".sdata2") f32 float_neg1_804228f4 = -1.0f;
__declspec(section ".sdata2") f32 float_neg4_804228f8 = -4.0f;
__declspec(section ".sdata2") f32 float_8_804228fc = 8.0f;
__declspec(section ".sdata2") f32 float_4_80422900 = 4.0f;
__declspec(section ".sdata2") f32 float_neg8_80422904 = -8.0f;
__declspec(section ".sdata2") f32 float_10_80422908 = 10.0f;
__declspec(section ".sdata2") f32 float_5_8042290c = 5.0f;
__declspec(section ".sdata2") f32 float_0p43633_80422910 = 0.4363323152065277f;
__declspec(section ".sdata2") f32 float_0p15_80422914 = 0.15000000596046448f;
__declspec(section ".sdata2") const u8 str_PCTd_80422918[3] = {0x25,0x64,0x00};
__declspec(section ".sdata2") f32 float_0p5_8042291c = 0.5f;
__declspec(section ".sdata2") f32 float_neg10_80422920 = -10.0f;
__declspec(section ".sdata2") f32 float_22_80422924 = 22.0f;
__declspec(section ".sdata2") f32 float_neg5_80422928 = -5.0f;
__declspec(section ".sdata2") f32 float_neg3_8042292c = -3.0f;
__declspec(section ".sdata2") f32 float_3_80422930 = 3.0f;
__declspec(section ".sdata2") f32 float_neg7_80422934 = -7.0f;
__declspec(section ".sdata2") f32 float_7_80422938 = 7.0f;
__declspec(section ".sdata2") f32 float_0p7_8042293c = 0.699999988079071f;
__declspec(section ".sdata2") f32 float_22p5_80422940 = 22.5f;
__declspec(section ".sdata2") f32 float_9_80422944 = 9.0f;
__declspec(section ".sdata2") f32 float_neg90_80422948 = -90.0f;
__declspec(section ".sdata2") f32 float_neg450_8042294c = -450.0f;
__declspec(section ".sdata2") f32 float_20_80422950 = 20.0f;
__declspec(section ".sdata2") f32 float_12_80422954 = 12.0f;


#undef FUKIDASHI_UNSIGNED_FLOAT
