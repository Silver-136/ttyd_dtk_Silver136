#include "effect/eff_number.h"


/* CHATGPT STUB FILL: main/effect/eff_number 20260624_184929 */

/* stub-fill: effNumberGX | missing_definition | ghidra_signature */
void effNumberGX(f32 matrix[3][4], s32 type, s32 value, u8 alpha, s32 layers) {
    typedef u32 GXTexObj[8];
    extern void* camGetCurPtr(void);
    extern void effGetTexObj(s32, GXTexObj);
    extern void GXLoadTexObj(GXTexObj, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32, u32*);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXSetTevSwapMode(s32,s32,s32);
    extern void GXSetTevSwapModeTable(s32,s32,s32,s32,s32);
    extern void PSMTXTrans(f32[3][4],f32,f32,f32);
    extern void PSMTXScale(f32[3][4],f32,f32,f32);
    extern void PSMTXConcat(f32[3][4],f32[3][4],f32[3][4]);
    extern void GXLoadPosMtxImm(f32[3][4],s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(f32[3][4],s32,s32);
    extern void GXBegin(s32,s32,s32);
    extern s32 sprintf(char*,const char*,...);
    extern u32 strlen(const char*);
    extern u32 dat_80428900, dat_80428904, dat_80428908, dat_8042890c;
    extern u32 dat_80428910, dat_80428914, dat_80428918, dat_8042891c;
    extern u32 float_0p011489_80428920;
    char text[32];
    GXTexObj tex;
    f32 scale[3][4];
    f32 trans[3][4];
    u32 color0;
    u32 color1;
    u32 tevColor;
    void* camera;
    u32 length;
    s32 layer;
    s32 i;
    s32 layerOffset;
    volatile f32* fifoF = (volatile f32*)0xCC008000;
    volatile u8* fifo8 = (volatile u8*)0xCC008000;

    camera = camGetCurPtr();
    switch (type) {
        case 0: color0=dat_80428900; color1=dat_80428904; break;
        case 1: color0=dat_80428908; color1=dat_8042890c; break;
        case 2: color0=dat_80428910; color1=dat_80428914; break;
        default: color0=dat_80428918; color1=dat_8042891c; break;
    }
    ((u8*)&color0)[3] = alpha;
    ((u8*)&color1)[3] = alpha;

    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,1,0,0,2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9,1);
    GXSetVtxDesc(0xB,1);
    GXSetVtxDesc(0xD,1);
    GXSetVtxAttrFmt(0,9,1,4,0);
    GXSetVtxAttrFmt(0,0xB,1,5,0);
    GXSetVtxAttrFmt(0,0xD,1,4,0);

    sprintf(text,"%d",value);
    length = strlen(text);
    layerOffset = 0;
    for (layer=0; layer<layers+1; layer++) {
        if (layer == 0) {
            effGetTexObj(1,tex); GXLoadTexObj(tex,0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0,0,0,4);
            GXSetTevColorOp(0,0,0,0,1,0);
            GXSetTevAlphaOp(0,0,0,0,1,0);
            GXSetTevColorIn(0,15,15,15,15);
            GXSetTevAlphaIn(0,7,5,4,7);
            GXSetTevSwapMode(0,0,0);
        } else {
            effGetTexObj(0x6C,tex); GXLoadTexObj(tex,0);
            GXSetNumTevStages(3);
            GXSetTevOrder(0,0,0,0xFF);
            GXSetTevColorOp(0,14,0,0,1,0);
            GXSetTevAlphaOp(0,14,0,0,1,0);
            GXSetTevColorIn(0,8,15,8,15);
            GXSetTevAlphaIn(0,4,7,4,7);
            GXSetTevSwapModeTable(1,0,1,2,0);
            GXSetTevSwapMode(0,1,1);
            GXSetTevOrder(1,0xFF,0xFF,4);
            GXSetTevColorOp(1,0,0,0,1,0);
            GXSetTevAlphaOp(1,0,0,0,1,0);
            GXSetTevColorIn(1,15,0,10,15);
            GXSetTevAlphaIn(1,7,7,7,0);
            GXSetTevSwapMode(1,0,0);
            GXSetTevOrder(2,0,0,4);
            GXSetTevColorOp(2,0,0,0,1,0);
            GXSetTevAlphaOp(2,0,0,0,1,0);
            GXSetTevColorIn(2,2,0,1,15);
            GXSetTevAlphaIn(2,7,4,5,7);
            tevColor = float_0p011489_80428920;
            GXSetTevColor(1,&tevColor);
            GXSetTevSwapMode(2,0,0);
        }
        for (i=0; i<(s32)length; i++) {
            s32 index = (length-1)-i;
            PSMTXTrans(trans,
                       32.0f*(f32)((s32)length/2-i)-(f32)layerOffset,
                       (f32)layerOffset,0.0f);
            PSMTXConcat(matrix,trans,trans);
            PSMTXConcat((f32 (*)[4])((u8*)camera+0x11C),trans,trans);
            GXLoadPosMtxImm(trans,0);
            GXSetCurrentMtx(0);
            PSMTXScale(scale,1.0f/11.0f,1.0f,1.0f);
            PSMTXTrans(trans,text[index]=='-' ? 10.0f : (f32)(text[index]-'0'),0.0f,0.0f);
            PSMTXConcat(scale,trans,trans);
            GXLoadTexMtxImm(trans,0x1E,1);
            GXBegin(0x80,0,4);
            *fifoF=-24.0f; *fifoF=24.0f; *fifoF=0.0f;
            *fifo8=((u8*)&color0)[0]; *fifo8=((u8*)&color0)[1]; *fifo8=((u8*)&color0)[2]; *fifo8=((u8*)&color0)[3];
            *fifoF=0.0f; *fifoF=0.0f;
            *fifoF=24.0f; *fifoF=24.0f; *fifoF=0.0f;
            *fifo8=((u8*)&color0)[0]; *fifo8=((u8*)&color0)[1]; *fifo8=((u8*)&color0)[2]; *fifo8=((u8*)&color0)[3];
            *fifoF=1.0f; *fifoF=0.0f;
            *fifoF=24.0f; *fifoF=-24.0f; *fifoF=0.0f;
            *fifo8=((u8*)&color1)[0]; *fifo8=((u8*)&color1)[1]; *fifo8=((u8*)&color1)[2]; *fifo8=((u8*)&color1)[3];
            *fifoF=1.0f; *fifoF=1.0f;
            *fifoF=-24.0f; *fifoF=-24.0f; *fifoF=0.0f;
            *fifo8=((u8*)&color1)[0]; *fifo8=((u8*)&color1)[1]; *fifo8=((u8*)&color1)[2]; *fifo8=((u8*)&color1)[3];
            *fifoF=0.0f; *fifoF=1.0f;
        }
        layerOffset += 2;
    }
    GXSetTevSwapModeTable(1,0,1,2,3);
    GXSetTevSwapMode(0,0,0);
}

