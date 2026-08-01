#include "mario/mariost.h"
#include "countdown.h"
#include "driver/arcdrv.h"
#include "driver/dispdrv.h"
#include "driver/envdrv.h"
#include "driver/hitdrv.h"
#include "driver/lightdrv.h"
#include "driver/seqdrv.h"

#include "event/evt_badgeshop.h"
#include "driver/fadedrv.h"
#include "statuswindow.h"

extern u32 OSGetResetCode(void);
extern s64 OSGetTime(void);
extern s32 DvdCheckTreadOn;
extern void OSYieldThread(void);
extern void makeKey(void);
extern void cardMain(void);
extern void L_gcResetCheck(void);
extern s32 DVDGetDriveStatus(void);
extern s32 cardIsExec(void);
extern const u32 dat_80420340;
extern const f32 float_25_80420344;
extern const f32 float_1p2667_80420348;
extern const f32 float_1_8042034c;
extern const f32 float_10000_80420350;
extern const f32 float_0p5_80420354;
extern const f32 float_60_80420358;
extern const f32 float_0_8042035c;
extern const u8 unk_80429568[4];

u8 sRMObjHReso[0x3C] = {
    0x00, 0x00, 0x00, 0x00, 0x02, 0x60, 0x01, 0xC0,
    0x01, 0xC0, 0x00, 0x1B, 0x00, 0x10, 0x02, 0x9A,
    0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x08, 0x08, 0x0A, 0x0C, 0x0A, 0x08,
    0x08, 0x00, 0x00, 0x00,
};

u8 sRMObjHReso_prog[0x3C] = {
    0x00, 0x00, 0x00, 0x02, 0x02, 0x60, 0x01, 0xC0,
    0x01, 0xC0, 0x00, 0x1B, 0x00, 0x10, 0x02, 0x9A,
    0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x08, 0x08, 0x0A, 0x0C, 0x0A, 0x08,
    0x08, 0x00, 0x00, 0x00,
};

u8 GXNtsc480IntDfMarioSt[0x3C] = {
    0x00, 0x00, 0x00, 0x00, 0x02, 0x60, 0x01, 0xE0,
    0x01, 0xE0, 0x00, 0x1E, 0x00, 0x00, 0x02, 0x94,
    0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x08, 0x08, 0x0A, 0x0C, 0x0A, 0x08,
    0x08, 0x00, 0x00, 0x00,
};

u8 GXNtsc480ProgMarioSt[0x3C] = {
    0x00, 0x00, 0x00, 0x02, 0x02, 0x60, 0x01, 0xE0,
    0x01, 0xE0, 0x00, 0x1E, 0x00, 0x00, 0x02, 0x94,
    0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x08, 0x08, 0x0A, 0x0C, 0x0A, 0x08,
    0x08, 0x00, 0x00, 0x00,
};

const f32 vec3_802c2a70[3] = { 0.0f, 0.0f, 0.0f };

GlobalWork marioSt;
GlobalWork* gp = &marioSt;
s64 none_key;
s32 _mariostSystemLevel;
s32 R_bReset;
s32 DvdCheckTreadOn;
u8 DvdCheckThread[0x318];

