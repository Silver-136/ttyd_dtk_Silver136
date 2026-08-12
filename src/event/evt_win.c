#include "event/evt_win.h"

extern void* _wp;
extern void* mapalloc_base_ptr;
extern s32 main_evt;
extern s32 mario_status_point_table;
extern u8 itemDataTable[];
extern const char str_uwnPCTd_80423e18;
extern char str_yubicursor_2_802f7124[];

s32 evtGetValue(void* entry, s32 index);
s32 evtSetValue(void* entry, s32 index, s32 value);
void* pouchGetPtr(void);
void pouchReviseMarioParam(void);
s32 pouchEquipCheckBadgeIndex(s32 index);
void pouchUnEquipBadgeIndex(s32 index);
void pouchRevisePartyParam(void);
s32 sprintf(char* str, const char* fmt, ...);
void iconChange(char* name, u16 iconId);
void iconDelete(char* name);
void iconEntry2D(char* name, u16 flags);
void iconFlagOn(char* name, u16 flags);
void mapObjFlagOff(void* obj, u32 flags);
void winMgrDelete(s32 winId);
void* _mapAlloc(void* heap, u32 size);
void _mapFree(void* heap, void* ptr);
void* memset(void* ptr, int value, u32 size);
void* evtEntry(s32* script, s32 priority, s32 flags);
extern char* msgSearch(s32 msgId);
extern s32 winMgrAction(s32 winId);
extern void FontDrawStart(void);
extern void FontDrawEdge(void);
extern void FontDrawColor(void* color);
extern void FontDrawEdgeOff(void);
extern void mapObjGetPos(void* obj, void* pos);
extern void PSMTXTrans(void* mtx, f64 x, f64 y, f64 z);
extern void iconNumberDispGx(void* mtx, s32 value, s32 unk, void* color);
extern void* gp;
extern f32 float_30_80423f48;
extern s32 pouchGetCoin(void);

/* fallback stub-fill: map=unk_8017b350 addr=0x8017b350 size=0x00000260 */
int unk_8017b350(void) {
    extern s32 FontGetMessageWidth(char* msg);
    extern void FontDrawString(f64 x, f64 y, char* msg);
    extern void* camGetPtr(s32 camId);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern s32 strlen(char* str);
    extern u32 dat_80423d0c;
    extern u32 dat_80423d10;
    extern f32 float_108_80423e34;
    extern f32 float_0p5_80423eb8;
    extern f32 float_12_80423f54;
    extern char str_PCTd_80423f04[];
    void* work;
    char* msg;
    s32 i;
    s32 mapOffset;
    s32 itemOffset;
    void* flagPtr;
    void* cam;
    u32 color;
    f32 pos[3];
    char buf[0x40];
    f32 mtx[3][4];
    s32 item;

    work = _wp;
    item = *(s32*)(*(s32*)((s32)work + 8) + *(s32*)((s32)work + 0x24) * 4);
    msg = msgSearch(*(s32*)((s32)itemDataTable + item * 0x28 + 4));
    if (winMgrAction(*(s32*)((s32)work + 0x60)) == 0) {
        FontDrawStart();
        FontDrawEdge();
        color = dat_80423d0c;
        FontDrawColor(&color);
        FontDrawString((f32)(-(s32)(FontGetMessageWidth(msg) & 0xFFFF) / 2), float_108_80423e34, msg);
        FontDrawEdgeOff();
    }

    mapOffset = 0;
    itemOffset = 0;
    flagPtr = work;
    i = 0;
    while (i < *(s32*)((s32)work + 0x18)) {
        if ((*(u16*)((s32)flagPtr + 0xC) & 1) == 0) {
            mapObjGetPos(*(void**)(*(s32*)((s32)work + 4) + mapOffset), pos);
            pos[1] += float_30_80423f48;
            cam = camGetPtr(4);
            PSMTXMultVec((void*)((s32)cam + 0x11C), pos, pos);
            cam = camGetPtr(4);
            PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
            pos[0] = pos[0] * (f32)*(u16*)((s32)gp + 0x170) * float_0p5_80423eb8;
            pos[1] = pos[1] * (f32)*(u16*)((s32)gp + 0x172) * float_0p5_80423eb8;
            item = *(s32*)(*(s32*)((s32)work + 8) + itemOffset);
            sprintf(buf, str_PCTd_80423f04, *(u16*)((s32)itemDataTable + item * 0x28 + 0x16));
            PSMTXTrans(mtx, pos[0] + float_12_80423f54 * (f32)(strlen(buf) - 1), pos[1], pos[2]);
            color = dat_80423d10;
            iconNumberDispGx(mtx, *(u16*)((s32)itemDataTable + item * 0x28 + 0x16), 0, &color);
        }
        flagPtr = (void*)((s32)flagPtr + 2);
        mapOffset += 0xC;
        itemOffset += 4;
        i++;
    }
}

