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


u8 winIconInit(void) {
    return 0;
}


void winTexInit(s32 texData) {
}


void winFontSetR(void* position, void* scale, void* color, char* format) {
}


void winFontSetEdge(void* position, void* scale, void* color, char* format) {
}


void winFontSet(void* position, void* scale, void* color, char* format) {
}


u8 cam_r(void) {
    return 0;
}


void winFontSetLabel(void* position, void* scale, void* color, char* label) {
}


u32 winGhostDiaryChk(void) {
    return 0;
}
