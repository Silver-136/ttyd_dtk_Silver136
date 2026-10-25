#include "evt/evt_cmd.h"
#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/hitdrv.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "data/item_data.h"
#include "driver/windowdrv.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_johoya.h"
#include "evt/evt_lottery.h"
#include "pmario_sound.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <stdio.h>
#include <string.h>
#include "icon_ids.h"
#include "mario/mario_party.h"
#include "driver/itemdrv.h"
#include "statuswindow.h"
#include "win/win_main.h"
#include "data_tables.h"

extern ItemData itemDataTable[];

void FontDrawString(f32 x, f32 y, const char* str);


typedef struct UnitWinWork {
    u16 flags;             // 0x0
    u16 pad2;              // 0x2
    UnitWinSell* entries; // 0x4
    s32* goods;            // 0x8
    u16 itemFlags[6];      // 0xC
    s32 goodsMax;          // 0x18
    EventEntry* evt;       // 0x1C
    EventEntry* childEvt;  // 0x20
    s32 goodsIdx;          // 0x24
    s32 timer;             // 0x28
    s32 coin;              // 0x2C
    s32 coinDisp;          // 0x30
    f32 cursor;            // 0x34
    u8 pad38[4];           // 0x38
    f32 cursorX;           // 0x3C
    f32 cursorY;           // 0x40
    f32 scroll;            // 0x44
    u8 pad48[4];           // 0x48
    s32 skipEntry;        // 0x4C, never written (0 after the memset); both lists skip the entry equal to it
    s32 mode;              // 0x50
    u16 flags2;            // 0x54
    u8 pad56[2];           // 0x56
    s32 count;             // 0x58
    s32 coinTick;          // 0x5C
    s32 winId[3];          // 0x60
    u8 pad6C[0xE0 - 0x6C]; // 0x6C
    s32 selected;          // 0xE0
    s32 bought;            // 0xE4
} UnitWinWork;

static s16* mario_status_point_table[3];
s32 unitwin_coin_se_wait;
static UnitWinWork* _wp;

USER_FUNC(evt_unitwin_main_func);
USER_FUNC(evt_unitwin_select_func);
void evt_unitwin_disp_func(CameraId cameraId, void* param);

void unitwin_help_main(WinMgrEntry* entry);
void unitwin_help_disp(WinMgrEntry* entry);
void unitwin_point_disp(WinMgrEntry* entry);
void unitwin_name_disp(WinMgrEntry* entry);

WinMgrDesc unitwin_desc[3] = {
    {0, 0, 8, -150, 120, 300, 50, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, unitwin_name_disp},
    {0, 0, 8, -240, -130, 500, 80, {0xF0, 0xF0, 0xF0, 0xFF}, unitwin_help_main, unitwin_help_disp},
    {2, 0, 8, -250, -130, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, unitwin_point_disp},
};

static s32 party_id_table[8] = {1, 2, 5, 4, 6, 3, 7, 0};
static s32 party_icon_table[7] = {ICON_155, ICON_156, ICON_15F, ICON_158, ICON_160, ICON_157, ICON_161};
static const char* party_labelname_table[7] = {("_kur"), ("_nok"), ("_win"), ("_yos"), ("_viv"), ("_bom"), ("_chu")};
static s32 mario_status_icon_table[3] = {ICON_19F, ICON_1A0, ICON_1A1};
static const char* mario_status_labelname_table[3] = {("_hp"), ("_fp"), ("_bp")};
static const char* mario_status_name_table[3] = {("\202g\202o"), ("\202e\202o"), ("\202a\202o")};
static s32 mario_status_henka_table[3] = {5, 5, 3};

static EVT_BEGIN(main_evt)
    CALL(evt_unitwin_main_func)
    RETURN
EVT_END()

EVT_BEGIN(select_evt)
    CALL(evt_unitwin_select_func)
    RETURN
EVT_END()

void unitwin_name_disp(WinMgrEntry* entry) {
    UnitWinWork* wp = _wp;
    const char* msg = msgSearch(itemDataTable[wp->goods[wp->goodsIdx]].name);
    s32 i;
    if (!winMgrAction(wp->winId[0])) {
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawString(-FontGetMessageWidth(msg) / 2, 108.0f, msg);
        FontDrawEdgeOff();
    }
    for (i = 0; i < wp->goodsMax; i++) {
        if (!(wp->itemFlags[i] & 1)) {
            Mtx mtx;
            char buf[64];
            Vec pos;
            mapObjGetPos(wp->entries[i].mapObj, &pos);
            pos.y += 30.0f;
            PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &pos, &pos);
            PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &pos, &pos);
            pos.x *= gp->fbWidth / 2.0f;
            pos.y *= gp->efbHeight / 2.0f;
            sprintf(buf, "%d", itemDataTable[wp->goods[i]].discount_price);
            PSMTXTrans(mtx, 12.0f * (strlen(buf) - 1) + pos.x, pos.y, pos.z);
            iconNumberDispGx(mtx, itemDataTable[wp->goods[i]].discount_price, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }
}

// never called: the linker strips it, its "%s_%04d" string stays in this object's .rodata (name descriptive)
static void unitwin_fmt(char* buf, const char* name, s32 n) {
    sprintf(buf, "%s_%04d", name, n);
}

void unitwin_help_main(WinMgrEntry* entry) {
    winMgrHelpMain(entry);
}