void marioStInit(void) {
    extern void DEMOInit(void*);
    extern void DEMOEnableGPHangWorkaround(s32);
    extern void N_setupErrorHandler(void);
    extern void DEMOPadInit(void);
    extern void GXSetCopyClear(void*, u32);
    extern void __GXSetIndirectMask(u32);
    extern void* memset(void*, s32, u32);
    extern void* DEMOGetRenderModeObj(void);
    extern s32 VIGetTvFormat(void);
    extern void yuugijou_init(void);
    extern void johoya_init(void);
    extern void* __memAlloc(s32, u32);
    extern void VISetPostRetraceCallback(void*);
    extern void romFontInit(void);
    extern s32 OSCreateThread(void*, void*, void*, void*, u32, s32, s16);
    extern s32 OSResumeThread(void*);
    extern void DVDMgrInit(void);
    extern void psndInit(void);
    extern void aramMgrInit(void);
    extern void fileInit(void);
    extern void smartInit(void);
    extern void camInit(void);
    extern void fontmgrInit(void);
    extern void windowInit(void);
    extern void mapInit(void);
    extern void itemInit(void);
    extern void iconInit(void);
    extern void bgInit(void);
    extern void shadowInit(void);
    extern void evtmgrInit(void);
    extern void animInit(void);
    extern void msgInit(void);
    extern void npcInit(void);
    extern void mobjInit(void);
    extern void effInit(void);
    extern void marioInit(void);
    extern void caseInit(void);
    extern void imgInit(void);
    extern void offscreenInit(void);
    extern void extInit(void);
    extern void swInit(void);
    extern void cardInit(void);
    extern void winInit(void);
    extern void nameEntInit(void);
    extern void winMgrInit(void);
    extern void seqSetSeq(s32, void*, void*);
    extern u32 OSGetSoundMode(void);
    extern void SoundSetOutputMode(s32);
    extern u8 gcDvdCheckThread(void);
    extern u8 viPostCallback(s32);
    void* rmode;
    s64 now;
    u32 reset;
    u32 clear;
    static u8 stack[0x1000];

    reset = OSGetResetCode();
    if ((s32)reset < 0 && (OSGetResetCode() & 2) == 0) {
        if (OSGetResetCode() & 1) {
            rmode = GXNtsc480ProgMarioSt;
        } else {
            rmode = GXNtsc480IntDfMarioSt;
        }
    } else {
        rmode = GXNtsc480IntDfMarioSt;
    }
    DEMOInit(rmode);
    DEMOEnableGPHangWorkaround(5);
    N_setupErrorHandler();
    DEMOPadInit();
    clear = *(const u32*)unk_80429568;
    GXSetCopyClear(&clear, 0xFFFFFF);
    __GXSetIndirectMask(0);
    memset(gp, 0, 0x13D8);
    *(u16*)((s32)gp + 0x1268) = *(u16*)((s32)DEMOGetRenderModeObj() + 4);
    *(u16*)((s32)gp + 0x126A) = *(u16*)((s32)DEMOGetRenderModeObj() + 6);
    *(s32*)((s32)gp + 0x131C) = 1;
    *(s32*)((s32)gp + 0x1274) = 0;
    *(s64*)((s32)gp + 0x20) = 0;
    *(s64*)((s32)gp + 0x28) = 0;
    now = OSGetTime();
    *(s64*)((s32)gp + 0x30) = now;
    *(s64*)((s32)gp + 0x40) = 0;
    *(s64*)((s32)gp + 0x38) = 0;
    *(s64*)((s32)gp + 0x60) = 0;
    *(s64*)((s32)gp + 0x58) = 0;
    *(s64*)((s32)gp + 0x50) = 0;
    *(s64*)((s32)gp + 0x48) = 0;
    *(s32*)((s32)gp + 0x126C) = 9;
    *(s32*)((s32)gp + 0x1270) = 300;
    *(s32*)((s32)gp + 0x125C) = 10;
    *(s32*)((s32)gp + 0x1260) = 300;
    *(s32*)((s32)gp + 0x124C) = 1;
    *(s32*)((s32)gp + 0x1250) = 300;
    *(s32*)((s32)gp + 0x1254) = 2;
    *(s32*)((s32)gp + 0x1258) = 300;
    if (VIGetTvFormat() == 0) {
        *(s32*)((s32)gp + 4) = 60;
    } else {
        *(s32*)((s32)gp + 4) = 50;
    }
    *(s32*)((s32)gp + 8) = 1;
    *(s32*)((s32)gp + 0x18) = 0;
    badgeShop_init();
    yuugijou_init();
    johoya_init();
    *(void**)((s32)gp + 0x1284) = __memAlloc(0, 0xA7800);
    VISetPostRetraceCallback(viPostCallback);
    romFontInit();
    OSCreateThread(DvdCheckThread, gcDvdCheckThread, 0, stack + sizeof(stack), sizeof(stack), 0x10, 1);
    DvdCheckTreadOn = 1;
    OSResumeThread(DvdCheckThread);
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
    seqSetSeq(0, 0, 0);
    *(s32*)((s32)gp + 0x1274) = 0;
    if (OSGetSoundMode() == 0) {
        SoundSetOutputMode(0);
    } else {
        SoundSetOutputMode(1);
    }
}

