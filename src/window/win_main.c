#include "window/win_main.h"

void* wp;
void* memset(void*, int, unsigned long);
void* __memAlloc(s32 heap, u32 size);
void FontDrawStart(void);
void statusWinForceOff(void);
void allPartyForceSlitOff(void);
void winRootDisp(s32 cameraId, void* work);
s32 evtSetValue(void* evt, s32 var, s32 value);
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


u8 winMain(void) {
    return 0;
}


u8 winIconGrayInit(void) {
    return 0;
}


s32 itemUseFunc(void* pEvt, int param_2) {
    return 0;
}


u8 winIconSet(void) {
    return 0;
}


u8 winTexSetRot(void) {
    return 0;
}


u8 winTexInit_x2(s32 param_1) {
    return 0;
}


void winTexSet(s32 texId, void* pos, void* size, void* color) {
}


u8 winTexSet_x2(void) {
    return 0;
}


void unk_8017c9bc(s32 msg, void* pos, void* size, void* color) {
}


u8 winDispKoopa(void) {
    return 0;
}


u8 winLectureKeyMask(void) {
    return 0;
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
}


void winFontSetEdge(void* position, void* scale, void* color, char* format) {
}


void winFontSet(void* position, void* scale, void* color, char* format) {
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
    return 0;
}
