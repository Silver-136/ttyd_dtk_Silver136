#include "mario/mario_pouch.h"

typedef s16 ItemType16;
extern u8 itemDataTable[];
extern void* gp;

const char str_name_party3_802cc728[] = "name_party3";

u8 _party_max_hp_table[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 10, 0, 20, 0, 30, 0, 200,
    0, 10, 0, 15, 0, 25, 0, 200,
    0, 20, 0, 30, 0, 40, 0, 200,
    0, 10, 0, 20, 0, 30, 0, 200,
    0, 15, 0, 25, 0, 35, 0, 200,
    0, 15, 0, 20, 0, 30, 0, 200,
    0, 15, 0, 20, 0, 25, 0, 200,
};

u32 gap_08_8041EB04_sbss;
s32 mpp;

s32 pouchCheckItem(s32 item);
PouchData* pouchGetPtr(void) {
    return (PouchData*)mpp;
}

#pragma optimize_for_size off
void pouchInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dest, int ch, u32 count);
    extern char* msgSearch(const char* msg);
    extern char* strcpy(char* dest, const char* src);
    extern const f32 float_0_8042187c;
    s32 i;
    s32 offset;
    s32 count;

    mpp = (s32)__memAlloc(0, 0x5D4);
    memset((void*)mpp, 0, 0x5D4);

    for (i = 0; i < 121; i++) ((PouchData*)mpp)->mKeyItems[i] = 0;
    *(s16*)(mpp + 0x192) = 0;
    *(s16*)(mpp + 0x194) = 0;
    *(s16*)(mpp + 0x196) = 0;
    *(s16*)(mpp + 0x198) = 0;
    *(s16*)(mpp + 0x19A) = 0;
    *(s16*)(mpp + 0x19C) = 0;
    *(s16*)(mpp + 0x19E) = 0;
    *(s16*)(mpp + 0x1A0) = 0;
    *(s16*)(mpp + 0x1A2) = 0;
    *(s16*)(mpp + 0x1A4) = 0;
    *(s16*)(mpp + 0x1A6) = 0;
    *(s16*)(mpp + 0x1A8) = 0;
    *(s16*)(mpp + 0x1AA) = 0;
    *(s16*)(mpp + 0x1AC) = 0;
    *(s16*)(mpp + 0x1AE) = 0;
    *(s16*)(mpp + 0x1B0) = 0;
    *(s16*)(mpp + 0x1B2) = 0;
    *(s16*)(mpp + 0x1B4) = 0;
    *(s16*)(mpp + 0x1B6) = 0;
    *(s16*)(mpp + 0x1B8) = 0;
    offset = 0;
    for (count = 0; count < 2; count++) {
        *(s16*)((u8*)mpp + 0x1BA + offset) = 0;
        *(s16*)((u8*)mpp + 0x1BC + offset) = 0;
        *(s16*)((u8*)mpp + 0x1BE + offset) = 0;
        *(s16*)((u8*)mpp + 0x1C0 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1C2 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1C4 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1C6 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1C8 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1CA + offset) = 0;
        *(s16*)((u8*)mpp + 0x1CC + offset) = 0;
        *(s16*)((u8*)mpp + 0x1CE + offset) = 0;
        *(s16*)((u8*)mpp + 0x1D0 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1D2 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1D4 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1D6 + offset) = 0;
        *(s16*)((u8*)mpp + 0x1D8 + offset) = 0;
        offset += 0x20;
    }

    offset = 0;
    for (count = 0; count < 0x19; count++) {
        *(s16*)((u8*)mpp + 0x1FA + offset) = 0;
        *(s16*)((u8*)mpp + 0x1FC + offset) = 0;
        *(s16*)((u8*)mpp + 0x1FE + offset) = 0;
        *(s16*)((u8*)mpp + 0x200 + offset) = 0;
        *(s16*)((u8*)mpp + 0x202 + offset) = 0;
        *(s16*)((u8*)mpp + 0x204 + offset) = 0;
        *(s16*)((u8*)mpp + 0x206 + offset) = 0;
        *(s16*)((u8*)mpp + 0x208 + offset) = 0;
        offset += 0x10;
    }

    offset = 0;
    for (count = 0; count < 0x19; count++) {
        *(s16*)((u8*)mpp + 0x38A + offset) = 0;
        *(s16*)((u8*)mpp + 0x38C + offset) = 0;
        *(s16*)((u8*)mpp + 0x38E + offset) = 0;
        *(s16*)((u8*)mpp + 0x390 + offset) = 0;
        *(s16*)((u8*)mpp + 0x392 + offset) = 0;
        *(s16*)((u8*)mpp + 0x394 + offset) = 0;
        *(s16*)((u8*)mpp + 0x396 + offset) = 0;
        *(s16*)((u8*)mpp + 0x398 + offset) = 0;
        offset += 0x10;
    }

    *(s16*)(mpp + 0x72) = 10;
    *(s16*)(mpp + 0x8E) = 10;
    *(s16*)(mpp + 0x76) = 5;
    *(s16*)(mpp + 0x90) = 5;
    *(s16*)(mpp + 0x92) = 3;
    *(s16*)(mpp + 0x94) = 3;
    *(s16*)(mpp + 0x70) = *(s16*)(mpp + 0x72);
    *(s16*)(mpp + 0x74) = *(s16*)(mpp + 0x76);
    *(s16*)(mpp + 0x7A) = 0;
    *(s16*)(mpp + 0x7E) = 0;
    *(s16*)(mpp + 0x7C) = 0;
    *(s16*)(mpp + 0x80) = 0;
    *(f32*)(mpp + 0x84) = float_0_8042187c;
    *(s16*)(mpp + 0x88) = 0;
    *(s16*)(mpp + 0x8A) = 1;
    *(s8*)(mpp + 0x98) = 0;
    *(s8*)(mpp + 0x99) = 0;

    for (i = 0; i < 8; i++) {
        PouchPartyData* party = &((PouchData*)mpp)->partyData[i];
        if (i != 0) {
            party->flags = 0;
            party->baseMaxHP = 10;
            party->currentMaxHP = party->baseMaxHP;
            party->currentHP = party->baseMaxHP;
            party->attackLevel = 0;
            party->techLevel = 0;
        }
    }

    strcpy((char*)(mpp + 0x5C0), msgSearch(str_name_party3_802cc728));
}
#pragma optimize_for_size reset

