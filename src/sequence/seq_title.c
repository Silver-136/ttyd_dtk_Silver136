#include "sequence/seq_title.h"
#define PAD_R      0x0020
#define PAD_L      0x0040
#define PAD_A      0x0100
#define PAD_B      0x0200
#define PAD_X      0x0400
#define PAD_Y      0x0800
#define PAD_START  0x1000
extern s32 gp;
extern s32 wp2;
extern const f32 float_1_8041f594;
extern const f32 float_neg200_8041f598;
extern const f32 float_0_8041f59c;
extern const f32 float_neg0p5_8041f5a0;
extern const f32 float_0p5_8041f5a4;
extern const f64 double_to_int_802bf4c0;
extern const f64 double_to_int_mask_802bf4c8;
extern u32 unk_80429508;
extern void L_titleInit(void);
extern void* camGetCurPtr(void);
extern void TEXGetGXTexObjFromPalette(s32 palette, void* texObj, s32 index);
extern void GXLoadTexObj(void* texObj, s32 mapId);
extern void GXSetNumTevStages(s32 stages);
extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
extern void GXSetTevColor(s32 reg, void* color);
extern void GXSetNumChans(s32 count);
extern void GXSetNumTexGens(s32 count);
extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXConcat(void* a, void* b, void* ab);
extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
extern void GXLoadPosMtxImm(void* mtx, s32 id);
extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
extern void GXSetZCompLoc(s32 beforeTex);
extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
extern void GXSetCullMode(s32 mode);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32 attr, s32 type);
extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
extern u32 GXGetTexObjHeight(void* texObj);
extern u32 GXGetTexObjWidth(void* texObj);
extern void GXSetCurrentMtx(s32 id);
extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
extern void GXSetTevSwapModeTable(s32 table, s32 r, s32 g, s32 b, s32 a);
#define READ_F32(addr) (*(volatile const f32*)&(addr))
#define FIFO_F32 (*(volatile f32*)0xCC008000)
extern u32 vec3_802bf3f8[];
extern u32 vec3_802bf404[];
extern u32 dat_8041f584;
extern u32 dat_8041f588;
extern u32 dat_8041f58c;
extern u32 dat_8041f590;
extern u32 dat_8041f558;
extern u32 dat_8041f55c;
extern u32 dat_8041f560;
extern u32 dat_8041f564;
extern u32 dat_8041f568;
extern u32 dat_8041f56c;
extern u32 dat_8041f570;
extern u32 dat_8041f574;
extern u32 dat_8041f578;
extern u32 dat_8041f57c;
extern u32 dat_8041f580;
extern void fileFree(void* file);
extern s32 seqGetNextSeq(void);
extern void psndStopAllFadeOut(void);
extern const f32 float_neg170_8041f5cc;
extern const f32 float_300_8041f5d0;
extern const f32 float_neg300_8041f5d4;
extern const f32 float_0p1_8041f5d8;
extern const f32 float_400_8041f5dc;
extern const f32 float_neg400_8041f5e0;
extern u8 psbgm[];
extern u8 sound[];
extern u32 psndBGMStartCheck(s32 id);
extern s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
extern s32 seqGetPrevSeq(void);
extern void fadeEntry(s32 type, s32 duration, void* color);
extern void fadeSetNarrowFast(void);
extern s32 fadeIsFinish(void);
extern void fadeReset(s32 type);
extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 duration);
extern s32 getMarioStDvdRoot(void);
extern void fileAsyncf(s32 id, s32 flags, void* path, ...);
extern s32 psndSFXOn(s32 id);
extern void psndBGMOff_f_d(s32 flags, s32 duration, s32 fadeType);
extern void seqSetSeq(s32 seq, void* arg1, void* arg2);
extern s32 pouchGetHP(void);
extern void pouchSetHP(s32 hp);
extern u32 keyGetButtonTrg(s32 controller);
extern void dispEntry(s32 cameraId, s32 order, void* callback, s32 param, f32 z);
extern const f32 float_20_8041f5a8;
extern const f32 float_6p2832_8041f5ac;
extern const f32 float_120_8041f5b0;
extern const f32 float_459_8041f5b4;
extern const f32 float_neg122_8041f5b8;
extern const f32 float_0p8_8041f5bc;
extern const f32 float_60_8041f5c0;
extern const f32 float_220_8041f5c4;
extern const f32 float_200_8041f5c8;
extern void* fileAllocf(s32 id, void* path, ...);
extern void psndSFXAllOff(void);
extern void psndBGMOn(s32 flags, void* name);
extern u32 OSGetSoundMode(void);
extern void SoundSetOutputMode(s32 mode);
extern const f32 float_1000_8041f5e4;
extern const f32 float_neg1000_8041f5e8;
extern s32 dat_ptr_80414f20;
extern s32 dat_ptr_80414f24;
extern void* camGetPtr(s32 cameraId);
extern void GXGetProjectionv(void* proj);
extern void GXGetViewportv(void* viewport);
extern void GXSetProjection(void* projMtx, s32 projType);
extern void GXSetViewport(f32 xOrig, f32 yOrig, f32 width, f32 height, f32 nearZ, f32 farZ);
extern void GXSetProjectionv(void* proj);
extern void winTexInit(s32 texData);
extern void unk_8017c9bc(s32 msg, void* pos, void* size, void* color);
extern void winTexSet(s32 texId, void* pos, void* size, void* color);
extern f64 sin(f64 angle);
extern u32 keyGetButton(s32 controller);
extern void FontDrawStart(void);
extern void FontDrawEdge(void);
extern void FontDrawColor(void* color);
extern void FontDrawScale(f32 scale);
extern void FontDrawString(f32 x, f32 y, s32 str);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void pressStartGX_80008888(u8 alpha, f32 x, f32 y) {
    void* cam;
    u8 tex0[0x20];
    u8 tex1[0x20];
    f32 workMtx[3][4];
    f32 scaleMtx[3][4];
    u32 color;
    u32 colorTemp;
    s32 work;
    s32 texData;
    s32 language;
    u16 height;
    u16 width;
    f32 h;
    f32 w;

    cam = camGetCurPtr();

    work = *(s32*)wp2;
    language = *(s32*)(gp + 0x16C);
    texData = *(s32*)(work + 0xA0);
    TEXGetGXTexObjFromPalette(*(s32*)texData, &tex0, language + 0xF);
    GXLoadTexObj(&tex0, 0);

    work = *(s32*)wp2;
    texData = *(s32*)(work + 0xA0);
    TEXGetGXTexObjFromPalette(*(s32*)texData, &tex1, 6);
    GXLoadTexObj(&tex1, 1);

    GXSetNumTevStages(2);

    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);

    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0, 8, 0xF);
    GXSetTevAlphaIn(1, 7, 0, 1, 7);

    colorTemp = unk_80429508;
    *((u8*)&colorTemp + 3) = alpha;
    color = colorTemp;
    GXSetTevColor(1, &color);

    GXSetNumChans(0);
    GXSetNumTexGens(2);

    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);

    PSMTXScale(scaleMtx, float_1_8041f594, float_1_8041f594, float_1_8041f594);

    PSMTXTrans(
        workMtx,
        (f32)*(s32*)(wp2 + 0x2C) / float_neg200_8041f598,
        float_0_8041f59c,
        float_0_8041f59c
    );

    PSMTXConcat(scaleMtx, workMtx, workMtx);
    GXLoadTexMtxImm(workMtx, 0x1E, 1);

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXSetCullMode(0);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    PSMTXTrans(workMtx, x, y, float_0_8041f59c);

    height = GXGetTexObjHeight(&tex0);
    h = (f32)height;

    width = GXGetTexObjWidth(&tex0);
    w = (f32)width;

    PSMTXScale(scaleMtx, w, h, float_1_8041f594);
    PSMTXConcat(workMtx, scaleMtx, workMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), workMtx, workMtx);

    GXLoadPosMtxImm(workMtx, 0);
    GXSetCurrentMtx(0);

    GXBegin(0x80, 0, 4);

    FIFO_F32 = READ_F32(float_neg0p5_8041f5a0);
    FIFO_F32 = READ_F32(float_neg0p5_8041f5a0);
    FIFO_F32 = READ_F32(float_0_8041f59c);
    FIFO_F32 = READ_F32(float_0_8041f59c);
    FIFO_F32 = READ_F32(float_1_8041f594);

    FIFO_F32 = READ_F32(float_0p5_8041f5a4);
    FIFO_F32 = READ_F32(float_neg0p5_8041f5a0);
    FIFO_F32 = READ_F32(float_0_8041f59c);
    FIFO_F32 = READ_F32(float_1_8041f594);
    FIFO_F32 = READ_F32(float_1_8041f594);

    FIFO_F32 = READ_F32(float_0p5_8041f5a4);
    FIFO_F32 = READ_F32(float_0p5_8041f5a4);
    FIFO_F32 = READ_F32(float_0_8041f59c);
    FIFO_F32 = READ_F32(float_1_8041f594);
    FIFO_F32 = READ_F32(float_0_8041f59c);

    FIFO_F32 = READ_F32(float_neg0p5_8041f5a0);
    FIFO_F32 = READ_F32(float_0p5_8041f5a4);
    FIFO_F32 = READ_F32(float_0_8041f59c);
    FIFO_F32 = READ_F32(float_0_8041f59c);
    FIFO_F32 = READ_F32(float_0_8041f59c);

    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void disp2(s32 cameraId) {
    void* cam8;
    void* cam;
    void* camAgain;
    s32 i;
    u32 camBackup[0x9C];
    f32 projBackup[6];
    f32 viewportBackup[6];
    u32* vec;
    u32 msgTbl[6];
    u32 color1;
    u32 color2;
    u32 fontColor;
    u32 size1[3];
    u32 basePos1[3];
    u32 pos1[3];
    u32 size2[3];
    u32 basePos2[3];
    u32 pos2[3];
    s32 lang;
    s32 counter;
    s32 alpha;
    f64 angle;
    f64 sineValue;
    f32 sineF;
    u32* copySrc;
    u32* copyDst;
    u32 copyA;
    u32 copyB;
    s32 copyCount;
    u32* vecMsg;
    vec = vec3_802bf3f8;
    cam8 = camGetPtr(8);
    vecMsg = vec;

    msgTbl[0] = vecMsg[6];
    msgTbl[1] = vecMsg[7];
    msgTbl[2] = vecMsg[8];
    msgTbl[3] = vecMsg[9];
    msgTbl[4] = vecMsg[10];
    msgTbl[5] = vecMsg[11];

    cam = camGetPtr(cameraId);
    copySrc = (u32*)((s32)cam - 4);
    copyDst = (u32*)((s32)&camBackup[4] - 4);
    for (i = 0; i < 0x4C; i++) {
        copyA = copySrc[1];
        copySrc += 2;
        copyB = copySrc[0];
        copyDst[1] = copyA;
        copyDst += 2;
        copyDst[0] = copyB;
    };

    camAgain = camGetPtr(cameraId);
    copySrc = (u32*)((s32)cam8 - 4);
    copyDst = (u32*)((s32)camAgain - 4);
    for (i = 0; i < 0x4C; i++) {
        copyA = copySrc[1];
        copySrc += 2;
        copyB = copySrc[0];
        copyDst[1] = copyA;
        copyDst += 2;
        copyDst[0] = copyB;
    }

    GXGetProjectionv(projBackup);
    GXGetViewportv(viewportBackup);

    GXSetProjection((void*)((s32)cam8 + 0x15C), *(s32*)((s32)cam8 + 0x19C));
    GXSetViewport(
        *(f32*)((s32)cam8 + 0xFC),
        *(f32*)((s32)cam8 + 0x100),
        *(f32*)((s32)cam8 + 0x104),
        *(f32*)((s32)cam8 + 0x108),
        *(f32*)((s32)cam8 + 0x10C),
        *(f32*)((s32)cam8 + 0x110)
    );

    winTexInit(**(s32**)(*(s32*)wp2 + 0xA0));

    color1 = dat_8041f588;

    size1[0] = vec[15];
    size1[1] = vec[16];
    size1[2] = vec[17];

    basePos1[0] = vec[12];
    basePos1[1] = vec[13];
    basePos1[2] = vec[14];

    basePos1[0] = *(u32*)(wp2 + 0xC);
    basePos1[1] = *(u32*)(wp2 + 0x10);
    *(f32*)&basePos1[1] = *(f32*)&basePos1[1] - float_20_8041f5a8;

    pos1[0] = basePos1[0];
    pos1[1] = basePos1[1];
    pos1[2] = basePos1[2];

    lang = *(s32*)(gp + 0x16C);
    unk_8017c9bc(msgTbl[lang], pos1, size1, &color1);

    color2 = dat_8041f58c;

    size2[0] = vec[21];
    size2[1] = vec[22];
    size2[2] = vec[23];

    basePos2[0] = vec[18];
    basePos2[1] = vec[19];
    basePos2[2] = vec[20];

    basePos2[0] = *(u32*)(wp2 + 0x14);
    basePos2[1] = *(u32*)(wp2 + 0x18);

    pos2[0] = basePos2[0];
    pos2[1] = basePos2[1];
    pos2[2] = basePos2[2];

    winTexSet(*(s32*)(gp + 0x16C) + 9, pos2, size2, &color2);

    *(s32*)(wp2 + 0x2C) = *(s32*)(wp2 + 0x2C) + 1;
    counter = *(s32*)(wp2 + 0x2C);

    angle = (f64)((float_6p2832_8041f5ac * (f32)counter) / float_120_8041f5b0);
    sineValue = sin(angle);
    sineF = (f32)sineValue;

    alpha = (s32)(*(f32*)(wp2 + 0x1C) * float_459_8041f5b4 * sineF);

    if (alpha < 0) {
        alpha = 0;
    }

    if (alpha > 0xFF) {
        alpha = 0xFF;
    }

    pressStartGX_80008888(alpha, float_0_8041f59c, float_neg122_8041f5b8);

    if (*(s32*)(wp2 + 0x30) < 0) {
        if ((keyGetButton(0) & 0x10) != 0) {
            FontDrawStart();
            FontDrawEdge();

            fontColor = dat_8041f590;
            FontDrawColor(&fontColor);

            FontDrawScale(float_0p8_8041f5bc);
            FontDrawString(float_60_8041f5c0, float_220_8041f5c4, dat_ptr_80414f20);
            FontDrawString(float_200_8041f5c8, float_220_8041f5c4, dat_ptr_80414f24);
        }
    }

    cam = camGetPtr(cameraId);
    for (i = 0; i < 0x98; i++) {
        ((u32*)cam)[i] = camBackup[i];
    }

    GXSetProjectionv(projBackup);
    GXSetViewport(
        viewportBackup[0],
        viewportBackup[1],
        viewportBackup[2],
        viewportBackup[3],
        viewportBackup[4],
        viewportBackup[5]
    );
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void disp1(s32 cameraId) {
    void* cam8;
    void* cam;
    u32 camBackup[0x98];
    f32 projBackup[6];
    f32 viewportBackup[6];

    u32 color;
    u32 pos[3];
    u32 size[3];

    u32* copySrc;
    u32* copyDst;
    u32 copyA;
    u32 copyB;
    s32 i;

    cam8 = camGetPtr(8);

    cam = camGetPtr(cameraId);
    copySrc = (u32*)((s32)cam - 4);
    copyDst = (u32*)((s32)camBackup - 4);
    for (i = 0; i < 0x4C; i++) {
        copyA = copySrc[1];
        copySrc += 2;
        copyB = copySrc[0];
        copyDst[1] = copyA;
        copyDst += 2;
        copyDst[0] = copyB;
    }

    cam = camGetPtr(cameraId);
    copySrc = (u32*)((s32)cam8 - 4);
    copyDst = (u32*)((s32)cam - 4);
    for (i = 0; i < 0x4C; i++) {
        copyA = copySrc[1];
        copySrc += 2;
        copyB = copySrc[0];
        copyDst[1] = copyA;
        copyDst += 2;
        copyDst[0] = copyB;
    }

    GXGetProjectionv(projBackup);
    GXGetViewportv(viewportBackup);

    GXSetProjection((void*)((s32)cam8 + 0x15C), *(s32*)((s32)cam8 + 0x19C));
    GXSetViewport(
        *(f32*)((s32)cam8 + 0xFC),
        *(f32*)((s32)cam8 + 0x100),
        *(f32*)((s32)cam8 + 0x104),
        *(f32*)((s32)cam8 + 0x108),
        *(f32*)((s32)cam8 + 0x10C),
        *(f32*)((s32)cam8 + 0x110)
    );

    winTexInit(**(s32**)(*(s32*)wp2 + 0xA0));

    color = dat_8041f584;

    size[0] = vec3_802bf404[0];
    size[1] = vec3_802bf404[1];
    size[2] = vec3_802bf404[2];

    pos[0] = vec3_802bf3f8[0];
    pos[1] = vec3_802bf3f8[1];
    pos[2] = vec3_802bf3f8[2];

    winTexSet(5, pos, size, &color);

    cam = camGetPtr(cameraId);
    copySrc = (u32*)((s32)camBackup - 4);
    copyDst = (u32*)((s32)cam - 4);
    for (i = 0; i < 0x4C; i++) {
        copyA = copySrc[1];
        copySrc += 2;
        copyB = copySrc[0];
        copyDst[1] = copyA;
        copyDst += 2;
        copyDst[0] = copyB;
    }

    GXSetProjectionv(projBackup);
    GXSetViewport(
        viewportBackup[0],
        viewportBackup[1],
        viewportBackup[2],
        viewportBackup[3],
        viewportBackup[4],
        viewportBackup[5]
    );
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void titleMain(void* seq) {
    u32* vec;
    s32 state;
    s32 prev;
    s32 bgmSlot;
    u16* soundTable;
    s32 fps;
    s32 counter;
    s32 root;
    s32 inputState;
    u32 buttons;
    f32 value;
    f32 alphaAdd;
    f32 alphaMax;
    u32 color0;
    u32 color1;
    u32 color2;
    u32 color3;
    u32 color4;
    u32 color5;
    u32 color6;
    u32 color7;
    u32 color8;
    u32 color9;
    u32 color10;

    vec = vec3_802bf3f8;
    state = *(s32*)(wp2 + 0x8);

    switch (state) {
        case 0:
            if (psndBGMStartCheck(0) != 0) {
                if (animGroupBaseAsync((void*)((s32)vec + 0xD8), 2, 0) != 0) {
                    if (animGroupBaseAsync((void*)((s32)vec + 0xF0), 2, 0) != 0) {
                        prev = seqGetPrevSeq();

                        if (prev == 6) {
                            color0 = dat_8041f558;
                            fadeEntry(0x40, 0, &color0);

                            color1 = dat_8041f55c;
                            fadeEntry(0x1F, 0, &color1);

                            *(f32*)(wp2 + 0x10) = float_120_8041f5b0;
                            *(f32*)(wp2 + 0x18) = float_neg170_8041f5cc;
                            *(s32*)(wp2 + 0x24) = 0;
                        } else {
                            prev = seqGetPrevSeq();

                            if (prev == 5) {
                                color2 = dat_8041f560;
                                fadeEntry(0x15, 0, &color2);

                                color3 = dat_8041f564;
                                fadeEntry(0x1D, 0, &color3);

                                *(s32*)(wp2 + 0x24) = 0x4E;
                            } else {
                                color4 = dat_8041f568;
                                fadeEntry(0x15, 0, &color4);

                                color5 = dat_8041f56c;
                                fadeEntry(0x22, 0, &color5);

                                fadeSetNarrowFast();
                                *(s32*)(wp2 + 0x24) = 0x17;
                            }
                        }

                        *(s32*)(wp2 + 0x8) = *(s32*)(wp2 + 0x8) + 1;
                    }
                }
            }
            break;

        case 1:
            *(s32*)(wp2 + 0x24) = *(s32*)(wp2 + 0x24) - 1;

            if (*(s32*)(wp2 + 0x24) == 0) {
                bgmSlot = *(s32*)((s32)psbgm + 0x4);
                if ((u32)(bgmSlot + 0x10000) != 0xFFFF) {
                    soundTable = *(u16**)((s32)sound + 0x100);
                    *(u16*)((s32)soundTable + bgmSlot * 0x138) =
                        *(u16*)((s32)soundTable + bgmSlot * 0x138) & 0x7FFF;
                }
            }

            if (fadeIsFinish() != 0) {
                prev = seqGetPrevSeq();

                if (prev != 6) {
                    color6 = dat_8041f570;
                    fadeEntry(0x14, 0x1F4, &color6);
                    *(s32*)(wp2 + 0x8) = *(s32*)(wp2 + 0x8) + 1;
                } else {
                    *(f32*)(wp2 + 0x10) = float_120_8041f5b0;
                    *(f32*)(wp2 + 0x18) = float_neg170_8041f5cc;
                    *(s32*)(wp2 + 0x20) = 0;
                    *(s32*)(wp2 + 0x2C) = 0;
                    *(s32*)(wp2 + 0x8) = 3;
                }
            }
            break;

        case 2:
            *(f32*)(wp2 + 0x0C) = float_0_8041f59c;

            value = intplGetValue(
                float_300_8041f5d0,
                float_120_8041f5b0,
                0xA,
                *(s32*)(wp2 + 0x20),
                *(s32*)(gp + 0x4)
            );
            *(f32*)(wp2 + 0x10) = (f32)value;

            *(s32*)(wp2 + 0x24) = *(s32*)(wp2 + 0x24) - 1;

            if (*(s32*)(wp2 + 0x24) == 0) {
                bgmSlot = *(s32*)((s32)psbgm + 0x4);
                if ((u32)(bgmSlot + 0x10000) != 0xFFFF) {
                    soundTable = *(u16**)((s32)sound + 0x100);
                    *(u16*)((s32)soundTable + bgmSlot * 0x138) =
                        *(u16*)((s32)soundTable + bgmSlot * 0x138) & 0x7FFF;
                }

                prev = seqGetPrevSeq();
                if (prev == 5) {
                    psndSFXOn(0xAB8);
                }
            }

            *(f32*)(wp2 + 0x14) = float_0_8041f59c;

            value = intplGetValue(
                float_neg300_8041f5d4,
                float_neg170_8041f5cc,
                0xB,
                *(s32*)(wp2 + 0x20),
                *(s32*)(gp + 0x4)
            );
            *(f32*)(wp2 + 0x18) = (f32)value;

            *(s32*)(wp2 + 0x20) = *(s32*)(wp2 + 0x20) + 1;

            counter = *(s32*)(wp2 + 0x20);
            fps = *(s32*)(gp + 0x4);
            if (counter > fps) {
                root = getMarioStDvdRoot();
                fileAsyncf(4, 0, (void*)((s32)vec + 0x60), root);

                animGroupBaseAsync((void*)((s32)vec + 0x6C), 0, 0);
                animGroupBaseAsync((void*)((s32)vec + 0x74), 0, 0);
                animGroupBaseAsync((void*)((s32)vec + 0x7C), 0, 0);
                animGroupBaseAsync((void*)((s32)vec + 0x94), 0, 0);
                animGroupBaseAsync((void*)((s32)vec + 0xA8), 0, 0);
                animGroupBaseAsync((void*)((s32)vec + 0xB0), 0, 0);
                animGroupBaseAsync((void*)((s32)vec + 0xBC), 0, 0);

                *(s32*)(wp2 + 0x20) = 0;
                *(s32*)(wp2 + 0x2C) = 0;
                *(s32*)(wp2 + 0x8) = *(s32*)(wp2 + 0x8) + 1;
            }
            break;

        case 3:
            *(s32*)(wp2 + 0x20) = *(s32*)(wp2 + 0x20) + 1;

            counter = *(s32*)(wp2 + 0x20);
            fps = *(s32*)(gp + 0x4);
            if (counter > fps) {
                *(f32*)(wp2 + 0x1C) = *(f32*)(wp2 + 0x1C) + float_0p1_8041f5d8;

                if (*(f32*)(wp2 + 0x1C) > float_1_8041f594) {
                    *(f32*)(wp2 + 0x1C) = float_1_8041f594;
                }

                buttons = keyGetButtonTrg(0);
                if ((buttons & (PAD_A | PAD_START)) != 0) {
                    psndSFXOn(8);
                    *(s32*)(wp2 + 0x8) = 0xA;
                } else {
                    counter = *(s32*)(wp2 + 0x28) + 1;
                    *(s32*)(wp2 + 0x28) = counter;

                    fps = *(s32*)(gp + 0x4);
                    if (counter > ((fps * 0x14) - fps)) {
                        *(s32*)(wp2 + 0x8) = 0x14;
                    }
                }
            }
            break;

        case 0xA:
            color7 = dat_8041f574;
            fadeEntry(0x3F, 0, &color7);
            *(s32*)(wp2 + 0x8) = *(s32*)(wp2 + 0x8) + 1;

        case 0xB:
            if (fadeIsFinish() != 0) {
                color8 = dat_8041f578;
                fadeEntry(0x1A, 0, &color8);
                *(s32*)(wp2 + 0x8) = *(s32*)(wp2 + 0x8) + 1;
            }
            break;

        case 0xC:
            if (fadeIsFinish() != 0) {
                fadeReset(4);
                seqSetSeq(6, 0, 0);
            }
            break;

        case 0x14:
            color9 = dat_8041f57c;
            fadeEntry(0x21, 0, &color9);

            color10 = dat_8041f580;
            fadeEntry(0xA, 0x3E8, &color10);

            psndBGMOff_f_d(0x200, 0x3E8, 0);

            *(s32*)(wp2 + 0x20) = 0;
            *(s32*)(wp2 + 0x8) = *(s32*)(wp2 + 0x8) + 1;
            break;

        case 0x15:
            value = intplGetValue(
                float_120_8041f5b0,
                float_400_8041f5dc,
                0xB,
                *(s32*)(wp2 + 0x20),
                *(s32*)(gp + 0x4)
            );
            *(f32*)(wp2 + 0x10) = (f32)value;

            value = intplGetValue(
                float_neg170_8041f5cc,
                float_neg400_8041f5e0,
                0xB,
                *(s32*)(wp2 + 0x20),
                *(s32*)(gp + 0x4)
            );
            *(f32*)(wp2 + 0x18) = (f32)value;

            *(s32*)(wp2 + 0x20) = *(s32*)(wp2 + 0x20) + 1;

            if (*(s32*)(gp + 0x4) < *(s32*)(wp2 + 0x20)) {
                *(s32*)(wp2 + 0x20) = *(s32*)(gp + 0x4);
            }

            *(f32*)(wp2 + 0x1C) = *(f32*)(wp2 + 0x1C) - float_0p1_8041f5d8;

            if (*(f32*)(wp2 + 0x1C) < float_0_8041f59c) {
                *(f32*)(wp2 + 0x1C) = float_0_8041f59c;
            }

            if (fadeIsFinish() != 0) {
                fadeReset(1);

                if (pouchGetHP() <= 0) {
                    pouchSetHP(1);
                }

                *(s32*)(gp + 0x110) = 0xA;
                *(s32*)(gp + 0x114) = 0;

                seqSetSeq(3, (void*)((s32)vec + 0x104), 0);
            }
            break;
    }

    if ((*(s32*)(wp2 + 0x8) != 0xC) && (*(s32*)((s32)seq + 0x4) < 0xE)) {
        dispEntry(4, 0, disp1, 0, float_0_8041f59c);
        dispEntry(7, 0, disp2, 0, float_0_8041f59c);

        inputState = *(s32*)(wp2 + 0x30);

        switch (inputState) {
            case 0:
                buttons = keyGetButtonTrg(0);
                if ((buttons & ~PAD_X) != 0) {
                    *(s32*)(wp2 + 0x30) = 0;
                } else {
                    buttons = keyGetButtonTrg(0);
                    if ((buttons & PAD_X) != 0) {
                        *(s32*)(wp2 + 0x30) = *(s32*)(wp2 + 0x30) + 1;
                    }
                }
                break;

            case 1:
                buttons = keyGetButtonTrg(0);
                if ((buttons & ~PAD_B) != 0) {
                    *(s32*)(wp2 + 0x30) = 0;
                } else {
                    buttons = keyGetButtonTrg(0);
                    if ((buttons & PAD_B) != 0) {
                        *(s32*)(wp2 + 0x30) = *(s32*)(wp2 + 0x30) + 1;
                    }
                }
                break;

            case 2:
                buttons = keyGetButtonTrg(0);
                if ((buttons & ~PAD_R) != 0) {
                    *(s32*)(wp2 + 0x30) = 0;
                } else {
                    buttons = keyGetButtonTrg(0);
                    if ((buttons & PAD_R) != 0) {
                        *(s32*)(wp2 + 0x30) = *(s32*)(wp2 + 0x30) + 1;
                    }
                }
                break;

            case 3:
                buttons = keyGetButtonTrg(0);
                if ((buttons & ~PAD_B) != 0) {
                    *(s32*)(wp2 + 0x30) = 0;
                } else {
                    buttons = keyGetButtonTrg(0);
                    if ((buttons & PAD_B) != 0) {
                        *(s32*)(wp2 + 0x30) = *(s32*)(wp2 + 0x30) + 1;
                    }
                }
                break;

            case 4:
                buttons = keyGetButtonTrg(0);
                if ((buttons & ~PAD_Y) != 0) {
                    *(s32*)(wp2 + 0x30) = 0;
                } else {
                    buttons = keyGetButtonTrg(0);
                    if ((buttons & PAD_Y) != 0) {
                        *(s32*)(wp2 + 0x30) = *(s32*)(wp2 + 0x30) + 1;
                    }
                }
                break;

            case 5:
                buttons = keyGetButtonTrg(0);
                if ((buttons & ~PAD_L) != 0) {
                    *(s32*)(wp2 + 0x30) = 0;
                } else {
                    buttons = keyGetButtonTrg(0);
                    if ((buttons & PAD_L) != 0) {
                        *(s32*)(wp2 + 0x30) = -1;
                    }
                }
                break;
        }
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void seq_titleExit(void) {
    fileFree(*(void**)wp2);

    if (seqGetNextSeq() != 6) {
        psndStopAllFadeOut();
    }
}

void seq_titleInit(void) {
    L_titleInit();

    *(u32*)gp &= ~0x2;
    *(u32*)gp &= ~0x40;
    *(u32*)gp &= ~0x20;
    *(u32*)gp &= ~0x1000;
}

void seq_titleMain(void* seq) {
    titleMain(seq);
}

s32 N_getDebugMode(void) {
    return *(s32*)(wp2 + 0x30);
}

void* DbgBtlSel_GetMsgDataPtr(void) {
    return 0;
}

void L_titleInit(void) {
    u32* vec;
    s32 root;
    s32 prev;
    s32 bgmSlot;
    u16* soundTable;
    void* cam;

    vec = vec3_802bf3f8;

    memset((void*)wp2, 0, 0x34);

    root = getMarioStDvdRoot();
    *(void**)wp2 = fileAllocf(4, (void*)((s32)vec + 0x10C), root);

    *(f32*)(wp2 + 0x0C) = float_0_8041f59c;
    *(f32*)(wp2 + 0x10) = float_1000_8041f5e4;
    *(f32*)(wp2 + 0x14) = float_0_8041f59c;
    *(f32*)(wp2 + 0x18) = float_neg1000_8041f5e8;
    *(f32*)(wp2 + 0x1C) = float_0_8041f59c;
    *(s32*)(wp2 + 0x30) = 0;

    prev = seqGetPrevSeq();
    if (prev == 6) {
        psndSFXAllOff();
    } else {
        prev = seqGetPrevSeq();
        if (prev != 0) {
            prev = seqGetPrevSeq();
            if (prev == 3) {
                psndBGMOff_f_d(0x200, 0xBB8, 0);
                psndBGMOff_f_d(0x201, 0xBB8, 0);
                psndSFXAllOff();
            } else {
                prev = seqGetPrevSeq();
                if (prev == 5) {
                    psndStopAllFadeOut();
                }
            }
        }
    }

    prev = seqGetPrevSeq();
    if (prev != 6) {
        prev = seqGetPrevSeq();
        if (prev == 5) {
            if (OSGetSoundMode() == 0) {
                SoundSetOutputMode(0);
            } else {
                SoundSetOutputMode(1);
            }

            psndBGMOn(0x200, (void*)((s32)vec + 0x11C));

            bgmSlot = *(s32*)((s32)psbgm + 0x4);
            if ((u32)(bgmSlot + 0x10000) != 0xFFFF) {
                soundTable = *(u16**)((s32)sound + 0x100);
                *(u16*)((s32)soundTable + bgmSlot * 0x138) =
                    *(u16*)((s32)soundTable + bgmSlot * 0x138) | 0x8000;
            }
        } else {
            psndBGMOn(0x200, (void*)((s32)vec + 0x128));

            bgmSlot = *(s32*)((s32)psbgm + 0x4);
            if ((u32)(bgmSlot + 0x10000) != 0xFFFF) {
                soundTable = *(u16**)((s32)sound + 0x100);
                *(u16*)((s32)soundTable + bgmSlot * 0x138) =
                    *(u16*)((s32)soundTable + bgmSlot * 0x138) | 0x8000;
            }
        }
    }

    cam = camGetPtr(8);
    *(u16*)cam &= ~0x100;

    cam = camGetPtr(8);
    *(u16*)cam |= 0x400;
}


/* CHATGPT STUB FILL: main/sequence/seq_title 20260624_183901 */

/* stub-fill: pressStartGX | missing_definition | ghidra_signature */
u8 pressStartGX(void) {
    return 0;
}
