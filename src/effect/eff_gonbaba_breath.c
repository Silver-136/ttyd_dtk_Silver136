#include "effect/eff_gonbaba_breath.h"


/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: effGonbabaBreathDisp | missing_definition | ghidra_signature */
void effGonbabaBreathDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 float_deg2rad_80428a9c;
    extern u8 col_tbl[];

    u8 texObj[0x20];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    f32 baseMtx[3][4];
    u8* work = *(u8**)((u8*)effect + 0xC);
    u8* child = work + 0x50;
    void* camera = camGetPtr(cameraId);
    s32 type = *(s32*)work;
    s32 baseAlpha = *(s32*)(work + 0x3C);
    s32 colorSet = type % 10;
    s32 i;

    if (type < 10) {
        PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    } else {
        void* camera3d = camGetPtr(4);
        f32 angle = 6.2832f * *(f32*)((u8*)camera3d + 0x114) / 360.0f;
        PSMTXTrans(trans,
            *(f32*)(work + 4) - 2.0f * (f32)sin(angle),
            *(f32*)(work + 8),
            *(f32*)(work + 0xC) + 2.0f * (f32)cos(angle));
    }
    PSMTXRotRad(rot, 'y', float_deg2rad_80428a9c * (*(f32*)(work + 0x48) - *(f32*)((u8*)camera + 0x114)));
    if (*(f32*)(work + 0x48) > 90.0f && *(f32*)(work + 0x48) < 270.0f) {
        PSMTXScale(scaleMtx, *(f32*)(work + 0x40), *(f32*)(work + 0x40), -*(f32*)(work + 0x40));
    } else {
        PSMTXScale(scaleMtx, *(f32*)(work + 0x40), *(f32*)(work + 0x40), *(f32*)(work + 0x40));
    }
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), trans, baseMtx);
    effGetTexObj(0x72, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    for (i = 1; i < *(s32*)((u8*)effect + 8); i++, child += 0x50) {
        if (*(s32*)(child + 0x4C) == 0) {
            f32 childTrans[3][4];
            f32 childScale[3][4];
            volatile f32* fifo = (volatile f32*)0xCC008000;
            s32 childType = *(s32*)(child + 0x50);
            s32 alpha = (*(s32*)(child + 0x4C - 0x0C) * baseAlpha) / 0xFF;
            u32 color;

            PSMTXTrans(childTrans, *(f32*)(child + 4), *(f32*)(child + 8), *(f32*)(child + 0xC));
            PSMTXScale(childScale, *(f32*)(child + 0x40), *(f32*)(child + 0x40), *(f32*)(child + 0x40));
            PSMTXConcat(childTrans, childScale, childTrans);
            PSMTXConcat(baseMtx, childTrans, childTrans);
            GXLoadPosMtxImm(childTrans, 0);
            GXSetCurrentMtx(0);
            if (childType < 10) {
                s32 j;
                for (j = 0; j < 3; j++) {
                    f32 edge = -((4.0f * (f32)j) - 16.0f) * 0.5f;
                    color = *(u32*)(col_tbl + colorSet * 16 + j * 4);
                    ((u8*)&color)[3] = (u8)alpha;
                    GXSetChanMatColor(4, &color);
                    GXBegin(0x80, 0, 4);
                    *fifo = -edge; *fifo = edge; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
                    *fifo = edge; *fifo = edge; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
                    *fifo = edge; *fifo = -edge; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
                    *fifo = -edge; *fifo = -edge; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
                }
            } else {
                f32 halfWidth = -4.0f * 0.5f;
                ((u8*)&color)[0] = *(u8*)(child + 0x74);
                ((u8*)&color)[1] = *(u8*)(child + 0x78);
                ((u8*)&color)[2] = *(u8*)(child + 0x7C);
                ((u8*)&color)[3] = (u8)alpha;
                GXSetChanMatColor(4, &color);
                GXBegin(0x80, 0, 4);
                *fifo = halfWidth; *fifo = 2.0f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
                *fifo = 2.0f; *fifo = 2.0f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
                *fifo = 2.0f; *fifo = halfWidth; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
                *fifo = halfWidth; *fifo = halfWidth; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
            }
        }
    }
}