s16 pouchKeyItem(s32 index) {
    return *(s16*)(mpp + 0xA0 + index * 2);
}

ItemType16 pouchHaveItem(s32 index) {
    return *(ItemType16*)((s32)mpp + 0x192 + index * 2);
}

s16 pouchKeepItem(s32 index) {
    return *(s16*)(mpp + 0x1BA + index * 2);
}

s16 pouchHaveBadge(s32 index) {
    return *(s16*)(mpp + 0x1FA + index * 2);
}


s16 L_pouchEquipBadge(int param_1) {
    return *(s16*)(mpp + 0x38A + param_1 * 2);
}
s32 pouchGetHaveItemCnt(void) {
    s32 count;
    s32 loops;
    PouchData* pouch;

    count = 0;
    loops = 2;
    pouch = (PouchData*)mpp;

    for (; loops > 0; loops--) {
        if (pouch->mItems[0] != 0) {
            count++;
        }
        if (pouch->mItems[1] != 0) {
            count++;
        }
        if (pouch->mItems[2] != 0) {
            count++;
        }
        if (pouch->mItems[3] != 0) {
            count++;
        }
        if (pouch->mItems[4] != 0) {
            count++;
        }
        if (pouch->mItems[5] != 0) {
            count++;
        }
        if (pouch->mItems[6] != 0) {
            count++;
        }
        if (pouch->mItems[7] != 0) {
            count++;
        }
        if (pouch->mItems[8] != 0) {
            count++;
        }
        if (pouch->mItems[9] != 0) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}

#pragma optimize_for_size off
int pouchGetKeepItemCnt(void) {
    s32 i, count;
    for (i = 0, count = 0; i < 32; i++) {
        if (((PouchData*)mpp)->mStoredItems[i] != 0) count++;
    }
    return count;
}
#pragma optimize_for_size reset

s32 pouchGetHaveBadgeCnt(void) {
    PouchData* pouch;
    s32 count;
    s32 loops;

    pouch = (PouchData*)mpp;
    count = 0;
    loops = 0x14;

    for (; loops > 0; loops--) {
        if (pouch->mBadges[0] != 0) {
            count++;
        }
        if (pouch->mBadges[1] != 0) {
            count++;
        }
        if (pouch->mBadges[2] != 0) {
            count++;
        }
        if (pouch->mBadges[3] != 0) {
            count++;
        }
        if (pouch->mBadges[4] != 0) {
            count++;
        }
        if (pouch->mBadges[5] != 0) {
            count++;
        }
        if (pouch->mBadges[6] != 0) {
            count++;
        }
        if (pouch->mBadges[7] != 0) {
            count++;
        }
        if (pouch->mBadges[8] != 0) {
            count++;
        }
        if (pouch->mBadges[9] != 0) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
s32 pouchGetEquipBadgeCnt(void) {
    PouchData* pouch;
    s32 count;
    s32 loops;

    pouch = (PouchData*)mpp;
    count = 0;
    loops = 0x14;

    for (; loops > 0; loops--) {
        if (pouch->mEquippedBadges[0] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[1] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[2] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[3] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[4] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[5] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[6] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[7] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[8] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[9] != 0) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
s32 pouchGetEmptyHaveItemCnt(void) {
    s32 hasStrangeSack;
    s32 slots;
    s32 empty;
    PouchData* pouch;

    hasStrangeSack = pouchCheckItem(ITEM_STRANGE_SACK);

    slots = 10;
    if (hasStrangeSack != 0) {
        slots = 20;
    }

    empty = 0;
    pouch = (PouchData*)mpp;

    for (; slots > 0; slots--) {
        if (pouch->mItems[0] == 0) {
            empty++;
        }

        pouch = (PouchData*)((s32)pouch + 2);
    }

    return empty;
}

#pragma optimize_for_size off
int pouchGetEmptyKeepItemCnt(void) {
    s32 i, count;
    for (i = 0, count = 0; i < 32; i++) {
        if (((PouchData*)mpp)->mStoredItems[i] == 0) count++;
    }
    return count;
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 pouchGetItem(s32 itemNo) {
    extern void* yuwp;
    extern s32 marioGetParty(void);
    extern s32 pouchCheckItem(s32 item);
    extern void swSet(s32 id);
    s16* list;
    s16* equipped;
    s32 count;
    s32 i;
    s32 j;
    s32 partyId;
    s32 offset;
    s32 valid;

    if (itemNo == 0x79) {
        if (*(s16*)(mpp + 0x78) < 999) {
            *(s16*)(mpp + 0x78) += 1;
        }
        return 1;
    }
    if (itemNo == 0x7A) {
        if (*(s32*)((s32)yuwp + 8) < 99999) {
            *(s32*)((s32)yuwp + 8) += 1;
        }
        return 1;
    }
    if (itemNo == 0x7B) {
        partyId = marioGetParty();
        if (*(s16*)(mpp + 0x70) < *(s16*)(mpp + 0x72)) {
            *(s16*)(mpp + 0x70) += 1;
        }
        if (partyId != 0) {
            offset = partyId * 0xE;
            if (*(s16*)(mpp + offset + 6) < *(s16*)(mpp + offset + 2)) {
                *(s16*)(mpp + offset + 6) += 1;
            }
        }
        return 1;
    }
    if (itemNo == 0x7C) {
        if (*(s16*)(mpp + 0x74) < *(s16*)(mpp + 0x76)) {
            *(s16*)(mpp + 0x74) += 1;
        }
        return 1;
    }
    if (itemNo == 0x7D) {
        *(s16*)(mpp + 0x9A) += 1;
        return 1;
    }
    if (itemNo == 0x57) {
        *(s16*)(mpp + 0x9C) += 1;
        return 1;
    }

    if (itemNo == 6 || itemNo == 7 || itemNo == 8) {
        switch (itemNo) {
        case 6: *(s8*)(mpp + 0x98) = 1; break;
        case 7: *(s8*)(mpp + 0x98) = 2; break;
        case 8: *(s8*)(mpp + 0x98) = 3; break;
        }
    } else if (itemNo == 9 || itemNo == 10 || itemNo == 11) {
        switch (itemNo) {
        case 9: *(s8*)(mpp + 0x99) = 1; break;
        case 10: *(s8*)(mpp + 0x99) = 2; break;
        case 11: *(s8*)(mpp + 0x99) = 3; break;
        }
    } else {
        if (itemNo == 0x59) return 1;
        if (itemNo >= 0xF0 && itemNo < 0x153) swSet(itemNo - 0x70);
    }

    count = 0;
    list = 0;
    equipped = 0;
    if (itemNo >= 0x79 && itemNo < 0xEC) {
        list = ((PouchData*)mpp)->mItems;
        if (pouchCheckItem(1) == 0) {
            count = 10;
        } else {
            count = 20;
        }
        valid = 1;
    } else if (itemNo >= 1 && itemNo < 0x79) {
        list = ((PouchData*)mpp)->mKeyItems;
        equipped = 0;
        count = 0x79;
        valid = 1;
    } else if (itemNo >= 0xF0 && itemNo < 0x153) {
        list = ((PouchData*)mpp)->mBadges;
        equipped = ((PouchData*)mpp)->mEquippedBadges;
        count = 200;
        valid = 1;
    } else {
        valid = 0;
    }
    if (!valid) return 0;

    for (i = count - 1; i >= 0; i--) {
        if (list[i] == 0) {
            if (equipped) {
                for (j = i; j > 0; j--) {
                    list[j] = list[j - 1];
                    equipped[j] = equipped[j - 1];
                }
            } else {
                for (j = i; j > 0; j--) list[j] = list[j - 1];
            }
            list[0] = itemNo;
            if (equipped != 0) {
                equipped[0] = 0;
            }
            return 1;
        }
    }

    return 0;
}
#pragma optimize_for_size reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

s32 pouchCheckItem(s32 itemId) {
    s32 count;
    s32 loops;
    PouchData* pouch;

    if (((s32)itemId >= 0x79) && ((s32)itemId < 0xEC)) {
        count = 0;
        loops = 2;
        pouch = (PouchData*)mpp;

        for (; loops > 0; loops--) {
            if (itemId == pouch->mItems[0]) {
                count++;
            }
            if (itemId == pouch->mItems[1]) {
                count++;
            }
            if (itemId == pouch->mItems[2]) {
                count++;
            }
            if (itemId == pouch->mItems[3]) {
                count++;
            }
            if (itemId == pouch->mItems[4]) {
                count++;
            }
            if (itemId == pouch->mItems[5]) {
                count++;
            }
            if (itemId == pouch->mItems[6]) {
                count++;
            }
            if (itemId == pouch->mItems[7]) {
                count++;
            }
            if (itemId == pouch->mItems[8]) {
                count++;
            }
            if (itemId == pouch->mItems[9]) {
                count++;
            }

            pouch = (PouchData*)&pouch->partyData[1].currentHP;
        }

        return count;
    }

    if (((s32)itemId >= 1) && ((s32)itemId < 0x79)) {
        count = 0;
        loops = 0x79;
        pouch = (PouchData*)mpp;

        for (; loops > 0; loops--) {
            if (itemId == pouch->mKeyItems[0]) {
                count++;
            }

            pouch = (PouchData*)((s32)pouch + 2);
        }

        return count;
    }

    if (((s32)itemId >= 0xF0) && ((s32)itemId < 0x153)) {
        count = 0;
        loops = 0x14;
        pouch = (PouchData*)mpp;

        for (; loops > 0; loops--) {
            if (itemId == pouch->mBadges[0]) {
                count++;
            }
            if (itemId == pouch->mBadges[1]) {
                count++;
            }
            if (itemId == pouch->mBadges[2]) {
                count++;
            }
            if (itemId == pouch->mBadges[3]) {
                count++;
            }
            if (itemId == pouch->mBadges[4]) {
                count++;
            }
            if (itemId == pouch->mBadges[5]) {
                count++;
            }
            if (itemId == pouch->mBadges[6]) {
                count++;
            }
            if (itemId == pouch->mBadges[7]) {
                count++;
            }
            if (itemId == pouch->mBadges[8]) {
                count++;
            }
            if (itemId == pouch->mBadges[9]) {
                count++;
            }

            pouch = (PouchData*)&pouch->partyData[1].currentHP;
        }

        return count;
    }

    return 0;
}

#pragma optimize_for_size off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 pouchRemoveItem(s32 itemType) {
    s32 count = 0;
    s16* list = NULL;
    s16* equipped = NULL;
    s32 valid;
    s32 i, j;
    if ((s32)itemType >= 0x79 && (s32)itemType < 0xEC) {
        list = ((PouchData*)mpp)->mItems;
        equipped = NULL;
        if (pouchCheckItem(1)) count = 20;
        else count = 10;
        valid = 1;
    } else if ((s32)itemType >= 1 && (s32)itemType < 0x79) {
        equipped = NULL;
        count = 0x79;
        valid = 1;
        list = ((PouchData*)mpp)->mKeyItems;
    } else if ((s32)itemType >= 0xF0 && (s32)itemType < 0x153) {
        count = 200;
        valid = 1;
        list = ((PouchData*)mpp)->mBadges;
        equipped = ((PouchData*)mpp)->mEquippedBadges;
    } else valid = 0;
    if (!valid) return 0;
    for (i = 0; i < count; i++) {
        if (list[i] == (s32)itemType) {
            list[i] = 0;
            if (equipped) equipped[i] = 0;
            if (equipped) {
                for (j = i; j < count - 1; j++) {
                    list[j] = list[j + 1];
                    list[j + 1] = 0;
                    equipped[j] = equipped[j + 1];
                    equipped[j + 1] = 0;
                }
            } else {
                for (j = i; j < count - 1; j++) {
                    list[j] = list[j + 1];
                    list[j + 1] = 0;
                }
            }
            return 1;
        }
    }
    return 0;
}
#pragma optimize_for_size reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma optimize_for_size off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 pouchRemoveItemIndex(u32 itemType, int itemIndex) {
    s16* list = NULL;
    s16* equipped = NULL;
    s32 count = 0;
    s32 valid;
    s32 i, j;
    if ((s32)itemType >= 0x79 && (s32)itemType < 0xEC) {
        list = ((PouchData*)mpp)->mItems;
        equipped = NULL;
        if (pouchCheckItem(1)) count = 20;
        else count = 10;
        valid = 1;
    } else if ((s32)itemType >= 1 && (s32)itemType < 0x79) {
        equipped = NULL;
        count = 0x79;
        valid = 1;
        list = ((PouchData*)mpp)->mKeyItems;
    } else if ((s32)itemType >= 0xF0 && (s32)itemType < 0x153) {
        count = 200;
        valid = 1;
        list = ((PouchData*)mpp)->mBadges;
        equipped = ((PouchData*)mpp)->mEquippedBadges;
    } else valid = 0;
    if (!valid) return 0;
    if (list[itemIndex] != (s32)itemType) return 0;
    for (i = itemIndex; i < count; i++) {
        if (list[i] == (s32)itemType) {
            list[i] = 0;
            if (equipped) equipped[i] = 0;
            if (equipped) {
                for (j = i; j < count - 1; j++) {
                    list[j] = list[j + 1];
                    list[j + 1] = 0;
                    equipped[j] = equipped[j + 1];
                    equipped[j + 1] = 0;
                }
            } else {
                for (j = i; j < count - 1; j++) {
                    list[j] = list[j + 1];
                    list[j + 1] = 0;
                }
            }
            return 1;
        }
    }
    return 0;
}
#pragma optimize_for_size reset
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800d48b0(u16* src, u16* dst) {
    extern u32 strlen(const char* str);
    extern char* strcpy(char* dst, const char* src);
    s32 count;
    s32 i;
    u16 value;

    count = strlen((const char*)src) >> 1;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        u16* read = src;
        u16* write = dst;
        for (i = 0; i < count; i++) {
            value = *read;
            if (value >= 0x829F && value < 0x82DE) {
                value += 0xA1;
            } else if (value >= 0x82DE && value <= 0x82F1) {
                value += 0xA2;
            }
            *write = value;
            read++;
            write++;
        }
        *(u8*)((s32)dst + i * 2) = 0;
    } else {
        strcpy((char*)dst, (const char*)src);
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 comp_aiueo(short* param_1, short* param_2) {
    extern char* msgSearch(char* msg);
    extern u32 strlen(char* str);
    extern char* strcpy(char* dst, char* src);
    extern s32 strcmp(char* s1, char* s2);

    u16 name1[64];
    u16 name2[64];
    u16* src;
    u16* dst;
    s32 count;
    u16 value;
    s32 i;

    s32 id1 = *param_1;
    char** names = (char**)(itemDataTable + 4);
    s32 id2 = *param_2;
    src = (u16*)msgSearch(names[id1 * 10]);
    count = strlen((char*)src) >> 1;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        dst = name1;
        for (i = 0; i < count; i++) {
            value = *src;
            if (value >= 0x829F && value < 0x82DE) {
                value += 0xA1;
            } else if (value >= 0x82DE && value <= 0x82F1) {
                value += 0xA2;
            }
            *dst = value;
            src++;
            dst++;

        }
        *(u8*)(name1 + i) = 0;
    } else {
        strcpy((char*)name1, (char*)src);
    }

    src = (u16*)msgSearch(names[id2 * 10]);
    count = strlen((char*)src) >> 1;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        dst = name2;
        for (i = 0; i < count; i++) {
            value = *src;
            if (value >= 0x829F && value < 0x82DE) {
                value += 0xA1;
            } else if (value >= 0x82DE && value <= 0x82F1) {
                value += 0xA2;
            }
            *dst = value;
            src++;
            dst++;

        }
        *(u8*)(name2 + i) = 0;
    } else {
        strcpy((char*)name2, (char*)src);
    }
    return strcmp((char*)name1, (char*)name2);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

int comp_kind(short* param_1, short* param_2) {
    s16* kinds = (s16*)(itemDataTable + 0x12);
    s32 kind1 = kinds[*param_1 * 20];
    s32 kind2 = kinds[*param_2 * 20];
    if (kind1 > kind2) return 1;
    if (kind1 < kind2) return -1;
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 comp_aiueo_r(short* param_1, short* param_2) {
    extern char* msgSearch(char* msg);
    extern u32 strlen(char* str);
    extern char* strcpy(char* dst, char* src);
    extern s32 strcmp(char* s1, char* s2);

    u16 name1[64];
    u16 name2[64];
    u16* src;
    u16* dst;
    s32 count;
    u16 value;
    s32 i;

    s32 id1 = *param_1;
    char** names = (char**)(itemDataTable + 4);
    s32 id2 = *param_2;
    src = (u16*)msgSearch(names[id1 * 10]);
    count = strlen((char*)src) >> 1;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        dst = name1;
        for (i = 0; i < count; i++) {
            value = *src;
            if (value >= 0x829F && value < 0x82DE) {
                value += 0xA1;
            } else if (value >= 0x82DE && value <= 0x82F1) {
                value += 0xA2;
            }
            *dst = value;
            src++;
            dst++;

        }
        *(u8*)(name1 + i) = 0;
    } else {
        strcpy((char*)name1, (char*)src);
    }

    src = (u16*)msgSearch(names[id2 * 10]);
    count = strlen((char*)src) >> 1;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        dst = name2;
        for (i = 0; i < count; i++) {
            value = *src;
            if (value >= 0x829F && value < 0x82DE) {
                value += 0xA1;
            } else if (value >= 0x82DE && value <= 0x82F1) {
                value += 0xA2;
            }
            *dst = value;
            src++;
            dst++;

        }
        *(u8*)(name2 + i) = 0;
    } else {
        strcpy((char*)name2, (char*)src);
    }
    return strcmp((char*)name2, (char*)name1);
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

int comp_kind_r(short* param_1, short* param_2) {
    s16* kinds = (s16*)(itemDataTable + 0x12);
    s32 kind1 = kinds[*param_1 * 20];
    s32 kind2 = kinds[*param_2 * 20];
    if (kind1 < kind2) return 1;
    if (kind1 > kind2) return -1;
    return 0;
}


#pragma optimize_for_size off
void pouchSortItem(u32 sortType) {
    extern void qqsort(void* base, u32 count, u32 width, void* compare);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern s32 strcmp(const char* a, const char* b);
    extern char* msgSearch(const char* msg);
    extern s32 comp_aiueo(short* a, short* b);
    extern s32 comp_kind(short* a, short* b);
    extern s32 comp_aiueo_r(short* a, short* b);
    extern s32 comp_kind_r(short* a, short* b);
    s16 items[20];
    s16 keys[122];
    s16 badges[200];
    s16 equipped[200];
    PouchData* pouch = (PouchData*)mpp;
    s16* src;
    s16* dst;
    s16* equipSrc;
    s16* equipDst;
    s32 count;
    s32 i;
    s32 j;
    s32 remain;
    s32 type = (s32)sortType;

    if (sortType < 2 || type == 7 || type == 8) {
        count = 0;
        for (i = 0; i < 20; i++) {
            if (pouch->mItems[i] != 0) items[count++] = pouch->mItems[i];
        }
        for (i = count; i < 20; i++) items[i] = 0;
        if (type == 0) {
            qqsort(items, count, 2, comp_aiueo);
        } else if (type == 1) {
            qqsort(items, count, 2, comp_kind);
        } else if (type == 7) {
            qqsort(items, count, 2, comp_aiueo_r);
        } else if (type == 8) {
            qqsort(items, count, 2, comp_kind_r);
        }
        memcpy(pouch->mItems, items, 0x28);
    }

    if ((u32)(type - 2) < 2 || type == 9 || type == 10) {
        count = 0;
        for (i = 0; i < 121; i++) {
            if (pouch->mKeyItems[i] != 0) keys[count++] = pouch->mKeyItems[i];
        }
        for (i = count; i < 121; i++) keys[i] = 0;
        if (type == 2) {
            qqsort(keys, count, 2, comp_aiueo);
        } else if (type == 3) {
            qqsort(keys, count, 2, comp_kind);
        } else if (type == 9) {
            qqsort(keys, count, 2, comp_aiueo_r);
        } else if (type == 10) {
            qqsort(keys, count, 2, comp_kind_r);
        }
        memcpy(pouch->mKeyItems, keys, 0xF2);
    }

    if ((u32)(type - 4) > 2 && (u32)(type - 11) > 1 && type != 13) {
        return;
    }
    dst = badges;
    equipDst = equipped;
    count = 0;
    for (i = 0; i < 200; i++) {
        if (pouch->mBadges[i] != 0) {
            *dst++ = pouch->mBadges[i];
            count++;
            *equipDst++ = pouch->mEquippedBadges[i];
        }
    }
    for (i = count; i < 200; i++) {
        badges[i] = 0;
        equipped[i] = 0;
    }

    dst = badges;
    equipDst = equipped;
    for (i = 0; i < count - 1; i++, dst++, equipDst++) {
        j = i + 1;
        src = badges + j;
        equipSrc = equipped + j;
        for (; j < count; j++, src++, equipSrc++) {
            s16 a = *dst;
            s16 b = *src;
            u8* recordA = itemDataTable + a * 0x28;
            char* nameA = msgSearch(*(char**)(recordA + 4));
            u8* recordB = itemDataTable + b * 0x28;
            char* nameB = msgSearch(*(char**)(recordB + 4));
            s8 bpA = *(s8*)(recordA + 0x1C);
            s8 bpB = *(s8*)(recordB + 0x1C);
            s32 swap = 0;
            if (type == 4) {
                if (strcmp(nameA, nameB) > 0) swap = 1;
            } else if (type == 11) {
                if (strcmp(nameA, nameB) < 0) swap = 1;
            } else if (type == 5) {
                if (*(s16*)(recordB + 0x12) < *(s16*)(recordA + 0x12)) swap = 1;
            } else if (type == 12) {
                if (*(s16*)(recordA + 0x12) < *(s16*)(recordB + 0x12)) swap = 1;
            } else if (type == 6) {
                if (bpB < bpA) swap = 1;
                else if (bpA == bpB && *(s16*)(recordB + 0x12) < *(s16*)(recordA + 0x12)) swap = 1;
            } else if (type == 13) {
                if (bpA < bpB) swap = 1;
                else if (bpA == bpB && *(s16*)(recordA + 0x12) < *(s16*)(recordB + 0x12)) swap = 1;
            }
            if (swap) {
                s16 temp = *dst;
                s16 equipTemp = *equipDst;
                s16 otherEquip = *equipSrc;
                *dst = *src;
                *equipDst = otherEquip;
                *src = temp;
                *equipSrc = equipTemp;
            }
        }
    }
    memcpy(pouch->mBadges, badges, 400);
    memcpy(pouch->mEquippedBadges, equipped, 400);
}
#pragma optimize_for_size reset

s32 pouchGetCoin(void) {
    return ((PouchData*)mpp)->mCoins;
}

s32 pouchAddCoin(s16 coins) {
    ((PouchData*)mpp)->mCoins = ((PouchData*)mpp)->mCoins + coins;

    if (((PouchData*)mpp)->mCoins < 0) {
        ((PouchData*)mpp)->mCoins = 0;
    }

    if (((PouchData*)mpp)->mCoins > 999) {
        ((PouchData*)mpp)->mCoins = 999;
    }

    return ((PouchData*)mpp)->mCoins;
}
s32 pouchSetCoin(s16 value) {
    ((PouchData*)mpp)->mCoins = value;

    if (((PouchData*)mpp)->mCoins < 0) {
        ((PouchData*)mpp)->mCoins = 0;
    }

    if (((PouchData*)mpp)->mCoins > 999) {
        ((PouchData*)mpp)->mCoins = 999;
    }

    return ((PouchData*)mpp)->mCoins;
}
s32 pouchGetSuperCoin(void) {
    return ((PouchData*)mpp)->mShineSpriteCount;
}
s16 pouchSetSuperCoin(s16 newCount) {
    ((PouchData*)mpp)->mShineSpriteCount = newCount;

    return ((PouchData*)mpp)->mShineSpriteCount;
}
s32 pouchGetStarPiece(void) {
    return ((PouchData*)mpp)->mStarPieceCount;
}
s32 pouchAddStarPiece(s16 amount) {
    ((PouchData*)mpp)->mStarPieceCount += amount;

    return ((PouchData*)mpp)->mStarPieceCount;
}
s32 pouchAddHP(s32 amount) {
    ((PouchData*)mpp)->currentHP = ((PouchData*)mpp)->currentHP + (s16)amount;

    if (((PouchData*)mpp)->currentHP < 0) {
        ((PouchData*)mpp)->currentHP = 0;
    }

    if (((PouchData*)mpp)->currentHP > ((PouchData*)mpp)->maxHP) {
        ((PouchData*)mpp)->currentHP = ((PouchData*)mpp)->maxHP;
    }

    return ((PouchData*)mpp)->currentHP;
}
s32 pouchGetHP(void) {
    return ((PouchData*)mpp)->currentHP;
}
s32 pouchGetMaxHP(void) {
    return ((PouchData*)mpp)->maxHP;
}
void pouchSetHP(s16 hp) {
    ((PouchData*)mpp)->currentHP = hp;

    if (((PouchData*)mpp)->currentHP <= ((PouchData*)mpp)->maxHP) {
        return;
    }

    ((PouchData*)mpp)->currentHP = ((PouchData*)mpp)->maxHP;
}
void pouchSetMaxHP(s16 maxHP) {
    ((PouchData*)mpp)->maxHP = maxHP;
    ((PouchData*)mpp)->baseMaxHP = ((PouchData*)mpp)->maxHP;
}

s32 pouchGetPartyHP(s32 partyId) {
    s32 offset;
    s32 addr;

    offset = partyId * 0xE;
    addr = mpp;
    addr += offset;

    return *(s16*)(addr + 0x6);
}
void pouchSetPartyHP(s32 partyId, s16 hp) {
    PouchPartyData* party;
    s32 offset;
    s32 base;

    offset = partyId * 0xE;
    base = mpp;
    party = (PouchPartyData*)(base + offset);
    party->currentHP = hp;

    base = mpp;
    party = (PouchPartyData*)(base + offset);

    if (party->currentHP <= party->currentMaxHP) {
        return;
    }

    party->currentHP = party->currentMaxHP;
}
s32 pouchGetFP(void) {
    return ((PouchData*)mpp)->currentFP;
}
s32 pouchGetMaxFP(void) {
    return ((PouchData*)mpp)->maxFP;
}
void pouchSetFP(s16 fp) {
    ((PouchData*)mpp)->currentFP = fp;

    if (((PouchData*)mpp)->currentFP <= ((PouchData*)mpp)->maxFP) {
        return;
    }

    ((PouchData*)mpp)->currentFP = ((PouchData*)mpp)->maxFP;
}
void pouchSetMaxFP(s16 maxFP) {
    ((PouchData*)mpp)->maxFP = maxFP;
    ((PouchData*)mpp)->baseMaxFP = ((PouchData*)mpp)->maxFP;
}
s32 pouchGetAP(void) {
    return ((PouchData*)mpp)->currentSP;
}

void pouchAddAP(s16 amount) {
    ((PouchData*)mpp)->currentSP = ((PouchData*)mpp)->currentSP + amount;

    if (((PouchData*)mpp)->currentSP < 0) {
        ((PouchData*)mpp)->currentSP = 0;
    }

    if (((PouchData*)mpp)->currentSP <= ((PouchData*)mpp)->maxSP) {
        return;
    }

    ((PouchData*)mpp)->currentSP = ((PouchData*)mpp)->maxSP;
}
void pouchSetAP(s16 sp) {
    ((PouchData*)mpp)->currentSP = sp;

    if (((PouchData*)mpp)->currentSP < 0) {
        ((PouchData*)mpp)->currentSP = 0;
    }

    if (((PouchData*)mpp)->currentSP <= ((PouchData*)mpp)->maxSP) {
        return;
    }

    ((PouchData*)mpp)->currentSP = ((PouchData*)mpp)->maxSP;
}
s32 pouchGetMaxAP(void) {
    return ((PouchData*)mpp)->maxSP;
}
f32 pouchGetAudienceNum(void) {
    return ((PouchData*)mpp)->lastAudienceCount;
}
void pouchSetAudienceNum(f32 audienceNum) {
    ((PouchData*)mpp)->lastAudienceCount = audienceNum;
}
s32 pouchGetJumpLv(void) {
    s32 level;

    level = 0;

    if (pouchCheckItem(0x8) == 0) {
        goto check_super;
    }

    level = 3;
    goto done;

check_super:
    if (pouchCheckItem(0x7) == 0) {
        goto check_normal;
    }

    level = 2;
    goto done;

check_normal:
    if (pouchCheckItem(0x6) == 0) {
        goto done;
    }

    level = 1;

done:
    ((PouchData*)mpp)->jumpLevel = level;
    return ((PouchData*)mpp)->jumpLevel;
}
s32 pouchGetHammerLv(void) {
    s32 level;

    level = 0;

    if (pouchCheckItem(0xB) == 0) {
        goto check_super;
    }

    level = 3;
    goto done;

check_super:
    if (pouchCheckItem(0xA) == 0) {
        goto check_normal;
    }

    level = 2;
    goto done;

check_normal:
    if (pouchCheckItem(0x9) == 0) {
        goto done;
    }

    level = 1;

done:
    ((PouchData*)mpp)->hammerLevel = level;
    return ((PouchData*)mpp)->hammerLevel;
}
s32 pouchGetPartyAttackLv(s32 partyId) {
    return ((PouchData*)mpp)->partyData[partyId].attackLevel;
}
#pragma optimize_for_size off
s32 pouchAddKeepItem(s16 item) {
    int i, j;
    for (i = 31; i >= 0; i--) {
        if (((PouchData*)mpp)->mStoredItems[i] == 0) {
            for (j = i; j > 0; j--) {
                ((PouchData*)mpp)->mStoredItems[j] = ((PouchData*)mpp)->mStoredItems[j - 1];
            }
            ((PouchData*)mpp)->mStoredItems[0] = item;
            return 1;
        }
    }
    return 0;
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
s32 pouchRemoveKeepItem(s32 item, s32 index) {
    int i, j;
    s16* items = ((PouchData*)mpp)->mStoredItems;
    if (items[index] != item) return 0;
    for (i = index; i < 32; i++) {
        if (((PouchData*)mpp)->mStoredItems[i] == item) {
            items[i] = 0;
            for (j = i; j < 31; j++) {
                ((PouchData*)mpp)->mStoredItems[j] = ((PouchData*)mpp)->mStoredItems[j + 1];
                ((PouchData*)mpp)->mStoredItems[j + 1] = 0;
            }
            return 1;
        }
    }
    return 0;
}
#pragma optimize_for_size reset

void pouchReviseMarioParam(void) {
    PouchData* pouch;
    s32 offset;
    s32 count;
    s32 item_base;
    s32 item_offset;
    s32 item_addr;
    s16 badge;
    s8 bpCost;
    s32 index;

    pouch = (PouchData*)mpp;
    pouch->maxHP = pouch->baseMaxHP;

    pouch = (PouchData*)mpp;
    pouch->maxFP = pouch->baseMaxFP;

    offset = 0;
index = 0;
count = 100;

for (; count > 0; count--) {
    pouch = (PouchData*)mpp;
    badge = *(s16*)((s32)pouch + offset + 0x38A);

    if (badge == 0) {
        goto badge0_done;
    }
    if (badge == 0x117) {
        goto badge0_done;
    }
    if (badge >= 0x117) {
        goto badge0_ge_117;
    }
    if (badge >= 0x116) {
        goto badge0_add_hp;
    }
    goto badge0_done;

badge0_ge_117:
    if (badge >= 0x119) {
        goto badge0_done;
    }
    goto badge0_add_fp;

badge0_add_hp:
    pouch->maxHP = pouch->maxHP + 5;
    goto badge0_done;

badge0_add_fp:
    pouch->maxFP = pouch->maxFP + 5;

badge0_done:
    pouch = (PouchData*)mpp;
    badge = *(s16*)((s32)pouch + offset + 0x38C);

    if (badge == 0) {
        goto badge1_done;
    }
    if (badge == 0x117) {
        goto badge1_done;
    }
    if (badge >= 0x117) {
        goto badge1_ge_117;
    }
    if (badge >= 0x116) {
        goto badge1_add_hp;
    }
    goto badge1_done;

badge1_ge_117:
    if (badge >= 0x119) {
        goto badge1_done;
    }
    goto badge1_add_fp;

badge1_add_hp:
    pouch->maxHP = pouch->maxHP + 5;
    goto badge1_done;

badge1_add_fp:
    pouch->maxFP = pouch->maxFP + 5;

badge1_done:
    offset += 4;
    index++;
}

    pouch = (PouchData*)mpp;
    if (pouch->currentHP > pouch->maxHP) {
        pouch->currentHP = pouch->maxHP;
    }

    pouch = (PouchData*)mpp;
    if (pouch->currentFP > pouch->maxFP) {
        pouch->currentFP = pouch->maxFP;
    }

    pouch = (PouchData*)mpp;
    pouch->availableBP = pouch->totalBP;

    item_base = (s32)itemDataTable;
    offset = 0;
    count = 0x19;

    for (; count > 0; count--) {
        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x38A);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x38C);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x38E);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x390);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x392);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x394);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x396);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x398);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        offset += 0x10;
    }
}
#pragma optimize_for_size off
void pouchRevisePartyParam(void) {
    PouchPartyData* party;
    int i, count;
    for (i = 0, count = 0; i < 200; i++) {
        if (((PouchData*)mpp)->mEquippedBadges[i] == 0x117) count++;
    }
    for (i = 0; i < 8; i++) {
        if (i != 0) {
            party = &((PouchData*)mpp)->partyData[i];
            party->baseMaxHP = ((s16 (*)[4])_party_max_hp_table)[i][party->hpLevel];
            party->currentMaxHP = (s16)(party->baseMaxHP + count * 5);
            if (party->currentHP > party->currentMaxHP) party->currentHP = party->currentMaxHP;
        }
    }
}
#pragma optimize_for_size reset

