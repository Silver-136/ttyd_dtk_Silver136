#include "driver/itemdrv.h"
extern void* gp;
extern u8 work[];

extern u8 itemDataTable[];

extern s32 winMgrAction(void* win);
extern void winMgrHelpDraw(void* win);
extern s32 winMgrHelpMain(void* win);
extern void* msgSearch(void* msgId);

u32 itemStatus(void* item, u32 flags) {
    return *(u32*)((s32)item + 0x38) & flags;
}

u16 itemFlag(void* item, u32 flags) {
    return *(u16*)item & flags;
}

void itemStatusOn(void* item, u32 flags) {
    *(u32*)((s32)item + 0x38) |= flags;
}

void itemFlagOff(void* item, u32 flags) {
    *(u16*)item &= ~flags;
}

void itemFlagOn(void* item, u32 flags) {
    *(u16*)item |= flags;
}

void itemSetPosition(void* item, f32 x, f32 y, f32 z) {
    *(f32*)((s32)item + 0x3C) = x;
    *(f32*)((s32)item + 0x40) = y;
    *(f32*)((s32)item + 0x44) = z;
}

void itemModeChange(void* item, u16 mode) {
    *(u16*)((s32)item + 0x24) = mode;
    *(u16*)((s32)item + 0x26) = 0;
}

void itemPickUp(void* item) {
    u32 status;

    status = *(u32*)((s32)item + 0x38);
    status |= 0x1000;
    *(u32*)((s32)item + 0x38) = status;

    *(u16*)((s32)item + 0x24) = 8;
    *(u16*)((s32)item + 0x26) = 0;
    *(s32*)((s32)item + 0x7C) = 0;
    *(s32*)((s32)item + 0x78) = 0;

    *(u32*)((s32)item + 0x38) &= ~0x100;
}

void itemForceGet(void* item) {
    *(s32*)((s32)item + 0x7C) = 0;
    *(s32*)((s32)item + 0x78) = 0;
    *(u16*)((s32)item + 0x24) = 2;
    *(u16*)((s32)item + 0x26) = 0;

    *(u16*)item |= 0x1000;
    *(u32*)((s32)item + 0x38) &= ~0x10000;
}

s32 N_itemPickUpFromFieldCheck(void) {
    void* workSet;
    void* item;
    s32 count;

    workSet = work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }

    count = *(s32*)workSet;
    item = *(void**)((s32)workSet + 0x4);

    while (count > 0) {
        if ((*(u16*)item & 1) != 0) {
            if (*(u16*)((s32)item + 0x24) == 2) {
                return 1;
            }
        }

        item = (void*)((s32)item + 0x98);
        count--;
    }

    return 0;
}

void winHelpDisp(void* win) {
    void* data;
    s32 offset;

    data = *(void**)((s32)win + 0x2C);

    if (winMgrAction(*(void**)((s32)data + 0x2C)) == 0) {
        offset = *(s32*)((s32)data + 0x4);
        offset *= 0x28;

        *(void**)((s32)win + 0x34) = msgSearch(
            *(void**)((s32)itemDataTable + offset + 0x8)
        );

        winMgrHelpDraw(win);
    }
}

s32 winHelpMain(void* win) {
    return winMgrHelpMain(win);
}


s32 itemEntry(s32 a, s32 item, s32 mode, s32 c, s32 d, f32 x, f32 y, f32 z) {
    return 0;
}


u8 itemMain(void) {
    return 0;
}


u8 itemHitCheckSide(void) {
    return 0;
}


u8 itemHitCheck(s64 posX, s64 posY, s64 posZ, s64 param_4) {
    return 0;
}