/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: effGonbabaBreathEntry | missing_definition | ghidra_signature */
void* effGonbabaBreathEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effGonbabaBreathMain(void* entry);
    extern void init_breath(void* work, s32 index, s32 type);
    extern char str_GonbabaBreath_80302b90[];
    extern f32 float_0_80428ab4;

    void* entry;
    void* work;
    s32 count;
    s32 i;
    void* child;
    void* childWork;
    void* grandChild;

    entry = effEntry();
    if (type == 7) {
        count = 0x11;
    } else if (type < 7) {
        if (type >= 0) {
            count = 0x21;
        }
    } else if (type < 0x12) {
        if (type >= 0xA) {
            count = 0x41;
        }
    }

    *(char**)((s32)entry + 0x14) = str_GonbabaBreath_80302b90;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x50);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effGonbabaBreathMain;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x40) = scale;
    *(s32*)((s32)work + 0x3C) = 0xFF;
    *(s32*)((s32)work + 0x20) = 0;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(f32*)((s32)work + 0x48) = float_0_80428ab4;
    *(void**)((s32)work + 0x4C) = 0;

    if (type < 10) {
        child = effGonbabaBreathEntry(type + 10, x, y, z, scale, timer);
        *(void**)((s32)work + 0x4C) = child;
        childWork = *(void**)((s32)child + 0xC);
        *(f32*)((s32)childWork + 0x48) = *(f32*)((s32)work + 0x48);
        grandChild = *(void**)((s32)childWork + 0x4C);
        if (grandChild != 0) {
            *(f32*)((s32)*(void**)((s32)grandChild + 0xC) + 0x48) = *(f32*)((s32)work + 0x48);
        }
    }

    work = (void*)((s32)work + 0x50);
    i = 1;
    while (i < count) {
        init_breath(work, i, type);
        i += 1;
        work = (void*)((s32)work + 0x50);
    }

    return entry;
}

/* CHATGPT STUB FILL: main/effect/eff_gonbaba_breath 20260624_185035 */