void marioStMain(void) {
    extern void bgMain(void);
    extern void shadowMain(void);
    extern void animMain(void);
    extern void battleMain(void);
    extern void camMain(void);
    extern void windowMain(void);
    extern void mapMain(void);
    extern void evtmgrMain(void);
    extern void mapDisp(void);
    extern void marioMain(void);
    extern void mobjMain(void);
    extern void npcMain(void);
    extern void imgMain(void);
    extern void offscreenMain(void);
    extern void itemMain(void);
    extern void caseMain(void);
    extern void iconMain(void);
    extern void extMain(void);
    extern void psndMain(void);
    extern void winMain(void);
    extern void nameEntMain(void);
    extern void winMgrMain(void);
    extern void effMain(void);
    u8* bytes;
    u32* words;
    s32 i;
    s32 count;
    s64 now;

    while (*(s32*)((s32)gp + 0x10) != 0) {
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

    if ((*(u32*)gp & 0x1000) != 0) {
        words = (u32*)gp;
        bytes = (u8*)gp;
        count = 0;
        for (i = 0; i < 2; i++) {
            if (*(u32*)((u8*)words + 0x1378) != 0) goto input_found;
            if (*(u32*)((u8*)words + 0x1328) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13B8) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13BC) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C0) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C4) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13C8) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13CC) != 0) goto input_found;
            count++;
            if (*(u32*)((u8*)words + 0x137C) != 0) goto input_found;
            if (*(u32*)((u8*)words + 0x132C) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13B9) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13BD) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C1) != 0) goto input_found;
            if (*(s8*)(bytes + 0x13C5) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13C9) != 0) goto input_found;
            if (*(u8*)(bytes + 0x13CD) != 0) goto input_found;
            words = (u32*)((u8*)words + 8);
            bytes += 2;
            count++;
        }
input_found:
        if (count >= 4) {
            now = OSGetTime();
            if ((now - none_key) / (*(u32*)0x800000F8 >> 2) > 120) {
                if (seqGetSeq() == 2 || seqGetSeq() == 4) {
                    *(u32*)gp |= 0x2000;
                }
            }
        } else {
            none_key = OSGetTime();
        }
    }
}

void marioStDisp(void) {
    extern void camDraw(void);
    extern s32 g_bFirstSmartAlloc;

    camDraw();
    g_bFirstSmartAlloc = 0;
}

void marioStSystemLevel(s32 level) {
    extern void evtStartAll(s32 mask);
    extern void evtStopAll(s32 mask);

    switch (level) {
        case 0:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~0xF;
            evtStartAll(0xEF);
            break;
        case 1:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~0xE;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 1;
            evtStopAll(1);
            break;
        case 2:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~0xC;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 3;
            evtStopAll(2);
            break;
        case 3:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) & ~8;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 7;
            evtStopAll(0x10);
            break;
        case 4:
            _mariostSystemLevel = level;
            *(u32*)((s32)gp + 0x18) = *(u32*)((s32)gp + 0x18) | 0xF;
            evtStopAll(0xEF);
            break;
    }
}

s32 marioStGetSystemLevel(void) {
    return *(s32*)((s32)gp + 0x18);
}