void winNameDisp(int param_1) {
    extern u32 FontGetMessageWidthLine(char* msg, s16* lineCount);
    extern void FontDrawStart(void);
    extern void FontDrawColor(void* color);
    extern u8 FontDrawMessage(u32 x, u32 y, char* msg);
    extern s32 sprintf(char* buf, const char* fmt, ...);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d1364(char* dst, char* src);
    extern u32 dat_804210a8;
    extern char str_msg_window_badge_get_802c92a8[];
    extern char str_msg_window_item_get_802c92c0[];

    u32 color;
    u16 lines[2];
    char buf[0x400];
    void* data = *(void**)(param_1 + 0x2C);
    char* itemMsg;
    u32 width;

    itemMsg = msgSearch(*(void**)((s32)itemDataTable + *(s32*)((s32)data + 4) * 0x28 + 4));

    if (*(void**)((s32)gp + 0x16C) == 0) {
        if (*(s32*)((s32)data + 4) >= 0xF0 && *(s32*)((s32)data + 4) < 0x153) {
            sprintf(buf, msgSearch(str_msg_window_badge_get_802c92a8), itemMsg);
        } else {
            sprintf(buf, msgSearch(str_msg_window_item_get_802c92c0), itemMsg);
        }
    } else {
        if (*(s32*)((s32)data + 4) >= 0xF0 && *(s32*)((s32)data + 4) < 0x153) {
            strcpy(buf, msgSearch(str_msg_window_badge_get_802c92a8));
        } else {
            strcpy(buf, msgSearch(str_msg_window_item_get_802c92c0));
        }
        unk_800d1364(buf, itemMsg);
    }

    width = FontGetMessageWidthLine(buf, (s16*)lines) & 0xFFFF;
    if ((s32)(width + 0x14) > *(s32*)(param_1 + 0x20)) {
        *(s32*)(param_1 + 0x20) = width + 0x14;
        *(s32*)(param_1 + 0x18) = -*(s32*)(param_1 + 0x20) / 2;
    }

    *(s32*)(param_1 + 0x24) = *(s32*)(*(s32*)(param_1 + 0x28) + 0x18) + lines[0] * 0x18;

    if (winMgrAction(*(void**)((s32)data + 0x28)) == 0) {
        FontDrawStart();
        color = dat_804210a8;
        FontDrawColor(&color);
        FontDrawMessage((u32)(-(s32)width / 2), *(s32*)(param_1 + 0x1C) - 8, buf);
    }
}

