#include "sequence/seq_load.h"
#include "countdown.h"
#include "driver/camdrv.h"
#include "driver/fadedrv.h"
#include "driver/swdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/filemgr.h"
#include "mario/mario.h"
#include "nameent.h"
#include "sound.h"

u8 loadMain(void* param_1);
u8 continueGame(void);
u8 loadDraw(void);

u8 work[76];
s32* wp = (s32*)work;
extern void* gp;
extern void* evtEntryType(void*, s32, s32, s32);
extern void seqSetSeq(s32, void*, void*);
extern s32 cardGetCode(void);
extern char* msgSearch(char*);


s32 unk_800f72e4(void* param_1) {
    fadeReset(4);
    wp[0x12] = *(s32*)((s32)param_1 + 0x178);
    return 2;
}


u8 seq_loadInit(void) {
    memset(wp, 0, 0x4C);
}

u8 seq_loadExit(void) {
    void* file = (void*)wp[1];

    if (file != NULL) {
        fileFree(file);
    }
    wp[1] = 0;
}

u8 seq_loadMain(void* param_1) {
    extern char* dat_ptrarr_802edb88[];
    extern char str_PCTs_w_PCTs_name_tpl_802ee004[];
    extern u32 dat_80422010;
    extern s32 win_dt[];
    extern void* evt_memcard_check;
    extern s32 getMarioStDvdRoot(void);
    extern s32 OSGetSoundMode(void);
    char* names[6];
    s32 state = *(s32*)((s32)param_1 + 4);

    switch (state) {
        case 0:
            names[0] = dat_ptrarr_802edb88[0];
            names[1] = dat_ptrarr_802edb88[1];
            names[2] = dat_ptrarr_802edb88[2];
            names[3] = dat_ptrarr_802edb88[3];
            names[4] = dat_ptrarr_802edb88[4];
            names[5] = dat_ptrarr_802edb88[5];
            if (fileAsyncf(4, 0, str_PCTs_w_PCTs_name_tpl_802ee004,
                           getMarioStDvdRoot(), names[*(s32*)((s32)gp + 0x16C)]) != 0) {
                wp[1] = (s32)fileAllocf(4, str_PCTs_w_PCTs_name_tpl_802ee004,
                                        getMarioStDvdRoot(), names[*(s32*)((s32)gp + 0x16C)]);
                *(s32*)((s32)param_1 + 4) = state + 1;
            }
            break;
        case 1:
            wp[0xF] = (s32)evtEntryType(&evt_memcard_check, 0, 0, 0);
            if ((*(u16*)wp & 0x80) != 0) {
                *(u16*)wp &= ~0x80;
                *(s32*)(wp[0xF] + 0xC4) = 1;
                *(s32*)(wp[0xF] + 0x178) = wp[0x12];
            }
            *(s32*)((s32)param_1 + 4) = state + 1;
            break;
        case 2:
            if (!evtCheckID(*(s32*)(wp[0xF] + 0x15C))) {
                if (evtGetValue(NULL, -90000000) == 1) {
                    wp[0xF] = 0;
                    *(s32*)((s32)param_1 + 4) = state + 1;
                    wp[6] = 0;
                    wp[7] = cardGetCode();
                    *(u16*)wp &= ~0x40;
                    if (OSGetSoundMode() == 0) {
                        SoundSetOutputMode(0);
                        *(s32*)((s32)gp + 0x1274) = 0;
                        *(s32*)((s32)wp + 0x24) = 0;
                    } else {
                        SoundSetOutputMode(1);
                        if (*(s32*)((s32)gp + 0x1274) == 0) {
                            *(s32*)((s32)wp + 0x24) = 1;
                        } else {
                            *(s32*)((s32)wp + 0x24) = 2;
                        }
                    }
                    *(s32*)((s32)wp + 0x28) = (*(s32*)((s32)gp + 0x1270) == 0);
                    *(s32*)((s32)wp + 0x2C) = *(s32*)((s32)wp + 0x24);
                    *(s32*)((s32)wp + 0x30) = *(s32*)((s32)wp + 0x28);
                    *(s32*)((s32)wp + 0x34) = *(s32*)((s32)gp + 0x11B8);
                    *(f32*)((s32)wp + 8) = *(f32*)((s32)win_dt + *(s32*)((s32)wp + 0x34) * 0x1C);
                    *(f32*)((s32)wp + 0xC) = *(f32*)((s32)win_dt + *(s32*)((s32)wp + 0x34) * 0x1C + 4);
                } else {
                    seqSetSeq(1, NULL, NULL);
                }
            }
            break;
        case 3:
        {
            u32 color = dat_80422010;
            fadeEntry(0x1C, 0, &color);
            *(s32*)((s32)param_1 + 4) = state + 1;
            break;
        }
        case 4:
            loadMain(param_1);
            break;
    }
}