u8 viPostCallback(s32 param_1) {
    extern s32 winCheck(void);
    extern u32 battleDisableHResetCheck(void);
    extern u32 OSGetResetButtonState(void);
    extern u32 keyGetButton(s32);
    extern u8 gcRumbleCheck(void);
    extern void psndMainInt(void);
    s64 now;
    s64 delta;
    u32 buttons;

    now = OSGetTime();
    delta = now - *(s64*)((s32)gp + 0x30);
    *(s64*)((s32)gp + 0x28) = delta;
    *(s64*)((s32)gp + 0x20) += delta;
    if (*(s64*)((s32)gp + 0x20) > ((s64)(*(u32*)0x800000F8 >> 2) * 359999999)) {
        *(s64*)((s32)gp + 0x20) = (s64)(*(u32*)0x800000F8 >> 2) * 359999999;
    }
    if (*(s32*)((s32)gp + 0x10) == 0 && winCheck() == 0) {
        if ((*(u32*)((s32)gp + 0x18) & 8) == 0) {
            *(s64*)((s32)gp + 0x60) += *(s64*)((s32)gp + 0x28);
        }
        if ((*(u32*)((s32)gp + 0x18) & 4) == 0) {
            *(s64*)((s32)gp + 0x58) += *(s64*)((s32)gp + 0x28);
        }
        if ((*(u32*)((s32)gp + 0x18) & 2) == 0) {
            *(s64*)((s32)gp + 0x50) += *(s64*)((s32)gp + 0x28);
        }
        if ((*(u32*)((s32)gp + 0x18) & 1) == 0) {
            *(s64*)((s32)gp + 0x48) += *(s64*)((s32)gp + 0x28);
        }
        *(s64*)((s32)gp + 0x38) += *(s64*)((s32)gp + 0x28);
        if (*(s32*)((s32)gp + 0x14) == 0) {
            *(s64*)((s32)gp + 0x40) += *(s64*)((s32)gp + 0x28);
        }
    }
    *(s64*)((s32)gp + 0x30) = now;
    *(s32*)((s32)gp + 0x1C) = param_1;
    if (battleDisableHResetCheck() == 0) {
        if (*(s32*)((s32)gp + 0x127C) == 0) {
            if (OSGetResetButtonState() != 0) {
                *(s32*)((s32)gp + 0x127C) = 1;
            }
        } else if (OSGetResetButtonState() == 0) {
            *(s32*)((s32)gp + 0x1278) = 1;
        }
        if (*(s32*)((s32)gp + 0x1324) != 0) {
            if ((s32)OSGetResetCode() < 0 && *(s32*)((s32)gp + 0x1290) == 0) {
                buttons = keyGetButton(0);
                if ((buttons & 0x1600) == 0x1600) {
                    *(s32*)((s32)gp + 0x1278) = 0;
                    gcRumbleCheck();
                    psndMainInt();
                    return 0;
                }
                *(s32*)((s32)gp + 0x1290) = 1;
            }
            buttons = keyGetButton(0);
            if ((buttons & 0x1600) == 0x1600) {
                if (*(s32*)((s32)gp + 0x1280) == 0) {
                    *(s32*)((s32)gp + 0x1280) = 1;
                    now = OSGetTime();
                    *(s32*)((s32)gp + 0x1288) = (s32)(now >> 32);
                    *(s32*)((s32)gp + 0x128C) = (s32)now;
                }
                now = OSGetTime();
                if ((now - *(s64*)((s32)gp + 0x1288)) / (*(u32*)0x800000F8 / 4000) > 500) {
                    *(s32*)((s32)gp + 0x1278) = 1;
                }
            } else {
                *(s32*)((s32)gp + 0x1280) = 0;
            }
            if (DVDGetDriveStatus() == -1) {
                *(s32*)((s32)gp + 0x1278) = 0;
            }
        }
    } else {
        *(s32*)((s32)gp + 0x127C) = 0;
    }
    gcRumbleCheck();
    psndMainInt();
    return 0;
}