u8 winNameDisp2(int param_1) {
    extern f32 FontGetMessageWidthLine(const char* msg, s32 line);
    extern void FontDrawStart(void);
    extern void FontDrawColor(void* color);
    extern u8 FontDrawMessage(u32 x, u32 y, char* msg);
    extern void* msgSearch(void* msgId);
    extern s32 sprintf(char* buf, const char* fmt, ...);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d1364(char* dst, const char* src);
    extern u32 dat_804210ac;
    extern char str_msg_window_item_rele_802c9290[];
    char buf[0x400];
    u16 lines[2];
    void* data;
    char* msg;
    u16 width;
    u32 color;

    data = *(void**)(param_1 + 0x2C);
    msg = msgSearch(*(void**)((s32)itemDataTable + (*(s32*)((s32)data + 4) * 0x28) + 4));
    if (*(void**)((s32)gp + 0x16C) == 0) {
        sprintf(buf, msgSearch(str_msg_window_item_rele_802c9290), msg);
    } else {
        strcpy(buf, msgSearch(str_msg_window_item_rele_802c9290));
        unk_800d1364(buf, msg);
    }
    width = (u16)FontGetMessageWidthLine(buf, (s32)lines);
    if ((s32)(width + 0x14) > *(s32*)(param_1 + 0x20)) {
        *(s32*)(param_1 + 0x20) = width + 0x14;
        *(s32*)(param_1 + 0x18) = -*(s32*)(param_1 + 0x20) / 2;
    }
    *(s32*)(param_1 + 0x24) = *(s32*)(*(s32*)(param_1 + 0x28) + 0x18) + lines[0] * 0x18;
    if (winMgrAction(*(void**)((s32)data + 0x28)) == 0) {
        FontDrawStart();
        color = dat_804210ac;
        FontDrawColor(&color);
        FontDrawMessage((u32)(-width / 2), *(s32*)(param_1 + 0x1C) - 8, buf);
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void itemDelete(void* name) {
    extern s32 strcmp(const char* a, const char* b);
    extern void pouchArriveBadge(s32 itemId);
    extern void effSoftDelete(void* eff);
    extern void iconDelete(void* iconName);
    void* workSet;
    void* item;
    s32 i;

    workSet = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }
    item = *(void**)((s32)workSet + 4);
    for (i = 0; i < *(s32*)workSet; i++, item = (void*)((s32)item + 0x98)) {
        if (strcmp((char*)((s32)item + 0xC), name) == 0) {
            if (*(s32*)((s32)gp + 0x14) == 0) {
                s32 itemId = *(s32*)((s32)item + 4);
                if (itemId >= 0xF0 && itemId < 0x153 && (*(u16*)item & 0x100) != 0) {
                    pouchArriveBadge(itemId);
                }
            }
            if (*(s32*)((s32)item + 4) >= 0x72 && *(s32*)((s32)item + 4) < 0x79) {
                effSoftDelete(*(void**)((s32)item + 0x1C));
            } else {
                iconDelete((void*)((s32)item + 0xC));
            }
            *(u16*)item &= ~1;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void itemReInit(void) {
    extern void pouchArriveBadge(s32 itemId);
    extern void iconDelete(void* iconName);
    extern void* memset(void* ptr, s32 value, u32 size);
    void* workSet;
    void* item;
    s32 i;

    workSet = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }
    if (*(s32*)((s32)gp + 0x14) == 0) {
        item = *(void**)((s32)workSet + 4);
        for (i = 0; i < *(s32*)workSet; i++, item = (void*)((s32)item + 0x98)) {
            u16 flags = *(u16*)item;
            if ((flags & 1) != 0) {
                s32 itemId = *(s32*)((s32)item + 4);
                if (itemId >= 0xF0 && itemId < 0x153 && (flags & 0x100) != 0) {
                    pouchArriveBadge(itemId);
                }
            }
        }
    }
    item = *(void**)((s32)workSet + 4);
    for (i = 0; i < *(s32*)workSet; i++, item = (void*)((s32)item + 0x98)) {
        iconDelete((void*)((s32)item + 0xC));
        *(u16*)item &= ~1;
        memset((void*)((s32)item + 0xC), 0, 0x10);
    }
}

void* itemNameToPtr(s32 name) {
    extern s32 strcmp(const char* a, const char* b);
    void* workSet;
    void* gpWork;
    void* item;
    s32 i;

    workSet = work;
    gpWork = gp;
    if (*(s32*)((s32)gpWork + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }
    item = *(void**)((s32)workSet + 4);
    for (i = 0; i < *(s32*)workSet; i++, item = (void*)((s32)item + 0x98)) {
        if (strcmp((char*)((s32)item + 0xC), (char*)name) == 0) {
            return item;
        }
    }
    return 0;
}

void itemInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    void* item;

    *(s32*)work = 0x20;
    item = __memAlloc(0, *(s32*)work * 0x98);
    *(void**)(work + 4) = item;
    memset(item, 0, *(s32*)work * 0x98);
    *(s32*)(work + 8) = 0;
    *(s32*)(work + 0x1C) = 0x20;
    item = __memAlloc(0, *(s32*)(work + 0x1C) * 0x98);
    *(void**)(work + 0x20) = item;
    memset(item, 0, *(s32*)(work + 0x1C) * 0x98);
    *(s32*)(work + 0x24) = 0;
}

void itemCoinDrop(float* param_1) {
    extern s32 pouchGetCoin(void);
    extern void pouchAddCoin(s32 coin);
    extern void* itemEntry(s32 name, s32 item, s32 mode, s32 script, s32 arg, f32 x, f32 y, f32 z);

    void* item;

    if (pouchGetCoin() > 0) {
        pouchAddCoin(-1);
        item = itemEntry(0, 0x79, 8, -1, 0, param_1[0], param_1[1], param_1[2]);
        if (item != 0) {
            *(u32*)((s32)item + 0x38) |= 0x40;
        }
    }
}

u8 itemseq_GetItem(void* pItem, s32 param_2, s32 param_3, s32 param_4, u32* param_5, u32 param_6) {
    return 0;
}


u8 itemseq_Bound(void* itemEntry) {
    return 0;
}


s32 itemGetCheck(void* itemEntry) {
    extern void* marioGetPtr(void);
    extern s32 marioStGetSystemLevel(void);
    extern s32 seqGetSeq(void);
    extern s32 vivianGetStatus(void);
    extern s32 nokonoko_holdItem(void);
    extern s32 N_fbatPreventMarioEventChk(void);

    void* player = marioGetPtr();
    void* workSet;
    void* item;
    s32 count;
    s32 mode;

    if (itemEntry == 0) {
        return 0;
    }
    if ((*(u32*)((s32)itemEntry + 0x38) & 0x100000) != 0) {
        return 0;
    }

    if ((s32)*(u8*)((s32)player + 0x3C) == 2) {
        if ((*(u32*)((s32)player + 0x14) & 1) == 0) {
            if ((u32)marioStGetSystemLevel() != 0) {
                return 0;
            }
            if ((*(u32*)player & 0xA) != 0) {
                return 0;
            }
        }
    } else {
        if ((u32)marioStGetSystemLevel() != 0) {
            return 0;
        }
        if ((*(u32*)player & 0xA) != 0) {
            return 0;
        }
    }

    if (*(s32*)((s32)gp + 0x164) != 0) {
        return 0;
    }
    if (seqGetSeq() != 2) {
        return 0;
    }
    if (*(u16*)((s32)player + 0x2E) == 0x19) {
        return 0;
    }
    if (*(u16*)((s32)player + 0x2E) == 0x18) {
        return 0;
    }
    if (vivianGetStatus() != 0) {
        return 0;
    }
    if (*(u16*)((s32)player + 0x2E) == 0x1F || *(u16*)((s32)player + 0x2E) == 0x20) {
        return 0;
    }
    if (nokonoko_holdItem() != 0) {
        return 0;
    }

    workSet = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)work + 0x1C);
    }

    count = *(s32*)workSet;
    item = *(void**)((s32)workSet + 4);
    while (count > 0) {
        if ((*(u16*)item & 1) != 0 && *(u16*)((s32)item + 0x24) == 2) {
            return 0;
        }
        item = (void*)((s32)item + 0x98);
        count--;
    }

    if (N_fbatPreventMarioEventChk() != 0) {
        return 0;
    }

    mode = *(u16*)((s32)itemEntry + 0x24);
    if (mode < 5) {
        if (mode != 2) {
            if (mode < 2) {
                if (mode != 0) {
                    return 1;
                }
            } else if ((*(u32*)((s32)itemEntry + 0x38) & 8) == 0) {
                return 1;
            }
        }
    } else {
        if (mode == 7) {
            return 1;
        }
        if (mode < 7 && *(u16*)((s32)itemEntry + 0x26) != 0 &&
            (*(u32*)((s32)itemEntry + 0x38) & 4) == 0 &&
            (*(u32*)((s32)itemEntry + 0x38) & 8) == 0) {
            return 1;
        }
    }

    return 0;
}