void unitwin_help_disp(WinMgrEntry* entry) {
    UnitWinWork* wp = _wp;
    s32 idx = wp->goodsIdx * sizeof(s32); // FAKE: byte offset of the goods slot computed before winMgrAction (slwi ahead of the call, lwzx after), as in the target; goods[wp->goodsIdx] and an index local both scale after the call
    s32* goods = wp->goods;
    if (!winMgrAction(wp->winId[1])) {
        entry->msg = msgSearch(itemDataTable[*(s32*)((u8*)goods + idx)].description);
        winMgrHelpDraw(entry);
    }
}

void unitwin_point_disp(WinMgrEntry* entry) {
    UnitWinWork* wp = _wp;
    PouchData* pouch = pouchGetPtr();
    Mtx trans;
    Mtx scale;
    s32 d;
    if (!(entry->flags & 4)) {
        PSMTXTrans(trans, 10.0f + (30.0f + entry->x), entry->y - 36.0f, 0.0f);
        if ((u32)gp->language == 0) {
            PSMTXScale(scale, 1.1f, 1.1f, 1.1f);
            PSMTXConcat(trans, scale, trans);
            iconDispGx2(trans, 0x10, VERSION_PICK(0x1F1, 0x226));
        } else {
            PSMTXScale(scale, 0.8f, 1.1f, 1.1f);
            PSMTXConcat(trans, scale, trans);
            iconDispGx2(trans, 0x10, VERSION_PICK(0x20A, 0x227));
        }
        PSMTXTrans(trans, 124.0f + entry->x, entry->y - 38.0f, 0.0f);
        {
            GXColor c = {0xFF, 0xFF, 0xFF, 0xFF};
            iconNumberDispGx(trans, wp->coinDisp, 0, c);
        }
        d = pouch->mShopPoints - wp->coinDisp;
        if (d) {
            d /= 10;
            if (!d) {
                d = pouch->mShopPoints > wp->coinDisp ? 1 : -1;
            }
            wp->coinDisp += d;
        }
    }
}

