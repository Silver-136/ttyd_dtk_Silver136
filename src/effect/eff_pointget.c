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
    typedef f32 Mtx[3][4]; typedef struct Color {u8 r,g,b,a;} Color; typedef u8 TexObj[0x20];
    extern void* camGetPtr(s32); extern void PSMTXTrans(Mtx,f64,f64,f64); extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*); extern void PSMTXIdentity(Mtx); extern void effGetTexObj(s32,void*);
    extern void GXLoadTexObj(void*,s32); extern u16 GXGetTexObjWidth(void*); extern u16 GXGetTexObjHeight(void*);
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetChanMatColor(s32,Color*);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevOp(s32,s32); extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32); extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetCurrentMtx(s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXBegin(s32,s32,s32);
    extern volatile f32 DAT_cc008000; extern void sprintf(char*,const char*,...); extern char str_PCTd_80427410[];
    u8* work=*(u8**)((u8*)effect+0xC); void* cam=camGetPtr(cameraId); Mtx model,trans,scale,texMtx; TexObj tex;
    Color color; char digits[32]; f32 total=0.0f; f32 x; f32 y; f32 w; f32 h; s32 pass; s32 i;
    sprintf(digits,str_PCTd_80427410,*(s32*)(work+0x10)^1);
    for(i=0;digits[i]!=0;i++){effGetTexObj(0x40+(digits[i]-'0'),tex);total+=(f32)GXGetTexObjWidth(tex)*0.05f;}
    PSMTXTrans(trans,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));
    PSMTXScale(scale,*(f32*)(work+0x14),*(f32*)(work+0x14),*(f32*)(work+0x14)); PSMTXConcat(trans,scale,model);
    PSMTXConcat((u8*)cam+0x11C,model,model);
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,0,0,0,2); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetNumTevStages(1); GXSetTevOrder(0,0,0,4); GXSetTevOp(0,0); GXSetCullMode(0); GXClearVtxDesc();
    GXSetVtxDesc(9,1); GXSetVtxDesc(13,1); GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,13,1,4,0);
    for(pass=1;pass>=0;pass--){
        color.r=color.g=color.b=0xFF;color.a=pass?0x80:0xFF;GXSetChanMatColor(4,&color);x=(f32)(pass*4)-total;y=(f32)(pass*-4);
        for(i=0;digits[i]!=0;i++){
            effGetTexObj(0x40+(digits[i]-'0'),tex);GXLoadTexObj(tex,0);w=(f32)GXGetTexObjWidth(tex)*0.5f;h=(f32)GXGetTexObjHeight(tex);
            PSMTXTrans(trans,x+w-total,y,0.0);PSMTXConcat(model,trans,trans);GXLoadPosMtxImm(trans,0);GXSetCurrentMtx(0);
            PSMTXIdentity(texMtx);GXLoadTexMtxImm(texMtx,0x1E,1);GXBegin(0x80,0,4);
            DAT_cc008000=-w;DAT_cc008000=h;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=0;
            DAT_cc008000=w;DAT_cc008000=h;DAT_cc008000=0;DAT_cc008000=1;DAT_cc008000=0;
            DAT_cc008000=w;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=1;DAT_cc008000=1;
            DAT_cc008000=-w;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=1;
            x+=w;
        }
    }
}