u8 loadMain(void* param_1) {
    typedef struct LoadEntry {
        f32 x;
        f32 y;
        f32 cursorX;
        f32 cursorY;
        u32 texture0;
        u32 texture1;
        u8 up;
        u8 down;
        u8 left;
        u8 right;
    } LoadEntry;
    extern s32* wp;
    extern LoadEntry win_dt[];
    extern LoadEntry win_dt2[];
    extern LoadEntry win_dt3[];
    extern u16 keyGetButtonTrg(s32);
    extern u16 keyGetButtonRep(s32);
    extern u32 keyGetDirRep(s32);
    extern s32 cardGetCode(void);
    extern s32 cardIsExec(void);
    extern s32 fadeIsFinish(void);
    extern void* cardGetFilePtr(void);
    extern char* msgSearch(char*);
    extern void psndSFXOn(char*);
    extern void psndBGMOff(s32);
    extern void psndBGMOn(s32, char*);
    extern void psndENVOff(s32);
    extern void psndENVOn(s32, char*);
    extern void psndBGMOff_f_d(s32, s32, s32);
    extern void fadeEntry(s32, s32, void*);
    extern void seqSetSeq(s32, void*, void*);
    extern void* evtEntryType(void*, s32, s32, s32);
    extern s32 evtCheckID(s32);
    extern s32 evtGetValue(void*, s32);
    extern void dispEntry(s32, s32, void*, s32, f32);
    extern f64 sin(f64);
    extern f64 __fabs(f64);
    extern s32 OSGetSoundMode(void);
    extern void OSSetSoundMode(s32);
    extern void* gp;
    extern void* evt_memcard_start;
    extern void* evt_memcard_copy;
    extern void* evt_continue;
    extern u32 dat_8042202c;
    extern u32 dat_80422030;
    extern u32 dat_80422034;
    extern u32 dat_80422038;
    extern u32 dat_8042203c;
    extern f32 float_30_80422114;
    extern f32 float_50_80422154;
    extern f32 float_neg74_8042215c;
    extern f32 float_4_80422160;
    extern s32 nameEntPrepare(void);
    extern void nameEntOn(s32);
    extern s32 nameEntWait(void);
    extern s32 nameEntIsCancel(void);
    extern void nameEntOff(void);
    extern char* nameEntGetName(void);
    extern char* strcpy(char*, const char*);
    extern void stg0_00_init(void);
    extern void marioPartyKill(void);
    extern void* marioGetPtr(void);
    extern void swInit(void);
    extern void swByteSet(s32, s32);
    extern void* evt_memcard_delete;
    extern void* unk_evt_803bac3c;
    extern u32 dat_80422040;
    extern char str_aaa_00_802ede78[];
    extern char str_prologue_802ede80[];
    extern char str_msg_savefile_select_802edfa0[];
    extern char str_msg_savefile_delete__802edfb4[];
    extern char str_msg_savefile_copy_se_802edfd0[];
    extern char str_msg_savefile_copy_po_802edfec[];
    u16 buttons;
    u32 direction;
    s32 cardCode;
    s32 i;
    s32 fileBase;
    u16 fileFlags;
    u32 oldSelection;
    u32 selection;
    void* player;

    buttons = keyGetButtonTrg(0);
    keyGetButtonRep(0);
    direction = keyGetDirRep(0);
    cardCode = cardGetCode();
    if (cardIsExec() != 0 || fadeIsFinish() == 0) {
        buttons = 0;
        direction = 0;
    }
    if (wp[7] != cardCode) {
        buttons = 0;
        direction = 0;
    }
    wp[7] = cardCode;

    i = 0;
    fileBase = 0;
    do {
        fileFlags = *(u16*)((u8*)cardGetFilePtr() + fileBase + 0x2000);
        if ((fileFlags & 1) == 0 || (fileFlags & 2) != 0) {
            break;
        }
        i++;
        fileBase += 0x4000;
    } while (i < 4);
    if (i < 4) {
        *(u16*)wp &= ~0x10;
    } else {
        *(u16*)wp |= 0x10;
    }

    i = 0;
    fileBase = 0;
    *(u16*)wp &= ~0x20;
    do {
        fileFlags = *(u16*)((u8*)cardGetFilePtr() + fileBase + 0x2000);
        if ((fileFlags & 1) == 0 && (fileFlags & 2) == 0) {
            break;
        }
        i++;
        fileBase += 0x4000;
    } while (i < 4);
    if (i > 3) {
        *(u16*)wp |= 0x20;
    }

    if (wp[6] == 0) {
        wp[6] = 1;
    }
    if (wp[6] == 1) {
        *(char**)&wp[14] = msgSearch(str_msg_savefile_select_802edfa0);
        oldSelection = wp[8];
        if ((direction & 0x1000) != 0) {
            wp[8] = win_dt[oldSelection].up;
        } else if ((direction & 0x2000) != 0) {
            wp[8] = win_dt[oldSelection].down;
        } else if ((direction & 0x4000) != 0) {
            wp[8] = win_dt[oldSelection].left;
        } else if ((direction & 0x8000) != 0) {
            wp[8] = win_dt[oldSelection].right;
        } else if ((buttons & 0x100) != 0) {
            selection = wp[8];
            fileFlags = *(u16*)((u8*)cardGetFilePtr() +
                                selection * 0x4000 + 0x2000);
            if (selection < 4) {
                if ((fileFlags & 2) != 0) {
                    psndSFXOn((char*)0x13);
                } else if ((fileFlags & 1) == 0) {
                    wp[6] = 0x14;
                    psndSFXOn((char*)0x12);
                } else {
                    wp[6] = 0x1E;
                    psndSFXOn((char*)0x12);
                }
            } else if (selection == 4 && (*(u16*)wp & 0x10) == 0) {
                i = 0;
                fileBase = 0;
                do {
                    fileFlags = *(u16*)((u8*)cardGetFilePtr() + fileBase + 0x2000);
                    if ((fileFlags & 1) == 0 || (fileFlags & 2) != 0) {
                        break;
                    }
                    i++;
                    fileBase += 0x4000;
                } while (i < 4);
                if (i < 4) {
                    psndSFXOn((char*)0x12);
                    wp[8] = 0;
                    wp[6] = 0x28;
                } else {
                    psndSFXOn((char*)0x13);
                }
            } else if (selection == 5 && (*(u16*)wp & 0x20) == 0) {
                i = 0;
                fileBase = 0;
                do {
                    fileFlags = *(u16*)((u8*)cardGetFilePtr() + fileBase + 0x2000);
                    if ((fileFlags & 1) == 0 && (fileFlags & 2) == 0) {
                        break;
                    }
                    i++;
                    fileBase += 0x4000;
                } while (i < 4);
                if (i < 4) {
                    psndSFXOn((char*)0x12);
                    wp[8] = 0;
                    wp[6] = 0x32;
                } else {
                    psndSFXOn((char*)0x13);
                }
            } else if (selection == 6) {
                wp[6] = 10;
                psndSFXOn((char*)0x13);
            } else if (selection == 7) {
                psndSFXOn((char*)0x12);
                psndBGMOff(0x800);
                wp[6] = 100;
            } else if (selection == 8) {
                psndSFXOn((char*)0x12);
                wp[6] = 200;
            }
        } else if ((buttons & 0x200) != 0) {
            psndSFXOn((char*)0x13);
            wp[6] = 10;
        }

        if (oldSelection != (u32)wp[8]) {
            if (wp[8] < 4) {
                psndSFXOn((char*)9);
            } else {
                psndSFXOn((char*)5);
            }
        }
        *(f32*)&wp[4] = win_dt[wp[8]].x + win_dt[wp[8]].cursorX;
        *(f32*)&wp[5] = win_dt[wp[8]].y + win_dt[wp[8]].cursorY;
    } else if (wp[6] == 10) {
        u32 color = dat_8042202c;
        fadeEntry(0x1B, 0, &color);
        wp[6]++;
    } else if (wp[6] == 11) {
        if (fadeIsFinish() != 0) {
            seqSetSeq(1, NULL, NULL);
        }
    } else if (wp[6] == 0x14) {
        *(u16*)wp |= 0x40;
        wp[15] = (s32)evtEntryType(&evt_memcard_start, 0, 0, 0);
        *(s32*)(wp[15] + 0xC4) = wp[8];
        wp[6]++;
    } else if (wp[6] == 0x15) {
        if (evtCheckID(*(s32*)(wp[15] + 0x15C)) == 0) {
            *(u16*)wp &= ~0x40;
            if (evtGetValue(NULL, -50000000) == 1) {
                wp[6]++;
            } else {
                wp[6] = 1;
            }
        }
    } else if (wp[6] == 0x16) {
        u32 color = dat_80422030;
        fadeEntry(0x1B, 0, &color);
        psndBGMOff_f_d(0x200, 2000, 0);
        wp[6]++;
    } else if (wp[6] == 0x17) {
        if (fadeIsFinish() != 0) {
            *(u16*)wp |= 0x40;
            wp[15] = (s32)evtEntryType(&evt_continue, 0, 0, 0);
            wp[6]++;
        }
    } else if (wp[6] == 0x18) {
        if (evtCheckID(*(s32*)(wp[15] + 0x15C)) == 0) {
            *(u16*)wp &= ~0x40;
            continueGame();
        }
    } else if (wp[6] == 0x1E) {
        u32 color = dat_80422034;
        fadeEntry(0x1B, 0, &color);
        wp[6]++;
    } else if (wp[6] == 0x1F) {
        if (fadeIsFinish() != 0) {
            wp[6]++;
        }
    } else if (wp[6] == 0x20) {
        if (nameEntPrepare() != 0) {
            u32 color = dat_80422038;
            nameEntOn(0);
            fadeEntry(0x1D, 0, &color);
            wp[6]++;
        }
    } else if (wp[6] == 0x21) {
        if (fadeIsFinish() != 0 && nameEntWait() == 0) {
            u32 color = dat_8042203c;
            fadeEntry(0x1B, 0, &color);
            if (nameEntIsCancel() == 0) {
                psndBGMOff_f_d(0x200, 3000, 0);
            }
            wp[6]++;
        }
    } else if (wp[6] == 0x22) {
        if (fadeIsFinish() != 0) {
            nameEntOff();
            if (nameEntIsCancel() == 0) {
                strcpy((char*)gp + 0x11BC, nameEntGetName());
                *(s32*)((u8*)gp + 0x11B8) = wp[8];
                stg0_00_init();
                *(s32*)((u8*)gp + 0x11D0) = wp[8];
                marioPartyKill();
                player = marioGetPtr();
                *(u8*)((u8*)player + 0x247) = 0;
                *(u8*)((u8*)player + 0x248) = 0;
                *(s32*)((u8*)gp + 0x110) = 0;
                *(s32*)((u8*)gp + 0x108) = 0x19;
                swInit();
                swByteSet(0, 0);
                if (cardCode == 0) {
                    *(s32*)((u8*)gp + 0x11D0) = wp[8];
                    wp[15] = (s32)evtEntryType(&unk_evt_803bac3c, 0, 0, 0);
                    *(s32*)(wp[15] + 0x9C) = 0x17;
                    wp[6]++;
                } else {
                    seqSetSeq(3, str_aaa_00_802ede78, str_prologue_802ede80);
                }
            } else {
                u32 color = dat_80422040;
                fadeEntry(0x1C, 0, &color);
                wp[6] = 1;
            }
        }
    } else if (wp[6] == 0x23) {
        if (evtCheckID(*(s32*)(wp[15] + 0x15C)) == 0) {
            seqSetSeq(3, str_aaa_00_802ede78, str_prologue_802ede80);
        }
    } else if (wp[6] == 0x28) {
        *(char**)&wp[14] = msgSearch(str_msg_savefile_delete__802edfb4);
        oldSelection = wp[8];
        if ((direction & 0x1000) != 0) {
            wp[8] = win_dt2[oldSelection].up;
        } else if ((direction & 0x2000) != 0) {
            wp[8] = win_dt2[oldSelection].down;
        } else if ((direction & 0x4000) != 0) {
            wp[8] = win_dt2[oldSelection].left;
        } else if ((direction & 0x8000) != 0) {
            wp[8] = win_dt2[oldSelection].right;
        } else if ((buttons & 0x200) != 0) {
            wp[8] = 4;
            wp[6] = 1;
            psndSFXOn((char*)0x13);
        } else if ((buttons & 0x100) != 0) {
            if (oldSelection < 4) {
                fileFlags = *(u16*)((u8*)cardGetFilePtr() +
                                    wp[8] * 0x4000 + 0x2000);
                if ((fileFlags & 2) == 0 && (fileFlags & 1) != 0) {
                    psndSFXOn((char*)0x13);
                } else {
                    psndSFXOn((char*)0x12);
                    wp[6]++;
                }
            } else {
                wp[8] = 4;
                wp[6] = 1;
                psndSFXOn((char*)0x13);
            }
        }
        if (oldSelection != (u32)wp[8] && wp[8] < 4) {
            psndSFXOn((char*)9);
        }
        *(f32*)&wp[4] = win_dt2[wp[8]].x + win_dt2[wp[8]].cursorX;
        *(f32*)&wp[5] = win_dt2[wp[8]].y + win_dt2[wp[8]].cursorY;
    } else if (wp[6] == 0x29) {
        *(u16*)wp |= 0x40;
        wp[15] = (s32)evtEntryType(&evt_memcard_delete, 0, 0, 0);
        *(s32*)(wp[15] + 0x9C) = wp[8];
        wp[6]++;
    } else if (wp[6] == 0x2A) {
        if (evtCheckID(*(s32*)(wp[15] + 0x15C)) == 0) {
            if (evtGetValue(NULL, -90000000) != 0) {
                *(u16*)wp |= 0x80;
                *(s32*)((u8*)param_1 + 4) = 1;
                return 0;
            }
            *(u16*)wp &= ~0x40;
            wp[8] = 4;
            wp[6] = 1;
        }
    } else if (wp[6] == 0x32) {
        *(char**)&wp[14] = msgSearch(str_msg_savefile_copy_se_802edfd0);
        oldSelection = wp[8];
        if ((direction & 0x1000) != 0) {
            wp[8] = win_dt2[oldSelection].up;
        } else if ((direction & 0x2000) != 0) {
            wp[8] = win_dt2[oldSelection].down;
        } else if ((direction & 0x4000) != 0) {
            wp[8] = win_dt2[oldSelection].left;
        } else if ((direction & 0x8000) != 0) {
            wp[8] = win_dt2[oldSelection].right;
        } else if ((buttons & 0x200) != 0) {
            wp[8] = 5;
            wp[6] = 1;
            psndSFXOn((char*)0x13);
        } else if ((buttons & 0x100) != 0 && oldSelection < 4) {
            fileFlags = *(u16*)((u8*)cardGetFilePtr() + wp[8] * 0x4000 + 0x2000);
            if ((fileFlags & 3) == 0) {
                wp[6] = 0x33;
                wp[12] = wp[8];
                wp[13] = (wp[12] + 1) % 4;
                wp[8] = wp[13];
                psndSFXOn((char*)0x12);
            }
        }
        if (oldSelection != (u32)wp[8] && wp[8] < 4) {
            psndSFXOn((char*)9);
        }
        *(f32*)&wp[4] = win_dt2[wp[8]].x + win_dt2[wp[8]].cursorX;
        *(f32*)&wp[5] = win_dt2[wp[8]].y + win_dt2[wp[8]].cursorY;
    } else if (wp[6] == 0x33) {
        *(char**)&wp[14] = msgSearch(str_msg_savefile_copy_po_802edfec);
        oldSelection = wp[8];
        if ((direction & 0x1000) != 0) {
            do wp[8] = win_dt3[wp[8]].up; while (wp[8] == wp[12]);
        } else if ((direction & 0x2000) != 0) {
            do wp[8] = win_dt3[wp[8]].down; while (wp[8] == wp[12]);
        } else if ((direction & 0x4000) != 0) {
            do wp[8] = win_dt3[wp[8]].left; while (wp[8] == wp[12]);
        } else if ((direction & 0x8000) != 0) {
            do wp[8] = win_dt3[wp[8]].right; while (wp[8] == wp[12]);
        } else if ((buttons & 0x200) != 0) {
            wp[8] = wp[12];
            wp[6] = 0x32;
            psndSFXOn((char*)0x13);
        } else if ((buttons & 0x100) != 0) {
            wp[6] = 0x34;
            wp[13] = wp[8];
            psndSFXOn((char*)0x12);
        }
        if (oldSelection != (u32)wp[8] && wp[8] < 4) {
            psndSFXOn((char*)9);
        }
        wp[13] = wp[8];
        *(f32*)&wp[4] = win_dt2[wp[8]].x + win_dt2[wp[8]].cursorX;
        *(f32*)&wp[5] = win_dt2[wp[8]].y + win_dt2[wp[8]].cursorY;
    } else if (wp[6] == 0x34) {
        *(u16*)wp |= 0x40;
        wp[15] = (s32)evtEntryType(&evt_memcard_copy, 0, 0, 0);
        *(s32*)(wp[15] + 0x9C) = wp[12];
        *(s32*)(wp[15] + 0xA0) = wp[13];
        wp[6]++;
    } else if (wp[6] == 0x35) {
        if (evtCheckID(*(s32*)(wp[15] + 0x15C)) == 0) {
            if (evtGetValue(NULL, -90000000) != 0) {
                *(u16*)wp |= 0x80;
                *(s32*)((u8*)param_1 + 4) = 1;
                return 0;
            }
            *(u16*)wp &= ~0x40;
            wp[8] = 5;
            wp[6] = 1;
        }
    } else if (wp[6] == 100) {
        s32 oldMode = wp[11];
        *(u16*)wp |= 1;
        if ((direction & 0x1000) != 0) {
            wp[11]--;
            psndSFXOn((char*)5);
        } else if ((direction & 0x2000) != 0) {
            wp[11]++;
            psndSFXOn((char*)5);
        } else if ((buttons & 0x100) != 0) {
            if (wp[11] == 0) {
                OSSetSoundMode(0);
                *(s32*)((u8*)gp + 0x1274) = 0;
            } else if (wp[11] == 1) {
                OSSetSoundMode(1);
                *(s32*)((u8*)gp + 0x1274) = 0;
            } else {
                OSSetSoundMode(1);
                *(s32*)((u8*)gp + 0x1274) = 1;
            }
            SoundSetOutputMode(OSGetSoundMode() == 0 ? 0 : 1);
            psndSFXOn((char*)0x12);
            psndBGMOn(0xA0, NULL);
            psndENVOff(0x200);
            wp[9] = wp[11];
            *(u16*)wp &= ~1;
            wp[6] = 1;
        } else if ((buttons & 0x200) != 0) {
            psndSFXOn((char*)0x13);
            psndBGMOn(0xA0, NULL);
            psndENVOff(0x200);
            wp[11] = wp[9];
            SoundSetOutputMode(OSGetSoundMode() == 0 ? 0 : 1);
            *(u16*)wp &= ~1;
            wp[6] = 1;
        }
        if (wp[11] > 2) wp[11] = 0;
        if (wp[11] < 0) wp[11] = 2;
        if (wp[11] != oldMode) {
            SoundSetOutputMode(wp[11]);
            psndENVOn(0x110, (char*)0x75);
        }
        *(f32*)&wp[4] = float_neg74_8042215c;
        *(f32*)&wp[5] = float_4_80422160 -
                         (float_50_80422154 * (f32)wp[11] - float_50_80422154);
    } else if (wp[6] == 200) {
        *(u16*)wp |= 1;
        if ((direction & 0x1000) != 0) {
            wp[12]--;
            psndSFXOn((char*)5);
        } else if ((direction & 0x2000) != 0) {
            wp[12]++;
            psndSFXOn((char*)5);
        } else if ((buttons & 0x100) != 0) {
            *(s32*)((u8*)gp + 0x1270) = (wp[12] == 0);
            psndSFXOn((char*)0x12);
            wp[10] = wp[12];
            *(u16*)wp &= ~1;
            wp[6] = 1;
        } else if ((buttons & 0x200) != 0) {
            psndSFXOn((char*)0x13);
            wp[12] = wp[10];
            *(u16*)wp &= ~1;
            wp[6] = 1;
        }
        if (wp[12] > 1) wp[12] = 0;
        if (wp[12] < 0) wp[12] = 1;
        *(f32*)&wp[4] = float_neg74_8042215c;
        *(f32*)&wp[5] = float_4_80422160 -
                         (float_50_80422154 * (f32)wp[12] - float_30_80422114);
    }

    *(f32*)&wp[2] += (*(f32*)&wp[4] - *(f32*)&wp[2]) * 0.25f;
    *(f32*)&wp[3] += (*(f32*)&wp[5] - *(f32*)&wp[3]) * 0.25f;
    *(f32*)&wp[2] += (f32)__fabs(2.0f * (f32)sin((f64)(*(s32*)((u8*)gp + 0x1C) * 0.125f)));
    dispEntry(4, 0, loadDraw, 0, 1000.0f);
    return 0;
}