void gcDvdCheckThread(void) {
    extern u32 VIGetRetraceCount(void);
    extern void SoundCloseCover(void);
    extern void SoundOpenCover(void);
    extern void DEMOBeforeRender(void);
    extern void DEMODoneRender(void);
    extern char* romFontGetMessage(s32 id);
    extern s32 romFontGetWidth(char* msg, ...);
    extern void C_MTXPerspective(void* mtx, f32 fovY, f32 aspect, f32 nearZ, f32 farZ);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void romFontPrintGX(f32 x, f32 y, f32 scale, void* color, char* msg);
    f32 projection[4][4];
    u32 color;
    char* message;
    s32 width;
    s32 status;
    s32 messageId;
    s32 lastStatus = 0;
    u32 lastRetrace = 0;
    s32 stableFrames = 0;

    for (;;) {
        messageId = 0;
        status = DVDGetDriveStatus();
        switch (status) {
            case 4:
            case 6:
                messageId = 11;
                *(s32*)((s32)gp + 0x1274) = 1;
                break;
            case 5:
                messageId = 10;
                *(s32*)((s32)gp + 0x1274) = 1;
                break;
            case 11:
                messageId = 12;
                *(s32*)((s32)gp + 0x1274) = 1;
                break;
            case -1:
                messageId = 13;
                *(s32*)((s32)gp + 0x1274) = 1;
                break;
            case 1:
                break;
            default:
                *(s32*)((s32)gp + 0x1274) = 0;
                break;
        }

        if (lastStatus == DVDGetDriveStatus()) {
            if (lastRetrace != VIGetRetraceCount()) {
                lastRetrace = VIGetRetraceCount();
                if (++stableFrames > 100) {
                    stableFrames = 100;
                }
            }
        } else {
            lastStatus = DVDGetDriveStatus();
            stableFrames = 0;
        }

        if (*(s32*)((s32)gp + 0x1274) == 0) {
            SoundCloseCover();
        } else {
            SoundOpenCover();
        }
        if (*(s32*)((s32)gp + 0x1274) != 0 && cardIsExec() != 0) {
            *(s32*)((s32)gp + 0x1274) = 0;
        }
        if (*(s32*)((s32)gp + 0x1274) != 0 && stableFrames > 10) {
            DEMOBeforeRender();
            if (messageId != 0) {
                message = romFontGetMessage(messageId);
                width = romFontGetWidth(message);
                C_MTXPerspective(projection, float_25_80420344, float_1p2667_80420348,
                                 float_1_8042034c, float_10000_80420350);
                GXSetProjection(projection, 0);
                color = dat_80420340;
                romFontPrintGX((f32)(-width) * float_0p5_80420354,
                               float_60_80420358, float_1_8042034c,
                               &color, message);
            }
            DEMODoneRender();
            makeKey();
            L_gcResetCheck();
        }
        OSYieldThread();
    }
}

