#include "window/win_main.h"

void* wp;
void* memset(void*, int, unsigned long);
void* __memAlloc(s32 heap, u32 size);
void FontDrawStart(void);
void statusWinForceOff(void);
void allPartyForceSlitOff(void);
void winRootDisp(s32 cameraId, void* work);
s32 evtSetValue(void* evt, s32 var, s32 value);
s32 evtGetValue(void* evt, s32 var);
s32 evtCheckID(s32 id);
s32 pouchGetPartyColor(s32 partyId);
s32 evtEntry(void* script, s32 priority, s32 flags);
extern s32 evt_lecture_msg[];
extern s32 winTexTpl;
extern s32 dat_80423ff8;
extern f32 float_0_80424004;
void GXSetCullMode(s32 mode);
void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
void GXSetNumChans(s32 count);
void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
void GXSetNumTevStages(s32 stages);
void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
void GXSetTevSwapMode(s32 stage, s32 rasSel, s32 texSel);
void GXSetNumTexGens(s32 count);
void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
void GXClearVtxDesc(void);
void GXSetVtxDesc(s32 attr, s32 type);
void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
void PSMTXConcat(void* a, void* b, void* ab);
void FontDrawColor(void* color);
char* msgSearch(char* msg);
void FontDrawMessageMtx(void* mtx, char* msg);
char* strstr(char* haystack, const char* needle);
void FontDrawStringMtx(void* mtx, char* str);
void FontDrawEdge(void);
void FontDrawEdgeOff(void);
extern const char str_PCT_80424014;
extern void* gp;
extern f32 float_3_8042401c;
extern f32 float_10_80424020;
extern f32 float_0p2_80424024;
void* camGetPtr(s32 cameraId);
f64 sin(f64 x);
void PSVECSubtract(void* a, void* b, void* ab);
void PSVECScale(void* src, void* dst, f32 scale);

void* winGetPtr(void) {
    return wp;
}

s32 unk_8017b84c(void) {
    *(u16*)wp &= ~0x800;
    return 2;
}

void winOpenDisable(void) {
    if (wp != 0) {
        *(u16*)wp |= 1;
    }
}

void winOpenEnable(void) {
    if (wp != 0) {
        *(u16*)wp &= ~1;
    }
}

void winFontInit(void) {
    FontDrawStart();
}

s32 itemUseFunc2(void) {
    statusWinForceOff();
    return 2;
}

s32 unk_8017d8f8(void) {
    allPartyForceSlitOff();
    return 2;
}

void winDisp(s32 cameraId) {
    winRootDisp(cameraId, wp);
}

void winReInit(void) {
    memset(wp, 0, 0x1218);
}

void winInit(void) {
    wp = __memAlloc(0, 0x1218);
    memset(wp, 0, 0x1218);
}

s32 winCheck(void) {
    void* work = wp;
    s32 value;

    if (work == 0) {
        return 0;
    }
    if ((*(u16*)work & 0x1000) != 0) {
        return 0;
    }
    value = *(s32*)((s32)work + 0x20);
    if (value != 0 && value < 200) {
        return 1;
    }
    return 0;
}