u8 loadDraw(void) {
    typedef struct CameraCopy {
        u32 words[0x98];
    } CameraCopy;

    typedef struct LoadDrawVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LoadDrawVec3;

    typedef struct LoadDrawEntry {
        f32 x;
        f32 y;
        f32 cursorX;
        f32 cursorY;
        u32 textureIx0;
        u32 textureIx1;
        u8 up;
        u8 down;
        u8 left;
        u8 right;
    } LoadDrawEntry;

    typedef struct LoadDrawAngleBlock {
        u32 words[16];
    } LoadDrawAngleBlock;

    typedef union LoadDrawDoubleBits {
        f64 value;
        struct {
            u32 hi;
            u32 lo;
        } words;
    } LoadDrawDoubleBits;

    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, u8, s32, s32, u8);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXGetViewportv(f32*);
    extern void GXGetProjectionv(f32*);
    extern void GXSetProjection(void*, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void GXSetProjectionv(f32*);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void* camGetPtr(s32);
    extern void* camGetCurPtr(void);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern u32 GXGetTexObjHeight(void*);
    extern u32 GXGetTexObjWidth(void*);
    extern void PSMTXScale(f32 (*)[4], f32, f32, f32);
    extern void GXLoadTexMtxImm(f32 (*)[4], s32, s32);
    extern void GXBegin(s32, s32, s32);
    extern f64 __fabs(f64);
    extern u8 lbl_802EDB88[];

    extern void winTexInit(void*);
    extern void winTexInit_x2(void*);
    extern void winTexSet(s32, void*, void*, void*);
    extern void winTexSet_x2(s32, s32, void*, void*, void*);
    extern LoadDrawEntry win_dt[];
    extern u32 dat_80422044;
    extern u32 dat_80422048;
    extern u32 dat_8042204c;
    extern u32 dat_8042206c;
    extern u32 dat_80422070;
    extern u32 dat_80422074;
    extern u32 dat_80422078;
    extern u32 dat_8042207c;
    extern u32 dat_80422080;
    extern u32 dat_80422084;
    extern u32 dat_80422088;
    extern u32 dat_8042208c;
    extern u32 dat_80422090;
    extern u32 dat_80422098;
    extern u32 dat_80422054;
    extern u32 dat_80422058;
    extern u32 dat_8042205c;
    extern u32 dat_80422060;
    extern u32 dat_80422064;
    extern const f32 float_60_804220c4[1];
    extern const f32 float_neg250_804220c8[1];
    extern const f32 float_200_804220cc[1];
    extern const f32 float_500_804220d0[1];
    extern const f32 float_40_804220d4[1];
    extern const f32 float_20_804220d8[1];
    extern const f32 float_0p6_804220dc[1];
    extern const f32 float_100_804220e8[1];
    extern const f32 float_84_804220ec[1];
    extern const f32 float_0p5_804220f0[1];
    extern const f32 float_28_804220f4[1];
    extern const f32 float_70_804220f8[1];
    extern const f32 float_12_804220fc[1];
    extern f32 float_0p05_80422140;
    extern f32 float_30_80422114;
    extern f32 float_50_80422154;
    extern f32 float_4_80422160;
    extern f32 float_2_80422158;
    extern void nameMaskGX(f32);

    extern void FontDrawStart(void);
    extern u32 FontGetMessageWidth(char*);
    extern void FontDrawMessage(u32, u32, char*);
    extern void FontDrawString(f64, f64, char*);
    extern void FontDrawColor(u32*);
    extern void FontDrawScale(f32);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern s32 sprintf(char*, char*, ...);
    extern u32 strlen(char*);
    extern void* cardGetFilePtr(void);
    extern char* winZenkakuStr(s32);
    extern void windowDispGX_Waku_col(f64, f64, f64, f64, f64, u16, u32*);
    extern char* strcat(char*, const char*);
    extern char* unk_JP_US_EU_40_801703e8(s32, s32);
    extern void FontDrawScaleVec(void*);
    extern void PSMTXTrans(f32 (*)[4], f64, f64, f64);
    extern void PSMTXConcat(f32 (*)[4], f32 (*)[4], f32 (*)[4]);
    extern void iconDispGxCol(void*, u16, u16, void*);
    extern f64 fmod(f64, f64);
    extern void winTexSetRot(f64, s32, void*, void*, void*);

    extern u32 dat_80422068;
    extern u32 dat_80422094;
    extern u32 stone_id[7];

    extern f32 float_1600_80422128;
    extern f32 float_8_8042212c;
    extern f32 float_22_80422130;
    extern f32 float_1p5_80422134;
    extern f32 float_106_80422138;
    extern f32 float_52_8042213c;
    extern f32 float_120_80422144;
    extern f32 float_140_80422148;
    extern f32 float_neg150_8042214c;
    extern f32 float_300_80422150;
    extern char str_PCTs_80422118[];
    extern char str_PCTs__8042211c[];
    extern char str__80422124[];
    extern char str_88_80422100[];

    extern const f32 float_0_804220a4[1];
    extern const f32 float_1_804220a8[1];
    extern const f32 float_neg304_804220ac[1];
    extern const f32 float_240_804220b0[1];
    extern const f32 float_neg240_804220b4[1];
    extern const f32 float_neg272_804220b8[1];
    extern const f32 float_272_804220bc[1];
    extern const f32 float_304_804220c0[1];
    extern u32 dat_80422050;
    extern const char str_PCTsPCTs_804220e0[];
    extern char str_PCTs_PCTs_8042210c[];

    char number[32];
    u8 texObj0[32];
    u8 texObj1[32];
    u8 texObj2[32];
    f32 texMtx0[3][4];
    f32 texMtx1[3][4];
    f32 texMtx2[3][4];
    f32 viewport[6];
    f32 projection[7];
    CameraCopy cameraBackup;
    CameraCopy* camera;
    CameraCopy* currentCamera;
    char* message;
    f32 texScaleX;
    f32 texScaleY;
    u32 fog;
    u16 texWidth;
    u16 texHeight;
    s32 state;
    s32 entryIx;
    LoadDrawEntry* entry;
    u32 normalColor;
    u32 hoverColor;
    volatile u32 disabledColor;
    const u8* roBase;
    f64 texAbsH;
    f64 texAbsW;
    LoadDrawDoubleBits heightConv;
    LoadDrawDoubleBits widthConv;

    roBase = lbl_802EDB88;
    camera = (CameraCopy*)camGetPtr(8);
    hoverColor = dat_80422044;
    normalColor = dat_80422048;
    disabledColor = dat_8042204c;
    state = wp[6];

    if (state < 0x20 || state >= 0x28) {
        GXGetViewportv(viewport);
        GXGetProjectionv(projection);
        GXSetProjection((u8*)camera + 0x15C, *(s32*)((u8*)camera + 0x19C));

        currentCamera = (CameraCopy*)camGetCurPtr();
        cameraBackup = *currentCamera;
        currentCamera = (CameraCopy*)camGetCurPtr();
        *currentCamera = *camera;

        GXSetBlendMode(0, 1, 0, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(0, 7, 0);

        fog = dat_80422050;
        GXSetFog(0, float_0_804220a4[0], float_0_804220a4[0],
                 float_0_804220a4[0], float_0_804220a4[0], &fog);
        GXSetNumChans(0);
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevOp(0, 3);
        GXSetCullMode(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(13, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 13, 1, 4, 0);
        GXLoadPosMtxImm((u8*)camera + 0x11C, 0);
        GXSetCurrentMtx(0);

        TEXGetGXTexObjFromPalette(**(void***)((s32)wp[1] + 0xA0), texObj0, 0x35);
        GXInitTexObjLOD(texObj0, 1, 1, float_0_804220a4[0], float_0_804220a4[0],
                        float_0_804220a4[0], 0, 0, 0);
        GXLoadTexObj(texObj0, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

        texAbsH = __fabs(*(const f64*)(roBase + 0x308));
        texAbsW = __fabs(*(const f64*)(roBase + 0x310));

        texHeight = (u16)GXGetTexObjHeight(texObj0);
        heightConv.words.hi = 0x43300000;
        heightConv.words.lo = (u32)texHeight;
        texScaleY =
            (f32)texAbsH /
            (f32)(heightConv.value - *(const f64*)(roBase + 0x408));

        texWidth = (u16)GXGetTexObjWidth(texObj0);
        widthConv.words.hi = 0x43300000;
        widthConv.words.lo = (u32)texWidth;
        texScaleX =
            (f32)texAbsW /
            (f32)(widthConv.value - *(const f64*)(roBase + 0x408));

        PSMTXScale(texMtx0, texScaleX, texScaleY, float_1_804220a8[0]);
        GXLoadTexMtxImm(texMtx0, 0x1E, 1);

        GXBegin(0x80, 0, 4);
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg304_804220ac)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_240_804220b0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg304_804220ac)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg240_804220b4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg272_804220b8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg240_804220b4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg272_804220b8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_240_804220b0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];

        TEXGetGXTexObjFromPalette(**(void***)((s32)wp[1] + 0xA0), texObj1, 0x34);
        GXInitTexObjLOD(texObj1, 1, 1, float_0_804220a4[0], float_0_804220a4[0],
                        float_0_804220a4[0], 0, 0, 0);
        GXLoadTexObj(texObj1, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

        texAbsH = __fabs(*(const f64*)(roBase + 0x308));
        texAbsW = __fabs(*(const f64*)(roBase + 0x318));

        texHeight = (u16)GXGetTexObjHeight(texObj1);
        heightConv.words.hi = 0x43300000;
        heightConv.words.lo = (u32)texHeight;
        texScaleY =
            (f32)texAbsH /
            (f32)(heightConv.value - *(const f64*)(roBase + 0x408));

        texWidth = (u16)GXGetTexObjWidth(texObj1);
        widthConv.words.hi = 0x43300000;
        widthConv.words.lo = (u32)texWidth;
        texScaleX =
            (f32)texAbsW /
            (f32)(widthConv.value - *(const f64*)(roBase + 0x408));

        PSMTXScale(texMtx1, texScaleX, texScaleY, float_1_804220a8[0]);
        GXLoadTexMtxImm(texMtx1, 0x1E, 1);

        GXBegin(0x80, 0, 4);
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg272_804220b8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_240_804220b0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg272_804220b8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg240_804220b4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_272_804220bc)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg240_804220b4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_272_804220bc)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_240_804220b0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];

        TEXGetGXTexObjFromPalette(**(void***)((s32)wp[1] + 0xA0), texObj2, 0x36);
        GXInitTexObjLOD(texObj2, 1, 1, float_0_804220a4[0], float_0_804220a4[0],
                        float_0_804220a4[0], 0, 0, 0);
        GXLoadTexObj(texObj2, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

        texAbsH = __fabs(*(const f64*)(roBase + 0x308));
        texAbsW = __fabs(*(const f64*)(roBase + 0x310));

        texHeight = (u16)GXGetTexObjHeight(texObj2);
        heightConv.words.hi = 0x43300000;
        heightConv.words.lo = (u32)texHeight;
        texScaleY =
            (f32)texAbsH /
            (f32)(heightConv.value - *(const f64*)(roBase + 0x408));

        texWidth = (u16)GXGetTexObjWidth(texObj2);
        widthConv.words.hi = 0x43300000;
        widthConv.words.lo = (u32)texWidth;
        texScaleX =
            (f32)texAbsW /
            (f32)(widthConv.value - *(const f64*)(roBase + 0x408));

        PSMTXScale(texMtx2, texScaleX, texScaleY, float_1_804220a8[0]);
        GXLoadTexMtxImm(texMtx2, 0x1E, 1);

        GXBegin(0x80, 0, 4);
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_272_804220bc)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_240_804220b0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_272_804220bc)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg240_804220b4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_304_804220c0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_neg240_804220b4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];

        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_304_804220c0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_240_804220b0)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_1_804220a8)[0];
        *(volatile f32*)0xCC008000 = ((volatile const f32*)float_0_804220a4)[0];

        /* Nine-entry save-menu texture pass. */
        entryIx = 0;
        entry = win_dt;
        do {
            volatile u32 drawColor = normalColor;

            if (entryIx == wp[8]) {
                drawColor = hoverColor;
            }

            if (wp[6] == 0x33 &&
                (entryIx == wp[12] || entryIx == wp[13])) {
                drawColor = hoverColor;
            }

            if (entryIx == 4 && ((*(u16*)wp & 0x10) != 0)) {
                drawColor = disabledColor;
            }

            if (entryIx == 5 && ((*(u16*)wp & 0x20) != 0)) {
                drawColor = disabledColor;
            }

            if (entry->textureIx0 >= 10 && entry->textureIx0 < 14) {
                volatile LoadDrawVec3 cardStage;
                LoadDrawVec3 cardPos;
                LoadDrawVec3 cardScale;
                volatile LoadDrawVec3 iconStage;
                LoadDrawVec3 iconPos;
                LoadDrawVec3 iconScale;
                u32 cardColor;
                u32 iconColor;

                *((volatile u8*)&drawColor + 3) = 0xFE;

                winTexInit(**(void***)((s32)wp[1] + 0xA0));

                cardStage = *(const LoadDrawVec3*)(roBase + 0x18);
                cardStage.x = entry->x;
                cardStage.y = entry->y;
                cardPos.x = cardStage.x;
                cardPos.y = cardStage.y;
                cardPos.z = cardStage.z;
                cardScale = *(const LoadDrawVec3*)(roBase + 0x24);
                cardColor = drawColor;
                winTexSet(0xE, &cardPos, &cardScale, &cardColor);

                winTexInit(**(void***)((s32)wp[1] + 0xA0));

                iconStage = *(const LoadDrawVec3*)(roBase + 0x30);
                iconStage.x = entry->x;
                iconStage.y = entry->y + float_60_804220c4[0];
                iconPos.x = iconStage.x;
                iconPos.y = iconStage.y;
                iconPos.z = iconStage.z;
                iconScale = *(const LoadDrawVec3*)(roBase + 0x3C);
                iconColor = drawColor;
                winTexSet(entry->textureIx0, &iconPos, &iconScale, &iconColor);
            } else {
                volatile LoadDrawVec3 posStage;
                LoadDrawVec3 pos;
                LoadDrawVec3 scale;
                u32 normalColorArg;

                winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));

                posStage = *(const LoadDrawVec3*)(roBase + 0x48);
                posStage.x = entry->x;
                posStage.y = entry->y;
                pos.x = posStage.x;
                pos.y = posStage.y;
                pos.z = posStage.z;
                scale = *(const LoadDrawVec3*)(roBase + 0x54);
                normalColorArg = drawColor;
                winTexSet_x2(entry->textureIx0, entry->textureIx1,
                             &pos, &scale, &normalColorArg);
            }

            entryIx++;
            entry++;
        } while (entryIx < 9);

        /* Save-file panel and four-slot information renderer. */
        {
            u32 panelColor = dat_80422054;
            s32 slot = 0;
            s32 fileOffset = 0;
            LoadDrawEntry* slotEntry = win_dt;

            windowDispGX_Waku_col(
                float_neg250_804220c8[0],
                float_200_804220cc[0],
                float_500_804220d0[0],
                float_40_804220d4[0],
                float_20_804220d8[0],
                0,
                &panelColor);

            FontDrawStart();
            message = (char*)wp[14];
            {
                u32 titleWidth = FontGetMessageWidth(message);
                FontDrawMessage(
                    (u32)(-(s32)((titleWidth & 0xFFFFU) / 2U)),
                    0xC0,
                    message);
            }

            do {
                u8* file = (u8*)cardGetFilePtr() + fileOffset + 0x2000;
                u16 fileFlags = *(u16*)file;
                char* saveName = (char*)(file + 0x11C4);
                s16 level = *(s16*)(file + 0x146A);
                u32 playHi = *(u32*)(file + 0x28);
                u32 playLo = *(u32*)(file + 0x2C);
                u16 stoneFlags = *(u16*)(file + 0x146C);
                s32 progress = *(s32*)(file + 0x17C);
                u32 labelColor = dat_80422058;
                u32 textColor = dat_80422060;

                FontDrawStart();
                FontDrawColor(&labelColor);
                if (slot == wp[8]) {
                    u32 selectedLabelColor = dat_8042205c;
                    FontDrawColor(&selectedLabelColor);
                }
                FontDrawScale(float_0p6_804220dc[0]);
                FontDrawEdge();

                if (*(volatile u32*)((u8*)gp + 0x16C) == 0) {
                    message = (char*)(roBase + 0x320);
                } else {
                    message = msgSearch((char*)(roBase + 0x328));
                }
                sprintf(number, (char*)str_PCTsPCTs_804220e0,
                        message, winZenkakuStr(slot + 1));
                FontDrawString(
                    (f64)(slotEntry->x - float_100_804220e8[0]),
                    (f64)(slotEntry->y + float_84_804220ec[0]),
                    number);

                FontDrawEdgeOff();
                FontDrawScale(float_1_804220a8[0]);
                FontDrawColor(&textColor);
                if (slot == wp[8]) {
                    u32 selectedTextColor = dat_80422064;
                    FontDrawColor(&selectedTextColor);
                }

                if ((fileFlags & 2) != 0) {
                    if (*(volatile u32*)((u8*)gp + 0x16C) == 0) {
                        message = (char*)(roBase + 0x338);
                    } else {
                        message = msgSearch((char*)(roBase + 0x348));
                    }

                    {
                        u32 msgWidth = FontGetMessageWidth(message);
                        f32 msgWidthF;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo = msgWidth & 0xFFFFU;
                        msgWidthF = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x408));

                        FontDrawStart();
                        FontDrawMessage(
                            (u32)(s32)(slotEntry->x -
                                msgWidthF * float_0p5_804220f0[0]),
                            (u32)(s32)(slotEntry->y + float_28_804220f4[0]),
                            message);
                    }

                    message = msgSearch((char*)(roBase + 0x360));
                    {
                        u32 msgWidth = FontGetMessageWidth(message);
                        f32 halfWidth;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo =
                            ((msgWidth >> 1) & 0x7FFFU) ^ 0x80000000U;
                        halfWidth = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x410));

                        FontDrawString(
                            (f64)(slotEntry->x - halfWidth),
                            (f64)(slotEntry->y + float_70_804220f8[0]),
                            message);
                    }
                } else if ((fileFlags & 1) == 0) {
                    if (*(volatile u32*)((u8*)gp + 0x16C) == 0) {
                        message = (char*)(roBase + 0x378);
                    } else {
                        message = msgSearch((char*)(roBase + 0x388));
                    }

                    {
                        u32 msgWidth = FontGetMessageWidth(message);
                        f32 msgWidthF;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo = msgWidth & 0xFFFFU;
                        msgWidthF = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x408));

                        FontDrawString(
                            (f64)(slotEntry->x -
                                msgWidthF * float_0p5_804220f0[0]),
                            (f64)(slotEntry->y + float_12_804220fc[0]),
                            message);
                    }
                } else if (progress == 0) {
                    if (*(volatile u32*)((u8*)gp + 0x16C) == 0) {
                        message = (char*)(roBase + 0x39C);
                    } else {
                        message = msgSearch((char*)(roBase + 0x3A8));
                    }

                    {
                        u32 msgWidth = FontGetMessageWidth(message);
                        f32 msgWidthF;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo = msgWidth & 0xFFFFU;
                        msgWidthF = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x408));

                        FontDrawString(
                            (f64)(slotEntry->x -
                                msgWidthF * float_0p5_804220f0[0]),
                            (f64)(slotEntry->y + float_12_804220fc[0]),
                            message);
                    }

                    if (strlen(saveName) != 0) {
                        u32 nameWidth = FontGetMessageWidth(saveName);
                        f32 halfNameWidth;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo =
                            ((nameWidth >> 1) & 0x7FFFU) ^ 0x80000000U;
                        halfNameWidth = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x410));

                        FontDrawString(
                            (f64)(slotEntry->x - halfNameWidth),
                            (f64)(slotEntry->y + float_70_804220f8[0]),
                            saveName);
                    }
                } else {
                    char levelText[252];
                    u32 hours;
                    u32 minutes;
                    char playText[252];
                    char prefixText[252];
                    char* playFmt;
                    u32 fullWidth;
                    u32 prefixWidth;
                    f32 drawnWidth;
                    f32 iconOffset;
                    LoadDrawVec3 textScale;
                    f32 iconMtx[3][4];
                    f32 iconScaleMtx[3][4];
                    u32 iconColor;
                    s32 stoneIx;
                    s32 stoneOffset;
                    u32* stonePtr;
                    unsigned long long rawTicks;
                    signed long long seconds;

                    /*
                     * Language 0 uses a distinct full-width "88" small-data
                     * string in the target; this partial currently uses the
                     * ASCII str_88 identity for the width probe.
                     */
                    FontGetMessageWidth(str_88_80422100);

                    rawTicks =
                        ((unsigned long long)playHi << 32) | playLo;
                    seconds =
                        (signed long long)rawTicks /
                        (signed long long)(*(volatile u32*)0x800000F8 >> 2);

                    if ((unsigned long long)seconds <= 359999ULL) {
                        hours = (u32)((unsigned long long)seconds / 3600ULL);
                        minutes =
                            (u32)(((unsigned long long)seconds % 3600ULL) / 60ULL);
                    } else {
                        hours = 99;
                        minutes = 59;
                    }

                    if (strlen(saveName) != 0) {
                        u32 nameWidth = FontGetMessageWidth(saveName);
                        f32 halfNameWidth;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo =
                            ((nameWidth >> 1) & 0x7FFFU) ^ 0x80000000U;
                        halfNameWidth = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x410));

                        FontDrawString(
                            (f64)(slotEntry->x - halfNameWidth),
                            (f64)(slotEntry->y + float_70_804220f8[0]),
                            saveName);
                    }

                    if (*(volatile u32*)((u8*)gp + 0x16C) == 0) {
                        sprintf(
                            levelText,
                            (char*)(roBase + 0x3C0),
                            msgSearch((char*)(roBase + 0x3C8)),
                            winZenkakuStr((s16)level));
                    } else {
                        sprintf(
                            levelText,
                            str_PCTs_PCTs_8042210c,
                            msgSearch((char*)(roBase + 0x3C8)),
                            winZenkakuStr((s16)level));
                    }

                    {
                        u32 levelWidth = FontGetMessageWidth(levelText);
                        f32 levelWidthF;

                        heightConv.words.hi = 0x43300000;
                        heightConv.words.lo = levelWidth & 0xFFFFU;
                        levelWidthF = (f32)(
                            heightConv.value -
                            *(const f64*)(roBase + 0x408));

                        FontDrawString(
                            (f64)(slotEntry->x -
                                levelWidthF * float_0p5_804220f0[0]),
                            (f64)(slotEntry->y + float_30_80422114),
                            levelText);
                    }

                    if (*(volatile u32*)((u8*)gp + 0x16C) == 0) {
                        playFmt = str_PCTs__8042211c;
                    } else {
                        playFmt = str_PCTs_80422118;
                    }

                    sprintf(
                        playText,
                        playFmt,
                        msgSearch((char*)(roBase + 0x3E0)));
                    strcat(playText, unk_JP_US_EU_40_801703e8(hours, 2));
                    strcat(playText, str__80422124);
                    strcat(playText, unk_JP_US_EU_40_801703e8(minutes, 2));

                    fullWidth = FontGetMessageWidth(playText);
                    heightConv.words.hi = 0x43300000;
                    heightConv.words.lo = fullWidth & 0xFFFFU;
                    drawnWidth = (f32)(
                        heightConv.value -
                        *(const f64*)(roBase + 0x408));
                    if (float_200_804220cc[0] < drawnWidth) {
                        textScale = *(const LoadDrawVec3*)(roBase + 0x60);
                        textScale.x = float_200_804220cc[0] / drawnWidth;
                        FontDrawScaleVec(&textScale);
                        drawnWidth = float_200_804220cc[0];
                    }

                    FontDrawString(
                        (f64)(slotEntry->x -
                            drawnWidth * float_0p5_804220f0[0]),
                        (f64)slotEntry->y,
                        playText);

                    textScale = *(const LoadDrawVec3*)(roBase + 0x6C);
                    FontDrawScaleVec(&textScale);

                    sprintf(
                        prefixText,
                        playFmt,
                        msgSearch((char*)(roBase + 0x3E0)));
                    strcat(prefixText, unk_JP_US_EU_40_801703e8(hours, 2));
                    prefixWidth = FontGetMessageWidth(prefixText);
                    heightConv.words.hi = 0x43300000;
                    heightConv.words.lo = prefixWidth & 0xFFFFU;
                    iconOffset = (f32)(
                        heightConv.value -
                        *(const f64*)(roBase + 0x408));

                    if (drawnWidth <= float_200_804220cc[0]) {
                        iconOffset += float_8_8042212c;
                    } else {
                        iconOffset =
                            iconOffset *
                                (float_200_804220cc[0] / drawnWidth) +
                            float_1600_80422128 / drawnWidth;
                        drawnWidth = float_200_804220cc[0];
                    }

                    PSMTXTrans(
                        iconMtx,
                        (f64)(iconOffset +
                            (slotEntry->x -
                                drawnWidth * float_0p5_804220f0[0])),
                        (f64)(float_1_804220a8[0] +
                            (slotEntry->y - float_22_80422130)),
                        (f64)float_0_804220a4[0]);
                    PSMTXScale(
                        iconScaleMtx,
                        float_1p5_80422134,
                        float_1p5_80422134,
                        float_1p5_80422134);
                    PSMTXConcat(iconMtx, iconScaleMtx, iconMtx);
                    iconColor = dat_80422068;
                    iconDispGxCol(iconMtx, 0, 0x208, &iconColor);

                    stoneIx = 0;
                    stoneOffset = 0;
                    stonePtr = stone_id;
                    do {
                        if ((stoneFlags &
                                (1U << ((stoneIx + 1) & 0x3F))) != 0) {
                            u32* stoneColorPtr = &normalColor;
                            u32 stoneColor;

                            if (slot == wp[8]) {
                                stoneColorPtr = &hoverColor;
                            }
                            stoneColor = *stoneColorPtr;

                            PSMTXTrans(
                                iconMtx,
                                (f64)((slotEntry->x -
                                    float_106_80422138) +
                                    (f32)stoneOffset),
                                (f64)(slotEntry->y -
                                    float_52_8042213c),
                                (f64)float_0_804220a4[0]);
                            iconDispGxCol(
                                iconMtx,
                                0,
                                (u16)*stonePtr,
                                &stoneColor);
                        }

                        stoneIx++;
                        stonePtr++;
                        stoneOffset += 0x20;
                    } while (stoneIx < 7);
                }

                slot++;
                slotEntry++;
                fileOffset += 0x4000;
            } while (slot < 4);
        }

        /* Mask fade before the sound/rumble option overlays. */
        if ((*(u16*)wp & 1) == 0) {
            *(f32*)((u8*)wp + 0x2C) -= float_0p05_80422140;
        } else {
            *(f32*)((u8*)wp + 0x2C) += float_0p05_80422140;
        }
        if (*(f32*)((u8*)wp + 0x2C) < float_0_804220a4[0]) {
            *(f32*)((u8*)wp + 0x2C) = float_0_804220a4[0];
        }
        if (*(f32*)((u8*)wp + 0x2C) > float_1_804220a8[0]) {
            *(f32*)((u8*)wp + 0x2C) = float_1_804220a8[0];
        }
        nameMaskGX(*(f32*)((u8*)wp + 0x2C));

        /*
         * Sound-output option overlay: states 100..199.
         */
        if (wp[6] >= 100 && wp[6] < 200) {
            LoadDrawVec3 pos;
            LoadDrawVec3 scale;
            u32 color;
            s32 option;
            f32 optionOffset;

            winTexInit(**(void***)((s32)wp[1] + 0xA0));
            pos = *(const LoadDrawVec3*)(roBase + 0x78);
            scale = *(const LoadDrawVec3*)(roBase + 0x84);
            color = dat_8042206c;
            winTexSet(0xF, &pos, &scale, &color);

            winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));
            pos = *(const LoadDrawVec3*)(roBase + 0x90);
            scale = *(const LoadDrawVec3*)(roBase + 0x9C);
            color = dat_80422070;
            winTexSet_x2(0x13, 0x1F, &pos, &scale, &color);

            option = 0;
            optionOffset = float_0_804220a4[0];
            do {
                if (option == wp[9]) {
                    winTexInit(**(void***)((s32)wp[1] + 0xA0));
                    pos = *(const LoadDrawVec3*)(roBase + 0xA8);
                    scale = *(const LoadDrawVec3*)(roBase + 0xB4);
                    pos.y = float_50_80422154 - optionOffset;
                    color = dat_80422074;
                    winTexSet(0x33, &pos, &scale, &color);

                    winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));
                    pos = *(const LoadDrawVec3*)(roBase + 0xC0);
                    scale = *(const LoadDrawVec3*)(roBase + 0xCC);
                    pos.y = (float_50_80422154 + float_4_80422160) - optionOffset;
                    color = dat_80422078;
                    winTexSet_x2(option + 0x15, 0x33, &pos, &scale, &color);
                } else {
                    winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));
                    pos = *(const LoadDrawVec3*)(roBase + 0xD8);
                    scale = *(const LoadDrawVec3*)(roBase + 0xE4);
                    pos.y = float_50_80422154 - optionOffset;
                    color = dat_8042207c;
                    winTexSet_x2(option + 0x15, 0x33, &pos, &scale, &color);
                }

                option++;
                optionOffset += float_50_80422154;
            } while (option < 3);
        }

        /*
         * Rumble option overlay: states 200..299.
         */
        if (wp[6] >= 200 && wp[6] < 300) {
            LoadDrawVec3 pos;
            LoadDrawVec3 scale;
            u32 color;
            s32 option;
            f32 optionOffset;

            winTexInit(**(void***)((s32)wp[1] + 0xA0));
            pos = *(const LoadDrawVec3*)(roBase + 0xF0);
            scale = *(const LoadDrawVec3*)(roBase + 0xFC);
            color = dat_80422080;
            winTexSet(0xF, &pos, &scale, &color);

            winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));
            pos = *(const LoadDrawVec3*)(roBase + 0x108);
            scale = *(const LoadDrawVec3*)(roBase + 0x114);
            color = dat_80422084;
            winTexSet_x2(0x14, 0x1F, &pos, &scale, &color);

            option = 0;
            optionOffset = float_0_804220a4[0];
            do {
                if (option == wp[10]) {
                    winTexInit(**(void***)((s32)wp[1] + 0xA0));
                    pos = *(const LoadDrawVec3*)(roBase + 0x120);
                    scale = *(const LoadDrawVec3*)(roBase + 0x12C);
                    pos.y = float_30_80422114 - optionOffset;
                    color = dat_80422088;
                    winTexSet(0x20, &pos, &scale, &color);

                    winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));
                    pos = *(const LoadDrawVec3*)(roBase + 0x138);
                    scale = *(const LoadDrawVec3*)(roBase + 0x144);
                    pos.y = (float_30_80422114 + float_4_80422160) - optionOffset;
                    color = dat_8042208c;
                    winTexSet_x2(option + 0x18, 0x20, &pos, &scale, &color);
                } else {
                    winTexInit_x2(**(void***)((s32)wp[1] + 0xA0));
                    pos = *(const LoadDrawVec3*)(roBase + 0x150);
                    scale = *(const LoadDrawVec3*)(roBase + 0x15C);
                    pos.y = float_30_80422114 - optionOffset;
                    color = dat_80422090;
                    winTexSet_x2(option + 0x18, 0x20, &pos, &scale, &color);
                }

                option++;
                optionOffset += float_50_80422154;
            } while (option < 2);
        }

        /*
         * Copy-selection rotating overlay for state 0x33. The explicit
         * table copy and signed conversion preserve the required codegen shape.
         */
        if (wp[6] == 0x33 && wp[13] < 4) {
            LoadDrawAngleBlock angleBlock;
            LoadDrawDoubleBits groupHalfBits;
            LoadDrawDoubleBits slotHalfBits;
            LoadDrawDoubleBits remainderBits;
            u32 angleByteOffset;
            f32 angle;
            f32 rotX;
            f32 rotY;

            angleBlock.words[0]  = *(const u32*)(roBase + 0x168);
            angleBlock.words[1]  = *(const u32*)(roBase + 0x16C);
            angleBlock.words[2]  = *(const u32*)(roBase + 0x170);
            angleBlock.words[3]  = *(const u32*)(roBase + 0x174);
            angleBlock.words[4]  = *(const u32*)(roBase + 0x178);
            angleBlock.words[5]  = *(const u32*)(roBase + 0x17C);
            angleBlock.words[6]  = *(const u32*)(roBase + 0x180);
            angleBlock.words[7]  = *(const u32*)(roBase + 0x184);
            angleBlock.words[8]  = *(const u32*)(roBase + 0x188);
            angleBlock.words[9]  = *(const u32*)(roBase + 0x18C);
            angleBlock.words[10] = *(const u32*)(roBase + 0x190);
            angleBlock.words[11] = *(const u32*)(roBase + 0x194);
            angleBlock.words[12] = *(const u32*)(roBase + 0x198);
            angleBlock.words[13] = *(const u32*)(roBase + 0x19C);
            angleBlock.words[14] = *(const u32*)(roBase + 0x1A0);
            angleBlock.words[15] = *(const u32*)(roBase + 0x1A4);

            angleByteOffset =
                ((u32)wp[12] << 4) + ((u32)wp[13] << 2);
            angle = *(f32*)((u8*)angleBlock.words + angleByteOffset);

            rotX = float_0_804220a4[0];
            rotY = float_50_80422154;

            if ((f32)fmod((f64)angle, *(const f64*)(roBase + 0x400)) ==
                float_0_804220a4[0]) {
                s32 group = wp[12];
                s32 slotGroup = wp[13];
                u32 groupSign = (u32)group >> 31;
                u32 slotSign = (u32)slotGroup >> 31;
                s32 groupHalfInt =
                    (group + (s32)groupSign) >> 1;
                s32 slotHalfInt =
                    (slotGroup + (s32)slotSign) >> 1;
                s32 groupRemainder =
                    (s32)(((u32)group & 1U) ^ groupSign) -
                    (s32)groupSign;
                f32 groupHalf;
                f32 slotHalf;
                f32 remainder;
                f64 signedBias = *(const f64*)(roBase + 0x410);

                groupHalfBits.words.hi = 0x43300000;
                groupHalfBits.words.lo =
                    ((u32)groupHalfInt) ^ 0x80000000U;
                slotHalfBits.words.hi = 0x43300000;
                slotHalfBits.words.lo =
                    ((u32)slotHalfInt) ^ 0x80000000U;
                remainderBits.words.hi = 0x43300000;
                remainderBits.words.lo =
                    ((u32)groupRemainder) ^ 0x80000000U;

                groupHalf =
                    (f32)(groupHalfBits.value - signedBias);
                slotHalf =
                    (f32)(slotHalfBits.value - signedBias);
                remainder =
                    (f32)(remainderBits.value - signedBias);

                if (groupHalf == slotHalf) {
                    rotY = -(
                        float_140_80422148 * groupHalf -
                        float_120_80422144);
                } else {
                    rotX =
                        float_300_80422150 * remainder +
                        float_neg150_8042214c;
                }
            }

            rotY -= float_40_804220d4[0];

            {
                LoadDrawVec3 shadowPos;
                LoadDrawVec3 shadowScale;
                LoadDrawVec3 mainPos;
                LoadDrawVec3 mainScale;
                u32 shadowColor;
                u32 mainColor;

                winTexInit(**(void***)((s32)wp[1] + 0xA0));

                shadowColor = dat_80422094;
                shadowPos =
                    *(const LoadDrawVec3*)(roBase + 0x1A8);
                shadowScale =
                    *(const LoadDrawVec3*)(roBase + 0x1B4);
                shadowPos.x = rotX + float_2_80422158;
                shadowPos.y = rotY - float_2_80422158;
                winTexSetRot(
                    angle, 0x1D,
                    &shadowPos, &shadowScale, &shadowColor);

                mainColor = hoverColor;
                mainPos =
                    *(const LoadDrawVec3*)(roBase + 0x1C0);
                mainScale =
                    *(const LoadDrawVec3*)(roBase + 0x1CC);
                mainPos.x = rotX;
                mainPos.y = rotY;
                winTexSetRot(
                    angle, 0x1D,
                    &mainPos, &mainScale, &mainColor);
            }
        }

        /* Main selection cursor with separate shadow/main stack objects. */
        if ((*(u16*)wp & 0x40) == 0) {
            volatile LoadDrawVec3 shadowStage;
            LoadDrawVec3 shadowPos;
            LoadDrawVec3 shadowScale;
            volatile LoadDrawVec3 mainStage;
            LoadDrawVec3 mainPos;
            LoadDrawVec3 mainScale;
            u32 shadowColor;
            u32 mainColor;

            winTexInit(**(void***)((s32)wp[1] + 0xA0));

            shadowColor = dat_80422098;
            shadowStage =
                *(const LoadDrawVec3*)(roBase + 0x1D8);
            shadowStage.x =
                *(f32*)((u8*)wp + 8) + float_2_80422158;
            shadowStage.y =
                *(f32*)((u8*)wp + 0xC) - float_2_80422158;
            shadowPos.x = shadowStage.x;
            shadowPos.y = shadowStage.y;
            shadowPos.z = shadowStage.z;
            shadowScale =
                *(const LoadDrawVec3*)(roBase + 0x1E4);
            winTexSet(
                0, &shadowPos, &shadowScale, &shadowColor);

            mainColor = hoverColor;
            mainStage =
                *(const LoadDrawVec3*)(roBase + 0x1F0);
            mainStage.x = *(f32*)((u8*)wp + 8);
            mainStage.y = *(f32*)((u8*)wp + 0xC);
            mainPos.x = mainStage.x;
            mainPos.y = mainStage.y;
            mainPos.z = mainStage.z;
            mainScale =
                *(const LoadDrawVec3*)(roBase + 0x1FC);
            winTexSet(
                0, &mainPos, &mainScale, &mainColor);
        }

        GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
        GXSetProjectionv(projection);

        currentCamera = (CameraCopy*)camGetCurPtr();
        *currentCamera = cameraBackup;
    }

    return 0;
}