u8 gcRumbleCheck(void) {
    extern u32 padGetRumbleStatus(s32);
    s32 local_flags[4];
    static s32 off_trg[4] = { 0 };
    static s64 off_time[4] = { 0 };
    s64 now;
    s32 i;

    local_flags[0] = 0;
    local_flags[1] = 0;
    local_flags[2] = 0;
    local_flags[3] = 0;
    now = OSGetTime();
    for (i = 0; i < 4; i++) {
        if (*(u8*)((s32)gp + 0x12E8 + i) == 0) {
            if ((padGetRumbleStatus(i) & 0xFF) == 1) {
                if (*(u8*)((s32)gp + 0x12C0 + i) == 0) {
                    *(u8*)((s32)gp + 0x12C0 + i) = 1;
                    *(s64*)((s32)gp + 0x12C8 + i * 8) = now;
                }
                if (((now - *(s64*)((s32)gp + 0x12C8 + i * 8)) /
                     (*(u32*)0x800000F8 / 4000) > 30000) &&
                    *(u8*)((s32)gp + 0x12EC + i) == 0) {
                    *(u8*)((s32)gp + 0x12EC + i) = 1;
                    *(u8*)((s32)gp + 0x12E8 + i) = 1;
                    *(s64*)((s32)gp + 0x12F0 + i * 8) = now;
                }
                off_trg[i] = 0;
            } else if (*(u8*)((s32)gp + 0x12C0 + i) != 0) {
                if (off_trg[i] == 0) {
                    off_time[i] = now;
                    off_trg[i] = 1;
                }
                if (off_trg[i] != 0 &&
                    (now - off_time[i]) / (*(u32*)0x800000F8 / 4000) > 100) {
                    off_trg[i] = 0;
                    *(u8*)((s32)gp + 0x12C0 + i) = 0;
                }
            }
        } else {
            if ((now - *(s64*)((s32)gp + 0x12F0 + i * 8)) /
                (*(u32*)0x800000F8 / 4000) > 30000) {
                *(u8*)((s32)gp + 0x12EC + i) = 0;
                *(u8*)((s32)gp + 0x12E8 + i) = 0;
                *(u8*)((s32)gp + 0x12C0 + i) = 0;
            }
            local_flags[i] = 1;
        }
        if (*(s32*)((s32)gp + 0x10) != 0) {
            local_flags[i] = 1;
        }
        if (*(s32*)((s32)gp + 0x18) != 0) {
            local_flags[i] = 1;
        }
        if (local_flags[i] == 0) {
            *(u8*)((s32)gp + 0x1310 + i) = 0;
        } else {
            *(u8*)((s32)gp + 0x1310 + i) = 1;
        }
    }
    return 0;
}

void L_gcResetCheck(void) {
    extern void VISetBlack(s32 black);
    extern void VIFlush(void);
    extern void VIWaitForRetrace(void);
    extern void psndExit(void);
    extern void DVDMgrDelete(void);
    extern void* OSGetCurrentThread(void);
    extern void OSCancelThread(void* thread);
    extern void OSResetSystem(s32 reset, u32 resetCode, s32 forceMenu);
    void* thread;
    u32 flags;
    u32 resetCode;

    if (*(s32*)((s32)gp + 0x1278) != 0 || R_bReset != 0) {
        R_bReset = 1;
        if (cardIsExec() == 0) {
            VISetBlack(1);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            VIWaitForRetrace();
            psndExit();
            DVDMgrDelete();
            thread = OSGetCurrentThread();
            if (thread != DvdCheckThread && DvdCheckTreadOn != 0) {
                OSCancelThread(DvdCheckThread);
                DvdCheckTreadOn = 0;
            }
            while (cardIsExec() != 0) {
                cardMain();
                VIWaitForRetrace();
            }
            if (*(s32*)((s32)gp + 0x1278) != 2) {
                flags = *(u32*)gp;
                if ((flags & 4) == 0) {
                    resetCode = 2;
                } else if ((flags & 8) == 0) {
                    resetCode = 0;
                } else {
                    resetCode = 1;
                }
                resetCode |= *(u32*)((s32)gp + 0x16C) << 2;
                OSResetSystem(0, resetCode, 0);
            } else {
                OSResetSystem(1, 0, 1);
            }
            for (;;) {
            }
        }
    }
}

const u32 dat_80420340 = 0xFFFFFFFF;
const f32 float_25_80420344 = 25.0f;
const f32 float_1p2667_80420348 = 1.26666665f;
const f32 float_1_8042034c = 1.0f;
const f32 float_10000_80420350 = 10000.0f;
const f32 float_0p5_80420354 = 0.5f;
const f32 float_60_80420358 = 60.0f;
const f32 float_0_8042035c = 0.0f;
const u8 unk_80429568[4] = { 0, 0, 0, 0 };
