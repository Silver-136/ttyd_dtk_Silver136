#include "effect/eff_kemutest.h"

/* CHATGPT STUB FILL: main/effect/eff_kemutest 20260624_184128 */

/* stub-fill: effKemuTestEntry | missing_definition | ghidra_signature */
void* effKemuTestEntry(f64 x, f64 y, f64 z, f64 scale, u32 type) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effKemuTestMain(void*);
    extern s32 irand(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void psndSFXOn_3D(void*, Vec*);
    extern void effConfettiEntry(void);
    extern void effHitEntry(void);
    extern void* effStardust2N64Entry(f64, f64, f64, f64, f64, s32, s32, s32);
    extern char str_KemuTest_802fe670[];
    void* effect = effEntry();
    u8* work;
    u8* part;
    s32 count = 1;
    s32 i;
    f32 angle;
    f32 speed;
    Vec pos;

    switch (type) {
        case 0: case 1: case 15: case 18: count = 2; break;
        case 2: case 16: count = 5; break;
        case 3: count = 33; break;
        case 4: count = 11; break;
        case 5: count = 4; break;
        case 6: count = 7; break;
        case 7: count = 7; break;
        case 8: count = 65; break;
        case 9: count = 21; break;
        case 10: count = 7; break;
        case 11: count = 9; break;
        case 12: count = irand(3) + 4; break;
        case 13: count = 11; break;
        case 14: count = 5; break;
        case 17: count = 11; scale *= 0.5; break;
        case 19: count = 4; break;
        case 20: count = 11; scale *= 0.5; break;
        case 21: count = 3; break;
        case 22: count = 4; break;
    }
    *(char**)effect = str_KemuTest_802fe670;
    *(s32*)((u8*)effect + 8) = count;
    work = __memAlloc(3, count * 0x58);
    *(u8**)((u8*)effect + 0xC) = work;
    *(void**)((u8*)effect + 0x10) = effKemuTestMain;
    *(u32*)work = type;
    *(f32*)(work + 4) = (f32)x;
    *(f32*)(work + 8) = (f32)y;
    *(f32*)(work + 0xC) = (f32)z;
    *(f32*)(work + 0x48) = (f32)scale;
    *(s32*)(work + 0x1C) = 4;
    *(s32*)(work + 0x28) = 0;
    *(s32*)(work + 0x44) = 0;
    *(f32*)(work + 0x50) = 0.0f;
    *(f32*)(work + 0x54) = 0.0f;

    part = work + 0x58;
    for (i = 1; i < count; i++, part += 0x58) {
        *(f32*)(part + 4) = 0.0f;
        *(f32*)(part + 8) = 0.0f;
        *(f32*)(part + 0xC) = 0.0f;
        *(s32*)(part + 0x28) = 0;
        *(s32*)(part + 0x30) = 32;
        *(s32*)(part + 0x44) = 0;
        *(u8*)(part + 0x4C) = 255;
        *(u8*)(part + 0x4D) = 255;
        *(u8*)(part + 0x4E) = 255;
        *(u8*)(part + 0x4F) = 255;
        *(f32*)(part + 0x50) = 1.0f;
        if (type == 0 || type == 15 || type == 18) {
            *(f32*)(part + 4) = 0.0f;
            *(f32*)(part + 8) = 0.0f;
            *(f32*)(part + 0xC) = 0.0f;
            *(s32*)(part + 0x30) = 32;
            *(f32*)(part + 0x50) = 1.0f;
            if (type == 15) {
                *(u8*)(part + 0x4C) = 0x50;
                *(u8*)(part + 0x4D) = 0x4B;
                *(u8*)(part + 0x4E) = 0x4B;
            }
        } else if (type == 1 || type == 19) {
            *(f32*)(part + 4) = 0.0f;
            *(f32*)(part + 8) = 0.0f;
            *(f32*)(part + 0xC) = 0.0f;
            *(s32*)(part + 0x30) = 128;
            *(f32*)(part + 0x34) = 0.95f;
            *(f32*)(part + 0x38) = -0.04f;
            *(f32*)(part + 0x3C) = 0.94f;
        } else if (type == 2 || type == 16) {
            angle = 6.2832f * (f32)irand(360) / 360.0f;
            *(f32*)(part + 4) = 20.0f * (f32)sin(angle);
            *(f32*)(part + 8) = 20.0f * (f32)cos(angle);
            *(f32*)(part + 0xC) = 0.0f;
            *(s32*)(part + 0x30) = 32;
            *(s32*)(part + 0x44) = (i - 1) * 6;
            *(f32*)(part + 0x50) = 1.0f;
        } else if (type == 3) {
            angle = 6.2832f * (f32)irand(360) / 360.0f;
            *(f32*)(part + 4) = 20.0f * (f32)sin(angle);
            *(f32*)(part + 8) = 20.0f * (f32)cos(angle);
            *(f32*)(part + 0xC) = 0.0f;
            *(s32*)(part + 0x30) = 32;
            *(s32*)(part + 0x44) = (i - 1) * 3;
            *(f32*)(part + 0x34) = 0.95f;
            *(f32*)(part + 0x38) = -0.04f;
            *(f32*)(part + 0x3C) = 0.94f;
            *(f32*)(part + 0x50) = 1.0f;
        } else if (type == 6 || type == 7 || type == 8) {
            angle = 6.2832f * (f32)irand(360) / 360.0f;
            speed = 1.0f + (f32)irand(40) / 10.0f;
            *(f32*)(part + 0x10) = speed * (f32)cos(angle);
            *(f32*)(part + 0x14) = speed * (f32)sin(angle);
            *(f32*)(part + 0x34) = type == 3 ? 6.0f : 1.0f;
            *(f32*)(part + 0x38) = -0.2f;
            *(f32*)(part + 0x3C) = 0.95f;
            *(s32*)(part + 0x44) = irand(16);
        } else if (type == 10 || type == 12 || type == 14) {
            angle = 6.2832f * (f32)(i - 1) / (f32)(count - 2);
            *(f32*)(part + 0x34) = 5.0f;
            *(f32*)(part + 0x38) = -0.1f;
            *(f32*)(part + 0x3C) = 0.95f;
            *(f32*)(part + 0x10) = (f32)sin(angle);
            *(f32*)(part + 0x14) = (f32)cos(angle);
            *(s32*)(part + 0x30) = 64;
        } else if (type == 11) {
            angle = 6.2832f * (f32)irand(360) / 360.0f;
            *(f32*)(part + 0x10) = 5.0f * (f32)sin(angle);
            *(f32*)(part + 0x18) = 5.0f * (f32)cos(angle);
            *(s32*)(part + 0x30) = 64;
        }
    }
    pos.x = (f32)x; pos.y = (f32)y; pos.z = (f32)z;
    if (type == 3) {
        psndSFXOn_3D((void*)"SFX_BTL_STAGE_BURST1", &pos);
        effConfettiEntry();
    } else if (type == 16) {
        effHitEntry();
    } else if (type == 20) {
        effStardust2N64Entry(x, y, z, 50.0 * scale, 50.0 * scale, 0, 8, 30);
    }
    return effect;
}



