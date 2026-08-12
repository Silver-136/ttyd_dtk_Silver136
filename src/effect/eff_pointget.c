#include "effect/eff_pointget.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effPointGetEntry(s32 kind, f32 x, f32 y, f32 z, s32 arg4, s32 arg5) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effPointGetMain(void);
    extern const char str_PointGet_802fed08[];
    extern f32 float_2000_8042743c;
    extern f32 float_30_80427440;
    extern f32 float_1_80427430;

    void* entry = effEntry();
    void* work;
    f32 scale;
    f32 posY;
    f32 posX;
    u32 flags;
    s32 zero;

    *(const char**)((s32)entry + 0x14) = str_PointGet_802fed08;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x24);
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = (s32)effPointGetMain;
    zero = 0;
    posX = x - float_2000_8042743c;
    flags = *(u32*)entry | 2;
    posY = y - float_30_80427440;
    scale = float_1_80427430;
    *(u32*)entry = flags;
    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = posX;
    *(f32*)((s32)work + 8) = posY;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x10) = arg4;
    *(f32*)((s32)work + 0x14) = scale;
    *(s32*)((s32)work + 0x18) = arg5;
    *(s32*)((s32)work + 0x1C) = zero;
    *(s32*)((s32)work + 0x20) = zero;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* stub-fill: effPointGetMain | prototype_only | source_prototype */
void effPointGetMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 frame, s32 maxFrame);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* data, f32 z);
    extern void effDelete(void* effect);
    extern void effPointGetDisp(s32 cameraId, void* effect);
    extern VecLocal vec3_802fece8;
    extern f32 float_neg1000_80427434;
    extern f32 float_0_80427414;
    extern f32 float_1000_80427438;

    void* work = *(void**)((s32)effect + 0xC);
    VecLocal pos = vec3_802fece8;
    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);

    *(s32*)((s32)work + 0x1C) += 1;
    switch (*(s32*)((s32)work + 0x20)) {
        case 0:
            *(f32*)((s32)work + 4) = intplGetValue(0xB, float_neg1000_80427434, float_0_80427414,
                                                    *(s32*)((s32)work + 0x1C), 0x2D);
            if (*(s32*)((s32)work + 0x1C) >= 0x2D) {
                *(s32*)((s32)work + 0x1C) = 0;
                *(s32*)((s32)work + 0x20) += 1;
            }
            break;
        case 1:
            if (*(s32*)((s32)work + 0x18) == 0) {
                if ((*(u32*)effect & 4) != 0) {
                    *(u32*)effect &= ~4U;
                    *(s32*)((s32)work + 0x1C) = 0;
                    *(s32*)((s32)work + 0x20) += 1;
                }
            } else if (*(s32*)((s32)work + 0x1C) >= *(s32*)((s32)work + 0x18)) {
                *(s32*)((s32)work + 0x1C) = 0;
                *(s32*)((s32)work + 0x20) += 1;
            }
            break;
        case 2:
            *(f32*)((s32)work + 4) = intplGetValue(0xB, float_0_80427414, float_1000_80427438,
                                                    *(s32*)((s32)work + 0x1C), 0x3C);
            if (*(s32*)((s32)work + 0x1C) >= 0x3C) {
                effDelete(effect);
                return;
            }
            break;
    }

    dispEntry(8, 2, effPointGetDisp, effect, dispCalcZ(&pos));
}

/* CHATGPT STUB FILL: main/effect/eff_pointget 20260624_184823 */

