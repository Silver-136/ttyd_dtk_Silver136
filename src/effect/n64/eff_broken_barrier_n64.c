#include "effect/n64/eff_broken_barrier_n64.h"


void effBrokenBarrierMain(void* effect) {
    extern void effDelete(void*);
    extern s32 rand(void);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effBrokenBarrierDisp(s32,void*);
    u8* entry=effect; u8* work=*(u8**)(entry+0xC); u8* part;
    f32 pos[3]; s32 type=*(s32*)work, timer, i;
    pos[0]=*(f32*)(work+4); pos[1]=*(f32*)(work+8); pos[2]=*(f32*)(work+0xC);
    if((*(u32*)entry&4)!=0){*(u32*)entry&=~4;*(s32*)(work+0x38)=16;}
    if(*(s32*)(work+0x38)<1000)(*(s32*)(work+0x38))--;
    (*(s32*)(work+0x3C))++; timer=*(s32*)(work+0x38);
    if(timer<0){effDelete(effect);return;}
    if(timer<8)*(s32*)(work+0x4C)=timer<<5;
    part=work+0x6C;
    for(i=1;i<*(s32*)(entry+8);i++,part+=0x6C){
        s32 frame=++*(s32*)(part+0x30);
        if(frame>19){frame=0;*(s32*)(part+0x30)=0;}
        if(frame==0){
            *(f32*)(part+4)=(f32)((rand()%360)-180);
            *(f32*)(part+8)=(f32)((rand()%360)-180);
            *(f32*)(part+0xC)=(f32)((rand()%360)-180);
            *(f32*)(part+0x18)=0.3f**(f32*)(part+4);
            *(f32*)(part+0x1C)=0.3f**(f32*)(part+8);
            *(f32*)(part+0x20)=0.3f**(f32*)(part+0xC);
            *(f32*)(part+0x34)=(f32)(rand()%15);
            *(f32*)(part+0x38)=(f32)(rand()%15);
            *(s32*)(part+0x40)=0xFF;
        }
        *(f32*)(part+4)+=*(f32*)(part+0x18);
        *(f32*)(part+8)+=*(f32*)(part+0x1C);
        *(f32*)(part+0xC)+=*(f32*)(part+0x20);
        *(f32*)(part+0x34)+=0.2f; *(f32*)(part+0x38)+=0.6f;
        *(f32*)(part+0x18)+=0.04f*(*(f32*)(part+0x24)-*(f32*)(part+0x18));
        *(f32*)(part+0x1C)+=0.04f*(*(f32*)(part+0x28)-*(f32*)(part+0x1C));
        *(f32*)(part+0x20)+=0.04f*(*(f32*)(part+0x2C)-*(f32*)(part+0x20));
        if(type==2||type==3)*(f32*)(part+0x18)*=0.95f;
    }
    dispEntry(4,2,effBrokenBarrierDisp,effect,dispCalcZ(pos));
}

void* effBrokenBarrierN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBrokenBarrierMain(void);
    extern char str_BrokenBarrierN64_802fac88[];
    extern f32 float_30_80424e80;
    extern f32 float_0p33333_80424e84;
    extern f32 float_60_80424e88;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 j;
    f32 step;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BrokenBarrierN64_802fac88;
    *(s32*)((s32)entry + 8) = 0x3D;
    work = __memAlloc(3, 0x19BC);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBrokenBarrierMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x3C) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x38) = 1000;
    } else {
        *(s32*)(work + 0x38) = timer;
    }

    *(s32*)(work + 0x4C) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x60) = scale;
    *(f32*)(work + 0x10) = float_30_80424e80;
    *(f32*)(work + 0x14) = float_30_80424e80;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x50) = 0xD7;
    *(s32*)(work + 0x54) = 0xFF;
    *(s32*)(work + 0x58) = 0xEB;
    *(s32*)(work + 0x5C) = 0xFF;

    if (timer < 20) {
        step = float_0p33333_80424e84;
    } else {
        step = (f32)timer / float_60_80424e88;
    }

    part = work + 0x6C;
    for (i = 0; i < 12; i++) {
        for (j = 1; j < 6; j++) {
            *(s32*)(part + 0x30 + (j - 1) * 0x6C) = (s32)(-(f32)j * step) - 1;
        }
        part += 0x21C;
    }

    return entry;
}

u8 effBrokenBarrierDisp(int param_1, int param_2) {
    return 0;
}


u8 main_dl(void) {
    return 0;
}
