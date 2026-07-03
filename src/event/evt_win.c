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

void unk_8017b330(void) {
    extern void winMgrHelpMain();

    winMgrHelpMain();
}


void evt_unitwin_disp_func(s32 param_1, void* param_2) {
    ;
}


s32 evt_unitwin_selltable_setup(int pEvt) {
    extern void mapObjGetPos(void* obj, void* pos);
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
s32 evt_unitwin_free(void* pEvt, s32 force) {
    s32 i;
    void* work;
    s32 offset;
    void* winPtr;
    char name[0x20];

    work = _wp;
    if (work == NULL) {
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
    _wp = NULL;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 set_new_goods_list(void* pEvt) {
    s32* args;
    void* work;
    s32 item;
    char name[0x20];

    args = *(s32**)((s32)pEvt + 0x18);
    work = (void*)evtGetValue(pEvt, args[0]);
    item = evtGetValue(pEvt, args[1]);
    *(s32*)(*(s32*)((s32)work + 0x8) + (*(s32*)((s32)work + 0x24) << 2)) = item;
    if (*(s32*)((s32)work + 0x24) < *(s32*)((s32)work + 0x18)) {
        sprintf(name, &str_uwnPCTd_80423e18, *(s32*)((s32)work + 0x24));
        if (item != 0) {
            iconChange(name, *(u16*)((s32)itemDataTable + item * 0x28 + 0x20));
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwincoin_disp_onoff(void* pEvt) {
    extern s32 evtGetValue(void* entry, s32 index);
    extern void* pouchGetPtr(void);
    extern s32 pouchGetCoin(void);
    extern void* _wp;
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unitwin_getpartylank(void* pEvt) {
    extern void* pouchGetPtr(void);
    extern s32 evtGetValue(void* entry, s32 index);
    extern s32 evtSetValue(void* entry, s32 index, s32 value);
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


u32 unitwincoin_wait(void) {
    extern s32 pouchGetCoin(void);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 unitwin_get_work_ptr(void* pEvt) {
    extern s32 evtSetValue(void* entry, s32 index, s32 value);
    extern void* _wp;

    evtSetValue(pEvt, **(s32**)((s32)pEvt + 0x18), (s32)_wp);
    return 2;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_8017b2b0(void* pWin) {
    extern s32 winMgrAction(s32 winId);
    extern char* msgSearch(s32 msgId);
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


/* CHATGPT FALLBACK MISSING STUBS: main/event/evt_win 20260624_191429 */

/* fallback stub-fill: map=unk_8017a844 addr=0x8017a844 size=0x00000360 */
int unk_8017a844() {
    return 0;
}

/* fallback stub-fill: map=unk_8017b0c4 addr=0x8017b0c4 size=0x000001ec */
int unk_8017b0c4(u32* pWin) {
    extern void PSMTXTrans(void* mtx, f64 x, f64 y, f64 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void iconDispGx2(void* mtx, s32 mode, u16 iconId);
    extern void iconNumberDispGx(void* mtx, s32 value, s32 unk, void* color);
    extern void* gp;
    extern f32 float_10_80423f44;
    extern f32 float_30_80423f48;
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

/* fallback stub-fill: map=unk_8017b350 addr=0x8017b350 size=0x00000260 */
int unk_8017b350(void) {
    extern char* msgSearch(s32 msgId);
    extern s32 winMgrAction(s32 winId);
    extern void FontDrawStart(void);
    extern void FontDrawEdge(void);
    extern void FontDrawColor(void* color);
    extern s32 FontGetMessageWidth(char* msg);
    extern void FontDrawString(f64 x, f64 y, char* msg);
    extern void FontDrawEdgeOff(void);
    extern void mapObjGetPos(void* obj, void* pos);
    extern void* camGetPtr(s32 camId);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern void PSMTXTrans(void* mtx, f64 x, f64 y, f64 z);
    extern s32 strlen(char* str);
    extern void iconNumberDispGx(void* mtx, s32 value, s32 unk, void* color);
    extern void* gp;
    extern u32 dat_80423d0c;
    extern u32 dat_80423d10;
    extern f32 float_108_80423e34;
    extern f32 float_30_80423f48;
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