s32 pouchGetStarPoint(void) {
    return ((PouchData*)mpp)->starPoints;
}
s32 pouchEquipBadgeIndex(s32 index) {
    ItemType16 badge;

    badge = ((PouchData*)mpp)->mBadges[index];

    if (badge == 0) {
        return 0;
    }

    ((PouchData*)mpp)->mEquippedBadges[index] = badge;
    return 1;
}
s32 pouchUnEquipBadgeIndex(s32 index) {
    s32 offset;
    s32 addr;
    s32 badge;

    offset = index * 2;
    addr = mpp;
    addr += offset;

    badge = *(s16*)(addr + 0x38A);

    if (badge == 0) {
        return 0;
    }

    *(s16*)(addr + 0x38A) = 0;
    return 1;
}
s32 pouchEquipCheckBadge(s32 itemId) {
    PouchData* pouch;
    s32 count;
    s32 i;

    pouch = (PouchData*)mpp;
    count = 0;

    for (i = 0; i != 0xB4; i += 9) {
        if (itemId == pouch->mEquippedBadges[0]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[1]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[2]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[3]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[4]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[5]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[6]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[7]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[8]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[9]) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
u32 pouchEquipCheckBadgeIndex(s32 index) {
    s32 offset;
    s32 badge;
    s32 negBadge;

    offset = index * 2;
    index = mpp + offset;

    badge = *(s16*)(index + 0x38A);
    negBadge = -badge;

    return (u32)(negBadge | badge) >> 31;
}
s32 pouchEquipBadgeID(s32 itemId) {
    PouchData* scan;
    PouchData* pouch;
    s32 index;
    s32 count;
    s16 badge;

    pouch = (PouchData*)mpp;
    count = 200;
    index = 0;
    scan = pouch;

    for (; count > 0; count--) {
        if ((s32)scan->mBadges[0] == itemId) {
            badge = pouch->mBadges[index];

            if (badge != 0) {
                pouch->mEquippedBadges[index] = badge;
            }

            return 1;
        }

        scan = (PouchData*)((s32)scan + 2);
        index++;
    }

    return 0;
}
s32 N_pouchUnEquipBadgeID(s32 badgeID) {
    PouchData* scan;
    PouchData* pouch;
    s32 index;
    s32 count;
    s16 badge;

    pouch = (PouchData*)mpp;
    count = 200;
    index = 0;
    scan = pouch;

    for (; count > 0; count--) {
        if ((s32)scan->mEquippedBadges[0] == badgeID) {
            badge = pouch->mEquippedBadges[index];

            if (badge != 0) {
                pouch->mEquippedBadges[index] = 0;
            }

            return 1;
        }

        scan = (PouchData*)((s32)scan + 2);
        index++;
    }

    return 0;
}

void pouchArriveBadge(s32 badge) {
    extern void* bdsw;
    extern void badgeShop_add(void* work, s16 badge, s32 flag);
    badgeShop_add(bdsw, badge, 1);
}

void pouchMajinaiInit(s32 value) {
    if (*(u8*)(mpp + 0x5B8) < value) {
        *(u8*)(mpp + 0x5B8) = value;
        *(s8*)(mpp + 0x5B9) = -1;
        *(u8*)(mpp + 0x5BA) = 0;
    }
}

s32 pouchGetKpaCoin(void) {
    return *(u8*)(mpp + 0x5BB);
}

s32 pouchAddKpaCoin(s32 value) {
    s32 ret = 0;
    *(u8*)(mpp + 0x5BB) += value;
    if (*(u8*)(mpp + 0x5BB) >= 100) {
        *(u8*)(mpp + 0x5BB) -= 100;
        ret = 1;
    }
    return ret;
}

s32 pouchGetKpaScore(void) {
    return *(s32*)(mpp + 0x5BC);
}

void pouchAddKpaScore(s32 value) {
    *(u32*)(mpp + 0x5BC) += value;
    if (*(u32*)(mpp + 0x5BC) > 999999) {
        *(u32*)(mpp + 0x5BC) = 999999;
    }
}

void pouchGetStarStone(s32 id) {
    extern const f32 float_2_80421878;

    if (*(u16*)(mpp + 0x8C) == 0) {
        *(f32*)(mpp + 0x84) = float_2_80421878;
    }

    *(u16*)(mpp + 0x8C) |= 1 << id;
    *(s16*)(mpp + 0x7C) = (id + 1) * 100;

    if (*(s16*)(mpp + 0x7C) < 0) {
        *(s16*)(mpp + 0x7C) = 0;
    }

    if (*(s16*)(mpp + 0x7C) > 0x320) {
        *(s16*)(mpp + 0x7C) = 0x320;
    }
}

#pragma optimize_for_size off
int pouchReceiveMailCount(void) {
    typedef struct MailFlagsView { u8 pad[0x580]; u32 received[4]; } MailFlagsView;
    s32 i, count;
    for (i = 0, count = 0; i < 99; i++) {
        if (((MailFlagsView*)mpp)->received[i / 32] & (1 << (i % 32))) count++;
    }
    return count;
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void pouchReceiveMail(s32 mail) {
    typedef struct MailPouchView {
        u8 pad[0x51A];
        u8 mailIds[0x66];
        u32 received[4];
    } MailPouchView;
    s32 count;
    s32 i;
    MailPouchView* pouch;
    s32 index;
    s32 shift;

    shift = mail % 32;
    index = mail / 32;
    count = 0;
    pouch = (MailPouchView*)mpp;
    for (i = count; i < 99; i++) {
        if (pouch->received[i / 32] & (1 << (i % 32))) count++;
    }
    pouch->received[index] |= 1 << shift;
    ((MailPouchView*)mpp)->mailIds[count] = (u8)mail;
}
#pragma optimize_for_size reset

void pouchOpenMail(s32 mail) {
    s32 word = mail / 32;
    s32 bit = mail % 32;
    *(u32*)(mpp + 0x590 + word * 4) |= 1 << bit;
}

s32 pouchCheckMail(s32 mail) {
    typedef struct MailFlagsView {
        u8 pad[0x580];
        u32 received[4];
        u32 read[4];
    } MailFlagsView;
    s32 mask = 1 << (mail % 32);
    u32 value;
    if (((MailFlagsView*)mpp)->read[mail / 32] & mask) {
        return 2;
    } else {
        value = ((MailFlagsView*)mpp)->received[mail / 32] & mask;
        return (-value | value) >> 31;
    }
}

s32 pouchGetPartyColor(s32 partyId) {
    return *(u16*)(mpp + partyId * 0xE) >> 13;
}

void pouchSetPartyColor(int partyId, int color) {
    u16* flags = (u16*)&((PouchData*)mpp)->partyData[partyId].flags;
    u16 value = *flags;

    value &= 0x1FFF;
    value |= color << 13;
    *flags = value;
}

void pouchSetYoshiName(char* name) {
    extern char* strcpy(char* dest, const char* src);
    strcpy((char*)(mpp + 0x5C0), name);
}

char* pouchGetYoshiName(void) {
    extern u32 strlen(const char* str);
    extern char* msgSearch(const char* msg);
    if (strlen((char*)(mpp + 0x5C0)) < 1) {
        return msgSearch(str_name_party3_802cc728);
    }
    return (char*)(mpp + 0x5C0);
}

const f32 float_2_80421878 = 2.0f;
const f32 float_0_8042187c = 0.0f;