u8 continueGame(void) {
    extern void cardCopy2Main(s32);
    extern void marioPartyKill(void);
    extern void psndSetFlag(s32);
    extern void marioPartyEntry(s32);
    extern void psndClearFlag(s32);
    extern void stg0_00_init(void);
    extern char str_aji_10_802eddcc[];
    extern char str_kpa_00_802eddd4[];
    extern char str_nok_01_802edddc[];
    extern char str_nokotarou_802edde4[];
    extern char str_hei_00_802eddf0[];
    extern char str_mri_00_802eddf8[];
    extern char str_mri_01_802ede00[];
    extern char str_tou_02_802ede08[];
    extern char str_usu_00_802ede10[];
    extern char str_gor_04_802ede18[];
    extern char str_muj_12_802ede20[];
    extern char str_muj_00_802ede28[];
    extern char str_pik_03_802ede30[];
    extern char str_gor_01_802ede38[];
    extern char str_resha_return_802ede40[];
    extern char str_pik_01_802ede50[];
    extern char str_aji_18_802ede58[];
    extern char str_e_bero_802ede60[];
    extern char str_gor_00_802ede68[];
    extern char str_jin_04_802ede70[];
    extern char str_aaa_00_802ede78[];
    extern char str_prologue_802ede80[];
    extern char str_next_8042209c[];
    void* player;
    s32 id;

    cardCopy2Main(wp[8]);
    *(s32*)((s32)gp + 0x11D0) = wp[8];
    *(s32*)((s32)gp + 0x11B8) = wp[8];
    marioPartyKill();
    player = marioGetPtr();
    *(s8*)((s32)player + 0x247) = 0;
    *(s8*)((s32)player + 0x248) = 0;
    psndSetFlag(4);
    marioPartyEntry(*(s32*)((s32)gp + 0x11E0));
    marioPartyEntry(*(s32*)((s32)gp + 0x11E4));
    psndClearFlag(4);
    countDownSaveReStart();
    *(u32*)gp |= 1;

    switch (swByteGet(4)) {
        case 0:
            *(s32*)((s32)gp + 0x110) = 0;
            *(s32*)((s32)gp + 0x108) = 0x1C;
            seqSetSeq(3, (void*)((s32)gp + 0x12C), NULL);
            break;
        case 1:
        case 4:
        case 7:
        case 0x18:
            *(s32*)((s32)gp + 0x110) = 0x3D;
            *(s32*)((s32)gp + 0x108) = 0x38;
            seqSetSeq(3, str_aji_10_802eddcc, NULL);
            break;
        case 2:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_kpa_00_802eddd4, NULL);
            break;
        case 3:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_nok_01_802edddc, str_nokotarou_802edde4);
            break;
        case 5:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_hei_00_802eddf0, NULL);
            break;
        case 6:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_mri_00_802eddf8, NULL);
            break;
        case 8:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_mri_01_802ede00, NULL);
            break;
        case 9:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_tou_02_802ede08, NULL);
            break;
        case 10:
            *(s32*)((s32)gp + 0x110) = 0;
            *(s32*)((s32)gp + 0x108) = 0x1C;
            seqSetSeq(3, str_usu_00_802ede10, NULL);
            break;
        case 0xB:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_gor_04_802ede18, NULL);
            break;
        case 0xC:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_usu_00_802ede10, NULL);
            break;
        case 0xD:
            *(s32*)((s32)gp + 0x110) = 0;
            *(s32*)((s32)gp + 0x108) = 0x1C;
            seqSetSeq(3, str_muj_12_802ede20, NULL);
            break;
        case 0xE:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_usu_00_802ede10, NULL);
            break;
        case 0xF:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_muj_00_802ede28, NULL);
            break;
        case 0x10:
            *(s32*)((s32)gp + 0x110) = 0;
            *(s32*)((s32)gp + 0x108) = 0x1C;
            seqSetSeq(3, str_pik_03_802ede30, str_next_8042209c);
            break;
        case 0x11:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_gor_01_802ede38, NULL);
            break;
        case 0x12:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_gor_04_802ede18, str_resha_return_802ede40);
            break;
        case 0x13:
            *(s32*)((s32)gp + 0x110) = 0x3E;
            *(s32*)((s32)gp + 0x108) = 0x3A;
            seqSetSeq(3, str_pik_01_802ede50, NULL);
            break;
        case 0x14:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_aji_18_802ede58, str_e_bero_802ede60);
            break;
        case 0x15:
            *(s32*)((s32)gp + 0x110) = 0x3C;
            *(s32*)((s32)gp + 0x108) = 0x36;
            seqSetSeq(3, str_gor_00_802ede68, NULL);
            break;
        case 0x16:
            *(s32*)((s32)gp + 0x110) = 0;
            *(s32*)((s32)gp + 0x108) = 0x1C;
            seqSetSeq(3, str_jin_04_802ede70, NULL);
            break;
        case 0x17:
            stg0_00_init();
            *(s32*)((s32)gp + 0x11D0) = wp[8];
            marioPartyKill();
            player = marioGetPtr();
            *(s8*)((s32)player + 0x247) = 0;
            *(s8*)((s32)player + 0x248) = 0;
            *(s32*)((s32)gp + 0x110) = 0;
            *(s32*)((s32)gp + 0x108) = 0x11;
            swInit();
            swByteSet(0, 0);
            seqSetSeq(3, str_aaa_00_802ede78, str_prologue_802ede80);
            break;
    }
}