/* stub-fill: init_breath | missing_definition | ghidra_signature */
void init_breath(void* workPtr, s32 index, s32 type) {
    extern s32 irand(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern u8 col_tbl[];
    extern u8* PTR_col_normal_8041e0a0;
    u8* work=workPtr;
    f32 angle,speed;
    s32 colorType;

    *(s32*)work=type;
    if(type<10) {
        if(type==7) {
            *(s32*)(work+0x44)=index<<3;
            *(f32*)(work+4)=0.0f; *(f32*)(work+8)=0.0f; *(f32*)(work+0xC)=0.0f;
            *(f32*)(work+0x40)=0.0f; *(f32*)(work+0x10)=-2.0f; *(f32*)(work+0x14)=0.1f;
            *(s32*)(work+0x1C)=0x40; *(s32*)(work+0x3C)=0xFF;
            return;
        }
        if(type>=0) {
            *(s32*)(work+0x44)=index<<2;
            *(f32*)(work+4)=0.0f; *(f32*)(work+8)=0.0f; *(f32*)(work+0xC)=0.0f;
            *(f32*)(work+0x40)=0.0f; *(f32*)(work+0x10)=-2.4f; *(f32*)(work+0x14)=0.1f;
            *(s32*)(work+0x1C)=0x40; *(s32*)(work+0x3C)=0xFF;
            return;
        }
        return;
    }
    if(type==0x11) {
        angle=(225.0f+(f32)irand(0x5A))*6.2832f/360.0f;
        speed=((f32)(irand(0x32)+10)/10.0f)*1.5f;
        *(s32*)(work+0x44)=irand(100); *(s32*)(work+0x1C)=0x3C; colorType=-1;
    } else if(type<0x11) {
        angle=(240.0f+(f32)irand(0x3C))*6.2832f/360.0f;
        speed=((f32)(irand(0x32)+10)/10.0f)*1.5f;
        *(s32*)(work+0x44)=index; *(s32*)(work+0x1C)=0x1E; colorType=type%10;
    } else return;
    *(f32*)(work+4)=0.0f; *(f32*)(work+8)=0.0f; *(f32*)(work+0xC)=0.0f;
    *(f32*)(work+0x10)=speed*(f32)sin(angle); *(f32*)(work+0x14)=speed*(f32)cos(angle);
    *(f32*)(work+0x40)=1.0f; *(s32*)(work+0x3C)=0xFF;
    if(colorType<0) {
        *(s32*)(work+0x24)=PTR_col_normal_8041e0a0[0xC]; *(s32*)(work+0x28)=PTR_col_normal_8041e0a0[0xD];
        *(s32*)(work+0x2C)=PTR_col_normal_8041e0a0[0xE]; *(s32*)(work+0x30)=PTR_col_normal_8041e0a0[0];
        *(s32*)(work+0x34)=PTR_col_normal_8041e0a0[1]; *(s32*)(work+0x38)=PTR_col_normal_8041e0a0[2];
    } else {
        u8* colors=col_tbl+colorType*16;
        *(s32*)(work+0x24)=colors[0xC]; *(s32*)(work+0x28)=colors[0xD]; *(s32*)(work+0x2C)=colors[0xE];
        *(s32*)(work+0x30)=colors[0]; *(s32*)(work+0x34)=colors[1]; *(s32*)(work+0x38)=colors[2];
    }
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_gonbaba_breath 20260624_191429 */

/* fallback stub-fill: map=effGonbabaBreathMain addr=0x8025b270 size=0x00000694 */
void effGonbabaBreathMain(void* effect) {
    extern void effDelete(void*);
    extern void effSoftDelete(void*);
    extern void init_breath(void*,s32,s32);
    extern f64 intplGetValue(f64,f64,s32,s32,s32);
    extern f64 dispCalcZ(f32*);
    extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effGonbabaBreathDisp(void);
    u8* entry=effect;
    u8* base=*(u8**)(entry+0xC);
    s32 count=*(s32*)(entry+8);
    f32 position[3]={0.0f,0.0f,0.0f};
    s32 i;
    position[0]=*(f32*)(base+4); position[1]=*(f32*)(base+8); position[2]=*(f32*)(base+0xC);
    if((*(u32*)entry&4)!=0) {
        *(u32*)entry&=~4; *(s32*)(base+0x1C)=0x20;
        if(*(void**)(base+0x4C)!=0) effSoftDelete(*(void**)(base+0x4C));
    }
    if(*(s32*)(base+0x1C)<1000) --*(s32*)(base+0x1C);
    ++*(s32*)(base+0x20);
    if(*(s32*)(base+0x1C)<0x20) *(s32*)(base+0x3C)=*(s32*)(base+0x1C)<<3;
    if(*(s32*)(base+0x1C)<0) { effDelete(effect); return; }
    for(i=1;i<count;i++) {
        u8* work=base+i*0x50;
        s32 type=*(s32*)work;
        s32 duration;
        if(*(s32*)(work+0x44)!=0) { --*(s32*)(work+0x44); continue; }
        if(type<10) {
            if(type==7) {
                if(*(s32*)(work+0x1C)==0) { init_breath(work,0,type); continue; }
            } else if(type>=0 && type<7) {
                if(*(s32*)(work+0x1C)==0) { init_breath(work,i,type); continue; }
            } else continue;
            duration=0x40;
        } else if(type==0x11) {
            if(*(s32*)(work+0x1C)==0) { init_breath(work,0,type); continue; }
            duration=0x3C;
        } else if(type<0x11) {
            if(*(s32*)(work+0x1C)==0) { init_breath(work,i,type); continue; }
            duration=0x1E;
        } else continue;
        --*(s32*)(work+0x1C);
        if(*(s32*)(work+0x1C)<(type<10?0x20:0x10))
            *(s32*)(work+0x3C)=*(s32*)(work+0x1C)<<(type<10?3:4);
        if(*(f32*)(work+0x40)>=1.4f) *(f32*)(work+0x40)*=(type<10?1.05f:1.03f);
        else *(f32*)(work+0x40)+=(1.5f-*(f32*)(work+0x40))*(type<10?0.25f:0.125f);
        *(f32*)(work+0x10)*=(type<10?0.995f:0.9f);
        *(f32*)(work+0x14)*=(type<10?1.05f:0.9f);
        *(f32*)(work+4)+=*(f32*)(work+0x10);
        *(f32*)(work+8)+=*(f32*)(work+0x14);
        if(type>=10) {
            s32 elapsed=duration-*(s32*)(work+0x1C);
            *(s32*)(work+0x24)=(s32)intplGetValue((f32)*(s32*)(work+0x24),(f32)*(s32*)(work+0x30),11,elapsed,duration);
            *(s32*)(work+0x28)=(s32)intplGetValue((f32)*(s32*)(work+0x28),(f32)*(s32*)(work+0x34),11,elapsed,duration);
            *(s32*)(work+0x2C)=(s32)intplGetValue((f32)*(s32*)(work+0x2C),(f32)*(s32*)(work+0x38),11,elapsed,duration);
        }
    }
    dispEntry(4,2,effGonbabaBreathDisp,effect,(f32)dispCalcZ(position));
}