void unk_8017b330(void) {
    extern void winMgrHelpMain();

    winMgrHelpMain();
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_8017b2b0(void* pWin) {
    extern void winMgrHelpDraw(void* pWin);
    s32 index;
    s32* itemTable;
    void* work;
    s32 item;
    void* itemData;

    work = _wp;
    index = *(s32*)((s32)work + 0x24) << 2;
    itemTable = *(s32**)((s32)work + 0x8);
    if (winMgrAction(*(s32*)((s32)work + 0x64)) == 0) {
        item = *(s32*)((s32)itemTable + index);
        itemData = itemDataTable;
        itemData = (void*)((s32)itemData + item * 0x28);
        *(char**)((s32)pWin + 0x34) = msgSearch(*(s32*)((s32)itemData + 8));
        winMgrHelpDraw(pWin);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* fallback stub-fill: map=unk_8017b0c4 addr=0x8017b0c4 size=0x000001ec */
int unk_8017b0c4(u32* pWin) {
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void iconDispGx2(void* mtx, s32 mode, u16 iconId);
    extern f32 float_10_80423f44;
    extern f32 float_36_80423ebc;
    extern f32 float_0_80423e58;
    extern f32 float_1p1_80423e88;
    extern f32 float_0p8_80423ecc;
    extern f32 float_124_80423f4c;
    extern f32 float_38_80423f50;
    extern u32 dat_80423d14;
    void* work;
    void* pouch;
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    u32 color;
    s32 diff;
    s32 step;

    work = _wp;
    pouch = pouchGetPtr();
    if ((*pWin & 4) == 0) {
        PSMTXTrans(mtxA,
                   (f32)((s32)pWin[6]) + float_30_80423f48 + float_10_80423f44,
                   (f32)((s32)pWin[7]) - float_36_80423ebc,
                   float_0_80423e58);
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            PSMTXScale(mtxB, float_1p1_80423e88, float_1p1_80423e88, float_1p1_80423e88);
            PSMTXConcat(mtxA, mtxB, mtxA);
            iconDispGx2(mtxA, 0x10, 0x226);
        } else {
            PSMTXScale(mtxB, float_0p8_80423ecc, float_1p1_80423e88, float_1p1_80423e88);
            PSMTXConcat(mtxA, mtxB, mtxA);
            iconDispGx2(mtxA, 0x10, 0x227);
        }
        PSMTXTrans(mtxA,
                   (f32)((s32)pWin[6]) + float_124_80423f4c,
                   (f32)((s32)pWin[7]) - float_38_80423f50,
                   float_0_80423e58);
        color = dat_80423d14;
        iconNumberDispGx(mtxA, *(s32*)((s32)work + 0x30), 0, &color);
        diff = *(s16*)((s32)pouch + 0x5B0) - *(s32*)((s32)work + 0x30);
        if (diff != 0) {
            step = diff / 10;
            if (step == 0) {
                step = -1;
                if (*(s32*)((s32)work + 0x30) < *(s16*)((s32)pouch + 0x5B0)) {
                    step = 1;
                }
            }
            *(s32*)((s32)work + 0x30) += step;
        }
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_unitwin_alloc(void* pEvt) {
    s32 value;
    void* work;
    void* pouch;

    value = evtGetValue(pEvt, **(s32**)((s32)pEvt + 0x18));
    work = _mapAlloc(mapalloc_base_ptr, 0xE8);
    memset(work, 0, 0xE8);
    *(s32*)((s32)work + 0x50) = 0;
    _wp = work;
    if (value != 0) {
        *(u16*)work |= 0x80;
    } else {
        *(u16*)work &= ~0x80;
    }
    iconEntry2D(str_yubicursor_2_802f7124, 0x1F8);
    iconFlagOn(str_yubicursor_2_802f7124, 2);
    iconFlagOn(str_yubicursor_2_802f7124, 4);
    if ((*(u16*)work & 0x80) == 0) {
        *(void**)((s32)work + 0x1C) = evtEntry(&main_evt, 0, 0);
        *(void**)((s32)*(void**)((s32)work + 0x1C) + 0x9C) = work;
    }
    pouch = pouchGetPtr();
    mario_status_point_table = (s32)pouch + 0x8E;
    pouch = pouchGetPtr();
    ((s32*)&mario_status_point_table)[1] = (s32)pouch + 0x90;
    pouch = pouchGetPtr();
    ((s32*)&mario_status_point_table)[2] = (s32)pouch + 0x94;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_unitwin_free(void* pEvt, s32 force) {
    void* winPtr;
    s32 offset;
    s32 i;
    void* work;
    char name[0x20];

    work = _wp;
    if (work == 0) {
        return 2;
    }

    if (force != 0) {
        *(u16*)((s32)work + 0x54) |= 1;
    }

    if ((*(u16*)((s32)work + 0x54) & 2) == 0) {
        return 0;
    }

    i = 0;
    offset = 0;
    while (i < *(s32*)((s32)work + 0x18)) {
        mapObjFlagOff(*(void**)(*(s32*)((s32)work + 0x4) + offset), 0x4000);
        sprintf(name, &str_uwnPCTd_80423e18, i);
        iconDelete(name);
        offset += 0xC;
        i++;
    }

    if ((*(u16*)work & 0x80) != 0) {
        i = 0;
        winPtr = work;
        while (i < 3) {
            winMgrDelete(*(s32*)((s32)winPtr + 0x60));
            i++;
            winPtr = (void*)((s32)winPtr + 4);
        }
    }

    iconDelete(str_yubicursor_2_802f7124);
    _mapFree(mapalloc_base_ptr, _wp);
    _wp = 0;

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_unitwin_selltable_setup(int pEvt) {
    extern void mapObjFlagOn(void* obj, u32 flags);
    extern void iconEntry(char* name, u16 iconId);
    extern void iconSetPos(f32 x, f32 y, f32 z, char* name);
    extern void* evtEntryType(void* script, s32 priority, s32 flags, s32 type);
    extern s32 winMgrEntry(void* desc);
    extern f32 float_6_80423f40;
    extern void* select_evt;
    extern u8 unk_80377e58[];
    s32* args;
    s32 mapTable;
    s32 itemTable;
    void* work;
    s32 mapOffset;
    s32 itemOffset;
    s32 count;
    void* flagPtr;
    s32 i;
    void* desc;
    f32 pos[3];
    char name[0x28];

    args = *(s32**)(pEvt + 0x18);
    mapTable = evtGetValue((void*)pEvt, args[0]);
    itemTable = evtGetValue((void*)pEvt, args[1]);
    work = _wp;
    mapOffset = 0;
    itemOffset = 0;
    *(s32*)((s32)work + 4) = mapTable;
    flagPtr = work;
    count = 0;
    *(s32*)((s32)work + 8) = itemTable;
    *(s32*)((s32)work + 0x18) = 0;
    while (*(s32*)(*(s32*)((s32)work + 4) + mapOffset) != 0) {
        *(s32*)((s32)work + 0x18) = *(s32*)((s32)work + 0x18) + 1;
        mapObjGetPos(*(void**)(*(s32*)((s32)work + 4) + mapOffset), pos);
        mapObjFlagOn(*(void**)(*(s32*)((s32)work + 4) + mapOffset), 0x4000);
        if (*(s32*)(*(s32*)((s32)work + 8) + itemOffset) == 0) {
            *(u16*)((s32)flagPtr + 0xC) |= 1;
        } else {
            sprintf(name, &str_uwnPCTd_80423e18, count);
            iconEntry(name, *(u16*)((s32)itemDataTable + *(s32*)(*(s32*)((s32)work + 8) + itemOffset) * 0x28 + 0x20));
            iconSetPos(pos[0], pos[1] + float_6_80423f40, pos[2], name);
            iconFlagOn(name, 0x40);
        }
        mapOffset += 0xC;
        itemOffset += 4;
        flagPtr = (void*)((s32)flagPtr + 2);
        count++;
    }
    *(void**)((s32)work + 0x1C) = evtEntryType(&select_evt, 0, 0x20, 0);
    *(void**)((s32)*(void**)((s32)work + 0x1C) + 0x9C) = work;
    desc = unk_80377e58;
    flagPtr = work;
    i = 0;
    do {
        *(s32*)((s32)flagPtr + 0x60) = winMgrEntry(desc);
        i++;
        desc = (void*)((s32)desc + 0x28);
        flagPtr = (void*)((s32)flagPtr + 4);
    } while (i < 3);
    *(s32*)((s32)work + 0xE0) = -1;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwincoin_disp_onoff(void* pEvt) {
    s32 onoff;
    void* work;

    onoff = evtGetValue(pEvt, **(s32**)((s32)pEvt + 0x18));
    pouchGetPtr();
    work = _wp;
    *(s32*)((s32)work + 0x2C) = pouchGetCoin();
    *(s32*)((s32)work + 0x5C) = 0;
    if (onoff != 0) {
        *(u16*)work |= 0x1000;
    } else {
        *(u16*)work &= ~0x1000;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u32 unitwincoin_wait(void) {
    void* work;
    s32 coin;
    s32 stored;
    s32 result;

    work = _wp;
    pouchGetPtr();
    coin = pouchGetCoin();
    stored = *(s32*)((s32)work + 0x2C);
    result = (stored - coin) | (coin - stored);
    result >>= 31;
    return 2 & ~result;
}

s32 unk_8017abf8(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 value = evtGetValue(pEvt, args[0]);
    void* work = _wp;

    if (value != 0) {
        *(u16*)work |= 0x100;
    } else {
        *(u16*)work &= ~0x100;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unk_8017aba4(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* work = (void*)evtGetValue(pEvt, args[0]);

    *(s32*)((s32)work + 0xE4) = evtGetValue(pEvt, args[1]);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT FALLBACK MISSING STUBS: main/event/evt_win 20260624_191429 */

/* fallback stub-fill: map=unk_8017a844 addr=0x8017a844 size=0x00000360 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
int unk_8017a844(void* pEvt) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 slot);
    extern u16 keyGetButtonTrg(s32 port);
    extern s32 marioChkCtrl(void);
    extern s32 marioChkKey(void);
    extern s32 winGhostDiaryChk(void);
    extern s32 N_itemPickUpFromFieldCheck(void);
    extern char* hitGetName(void* hit);
    extern s32 strcmp(char* s1, char* s2);
    extern void winMgrOpen(s32 winId);
    extern void winMgrClose(s32 winId);
    extern void* winMgrGetPtr(s32 winId);
    extern void winMgrHelpInit(void* winMgr);
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    extern s32 evtCheckID(s32 evtId);
    extern void* mapGetMapObj(char* name);
    extern void iconFlagOn(char* name, u16 flags);
    extern void iconFlagOff(char* name, u16 flags);
    extern const char str_uwnPCTd_80423e18;

    void* work;
    void* mario;
    void* party;
    void* hit;
    u16 button;
    s32 found;
    s32 i;
    s32 offset;
    char name[0x20];

    found = 0;
    work = *(void**)((s32)pEvt + 0x9C);
    mario = marioGetPtr();
    party = partyGetPtr(0);
    hit = *(void**)((s32)mario + 0x1E8);

    if ((*(u16*)((s32)work + 0x54) & 1) != 0) {
        *(u16*)((s32)work + 0x54) |= 2;
        return 2;
    }

    *(u16*)work &= 0xFFFE;
    button = keyGetButtonTrg(0);
    if (marioChkCtrl() == 0 || marioChkKey() == 0) {
        button = 0;
    }

    if (party != NULL && (*(u32*)party & 0x100) != 0) {
        if (*(s32*)((s32)work + 0xE0) != -1) {
            *(s32*)((s32)work + 0xE0) = -1;
            winMgrClose(*(s32*)((s32)work + 0x60));
            winMgrClose(*(s32*)((s32)work + 0x64));
        }
        return 0;
    }

    if (winGhostDiaryChk() != 0 || N_itemPickUpFromFieldCheck() != 0) {
        if (*(s32*)((s32)work + 0xE0) != -1) {
            *(s32*)((s32)work + 0xE0) = -1;
            winMgrClose(*(s32*)((s32)work + 0x60));
            winMgrClose(*(s32*)((s32)work + 0x64));
        }
        return 0;
    }

    if ((*(u16*)work & 2) == 0) {
        if (hit != NULL) {
            for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0xC) {
                if (strcmp(*(char**)(*(s32*)((s32)work + 4) + offset + 4), hitGetName(hit)) == 0) {
                    break;
                }
            }
            if (i < *(s32*)((s32)work + 0x18) &&
                ((*(u16*)((s32)work + 0xC + i * 2) & 1) == 0)) {
                if (*(s32*)((s32)work + 0xE0) != i) {
                    if (*(s32*)((s32)work + 0xE0) == -1) {
                        winMgrOpen(*(s32*)((s32)work + 0x60));
                        winMgrOpen(*(s32*)((s32)work + 0x64));
                    }
                    *(s32*)((s32)work + 0xE0) = i;
                    *(s32*)((s32)work + 0x24) = i;
                    winMgrHelpInit(winMgrGetPtr(*(s32*)((s32)work + 0x64)));
                }
                found = 1;
            }
        }
        if (found == 0 && *(s32*)((s32)work + 0xE0) != -1) {
            *(s32*)((s32)work + 0xE0) = -1;
            winMgrClose(*(s32*)((s32)work + 0x60));
            winMgrClose(*(s32*)((s32)work + 0x64));
        }
        if (*(s32*)((s32)work + 0xE0) != -1) {
            *(u16*)work |= 1;
            if ((button & 0x100) != 0) {
                marioKeyOff();
                *(void**)((s32)work + 0x20) =
                    evtEntry(*(void**)(*(s32*)((s32)work + 4) +
                                       *(s32*)((s32)work + 0x24) * 0xC + 8), 0, 0);
                *(void**)(*(s32*)((s32)work + 0x20) + 0x9C) = work;
                *(u16*)work |= 2;
                winMgrClose(*(s32*)((s32)work + 0x60));
                winMgrClose(*(s32*)((s32)work + 0x64));
                *(s32*)((s32)work + 0xE0) = -1;
                *(s32*)((s32)work + 0xE4) = 0;
            }
        }
    } else {
        if (evtCheckID(*(s32*)(*(s32*)((s32)work + 0x20) + 0x15C)) == 0) {
            *(u16*)work &= 0xFFFD;
            marioKeyOn();
            if (*(s32*)((s32)work + 0xE4) != 0) {
                *(u16*)((s32)work + 0xC + *(s32*)((s32)work + 0x24) * 2) |= 1;
            }
        }
    }

    for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0xC) {
        void* obj = mapGetMapObj(*(char**)(*(s32*)((s32)work + 4) + offset));
        if (obj != NULL) {
            sprintf(name, &str_uwnPCTd_80423e18, i);
            if ((*(u16*)((s32)work + 0xC + i * 2) & 1) == 0) {
                if ((*(u32*)obj & 1) == 0) {
                    iconFlagOff(name, 2);
                } else {
                    iconFlagOn(name, 2);
                }
            } else {
                iconFlagOn(name, 2);
            }
        }
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_unitwin_main_func(void* pEvt) {
    extern f32 float_1000_80423f3c;
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority);
    extern void evt_unitwin_disp_func(s32 param_1, void* param_2);
    void* work = *(void**)((s32)pEvt + 0x9C);
    u16 flags = *(u16*)((s32)work + 0x54);

    if ((flags & 1) != 0) {
        *(u16*)((s32)work + 0x54) = flags | 2;
        return 2;
    }
    if (*(s32*)((s32)work + 0x28) > 0) {
        *(s32*)((s32)work + 0x28) = *(s32*)((s32)work + 0x28) - 1;
    } else {
        *(u16*)work = *(u16*)work & ~1;
    }
    dispEntry(8, 0, evt_unitwin_disp_func, float_1000_80423f3c);
    return 0;
}


void evt_unitwin_disp_func(s32 cameraId, void* work) {
    typedef f32 Mtx[3][4];
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern s32 pouchGetStarPiece(void);
    extern s32 pouchGetSuperCoin(void);
    extern s32 pouchGetHaveItemCnt(void);
    extern s32 pouchGetHaveBadgeCnt(void);
    extern s16 pouchHaveItem(s32 index);
    extern s16 pouchHaveBadge(s32 index);
    extern s32 pouchGetKeepItemCnt(void);
    extern s16 pouchKeepItem(s32 index);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 badgeShop_get(void* shop, s16 item);
    extern s32 badgeShop_ThrowCheck(s32 item);
    extern void FontDrawScale(f32 scale);
    extern u16 FontGetMessageWidth(char* text);
    extern char* strcpy(char* dst, const char* src);
    extern void GXGetScissor(s32* x, s32* y, s32* w, s32* h);
    extern void GXSetScissor(s32 x, s32 y, s32 w, s32 h);
    extern void iconDispGx(f32 scale, Vec3* pos, s32 flags, s32 iconId);
    extern void iconDispGxCol(Mtx m, s32 flags, s32 iconId, u32* color);
    extern void iconFlagOff(char* name, u16 flags);
    extern void iconFlagOn(char* name, u16 flags);
    extern void iconSetPos(f32 x, f32 y, f32 z, char* name);
    extern u32 badge_bottakuru100_table[];
    extern u8 bdsw[];
    extern u8 _jdt[];
    extern s32 johoya_get(s32 table, s16 id);
    extern s32 party_id_table[];
    extern s32 party_icon_table[];
    extern char* party_labelname_table[];
    extern s32 mario_status_icon_table[];
    extern char* mario_status_name_table[];
    extern char* mario_status_labelname_table[];
    extern s32 mario_status_henka_table[];
    extern char* pouchGetYoshiName(void);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void windowDispGX_Waku_col(f32 x, f32 y, f32 width, f32 height,
                                      f32 radius, s32 style, void* color);
    extern void FontDrawString(f32 x, f32 y, char* text);
    extern void FontDrawMessage(s32 x, s32 y, char* text);
    extern char* msgSearch(char* key);
    extern u32 strlen(const char* text);
    extern u8 itemDataTable[];
    extern void* lotteryGetPtr(void);
    extern s32 sprintf(char* buffer, const char* format, ...);
    extern void iconDispGx2(Mtx m, s32 alpha, s32 icon);
    extern void iconNumberDispGx(Mtx m, s32 number, s32 flag, void* color);
    extern void statusWinForceUpdateCoin(void);
    extern void mapObjGetPos(void* object, Vec3* position);
    extern void* camGetPtr(s32 cameraId);
    extern void GXSetProjection(void* matrix, s32 type);
    extern void GXGetProjectionv(f32* projection);
    void* pouch = pouchGetPtr();
    extern void GXGetViewportv(f32* viewport);
    extern void GXProject(f32 x, f32 y, f32 z, void* matrix,
                          f32* projection, f32* viewport,
                          f32* screenX, f32* screenY, f32* screenZ);
    Mtx trans, scale;
    u32 white = 0xFFFFFFFF;
    u32 titleFrameColor = 0xFFFFFFFF;
    u32 titleFontColor = 0xFFFFFFFF;
    u32 descriptionFrameColor = 0xFFFFFFFF;
    u16 flags = *(u16*)work;
    s32 value;
    s32 i;

    if ((flags & 1) == 0) return;
    {
        s32 selected = *(s32*)((s32)work + 0x24);
        s32 item = *(s32*)(*(s32*)((s32)work + 8) + selected * 4);
        char* itemName = *(char**)(itemDataTable + item * 0x28 + 4);
        char* itemDescription = *(char**)(itemDataTable + item * 0x28 + 8);

        windowDispGX_Waku_col(-150.0f, 120.0f, 300.0f, 50.0f,
                              20.0f, 0, &titleFrameColor);
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor(&titleFontColor);
        FontDrawString(-((f32)strlen(itemName) * 24.0f) * 0.25f,
                       108.0f, itemName);
        FontDrawEdgeOff();
        windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                              20.0f, 0, &descriptionFrameColor);
        FontDrawStart();
        FontDrawMessage(-220, -138, msgSearch(itemDescription));

        for (i = 0; i < *(s32*)((s32)work + 0x18); i++) {
            s32 listedItem = *(s32*)(*(s32*)((s32)work + 8) + i * 4);
            if (listedItem != 0) {
                Vec3 world;
                f32 projection[7];
                f32 viewport[6];
                f32 screenX, screenY, screenZ;
                void* camera;
                s32 number;

                mapObjGetPos(*(void**)(*(s32*)((s32)work + 4) + i * 0xC),
                             &world);
                camera = camGetPtr(4);
                GXSetProjection((u8*)camera + 0x15C,
                                *(s32*)((s32)camera + 0x19C));
                GXGetProjectionv(projection);
                GXGetViewportv(viewport);
                GXProject(world.x, world.y, world.z,
                          (u8*)camera + 0x11C, projection, viewport,
                          &screenX, &screenY, &screenZ);
                camera = camGetPtr(cameraId);
                GXSetProjection((u8*)camera + 0x15C,
                                *(s32*)((s32)camera + 0x19C));
                if (listedItem >= 0xF0 && listedItem < 0x153)
                    number = *(u16*)(itemDataTable + listedItem * 0x28 + 0x16) & 0xFF;
                else
                    number = *(u16*)(itemDataTable + listedItem * 0x28 + 0x14) & 0xFF;
                PSMTXTrans(trans, screenX - 296.0f,
                           300.0f - screenY, 0.0f);
                iconNumberDispGx(trans, number, 1, &white);
            }
        }
    }
    if ((flags & 0xD000) != 0) {
        windowDispGX_Waku_col(-250.0f, -75.0f, 150.0f, 45.0f,
                              20.0f, 0, &white);
        PSMTXTrans(trans, -220.0f, -113.0f, 0.0f);
        PSMTXScale(scale, 0.7f, 0.7f, 0.7f);
        PSMTXConcat(trans, scale, trans);
        if ((flags & 0x1000) != 0) {
            iconDispGx2(trans, 0x10, 0x193);
            value = pouchGetCoin();
        } else if ((flags & 0x4000) != 0) {
            iconDispGx2(trans, 0x10, 0x10A);
            value = pouchGetSuperCoin();
        } else {
            iconDispGx2(trans, 0x10, 0x195);
            value = pouchGetStarPiece();
        }
        PSMTXTrans(trans, -192.0f, -113.0f, 0.0f);
        iconDispGx2(trans, 0x10, 0x1DE);
        PSMTXTrans(trans, -126.0f, -113.0f, 0.0f);
        iconNumberDispGx(trans, *(s32*)((s32)work + 0x2C), 0, &white);
        if (*(s32*)((s32)work + 0x2C) != value) {
            s32 delta = (value - *(s32*)((s32)work + 0x2C)) / 10;
            if (delta == 0) delta = value > *(s32*)((s32)work + 0x2C) ? 1 : -1;
            if ((*(u32*)((s32)work + 0x5C) & 1) != 0) {
                *(s32*)((s32)work + 0x2C) += delta;
                statusWinForceUpdateCoin();
            }
            *(s32*)((s32)work + 0x5C) += 1;
        }
    }
    if ((flags & 0x2000) != 0) {
        windowDispGX_Waku_col(-250.0f, -130.0f, 150.0f, 45.0f,
                              20.0f, 0, &white);
        PSMTXTrans(trans, -210.0f, -166.0f, 0.0f);
        PSMTXScale(scale, 1.1f, 1.1f, 1.1f);
        PSMTXConcat(trans, scale, trans);
        iconDispGx2(trans, 0x10, 0x226);
        PSMTXTrans(trans, -126.0f, -168.0f, 0.0f);
        iconNumberDispGx(trans, *(s32*)((s32)work + 0x30), 0, &white);
        if ((*(u32*)((s32)gp + 0x1C) & 3) == 0) {
            s32 target = *(s16*)((s32)pouch + 0x9C);
            s32 shown = *(s32*)((s32)work + 0x30);
            s32 delta = (target - shown) / 10;
            if (delta == 0 && target != shown)
                delta = target > shown ? 1 : -1;
            *(s32*)((s32)work + 0x30) = shown + delta;
        }
    }
    if ((flags & 0x100) != 0 && (*(u16*)lotteryGetPtr() & 2) != 0) {
        char digits[8];
        void* lottery = lotteryGetPtr();

        windowDispGX_Waku_col(-250.0f, -130.0f, 150.0f, 45.0f,
                              20.0f, 0, &white);
        PSMTXTrans(trans, -220.0f, -172.0f, 0.0f);
        iconDispGx2(trans, 0x10, 0x148);
        sprintf(digits, "%04d", (s32)*(s16*)((s32)lottery + 0x20));
        for (i = 0; i < 4; i++) {
            PSMTXTrans(trans, -186.0f + 20.0f * (f32)i,
                       -168.0f, 0.0f);
            iconNumberDispGx(trans, digits[i] - '0', 0, &white);
        }
    }
    {
        s32 menuState;
        f32 scrollY;
        f32 selectedRow;
        s32 freeRows;
        s32 listHeight;
        s32 oldX, oldY, oldW, oldH;
        s32 visible;
        s32 itemIndex;
        s32 itemId;
        s32 limit;
        s32 titleOffset;
        s32 priceX;
        f32 priceScale;
        char* title;
        char* description;
        char priceText[0x20];
        Vec3 iconPos;
        Vec3 priceIconPos;
        Mtx listTrans;
        Mtx listScale;

        menuState = *(s32*)((s32)work + 0x50);
        scrollY = *(f32*)((s32)work + 0x44);
        selectedRow = *(f32*)((s32)work + 0x34);

        switch (menuState) {
        case 4:
            visible = 0;
            description = 0;
            freeRows = 8 - pouchGetKeepItemCnt();
            if (freeRows < 1)
                freeRows = 0;

            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f,
                                  (f32)(220 - freeRows * 24),
                                  20.0f, 0, &white);
            windowDispGX_Waku_col(51.0f, 114.0f, 150.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(81.0f, 111.0f, msgSearch("msg_window_title_1"));
            FontDrawEdgeOff();

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            for (i = 0; i < 0x20; i++) {
                itemId = (s32)pouchKeepItem(i);
                if (itemId != 0) {
                    iconPos.x = 11.0f;
                    iconPos.y = scrollY + (f32)(0x38 - visible * 24);
                    iconPos.z = 0.0f;
                    iconDispGx(0.5f, &iconPos, 0x10,
                               (s32)*(s16*)(itemDataTable + itemId * 0x28 + 0x20));

                    FontDrawStart();
                    FontDrawString(36.0f,
                                   scrollY + (f32)(0x50 - visible * 24),
                                   *(char**)(itemDataTable + itemId * 0x28));

                    if ((f32)visible == selectedRow)
                        description = *(char**)(itemDataTable + itemId * 0x28 + 8);

                    visible++;
                }
            }

            GXSetScissor(oldX, oldY, oldW, oldH);

            if (description != 0) {
                windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                                      20.0f, 0, &white);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(description));
            }
            break;

        case 2:
        case 3:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:
        case 0x10:
            visible = 0;
            description = 0;

            if (menuState == 2 || menuState == 3) {
                freeRows = 8 - pouchGetHaveItemCnt();
                if (freeRows < 1)
                    freeRows = 0;
                limit = 0x14;
                title = msgSearch("msg_window_title_1");
                titleOffset = 0x1E;
            } else if (menuState == 0xC) {
                freeRows = 8 - pouchGetHaveBadgeCnt();
                if (freeRows < 1)
                    freeRows = 0;
                limit = 200;
                title = msgSearch("msg_window_title_2");
                titleOffset = 0x28;
            } else if (menuState == 0xE) {
                freeRows = 8 - *(s32*)((s32)work + 0x58);
                if (freeRows < 0)
                    freeRows = 0;
                limit = 0x152;
                title = msgSearch("msg_window_title_3");
                titleOffset = 0x14;
            } else if (menuState == 0xF) {
                limit = *(s32*)((s32)work + 0x58);
                freeRows = 8 - limit;
                if (freeRows < 0)
                    freeRows = 0;
                title = msgSearch("msg_window_title_3");
                titleOffset = 0x14;
            } else if (menuState == 0x10) {
                freeRows = 8 - *(s32*)((s32)work + 0x58);
                if (freeRows < 0)
                    freeRows = 0;
                limit = 0x152;
                title = msgSearch("msg_window_title_4");
                titleOffset = 0x1E;
            } else {
                freeRows = 8 - *(s32*)((s32)work + 0x58);
                if (freeRows < 0)
                    freeRows = 0;
                limit = 99;
                title = msgSearch("msg_window_title_2");
                titleOffset = 0x28;
            }

            listHeight = 220 - freeRows * 24;
            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f, (f32)listHeight,
                                  20.0f, 0, &white);
            windowDispGX_Waku_col(51.0f, 114.0f, 150.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString((f32)(0x33 + titleOffset), 111.0f, title);
            FontDrawEdgeOff();

            if (menuState != 3) {
                PSMTXTrans(listTrans, 244.0f, 82.0f, 0.0f);
                PSMTXScale(listScale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(listTrans, listScale, listTrans);
                iconDispGxCol(listTrans, 0x10, 0x213, &white);

                priceIconPos.x = 244.0f;
                priceIconPos.y = 86.0f;
                priceIconPos.z = 0.0f;
                if (menuState == 0xD) {
                    itemIndex = 0x195;
                } else if (menuState == 0x10) {
                    itemIndex = 0x147;
                } else {
                    itemIndex = 0x193;
                }
                iconDispGx(0.6f, &priceIconPos, 0x10, itemIndex);
            }

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            for (i = 0; i < limit; i++) {
                u32 itemColor;
                s32 iconFlags;

                itemId = 0;
                itemColor = 0xFFFFFFFF;
                iconFlags = 0x10;

                switch (menuState) {
                case 2:
                case 3:
                    itemId = (s32)pouchHaveItem(i);
                    break;
                case 0xB:
                    if ((s16)badgeShop_get(bdsw + 0x19, (s16)(i + 0xF0)) -
                        badgeShop_ThrowCheck(i + 0xF0) > 0) {
                        itemId = i + 0xF0;
                    }
                    break;
                case 0xC:
                    itemId = (s32)pouchHaveBadge(i);
                    break;
                case 0xD:
                    if ((s16)badgeShop_get(bdsw + 0x32, (s16)(i + 0xF0)) > 0)
                        itemId = i + 0xF0;
                    break;
                case 0xE:
                    if ((s16)badgeShop_get(bdsw + 0x4B, (s16)(i + 1)) > 0)
                        itemId = i + 1;
                    break;
                case 0xF:
                    itemId = (s32)badge_bottakuru100_table[i];
                    break;
                case 0x10:
                    if ((s16)badgeShop_get(bdsw + 0xA0, (s16)(i + 1)) > 0)
                        itemId = i + 1;
                    break;
                }

                if (itemId != 0) {
                    if (menuState == 0xD &&
                        pouchGetStarPiece() <
                            (s32)*(s16*)(itemDataTable + itemId * 0x28 + 0x18)) {
                        iconFlags |= 0x20;
                        itemColor = 0xA0A0A0FF;
                    }

                    iconPos.x = 11.0f;
                    iconPos.y = scrollY + (f32)(0x38 - visible * 24);
                    iconPos.z = 0.0f;
                    iconDispGx(0.5f, &iconPos, iconFlags,
                               (s32)*(s16*)(itemDataTable + itemId * 0x28 + 0x20));

                    FontDrawStart();
                    FontDrawScale(0.8f);
                    FontDrawColor(&itemColor);
                    FontDrawString(36.0f,
                                   scrollY + (f32)(0x50 - visible * 24) - 2.0f,
                                   *(char**)(itemDataTable + itemId * 0x28));

                    if (menuState != 3) {
                        s32 price;

                        priceX = 0;
                        priceScale = 1.0f;

                        if (menuState == 0xE) {
                            price = ((u32)*(u16*)(itemDataTable + itemId * 0x28 + 0x14) * 12) / 10;
                            sprintf(priceText, "%2d", price);
                        } else if (menuState == 0xC || menuState == 2) {
                            price = (u32)*(u16*)(itemDataTable + itemId * 0x28 + 0x1A);
                            sprintf(priceText, "%2d", price);
                        } else if (menuState == 0xB) {
                            price = (u32)*(u16*)(itemDataTable + itemId * 0x28 + 0x14);
                            sprintf(priceText, "%2d", price);
                        } else if (menuState == 0xD) {
                            price = (u32)*(u16*)(itemDataTable + itemId * 0x28 + 0x18);
                            sprintf(priceText, "%2d", price);
                        } else if (menuState == 0x10) {
                            price = ((u32)*(u16*)(itemDataTable + itemId * 0x28 + 0x14) * 2 + 2) / 3;
                            sprintf(priceText, "%4d", price);
                            priceX = -20;
                            priceScale = 0.8f;
                        } else {
                            s32 mult = evtGetValue(0, -0x0A21F957) + 0xB;
                            price = ((u32)*(u16*)(itemDataTable + itemId * 0x28 + 0x14) * mult) / 10;
                            sprintf(priceText, "%3d", price);
                            priceX = -14;
                        }

                        FontDrawStart();
                        FontDrawScale(priceScale);
                        FontDrawColor(&itemColor);
                        FontDrawString((f32)(0xE2 + priceX),
                                       scrollY + (f32)(0x50 - visible * 24),
                                       priceText);
                    }

                    if ((f32)visible == selectedRow)
                        description = *(char**)(itemDataTable + itemId * 0x28 + 8);

                    visible++;
                }
            }

            GXSetScissor(oldX, oldY, oldW, oldH);

            if (description != 0) {
                windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                                      20.0f, 0, &white);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(description));
            }
            break;

        case 0x11:
        case 0x12:
            freeRows = 8 - *(s32*)((s32)work + 0x58);
            if (freeRows < 0)
                freeRows = 0;

            if (menuState == 0x11) {
                title = msgSearch("msg_window_title_5");
                limit = (s32)&_jdt[0x21];
            } else {
                title = msgSearch("msg_window_title_6");
                limit = (s32)&_jdt[0x2E];
            }

            listHeight = 220 - freeRows * 24;
            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f,
                                  (f32)listHeight, 20.0f, 0, &white);
            windowDispGX_Waku_col(51.0f, 114.0f, 150.0f, 28.0f,
                                  8.0f, 0, &white);

            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(71.0f, 111.0f, title);
            FontDrawEdgeOff();

            if (menuState == 0x11) {
                PSMTXTrans(listTrans, 244.0f, 82.0f, 0.0f);
                PSMTXScale(listScale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(listTrans, listScale, listTrans);
                iconDispGxCol(listTrans, 0x10, 0x213, &white);

                iconPos.x = 244.0f;
                iconPos.y = 86.0f;
                iconPos.z = 0.0f;
                iconDispGx(0.6f, &iconPos, 0x10, 0x193);
            }

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            visible = 0;
            description = *(char**)_jdt;
            for (i = 0; i < *(s32*)((s32)work + 0x58); i++) {
                itemId = *(s16*)(description + 0x0C);

                if ((s16)johoya_get(limit, (s16)itemId) == 0) {
                    sprintf(priceText, "%3d",
                            (s32)*(s16*)(description + 0x0A));
                    white = 0x000000FF;
                } else {
                    sprintf(priceText, "");
                    white = 0xA0A0A0FF;
                }

                FontDrawStart();

                title = *(char**)(description + 4);
                itemIndex = *(s16*)(description + 8);
                value = (u8)title[itemIndex];
                title[itemIndex] = 0;

                FontDrawColor(&white);
                FontDrawScale(0.8f);
                FontDrawString(1.0f,
                               scrollY + (f32)(0x50 - visible),
                               title);

                title[itemIndex] = (char)value;

                if (menuState == 0x11) {
                    FontDrawStart();
                    FontDrawColor(&white);
                    FontDrawString(221.0f,
                                   scrollY + (f32)(0x50 - visible),
                                   priceText);
                }

                visible += 0x18;
                description += 0x10;
            }

            GXSetScissor(oldX, oldY, oldW, oldH);
            break;

        case 5:
        case 6:
        case 7:
        {
            s32 partyIndex;
            s32 partyId;
            s32 techLevel;
            s32 selectableState;
            s32 row;
            s32 iconId;
            s32 iconFlags;
            u32 rowColor;
            char* partyName;
            char partnerValue[0x20];
            char helper[0x20];
            char direction[0x20];
            char partnerHelp[0x100];

            partnerHelp[0] = 0;
            freeRows = 8 - *(s32*)((s32)work + 0x58);
            if (freeRows < 0)
                freeRows = 0;

            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f,
                                  (f32)(220 - freeRows * 24),
                                  20.0f, 0, &white);
            windowDispGX_Waku_col(16.0f, 114.0f, 120.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(41.0f, 111.0f, msgSearch("msg_window_title_7"));
            FontDrawEdgeOff();

            windowDispGX_Waku_col(167.0f, 114.0f, 85.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_8"));
            FontDrawEdgeOff();

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            row = 0;
            partyIndex = 0;
            while ((partyId = party_id_table[partyIndex]) != 0) {
                s32 drawRow;

                drawRow = 0;
                selectableState = 0;
                iconFlags = 0x10;
                rowColor = 0xFFFFFFFF;

                if (menuState == 5 || menuState == 6) {
                    if (partyChkJoin(partyId) != 0) {
                        s32 special = evtGetValue(0, 0xF8406022);
                        techLevel = *(s16*)((s32)pouch + partyId * 0xE + 0xC);
                        drawRow = 1;

                        if (special == 0) {
                            if (techLevel < 1) {
                                selectableState = 0;
                            } else {
                                selectableState = 1;
                                iconFlags |= 0x20;
                                rowColor = 0xA0A0A0FF;
                            }
                        } else {
                            if (techLevel < 2) {
                                selectableState = 0;
                            } else {
                                selectableState = 2;
                                iconFlags |= 0x20;
                                rowColor = 0xA0A0A0FF;
                            }
                        }
                    }
                } else {
                    if (partyChkJoin(partyId) != 0 &&
                        *(s32*)((s32)work + 0x4C) != partyId) {
                        techLevel = *(s16*)((s32)pouch + partyId * 0xE + 0xC);
                        drawRow = 1;
                        if (techLevel < 1) {
                            selectableState = 1;
                            iconFlags |= 0x20;
                            rowColor = 0xA0A0A0FF;
                        } else {
                            selectableState = 0;
                        }
                    }
                }

                if (drawRow) {
                    iconId = party_icon_table[partyIndex];
                    if (iconId == 0x160) {
                        switch (pouchGetPartyColor(4)) {
                        case 0: iconId = 0x160; break;
                        case 1: iconId = 0x161; break;
                        case 2: iconId = 0x162; break;
                        case 3: iconId = 0x163; break;
                        case 4: iconId = 0x164; break;
                        case 5: iconId = 0x165; break;
                        default: iconId = 0x166; break;
                        }
                    }

                    iconPos.x = 11.0f;
                    iconPos.y = scrollY + (f32)(0x38 - row * 24);
                    iconPos.z = 0.0f;
                    iconDispGx(0.5f, &iconPos, iconFlags, iconId);

                    FontDrawStart();
                    FontDrawColor(&rowColor);
                    if (partyIndex == 3) {
                        partyName = pouchGetYoshiName();
                    } else {
                        partyName = msgSearch(party_labelname_table[partyIndex]);
                    }
                    FontDrawString(36.0f,
                                   scrollY + (f32)(0x50 - row * 24),
                                   partyName);

                    techLevel = *(s16*)((s32)pouch + partyId * 0xE + 0xC);
                    sprintf(partnerValue, "%d", techLevel);
                    FontDrawStart();
                    FontDrawString(236.0f,
                                   scrollY + (f32)(0x50 - row * 24),
                                   partnerValue);

                    if ((f32)row == selectedRow) {
                        if (selectableState == 0)
                            sprintf(helper, "help");
                        else
                            sprintf(helper, "helperr");

                        if (menuState == 5 || menuState == 6)
                            sprintf(direction, "lankup");
                        else
                            sprintf(direction, "lankdown");

                        sprintf(partnerHelp, "%s_%s_lank%d%s",
                                helper, direction, techLevel + 1,
                                party_labelname_table[partyIndex]);
                    }

                    row++;
                }

                partyIndex++;
            }

            GXSetScissor(oldX, oldY, oldW, oldH);

            if (partnerHelp[0] != 0) {
                windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                                      20.0f, 0, &white);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(partnerHelp));
            }
            break;
        }

        case 8:
        {
            s32** pointTable;
            s32 statusCount;
            s32 statusIndex;
            s32 row;
            s32 other;
            s32 disabledCount;
            s32 blocked;
            s32 iconFlags;
            s32 currentValue;
            s32 delta;
            s32 previewValue;
            u32 rowColor;
            char statText[0x40];
            char statHelp[0x100];
            char helper[0x20];
            Vec3 previewPos;

            pointTable = (s32**)&mario_status_point_table;
            statHelp[0] = 0;
            statusCount = *(s32*)((s32)work + 0x58);

            freeRows = 8 - *(s32*)((s32)work + 0x58);
            if (freeRows < 0)
                freeRows = 0;

            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f,
                                  (f32)(220 - freeRows * 24),
                                  20.0f, 0, &white);
            windowDispGX_Waku_col(16.0f, 114.0f, 120.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();

            windowDispGX_Waku_col(167.0f, 114.0f, 85.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            row = 0;
            for (statusIndex = 0; statusIndex < statusCount; statusIndex++) {
                s16* pointPtr;

                pointPtr = (s16*)pointTable[statusIndex];
                currentValue = *pointPtr;
                delta = mario_status_henka_table[statusIndex];
                disabledCount = 0;

                for (other = 0; other < *(s32*)((s32)work + 0x58); other++) {
                    if ((f32)other != selectedRow) {
                        s16* otherPtr = (s16*)pointTable[other];
                        if ((s32)*otherPtr - mario_status_henka_table[other] < 1)
                            disabledCount++;
                    }
                }

                blocked = disabledCount != 0;
                if (blocked) {
                    iconFlags = 0x30;
                    rowColor = 0xA0A0A0FF;
                } else {
                    iconFlags = 0x10;
                    rowColor = 0xFFFFFFFF;
                }

                iconPos.x = 11.0f;
                iconPos.y = scrollY + (f32)(0x38 - row * 24);
                iconPos.z = 0.0f;
                iconDispGx(0.5f, &iconPos, iconFlags,
                           mario_status_icon_table[statusIndex]);

                sprintf(statText, "%s%3d",
                        mario_status_name_table[statusIndex],
                        currentValue);
                FontDrawStart();
                FontDrawColor(&rowColor);
                FontDrawString(36.0f,
                               scrollY + (f32)(0x50 - row * 24),
                               statText);

                if ((f32)row == selectedRow) {
                    if (blocked)
                        sprintf(helper, "helperr");
                    else
                        sprintf(helper, "help");

                    sprintf(statHelp, "%s_levelup%s",
                            helper,
                            mario_status_labelname_table[statusIndex]);
                    previewValue = currentValue + delta * 2;

                    if (previewValue < 1)
                        sprintf(statText, "--");
                    else
                        sprintf(statText, "%3d", previewValue);

                    FontDrawStart();
                    FontDrawString(206.0f,
                                   scrollY + (f32)(0x50 - row * 24),
                                   statText);

                    previewPos.x = 187.0f;
                    previewPos.y = scrollY + (f32)(0x38 - row * 24);
                    previewPos.z = 0.0f;

                    itemIndex = 0x1BD;
                    if (previewValue < 1)
                        itemIndex = 0x16A;

                    iconDispGx(0.5f, &previewPos, iconFlags, itemIndex);
                } else {
                    previewValue = currentValue - delta;
                    if (previewValue < 1)
                        sprintf(statText, "--");
                    else
                        sprintf(statText, "%3d", previewValue);

                    FontDrawStart();
                    FontDrawString(206.0f,
                                   scrollY + (f32)(0x50 - row * 24),
                                   statText);
                }

                row++;
            }

            GXSetScissor(oldX, oldY, oldW, oldH);

            if (statHelp[0] != 0) {
                windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                                      20.0f, 0, &white);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(statHelp));
            }
            break;
        }

        case 9:
        {
            s32** pointTable;
            s32 statusCount;
            s32 statusIndex;
            s32 row;
            s32 other;
            s32 disabledCount;
            s32 blocked;
            s32 iconFlags;
            s32 currentValue;
            s32 delta;
            s32 previewValue;
            u32 rowColor;
            char statText[0x40];
            char statHelp[0x100];
            char helper[0x20];
            Vec3 previewPos;

            pointTable = (s32**)&mario_status_point_table;
            statHelp[0] = 0;
            statusCount = *(s32*)((s32)work + 0x58);

            freeRows = 8 - *(s32*)((s32)work + 0x58);
            if (freeRows < 0)
                freeRows = 0;

            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f,
                                  (f32)(220 - freeRows * 24),
                                  20.0f, 0, &white);
            windowDispGX_Waku_col(16.0f, 114.0f, 120.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();

            windowDispGX_Waku_col(167.0f, 114.0f, 85.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            row = 0;
            for (statusIndex = 0; statusIndex < statusCount; statusIndex++) {
                s16* pointPtr;

                pointPtr = (s16*)pointTable[statusIndex];
                currentValue = *pointPtr;
                delta = mario_status_henka_table[statusIndex];
                disabledCount = 0;

                for (other = 0; other < *(s32*)((s32)work + 0x58); other++) {
                    if ((f32)other != selectedRow) {
                        s16* otherPtr = (s16*)pointTable[other];
                        if ((s32)*otherPtr - mario_status_henka_table[other] < 1)
                            disabledCount++;
                    }
                }

                blocked = disabledCount > 1;
                if (blocked) {
                    iconFlags = 0x30;
                    rowColor = 0xA0A0A0FF;
                } else {
                    iconFlags = 0x10;
                    rowColor = 0xFFFFFFFF;
                }

                iconPos.x = 11.0f;
                iconPos.y = scrollY + (f32)(0x38 - row * 24);
                iconPos.z = 0.0f;
                iconDispGx(0.5f, &iconPos, iconFlags,
                           mario_status_icon_table[statusIndex]);

                sprintf(statText, "%s%3d",
                        mario_status_name_table[statusIndex],
                        currentValue);
                FontDrawStart();
                FontDrawColor(&rowColor);
                FontDrawString(36.0f,
                               scrollY + (f32)(0x50 - row * 24),
                               statText);

                if ((f32)row == selectedRow) {
                    if (blocked)
                        sprintf(helper, "helperr");
                    else
                        sprintf(helper, "help");

                    sprintf(statHelp, "%s_levelup%s",
                            helper,
                            mario_status_labelname_table[statusIndex]);
                    previewValue = currentValue + delta;

                    if (previewValue < 1)
                        sprintf(statText, "--");
                    else
                        sprintf(statText, "%3d", previewValue);

                    FontDrawStart();
                    FontDrawString(206.0f,
                                   scrollY + (f32)(0x50 - row * 24),
                                   statText);

                    previewPos.x = 187.0f;
                    previewPos.y = scrollY + (f32)(0x38 - row * 24);
                    previewPos.z = 0.0f;

                    itemIndex = 0x1BD;
                    if (previewValue < 1)
                        itemIndex = 0x16A;

                    iconDispGx(0.5f, &previewPos, iconFlags, itemIndex);
                }

                row++;
            }

            GXSetScissor(oldX, oldY, oldW, oldH);

            if (statHelp[0] != 0) {
                windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                                      20.0f, 0, &white);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(statHelp));
            }
            break;
        }

        case 10:
        {
            s32** pointTable;
            s32 statusIndex;
            s32 row;
            s32 blocked;
            s32 iconFlags;
            s32 currentValue;
            s32 delta;
            s32 previewValue;
            u32 rowColor;
            char statText[0x40];
            char statHelp[0x100];
            char helper[0x20];
            Vec3 previewPos;

            pointTable = (s32**)&mario_status_point_table;
            statHelp[0] = 0;

            freeRows = 8 - *(s32*)((s32)work + 0x58);
            if (freeRows < 0)
                freeRows = 0;

            windowDispGX_Waku_col(-24.0f, 100.0f, 300.0f,
                                  (f32)(220 - freeRows * 24),
                                  20.0f, 0, &white);
            windowDispGX_Waku_col(16.0f, 114.0f, 120.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();

            windowDispGX_Waku_col(167.0f, 114.0f, 85.0f, 28.0f,
                                  8.0f, 0, &white);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor(&white);
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();

            GXGetScissor(&oldX, &oldY, &oldW, &oldH);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - freeRows * 24);

            row = 0;
            for (statusIndex = 0; statusIndex < 3; statusIndex++) {
                s16* pointPtr;

                if (statusIndex == *(s32*)((s32)work + 0x4C))
                    continue;

                pointPtr = (s16*)pointTable[statusIndex];
                currentValue = *pointPtr;
                delta = mario_status_henka_table[statusIndex];
                blocked = currentValue - delta < 1;

                if (blocked) {
                    iconFlags = 0x30;
                    rowColor = 0xA0A0A0FF;
                } else {
                    iconFlags = 0x10;
                    rowColor = 0xFFFFFFFF;
                }

                iconPos.x = 11.0f;
                iconPos.y = scrollY + (f32)(0x38 - row * 24);
                iconPos.z = 0.0f;
                iconDispGx(0.5f, &iconPos, iconFlags,
                           mario_status_icon_table[statusIndex]);

                sprintf(statText, "%s%3d",
                        mario_status_name_table[statusIndex],
                        currentValue);
                FontDrawStart();
                FontDrawColor(&rowColor);
                FontDrawString(36.0f,
                               scrollY + (f32)(0x50 - row * 24),
                               statText);

                if ((f32)row == selectedRow) {
                    if (blocked)
                        sprintf(helper, "helperr");
                    else
                        sprintf(helper, "help");

                    sprintf(statHelp, "%s_leveldown%s",
                            helper,
                            mario_status_labelname_table[statusIndex]);
                    previewValue = currentValue - delta;

                    if (previewValue < 1)
                        sprintf(statText, "--");
                    else
                        sprintf(statText, "%3d", previewValue);

                    FontDrawStart();
                    FontDrawString(206.0f,
                                   scrollY + (f32)(0x50 - row * 24),
                                   statText);

                    previewPos.x = 187.0f;
                    previewPos.y = scrollY + (f32)(0x38 - row * 24);
                    previewPos.z = 0.0f;

                    itemIndex = 0x1BE;
                    if (previewValue < 1)
                        itemIndex = 0x16A;

                    iconDispGx(0.5f, &previewPos, iconFlags, itemIndex);
                }

                row++;
            }

            GXSetScissor(oldX, oldY, oldW, oldH);

            if (statHelp[0] != 0) {
                windowDispGX_Waku_col(-240.0f, -130.0f, 480.0f, 70.0f,
                                      20.0f, 0, &white);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(statHelp));
            }
            break;
        }

        default:
            break;
        }
    }

    {
        s32 menuState;
        char helpText[0x88];
        char* secondLine;
        char* p;
        char* msg;
        s32 lineCount;
        s32 firstWidth;
        s32 secondWidth;
        s32 maxWidth;
        s32 xOffset;
        u32 helpColor;

        menuState = *(s32*)((s32)work + 0x50);

        if (menuState == 0 || menuState == 1) {
            iconFlagOn(str_yubicursor_2_802f7124, 2);
        } else {
            secondLine = 0;
            helpText[0] = 0;

            switch (menuState) {
            case 2:
            case 0xC:
                msg = msgSearch("msg_window_select_2");
                strcpy(helpText, msg);
                break;
            case 3:
                msg = msgSearch("msg_window_select_3");
                strcpy(helpText, msg);
                break;
            case 4:
                msg = msgSearch("msg_window_select_4");
                strcpy(helpText, msg);
                break;
            case 5:
                msg = msgSearch("msg_window_select_11");
                strcpy(helpText, msg);
                break;
            case 6:
                msg = msgSearch("msg_window_select_11");
                strcpy(helpText, msg);
                break;
            case 7:
                msg = msgSearch("msg_window_select_12");
                strcpy(helpText, msg);
                break;
            case 8:
            case 9:
                msg = msgSearch("msg_window_select_9");
                strcpy(helpText, msg);
                break;
            case 10:
                msg = msgSearch("msg_window_select_9_");
                strcpy(helpText, msg);
                break;
            case 0xB:
            case 0xE:
            case 0xF:
                msg = msgSearch("msg_window_select_6");
                strcpy(helpText, msg);
                break;
            case 0xD:
            case 0x10:
                msg = msgSearch("msg_window_select_10");
                strcpy(helpText, msg);
                break;
            case 0x11:
                msg = msgSearch("msg_window_select_8");
                strcpy(helpText, msg);
                break;
            case 0x12:
                msg = msgSearch("msg_window_select_7");
                strcpy(helpText, msg);
                break;
            default:
                helpText[0] = 0;
                break;
            }
            lineCount = 1;
            p = helpText;
            while (*p != 0) {
                if (*p == '\n') {
                    *p = 0;
                    secondLine = p + 1;
                    lineCount = 2;
                    break;
                }
                p++;
            }

            helpColor = 0xFFFFFFFF;
            windowDispGX_Waku_col(-280.0f, 100.0f, 240.0f,
                                  (f32)(45 + (lineCount - 1) * 24),
                                  20.0f, 0, &helpColor);
            FontDrawStart();

            firstWidth = (s32)(FontGetMessageWidth(helpText) & 0xFFFF);
            if (secondLine != 0)
                secondWidth = (s32)(FontGetMessageWidth(secondLine) & 0xFFFF);
            else
                secondWidth = 0;

            maxWidth = firstWidth;
            if (secondWidth > maxWidth)
                maxWidth = secondWidth;

            if (maxWidth < 0xE5) {
                xOffset = (0xE4 - maxWidth) / 2;
            } else {
                FontDrawScale(228.0f / (f32)maxWidth);
                xOffset = 0;
            }

            FontDrawString((f32)(xOffset - 0x112), 90.0f, helpText);
            if (secondLine != 0)
                FontDrawString((f32)(xOffset - 0x112), 66.0f, secondLine);

            iconFlagOff(str_yubicursor_2_802f7124, 2);
            iconSetPos(*(f32*)((s32)work + 0x3C),
                       *(f32*)((s32)work + 0x40),
                       0.0f,
                       str_yubicursor_2_802f7124);
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 set_new_goods_list(void* pEvt) {
    s32* args;
    void* work;
    s32 item;
    void* itemData;
    char name[0x20];

    args = *(s32**)((s32)pEvt + 0x18);
    work = (void*)evtGetValue(pEvt, args[0]);
    item = evtGetValue(pEvt, args[1]);
    *(s32*)(*(s32*)((s32)work + 0x8) + (*(s32*)((s32)work + 0x24) << 2)) = item;
    if (*(s32*)((s32)work + 0x24) < *(s32*)((s32)work + 0x18)) {
        sprintf(name, &str_uwnPCTd_80423e18, *(s32*)((s32)work + 0x24));
        if (item != 0) {
            itemData = itemDataTable;
            itemData = (void*)((s32)itemData + item * 0x28);
            iconChange(name, *(u16*)((s32)itemData + 0x20));
        } else {
            iconDelete(name);
        }
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 unitwin_get_work_ptr(void* pEvt) {

    evtSetValue(pEvt, **(s32**)((s32)pEvt + 0x18), (s32)_wp);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwin_getpartylank(void* pEvt) {
    s32* args;
    void* pouch;
    void* partyData;
    s32 party;

    args = *(s32**)((s32)pEvt + 0x18);
    pouch = pouchGetPtr();
    party = evtGetValue(pEvt, args[0]);
    partyData = (void*)((s32)pouch + party * 0xE);
    evtSetValue(pEvt, args[1], *(s16*)((s32)partyData + 0xC));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwin_setpartylank(void* pEvt) {
    s32 offset;
    s32* args;
    void* pouch;
    void* partyData;
    s32 rank;

    args = *(s32**)((s32)pEvt + 0x18);
    pouch = pouchGetPtr();
    offset = evtGetValue(pEvt, args[0]) * 0xE;
    rank = evtGetValue(pEvt, args[1]);
    partyData = (void*)((s32)pouch + offset);
    *(s16*)((s32)partyData + 0xC) = rank;
    *(s16*)((s32)partyData + 0xA) = rank;
    *(s16*)((s32)partyData + 0x8) = rank;
    pouchRevisePartyParam();
    partyData = (void*)((s32)pouch + offset);
    *(s16*)((s32)partyData + 0x6) = *(s16*)((s32)partyData + 0x2);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwin_setmariolevel(void* pEvt) {
    s32* args;
    void* pouch;
    s32 i;
    s32 changed;

    args = *(s32**)((s32)pEvt + 0x18);
    pouch = pouchGetPtr();
    *(s16*)((s32)pouch + 0x8E) = evtGetValue(pEvt, args[0]);
    *(s16*)((s32)pouch + 0x90) = evtGetValue(pEvt, args[1]);
    *(s16*)((s32)pouch + 0x94) = evtGetValue(pEvt, args[2]);
    pouchReviseMarioParam();
    *(s16*)((s32)pouch + 0x70) = *(s16*)((s32)pouch + 0x72);
    *(s16*)((s32)pouch + 0x74) = *(s16*)((s32)pouch + 0x76);
    if (*(s16*)((s32)pouch + 0x92) < 0) {
        for (i = 0; i < 200; i++) {
            if (pouchEquipCheckBadgeIndex(i) != 0) {
                pouchUnEquipBadgeIndex(i);
            }
        }
        pouchReviseMarioParam();
        changed = 1;
    } else {
        changed = 0;
    }
    evtSetValue(pEvt, args[3], changed);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