const u32 dat_80422010 = 0x000000FF;
const char str_jp_80422014[] = "jp";
const char str_us_80422018[] = "us";
const char str_ge_8042201c[] = "ge";
const char str_fr_80422020[] = "fr";
const char str_sp_80422024[] = "sp";
const char str_it_80422028[] = "it";
const u32 dat_8042202c = 0x000000FF;
const u32 dat_80422030 = 0x000000FF;
const u32 dat_80422034 = 0x000000FF;
const u32 dat_80422038 = 0x000000FF;
const u32 dat_8042203c = 0x000000FF;
const u32 dat_80422040 = 0x000000FF;
const u32 dat_80422044 = 0xFFFFFFFF;
const u32 dat_80422048 = 0xA0A0A0FF;
const u32 dat_8042204c = 0x80808080;
const u32 dat_80422050 = 0xFFFFFFFF;
const u32 dat_80422054 = 0xFFFFFFFF;
const u32 dat_80422058 = 0xC0C0C0FF;
const u32 dat_8042205c = 0xFFFFFFFF;
const u32 dat_80422060 = 0x202020FF;
const u32 dat_80422064 = 0x000000FF;
const u32 dat_80422068 = 0x000000FF;
const u32 dat_8042206c = 0xFFFFFFFF;
const u32 dat_80422070 = 0xFFFFFFFF;
const u32 dat_80422074 = 0x00000040;
const u32 dat_80422078 = 0xFFFFFFFF;
const u32 dat_8042207c = 0xFFFFFFFF;
const u32 dat_80422080 = 0xFFFFFFFF;
const u32 dat_80422084 = 0xFFFFFFFF;
const u32 dat_80422088 = 0x00000040;
const u32 dat_8042208c = 0xFFFFFFFF;
const u32 dat_80422090 = 0xFFFFFFFF;
const u32 dat_80422094 = 0x00000080;
const u32 dat_80422098 = 0x00000080;
const char str_next_8042209c[] = "next";