void* itemNearDistCheck(float posX, float posY, float posZ, float maxDist) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern void* marioGetPtr(void);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 PSVECMag(void* v);

    void* workSet;
    void* item;
    void* nearest;
    Vec3 pos;
    Vec3 itemPos;
    Vec3 delta;
    f32 dist;
    s32 i;

    marioGetPtr();

    workSet = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }

    item = *(void**)((s32)workSet + 4);
    nearest = 0;

    pos.x = posX;
    pos.y = posY;
    pos.z = posZ;

    for (i = 0; i < *(s32*)workSet; i++, item = (void*)((s32)item + 0x98)) {
        if ((*(u16*)item & 1) != 0) {
            u32 status = *(u32*)((s32)item + 0x38);
            if ((status & 0x1000) == 0 && (status & 0x100000) == 0 &&
                *(u16*)((s32)item + 0x24) != 2 && (status & 0x2000000) == 0) {
                itemPos.x = *(f32*)((s32)item + 0x3C);
                itemPos.y = *(f32*)((s32)item + 0x40);
                itemPos.z = *(f32*)((s32)item + 0x44);
                PSVECSubtract(&itemPos, &pos, &delta);
                dist = PSVECMag(&delta);
                if (dist < maxDist) {
                    nearest = item;
                    maxDist = dist;
                }
            }
        }
    }

    return nearest;
}

