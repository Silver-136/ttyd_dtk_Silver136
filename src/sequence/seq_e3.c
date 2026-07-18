#include "sequence/seq_e3.h"

extern void* wp;
extern void* gp;
extern void str_b_mario_80302608(void);
extern char str_fOff_peron_803027c0[];
extern u32 dat_804286e0;
extern f32 float_270_80428784;

void* getMarioStDvdRoot(void);
void* fileAllocf(s32 size, const char* fmt, ...);
void sysWaitDrawSync(void);
void GXClearBoundingBox(void);
void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
void e3Disp(s32 camera, void* param);
s32 offscreenNameToId(char* name);
void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);
void OSCreateAlarm(void* alarm);
void OSSetAlarm(void* alarm, u64 tick, void* handler);
u64 OSGetTime(void);
void fadeEntry(s32 type, s32 duration, void* color);
void fadeReset(s32 type);
void psndStopAllFadeOut(void);
void psndBGMOn(s32 flags, void* name);
void animGroupBaseAsync(void* name, s32 mode, s32 flags);

void alarm_handler(void) {
    *(u32*)gp |= 0x2000;
}

void seq_e3Exit(void) {
    extern void fileFree(void* ptr);

    fileFree(*(void**)wp);
    psndStopAllFadeOut();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void e3DispOffscreen(s32 camera, void* param) {
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;

    *(s32*)((s32)wp + 4) = *(s32*)((s32)wp + 8);
    sysWaitDrawSync();
    GXClearBoundingBox();
    e3Disp(camera, param);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(offscreenNameToId(str_fOff_peron_803027c0), left, top, right, bottom);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void seq_e3Init(void* seq) {
    register char* rodata;
    register void* seqLocal;
    u32 color;
    u64 time;
    f32 angle;

    rodata = (char*)str_b_mario_80302608;
    seqLocal = seq;
    *(void**)wp = fileAllocf(4, rodata + 0x204, getMarioStDvdRoot());
    angle = float_270_80428784;
    *(s32*)((s32)wp + 4) = 0;
    *(s32*)((s32)wp + 8) = 0;
    *(s32*)((s32)wp + 0xC) = 0;
    *(f32*)((s32)wp + 0x64) = angle;
    OSCreateAlarm((void*)((s32)wp + 0x20));
    OSSetAlarm((void*)((s32)wp + 0x20), ((u64)(*(u32*)0x800000F8 >> 2) * 0x12C), alarm_handler);
    time = OSGetTime();
    *(u64*)((s32)wp + 0x48) = time;
    *(u32*)gp &= ~0x2;
    *(u32*)gp |= 0x20;
    *(u32*)gp |= 0x1000;
    *(u32*)gp &= ~0x2000;
    color = dat_804286e0;
    fadeEntry(9, 0xC8, &color);
    fadeReset(1);
    psndStopAllFadeOut();
    psndBGMOn(0x200, rodata + 0x214);
    *(s32*)((s32)seqLocal + 4) = 0;
    animGroupBaseAsync(rodata + 0x224, 0, 0);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/sequence/seq_e3 20260624_184929 */

/* stub-fill: pressStartGX | missing_definition | ghidra_signature */
void pressStartGX(f32 x, f32 y, u8 alpha) {
    extern void* wp;
    extern void* gpGlobals;
    extern void* camGetCurPtr(void);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* object, s32 index);
    extern void GXLoadTexObj(void* object, s32 mapId);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevColor(s32 reg, const u32* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 source,
                                  s32 matrix, s32 normalize, s32 postMatrix);
    extern void PSMTXScale(f32 matrix[3][4], f32 sx, f32 sy, f32 sz);
    extern void PSMTXTrans(f32 matrix[3][4], f32 tx, f32 ty, f32 tz);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void GXLoadTexMtxImm(f32 matrix[3][4], s32 id, s32 type);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZCompLoc(s32 beforeTexture);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern u16 GXGetTexObjHeight(void* object);
    extern u16 GXGetTexObjWidth(void* object);
    extern void GXLoadPosMtxImm(f32 matrix[3][4], s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void GXSetTevSwapModeTable(s32 table, s32 red, s32 green, s32 blue, s32 alpha);
    extern u32 unk_804298f0;

    void* camera = camGetCurPtr();
    void* palette = **(void***)(*(s32*)wp + 0xA0);
    u8 texture0[32];
    u8 texture1[32];
    f32 scaleMatrix[3][4];
    f32 matrix[3][4];
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u32 tevColor = (unk_804298f0 & 0xFFFFFF00) | alpha;

    TEXGetGXTexObjFromPalette(palette, texture0, 3);
    GXLoadTexObj(texture0, 0);
    TEXGetGXTexObjFromPalette(palette, texture1, 4);
    GXLoadTexObj(texture1, 1);

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
    GXSetTevColor(1, &tevColor);

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);

    PSMTXScale(scaleMatrix, 1.0f, 1.0f, 1.0f);
    PSMTXTrans(matrix,
               (f32)*(u32*)((s32)gpGlobals + 0x1C) / -200.0f,
               0.0f, 0.0f);
    PSMTXConcat(scaleMatrix, matrix, matrix);
    GXLoadTexMtxImm(matrix, 0x1E, 1);

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    PSMTXTrans(matrix, x, y, 0.0f);
    PSMTXScale(scaleMatrix,
               (f32)GXGetTexObjWidth(texture0),
               (f32)GXGetTexObjHeight(texture0), 1.0f);
    PSMTXConcat(matrix, scaleMatrix, matrix);
    PSMTXConcat((f32(*)[4])((s32)camera + 0x11C), matrix, matrix);
    GXLoadPosMtxImm(matrix, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    *fifo = -0.5f;
    *fifo = -0.5f;
    *fifo = 0.0f;
    *fifo = 0.0f;
    *fifo = 1.0f;

    *fifo = 0.5f;
    *fifo = -0.5f;
    *fifo = 0.0f;
    *fifo = 1.0f;
    *fifo = 1.0f;

    *fifo = 0.5f;
    *fifo = 0.5f;
    *fifo = 0.0f;
    *fifo = 1.0f;
    *fifo = 0.0f;

    *fifo = -0.5f;
    *fifo = 0.5f;
    *fifo = 0.0f;
    *fifo = 0.0f;
    *fifo = 0.0f;

    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
}

/* CHATGPT STUB FILL: main/sequence/seq_e3 20260624_184929 */

/* stub-fill: e3Disp | prototype_only | source_prototype */
void e3Disp(s32 cameraId, void* unused) {
    extern void* wp;
    extern void* gp;
    extern f32 e3_tbl[];
    extern void* camGetPtr(s32);
    extern void GXGetProjectionv(f32*);
    extern void GXGetViewportv(f32*);
    extern void GXSetProjection(void*,s32);
    extern void GXSetViewport(f32,f32,f32,f32,f32,f32);
    extern void winTexInit(void*);
    extern void winTexSet(s32,void*,void*,void*);
    extern void pressStartGX(f32,f32,s32);
    extern void windowDispGX_Waku_col(f32,f32,f32,f32,f32,s32,u32*);
    extern void FontDrawStart(void);
    extern void FontDrawColor(u32*);
    extern char* msgSearch(const char*);
    extern u32 FontGetMessageWidth(char*);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void FontDrawStringMtx(void*,char*);
    extern f64 sin(f64);
    extern void* memcpy(void*, const void*, u32);
    u8* work=(u8*)wp;
    u8* globals=(u8*)gp;
    u8* camera2d=camGetPtr(4);
    u8* camera=camGetPtr(cameraId);
    s32 page=*(s32*)(work+4);
    f32 projection[7],viewport[6];
    u8 cameraBackup[608];
    u8 pos[24],scale[24];
    u32 color=0xFFFFFFFF;
    s32 alpha;
    if(cameraId==5) page=1-page;
    memcpy(cameraBackup,camera,608);
    memcpy(camera,camera2d,608);
    GXGetProjectionv(projection);
    GXGetViewportv(viewport);
    GXSetProjection(camera2d+0x40,*(s32*)(camera2d+0x3C));
    GXSetViewport(*(f32*)(camera2d+0x90),*(f32*)(camera2d+0x94),*(f32*)(camera2d+0x98),*(f32*)(camera2d+0x9C),*(f32*)(camera2d+0xA0),*(f32*)(camera2d+0xA4));
    if(page==0) {
        alpha=(s32)(459.0f*(f32)sin((f64)(*(u32*)(globals+0x10)/30.0f)));
        if(alpha<0) alpha=0; if(alpha>255) alpha=255;
        winTexInit(**(void***)(*(u8**)work+0xA0));
        winTexSet(0xC,pos,scale,&color);
        pressStartGX(0.0f,-110.0f,alpha);
    } else {
        s32 row;
        s32 column;
        winTexInit(**(void***)(*(u8**)work+0xA0));
        for (row = 0; row < 40; row++) {
            for (column = 0; column < 32; column++) {
                f32 gridPos[3];
                f32 gridScale[3] = { 1.0f, 1.0f, 1.0f };
                u32 gridColor = 0xFFFFFFFF;
                gridPos[0] = -304.0f + 16.0f * (f32)row;
                gridPos[1] = 240.0f - 16.0f * (f32)column;
                gridPos[2] = 0.0f;
                winTexSet(5, gridPos, gridScale, &gridColor);
            }
        }
        for (row = 0; row < 4; row++) {
            static const char* names[4] = {
                "menu_monosiri_gor", "menu_monosiri_hei",
                "menu_monosiri_gon", "menu_monosiri_win"
            };
            f32 itemPos[3];
            f32 itemScale[3] = { 1.0f, 1.0f, 1.0f };
            f32 textMtx[3][4];
            f32 scaleMtx[3][4];
            u32 itemColor = row == *(s32*)(work + 0xC) ? 0xFFFFFFFF : 0xA0A0A0FF;
            char* message;
            u32 width;
            itemPos[0] = e3_tbl[row * 3 + 1];
            itemPos[1] = e3_tbl[row * 3 + 2];
            itemPos[2] = 0.0f;
            winTexSet(row + 8, itemPos, itemScale, &itemColor);
            if (row == *(s32*)(work + 0xC)) {
                winTexSet(row + 8, itemPos, itemScale, &color);
            }
            windowDispGX_Waku_col(itemPos[0] - 100.0f, itemPos[1] - 60.0f,
                                  200.0f, 40.0f, 20.0f, 0, &itemColor);
            FontDrawStart();
            FontDrawColor(&color);
            message = msgSearch(names[row]);
            width = FontGetMessageWidth(message);
            PSMTXTrans(textMtx, itemPos[0] - 0.35f * (f32)width,
                       itemPos[1] - 68.0f, 0.0f);
            PSMTXScale(scaleMtx, 0.7f, 1.0f, 1.0f);
            PSMTXConcat(textMtx, scaleMtx, textMtx);
            FontDrawStringMtx(textMtx, message);
        }
        for (row = 1; row >= 0; row--) {
            f32 arrowScale[3] = { 1.0f, 1.0f, 1.0f };
            f32 arrowPos[3];
            u32 arrowColor = row == 0 ? 0xFFFFFFFF : 0xA0A0A0FF;
            s32 arrowTexture = ((*(u32*)(globals + 0x10) >> 4) & 1) ? 6 : 7;
            f32 adjust = (f32)(row * 5);
            arrowPos[0] = *(f32*)(work + 0x10) + 110.0f + adjust;
            arrowPos[1] = *(f32*)(work + 0x14) + 90.0f - adjust;
            arrowPos[2] = 0.0f;
            winTexSet(arrowTexture, arrowPos, arrowScale, &arrowColor);
            arrowPos[0] = *(f32*)(work + 0x10) - 110.0f + adjust;
            winTexSet(arrowTexture, arrowPos, arrowScale, &arrowColor);
            arrowPos[0] = *(f32*)(work + 0x10) + 110.0f + adjust;
            arrowPos[1] = *(f32*)(work + 0x14) - 90.0f - adjust;
            winTexSet(arrowTexture, arrowPos, arrowScale, &arrowColor);
            arrowPos[0] = *(f32*)(work + 0x10) - 110.0f + adjust;
            winTexSet(arrowTexture, arrowPos, arrowScale, &arrowColor);
        }
    }
    memcpy(camera,cameraBackup,608);
    GXSetProjection(projection,*(s32*)(camera+0x3C));
    GXSetViewport(viewport[0],viewport[1],viewport[2],viewport[3],viewport[4],viewport[5]);
}

/* CHATGPT FALLBACK MISSING STUBS: main/sequence/seq_e3 20260624_191429 */

/* fallback stub-fill: map=seq_e3Main addr=0x80252818 size=0x00000748 */
void seq_e3Main(void* sequence) {
    extern void* wp;
    extern f32 e3_tbl[];
    extern f32 vec3_80302664[];
    extern f32 vec3_80302670[];
    extern f64 tan(f64 angle);
    extern void* camGetPtr(s32 camera);
    extern u32 fadeIsFinish(void);
    extern void fadeEntry(s32 type, s32 duration, void* color);
    extern void fadeSetOffscreenCallback(void* callback, void* param);
    extern u16 keyGetButtonTrg(s32 pad);
    extern void keyGetStickX(s32 pad);
    extern void keyGetStickY(s32 pad);
    extern u32 keyGetDirTrg(s32 pad);
    extern void psndSFXOn(void* sound);
    extern void e3DispOffscreen(void);
    extern void e3Disp(void);
    extern void dispEntry(s32 camera, s32 mode, void* callback, void* param, f32 order);
    u8* seq = sequence;
    u8* work = wp;
    u8* camera = camGetPtr(4);
    s32 state = *(s32*)(seq + 0xC);
    s32 selection;
    s32 oldSelection;
    u32 color;

    *(f32*)(camera + 0x18) = vec3_80302664[0];
    *(f32*)(camera + 0x1C) = vec3_80302664[1];
    *(f32*)(camera + 0x20) = vec3_80302664[2];
    *(f32*)(camera + 0xC) = vec3_80302670[0];
    *(f32*)(camera + 0x10) = vec3_80302670[1];
    *(f32*)(camera + 0x14) = (240.0f / (f32)tan(0.21817)) * 0.5f;

    if (state == 0) {
        *(s32*)(seq + 0xC) = 1;
    } else if (state == 1) {
        if (fadeIsFinish() && (keyGetButtonTrg(0) & 0x1100)) {
            psndSFXOn("SFX_PRESS_START1");
            color = 0;
            fadeEntry(1, 0, &color);
            fadeSetOffscreenCallback(e3DispOffscreen, wp);
            *(s32*)(work + 8) = 1;
            *(s32*)(seq + 0xC) += 1;
        }
    } else if (state == 2) {
        if (fadeIsFinish()) {
            color = 0;
            fadeEntry(2, 0, &color);
            selection = *(s32*)(work + 0xC) * 3;
            *(f32*)(work + 0x18) = e3_tbl[selection];
            *(f32*)(work + 0x10) = e3_tbl[selection];
            *(f32*)(work + 0x1C) = e3_tbl[selection + 1];
            *(f32*)(work + 0x14) = e3_tbl[selection + 1];
            *(s32*)(seq + 0xC) += 1;
        }
    } else if (state == 3) {
        if (fadeIsFinish()) {
            *(s32*)(seq + 0xC) += 1;
        }
    } else if (state == 4) {
        oldSelection = *(s32*)(work + 0xC);
        keyGetStickX(0);
        keyGetStickY(0);
        if (keyGetDirTrg(0) & 0xC000) {
            *(s32*)(work + 0xC) ^= 1;
        } else if (keyGetDirTrg(0) & 0x3000) {
            *(s32*)(work + 0xC) ^= 2;
        }
        if (oldSelection != *(s32*)(work + 0xC)) {
            psndSFXOn((void*)5);
        } else if (keyGetButtonTrg(0) & 0x200) {
            psndSFXOn((void*)0x13);
            color = 0;
            fadeEntry(1, 0, &color);
            fadeSetOffscreenCallback(e3DispOffscreen, sequence);
            *(s32*)(work + 8) = 0;
            *(s32*)(seq + 0xC) += 1;
        } else if (keyGetButtonTrg(0) & 0x100) {
            psndSFXOn((void*)0x12);
            *(s32*)(seq + 0xC) = 100;
        }
    } else if (state == 5) {
        if (fadeIsFinish()) {
            color = 0;
            fadeEntry(2, 0, &color);
            *(s32*)(seq + 0xC) = 0;
        }
    } else if (state == 100) {
        color = 0;
        fadeEntry(4, 200, &color);
        *(s32*)(seq + 0xC) += 1;
    } else if (state == 101) {
        if (fadeIsFinish()) {
            *(s32*)(seq + 0xC) += 1;
        }
    }

    selection = *(s32*)(work + 0xC) * 3;
    *(f32*)(work + 0x18) = e3_tbl[selection];
    *(f32*)(work + 0x1C) = e3_tbl[selection + 1];
    *(f32*)(work + 0x10) += (*(f32*)(work + 0x18) - *(f32*)(work + 0x10)) * 0.25f;
    *(f32*)(work + 0x14) += (*(f32*)(work + 0x1C) - *(f32*)(work + 0x14)) * 0.25f;
    dispEntry(4, 0, e3Disp, sequence, 0.0f);
}