/* stub-fill: effPointGetDisp | missing_definition | ghidra_signature */
void effPointGetDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct Color { u8 r, g, b, a; } Color;
    typedef u8 TexObj[0x20];
    typedef struct TexEntry { s32 type; s32 texId; s32 shadowTexId; } TexEntry;
    typedef struct TexList { TexEntry* list[2]; s32 count; } TexList;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXIdentity(Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern u16 GXGetTexObjWidth(void*);
    extern u16 GXGetTexObjHeight(void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, Color*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern volatile f32 DAT_cc008000;
    extern void sprintf(char*, const char*, ...);
    extern u32 strlen(const char*);
    extern char str_PCTd_80427410[];
    extern void* gpGlobals;
    extern TexList texid_tbl[];
    extern u32 dat_80427408;
    extern u32 dat_8042740c;
    extern f32 float_0_80427414;
    extern f32 float_10_80427418;
    extern f32 float_0p5_8042741c;
    extern f32 float_5_80427420;
    extern f32 float_200_80427424;
    extern f32 float_1p4286_80427428;
    extern f32 float_0p1_8042742c;
    extern f32 float_1_80427430;
    u8* work;
    void* camera;
    Mtx model;
    Mtx trans;
    Mtx scaleMtx;
    TexObj texObj;
    Color color;
    Color colorCopy;
    char digits[32];
    TexEntry* list;
    TexEntry* cur;
    s32 language;
    s32 count;
    s32 kind;
    s32 selector;
    s32 pass;
    s32 i;
    s32 texId;
    f32 scale;
    f32 total;
    f32 cursor;
    f32 yOffset;
    f32 width;
    f32 height;
    f32 left;
    f32 right;
    char* digit;

    work = *(u8**)((u8*)effect + 0xC);
    camera = camGetPtr(cameraId);
    kind = *(s32*)(work + 0x10);
    scale = *(f32*)(work + 0x14);
    language = *(s32*)((u8*)gpGlobals + 0x16C);
    selector = ((((kind ^ 1) >> 1) - ((kind ^ 1) & kind)) >> 31) & 1;
    list = texid_tbl[language].list[selector];
    count = texid_tbl[language].count;
    sprintf(digits, str_PCTd_80427410, kind);

    total = 0.0f;
    cur = list;
    for (i = 0; i < count; i++, cur++) {
        if (cur->type == 0) {
            effGetTexObj(cur->texId, texObj);
            width = (f32)GXGetTexObjWidth(texObj);
            total += (f32)strlen(digits) * width * 0.05f;
        } else if (cur->type == -1) {
            total += float_5_80427420;
        } else if (cur->type == 1) {
            effGetTexObj(cur->texId, texObj);
            total += (f32)GXGetTexObjWidth(texObj) * 0.5f;
        }
    }
    if (total > float_200_80427424) {
        scale *= float_200_80427424 / total;
    }

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(trans, scaleMtx, model);
    PSMTXConcat((u8*)camera + 0x11C, model, model);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    pass = 1;
    do {
        yOffset = (f32)(pass * -4);
        if (pass == 0) {
            *(u32*)&color = dat_8042740c;
        } else {
            *(u32*)&color = dat_80427408;
        }
        colorCopy = color;
        cur = list;
        cursor = 0.0f;
        for (i = 0; i < count; i++, cur++) {
            if (cur->type == 0) {
                texId = pass ? cur->shadowTexId : cur->texId;
                effGetTexObj(texId, texObj);
                GXLoadTexObj(texObj, 0);
                effGetTexObj(cur->texId, texObj);
                GXSetChanMatColor(4, &colorCopy);
                digit = digits;
                while ((u32)(digit - digits) < strlen(digits)) {
                    cursor += ((f32)GXGetTexObjWidth(texObj) / float_10_80427418) * 0.5f;
                    PSMTXTrans(trans, (f32)(pass * 4) + (cursor - total), yOffset, 0.0);
                    PSMTXScale(scaleMtx, float_1p4286_80427428, float_1p4286_80427428, float_1p4286_80427428);
                    PSMTXConcat(model, trans, trans);
                    PSMTXConcat(trans, scaleMtx, trans);
                    GXLoadPosMtxImm(trans, 0);
                    GXSetCurrentMtx(0);
                    PSMTXScale(scaleMtx, float_0p1_8042742c, 1.0f, 1.0f);
                    PSMTXTrans(trans, (f32)(*digit - '0'), 0.0, 0.0);
                    PSMTXConcat(scaleMtx, trans, trans);
                    GXLoadTexMtxImm(trans, 0x1E, 1);
                    GXBegin(0x80, 0, 4);
                    width = (f32)GXGetTexObjWidth(texObj) / float_10_80427418;
                    height = (f32)GXGetTexObjHeight(texObj);
                    left = -width * 0.5f;
                    right = width * 0.5f;
                    DAT_cc008000 = left; DAT_cc008000 = height; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f;
                    DAT_cc008000 = right; DAT_cc008000 = height; DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f; DAT_cc008000 = 0.0f;
                    DAT_cc008000 = right; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f; DAT_cc008000 = 1.0f;
                    DAT_cc008000 = left; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f;
                    digit++;
                    cursor += ((f32)GXGetTexObjWidth(texObj) / float_10_80427418) * 0.5f;
                }
            } else if (cur->type == -1) {
                cursor += float_10_80427418;
            } else if (cur->type == 1) {
                texId = pass ? cur->shadowTexId : cur->texId;
                effGetTexObj(texId, texObj);
                GXLoadTexObj(texObj, 0);
                effGetTexObj(cur->texId, texObj);
                GXSetChanMatColor(4, &colorCopy);
                cursor += (f32)GXGetTexObjWidth(texObj) * 0.5f;
                PSMTXTrans(trans, (f32)(pass * 4) + (cursor - total), yOffset, 0.0);
                PSMTXConcat(model, trans, trans);
                GXLoadPosMtxImm(trans, 0);
                GXSetCurrentMtx(0);
                PSMTXIdentity(trans);
                GXLoadTexMtxImm(trans, 0x1E, 1);
                GXBegin(0x80, 0, 4);
                width = (f32)GXGetTexObjWidth(texObj);
                height = (f32)GXGetTexObjHeight(texObj);
                left = -width * 0.5f;
                right = width * 0.5f;
                DAT_cc008000 = left; DAT_cc008000 = height; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f;
                DAT_cc008000 = right; DAT_cc008000 = height; DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f; DAT_cc008000 = 0.0f;
                DAT_cc008000 = right; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f; DAT_cc008000 = 1.0f;
                DAT_cc008000 = left; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 0.0f; DAT_cc008000 = 1.0f;
                cursor += (f32)GXGetTexObjWidth(texObj) * 0.5f;
            }
        }
        pass--;
    } while (pass > -1);
}