u32 itemGetNokoCheck(void* itemEntry) {
    extern void* marioGetPtr(void);
    extern s32 marioStGetSystemLevel(void);
    extern s32 seqGetSeq(void);
    extern s32 vivianGetStatus(void);
    extern s32 N_fbatPreventMarioEventChk(void);

    void* player = marioGetPtr();
    void* workSet;
    void* item;
    s32 count;

    if (itemEntry == 0) {
        return 0;
    }
    if ((*(u32*)((s32)itemEntry + 0x38) & 0x100000) != 0) {
        return 0;
    }
    if ((u32)marioStGetSystemLevel() != 0) {
        return 0;
    }
    if ((*(u32*)player & 0xA) != 0) {
        return 0;
    }
    if ((*(u32*)((s32)itemEntry + 0x38) & 0x2000000) != 0) {
        return 0;
    }
    if (*(s32*)((s32)gp + 0x164) != 0) {
        return 0;
    }
    if (seqGetSeq() != 2) {
        return 0;
    }
    if (*(u16*)((s32)player + 0x2E) == 0x19) {
        return 0;
    }
    if (vivianGetStatus() != 0) {
        return 0;
    }
    if (*(u16*)((s32)player + 0x2E) == 0x1F || *(u16*)((s32)player + 0x2E) == 0x20) {
        return 0;
    }
    if ((u16)(*(u16*)((s32)player + 0x2E) - 5) <= 3 ||
        *(u16*)((s32)player + 0x2E) == 0xF ||
        *(u16*)((s32)player + 0x2E) == 0xE) {
        return 0;
    }

    workSet = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)work + 0x1C);
    }

    count = *(s32*)workSet;
    item = *(void**)((s32)workSet + 4);
    while (count > 0) {
        if ((*(u16*)item & 1) != 0 && *(u16*)((s32)item + 0x24) == 2) {
            return 0;
        }
        item = (void*)((s32)item + 0x98);
        count--;
    }

    return N_fbatPreventMarioEventChk() == 0;
}

void winFullDisp(void* winMgrEntry) {
    extern s32 FontGetMessageWidth(const char* msg);
    extern void FontDrawStart(void);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern void FontDrawColor(void* color);
    extern u8 FontDrawMessage(u32 x, u32 y, char* msg);
    extern u32 dat_804210b0;
    extern char str_msg_window_badge_ful_802c9260[];
    extern char str_msg_window_item_full_802c9278[];

    s32 badge;
    void* data;
    char* msg;
    u16 width;
    u32 color;

    badge = 0;
    data = *(void**)((s32)winMgrEntry + 0x2C);
    if (*(s32*)((s32)data + 4) >= 0xF0 && *(s32*)((s32)data + 4) < 0x153) {
        badge = 1;
    }
    if (badge != 0) {
        msg = msgSearch(str_msg_window_badge_ful_802c9260);
    } else {
        msg = msgSearch(str_msg_window_item_full_802c9278);
    }
    width = (u16)FontGetMessageWidth(msg);
    if ((s32)(width + 0x14) > *(s32*)((s32)winMgrEntry + 0x20)) {
        *(s32*)((s32)winMgrEntry + 0x20) = width + 0x14;
        *(s32*)((s32)winMgrEntry + 0x18) = -*(s32*)((s32)winMgrEntry + 0x20) / 2;
    }
    if (winMgrAction(*(void**)((s32)data + 0x30)) == 0) {
        FontDrawStart();
        FontDrawEdge();
        color = dat_804210b0;
        FontDrawColor(&color);
        FontDrawMessage((u32)(-width / 2), *(s32*)((s32)winMgrEntry + 0x1C) - 10, msg);
        FontDrawEdgeOff();
    }
}

void itemNokoForceGet(void* item) {
    if ((s32)itemGetNokoCheck(item) != 0) {
        *(s32*)((s32)item + 0x7C) = 0;
        *(s32*)((s32)item + 0x78) = 0;
        *(u16*)((s32)item + 0x24) = 2;
        *(u16*)((s32)item + 0x26) = 0;
        *(u16*)item |= 0x5000;
        *(u32*)((s32)item + 0x38) &= ~0x10000;
    } else {
        *(s32*)((s32)item + 0x7C) = 0;
        *(s32*)((s32)item + 0x78) = 0;
        *(u16*)((s32)item + 0x24) = 1;
        *(u16*)((s32)item + 0x26) = 0;
        *(u32*)((s32)item + 0x38) &= ~4;
        *(u32*)((s32)item + 0x38) &= ~8;
        *(u32*)((s32)item + 0x38) &= ~0x4000;
        *(u32*)((s32)item + 0x38) &= ~0x1000;
    }
}

