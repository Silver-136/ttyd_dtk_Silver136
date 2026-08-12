#include "unk_0x8025DE70.h"
#include "sequence/seq_title.h"

void N_setupErrorHandler(void) {
    extern void OSSetErrorHandler(s32 error, void* handler);
    extern void N_systemErrorHandler(void);

    OSSetErrorHandler(2, N_systemErrorHandler);
    OSSetErrorHandler(3, N_systemErrorHandler);
    OSSetErrorHandler(6, N_systemErrorHandler);
}


/* CHATGPT STUB FILL: main/unk_0x8025DE70 20260624_185035 */

/* stub-fill: N_systemErrorHandler | prototype_only | source_prototype */
void N_systemErrorHandler(u16 error, u8* context, u32 dsisr, u32 dar) {
    extern char* strcpy(char*, const char*); extern char* strcat(char*, const char*);
    extern s32 sprintf(char*, const char*, ...); extern void psndExit(void);
    extern void L_OSFillFPUContext(void*); extern void L_smartReInit(void);
    extern u16 OSGetFontEncode(void); extern void* smartAlloc(u32,s32); extern void L_OSInitFont(void*);
    extern void PPCHalt(void); extern void GXSetCopyClear(void*,u32); extern void* L_DEMOGetCurrentBuffer(void);
    extern void GXCopyDisp(void*,s32); extern void GXSetViewport(f32,f32,f32,f32,f32,f32);
    extern void GXSetScissor(u32,u32,u32,u32); extern void C_MTXOrtho(void*,f32,f32,f32,f32,f32,f32);
    extern void GXSetProjection(void*,s32); extern void PSMTXScale(void*,f32,f32,f32);
    extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetZMode(s32,s32,s32);
    extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOp(s32,s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern u32 VIGetRetraceCount(void); extern void L_OSGetFontTexture(char*,void**,s32*,s32*,u32*);
    extern void GXInitTexObj(void*,void*,u16,u16,u32,s32,s32,s32); extern void GXLoadTexObj(void*,s32);
    extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXBegin(s32,s32,u16);
    extern void GXSetColorUpdate(s32); extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetAlphaUpdate(s32); extern void VISetNextFrameBuffer(void*); extern void VISetBlack(s32);
    extern void VIFlush(void); extern void* DemoCurrentBuffer; extern void* DemoFrameBuffer1; extern void* DemoFrameBuffer2;
    extern u32 dat_80428ba8;
    extern f32 float_0_80428bb0, float_608_80428bb4, float_480_80428bb8;
    extern f32 float_1_80428bbc, float_0p8_80428bc0;
    char message[4096]; char line[1024]; u32* backchain; void* allocation; s32 i;
    f32 projection[4][4]; f32 model[3][4]; f32 texMtx[3][4]; u8 texObj[32];
    void* texture; s32 texX, texY; u32 advance; s32 scroll, x, y, index; u32 retrace; char* p;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    if (N_getDebugMode() < 0) {
        if (error == 3) strcpy(message, "---- OS_ERROR_ISI ----\n");
        else if (error < 3) { if (error > 1) strcpy(message, "---- OS_ERROR_DSI ----\n"); }
        else if (error == 6) strcpy(message, "---- OS_ERROR_PROGRAM ----\n");
        sprintf(line,"---- Context 0x%08x ----\n",context); strcat(message,line);
        for (i=0;i<16;i++) { sprintf(line,"r%-2d  = 0x%08x r%-2d  = 0x%08x\n",i,*(u32*)(context+i*4),i+16,*(u32*)(context+0x40+i*4)); strcat(message,line); }
        sprintf(line,"LR   = 0x%08x CR   = 0x%08x\n",*(u32*)(context+0x84),*(u32*)(context+0x80)); strcat(message,line);
        sprintf(line,"SRR0 = 0x%08x SRR1 = 0x%08x\n",*(u32*)(context+0x198),*(u32*)(context+0x19C)); strcat(message,line);
        sprintf(line,"DSISR= 0x%08x DAR  = 0x%08x\n",dsisr,dar); strcat(message,line); strcat(message,"\n");
        sprintf(line,"Address:     Back Chain   LR Save\n"); strcat(message,line); backchain=*(u32**)(context+4); i=0;
        while(backchain && backchain!=(u32*)-1 && i++<16){sprintf(line,"0x%08x:  0x%08x   0x%08x\n",backchain,backchain[0],backchain[1]);strcat(message,line);backchain=(u32*)backchain[0];}
        strcat(message,"\n"); sprintf(line,"Instruction at 0x%x\n(read from SRR0) attempted to access\ninvalid address 0x%x (read from DAR)\n",*(u32*)(context+0x198),dar); strcat(message,line);
        psndExit(); L_OSFillFPUContext(context); L_smartReInit(); allocation=smartAlloc(OSGetFontEncode(),0); L_OSInitFont(*(void**)allocation);
        GXSetCopyClear(&dat_80428ba8,0xFFFFFF); GXCopyDisp(L_DEMOGetCurrentBuffer(),1);
        GXSetViewport(float_0_80428bb0,float_0_80428bb0,float_608_80428bb4,float_480_80428bb8,float_0_80428bb0,float_1_80428bbc); GXSetScissor(0,0,608,480);
        C_MTXOrtho(projection,float_0_80428bb0,float_608_80428bb4,float_0_80428bb0,float_480_80428bb8,float_0_80428bb0,float_1_80428bbc); GXSetProjection(projection,1);
        PSMTXScale(model,float_0p8_80428bc0,float_1_80428bbc,float_1_80428bbc); GXLoadPosMtxImm(model,0); GXSetCurrentMtx(0);
        GXSetZMode(1,3,1); GXSetNumChans(0); GXSetNumTevStages(1); GXSetTevOp(0,3); GXSetTevOrder(0,0,0,0xFF);
        GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,30,0,125); GXSetBlendMode(1,1,1,0);
        GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxDesc(13,1); GXSetVtxAttrFmt(0,9,1,3,0); GXSetVtxAttrFmt(0,13,1,3,0);
        for(scroll=0;scroll>=-960;scroll--){
            retrace=VIGetRetraceCount(); x=40; y=scroll+50; index=0; p=message;
            while(*p){
                if(*p=='\n'){x=40;y+=24;p++;index++;continue;}
                L_OSGetFontTexture(message+index,&texture,&texX,&texY,&advance);
                GXInitTexObj(texObj,texture,*(u16*)((u8*)*(void**)allocation+0x1E),*(u16*)((u8*)*(void**)allocation+0x20),*(u16*)((u8*)*(void**)allocation+0x18),0,0,0);
                GXLoadTexObj(texObj,0); PSMTXScale(texMtx,float_1_80428bbc/(*(u16*)((u8*)*(void**)allocation+0x1E)),float_1_80428bbc/(*(u16*)((u8*)*(void**)allocation+0x20)),float_1_80428bbc); GXLoadTexMtxImm(texMtx,30,1);
                GXBegin(0x80,0,4);
                *fifo=(u16)x;*fifo=(u16)y;*fifo=0;*fifo=(u16)texX;*fifo=(u16)texY;
                *fifo=(u16)(x+*(s16*)((u8*)*(void**)allocation+0x10));*fifo=(u16)y;*fifo=0;*fifo=(u16)(texX+*(s16*)((u8*)*(void**)allocation+0x10));*fifo=(u16)texY;
                *fifo=(u16)(x+*(s16*)((u8*)*(void**)allocation+0x10));*fifo=(u16)(y+*(s16*)((u8*)*(void**)allocation+0x12));*fifo=0;*fifo=(u16)(texX+*(s16*)((u8*)*(void**)allocation+0x10));*fifo=(u16)(texY+*(s16*)((u8*)*(void**)allocation+0x12));
                *fifo=(u16)x;*fifo=(u16)(y+*(s16*)((u8*)*(void**)allocation+0x12));*fifo=0;*fifo=(u16)texX;*fifo=(u16)(texY+*(s16*)((u8*)*(void**)allocation+0x12));
                x += advance; p++; index++;
            }
            GXSetZMode(1,3,1); GXSetColorUpdate(1); GXSetAlphaCompare(7,0,1,7,0); GXSetAlphaUpdate(1); GXCopyDisp(DemoCurrentBuffer,1);
            VISetNextFrameBuffer(DemoCurrentBuffer); VISetBlack(0); VIFlush(); while(retrace==VIGetRetraceCount()){}
            DemoCurrentBuffer = DemoCurrentBuffer==DemoFrameBuffer1 ? DemoFrameBuffer2 : DemoFrameBuffer1;
        }
    }
    PPCHalt(); for(;;){}
}
