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


u8 winNameDisp(int param_1) {
    return 0;
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

u8 itemCoinDrop(float* param_1) {
    return 0;
}


u8 itemseq_GetItem(void* pItem, s32 param_2, s32 param_3, s32 param_4, u32* param_5, u32 param_6) {
    return 0;
}


u8 itemseq_Bound(void* itemEntry) {
    return 0;
}


s32 itemGetCheck(void* itemEntry) {
    return 0;
}


void* itemNearDistCheck(float posX, float posY, float posZ, s64 param_4) {
    return 0;
}


u32 itemGetNokoCheck(void* itemEntry) {
    return 0;
}


u8 winFullDisp(void* winMgrEntry) {
    extern s32 FontGetMessageWidth(const char* msg);
    extern void FontDrawStart(void);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern void FontDrawColor(void* color);
    extern u8 FontDrawMessage(u32 x, u32 y, char* msg);
    extern u32 dat_804210b0;
    extern char str_msg_window_badge_ful_802c9260[];
    extern char str_msg_window_item_full_802c9278[];
    void* data;
    char* msg;
    u16 width;
    u32 color;
    s32 badge;

    data = *(void**)((s32)winMgrEntry + 0x2C);
    badge = 0;
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
    return 0;
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

