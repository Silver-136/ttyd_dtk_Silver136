#include "battle/battle_icon.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;

void* memset(void* dst, int value, u32 size);
void iconDelete(void* icon);
void* BtlIconGetPtr(s32 id);
void BtlIcon_Delete(void* icon);

void BattleIconInit(void) {
    memset((void*)((s32)_battleWorkPointer + 0x182CC), 0, 0x9C0);
}

void* BtlIconGetPtr(s32 id) {
    s32 i;
    s32 offset = 0;
    s32 work = (s32)_battleWorkPointer;

    for (i = 0; i < 0x10; i++, offset += 0x9C) {
        void* icon = (void*)(work + offset + 0x182CC);

        if ((*(u16*)icon & 1) && (*(s32*)((s32)icon + 4) == id)) {
            return icon;
        }
    }

    return 0;
}

void BtlIcon_Delete(void* icon) {
    if (*(u16*)icon & 1) {
        iconDelete((void*)(*(s32*)((s32)icon + 0x1C) + 0x18));
        *(u16*)icon &= ~1;
    }
}

void BattleIconEnd(void) {
    s32 work = (s32)_battleWorkPointer;
    s32 i;
    s32 offset = 0;

    for (i = 0; i < 0x10; i++, offset += 0x9C) {
        BtlIcon_Delete((void*)(work + offset + 0x182CC));
    }
}

USER_FUNC(btlevtcmd_BtlIconDelete) {
    BtlIcon_Delete(BtlIconGetPtr(evtGetValue(event, event->args[0])));
    return 2;
}

USER_FUNC(btlevtcmd_BtlIconSetFallAccel) {
    s32* args = event->args;
    void* icon = BtlIconGetPtr(evtGetValue(event, args[0]));

    *(f32*)((s32)icon + 0x4C) = evtGetFloat(event, args[1]);
    return 2;
}