USER_FUNC(evt_unitwin_alloc) {
    s32 arg = evtGetValue(event, event->args[0]);
    UnitWinWork* wp = _mapAlloc(sizeof(UnitWinWork));
    memset(wp, 0, sizeof(UnitWinWork));
    wp->mode = 0;
    _wp = wp;
    if (arg) {
        wp->flags |= 0x80;
    } else {
        wp->flags &= ~0x80;
    }
    iconEntry2D("yubicursor_2", ICON_1F0);
    iconFlagOn("yubicursor_2", 2);
    iconFlagOn("yubicursor_2", 4);
    if (!(wp->flags & 0x80)) {
        wp->evt = evtEntry(main_evt, 0, 0);
        wp->evt->lwData[0] = (EvtWord)wp;
    }
    mario_status_point_table[0] = &pouchGetPtr()->baseMaxHP;
    mario_status_point_table[1] = &pouchGetPtr()->baseMaxFP;
    mario_status_point_table[2] = &pouchGetPtr()->maximumBP;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_unitwin_free) {
    UnitWinWork* wp = _wp;
    s32 i;
    char name[32];
    if (wp == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        wp->flags2 |= 1;
    }
    if (!(wp->flags2 & 2)) {
        return EVT_RETURN_BLOCK;
    }
    for (i = 0; i < wp->goodsMax; i++) {
        mapObjFlagOff(wp->entries[i].mapObj, 0x4000);
        sprintf(name, "uwn%d", i);
        iconDelete(name);
    }
    if (wp->flags & 0x80) {
        for (i = 0; i < 3; i++) {
            winMgrDelete(wp->winId[i]);
        }
    }
    iconDelete("yubicursor_2");
    _mapFree(_wp);
    _wp = NULL;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_unitwin_selltable_setup) {
    s32 i;
    EvtWord* args = event->args;
    UnitWinSell* tbl = (UnitWinSell*)evtGetValue(event, args[0]);
    s32* goods = (s32*)evtGetValue(event, args[1]);
    UnitWinWork* wp = _wp;
    char name[32];
    Vec pos;
    wp->entries = tbl;
    wp->goods = goods;
    wp->goodsMax = 0;
    for (i = 0; wp->entries[i].mapObj != NULL; i++) {
        wp->goodsMax++;
        mapObjGetPos(wp->entries[i].mapObj, &pos);
        mapObjFlagOn(wp->entries[i].mapObj, 0x4000);
        if (wp->goods[i] != 0) {
            sprintf(name, "uwn%d", i);
            iconEntry(name, itemDataTable[wp->goods[i]].icon_id);
            iconSetPos(name, pos.x, pos.y + 6.0f, pos.z);
            iconFlagOn(name, 0x40);
        } else {
            wp->itemFlags[i] |= 1;
        }
    }
    wp->evt = evtEntryType(select_evt, 0, 0x20, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
    for (i = 0; i < 3; i++) {
        wp->winId[i] = winMgrEntry(&unitwin_desc[i]);
    }
    wp->selected = -1;
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwincoin_disp_onoff) {
    s32 on = evtGetValue(event, event->args[0]);
    UnitWinWork* wp;
    pouchGetPtr();
    wp = _wp;
    wp->coin = pouchGetCoin();
    wp->coinTick = 0;
    if (on) {
        wp->flags |= 0x1000;
    } else {
        wp->flags &= ~0x1000;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwincoin_wait) {
    UnitWinWork* wp = _wp;
    pouchGetPtr();
    if (wp->coin != pouchGetCoin()) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwin_lottery_disp_onoff) {
    s32 on = evtGetValue(event, event->args[0]);
    UnitWinWork* wp = _wp;
    if (on) {
        wp->flags |= 0x100;
    } else {
        wp->flags &= ~0x100;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwin_set_bought) {
    EvtWord* args = event->args;
    UnitWinWork* wp = (UnitWinWork*)evtGetValue(event, args[0]);
    wp->bought = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

#define UNITWIN_CLOSE(wp)                \
    if ((wp)->selected != -1) {          \
        (wp)->selected = -1;             \
        winMgrClose((wp)->winId[0]);     \
        winMgrClose((wp)->winId[1]);     \
    }

USER_FUNC(evt_unitwin_select_func) {
    UnitWinWork* wp = (UnitWinWork*)event->lwData[0];
    s32 found = FALSE;
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    HitEntry* hit = mario->hitobjStandOn;
    u32 btn;
    s32 i;
    char name[32];
    MapObject* obj;
    if (wp->flags2 & 1) {
        wp->flags2 |= 2;
        return EVT_RETURN_DONE;
    }
    wp->flags &= ~1;
    btn = keyGetButtonTrg(0);
    if ((marioChkCtrl() ? FALSE : TRUE) || (marioChkKey() ? FALSE : TRUE)) {
        btn = 0;
    }
    if (party != NULL && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        UNITWIN_CLOSE(wp);
        return EVT_RETURN_BLOCK;
    }
    if (winGhostDiaryChk() || N_itemPickUpFromFieldCheck()) {
        UNITWIN_CLOSE(wp);
        return EVT_RETURN_BLOCK;
    }
    if (!(wp->flags & 2)) {
        if (hit != NULL) {
            for (i = 0; i < wp->goodsMax; i++) {
                const char* hitName = hitGetName(hit);
                if (!strcmp(wp->entries[i].hitName, hitName)) {
                    break;
                }
            }
            if (!(wp->itemFlags[i] & 1) && i < wp->goodsMax) {
                if (wp->selected != i) {
                    if (wp->selected == -1) {
                        winMgrOpen(wp->winId[0]);
                        winMgrOpen(wp->winId[1]);
                    }
                    wp->selected = i;
                    wp->goodsIdx = i;
                    winMgrHelpInit(winMgrGetPtr(wp->winId[1]));
                }
                found = TRUE;
            }
        }
        if (!found) {
            UNITWIN_CLOSE(wp);
        }
        if (wp->selected != -1) {
            wp->flags |= 1;
            if (btn & 0x100) {
                marioKeyOff();
                wp->childEvt = evtEntry(wp->entries[wp->goodsIdx].evt, 0, 0);
                wp->childEvt->lwData[0] = (EvtWord)wp;
                wp->flags |= 2;
                winMgrClose(wp->winId[0]);
                winMgrClose(wp->winId[1]);
                wp->selected = -1;
                wp->bought = 0;
            }
        }
    } else if (!evtCheckID(wp->childEvt->eventId)) {
        wp->flags &= ~2;
        marioKeyOn();
        if (wp->bought) {
            wp->itemFlags[wp->goodsIdx] |= 1;
        }
    }
    for (i = 0; i < wp->goodsMax; i++) {
        obj = mapGetMapObj(wp->entries[i].mapObj);
        if (obj != NULL) {
            sprintf(name, "uwn%d", i);
            if (wp->itemFlags[i] & 1) {
                iconFlagOn(name, 2);
            } else if (obj->flags & 1) {
                iconFlagOn(name, 2);
            } else {
                iconFlagOff(name, 2);
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_unitwin_main_func) {
    UnitWinWork* wp = (UnitWinWork*)event->lwData[0];
    if (wp->flags2 & 1) {
        wp->flags2 |= 2;
        return EVT_RETURN_DONE;
    }
    if (wp->timer > 0) {
        wp->timer--;
    } else {
        wp->flags &= ~1;
    }
    dispEntry(CAMERA_2D, 1000.0f, 0, evt_unitwin_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

static inline s32 unitwin_party_icon(s32 icon) {
    if (icon == ICON_158) {
        switch (pouchGetPartyColor(PARTNER_YOSHI)) {
            case 0:
                icon = ICON_158;
                break;
            case 1:
                icon = ICON_159;
                break;
            case 2:
                icon = ICON_15A;
                break;
            case 3:
                icon = ICON_15B;
                break;
            case 4:
                icon = ICON_15C;
                break;
            case 5:
                icon = ICON_15D;
                break;
            case 6:
            default:
                icon = ICON_15E;
                break;
        }
    }
    return icon;
}

#define ICONGX ((void (*)(Vec*, u16, u16, f32))iconDispGx)
#define ICONGX2 ((void (*)(Vec*, u16, f32, u16))iconDispGx)

static inline const char* unitwin_party_name(s32 i) {
    const char* name;
    switch (i) {
        case 0:
            name = msgSearch("name_party0");
            break;
        case 1:
            name = msgSearch("name_party1");
            break;
        case 2:
            name = msgSearch("name_party4");
            break;
        case 3:
            name = pouchGetYoshiName();
            break;
        case 4:
            name = msgSearch("name_party5");
            break;
        case 5:
            name = msgSearch("name_party2");
            break;
        case 6:
            name = msgSearch("name_party6");
            break;
    }
    return name;
}
void evt_unitwin_disp_func(CameraId cameraId, void* param) {
    PouchData* pouch = pouchGetPtr();
    UnitWinWork* wp = (UnitWinWork*)param;
    s32 mode;
    s32 row;
    s32 i;
    s32 flag;
    s32 lv;

    flag = 0; // FAKE: dead store, moves flag's variable record before i so the case 5-7 flag web is a split temp coloured before i's (target r21)
    if (wp->flags & 1) {
        const char* name = itemDataTable[wp->goods[wp->goodsIdx]].id;
        const char* desc = itemDataTable[wp->goods[wp->goodsIdx]].description;

        windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, -150.0f, 120.0f, 300.0f, 50.0f, 20.0f);
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawString((s32)(strlen(name) * 24 / 4) * -1, 108.0f, name);
        FontDrawEdgeOff();
        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
        FontDrawStart();
        FontDrawMessage(-220, -138, msgSearch(desc));

        for (i = 0; i < wp->goodsMax; i++) {
            if (wp->goods[i] != 0) {
                Vec pos;
                f32 proj[7];
                f32 vp[6];
                f32 sx, sy, sz;
                Mtx mtx;
                CameraEntry* cam;
                s32 id;
                u8 price;

                mapObjGetPos(wp->entries[i].mapObj, &pos);
                cam = camGetPtr(CAMERA_3D);
                GXSetProjection(cam->projection, cam->projType);
                GXGetProjectionv(proj);
                GXGetViewportv(vp);
                GXProject(pos.x, pos.y, pos.z, cam->view, proj, vp, &sx, &sy, &sz);
                cam = camGetPtr(cameraId);
                GXSetProjection(cam->projection, cam->projType);
                id = wp->goods[i];
                if (id >= 0xF0 && id < 0x153) {
                    price = itemDataTable[id].discount_price;
                } else {
                    price = itemDataTable[id].buy_price;
                }
                PSMTXTrans(mtx, 8.0f + (-304.0f + sx), 240.0f - (sy - 60.0f), 0.0f);
                iconNumberDispGx(mtx, price, 1, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            }
        }
    }

    if (wp->flags & 0xD000) {
        Mtx mtx;
        Mtx scale;
        s32 coin;
        s32 delta;

        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -75.0f, 150.0f, 45.0f, 20.0f);
        PSMTXTrans(mtx, -220.0f, -113.0f, 0.0f);
        PSMTXScale(scale, 0.7f, 0.7f, 0.7f);
        PSMTXConcat(mtx, scale, mtx);
        if (wp->flags & 0x1000) {
            iconDispGx2(mtx, 0x10, ICON_18B);
            coin = pouchGetCoin();
        } else if (wp->flags & 0x4000) {
            iconDispGx2(mtx, 0x10, ICON_102);
            coin = pouchGetSuperCoin();
        } else {
            iconDispGx2(mtx, 0x10, ICON_18D);
            coin = pouchGetStarPiece();
        }
        PSMTXTrans(mtx, -192.0f, -113.0f, 0.0f);
        iconDispGx2(mtx, 0x10, ICON_1D6);
        PSMTXTrans(mtx, -126.0f, -113.0f, 0.0f);
        iconNumberDispGx(mtx, wp->coin, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        if ((delta = coin - wp->coin)) {
            delta /= 10;
            if (!delta) {
                delta = coin > wp->coin ? 1 : -1;
            }
            if (wp->coinTick & 1) {
                wp->coin += delta;
                statusWinForceUpdateCoin();
            }
            wp->coinTick++;
            wp->coinTick %= 1200;
            if (!unitwin_coin_se_wait) {
                psndSFXOn("SFX_COUNT_COIN1");
                unitwin_coin_se_wait = 7;
            }
        }
        if (unitwin_coin_se_wait > 0) {
            unitwin_coin_se_wait--;
        }
    }

    if (wp->flags & 0x2000) {
        Mtx mtx;
        Mtx scale;
        s32 delta;

        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -130.0f, 150.0f, 45.0f, 20.0f);
        PSMTXTrans(mtx, -210.0f, -166.0f, 0.0f);
        PSMTXScale(scale, 1.1f, 1.1f, 1.1f);
        PSMTXConcat(mtx, scale, mtx);
        iconDispGx2(mtx, 0x10, VERSION_PICK(0x1F1, 0x226));
        PSMTXTrans(mtx, -126.0f, -168.0f, 0.0f);
        iconNumberDispGx(mtx, wp->coinDisp, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        if (!(gp->retraceCount & 3)) {
            if ((delta = pouch->mShineSpriteCount - wp->coinDisp)) {
                delta /= 10;
                if (!delta) {
                    delta = pouch->mShineSpriteCount > wp->coinDisp ? 1 : -1;
                }
                wp->coinDisp += delta;
            }
        }
    }

    if ((wp->flags & 0x100) && (lotteryGetPtr()->flags & 2)) {
        Mtx mtx;
        char buf[32];

        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -130.0f, 150.0f, 45.0f, 20.0f);
        PSMTXTrans(mtx, -220.0f, -172.0f, 0.0f);
        iconDispGx2(mtx, 0x10, ICON_140);
        sprintf(buf, "%04d", lotteryGetPtr()->number);
        for (i = 0; i < 4; i++) {
            PSMTXTrans(mtx, 20.0f * i + -126.0f - 60.0f, -168.0f, 0.0f);
            iconNumberDispGx(mtx, (s8)buf[i] - '0', 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }

    switch (mode = wp->mode) {
        case 0:
        case 1:
            break;
        case 4: {
            const char* desc;
            s32 h;
            u32 sl, st, sw, sh;

            row = 0;
            desc = NULL;

            h = (8 - pouchGetKeepItemCnt() > 0 ? 8 - pouchGetKeepItemCnt() : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 51.0f, 114.0f, 150.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(81.0f, 111.0f, msgSearch("msg_window_title_1"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < 32; i++) {
                s32 id = pouchKeepItem(i);
                ItemData* item = &itemDataTable[id];
                if (id) {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, 0x10, 0.5f, item->icon_id);
                    FontDrawStart();
                    FontDrawString(36.0f, wp->scroll + (80 - row * 24), item->id);
                    if (row == wp->cursor) {
                        desc = item->description;
                    }
                    row++;
                }
            }
            GXSetScissor(sl, st, sw, sh);
            if (desc) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(desc));
            }
            break;
        }
        case 2:
        case 3:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16: {
            s32 h;
            s32* tbl;
            const char* title;
            s32 titleX;
            const char* desc;
            s32 max;
            u32 sl, st, sw, sh;

            row = 0;
            desc = NULL;
            if (mode == 2 || mode == 3) {
                h = (8 - pouchGetHaveItemCnt() > 0 ? 8 - pouchGetHaveItemCnt() : 0) * 24;
                max = 20;
                title = msgSearch("msg_window_title_1");
                titleX = 30;
            } else if (mode == 12) {
                h = (8 - pouchGetHaveBadgeCnt() > 0 ? 8 - pouchGetHaveBadgeCnt() : 0) * 24;
                max = 200;
                title = msgSearch("msg_window_title_2");
                titleX = 40;
            } else if (mode == 14) {
                h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
                max = 0x152;
                title = msgSearch("msg_window_title_3");
                titleX = 20;
            } else if (mode == 15) {
                h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
                max = wp->count;
                title = msgSearch("msg_window_title_3");
                titleX = 20;
            } else if (mode == 16) {
                h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
                max = 0x152;
                title = msgSearch("msg_window_title_4");
                titleX = 30;
            } else {
                h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
                max = 99;
                title = msgSearch("msg_window_title_2");
                titleX = 40;
            }
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 51.0f, 114.0f, 150.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(titleX + 51, 111.0f, title);
            FontDrawEdgeOff();
            if (wp->mode != 3) {
                Mtx mtx;
                Mtx scale;
                s32 icon;
                PSMTXTrans(mtx, 244.0f, 82.0f, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, 0x10, ICON_211, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                if (wp->mode == 13) {
                    icon = ICON_18D;
                } else if (wp->mode == 16) {
                    icon = ICON_13F;
                } else {
                    icon = ICON_18B;
                }
                {
                    iconDispGx(&(Vec){244.0f, 86.0f, 0.0f}, 0x10, icon, 0.6f);
                }
            }
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (tbl = badge_bottakuru100_table, i = 0; i < max; i++) {
                
                GXColor color = {0x00, 0x00, 0x00, 0xFF};
                s32 id;
                ItemData* item;
                flag = 0;
                switch (wp->mode) {
                    case 2:
                    case 3:
                        id = pouchHaveItem(i);
                        break;
                    case 12:
                        id = pouchHaveBadge(i);
                        break;
                    case 11:
                        id = (s16)badgeShop_get(bdsw->bargain_table, i + 0xF0);
                        if (id - badgeShop_ThrowCheck(i + 0xF0) > 0) {
                            id = i + 0xF0;
                        } else {
                            id = 0;
                        }
                        break;
                    case 13:
                        if ((s16)badgeShop_get(bdsw->starmaniac_table, i + 0xF0) > 0) {
                            id = i + 0xF0;
                        } else {
                            id = 0;
                        }
                        break;
                    case 14:
                        if ((s16)badgeShop_get(bdsw->bottakuru_table, i + 1) > 0) {
                            id = i + 1;
                        } else {
                            id = 0;
                        }
                        break;
                    case 15:
                        id = tbl[i];
                        break;
                    case 16:
                        if ((s16)badgeShop_get(bdsw->bteresa_table, i + 1) > 0) {
                            id = i + 1;
                        } else {
                            id = 0;
                        }
                        break;
                }
                item = &itemDataTable[id];
                if (id) {
                    if (wp->mode == 13 && item->star_piece_price > pouchGetStarPiece()) {
                        color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                        flag = 0x20;
                    }
                    {
                        ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | 0x10, 0.5f, item->icon_id);
                    }
                    FontDrawStart();
                    FontDrawScale(0.8f);
                    FontDrawColor(color);
                    FontDrawString(36.0f, wp->scroll + (80 - row * 24) - 2.0f, item->id);
                    if (wp->mode != 3) {
                        char buf[32];
                        f32 sc;
                        s32 xoff;
                        switch (wp->mode) {
                            case 14:
                                sprintf(buf, "%2d", item->buy_price * 12 / 10);
                                sc = 1.0f;
                                xoff = 0;
                                break;
                            case 15:
                                sprintf(buf, "%3d", item->buy_price * (evtGetValue(NULL, 0xF5DE06A9) + 11) / 10);
                                sc = 1.0f;
                                xoff = -14;
                                break;
                            case 16:
                                sprintf(buf, "%4d", (item->buy_price * 2 + 2) / 3);
                                sc = 0.8f;
                                xoff = -20;
                                break;
                            case 11:
                                sprintf(buf, "%2d", item->buy_price);
                                sc = 1.0f;
                                xoff = 0;
                                break;
                            case 13:
                                sprintf(buf, "%2d", item->star_piece_price);
                                sc = 1.0f;
                                xoff = 0;
                                break;
                            case 12:
                            default:
                                sprintf(buf, "%2d", item->sell_price);
                                xoff = 0;
                                sc = 1.0f;
                                break;
                        }
                        FontDrawStart();
                        FontDrawScale(sc);
                        FontDrawColor(color);
                        FontDrawString(xoff + 226, wp->scroll + (80 - row * 24), buf);
                    }
                    if (row == wp->cursor) {
                        desc = item->description;
                    }
                    row++;
                }
            }
            GXSetScissor(sl, st, sw, sh);
            if (desc) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(desc));
            }
            break;
        }
        case 17:
        case 18: {
            InformantWork* jdt = &_jdt;
            InformantInfo* info;
            u8* tbl;
            const char* title;
            s32 h;
            s32 n;
            u32 sl, st, sw, sh;

            n = 8 - wp->count;
            h = (n > 0 ? n : 0) * 24;
            if (mode == 17) {
                title = msgSearch("msg_window_title_5");
                tbl = jdt->readFlags.perfectRead;
            } else {
                title = msgSearch("msg_window_title_6");
                tbl = jdt->readFlags.luigiRead;
            }
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 51.0f, 114.0f, 150.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(71.0f, 111.0f, title);
            FontDrawEdgeOff();
            if (wp->mode == 17) {
                Mtx mtx;
                Mtx scale;
                PSMTXTrans(mtx, 244.0f, 82.0f, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(mtx, scale, mtx);
                iconDispGxCol(mtx, 0x10, ICON_211, (GXColor){0xF0, 0xF0, 0xF0, 0xFF});
                {
                    iconDispGx(&(Vec){244.0f, 86.0f, 0.0f}, 0x10, ICON_18B, 0.6f);
                }
            }
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0, info = jdt->info; i < wp->count; i++, info++) {
                char buf[32];
                GXColor color;
                char c;
                if (johoya_get(tbl, info->storyId)) {
                    sprintf(buf, "");
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                } else {
                    sprintf(buf, "%3d", info->cost);
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                }
                FontDrawStart();
                c = info->initEndPos[info->tagEndPos];
                info->initEndPos[info->tagEndPos] = 0;
                FontDrawColor(color);
                FontDrawScale(0.8f);
                FontDrawString(1.0f, wp->scroll + (80 - i * 24), info->initEndPos);
                info->initEndPos[info->tagEndPos] = c;
                if (wp->mode == 17) {
                    FontDrawStart();
                    FontDrawColor(color);
                    FontDrawString(221.0f, wp->scroll + (80 - i * 24), buf);
                }
            }
            GXSetScissor(sl, st, sw, sh);
            break;
        }
        case 5:
        case 6:
        case 7: {
            char msgName[64] = {0};
            s32 lv;
            s32 row = 0;
            s32 h;
            u32 sl, st, sw, sh;
            GXColor color;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(41.0f, 111.0f, msgSearch("msg_window_title_7"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_8"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; party_id_table[i]; i++) {
                const char* name;
                s32 icon;
                s32 ty;
                char buf[32];
                char help[16];
                char dir[16];
                if (wp->mode == 5 || wp->mode == 6) {
                    if (!partyChkJoin(party_id_table[i])) {
                        continue;
                    }
                    if (evtGetValue(NULL, 0xF8406022) == 0) {
                        if (pouch->partyData[party_id_table[i]].techLevel < 1) {
                            color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                            lv = 0;
                            flag = 0;
                        } else {
                            color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                            lv = 1;
                            flag = 0x20;
                        }
                    } else {
                        if (pouch->partyData[party_id_table[i]].techLevel < 2) {
                            color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                            lv = 0;
                            flag = 0;
                        } else {
                            color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                            lv = 2;
                            flag = 0x20;
                        }
                    }
                } else {
                    if (!partyChkJoin(party_id_table[i])) {
                        continue;
                    }
                    if (wp->skipEntry == party_id_table[i]) {
                        continue;
                    }
                    if (pouch->partyData[party_id_table[i]].techLevel > 0) {
                        color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                        lv = 0;
                        flag = 0;
                    } else {
                        color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                        lv = 1;
                        flag = 0x20;
                    }
                }
                ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | 0x10, 0.5f, unitwin_party_icon(party_icon_table[i]));
                FontDrawStart();
                FontDrawColor(color);
                name = unitwin_party_name(i);
                ty = 80 - row * 24;
                FontDrawString(36.0f, wp->scroll + ty, name);
                sprintf(buf, "%d", pouch->partyData[party_id_table[i]].techLevel);
                FontDrawStart();
                FontDrawString(236.0f, wp->scroll + ty, buf);
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    if (wp->mode == 5 || wp->mode == 6) {
                        sprintf(dir, "lankup");
                    } else {
                        sprintf(dir, "lankdown");
                    }
                    sprintf(msgName, "%s_%s_lank%d%s", help, dir, pouch->partyData[party_id_table[i]].techLevel + 1, party_labelname_table[i]);
                }
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
        case 8: {
            char msgName[64] = {0};
            s32 h;
            u32 sl, st, sw, sh;
            
            s32 j;
            s32 flag;
            s32 lv;
            s32 bob;
            s32 row = 0;
            s32 v;
            s32 ty;
            GXColor color;
            s32 icon;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < wp->count; i++) {
                char buf[32];
                char help[16];
                flag = 0;
                for (j = 0; j < wp->count; j++) {
                    if (j != wp->cursor && *mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                        flag = 1;
                    }
                }
                if (!flag) {
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                    lv = 0;
                    flag = 0;
                } else {
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                    lv = 1;
                    flag = 0x20;
                }
                {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | 0x10, 0.5f, mario_status_icon_table[i]);
                }
                FontDrawStart();
                FontDrawColor(color);
                sprintf(buf, "%s%3d", mario_status_name_table[i], *mario_status_point_table[i]);
                FontDrawString(36.0f, wp->scroll + (80 - row * 24), buf);
                bob = (gp->retraceCount & 0xF) > 7 ? 0 : 2;
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    sprintf(msgName, "%s_levelup%s", help, mario_status_labelname_table[i]);
                    v = *mario_status_point_table[i] + mario_status_henka_table[i] * 2;
                    if (v > 0) {
                        icon = ICON_1B5;
                    } else {
                        icon = ICON_162;
                        bob = 0;
                    }
                    {
                        ICONGX2(&(Vec){166.0f, bob + (wp->scroll + (56 - row * 24)), 0.0f}, flag | 0x10, 0.5f, icon);
                    }
                    {
                        ICONGX2(&(Vec){186.0f, bob + (wp->scroll + (56 - row * 24)), 0.0f}, flag | 0x10, 0.5f, icon);
                    }
                } else {
                    v = *mario_status_point_table[i] - mario_status_henka_table[i];
                    if (v > 0) {
                        icon = ICON_1B6;
                    } else {
                        icon = ICON_162;
                        bob = 0;
                    }
                    {
                        ICONGX2(&(Vec){176.0f, (wp->scroll + (56 - row * 24)) - bob, 0.0f}, flag | 0x10, 0.5f, icon);
                    }
                }
                if (v > 0) {
                    sprintf(buf, "%3d", v);
                } else {
                    sprintf(buf, "  \x81\x48", v);
                }
                FontDrawStart();
                FontDrawString(206.0f, wp->scroll + (80 - row * 24), buf);
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
        case 9: {
            char msgName[64] = {0};
            s32 row = 0;
            s32 h;
            u32 sl, st, sw, sh;
            
            s32 j;
            s32 cnt;
            s32 v;
            s32 ty;
            GXColor color;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < wp->count; i++) {
                char buf[32];
                char help[16];
                cnt = 0;
                for (j = 0; j < wp->count; j++) {
                    if (j != wp->cursor && *mario_status_point_table[j] - mario_status_henka_table[j] <= 0) {
                        cnt++;
                    }
                }
                if (cnt < 2) {
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                    lv = 0;
                    j = 0;
                } else {
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                    lv = 1;
                    j = 0x20;
                }
                {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, j | 0x10, 0.5f, mario_status_icon_table[i]);
                }
                FontDrawStart();
                FontDrawColor(color);
                sprintf(buf, "%s%3d", mario_status_name_table[i], *mario_status_point_table[i]);
                FontDrawString(36.0f, wp->scroll + (80 - row * 24), buf);
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    sprintf(msgName, "%s_levelup%s", help, mario_status_labelname_table[i]);
                    v = *mario_status_point_table[i] + mario_status_henka_table[i];
                    if (v > 0) {
                        sprintf(buf, "%3d", v);
                    } else {
                        sprintf(buf, "  \x81\x48", v);
                    }
                    FontDrawStart();
                    FontDrawString(206.0f, wp->scroll + (80 - row * 24), buf);
                }
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
        case 10: {
            char msgName[64] = {0};
            s32 row = 0;
            s32 h;
            u32 sl, st, sw, sh;
            
            s32 v;
            s32 ty;
            s32 flag;
            GXColor color;
            s32 lv;

            h = 8 - wp->count;
            h = (h > 0 ? h : 0) * 24;
            windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -24.0f, 100.0f, 300.0f, 220 - h, 20.0f);
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 16.0f, 114.0f, 120.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(21.0f, 111.0f, msgSearch("msg_window_title_9"));
            FontDrawEdgeOff();
            windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA4, 0xFF, 0xFF}, 167.0f, 114.0f, 85.0f, 28.0f, 8.0f);
            FontDrawStart();
            FontDrawEdge();
            FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            FontDrawString(177.0f, 111.0f, msgSearch("msg_window_title_10"));
            FontDrawEdgeOff();
            GXGetScissor(&sl, &st, &sw, &sh);
            GXSetScissor(0x120, 0xA2, 0x11C, 0xBE - h);
            for (i = 0; i < 3; i++) {
                char buf[32];
                char help[16];
                if (i == wp->skipEntry) {
                    continue;
                }
                if (*mario_status_point_table[i] - mario_status_henka_table[i] > 0) {
                    color = (GXColor){0x00, 0x00, 0x00, 0xFF};
                    lv = 0;
                    flag = 0;
                } else {
                    color = (GXColor){0xA0, 0xA0, 0xA0, 0xFF};
                    lv = 1;
                    flag = 0x20;
                }
                {
                    ICONGX2(&(Vec){11.0f, wp->scroll + (56 - row * 24), 0.0f}, flag | 0x10, 0.5f, mario_status_icon_table[i]);
                }
                FontDrawStart();
                FontDrawColor(color);
                sprintf(buf, "%s%3d", mario_status_name_table[i], *mario_status_point_table[i]);
                FontDrawString(36.0f, wp->scroll + (80 - row * 24), buf);
                if (row == wp->cursor) {
                    if (lv == 0) {
                        sprintf(help, "help");
                    } else {
                        sprintf(help, "helperr");
                    }
                    sprintf(msgName, "%s_leveldown%s", help, mario_status_labelname_table[i]);
                    v = *mario_status_point_table[i] - mario_status_henka_table[i];
                    if (v > 0) {
                        sprintf(buf, "%3d", v);
                    } else {
                        sprintf(buf, "  \x81\x48", v);
                    }
                    FontDrawStart();
                    FontDrawString(206.0f, wp->scroll + (80 - row * 24), buf);
                }
                row++;
            }
            GXSetScissor(sl, st, sw, sh);
            if (*(s8*)msgName) {
                windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -240.0f, -130.0f, 480.0f, 70.0f, 20.0f);
                FontDrawStart();
                FontDrawMessage(-220, -138, msgSearch(msgName));
            }
            break;
        }
    }

    if (wp->mode != 0 && wp->mode != 1) {
        char text[136];
        char* line1;
        char* line2 = NULL;
        s32 k;
        s32 lines;
        s32 w2;

        switch (wp->mode) {
            case 0:
            case 1:
                break;
            case 11:
            case 14:
            case 15:
                strcpy(text, msgSearch("msg_window_select_6"));
                break;
            case 17:
                strcpy(text, msgSearch("msg_window_select_8"));
                break;
            case 18:
                strcpy(text, msgSearch("msg_window_select_7"));
                break;
            case 13:
            case 16:
                strcpy(text, msgSearch("msg_window_select_10"));
                break;
            case 2:
            case 12:
                strcpy(text, msgSearch("msg_window_select_2"));
                break;
            case 3:
                strcpy(text, msgSearch("msg_window_select_3"));
                break;
            case 4:
                strcpy(text, msgSearch("msg_window_select_4"));
                break;
            case 5:
                strcpy(text, msgSearch("msg_window_select_11"));
                break;
            case 6:
                strcpy(text, msgSearch("msg_window_select_11"));
                break;
            case 7:
                strcpy(text, msgSearch("msg_window_select_12"));
                break;
            case 8:
            case 9:
                strcpy(text, msgSearch("msg_window_select_9"));
                break;
            case 10:
                strcpy(text, msgSearch("msg_window_select_9_1"));
                break;
        }
        line1 = text;
        lines = 1;
        for (k = 0; ((s8*)line1)[k]; k++) {
            if (((s8*)line1)[k] == '\n') {
                text[k] = 0;
                lines = 2;
                line2 = &text[k] + 1;
                break;
            }
        }
        windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -280.0f, 100.0f, 240.0f, (lines - 1) * 24 + 45, 20.0f);
        FontDrawStart();
        k = line1 ? FontGetMessageWidth(line1) : 0;
        if (line2) {
            w2 = FontGetMessageWidth(line2);
        } else {
            w2 = 0;
        }
        if (w2 > k) {
            k = w2;
        }
        if (k <= 228) {
            k = (228 - k) / 2;
        } else {
            FontDrawScale(228.0f / k);
            k = 0;
        }
        FontDrawString(k - 274, 90.0f, line1);
        if (line2) {
            FontDrawString(k - 274, 66.0f, line2);
        }
        iconFlagOff("yubicursor_2", 2);
        iconSetPos("yubicursor_2", wp->cursorX, wp->cursorY, 0.0f);
    } else {
        iconFlagOn("yubicursor_2", 2);
    }
}







USER_FUNC(set_new_goods_list) {
    EvtWord* args = event->args;
    UnitWinWork* wp = (UnitWinWork*)evtGetValue(event, args[0]);
    s32 item = evtGetValue(event, args[1]);
    char name[32];
    wp->goods[wp->goodsIdx] = item;
    if (wp->goodsIdx < wp->goodsMax) {
        sprintf(name, "uwn%d", wp->goodsIdx);
        if (item != 0) {
            iconChange(name, itemDataTable[item].icon_id);
        } else {
            iconDelete(name);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwin_get_work_ptr) {
    evtSetValue(event, event->args[0], (EvtWord)_wp);
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwin_getpartylank) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 idx = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], pouch->partyData[idx].techLevel);
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwin_setpartylank) {
    PouchPartyData* party;
    u32 ofs;
    s16 v;
    s32 ofs2;
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    ofs = evtGetValue(event, args[0]) * sizeof(PouchPartyData); // byte offset of partyData[i]: the target multiplies right after the read
    v = evtGetValue(event, args[1]);
    ofs2 = ofs; // FAKE: signed copy for the first stores, so pouch + ofs is added again after pouchRevisePartyParam as in the target
    party = (PouchPartyData*)((u8*)pouch->partyData + ofs2);
    party->HPLevel = party->attackLevel = party->techLevel = v;
    pouchRevisePartyParam();
    party = (PouchPartyData*)((u8*)pouch->partyData + ofs);
    party->currentHP = party->maximumHP;
    return EVT_RETURN_DONE;
}

USER_FUNC(unitwin_setmariolevel) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 i;
    BOOL r;
    pouch->baseMaxHP = evtGetValue(event, args[0]);
    pouch->baseMaxFP = evtGetValue(event, args[1]);
    pouch->maximumBP = evtGetValue(event, args[2]);
    pouchReviseMarioParam();
    pouch->currentHP = pouch->maximumHP;
    pouch->currentFP = pouch->maximumFP;
    if (pouch->availableBP < 0) {
        for (i = 0; i < 200; i++) {
            if (pouchEquipCheckBadgeIndex(i)) {
                pouchUnEquipBadgeIndex(i);
            }
        }
        pouchReviseMarioParam();
        r = TRUE;
    } else {
        r = FALSE;
    }
    evtSetValue(event, args[3], r);
    return EVT_RETURN_DONE;
}

