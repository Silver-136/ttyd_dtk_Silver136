#include "mario/mariost.h"

#include "battle/battle.h"

#include "driver/animdrv.h"
#include "driver/arcdrv.h"
#include "driver/bgdrv.h"
#include "driver/camdrv.h"
#include "driver/casedrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/envdrv.h"
#include "driver/extdrv.h"
#include "driver/fadedrv.h"
#include "driver/hitdrv.h"
#include "driver/icondrv.h"
#include "driver/imgdrv.h"
#include "driver/itemdrv.h"
#include "driver/lightdrv.h"
#include "driver/mapdrv.h"
#include "driver/mobjdrv.h"
#include "driver/msgdrv.h"
#include "driver/npcdrv.h"
#include "driver/offscreendrv.h"
#include "driver/seqdrv.h"
#include "driver/shadowdrv.h"
#include "driver/swdrv.h"
#include "driver/windowdrv.h"

#include "evt/evt_badgeshop.h"
#include "evt/evt_johoya.h"
#include "evt/evt_yuugijou.h"

#include "mario/mario.h"

#include "manager/arammgr.h"
#include "manager/cardmgr.h"
#include "manager/dvdmgr.h"
#include "manager/evtmgr.h"
#include "manager/filemgr.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"

#include "sdk/DEMOInit.h"

#include "win/win_main.h"

#include "countdown.h"
#include "error_handler.h"
#include "memory.h"
#include "nameent.h"
#include "pmario_sound.h"
#include "romfont.h"
#include "statuswindow.h"
#include "system.h"

#include <dolphin/vi.h>
#include <string.h>
#include <demo/DEMOPad.h>

extern void SoundSetOutputMode(s32 mode);
extern void SoundOpenCover(void);
extern void SoundCloseCover(void);
#define OSIsRestart() ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

//.bss
GlobalWork marioSt;
OSThread DvdCheckThread;

//.sdata
GlobalWork* gp = &marioSt;

//.sbss
OSTime none_key;
s32 _mariostSystemLevel;
#ifdef VERSION_G8ME01
BOOL R_bReset; // before DvdCheckTreadOn: .sbss is emitted in reverse
#endif
BOOL DvdCheckTreadOn;

