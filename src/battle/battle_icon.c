#include "battle/battle_icon.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;

void* memset(void* dst, int value, u32 size);
void iconDelete(void* icon);
void* BtlIconGetPtr(s32 id);
void BtlIcon_Delete(void* icon);
void iconSetPos(void* icon, f32 x, f32 y, f32 z);
void iconSetScale(void* icon, f32 scale);
void iconFlagOn(void* icon, s32 flag);
void iconFlagOff(void* icon, s32 flag);
void iconEntry(char* name, u16 type);
void* iconNameToPtr(char* name);
int sprintf(char* str, const char* format, ...);

extern u8 itemDataTable[];
extern f32 float_1_80424b1c;
extern const char str_biu_PCT02x_802fa888[];

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
    s32 offset;
    s32 work;
    s32 i;

    i = 0;
    offset = 0;
    work = (s32)_battleWorkPointer;
    for (; i < 0x10; i++, offset += 0x9C) {
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


USER_FUNC(btlevtcmd_BtlIconJumpPosition) {
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 float_0_80424b18;
    extern f32 float_0p5_80424b20;

    s32* args;
    s32 id;
    f32 x;
    f32 y;
    f32 z;
    s32 frames;
    void* icon;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 dist;
    f32 zero;
    f32 framef;

    args = event->args;
    id = evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    frames = evtGetValue(event, args[4]);
    icon = BtlIconGetPtr(id);

    if (isFirstCall != 0) {
        *(u8*)((s32)event + 0xD) = 1;
        if (frames != 0) {
            *(s32*)((s32)icon + 0x44) = frames;
        }

        *(f32*)((s32)icon + 0x38) = x;
        *(f32*)((s32)icon + 0x3C) = y;
        *(f32*)((s32)icon + 0x40) = z;

        startX = *(f32*)((s32)icon + 8);
        startY = *(f32*)((s32)icon + 0xC);
        startZ = *(f32*)((s32)icon + 0x10);

        *(f32*)((s32)icon + 0x2C) = startX;
        *(f32*)((s32)icon + 0x20) = startX;
        *(f32*)((s32)icon + 0x30) = startY;
        *(f32*)((s32)icon + 0x24) = startY;
        *(f32*)((s32)icon + 0x34) = startZ;
        *(f32*)((s32)icon + 0x28) = startZ;

        *(f32*)((s32)icon + 0x54) = angleABf(startX, startZ, x, z);
        dist = distABf(startX, startZ, x, z);
        zero = float_0_80424b18;
        *(f32*)((s32)icon + 0x58) = dist;

        if (x - startX > zero) {
            *(s8*)((s32)icon + 0x60) = 1;
        } else if (x - startX < zero) {
            *(s8*)((s32)icon + 0x60) = -1;
        }

        if (*(s32*)((s32)icon + 0x44) == 0) {
            *(s32*)((s32)icon + 0x44) = (s32)(*(f32*)((s32)icon + 0x58) / *(f32*)((s32)icon + 0x48));
        } else {
            *(f32*)((s32)icon + 0x48) = *(f32*)((s32)icon + 0x58) / (f32)*(s32*)((s32)icon + 0x44);
        }

        framef = (f32)*(s32*)((s32)icon + 0x44);
        *(f32*)((s32)icon + 0x58) = -((framef * *(f32*)((s32)icon + 0x48)) - *(f32*)((s32)icon + 0x58));

        if (*(s32*)((s32)icon + 0x44) == 0) {
            return 2;
        }

        framef = (f32)*(s32*)((s32)icon + 0x44);
        *(f32*)((s32)icon + 0x50) = (*(f32*)((s32)icon + 0x4C) * framef * float_0p5_80424b20) + ((y - startY) / framef);
        *(f32*)((s32)icon + 0x48) += *(f32*)((s32)icon + 0x58) / framef;
    }

    zero = float_0_80424b18;
    *(f32*)((s32)icon + 0x30) += *(f32*)((s32)icon + 0x50);
    *(f32*)((s32)icon + 0x50) -= *(f32*)((s32)icon + 0x4C);
    if (*(f32*)((s32)icon + 0x50) < zero && *(f32*)((s32)icon + 0x30) < *(f32*)((s32)icon + 0x3C)) {
        *(f32*)((s32)icon + 0x30) = *(f32*)((s32)icon + 0x3C);
    }

    btlMovePos(*(f32*)((s32)icon + 0x48), *(f32*)((s32)icon + 0x54), (f32*)((s32)icon + 0x2C), (f32*)((s32)icon + 0x34));

    *(f32*)((s32)icon + 8) = *(f32*)((s32)icon + 0x2C);
    *(f32*)((s32)icon + 0xC) = *(f32*)((s32)icon + 0x30);
    *(f32*)((s32)icon + 0x10) = *(f32*)((s32)icon + 0x34);

    *(s32*)((s32)icon + 0x44) -= 1;
    if (*(s32*)((s32)icon + 0x44) < 1) {
        *(f32*)((s32)icon + 8) = *(f32*)((s32)icon + 0x38);
        *(f32*)((s32)icon + 0xC) = *(f32*)((s32)icon + 0x3C);
        *(f32*)((s32)icon + 0x10) = *(f32*)((s32)icon + 0x40);
        return 1;
    }

    return 0;
}

void* BtlIcon_Entry(u16 type, f32 x, f32 y, f32 z) {
    s32 i;
    s32 offset;
    void* icon;
    char name[12];

    i = 0;
    offset = 0;
    icon = NULL;
    while (i < 0x10) {
        icon = (void*)((s32)_battleWorkPointer + offset + 0x182CC);
        if (!(*(u16*)icon & 1)) {
            break;
        }
        i++;
        offset += 0x9C;
    }

    if (i >= 0x10) {
        return NULL;
    }

    memset(icon, 0, 0x9C);
    *(s32*)((s32)icon + 4) = i;
    *(u16*)icon = 1;
    *(f32*)((s32)icon + 0x14) = float_1_80424b1c;
    *(f32*)((s32)icon + 0x18) = float_1_80424b1c;
    *(f32*)((s32)icon + 8) = x;
    *(f32*)((s32)icon + 0xC) = y;
    *(f32*)((s32)icon + 0x10) = z;
    *(u16*)((s32)icon + 2) = type;
    sprintf(name, str_biu_PCT02x_802fa888, *(s32*)((s32)icon + 4));
    iconEntry(name, *(u16*)((s32)icon + 2));
    *(void**)((s32)icon + 0x1C) = iconNameToPtr(name);
    iconSetPos((void*)(*(s32*)((s32)icon + 0x1C) + 0x18),
               *(f32*)((s32)icon + 8),
               *(f32*)((s32)icon + 0xC),
               *(f32*)((s32)icon + 0x10));
    return icon;
}


USER_FUNC(btlevtcmd_BtlIconEntryItemId) {
    s32* args;
    s32 itemId;
    f32 x;
    f32 y;
    void* icon;
    s32 out;

    args = event->args;
    itemId = evtGetValue(event, args[0]);
    x = evtGetValue(event, args[1]);
    y = evtGetValue(event, args[2]);
    icon = BtlIcon_Entry(*(u16*)((s32)itemDataTable + itemId * 0x28 + 0x20),
                         x,
                         y,
                         (f32)evtGetValue(event, args[3]));
    out = args[4];
    evtSetValue(event, out, *(s32*)((s32)icon + 4));
    return 2;
}


USER_FUNC(btlevtcmd_BtlIconEntry) {
    s32* args;
    s32 type;
    f32 x;
    f32 y;
    void* icon;
    s32 out;

    args = event->args;
    type = evtGetValue(event, args[0]);
    x = evtGetValue(event, args[1]);
    y = evtGetValue(event, args[2]);
    icon = BtlIcon_Entry((u16)type, x, y, (f32)evtGetValue(event, args[3]));
    out = args[4];
    evtSetValue(event, out, *(s32*)((s32)icon + 4));
    return 2;
}


void BattleIconMain(void) {
    void* work;
    s32 offset;
    void* icon;
    s32 i;

    i = 0;
    offset = 0;
    work = _battleWorkPointer;
    for (; i < 0x10; i++, offset += 0x9C) {
        icon = (void*)((s32)work + offset + 0x182CC);
        if ((*(u16*)icon & 1) && *(void**)((s32)icon + 0x1C) != NULL) {
            iconSetPos((void*)(*(s32*)((s32)icon + 0x1C) + 0x18),
                       *(f32*)((s32)icon + 8),
                       *(f32*)((s32)icon + 0xC),
                       *(f32*)((s32)icon + 0x10));
            iconSetScale((void*)(*(s32*)((s32)icon + 0x1C) + 0x18),
                         *(f32*)((s32)icon + 0x14) * *(f32*)((s32)icon + 0x18));
            if (*(u16*)icon & 2) {
                iconFlagOn((void*)(*(s32*)((s32)icon + 0x1C) + 0x18), 2);
            } else {
                iconFlagOff((void*)(*(s32*)((s32)icon + 0x1C) + 0x18), 2);
            }
        }
    }
}


USER_FUNC(btlevtcmd_BtlIconSetPosition) {
    s32* args;
    void* icon;
    f32 x;
    f32 y;
    f32 z;

    args = event->args;
    icon = BtlIconGetPtr(evtGetValue(event, args[0]));
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    *(f32*)((s32)icon + 8) = x;
    *(f32*)((s32)icon + 0xC) = y;
    *(f32*)((s32)icon + 0x10) = z;
    return 2;
}
