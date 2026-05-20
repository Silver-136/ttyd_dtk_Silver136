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