//.data
GXRenderModeObj sRMObjHReso = {
    VI_TVMODE_NTSC_INT, //viTVmode
    0x260, //fbWidth
    0x1C0, //efbHeight
    0x1C0, //xfbHeight
    0x1B, //viXOrigin
    0x10, //viYOrigin
    0x29A, //viWidth
    0x1C0, //viHeight
    VI_XFBMODE_DF, //xFBmode
    0, //field_rendering
    0, //aa
    6, 6, 6, 6, 6, 6, //pix 0
    6, 6, 6, 6, 6, 6, //pix 1
    6, 6, 6, 6, 6, 6, //pix 2
    6, 6, 6, 6, 6, 6, //pix 3
    8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj sRMObjHReso_prog = {
    VI_TVMODE_NTSC_PROG, //viTVmode
    0x260, //fbWidth
    0x1C0, //efbHeight
    0x1C0, //xfbHeight
    0x1B, //viXOrigin
    0x10, //viYOrigin
    0x29A, //viWidth
    0x1C0, //viHeight
    VI_XFBMODE_SF, //xFBmode
    0, //field_rendering
    0, //aa
    6, 6, 6, 6, 6, 6, //pix 0
    6, 6, 6, 6, 6, 6, //pix 1
    6, 6, 6, 6, 6, 6, //pix 2
    6, 6, 6, 6, 6, 6, //pix 3
    8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj GXNtsc480IntDfMarioSt = {
    VI_TVMODE_NTSC_INT, //viTVmode
    0x260, //fbWidth
    0x1E0, //efbHeight
    0x1E0, //xfbHeight
    0x1E, //viXOrigin
    0x0, //viYOrigin
    0x294, //viWidth
    0x1E0, //viHeight
    VI_XFBMODE_DF, //xFBmode
    0, //field_rendering
    0, //aa
    6, 6, 6, 6, 6, 6, //pix 0
    6, 6, 6, 6, 6, 6, //pix 1
    6, 6, 6, 6, 6, 6, //pix 2
    6, 6, 6, 6, 6, 6, //pix 3
    8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj GXNtsc480ProgMarioSt = {
    VI_TVMODE_NTSC_PROG, //viTVmode
    0x260, //fbWidth
    0x1E0, //efbHeight
    0x1E0, //xfbHeight
    0x1E, //viXOrigin
    0x0, //viYOrigin
    0x294, //viWidth
    0x1E0, //viHeight
    VI_XFBMODE_SF, //xFBmode
    0, //field_rendering
    0, //aa
    6, 6, 6, 6, 6, 6, //pix 0
    6, 6, 6, 6, 6, 6, //pix 1
    6, 6, 6, 6, 6, 6, //pix 2
    6, 6, 6, 6, 6, 6, //pix 3
    8, 8, 10, 12, 10, 8, 8, //vfilter
};

//local prototypes
void viPostCallback(u32 retraceCount);
void* gcDvdCheckThread(void* param);
void gcRumbleCheck(void);
void L_gcResetCheck(void);


void marioStInit(void) {
    static u8 stack[0x1000];
    if (!OSIsRestart() || (OSGetResetCode() & OS_RESET_SHUTDOWN)) {
        DEMOInit(&GXNtsc480IntDfMarioSt);
    }
    else {
        switch (OSGetResetCode() & OS_RESET_HOTRESET) {
            case 0:
                DEMOInit(&GXNtsc480IntDfMarioSt);
                break;
            case 1:
                DEMOInit(&GXNtsc480ProgMarioSt);
                break;
        }
    }
    DEMOEnableGPHangWorkaround(5);
    N_setupErrorHandler();
    DEMOPadInit();
    GXSetCopyClear((GXColor){0, 0, 0, 0}, 0xFFFFFF);
    __GXSetIndirectMask(0);
    memset(gp, 0, sizeof(GlobalWork));
    gp->fbWidth = DEMOGetRenderModeObj()->fbWidth;
    gp->efbHeight = DEMOGetRenderModeObj()->efbHeight;
    gp->rumbleEnabled = 1;
    gp->soundMode = 0;

    gp->retraceLocalTime = 0;
    gp->retraceDeltaTime = 0;
    gp->retraceTime = OSGetTime();

    gp->renderFieldTime = 0;
    gp->renderTime = 0;

    gp->systemLv3Time = 0;
    gp->systemLv2Time = 0;
    gp->systemLv1Time = 0;
    gp->systemLv0Time = 0;

    gp->mapFadeOutType = FADE_IN_BLACK;
    gp->mapFadeOutDuration = 300;
    gp->mapFadeInType = FADE_OUT_BLACK;
    gp->mapFadeInDuration = 300;
    gp->areaFadeOutType = FADE_IN_BLACK;
    gp->areaFadeOutDuration = 300;
    gp->areaFadeInType = FADE_OUT_BLACK;
    gp->areaFadeInDuration = 300;
#ifdef VERSION_G8ME01
    if (VIGetTvFormat() == 0)
        gp->framerate = 60;
    else
        gp->framerate = 50;
    gp->language = 1;
#else
    if ((OSGetFontEncode() == OS_FONT_ENCODE_SJIS) && !(gp->flags & 0x1000))
        gp->language = 0;
    else
        gp->language = 1;
    gp->framerate = 60;
#endif
    gp->systemLevelFlags = 0;
    badgeShop_init(); //Badge Shop
    yuugijou_init(); //Pianta Parlor
    johoya_init(); //information broker (johoya)
#ifdef VERSION_G8ME01
    gp->mapAlloc = __memAlloc(HEAP_DEFAULT, 670 * 1024);
#else
    gp->mapAlloc = __memAlloc(HEAP_DEFAULT, 660 * 1024);
#endif
    VISetPostRetraceCallback(viPostCallback);
    romFontInit();
    OSCreateThread(&DvdCheckThread, gcDvdCheckThread, NULL, stack + sizeof(stack), sizeof(stack), 16, OS_THREAD_ATTR_DETACH);
    DvdCheckTreadOn = TRUE;
    OSResumeThread(&DvdCheckThread);
    DVDMgrInit();
    psndInit();
    aramMgrInit();
    fileInit();
    smartInit();
    dispInit();
    camInit();
    fontmgrInit();
    windowInit();
    mapInit();
    hitInit();
    itemInit();
    iconInit();
    fadeInit();
    bgInit();
    shadowInit();
    evtmgrInit();
    animInit();
    msgInit();
    npcInit();
    mobjInit();
    effInit();
    marioInit();
    caseInit();
    imgInit();
    lightInit();
    offscreenInit();
    arcInit();
    extInit();
    swInit();
    cardInit();
    winInit();
    statusWinInit();
    envInit();
    countDownInit();
    nameEntInit();
    winMgrInit();
    seqInit_MARIOSTORY();
    seqSetSeq(SEQ_LOGO, NULL, NULL);
    gp->soundMode = 0;
    if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
        SoundSetOutputMode(0);
    }
    else {
        SoundSetOutputMode(1);
    }
}

void marioStMain(void) {
    int i;

    while (gp->dvdError) {
        OSYieldThread();
    }
    makeKey();
    seqMain();
    cardMain();
    bgMain();
    shadowMain();
    animMain();
    battleMain();
    camMain();
    windowMain();
    mapMain();
    hitMain();
    evtmgrMain();
    mapDisp();
    marioMain();
    mobjMain();
    npcMain();
    fadeMain();
    imgMain();
    lightMain();
    offscreenMain();
    itemMain();
    caseMain();
    iconMain();
    extMain();
    psndMain();
    winMain();
    statusWinMain();
    envMain();
    countDownMain();
    nameEntMain();
    winMgrMain();
    effMain();
    L_gcResetCheck();
    if (gp->flags & 0x1000) {
        for (i = 0; i < 4; i++) {
            if (gp->dirs[i]) break;
            if (gp->button[i]) break;
            if (gp->stickX[i]) break;
            if (gp->stickY[i]) break;
            if (gp->substickX[i]) break;
            if (gp->substickY[i]) break;
            if (gp->triggerLeft[i]) break;
            if (gp->triggerRight[i]) break;
        }
        if (i >= 4) {
            if (OSTicksToSeconds(OSGetTime() - none_key) > 120 && (seqGetSeq() == 2 || seqGetSeq() == 4)) {
                gp->flags |= 0x2000;
            }
        }
        else {
            none_key = OSGetTime();
        }
    }
}

void marioStDisp(void) {
    camDraw();
    g_bFirstSmartAlloc = FALSE;
}

void marioStSystemLevel(s32 level) {
    switch (level) {
        case 0:
            _mariostSystemLevel = level;
            gp->systemLevelFlags &= ~0xF;
            evtStartAll(0xEF);
            break;
        case 1:
            _mariostSystemLevel = level;
            gp->systemLevelFlags &= ~0xE;
            gp->systemLevelFlags |= 1;
            evtStopAll(1);
            break;
        case 2:
            _mariostSystemLevel = level;
            gp->systemLevelFlags &= ~0xC;
            gp->systemLevelFlags |= 3;
            evtStopAll(2);
            break;
        case 3:
            _mariostSystemLevel = level;
            gp->systemLevelFlags &= ~8;
            gp->systemLevelFlags |= 7;
            evtStopAll(0x10);
            break;
        case 4:
            _mariostSystemLevel = level;
            gp->systemLevelFlags |= 0xF;
            evtStopAll(0xEF);
            break;
    }
}

u32 marioStGetSystemLevel(void) {
    return gp->systemLevelFlags;
}

void viPostCallback(u32 retraceCount) {
    OSTime time;
    u32 reset = PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_START;

    time = OSGetTime();
    gp->retraceDeltaTime = time - gp->retraceTime;
    gp->retraceLocalTime += gp->retraceDeltaTime;
    if (gp->retraceLocalTime > (OS_TIMER_CLOCK * (OSTime)359999999)) {
        gp->retraceLocalTime = (OS_TIMER_CLOCK * (OSTime)359999999);
    }
#ifdef VERSION_G8ME01
    if (!gp->dvdError && !winCheck()) {
        if (!(gp->systemLevelFlags & 8)) {
            gp->systemLv3Time += gp->retraceDeltaTime;
        }
        if (!(gp->systemLevelFlags & 4)) {
            gp->systemLv2Time += gp->retraceDeltaTime;
        }
        if (!(gp->systemLevelFlags & 2)) {
            gp->systemLv1Time += gp->retraceDeltaTime;
        }
        if (!(gp->systemLevelFlags & 1)) {
            gp->systemLv0Time += gp->retraceDeltaTime;
        }
#else
    if (!(gp->systemLevelFlags & 8)) {
        gp->systemLv3Time += gp->retraceDeltaTime;
    }
    if (!(gp->systemLevelFlags & 4)) {
        gp->systemLv2Time += gp->retraceDeltaTime;
    }
    if (!(gp->systemLevelFlags & 2)) {
        gp->systemLv1Time += gp->retraceDeltaTime;
    }
    if (!(gp->systemLevelFlags & 1)) {
        gp->systemLv0Time += gp->retraceDeltaTime;
    }
    if (!gp->dvdError && !winCheck()) {
#endif
        gp->renderTime += gp->retraceDeltaTime;
        if (!gp->inBattle) {
            gp->renderFieldTime += gp->retraceDeltaTime;
        }
    }
    gp->retraceTime = time;
    gp->retraceCount = retraceCount;
    if (battleDisableHResetCheck()) {
        gp->resetButtonDown = FALSE;
    }
    else {
        if (!gp->resetButtonDown) {
            if (OSGetResetButtonState()) {
                gp->resetButtonDown = TRUE;
            }
        }
        else {
            if (!OSGetResetButtonState()) {
                gp->resetType = 1;
            }
        }
        if (gp->keyDataValid) {
            if (OSIsRestart() && !gp->restartChecked) {
                if ((keyGetButton(0) & reset) == reset) {
                    gp->resetType = 0;
                    goto exitcheck;
                }
                else {
                    gp->restartChecked = TRUE;
                }
            }
            if ((keyGetButton(0) & reset) == reset) {
                if (!gp->softResetWait) {
                    gp->softResetWait = TRUE;
                    gp->softResetTimeout = OSGetTime();
                }
                if (OSTicksToMilliseconds(OSGetTime() - gp->softResetTimeout) > 500) {
                    gp->resetType = 1;
                }
            }
            else {
                gp->softResetWait = FALSE;
            }
            if (DVDGetDriveStatus() == -1) {
                gp->resetType = 0;
            }
        }
    }
exitcheck:
    gcRumbleCheck();
    psndMainInt();
}

void* gcDvdCheckThread(void* param) {
#ifdef VERSION_G8ME01
    s32 prevStatus = 0;
    s32 stableCount = 0;
    u32 prevRetrace = 0;
#endif
    const char* message;
    s32 msgId;
    s32 width;
    f32 xpos;
    Mtx44 mtx;

    while (1) {
        msgId = 0;
        switch (DVDGetDriveStatus()) {
            case DVD_STATE_COVER_OPEN:
#ifdef VERSION_G8ME01
                msgId = 10;
#else
                msgId = 5;
#endif
                gp->dvdError = TRUE;
                break;
            case DVD_STATE_NO_DISK:
#ifdef VERSION_G8ME01
                msgId = 11;
#else
                msgId = 6;
#endif
                gp->dvdError = TRUE;
                break;
            case DVD_STATE_WRONG_DISK:
#ifdef VERSION_G8ME01
                msgId = 11;
#else
                msgId = 6;
#endif
                gp->dvdError = TRUE;
                break;
            case DVD_STATE_RETRY:
#ifdef VERSION_G8ME01
                msgId = 12;
#else
                msgId = 7;
#endif
                gp->dvdError = TRUE;
                break;
            case DVD_RESULT_FATAL_ERROR:
#ifdef VERSION_G8ME01
                msgId = 13;
#else
                msgId = 8;
#endif
                gp->dvdError = TRUE;
                break;
            default:
                gp->dvdError = FALSE;
                break;
            case DVD_STATE_BUSY:
                break;
        }
#ifdef VERSION_G8ME01
        if (DVDGetDriveStatus() != prevStatus) {
            prevStatus = DVDGetDriveStatus();
            stableCount = 0;
        } else if (VIGetRetraceCount() != prevRetrace) {
            prevRetrace = VIGetRetraceCount();
            stableCount++;
            if (stableCount > 100) {
                stableCount = 100;
            }
        }
#endif
        if (gp->dvdError) {
            SoundOpenCover();
        }
        else {
            SoundCloseCover();
        }
        if (gp->dvdError) {
            if (cardIsExec()) {
                gp->dvdError = FALSE;
            }
        }
#ifdef VERSION_G8ME01
        if (gp->dvdError && stableCount > 10) {
#else
        if (gp->dvdError) {
#endif
            DEMOBeforeRender();
            if (msgId) {
                message = romFontGetMessage(msgId);
                width = (f32)romFontGetWidth(message);
                MTXPerspective(mtx, 25.0f, 1.2666667f, 1.0f, 10000.0f);
                GXSetProjection(mtx, GX_PERSPECTIVE);
                xpos = -width / 2.0f;
                romFontPrintGX(xpos, 60.0f, 1.0f, &(GXColor){0xFF, 0xFF, 0xFF, 0xFF}, message);
            }
            DEMODoneRender();
            makeKey();
            L_gcResetCheck();
        }
        OSYieldThread();
    }
}

void gcRumbleCheck(void) {
    static BOOL off_trg[4] = {FALSE, FALSE, FALSE, FALSE};
    static OSTime off_time[4] = {0, 0, 0, 0};
    OSTime now;
    OSTime diff;
    BOOL array[4] = {0};
    int i;

    now = OSGetTime();
    for (i = 0; i < 4; i++) {
        if (!gp->rumbleForcedOff[i]) {
            if (padGetRumbleStatus(i) == 1) {
                if (!gp->rumbleStart[i]) {
                    gp->rumbleStart[i] = TRUE;
                    gp->rumbleStartTime[i] = now;
                }
diff = (s32)now - (s32)gp->rumbleStartTime[i];
                if (OSTicksToMilliseconds(diff) > 30000) {
                    if (!gp->rumbleForcedOffLatch[i]) {
                        gp->rumbleForcedOffLatch[i] = TRUE;
                        gp->rumbleForcedOff[i] = TRUE;
                        gp->rumbleForcedOffTime[i] = now;
                    }
                }
                off_trg[i] = FALSE;
            }
            else if (gp->rumbleStart[i]) {
                if (!off_trg[i]) {
                    off_trg[i] = TRUE;
                    off_time[i] = now;
                }
                if (off_trg[i]) {
diff = (s32)now - (s32)off_time[i];
                    if (OSTicksToMilliseconds(diff) > 100) {
                        off_trg[i] = FALSE;
                        gp->rumbleStart[i] = FALSE;
                    }
                }
            }
        }
        else {
diff = (s32)now - (s32)gp->rumbleForcedOffTime[i];
            if (OSTicksToMilliseconds(diff) > 30000) {
                gp->rumbleForcedOffLatch[i] = FALSE;
                gp->rumbleForcedOff[i] = FALSE;
                gp->rumbleStart[i] = FALSE;
            }
            array[i] = TRUE;
        }
        if (gp->dvdError) {
            array[i] = TRUE;
        }
        if (gp->systemLevelFlags != 0) {
            array[i] = TRUE;
        }
        if (array[i]) {
            gp->rumbleHardStop[i] = TRUE;
        }
        else {
            gp->rumbleHardStop[i] = FALSE;
        }
    }
}

void L_gcResetCheck(void) {
    u32 code;

#ifdef VERSION_G8ME01
    if ((gp->resetType || R_bReset) && (R_bReset = TRUE, !cardIsExec())) {
#else
    if (gp->resetType) {
#endif
        VISetBlack(TRUE);
        VIFlush();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        psndExit();
        DVDMgrDelete();
        if ((OSGetCurrentThread() != &DvdCheckThread) && DvdCheckTreadOn) {
            OSCancelThread(&DvdCheckThread);
            DvdCheckTreadOn = FALSE;
        }
        while (cardIsExec()) {
            cardMain();
            VIWaitForRetrace();
        }
        if (gp->resetType != 2) {
            if (!(gp->flags & 4)) {
                code = OS_RESET_SHUTDOWN;
            }
            else {
                if (!(gp->flags & 8)) {
                    code = OS_RESET_RESTART;
                }
                else {
                    code = OS_RESET_HOTRESET;
                }
            }
#ifdef VERSION_G8ME01
            code |= gp->language << 2;
#endif
            OSResetSystem(OS_RESET_RESTART, code, FALSE);
        }
        else {
            OSResetSystem(OS_RESET_HOTRESET, 0, TRUE);
        }
        while (1) {}
    }
}

// forward declarations: these functions are defined further down
void L_gcResetCheck(void);
void gcRumbleCheck(void);
void* gcDvdCheckThread(void* param);
void viPostCallback(u32 retraceCount);
void marioStSystemLevel(s32 level);
void marioStDisp(void);
void marioStMain(void);
