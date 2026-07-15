#include "sequence/seq_load.h"

extern s32* wp;
extern void* memset(void*, int, unsigned long);
extern void fileFree(void*);
extern void fadeReset(s32);


u8 loadDraw(void) {
    extern s32* wp;
    extern void* camGetPtr(s32);
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
    extern void FontDrawStart(void);
    extern f32 FontGetMessageWidth(char*);
    extern void FontDrawMessage(f32, f32, char*);
    extern void FontDrawColor(u32);
    extern void FontDrawScale(f32);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern char* msgSearch(char*);
    extern s32 sprintf(char*, char*, ...);
    extern char str_PCTd_802EDE90[];
    char number[32];
    char* message;
    f32 width;
    s32 state = wp[0];

    camGetPtr(1);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetNumTexGens(1);

    FontDrawStart();
    FontDrawColor(0xFFFFFFFF);
    FontDrawScale(1.0f);
    FontDrawEdge();
    if (state < 2) message = msgSearch((char*)0x802EDEA0);
    else if (state < 5) message = msgSearch((char*)0x802EDEB0);
    else message = msgSearch((char*)0x802EDEC0);
    width = FontGetMessageWidth(message);
    FontDrawMessage(304.0f - width * 0.5f, 360.0f, message);
    sprintf(number, str_PCTd_802EDE90, wp[8]);
    width = FontGetMessageWidth(number);
    FontDrawMessage(304.0f - width * 0.5f, 390.0f, number);
    FontDrawEdgeOff();
    return 0;
}

u8 continueGame(void) {
    extern void* gp;
    extern s32* wp;
    extern void cardCopy2Main(s32);
    extern void marioPartyKill(void);
    extern void* marioGetPtr(void);
    extern void psndSetFlag(s32);
    extern void marioPartyEntry(s32);
    extern void psndClearFlag(s32);
    extern void countDownSaveReStart(void);
    extern s32 swByteGet(s32);
    extern void swByteSet(s32, s32);
    extern void swInit(void);
    extern void seqSetSeq(s32, void*, void*);
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

u8 seq_loadExit(void) {
    void* file = (void*)wp[1];

    if (file != NULL) {
        fileFree(file);
    }
    wp[1] = 0;
}


u8 seq_loadInit(void) {
    memset(wp, 0, 0x4C);
}


s32 unk_800f72e4(void* param_1) {
    fadeReset(4);
    wp[0x12] = *(s32*)((s32)param_1 + 0x178);
    return 2;
}


u8 loadMain(void* param_1) {
    extern s32* wp;
    extern u16 keyGetButtonTrg(s32 chan);
    extern u32 keyGetDirRep(s32 chan);
    extern s32 cardGetCode(void);
    extern s32 cardIsExec(void);
    extern s32 fadeIsFinish(void);
    extern void* cardGetFilePtr(void);
    u16 buttons;
    u32 direction;
    s32 cardCode;
    s32 i;
    s32 fileBase;
    u16 flags;

    buttons = keyGetButtonTrg(0);
    direction = keyGetDirRep(0);
    cardCode = cardGetCode();
    if (cardIsExec() != 0 || fadeIsFinish() == 0 || wp[7] != cardCode) {
        buttons = 0;
        direction = 0;
    }
    wp[7] = cardCode;

    for (i = 0, fileBase = 0; i < 4; i++, fileBase += 0x4000) {
        flags = *(u16*)((s32)cardGetFilePtr() + fileBase + 0x2000);
        if ((flags & 1) == 0 || (flags & 2) != 0) {
            break;
        }
    }
    if (i < 4) {
        *(u16*)wp &= ~0x10;
    } else {
        *(u16*)wp |= 0x10;
    }

    *(u16*)wp &= ~0x20;
    for (i = 0, fileBase = 0; i < 4; i++, fileBase += 0x4000) {
        flags = *(u16*)((s32)cardGetFilePtr() + fileBase + 0x2000);
        if ((flags & 3) == 0) {
            break;
        }
    }
    if (i >= 4) {
        *(u16*)wp |= 0x20;
    }

    if (wp[1] == 0) {
        wp[1] = 1;
    } else if (wp[1] == 1 && (buttons & 0x100) != 0) {
        wp[1] = 10;
    } else if (wp[1] == 1 && direction != 0) {
        wp[2] = (wp[2] + 1) % 7;
    }
    return 0;
}

u8 seq_loadMain(void* param_1) {
    extern void* gp;
    extern s32* wp;
    extern char* dat_ptrarr_802edb88[];
    extern char str_PCTs_w_PCTs_name_tpl_802ee004[];
    extern u32 dat_80422010;
    extern s32 win_dt[];
    extern void* evt_memcard_check;
    extern s32 getMarioStDvdRoot(void);
    extern s32 fileAsyncf(s32, s32, const char*, ...);
    extern void* fileAllocf(s32, const char*, ...);
    extern void* evtEntryType(void*, s32, s32, s32);
    extern s32 evtCheckID(s32);
    extern s32 evtGetValue(void*, s32);
    extern void seqSetSeq(s32, void*, void*);
    extern s32 cardGetCode(void);
    extern s32 OSGetSoundMode(void);
    extern void SoundSetOutputMode(s32);
    extern void fadeEntry(s32, s32, void*);
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