s32 famicom_check(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);

    if ((*(u16*)wp & 0x2000) != 0) {
        evtSetValue(evt, args[0], 1);
    } else {
        evtSetValue(evt, args[0], 0);
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 party_color(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    evtSetValue(evt, args[0], pouchGetPartyColor(4));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 winLectureCheck(void) {
    void* work = wp;

    if ((*(u16*)work & 0x1000) != 0 && evtCheckID(*(s32*)((s32)*(void**)((s32)work + 0x18) + 0x15C)) != 0) {
        return 1;
    }
    *(u16*)wp &= ~0x1000;
    return 0;
}

void winLectureOn(void) {
    *(u16*)wp |= 0x1000;
    *(u16*)wp |= 0x800;
    *(s32*)((s32)wp + 0x14) = 0;
    *(s32*)((s32)wp + 0x18) = evtEntry(evt_lecture_msg, 0, 0);
}

s32 unk_8017b864(void* evt, s32 flag) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 value = evtGetValue(evt, args[0]);
    void* work;

    if (flag != 0) {
        *(u16*)wp &= ~0x800;
    }
    work = wp;
    if (*(s32*)((s32)work + 0x14) < value) {
        return 0;
    }
    *(u16*)work |= 0x800;
    return 2;
}


u8 winMain(void) {
    return 0;
}


u8 winIconGrayInit(void) {
    return 0;
}


s32 itemUseFunc(void* pEvt, int param_2) {
    return 0;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winIconSet(s32 iconId, void* pos, void* size, void* color) {
    extern void iconGetTexObj(void* texObj, u16 iconId);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetTevColor(s32 reg, void* color);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    u8 texObj[0x20];
    f32 trans[3][4];
    f32 scale[3][4];
    f32 mtx[3][4];
    s32 tevColor;
    volatile f32* fifo;

    if (iconId == 0x11C) {
        iconId = 0x11B;
    }
    iconGetTexObj(texObj, (u16)iconId);
    GXInitTexObjLOD(texObj, 1, 1, float_0_80424004, float_0_80424004, float_0_80424004, 0, 0, 0);
    GXLoadTexObj(texObj, 0);

    if (((u8*)color)[3] != 0xFF) {
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 7, 0);
    } else {
        GXSetBlendMode(0, 4, 5, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(0, 7, 0);
    }

    tevColor = *(s32*)color;
    GXSetTevColor(1, &tevColor);
    PSMTXTrans(trans, ((f32*)pos)[0], ((f32*)pos)[1], ((f32*)pos)[2]);
    PSMTXScale(scale, ((f32*)size)[0] * GXGetTexObjWidth(texObj), ((f32*)size)[1] * GXGetTexObjHeight(texObj), ((f32*)size)[2]);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat((void*)((s32)camGetPtr(8) + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    fifo = (volatile f32*)0xCC008000;
    *fifo = -0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    *fifo = -0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevSwapMode(2, 0, 0);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winTexSetRot(s32 texId, void* pos, void* size, void* color, f32 angle) {
    extern f32 float_deg2rad_80424010;
    extern void TEXGetGXTexObjFromPalette(s32 tpl, void* texObj, s32 id);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetTevColor(s32 reg, void* color);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    u8 texObj[0x20];
    f32 mtx[3][4];
    f32 rot[3][4];
    f32 scale[3][4];
    f32 trans[3][4];
    s32 tevColor;
    volatile f32* fifo;

    if (((u8*)color)[3] != 0xFF) {
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 7, 0);
    } else {
        GXSetBlendMode(0, 4, 5, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(0, 7, 0);
    }

    TEXGetGXTexObjFromPalette(winTexTpl, texObj, texId);
    GXInitTexObjLOD(texObj, 0, 0, float_0_80424004, float_0_80424004, float_0_80424004, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    tevColor = *(s32*)color;
    GXSetTevColor(1, &tevColor);
    PSMTXTrans(trans, ((f32*)pos)[0], ((f32*)pos)[1], ((f32*)pos)[2]);
    PSMTXRotRad(rot, 0x7A, float_deg2rad_80424010 * angle);
    PSMTXScale(scale, ((f32*)size)[0] * GXGetTexObjWidth(texObj), ((f32*)size)[1] * GXGetTexObjHeight(texObj), ((f32*)size)[2]);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat((void*)((s32)camGetPtr(8) + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    fifo = (volatile f32*)0xCC008000;
    *fifo = -0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    *fifo = -0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 winTexInit_x2(s32 texData) {
    s32 color;

    winTexTpl = texData;
    GXSetCullMode(0);
    color = dat_80423ff8;
    GXSetFog(0, float_0_80424004, float_0_80424004, float_0_80424004, float_0_80424004, &color);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 2, 0, 0xF);
    GXSetTevAlphaIn(1, 7, 1, 4, 7);
    GXSetTevSwapMode(1, 0, 0);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winTexSet(s32 texId, void* pos, void* size, void* color) {
    extern void TEXGetGXTexObjFromPalette(s32 tpl, void* texObj, s32 id);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetTevColor(s32 reg, void* color);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    u8 texObj[0x20];
    f32 trans[3][4];
    f32 scale[3][4];
    f32 mtx[3][4];
    s32 tevColor;
    volatile f32* fifo;

    if (((u8*)color)[3] != 0xFF) {
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 7, 0);
    } else {
        GXSetBlendMode(0, 4, 5, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(0, 7, 0);
    }

    TEXGetGXTexObjFromPalette(winTexTpl, texObj, texId);
    GXInitTexObjLOD(texObj, 0, 0, float_0_80424004, float_0_80424004, float_0_80424004, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    tevColor = *(s32*)color;
    GXSetTevColor(1, &tevColor);
    PSMTXTrans(trans, ((f32*)pos)[0], ((f32*)pos)[1], ((f32*)pos)[2]);
    PSMTXScale(scale, ((f32*)size)[0] * GXGetTexObjWidth(texObj), ((f32*)size)[1] * GXGetTexObjHeight(texObj), ((f32*)size)[2]);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat((void*)((s32)camGetPtr(8) + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    fifo = (volatile f32*)0xCC008000;
    *fifo = -0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    *fifo = -0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 winTexSet_x2(s32 texId0, s32 texId1, void* pos, void* size, void* color) {
    extern void TEXGetGXTexObjFromPalette(s32 tpl, void* texObj, s32 id);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetTevColor(s32 reg, void* color);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    u8 texObj[0x20];
    f32 trans[3][4];
    f32 scale[3][4];
    f32 mtx[3][4];
    s32 tevColor;
    volatile f32* fifo;

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 7, 0);

    TEXGetGXTexObjFromPalette(winTexTpl, texObj, texId0);
    GXInitTexObjLOD(texObj, 0, 0, float_0_80424004, float_0_80424004, float_0_80424004, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    TEXGetGXTexObjFromPalette(winTexTpl, texObj, texId1);
    GXInitTexObjLOD(texObj, 0, 0, float_0_80424004, float_0_80424004, float_0_80424004, 0, 0, 0);
    GXLoadTexObj(texObj, 1);

    tevColor = *(s32*)color;
    GXSetTevColor(1, &tevColor);
    PSMTXTrans(trans, ((f32*)pos)[0], ((f32*)pos)[1], ((f32*)pos)[2]);
    PSMTXScale(scale, ((f32*)size)[0] * GXGetTexObjWidth(texObj), ((f32*)size)[1] * GXGetTexObjHeight(texObj), ((f32*)size)[2]);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat((void*)((s32)camGetPtr(8) + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    fifo = (volatile f32*)0xCC008000;
    *fifo = -0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    *fifo = -0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_8017c9bc(s32 texId, void* pos, void* size, void* color) {
    extern void TEXGetGXTexObjFromPalette(s32 tpl, void* texObj, s32 id);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetTevColor(s32 reg, void* color);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    u8 texObj[0x20];
    f32 trans[3][4];
    f32 scale[3][4];
    f32 mtx[3][4];
    s32 tevColor;
    volatile f32* fifo;

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 7, 0);

    TEXGetGXTexObjFromPalette(winTexTpl, texObj, texId);
    GXInitTexObjLOD(texObj, 0, 0, float_0_80424004, float_0_80424004, float_0_80424004, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    tevColor = *(s32*)color;
    GXSetTevColor(1, &tevColor);
    PSMTXTrans(trans, ((f32*)pos)[0], ((f32*)pos)[1], ((f32*)pos)[2]);
    PSMTXScale(scale, ((f32*)size)[0] * GXGetTexObjWidth(texObj), ((f32*)size)[1] * GXGetTexObjHeight(texObj), ((f32*)size)[2]);
    PSMTXConcat(trans, scale, mtx);
    PSMTXConcat((void*)((s32)camGetPtr(8) + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    fifo = (volatile f32*)0xCC008000;
    *fifo = -0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = -0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
    *fifo = 0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    *fifo = -0.5f; *fifo = 0.5f; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 winDispKoopa(void) {
    extern s32 dat_80423f58;
    extern f32 vec3_802f7150[3];
    extern f32 vec3_802f715c[3];
    s32 color;
    f32 size[3];
    f32 pos[3];

    winTexTpl = *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x28) + 0xA0));
    GXSetCullMode(0);
    color = dat_80423ff8;
    GXSetFog(0, float_0_80424004, float_0_80424004, float_0_80424004, float_0_80424004, &color);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    color = dat_80423f58;
    size[0] = vec3_802f715c[0];
    size[1] = vec3_802f715c[1];
    size[2] = vec3_802f715c[2];
    pos[0] = vec3_802f7150[0];
    pos[1] = vec3_802f7150[1];
    pos[2] = vec3_802f7150[2];
    winTexSet(0xB7, pos, size, &color);
}

u8 winLectureKeyMask(void) {
    extern u16 keyGetButtonTrg(s32 controller);
    void* work;
    s32 state;

    work = wp;
    if ((*(u16*)work & 0x1000) != 0) {
        if ((*(u16*)work & 0x800) != 0) {
            *(s32*)((s32)work + 4) = 0;
            *(s32*)((s32)wp + 8) = 0;
            *(s32*)((s32)wp + 0xC) = 0;
            *(s32*)((s32)wp + 0x10) = 0;
        } else {
            state = *(s32*)((s32)work + 0x14);
            if (state < 5) {
                if (state < 2) {
                    if (state >= 0) {
                        *(s32*)((s32)work + 4) = 0;
                        *(s32*)((s32)wp + 8) = 0;
                        *(s32*)((s32)wp + 0xC) = 0;
                        *(s32*)((s32)wp + 0x10) = 0;
                        if ((keyGetButtonTrg(0) & 0x100) != 0) {
                            *(s32*)((s32)wp + 0x14) = *(s32*)((s32)wp + 0x14) + 1;
                        }
                    }
                } else {
                    *(s32*)((s32)work + 4) = 0;
                    *(s32*)((s32)wp + 8) = 0;
                    *(s32*)((s32)wp + 0xC) &= 0x8000;
                    *(s32*)((s32)wp + 0x10) &= 0x8000;
                    if (*(s32*)((s32)wp + 0xC) != 0 || *(s32*)((s32)wp + 0x10) != 0) {
                        *(s32*)((s32)wp + 0x14) = *(s32*)((s32)wp + 0x14) + 1;
                    }
                }
            } else if (state == 9) {
                *(s32*)((s32)work + 4) &= 0x1000;
                *(s32*)((s32)wp + 8) &= 0x1000;
                *(s32*)((s32)wp + 0xC) = 0;
                *(s32*)((s32)wp + 0x10) = 0;
                if (*(s32*)((s32)wp + 4) != 0 || *(s32*)((s32)wp + 8) != 0) {
                    *(s32*)((s32)wp + 0x14) = *(s32*)((s32)wp + 0x14) + 1;
                }
            } else if (state < 9) {
                *(s32*)((s32)work + 4) &= 0x100;
                *(s32*)((s32)wp + 8) &= 0x100;
                *(s32*)((s32)wp + 0xC) = 0;
                *(s32*)((s32)wp + 0x10) = 0;
                if (*(s32*)((s32)wp + 4) != 0 || *(s32*)((s32)wp + 8) != 0) {
                    *(s32*)((s32)wp + 0x14) = *(s32*)((s32)wp + 0x14) + 1;
                }
            }
        }
    }
}

void winIconInit(void) {
    s32 color;

    winTexTpl = 0;
    GXSetCullMode(0);
    color = dat_80423ff8;
    GXSetFog(0, float_0_80424004, float_0_80424004, float_0_80424004, float_0_80424004, &color);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
}

void winTexInit(s32 texData) {
    s32 color;

    winTexTpl = texData;
    GXSetCullMode(0);
    color = dat_80423ff8;
    GXSetFog(0, float_0_80424004, float_0_80424004, float_0_80424004, float_0_80424004, &color);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
}

void winFontSetR(void* position, void* scale, void* color, char* format) {
    ;
}


void winFontSetEdge(void* position, void* scale, void* color, char* format) {
    ;
}


void winFontSet(void* position, void* scale, void* color, char* format) {
    ;
}


s32 cam_r(void) {
    void* cam;
    f32 vec[3];
    f32 value;

    cam = camGetPtr(4);
    *(f32*)((s32)cam + 0x14C) = *(f32*)((s32)cam + 0x14C) + float_3_8042401c;
    value = (f32)sin((f32)*(s32*)((s32)gp + 0x1C) / float_10_80424020);
    PSVECSubtract((void*)((s32)cam + 0x18), (void*)((s32)cam + 0xC), vec);
    PSVECScale(vec, vec, float_0p2_80424024 * value);
    *(f32*)((s32)cam + 0x150) = vec[0];
    *(f32*)((s32)cam + 0x154) = vec[1];
    *(f32*)((s32)cam + 0x158) = vec[2];
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winFontSetLabel(void* position, void* scale, void* color, char* label) {
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    s32 colorValue;

    if (label != NULL) {
        PSMTXTrans(transMtx,
                  *(f32*)((s32)position + 0x0),
                  *(f32*)((s32)position + 0x4),
                  *(f32*)((s32)position + 0x8));
        PSMTXScale(scaleMtx,
                   *(f32*)((s32)scale + 0x0),
                   *(f32*)((s32)scale + 0x4),
                   *(f32*)((s32)scale + 0x8));
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        colorValue = *(s32*)color;
        FontDrawColor(&colorValue);
        FontDrawMessageMtx(transMtx, msgSearch(label));
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u32 winGhostDiaryChk(void) {
    extern s32 seqGetSeq(void);

    if (*(s32*)((s32)wp + 0x20) == 0x12D) {
        return 1;
    }
    return seqGetSeq() == 5;
}


/* CHATGPT FALLBACK MISSING STUBS: main/window/win_main 20260624_191429 */

/* fallback stub-fill: map=unk_8017d10c addr=0x8017d10c size=0x00000180 */
int unk_8017d10c() {
    return 0;
}

/* fallback stub-fill: map=winFontSetPitch addr=0x8017d52c size=0x00000180 */
int winFontSetPitch() {
    return 0;
}