const f32 float_0_804220a4[1] = { 0.0f };
const f32 float_1_804220a8[1] = { 1.0f };
const f32 float_neg304_804220ac[1] = { -304.0f };
const f32 float_240_804220b0[1] = { 240.0f };
const f32 float_neg240_804220b4[1] = { -240.0f };
const f32 float_neg272_804220b8[1] = { -272.0f };
const f32 float_272_804220bc[1] = { 272.0f };
const f32 float_304_804220c0[1] = { 304.0f };
const f32 float_60_804220c4[1] = { 60.0f };
const f32 float_neg250_804220c8[1] = { -250.0f };
const f32 float_200_804220cc[1] = { 200.0f };
const f32 float_500_804220d0[1] = { 500.0f };
const f32 float_40_804220d4[1] = { 40.0f };
const f32 float_20_804220d8[1] = { 20.0f };
const f32 float_0p6_804220dc[1] = { 0.6f };
const char str_PCTsPCTs_804220e0[] = "%s%s";
const f32 float_100_804220e8[1] = { 100.0f };
const f32 float_84_804220ec[1] = { 84.0f };
const f32 float_0p5_804220f0[1] = { 0.5f };
const f32 float_28_804220f4[1] = { 28.0f };
const f32 float_70_804220f8[1] = { 70.0f };
const f32 float_12_804220fc[1] = { 12.0f };
