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
    return 0;
}


void itemDelete(void* item) {
}


void itemReInit(void) {
}


void* itemNameToPtr(s32 name) {
    return 0;
}


u8 itemInit(void) {
    return 0;
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
    return 0;
}


u8 itemNokoForceGet(void* itemEntry) {
    return 0;
}