/* CHATGPT STUB FILL: main/effect/eff_kemutest 20260624_184128 */

/* stub-fill: effKemuTestMain | missing_definition | ghidra_signature */
void effKemuTestMain(void* effect)  {
    typedef struct Vec3 {
        f32 x,y,z;
    }
    Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effKemuTestDisp(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void psndSFXOn_3D(char*,Vec3*);
    extern void effConfettiEntry(void);
    extern char str_SFX_BTL_STAGE_BURST1_802fe658[];
    u8* base=*(u8**)((s32)effect+0xC);
    u8* p=base+0x58;
    Vec3 pos;
    u32 type=*(u32*)base;
    s32 camera=*(s32*)(base+0x1C);
    s32 done=0;
    s32 burst=0;
    s32 i;
    f32 angle;
    f32 decay;
    pos.x=*(f32*)(base+4);
    pos.y=*(f32*)(base+8);
    pos.z=*(f32*)(base+0xC);
    for(i=1;i<*(s32*)((s32)effect+8);i++,p+=0x58) {
        if(*(s32*)(p+0x44)>0) {
            (*(s32*)(p+0x44))--;
            continue;
        }
        *(s32*)(p+0x20)=(16 * *(s32*)(p+0x28))/ *(s32*)(p+0x30);
        *(s32*)(p+0x24)=(16*(*(s32*)(p+0x28)+1))/ *(s32*)(p+0x30);
        if(*(s32*)(p+0x24)>15)*(s32*)(p+0x24)=15;
        (*(s32*)(p+0x28))++;
        decay=0.9f;
        switch(type) {
            case 1:case 3:case 19:*(f32*)(p+8)+=*(f32*)(p+0x34);
            *(f32*)(p+0x34)+=*(f32*)(p+0x38);
            *(f32*)(p+0x38)*=*(f32*)(p+0x3C);
            break;
            case 4:case 5:case 7:case 8:case 9:case 16:case 17:case 20:case 21:*(f32*)(p+4)+=*(f32*)(p+0x10);
            *(f32*)(p+8)+=*(f32*)(p+0x14);
            *(f32*)(p+0x10)*=decay;
            *(f32*)(p+0x14)*=decay;
            break;
            case 6:*(f32*)(p+4)+=*(f32*)(p+0x10);
            *(f32*)(p+0x10)*=0.95f;
            *(f32*)(p+8)+=*(f32*)(p+0x34);
            *(f32*)(p+0x34)+=*(f32*)(p+0x38);
            *(f32*)(p+0x38)*=*(f32*)(p+0x3C);
            break;
            case 10:case 14:angle=(6.2832f*(90.0f+180.0f*(f32)(i-1)/(f32)(*(s32*)((s32)effect+8)-2)))/360.0f;
            *(f32*)(p+4)+=*(f32*)(p+0x34)*(f32)sin(angle);
            *(f32*)(p+8)-=*(f32*)(p+0x34)*(f32)cos(angle);
            *(f32*)(p+0x34)+=*(f32*)(p+0x38);
            *(f32*)(p+0x38)*=*(f32*)(p+0x3C);
            break;
            case 11:*(f32*)(p+4)+=*(f32*)(p+0x10);
            *(f32*)(p+8)+=*(f32*)(p+0x14);
            *(f32*)(p+0xC)+=*(f32*)(p+0x18);
            *(f32*)(p+0x10)*=decay;
            *(f32*)(p+0x14)*=decay;
            *(f32*)(p+0x18)*=decay;
            break;
            case 12:angle=(6.2832f*(90.0f+360.0f*(f32)(i-1)/(f32)(*(s32*)((s32)effect+8)-2)))/360.0f;
            *(f32*)(p+4)+=*(f32*)(p+0x34)*(f32)sin(angle);
            *(f32*)(p+8)-=*(f32*)(p+0x34)*(f32)cos(angle);
            *(f32*)(p+0x34)+=*(f32*)(p+0x38);
            *(f32*)(p+0x38)*=*(f32*)(p+0x3C);
            *(f32*)(p+0x40)+=0.05f;
            break;
            case 13:*(f32*)(p+4)+=*(f32*)(p+0x10);
            *(f32*)(p+8)+=*(f32*)(p+0x14)+*(f32*)(p+0x34);
            *(f32*)(p+0x10)*=decay;
            *(f32*)(p+0x14)*=decay;
            *(f32*)(p+0x34)+=*(f32*)(p+0x38);
            *(f32*)(p+0x38)*=*(f32*)(p+0x3C);
            break;
        }
        if(type==3||type==6||type==11||type==16||type==18) {
            s32 b=*(s32*)(p+0x28)*-60+255;
            s32 g=255,sr=255;
            if(b<0) {
                g=(s32)(0.8f*(f32)b)+255;
                b=0;
                if(g<0) {
                    sr=(s32)(0.4f*(f32)g)+255;
                    g=0;
                    if(sr<0)sr=0;
                }
            }
            *(u8*)(p+0x4C)=(u8)sr;
            *(u8*)(p+0x4D)=(u8)g;
            *(u8*)(p+0x4E)=(u8)b;
            if(type==3&&i==1&&*(s32*)(p+0x28)==30)burst=1;
        } else if(type==7) {
            *(u8*)(p+0x4C)+=(s8)((189-*(u8*)(p+0x4C))/10);
            *(u8*)(p+0x4D)+=(s8)((16-*(u8*)(p+0x4D))/10);
            *(u8*)(p+0x4E)+=(s8)((170-*(u8*)(p+0x4E))/10);
        } else if(type==12) {
            *(u8*)(p+0x4C)+=(s8)((130-*(u8*)(p+0x4C))/64);
            *(u8*)(p+0x4D)+=(s8)((130-*(u8*)(p+0x4D))/64);
            *(u8*)(p+0x4E)+=(s8)((130-*(u8*)(p+0x4E))/64);
        }
        if(*(s32*)(p+0x28)>=*(s32*)(p+0x30))done++;
    }
    if(burst) {
        psndSFXOn_3D(str_SFX_BTL_STAGE_BURST1_802fe658,&pos);
        effConfettiEntry();
    }
    if(done<*(s32*)((s32)effect+8)-1)dispEntry(camera,2,effKemuTestDisp,effect,dispCalcZ(&pos));
    else effDelete(effect);
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_kemutest 20260624_191429 */

/* fallback stub-fill: map=effKemuTestDisp addr=0x8020cf24 size=0x00000730 */
void effKemuTestDisp(s32 cameraId, void* effect)  {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,s32,f32);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void effGetTexObj(s32,void*);
    extern void GXLoadTexObj(void*,s32);
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
    extern void GXSetTevColor(s32,void*);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetChanMatColor(s32,void*);
    extern void GXBegin(s32,s32,s32);
    u8* base=*(u8**)((s32)effect+0xC);
    u8* p=base+0x58;
    void* cam=camGetPtr(cameraId);
    f32 root[3][4],m[3][4],r[3][4],s[3][4];
    u8 texObj[0x20];
    u32 type=*(u32*)base;
    u32 color;
    volatile f32* fifo=(volatile f32*)0xCC008000;
    s32 start=0;
    s32 i,pass;
    f32 left=-16.0f,right=16.0f;
    PSMTXTrans(m,*(f32*)(base+4),*(f32*)(base+8),*(f32*)(base+0xC));
    if(type==11) {
        PSMTXRotRad(r,0x78,0.017453292f * *(f32*)(base+0x50));
        PSMTXRotRad(s,0x7A,0.017453292f * *(f32*)(base+0x54));
        PSMTXConcat(r,s,r);
    } else PSMTXRotRad(r,0x79,0.017453292f*-*(f32*)((u8*)camGetPtr(cameraId)+0x114));
    PSMTXScale(s,*(f32*)(base+0x40),*(f32*)(base+0x40),*(f32*)(base+0x40));
    PSMTXConcat(m,r,m);
    PSMTXConcat(m,s,m);
    PSMTXConcat((u8*)cam+0x11C,m,root);
    effGetTexObj(0x28,texObj);
    GXLoadTexObj(texObj,0);
    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,0,0,0,2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,0xFF);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,0,0,0,8);
    GXSetTevAlphaIn(0,0,0,0,4);
    GXSetTevOrder(1,1,0,0xFF);
    GXSetTevColorOp(1,0,0,0,1,0);
    GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,0,8,2,0);
    GXSetTevAlphaIn(1,6,4,7,7);
    color=0xFFFFFFFF;
    GXSetTevColor(1,&color);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9,1);
    GXSetVtxDesc(13,1);
    GXSetVtxAttrFmt(0,9,1,4,0);
    GXSetVtxAttrFmt(0,13,1,4,0);
    if(type==1||type==10||type==12)start=1;
    for(i=1;i<*(s32*)((s32)effect+8);i++,p+=0x58) {
        if(*(s32*)(p+0x44)!=0||*(s32*)(p+0x20)>=16)continue;
        PSMTXTrans(m,0.0625f*(f32)*(s32*)(p+0x20),0.0f,0.0f);
        PSMTXScale(s,0.0625f,1.0f,1.0f);
        PSMTXConcat(m,s,m);
        GXLoadTexMtxImm(m,0x1E,1);
        PSMTXTrans(m,0.0625f*(f32)*(s32*)(p+0x24),0.0f,0.0f);
        PSMTXConcat(m,s,m);
        GXLoadTexMtxImm(m,0x21,1);
        for(pass=start;pass<2;pass++) {
            if(pass==0) {
                color=0xFFFFFFC8;
                PSMTXTrans(m,*(f32*)(p+4)+2.0f,*(f32*)(p+8)-2.0f,*(f32*)(p+0xC));
            } else {
                color=*(u32*)(p+0x4C);
                PSMTXTrans(m,*(f32*)(p+4),*(f32*)(p+8),*(f32*)(p+0xC));
            }
            GXSetChanMatColor(4,&color);
            PSMTXScale(s,*(f32*)(p+0x40),*(f32*)(p+0x40),*(f32*)(p+0x40));
            PSMTXConcat(root,m,m);
            PSMTXConcat(m,s,m);
            GXLoadPosMtxImm(m,0);
            GXBegin(0x80,0,4);
            *fifo=left;
            *fifo=16.0f;
            *fifo=0.0f;
            *fifo=0.0f;
            *fifo=0.0f;
            *fifo=16.0f;
            *fifo=16.0f;
            *fifo=0.0f;
            *fifo=1.0f;
            *fifo=0.0f;
            *fifo=16.0f;
            *fifo=left;
            *fifo=0.0f;
            *fifo=1.0f;
            *fifo=1.0f;
            *fifo=left;
            *fifo=left;
            *fifo=0.0f;
            *fifo=0.0f;
            *fifo=1.0f;
        }
    }
}

void effKemuTestDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x1C) = camId;
}

void effKemuTestSetRxRz(void* effect, f32 rx, f32 rz) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x50) = rx;
    *(f32*)((s32)work + 0x54) = rz;
}

