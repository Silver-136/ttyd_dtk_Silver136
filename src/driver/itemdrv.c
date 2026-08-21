#include "driver/itemdrv.h"
#include "driver/seqdrv.h"
extern void* gp;
static u8 work[56];

extern u8 itemDataTable[];

extern s32 winMgrAction(void* win);
extern void winMgrHelpDraw(void* win);
extern s32 winMgrHelpMain(void* win);
extern void* msgSearch(void* msgId);

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

s32 itemGetCheck(void* itemEntry) {
    extern void* marioGetPtr(void);
    extern s32 marioStGetSystemLevel(void);
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

u32 itemGetNokoCheck(void* itemEntry) {
    extern void* marioGetPtr(void);
    extern s32 marioStGetSystemLevel(void);
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

u8 itemMain(void) {
    extern void* gp;
    extern void* marioGetPtr(void);
    extern void* itemHitCheck(f64, f64, f64, f64);
    extern s32 itemGetCheck(void*);
    extern void itemseq_GetItem(void*);
    extern void itemseq_Bound(void*);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void iconSetPos(f64, f64, f64, char*);
    extern void iconSetScale(f64, char*);
    extern void iconSetAlpha(char*, u8);
    extern void iconDelete(char*);
    extern s32 strcmp(char*, char*);
    extern void pouchArriveBadge(s16, char*);
    extern void effSoftDelete(void*);
    extern void* effHagetakaFlushN64Entry(s32, f32, f32, f32, f32);
    extern s32 irand(s32);
    extern void* hitCheckFilter(f64, f64, f64, f64, f64, f64, s32, void*, void*, void*, void*, void*, void*, void*);
    extern f32 float_0p5_804210d0, float_12_80421128;
    extern f32 float_16_804210e0, float_neg1000_80421108;
    extern f32 float_0_804210b4, float_20_804210b8, float_neg1_804210c4;
    void* workSet = work;
    void* otherWork;
    void* player;
    u8* item;
    u8* scan;
    void* party;
    s32 count, i, j;
    u16 mode;

    if (*(s32*)((s32)gp + 0x14) != 0) workSet = work + 0x1C;
    player = marioGetPtr();
    item = itemHitCheck(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x90),
                        *(f32*)((s32)player + 0x94),
                        float_0p5_804210d0 * *(f32*)((s32)player + 0x1B8) + float_12_80421128);
    if (itemGetCheck(item) != 0) {
        *(u16*)(item + 0x24) = 2;
        *(u16*)(item + 0x26) = 0;
    }
    count = *(s32*)workSet;
    item = *(u8**)((s32)workSet + 4);
    for (i = 0; i < count; i++, item += 0x98) {
        s32 update = 1;
        if ((*(u16*)item & 1) == 0) continue;
        mode = *(u16*)(item + 0x24);
        if (mode == 0 || mode == 1 || (mode >= 3 && mode < 7) || mode == 9 || mode > 10) {
            if ((*(u32*)((s32)gp + 0x18) & 0xF) != 0) update = 0;
        }
        if (!update) {
            *(u32*)(item + 0x68) = *(u32*)((s32)gp + 0x38);
            *(u32*)(item + 0x6C) = *(u32*)((s32)gp + 0x3C);
            continue;
        }
        if (mode == 2) {
            *(u32*)(item + 0x38) &= ~0x100;
        } else if (*(s64*)(item + 0x78) == 0) {
            *(u32*)(item + 0x38) &= ~0x100;
        } else if ((*(u32*)(item + 0x38) & 0x4000) == 0) {
            s64 elapsed = ((*(s64*)((s32)gp + 0x38) - *(s64*)(item + 0x68)) * 8) /
                          (*(u32*)0x800000F8 / 500000);
            *(s64*)(item + 0x78) = (s64)((f32)*(s64*)(item + 0x78) - (f32)elapsed);
            if (*(s64*)(item + 0x78) <= 0) {
                *(u16*)(item + 0x24) = 9;
                *(u16*)(item + 0x26) = 0;
            } else if (*(s64*)(item + 0x78) < 3000 && ((*(s64*)(item + 0x78) >> 4) & 1)) {
                *(u32*)(item + 0x38) ^= 0x100;
            }
        } else {
            *(u32*)(item + 0x38) &= ~0x100;
        }
        switch (mode) {
        case 1: {
            f32 oldY = *(f32*)(item + 0x40);
            f32 nextY;
            f32 distance;
            f32 normalX, normalY, normalZ;
            u32 hitWork[2];
            void* hit = 0;
            if ((*(u32*)(item + 0x38) & 0x100000) == 0) {
                distance = 21.0f;
                hit = hitCheckFilter(*(f32*)(item + 0x3C), oldY + float_20_804210b8,
                                     *(f32*)(item + 0x44), float_0_804210b4,
                                     float_neg1_804210c4, float_0_804210b4, 0,
                                     hitWork, &nextY, &normalX, &distance, &normalY, 0, &normalZ);
            }
            if (hit == 0) {
                *(u16*)(item + 0x24) = 3;
                *(u16*)(item + 0x26) = 0;
            } else {
                *(f32*)(item + 0x40) = nextY;
            }
            break;
        }
        case 2:
            itemseq_GetItem(item);
            break;
        case 3: case 4: case 5: case 6: case 10:
            itemseq_Bound(item);
            break;
        case 8:
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                *(u16*)(item + 0x24) = 3;
                *(u16*)(item + 0x26) = 0;
                *(u32*)(item + 0x38) &= ~0x1000;
            } else {
                *(f32*)(item + 0x3C) = *(f32*)((s32)party + 0xC);
                *(f32*)(item + 0x40) = *(f32*)((s32)party + 0x10) + 15.0f;
                *(f32*)(item + 0x44) = *(f32*)((s32)party + 0x14);
            }
            break;
        case 9:
            scan = *(u8**)((s32)workSet + 4);
            for (j = 0; j < *(s32*)workSet; j++, scan += 0x98) {
                if (strcmp((char*)(scan + 0xC), (char*)(item + 0xC)) == 0) {
                    if (*(s32*)((s32)gp + 0x14) == 0 && *(s32*)(scan + 4) > 0xEF &&
                        *(s32*)(scan + 4) < 0x153 && (*(u32*)(scan + 0x38) & 0x1000000)) {
                        pouchArriveBadge(*(s16*)(scan + 4), (char*)(item + 0xC));
                    }
                    if (*(s32*)(scan + 4) > 0x71 && *(s32*)(scan + 4) < 0x79) {
                        effSoftDelete(*(void**)(scan + 0x1C));
                    } else {
                        iconDelete((char*)(scan + 0xC));
                    }
                    *(u16*)scan &= ~1;
                }
            }
            break;
        }
        if ((*(u16*)item & 1) != 0) {
            if (*(s32*)(item + 4) > 0x71 && *(s32*)(item + 4) < 0x79) {
                void* eff = *(void**)(item + 0x1C);
                u8* alloc = *(u8**)((s32)eff + 4);
                *(f32*)(alloc + 8) = *(f32*)(item + 0x3C);
                *(f32*)(alloc + 0xC) = (*(u32*)(item + 0x38) & 0x100)
                                                ? float_neg1000_80421108
                                                : *(f32*)(item + 0x40) + float_16_804210e0;
                *(f32*)(alloc + 0x10) = *(f32*)(item + 0x44);
            } else if ((*(u32*)(item + 0x38) & 0x100) == 0) {
                iconSetPos(*(f32*)(item + 0x3C), *(f32*)(item + 0x40),
                           *(f32*)(item + 0x44), (char*)(item + 0xC));
                iconSetScale(*(f32*)(item + 0x48), (char*)(item + 0xC));
                iconSetAlpha((char*)(item + 0xC), *(u8*)(item + 0x54));
            } else {
                iconSetPos(*(f32*)(item + 0x3C), float_neg1000_80421108,
                           *(f32*)(item + 0x44), (char*)(item + 0xC));
            }
            *(u32*)(item + 0x68) = *(u32*)((s32)gp + 0x38);
            *(u32*)(item + 0x6C) = *(u32*)((s32)gp + 0x3C);
            if ((*(s32*)(item + 4) == 0x79 || *(s32*)(item + 4) == 0x7A) && irand(1000) < 30) {
                f32 dx = (f32)(10 - irand(20));
                f32 dy = (f32)(irand(20) + 10);
                effHagetakaFlushN64Entry(0, *(f32*)(item + 0x3C) + dx,
                    *(f32*)(item + 0x40) + dy, *(f32*)(item + 0x44), 1.0f);
            }
        }
    }

    otherWork = (*(s32*)((s32)gp + 0x14) != 0) ? (void*)work : (void*)(work + 0x1C);
    item = *(u8**)((s32)otherWork + 4);
    for (i = 0; i < *(s32*)otherWork; i++, item += 0x98) {
        if ((*(u32*)item & 0x10000) != 0) {
            *(u32*)(item + 0x68) = *(u32*)((s32)gp + 0x38);
            *(u32*)(item + 0x6C) = *(u32*)((s32)gp + 0x3C);
        }
    }
    return 0;
}

void itemModeChange(void* item, u16 mode) {
    *(u16*)((s32)item + 0x24) = mode;
    *(u16*)((s32)item + 0x26) = 0;
}


s32 itemEntry(s32 nameArg, s32 itemId, s32 mode, s32 collectExpr, s32 script, f32 x, f32 y, f32 z) {
    extern void* gp;
    extern u8 itemDataTable[];
    extern u32 swGet(s32 id);
    extern u32 _swGet(s32 id);
    extern s32 sprintf(char* dst, char* fmt, ...);
    extern s32 strcmp(char* a, char* b);
    extern void* memset(void* dst, s32 value, u32 size);
    extern char* strcpy(char* dst, char* src);
    extern void* effStarStoneEntry(f64 x, f64 y, f64 z, f64 scale, s32 type);
    extern void iconEntry(char* name, u16 icon);
    extern void iconSetPos(f64 x, f64 y, f64 z, char* name);
    extern void iconSetScale(f64 scale, char* name);
    extern void iconSetAlpha(char* name, u8 alpha);
    extern void iconDelete(char* name);
    extern void effSoftDelete(void* effect);
    extern void pouchArriveBadge(s32 itemId);
    extern f32 float_1_804210bc;
    extern f32 float_16_804210e0;
    extern char str_iPCT05x_802c9258;

    void* workSet = work;
    void* entry;
    char generated[12];
    char* name = (char*)nameArg;
    s32 count;
    s32 i;
    s32 wantedId;
    s32 collectedBySw = 0;
    s32 collectedByLocal = 0;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }
    if (collectExpr != -1) {
        if (collectExpr >= -130000000 && collectExpr <= -120000000) {
            collectedBySw = 1;
            collectExpr += 130000000;
            if (swGet(collectExpr) != 0) return 0;
        } else if (collectExpr > -110000001 && collectExpr < -99999999) {
            collectedByLocal = 1;
            collectExpr += 110000000;
            if (_swGet(collectExpr) != 0) return 0;
        }
    }
    if (name == 0) {
        s32 serial = *(s32*)((s32)workSet + 8) + 1;
        if (serial > 0xFFFFE) serial = 1;
        *(s32*)((s32)workSet + 8) = serial;
        sprintf(generated, &str_iPCT05x_802c9258, serial);
        name = generated;
    }

    count = *(s32*)workSet;
    entry = *(void**)((s32)workSet + 4);
    for (i = 0; i < count; i++, entry = (void*)((s32)entry + 0x98)) {
        if ((*(u16*)entry & 1) == 0) break;
        if ((*(u16*)entry & 0x10000) != 0 && strcmp((char*)((s32)entry + 0xC), name) == 0) break;
    }
    if (i >= count) {
        void* sacrifice = 0;
        void* scan;

        wantedId = 0x79;
        for (;;) {
            scan = *(void**)((s32)workSet + 4);
            for (i = 0; i < count; i++, scan = (void*)((s32)scan + 0x98)) {
                if ((*(u16*)scan & 1) != 0 && *(s32*)((s32)scan + 4) == wantedId &&
                    (*(u16*)scan & 0x400) != 0) {
                    sacrifice = scan;
                    break;
                }
            }
            if (sacrifice != 0 || wantedId == 0x7B) {
                break;
            }
            wantedId = wantedId == 0x79 ? 0x7C : 0x7B;
        }

        if (sacrifice == 0) {
            return 0;
        }

        scan = *(void**)((s32)workSet + 4);
        for (i = 0; i < count; i++, scan = (void*)((s32)scan + 0x98)) {
            if (strcmp((char*)((s32)scan + 0xC), (char*)((s32)sacrifice + 0xC)) == 0) {
                s32 oldItemId = *(s32*)((s32)scan + 4);

                if (*(s32*)((s32)gp + 0x14) == 0 && oldItemId >= 0xF0 && oldItemId < 0x153 &&
                    (*(u16*)scan & 0x100) != 0) {
                    pouchArriveBadge(oldItemId);
                }
                if (oldItemId > 0x71 && oldItemId < 0x79) {
                    effSoftDelete(*(void**)((s32)scan + 0x84));
                } else {
                    iconDelete((char*)((s32)scan + 0xC));
                }
                *(u16*)scan &= (u16)~1;
            }
        }
        entry = sacrifice;
    }

    memset(entry, 0, 0x98);
    strcpy((char*)((s32)entry + 0xC), name);
    *(f32*)((s32)entry + 0x3C) = x;
    *(f32*)((s32)entry + 0x40) = y;
    *(f32*)((s32)entry + 0x44) = z;
    *(s32*)((s32)entry + 0x4) = itemId;
    *(s16*)((s32)entry + 2) = mode;
    *(void**)((s32)entry + 0x20) = (void*)script;
    *(u16*)entry |= 1;
    *(f32*)((s32)entry + 0x48) = float_1_804210bc;
    *(u8*)((s32)entry + 0x54) = 0xFF;
    if (collectExpr == -1) {
        *(s32*)((s32)entry + 8) = -1;
    } else if (collectedBySw) {
        *(s32*)((s32)entry + 8) = collectExpr;
        *(u16*)entry |= 0x20;
    } else if (collectedByLocal) {
        *(s32*)((s32)entry + 8) = collectExpr;
        *(u16*)entry |= 0x40;
    }

    if (itemId > 0x71 && itemId < 0x79) {
        *(void**)((s32)entry + 0x84) = effStarStoneEntry(x, y + float_16_804210e0, z, float_1_804210bc, itemId - 0x72);
    } else {
        *(void**)((s32)entry + 0x84) = 0;
        iconEntry((char*)((s32)entry + 0xC), *(u16*)((s32)itemDataTable + itemId * 0x28 + 0x20));
        iconSetPos(x, y, z, (char*)((s32)entry + 0xC));
        iconSetScale(*(f32*)((s32)entry + 0x48), (char*)((s32)entry + 0xC));
        iconSetAlpha((char*)((s32)entry + 0xC), *(u8*)((s32)entry + 0x54));
    }
    *(f32*)((s32)entry + 0x58) = float_1_804210bc;
    *(f32*)((s32)entry + 0x5C) = float_1_804210bc;
    if (itemId > 0x78 && itemId < 0x7D) *(u16*)entry |= 0x80;

    *(u16*)((s32)entry + 0x24) = 1;
    *(u16*)((s32)entry + 0x26) = 0;
    switch ((u16)mode) {
        case 1: *(u16*)entry |= 2; break;
        case 2: *(u16*)((s32)entry + 0x24) = 5; break;
        case 3: *(s32*)((s32)entry + 0x68) = 0; *(s32*)((s32)entry + 0x6C) = 5000; *(u16*)((s32)entry + 0x24) = 5; break;
        case 4: *(u16*)((s32)entry + 0x24) = 6; break;
        case 6: *(u32*)((s32)entry + 0x38) |= 0x40; goto timer5000;
        case 7: *(u32*)((s32)entry + 0x38) |= 0x4000;
        case 5:
timer5000: *(s32*)((s32)entry + 0x68) = 0; *(s32*)((s32)entry + 0x6C) = 5000; *(u16*)((s32)entry + 0x24) = 6; break;
        case 9: *(u16*)entry |= 0x100;
        case 8: *(u32*)((s32)entry + 0x38) |= 0x4000; *(s32*)((s32)entry + 0x68) = 0; *(s32*)((s32)entry + 0x6C) = 8000; *(u16*)((s32)entry + 0x24) = 6; break;
        case 10: *(u16*)entry |= 0x400; *(u32*)((s32)entry + 0x38) |= 0x4000; *(s32*)((s32)entry + 0x68) = 0; *(s32*)((s32)entry + 0x6C) = 8000; *(u16*)((s32)entry + 0x24) = 6; break;
        case 11: case 12: *(u32*)((s32)entry + 0x38) |= 0x4000; *(s32*)((s32)entry + 0x68) = 0; *(s32*)((s32)entry + 0x6C) = 20000; *(u16*)((s32)entry + 0x24) = 6; if (mode == 12) *(u16*)entry |= 0x400; break;
        case 13: *(u16*)((s32)entry + 0x24) = 3; break;
        case 14: *(u16*)((s32)entry + 0x24) = 4; break;
        case 15: *(u16*)((s32)entry + 0x24) = 10; break;
        case 17: *(u16*)entry |= 2;
        case 16: *(u16*)((s32)entry + 0x24) = 7; break;
    }
    return (s32)entry;
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

void* itemHitCheck(f64 posX, f64 posY, f64 posZ, f64 radius) {
    extern void* marioGetPtr(void);
    extern s32 marioItemGetOk(void);
    extern s32 marioGetJabaraState(void);
    extern s32 hitCheckAttr(f64 x, f64 y, f64 z, f64 dirX, f64 dirY, f64 dirZ,
                            u32 attr, void* hitOut, void* hitX, void* hitY,
                            void* radius, void* outX, void* outY, void* outZ);
    extern f64 __frsqrte(f64 value);
    extern f32 __float_nan;
    extern f32 float_0_804210b4;
    extern f32 float_0p5_804210d0;
    extern f32 float_1_804210bc;
    extern f32 float_10_804210c8;

    void* workSet = work;
    void* item;
    void* best = 0;
    void* mario;
    s32 count;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distXZ;
    f32 dist;
    f32 height;
    f32 itemY;
    f32 yDiff;
    f32 hitRadius;
    u32 hitWork[4];
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    s32 blocked;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x1C);
    }

    mario = marioGetPtr();
    count = *(s32*)workSet;
    item = *(void**)((s32)workSet + 4);

    for (i = 0; i < count; i++, item = (void*)((s32)item + 0x98)) {
        if (((*(u16*)item & 1) != 0) &&
            ((*(u16*)item & 2) == 0) &&
            ((*(u32*)((s32)item + 0x38) & 0x1000) == 0) &&
            (*(u16*)((s32)item + 0x24) != 2)) {
            dx = (f32)(posX - (f64)*(f32*)((s32)item + 0x3C));
            dz = (f32)(posZ - (f64)*(f32*)((s32)item + 0x44));
            dy = (f32)(posY - (f64)*(f32*)((s32)item + 0x40));
            {
                f32 value = dx * dx + dz * dz;
                if (value > float_0_804210b4) {
                    f64 d = value;
                    f64 inv = __frsqrte(d);
                    inv = 0.5 * inv * -(d * inv * inv - 3.0);
                    inv = 0.5 * inv * -(d * inv * inv - 3.0);
                    distXZ = (f32)(d * 0.5 * inv * -(d * inv * inv - 3.0));
                } else if ((f64)value < 0.0) {
                    distXZ = __float_nan;
                } else {
                    f32 classifyValue = value;
                    u32 bits = *(u32*)&classifyValue;
                    u32 kindBits = bits & 0x7F800000;
                    s32 kind;
                    if (kindBits == 0x7F800000) {
                        kind = (bits & 0x7FFFFF) == 0 ? 2 : 1;
                    } else if (kindBits < 0x7F800000 && kindBits == 0) {
                        kind = (bits & 0x7FFFFF) == 0 ? 3 : 5;
                    } else {
                        kind = 4;
                    }
                    distXZ = kind == 1 ? __float_nan : value;
                }
            }
            {
                f32 value = dx * dx + dy * dy + dz * dz;
                if (value > float_0_804210b4) {
                    f64 d = value;
                    f64 inv = __frsqrte(d);
                    inv = 0.5 * inv * -(d * inv * inv - 3.0);
                    inv = 0.5 * inv * -(d * inv * inv - 3.0);
                    dist = (f32)(d * 0.5 * inv * -(d * inv * inv - 3.0));
                } else if ((f64)value < 0.0) {
                    dist = __float_nan;
                } else {
                    f32 classifyValue = value;
                    u32 bits = *(u32*)&classifyValue;
                    u32 kindBits = bits & 0x7F800000;
                    s32 kind;
                    if (kindBits == 0x7F800000) {
                        kind = (bits & 0x7FFFFF) == 0 ? 2 : 1;
                    } else if (kindBits < 0x7F800000 && kindBits == 0) {
                        kind = (bits & 0x7FFFFF) == 0 ? 3 : 5;
                    } else {
                        kind = 4;
                    }
                    dist = kind == 1 ? __float_nan : value;
                }
            }

            if (((f64)distXZ < radius) &&
                (marioItemGetOk() != 0) &&
                (marioGetJabaraState() != 1) &&
                (marioGetJabaraState() != 4) &&
                (marioGetJabaraState() != 5)) {
                height = *(f32*)((s32)mario + 0xFC);
                itemY = *(f32*)((s32)item + 0x40);
                yDiff = (f32)((float_0p5_804210d0 * height + (f32)posY) - itemY);
                if (yDiff < float_0_804210b4) {
                    yDiff = -yDiff;
                }
                if (yDiff < (float_0p5_804210d0 * height + float_10_804210c8)) {
                    blocked = 0;
                    if (posY < (f64)itemY) {
                        hitRadius = float_10_804210c8 + (itemY - (f32)posY);
                        if (float_0_804210b4 < hitRadius) {
                            blocked = hitCheckAttr(posX, posY, posZ,
                                                   (f64)float_0_804210b4,
                                                   (f64)float_1_804210bc,
                                                   (f64)float_0_804210b4,
                                                   0x80000000,
                                                   hitWork, &hitX, &hitY,
                                                   &hitRadius, &hitX, &hitY, &hitZ);
                        }
                    }
                    if (blocked == 0) {
                        best = item;
                        radius = (f64)dist;
                    }
                }
            }
        }
    }

    return best;
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

s32 winHelpMain(void* win) {
    return winMgrHelpMain(win);
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

void itemseq_GetItem(void* item, s32 p2, s32 p3, s32 p4, u32* p5, u32 p6) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32);
    extern s32 pouchGetItem(s32);
    extern void pouchRemoveItem(s32);
    extern void swSet(u32);
    extern void _swSet(u32);
    extern u32 swGet(u32);
    extern void marioKeyOff(void);
    extern void marioCtrlOff(void);
    extern void marioCtrlOn(void);
    extern void marioKeyOn(void);
    extern void marioStSystemLevel(s32);
    extern void psndSetFlag(u32);
    extern void psndClearFlag(u32);
    extern u32 marioChkItemGetMotion(void);
    extern void marioChgGetItemMotion(void);
    extern void marioChgStayMotion(void);
    extern void* effItemGetEntry(f64, f64, f64, s32);
    extern void iconFlagOn(char*, u32);
    extern void iconFlagOff(char*, u32);
    extern void iconChange(char*, u16);
    extern void* iconNameToPtr(char*);
    extern void marioSetCamId(s32);
    extern u32 psndBGMChkSilent(s32);
    extern s32 psndBGMChk(s32);
    extern void psndBGMOff(s32);
    extern void psndBGMOn(s32, char*);
    extern u32 psndSFXOn_3D(char*, void*);
    extern void effStardustEntry(f64, f64, f64, f64, f64, s32, s32, s32);
    extern s32 strcmp(const char*, const char*);
    extern void effSoftDelete(void*);
    extern void iconDelete(char*);
    extern void* evtEntry(void*, s32, s32);
    extern void* evtEntryType(void*, s8, u32, u32);
    extern s32 evtCheckID(s32);
    extern void pouchArriveBadge(s32);
    extern u32 evtGetValue(void*, s32);
    extern void evtSetValue(void*, s32, u32);
    extern u32 winMgrAction(s32);
    extern void winMgrCloseAutoDelete(s32);
    extern s32 winMgrEntry(void*);
    extern void winMgrOpen(s32);
    extern void winMgrSetParam(s32, void*);
    extern void* winMgrSelectEntry(s32, s32, s32);
    extern s32 winMgrSelect(void*);
    extern void winMgrSelectDelete(void*);
    extern u32 windowCheckID(s32);
    extern u32 keyGetButtonTrg(s32);
    extern s32 sprintf(char*, const char*, ...);
    extern s32 msgWindow_Entry(char*, s32, s16);

    extern u8 itemdrv_window_desc[];
    extern u8 all_juyoitem_lecture[];

    extern char str_BGM_FF_GET_STARPIECE_802c91d8[];
    extern char str_BGM_FF_GET_KEYITEM1_802c91f0[];
    extern char str_BGM_FF_GET_BADGE1_802c9204[];
    extern char str_BGM_FF_GET_IMPORTANT_802c9218[];
    extern char str_BGM_FF_GET_ITEM1_802c9234[];
    extern char str_item_first_hit_802c9248[];
    extern char str_iPCT05x_802c9258[];
    extern char str_SFX_COIN_GET1_802c90c0[];
    extern char str_SFX_EVT_GAME_MONTE_C_802c90d0[];
    extern char str_SFX_HEART_GET1_802c90ec[];
    extern char str_SFX_FLOWER_GET1_802c90fc[];

    extern f32 float_0p7_8042110c;
    extern f32 float_1_804210bc;
    extern f32 float_85_80421120;
    extern f32 float_120_80421124;

    u8* workSet;
    void* player;
    u16* mode;
    u16* flags;
    u32* status;
    f32* pos;
    s32 itemId;
    s32 effectType;
    s32 bgmType;
    void* effect;
    VecLocal soundPos0;
    VecLocal soundPos1;

    workSet = work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet += 0x1C;
    }

    player = marioGetPtr();
    mode = (u16*)((s32)item + 0x26);
    flags = (u16*)item;
    status = (u32*)((s32)item + 0x38);
    pos = (f32*)((s32)item + 0x3C);

    switch (*mode) {
    case 0:
        *status &= ~0x10000;
        if ((*flags & 0x100) == 0) {
            *status &= ~0x4000000;
        } else {
            *status |= 0x4000000;
        }
        *flags &= ~0x100;

        if ((*flags & 0x200) != 0) {
            itemId = *(s32*)((s32)item + 4);

            if (pouchGetItem(itemId) != 0) {
                void* entry;
                s32 i;

                if ((*flags & 0x20) != 0) {
                    swSet(*(u32*)((s32)item + 8));
                } else if ((*flags & 0x40) != 0) {
                    _swSet(*(u32*)((s32)item + 8));
                }

                if (*(void**)((s32)item + 0x20) != 0) {
                    void* evt;
                    evt = evtEntry(*(void**)((s32)item + 0x20), 0, 0x20);
                    *(s32*)((s32)evt + 0x9C) =
                        (s32)(1024.0f * pos[0]) - 230000000;
                    *(s32*)((s32)evt + 0xA0) =
                        (s32)(1024.0f * pos[1]) - 230000000;
                    *(s32*)((s32)evt + 0xA4) =
                        (s32)(1024.0f * pos[2]) - 230000000;
                }

                *status |= 1;
                *(u32*)((s32)item + 0x78) = 0;
                *(u32*)((s32)item + 0x7C) = 0;

                soundPos0.x = pos[0];
                soundPos0.y = pos[1];
                soundPos0.z = pos[2];

                if (itemId == 0x7B) {
                    psndSFXOn_3D(str_SFX_HEART_GET1_802c90ec, &soundPos0);
                } else if (itemId < 0x7B) {
                    if (itemId == 0x79) {
                        effStardustEntry(
                            soundPos0.x, soundPos0.y + 35.0f, soundPos0.z,
                            16.0, 16.0, 1, 3, 30);
                        psndSFXOn_3D(str_SFX_COIN_GET1_802c90c0, &soundPos0);
                    } else if (itemId > 0x78) {
                        effStardustEntry(
                            soundPos0.x, soundPos0.y + 35.0f, soundPos0.z,
                            16.0, 16.0, 1, 3, 30);
                        psndSFXOn_3D(
                            str_SFX_EVT_GAME_MONTE_C_802c90d0, &soundPos0);
                    }
                } else if (itemId != 0x7D && itemId < 0x7D) {
                    psndSFXOn_3D(str_SFX_FLOWER_GET1_802c90fc, &soundPos0);
                }

                entry = *(void**)(workSet + 4);
                for (i = 0; i < *(s32*)workSet;
                     i++, entry = (void*)((s32)entry + 0x98)) {
                    if (strcmp(
                            (char*)((s32)entry + 0xC),
                            (char*)((s32)item + 0xC)) == 0) {
                        if (*(s32*)((s32)gp + 0x14) == 0 &&
                            *(s32*)((s32)entry + 4) >= 0xF0 &&
                            *(s32*)((s32)entry + 4) < 0x153 &&
                            (*(u32*)((s32)entry + 0x38) & 0x1000000) != 0) {
                            pouchArriveBadge(*(s32*)((s32)entry + 4));
                        }

                        if (*(s32*)((s32)entry + 4) >= 0x72 &&
                            *(s32*)((s32)entry + 4) < 0x79) {
                            effSoftDelete(*(void**)((s32)entry + 0x1C));
                        } else {
                            iconDelete((char*)((s32)entry + 0xC));
                        }

                        *(u16*)entry &= ~1;
                    }
                }
                return;
            }

            *(u16*)((s32)item + 0x24) = 4;
            *mode = 0;
            return;
        }

        *mode = 1;
        *(u32*)((s32)item + 0x78) = 0;
        *(u32*)((s32)item + 0x7C) = 0;
        *(u16*)camGetPtr(4) |= 0x200;
        marioKeyOff();
        marioStSystemLevel(2);
        psndSetFlag(0x80);

        *(f32*)((s32)player + 0x174) = 0.0f;
        *(f32*)((s32)player + 0x17C) = 0.0f;
        *(f32*)((s32)player + 0x180) = 0.0f;
        *(f32*)((s32)player + 0x18C) = 0.0f;

        itemId = *(s32*)((s32)item + 4);
        if (itemId == 7 || itemId == 8 || itemId == 10 || itemId == 11) {
            *status |= 0x2000;
        }

    case 1:
        if (((*flags & 0x1000) == 0 || (*flags & 0x4000) != 0) &&
            marioChkItemGetMotion() == 0) {
            u16 motion;

            motion = *(u16*)((s32)player + 0x2E);
            if (motion != 5 &&
                (u16)(motion - 6) > 2 &&
                motion != 0xF &&
                motion != 0xE) {
                return;
            }

            *mode = 8;
            return;
        }

        *mode = 2;
        marioChgGetItemMotion();

        effectType = 7;
        bgmType = -1;

        pos[0] = *(f32*)((s32)player + 0x8C);
        pos[1] = *(f32*)((s32)player + 0x90) + 50.0f;
        pos[2] = *(f32*)((s32)player + 0x94);

        itemId = *(s32*)((s32)item + 4);
        if (itemId == 0x57) {
            effectType = 4;
            bgmType = -1;
        } else if (itemId == 0x7D) {
            effectType = 8;
            bgmType = 0;
        } else if (itemId < 0xC) {
            effectType = 3;
            bgmType = 3;
        } else if (itemId < 0x72) {
            effectType = 6;
            bgmType = 1;
        } else if (itemId < 0x79) {
            effectType = 0;
            bgmType = -1;
        } else if (itemId < 0xEC) {
            effectType = 7;
            bgmType = -1;
        } else if (itemId < 0x153) {
            effectType = 5;
            bgmType = 2;
        }

        effect = effItemGetEntry(
            *(f32*)((s32)player + 0x8C),
            *(f32*)((s32)player + 0x90) + 62.0f,
            *(f32*)((s32)player + 0x94),
            effectType);

        *(void**)(workSet + 0x0C) = effect;

        if ((*status & 0x2000) == 0) {
            *(s32*)(*(u8**)((s32)effect + 0x0C) + 0x38) = 5;
            iconFlagOn((char*)((s32)item + 0xC), 0x100);
            if (effectType == 0 || effectType == 3) {
                marioSetCamId(5);
            }
        } else {
            *(s32*)(*(u8**)((s32)effect + 0x0C) + 0x38) = 5;
            iconFlagOn((char*)((s32)item + 0xC), 0x200);
            marioSetCamId(7);
        }

        if ((*flags & 0x8000) == 0) {
            if (psndBGMChkSilent(0) != 0) {
                *flags |= 8;
            }

            if (bgmType == 2) {
                if ((*flags & 8) == 0) {
                    psndBGMOff(0x3800);
                }
                psndBGMOn(0x211, str_BGM_FF_GET_BADGE1_802c9204);
            } else if (bgmType == 0) {
                if ((*flags & 8) == 0) {
                    psndBGMOff(0x3800);
                }
                psndBGMOn(0x211, str_BGM_FF_GET_STARPIECE_802c91d8);
            } else if (bgmType == 1) {
                if ((*flags & 8) == 0) {
                    psndBGMOff(0x3800);
                }
                psndBGMOn(0x211, str_BGM_FF_GET_KEYITEM1_802c91f0);
            } else if (bgmType == 3) {
                if ((*flags & 8) == 0) {
                    psndBGMOff(0x3800);
                }
                psndBGMOn(0x211, str_BGM_FF_GET_IMPORTANT_802c9218);
            } else {
                if ((*flags & 8) == 0) {
                    psndBGMOff(0x3800);
                }
                psndBGMOn(0x211, str_BGM_FF_GET_ITEM1_802c9234);
            }
        }

        if (effectType == 8) {
            u32 count;

            count = evtGetValue(0, -0xA21FE5E);
            evtSetValue(0, -0xA21FE5E, count + 1);
        }

        *flags &= ~0x2000;

        soundPos1.x = pos[0];
        soundPos1.y = pos[1];
        soundPos1.z = pos[2];

        if (itemId == 0x7B) {
            psndSFXOn_3D(str_SFX_HEART_GET1_802c90ec, &soundPos1);
        } else if (itemId < 0x7B) {
            if (itemId == 0x79) {
                effStardustEntry(
                    soundPos1.x, soundPos1.y + 35.0f, soundPos1.z,
                    16.0, 16.0, 1, 3, 30);
                psndSFXOn_3D(str_SFX_COIN_GET1_802c90c0, &soundPos1);
            } else if (itemId > 0x78) {
                effStardustEntry(
                    soundPos1.x, soundPos1.y + 35.0f, soundPos1.z,
                    16.0, 16.0, 1, 3, 30);
                psndSFXOn_3D(
                    str_SFX_EVT_GAME_MONTE_C_802c90d0, &soundPos1);
            }
        } else if (itemId != 0x7D && itemId < 0x7D) {
            psndSFXOn_3D(str_SFX_FLOWER_GET1_802c90fc, &soundPos1);
        }

        *(s32*)((s32)item + 0x28) =
            winMgrEntry(itemdrv_window_desc);
        winMgrOpen(*(s32*)((s32)item + 0x28));
        winMgrSetParam(*(s32*)((s32)item + 0x28), item);

        if ((*status & 0x2000) == 0) {
            *(s32*)((s32)item + 0x2C) =
                winMgrEntry(itemdrv_window_desc + 0x28);
            winMgrOpen(*(s32*)((s32)item + 0x2C));
            winMgrSetParam(*(s32*)((s32)item + 0x2C), item);
        }

        if (pouchGetItem(*(s32*)((s32)item + 4)) != 0) {
            if ((*flags & 0x20) != 0) {
                swSet(*(u32*)((s32)item + 8));
            } else if ((*flags & 0x40) != 0) {
                _swSet(*(u32*)((s32)item + 8));
            }

            if (*(void**)((s32)item + 0x20) != 0) {
                void* evt;

                evt = evtEntry(*(void**)((s32)item + 0x20), 0, 0x20);
                *(s32*)((s32)evt + 0x9C) =
                    (s32)(1024.0f * pos[0]) - 230000000;
                *(s32*)((s32)evt + 0xA0) =
                    (s32)(1024.0f * pos[1]) - 230000000;
                *(s32*)((s32)evt + 0xA4) =
                    (s32)(1024.0f * pos[2]) - 230000000;
            }

            *status |= 1;
        } else {
            *(s32*)((s32)item + 0x30) =
                winMgrEntry(itemdrv_window_desc + 0x50);
            winMgrOpen(*(s32*)((s32)item + 0x30));
            winMgrSetParam(*(s32*)((s32)item + 0x30), item);
        }

        if (*(s32*)((s32)item + 4) == 0x49) {
            iconChange((char*)((s32)item + 0xC), 0x11B);
        }

        if (*(void**)((s32)item + 0x1C) == 0) {
            u8* icon;
            icon = iconNameToPtr((char*)((s32)item + 0xC));
            *(u16*)icon |= 0x80;
        }
        break;

    case 2:
        if ((*status & 0x2000) == 0) {
            marioCtrlOff();
            *status |= 0x1000000;
        }
        *mode = 3;

    case 3:
        if ((*flags & 0x8000) == 0 &&
            (*flags & 0x2000) == 0 &&
            psndBGMChk(1) != 0) {
            *flags |= 0x2000;
            psndBGMOff(0x201);
            if ((*flags & 8) == 0) {
                psndBGMOn(0xA0, 0);
            }
        }

        if (winMgrAction(*(s32*)((s32)item + 0x28)) == 0 &&
            (keyGetButtonTrg(0) & 0x300) != 0) {
            winMgrCloseAutoDelete(*(s32*)((s32)item + 0x28));

            if ((*status & 0x2000) == 0) {
                winMgrCloseAutoDelete(*(s32*)((s32)item + 0x2C));
            }

            if ((*flags & 0x8000) == 0 &&
                (*flags & 0x2000) == 0) {
                psndBGMOff(0x201);
                if ((*flags & 8) == 0) {
                    psndBGMOn(0xA0, 0);
                }
            }

            if ((*status & 0x2000) != 0) {
                *mode = 6;
                break;
            }

            itemId = *(s32*)((s32)item + 4);
            if (itemId >= 0x79 &&
                itemId < 0xEC &&
                itemId != 0x7D &&
                swGet(0xEB) == 0 &&
                (*(u32*)gp & 0x1000) == 0) {
                char* msg;

                swSet(0xEB);
                *mode = 4;
                msg = msgSearch(str_item_first_hit_802c9248);
                *(s32*)(workSet + 0x18) =
                    msgWindow_Entry(msg, 0, 0);
                break;
            }

            effSoftDelete(*(void**)(workSet + 0x0C));
            marioSetCamId(4);
            iconFlagOff((char*)((s32)item + 0xC), 0x100);

            if (*(s32*)((s32)item + 4) == 0x49) {
                iconChange((char*)((s32)item + 0xC), 0x11C);
            }

            if (*(void**)((s32)item + 0x1C) == 0) {
                u8* icon;
                icon = iconNameToPtr((char*)((s32)item + 0xC));
                *(u16*)icon &= ~0x80;
            }

            if ((*status & 1) == 0) {
                *mode = 5;
                winMgrCloseAutoDelete(*(s32*)((s32)item + 0x30));
                marioChgStayMotion();
                iconFlagOn((char*)((s32)item + 0xC), 2);

                itemId = *(s32*)((s32)item + 4);
                if (itemId >= 0xF0 && itemId < 0x153) {
                    *(void**)(workSet + 0x10) =
                        winMgrSelectEntry(2, itemId, 1);
                } else {
                    *(void**)(workSet + 0x10) =
                        winMgrSelectEntry(1, itemId, 1);
                }
            } else {
                void* entry;
                s32 i;

                marioStSystemLevel(0);
                psndClearFlag(0x80);

                if ((*status & 0x1000000) != 0) {
                    *status &= ~0x1000000;
                    marioCtrlOn();
                }

                marioKeyOn();
                marioChgStayMotion();
                *(u16*)camGetPtr(4) &= ~0x200;

                entry = *(void**)(workSet + 4);
                for (i = 0; i < *(s32*)workSet;
                     i++, entry = (void*)((s32)entry + 0x98)) {
                    if (strcmp(
                            (char*)((s32)entry + 0xC),
                            (char*)((s32)item + 0xC)) == 0) {
                        if (*(s32*)((s32)gp + 0x14) == 0 &&
                            *(s32*)((s32)entry + 4) >= 0xF0 &&
                            *(s32*)((s32)entry + 4) < 0x153 &&
                            (*(u32*)((s32)entry + 0x38) & 0x1000000) != 0) {
                            pouchArriveBadge(*(s32*)((s32)entry + 4));
                        }

                        if (*(s32*)((s32)entry + 4) >= 0x72 &&
                            *(s32*)((s32)entry + 4) < 0x79) {
                            effSoftDelete(*(void**)((s32)entry + 0x1C));
                        } else {
                            iconDelete((char*)((s32)entry + 0xC));
                        }

                        *(u16*)entry &= ~1;
                    }
                }
            }
        }
        break;

    case 4:
        if (windowCheckID(*(s32*)(workSet + 0x18)) == 0) {
            void* entry;
            s32 i;

            effSoftDelete(*(void**)(workSet + 0x0C));
            marioSetCamId(4);
            iconFlagOff((char*)((s32)item + 0xC), 0x100);
            marioStSystemLevel(0);
            psndClearFlag(0x80);

            if ((*status & 0x1000000) != 0) {
                *status &= ~0x1000000;
                marioCtrlOn();
            }

            marioKeyOn();
            marioChgStayMotion();
            *(u16*)camGetPtr(4) &= ~0x200;

            entry = *(void**)(workSet + 4);
            for (i = 0; i < *(s32*)workSet;
                 i++, entry = (void*)((s32)entry + 0x98)) {
                if (strcmp(
                        (char*)((s32)entry + 0xC),
                        (char*)((s32)item + 0xC)) == 0) {
                    if (*(s32*)((s32)gp + 0x14) == 0 &&
                        *(s32*)((s32)entry + 4) >= 0xF0 &&
                        *(s32*)((s32)entry + 4) < 0x153 &&
                        (*(u32*)((s32)entry + 0x38) & 0x1000000) != 0) {
                        pouchArriveBadge(*(s32*)((s32)entry + 4));
                    }

                    if (*(s32*)((s32)entry + 4) >= 0x72 &&
                        *(s32*)((s32)entry + 4) < 0x79) {
                        effSoftDelete(*(void**)((s32)entry + 0x1C));
                    } else {
                        iconDelete((char*)((s32)entry + 0xC));
                    }

                    *(u16*)entry &= ~1;
                }
            }
        }
        break;

    case 5:
        {
            s32 selected;

            selected = winMgrSelect(*(void**)(workSet + 0x10));
            if (selected != 0) {
                winMgrSelectDelete(*(void**)(workSet + 0x10));

                if (selected < 0) {
                    if ((*status & 0x4000000) != 0) {
                        *flags |= 0x100;
                    }
                } else {
                    u8* icon;

                    pouchRemoveItem(selected);

                    if (pouchGetItem(*(s32*)((s32)item + 4)) != 0) {
                        if ((*flags & 0x20) != 0) {
                            swSet(*(u32*)((s32)item + 8));
                        } else if ((*flags & 0x40) != 0) {
                            _swSet(*(u32*)((s32)item + 8));
                        }

                        if (*(void**)((s32)item + 0x20) != 0) {
                            void* evt;

                            evt = evtEntry(
                                *(void**)((s32)item + 0x20), 0, 0x20);
                            *(s32*)((s32)evt + 0x9C) =
                                (s32)(1024.0f * pos[0]) - 230000000;
                            *(s32*)((s32)evt + 0xA0) =
                                (s32)(1024.0f * pos[1]) - 230000000;
                            *(s32*)((s32)evt + 0xA4) =
                                (s32)(1024.0f * pos[2]) - 230000000;
                        }

                        *status |= 1;
                    }

                    *status &= ~1;
                    *(s32*)((s32)item + 4) = selected;
                    *(u32*)((s32)item + 0x20) = 0;
                    *(u32*)((s32)item + 8) = 0;
                    *flags &= ~0x60;

                    iconChange(
                        (char*)((s32)item + 0xC),
                        *(u16*)((s32)itemDataTable +
                                selected * 0x28 + 0x20));

                    (*(s32*)(workSet + 8))++;

                    icon = iconNameToPtr((char*)((s32)item + 0xC));
                    sprintf(
                        (char*)(icon + 0x18),
                        str_iPCT05x_802c9258,
                        *(s32*)(workSet + 8));
                    sprintf(
                        (char*)((s32)item + 0xC),
                        str_iPCT05x_802c9258,
                        *(s32*)(workSet + 8));

                    *flags |= 0x100;
                }

                *flags &= ~0x1000;
                *(f32*)((s32)item + 0x50) = float_85_80421120;
                *(f32*)((s32)item + 0x58) = float_120_80421124;
                *(f32*)((s32)item + 0x80) = float_0p7_8042110c;
                *(f32*)((s32)item + 0x84) = float_1_804210bc;

                *(s32*)((s32)item + 0x28) =
                    winMgrEntry(itemdrv_window_desc + 0x78);
                winMgrOpen(*(s32*)((s32)item + 0x28));
                winMgrSetParam(*(s32*)((s32)item + 0x28), item);

                marioChgGetItemMotion();
                iconFlagOff((char*)((s32)item + 0xC), 2);

                if ((*(u32*)player & 0x02000000) != 0) {
                    *status |= 0x8000;
                }

                *mode = 9;
            }
        }
        break;

    case 6:
        {
            u8* evt;

            evt = evtEntryType(all_juyoitem_lecture, 0, 0, 0);
            *(void**)(workSet + 0x14) = evt;
            *(void**)(evt + 0xBC) = item;
            *(void**)(evt + 0xC0) = *(void**)(workSet + 0x0C);

            psndClearFlag(0x80);
            *mode = 7;
        }
        break;

    case 7:
        {
            u8* evt;

            evt = *(u8**)(workSet + 0x14);
            if (evtCheckID(*(s32*)(evt + 0x15C)) == 0) {
                void* entry;
                s32 i;

                marioStSystemLevel(0);
                psndClearFlag(0x80);

                if ((*status & 0x1000000) != 0) {
                    *status &= ~0x1000000;
                    marioCtrlOn();
                }

                marioKeyOn();
                marioChgStayMotion();
                *(u16*)camGetPtr(4) &= ~0x200;

                entry = *(void**)(workSet + 4);
                for (i = 0; i < *(s32*)workSet;
                     i++, entry = (void*)((s32)entry + 0x98)) {
                    if (strcmp(
                            (char*)((s32)entry + 0xC),
                            (char*)((s32)item + 0xC)) == 0) {
                        if (*(s32*)((s32)gp + 0x14) == 0 &&
                            *(s32*)((s32)entry + 4) >= 0xF0 &&
                            *(s32*)((s32)entry + 4) < 0x153 &&
                            (*(u32*)((s32)entry + 0x38) & 0x1000000) != 0) {
                            pouchArriveBadge(*(s32*)((s32)entry + 4));
                        }

                        if (*(s32*)((s32)entry + 4) >= 0x72 &&
                            *(s32*)((s32)entry + 4) < 0x79) {
                            effSoftDelete(*(void**)((s32)entry + 0x1C));
                        } else {
                            iconDelete((char*)((s32)entry + 0xC));
                        }

                        *(u16*)entry &= ~1;
                    }
                }
            }
        }
        break;

    case 8:
        if ((*(u32*)player & 0x02000000) != 0) {
            *status |= 0x8000;
        }

        *(u32*)((s32)item + 0x78) = 0;
        *(u32*)((s32)item + 0x7C) = 0;

        if ((*status & 0x1000) == 0) {
            *(f32*)((s32)item + 0x50) = float_85_80421120;
            *(f32*)((s32)item + 0x58) = float_120_80421124;
            *(f32*)((s32)item + 0x80) = float_0p7_8042110c;
            *(f32*)((s32)item + 0x84) = float_1_804210bc;
            *status |= 8;
            *(u16*)((s32)item + 0x24) = 6;
            *mode = 0;
            *flags &= ~0x10;
            *status &= ~0x1000;
            *flags &= ~2;
            *flags &= ~0x1000;
        } else {
            *(u16*)((s32)item + 0x24) = 1;
            *mode = 0;
            *status &= ~4;
            *status &= ~8;
            *status &= ~0x4000;
            *status &= ~0x1000;
            *flags &= ~0x1000;
        }

        marioStSystemLevel(0);
        psndClearFlag(0x80);

        if ((*status & 0x1000000) != 0) {
            *status &= ~0x1000000;
            marioCtrlOn();
        }

        marioKeyOn();
        break;

    case 9:
        if ((keyGetButtonTrg(0) & 0x100) != 0) {
            winMgrCloseAutoDelete(*(s32*)((s32)item + 0x28));
            *status |= 8;
            *(u16*)((s32)item + 0x24) = 6;
            *mode = 0;
            *flags &= ~0x10;

            itemId = *(s32*)((s32)item + 4);
            if (itemId < 1 || itemId > 0x78) {
                if (itemId < 0xF0 || itemId > 0x152) {
                    *(u32*)((s32)item + 0x7C) = 20000;
                    *(u32*)((s32)item + 0x78) = 0;
                } else {
                    *(u32*)((s32)item + 0x7C) = 0;
                    *(u32*)((s32)item + 0x78) = 0;
                }
            } else {
                *(u32*)((s32)item + 0x7C) = 0;
                *(u32*)((s32)item + 0x78) = 0;
            }

            *status |= 0x10000;
            *status &= ~0x1000;
            *flags &= ~2;

            marioStSystemLevel(0);
            psndClearFlag(0x80);

            if ((*status & 0x1000000) != 0) {
                *status &= ~0x1000000;
                marioCtrlOn();
            }

            marioKeyOn();
            marioChgStayMotion();
            *(u16*)camGetPtr(4) &= ~0x200;
        }
        break;
    }
}

u8 itemseq_Bound(void* item) {
    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32);
    extern void* gp;
    extern void* hitCheckFilter(f64, f64, f64, f64, f64, f64, s32, void*, f32*, void*, f32*, void*, void*, void*);
    extern u32 hitGetAttr(void*);
    extern u32 psndSFXOn_3D(s32, void*);
    extern void effStardustEntry(f64, f64, f64, f64, f64, s32, s32, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    f32* pos = (f32*)((s32)item + 0x3C);
    u32* status = (u32*)((s32)item + 0x38);
    u32* flags = (u32*)((s32)item + 0x34);
    f32 speed;
    s32 angle;
    f32 gravity;
    f32 jump;
    f32 nextX, nextZ, floorY, distance;
    s32 nextAngle;
    void* hit;
    f32 radians, dx, dz, dy;

    marioGetPtr();
    camGetPtr(4);
    if (*(u16*)((s32)item + 0x26) == 0) {
        *(u16*)((s32)item + 0x26) = 1;
        *(u32*)((s32)item + 0x64) = 0;
        *(u32*)((s32)item + 0x60) = 0;
        *(u32*)((s32)item + 0x6C) = *(u32*)((s32)gp + 0x3C);
        *(u32*)((s32)item + 0x68) = *(u32*)((s32)gp + 0x38);
        *(u32*)((s32)item + 0x74) = *(u32*)((s32)gp + 0x3C);
        *(u32*)((s32)item + 0x70) = *(u32*)((s32)gp + 0x38);
        *(s32*)((s32)item + 0x8C) = 0;
        if ((*(u16*)item & 0x10) != 0) return 0;
        switch (*(u16*)((s32)item + 0x24)) {
        case 3:
        case 4:
            if (*(u16*)((s32)item + 0x24) == 4) {
                *status |= 8;
            }
            if ((*flags & 0x10) == 0) {
                *(f32*)((s32)item + 0x80) = 1.0f;
                *(f32*)((s32)item + 0x58) = 1.0f;
                *(f32*)((s32)item + 0x84) = 0.0f;
            }
            *(f32*)((s32)item + 0x50) = 0.0f;
            *(s32*)((s32)item + 0x54) = 0;
            *(f32*)((s32)item + 0x5C) = 0.0f;
            break;
        case 6:
            *status |= 4;
            if ((*flags & 0x10) == 0) {
                *(f32*)((s32)item + 0x80) = 1.0f;
                *(f32*)((s32)item + 0x58) = 1.0f;
                *(f32*)((s32)item + 0x84) = 0.0f;
                *(f32*)((s32)item + 0x5C) = 500.0f;
            }
            break;
        case 10:
            *(f32*)((s32)item + 0x80) = 1.0f;
            *(f32*)((s32)item + 0x58) = 1.0f;
            *(f32*)((s32)item + 0x84) = 0.0f;
            *(f32*)((s32)item + 0x5C) = 500.0f;
            *(f32*)((s32)item + 0x50) = 0.0f;
            *(s32*)((s32)item + 0x54) = 0;
            *status |= 0x24;
            break;
        }
    }
    if (*(u16*)((s32)item + 0x26) != 1) {
        return 0;
    }
    speed = *(f32*)((s32)item + 0x50);
    angle = *(s32*)((s32)item + 0x54);
    gravity = *(f32*)((s32)item + 0x58);
    jump = *(f32*)((s32)item + 0x5C);
    nextAngle = angle;
    radians = 6.2832f * (f32)angle / 360.0f;
    dx = speed * (f32)sin(radians);
    dz = -speed * (f32)cos(radians);
    dy = jump - gravity * 0.98f;
    if ((*status & 4) != 0 && (*(s32*)((s32)item + 0x8C) != 0 || dy < 0.0f)) *status &= ~4;
    if ((*status & 8) != 0 && *(s32*)((s32)item + 0x8C) != 0) *status &= ~8;
    if ((*status & 0x20) != 0 && dy < 0.0f) {
        effStardustEntry(pos[0], pos[1], pos[2], 16.0, 16.0, 1, 3, 30);
        return 0;
    }
    nextX = pos[0];
    nextZ = pos[2];
    if (itemHitCheckSide(dx, dz, item, &nextX, &nextZ, &nextAngle) != 0) {
        *(f32*)((s32)item + 0x50) *= 0.25f;
    }
    pos[0] = nextX;
    pos[2] = nextZ;
    *(u32*)((s32)item + 0x54) = nextAngle;

    hit = 0;
    if ((*status & 0x20) == 0 && (*flags & 0x800) == 0) {
        if (dy > 0.0f) {
            distance = 20.0f + dy;
            hit = hitCheckFilter(pos[0], pos[1] + 20.0f, pos[2],
                                 0.0, 1.0, 0.0, 0,
                                 0, &floorY, 0, &distance, 0, 0, 0);
            if (hit == 0) {
                distance = 20.0f + dy;
                hit = hitCheckFilter(pos[0], pos[1], pos[2],
                                     0.0, 1.0, 0.0, 0,
                                     0, &floorY, 0, &distance, 0, 0, 0);
                if (hit != 0) {
                    pos[1] = floorY - 20.0f;
                }
            } else {
                pos[1] = floorY - 24.0f;
            }
        }
        if (hit != 0) {
            *(f32*)((s32)item + 0x5C) = -jump * 0.25f;
            *(u32*)((s32)item + 0x60) = 0;
            *(u32*)((s32)item + 0x64) = 0;
            *(u32*)((s32)item + 0x68) = *(u32*)((s32)item + 0x70);
            *(u32*)((s32)item + 0x6C) = *(u32*)((s32)item + 0x74);
        }
    }
    if (hit == 0) {
        pos[1] += dy;
    }
    if ((*status & 0x100000) != 0 && pos[1] <= -1000.0f) {
        *(u16*)((s32)item + 0x24) = 7;
        *(u16*)((s32)item + 0x26) = 0;
        *status &= ~(4 | 8 | 0x4000 | 0x1000);
        *status |= 0x10000;
        *flags |= 0x100;
        return 0;
    }
    if (hit != 0 && (hitGetAttr(hit) & 0x600) == 0) {
        s32 bounds = ++*(s32*)((s32)item + 0x8C);
        if ((hitGetAttr(hit) & 0x80000000) == 0 && *(f32*)((s32)item + 0x90) != 0.0f &&
            bounds >= (s32)*(f32*)((s32)item + 0x90)) {
            *(u16*)((s32)item + 0x24) = 1;
            *(u16*)((s32)item + 0x26) = 0;
            *status &= ~(4 | 8 | 0x4000);
        } else {
            *(f32*)((s32)item + 0x5C) = -jump * 0.25f;
            psndSFXOn_3D(0x1D0, pos);
        }
    }
    return 0;
}

void itemSetPosition(void* item, f32 x, f32 y, f32 z) {
    *(f32*)((s32)item + 0x3C) = x;
    *(f32*)((s32)item + 0x40) = y;
    *(f32*)((s32)item + 0x44) = z;
}

s32 itemHitCheckSide(f32 moveX, f32 moveZ, void* item, f32* outX, f32* outZ, s32* outAngle) {
    extern f32 angleABf(f32, f32, f32, f32);
    extern void sincosf(f32, f32*, f32*);
    extern s32 hitCheckFilter(
        f64, f64, f64, f64, f64, f64, s32,
        void*, void*, void*, void*, void*, void*, void*);
    extern f32 reviseAngle(f32);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;

    extern f32 float_0_804210b4;
    extern f32 float_10_804210c8;
    extern f32 float_15_804210cc;
    extern f32 float_0p5_804210d0;
    extern f32 float_180_804210d4;
    extern f32 float_100_804210d8;

    /*
     * Declaration order is intentional.  The target's two hitCheckFilter
     * calls pass stack slots in the descending sequence:
     *   hitWork, hitX, hitY, radius, normalX, hitZ, normalZ,
     * with sin/cos immediately below those slots.
     */
    u32 hitWork[2];
    f32 hitX;
    f32 hitY;
    f32 radius;
    f32 normalX;
    f32 hitZ;
    f32 normalZ;
    f32 sinv;
    f32 cosv;
    f32 lenSq;
    s32 hit;

    *outX = *(f32*)((s32)item + 0x3C);
    *outZ = *(f32*)((s32)item + 0x44);

    if (moveX == float_0_804210b4 &&
        moveZ == float_0_804210b4) {
        return 0;
    }

    sincosf(
        angleABf(
            float_0_804210b4,
            float_0_804210b4,
            moveX,
            moveZ),
        &sinv,
        &cosv);

    lenSq = moveX * moveX + moveZ * moveZ;

    {
        f32 moveLen;

        /*
         * Keep lenSq itself live.  The previous A1 type-punned lenSq
         * directly, which forced an unconditional early stack spill.
         * The target spills only inside the non-positive classification
         * path, so classify a branch-local copy instead.
         */
        if (lenSq > float_0_804210b4) {
            f64 d;
            f64 inv;

            d = (f64)lenSq;
            inv = __frsqrte(d);
            inv = 0.5 * inv * -(d * inv * inv - 3.0);
            inv = 0.5 * inv * -(d * inv * inv - 3.0);
            moveLen = (f32)(
                d * 0.5 * inv *
                -(d * inv * inv - 3.0));
        } else if ((f64)lenSq < 0.0) {
            moveLen = __float_nan;
        } else {
            f32 classifyValue;
            u32 bits;
            u32 kindBits;
            s32 kind;

            classifyValue = lenSq;
            bits = *(u32*)&classifyValue;
            kindBits = bits & 0x7F800000;

            if (kindBits == 0x7F800000) {
                if ((bits & 0x7FFFFF) == 0) {
                    kind = 2;
                } else {
                    kind = 1;
                }
            } else if (kindBits < 0x7F800000) {
                if (kindBits == 0) {
                    if ((bits & 0x7FFFFF) == 0) {
                        kind = 3;
                    } else {
                        kind = 5;
                    }
                } else {
                    kind = 4;
                }
            } else {
                kind = 4;
            }

            if (kind == 1) {
                moveLen = __float_nan;
            } else {
                moveLen = lenSq;
            }
        }

        radius = float_10_804210c8 + moveLen;
    }

    hit = hitCheckFilter(
        *(f32*)((s32)item + 0x3C),
        *(f32*)((s32)item + 0x40) + float_15_804210cc,
        *(f32*)((s32)item + 0x44),
        sinv,
        float_0_804210b4,
        cosv,
        0,
        hitWork,
        &hitX,
        &hitY,
        &radius,
        &normalX,
        &hitZ,
        &normalZ);

    if (hit != 0) {
        f32 dot;
        f32 reflectX;
        f32 reflectZ;
        f32 incoming;
        f32 normalAngle;

        dot = moveX * normalX + moveZ * normalZ;

        reflectX = -(dot * normalX - moveX);
        reflectZ = -(dot * normalZ - moveZ);
        reflectX = float_0p5_804210d0 * reflectX;
        reflectZ = float_0p5_804210d0 * reflectZ;

        *outX = *outX + reflectX;
        *outZ = *outZ + reflectZ;

        incoming = reviseAngle(
            float_180_804210d4 + *outAngle);

        normalAngle = reviseAngle(
            angleABf(
                float_0_804210b4,
                float_0_804210b4,
                float_100_804210d8 * normalX,
                float_100_804210d8 * normalZ));

        incoming = reviseAngle(normalAngle - incoming);

        *outAngle =
            (s32)reviseAngle(normalAngle + incoming);

        return hit;
    }

    {
        f32 moveLen;

        if (lenSq > float_0_804210b4) {
            f64 d;
            f64 inv;

            d = (f64)lenSq;
            inv = __frsqrte(d);
            inv = 0.5 * inv * -(d * inv * inv - 3.0);
            inv = 0.5 * inv * -(d * inv * inv - 3.0);
            moveLen = (f32)(
                d * 0.5 * inv *
                -(d * inv * inv - 3.0));
        } else if ((f64)lenSq < 0.0) {
            moveLen = __float_nan;
        } else {
            f32 classifyValue;
            u32 bits;
            u32 kindBits;
            s32 kind;

            classifyValue = lenSq;
            bits = *(u32*)&classifyValue;
            kindBits = bits & 0x7F800000;

            if (kindBits == 0x7F800000) {
                if ((bits & 0x7FFFFF) == 0) {
                    kind = 2;
                } else {
                    kind = 1;
                }
            } else if (kindBits < 0x7F800000) {
                if (kindBits == 0) {
                    if ((bits & 0x7FFFFF) == 0) {
                        kind = 3;
                    } else {
                        kind = 5;
                    }
                } else {
                    kind = 4;
                }
            } else {
                kind = 4;
            }

            if (kind == 1) {
                moveLen = __float_nan;
            } else {
                moveLen = lenSq;
            }
        }

        radius = float_10_804210c8 + moveLen;
    }

    hit = hitCheckFilter(
        *(f32*)((s32)item + 0x3C),
        *(f32*)((s32)item + 0x40) + float_15_804210cc,
        *(f32*)((s32)item + 0x44),
        sinv,
        float_0_804210b4,
        cosv,
        0,
        hitWork,
        &hitX,
        &hitY,
        &radius,
        &normalX,
        &hitZ,
        &normalZ);

    /*
     * Target lays out the successful second-hit path first and branches
     * to the no-hit movement tail.  Keep that polarity instead of A1's
     * early no-hit return.
     */
    if (hit != 0) {
        f32 dot;
        f32 reflectX;
        f32 reflectZ;
        f32 incoming;
        f32 normalAngle;

        dot = moveX * normalX + moveZ * normalZ;

        reflectX = -(dot * normalX - moveX);
        reflectZ = -(dot * normalZ - moveZ);
        reflectX = float_0p5_804210d0 * reflectX;
        reflectZ = float_0p5_804210d0 * reflectZ;

        *outX = *outX + reflectX;
        *outZ = *outZ + reflectZ;

        incoming = reviseAngle(
            float_180_804210d4 + *outAngle);

        normalAngle = reviseAngle(
            angleABf(
                float_0_804210b4,
                float_0_804210b4,
                float_100_804210d8 * normalX,
                float_100_804210d8 * normalZ));

        incoming = reviseAngle(normalAngle - incoming);

        *outAngle =
            (s32)reviseAngle(normalAngle + incoming);

        return hit;
    }

    *outX = *outX + moveX;
    *outZ = *outZ + moveZ;
    return 0;
}

void itemForceGet(void* item) {
    *(s32*)((s32)item + 0x7C) = 0;
    *(s32*)((s32)item + 0x78) = 0;
    *(u16*)((s32)item + 0x24) = 2;
    *(u16*)((s32)item + 0x26) = 0;

    *(u16*)item |= 0x1000;
    *(u32*)((s32)item + 0x38) &= ~0x10000;
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

void itemFlagOn(void* item, u32 flags) {
    *(u16*)item |= flags;
}

void itemFlagOff(void* item, u32 flags) {
    *(u16*)item &= ~flags;
}

u16 itemFlag(void* item, u32 flags) {
    return *(u16*)item & flags;
}

void itemStatusOn(void* item, u32 flags) {
    *(u32*)((s32)item + 0x38) |= flags;
}

u32 itemStatus(void* item, u32 flags) {
    return *(u32*)((s32)item + 0x38) & flags;
}

const u32 dat_804210a8 = 0x000000FF;
const u32 dat_804210ac = 0x000000FF;
const u32 dat_804210b0 = 0xFFFFFFFF;
const f32 float_0_804210b4 = 0.0f;
const f32 float_20_804210b8 = 20.0f;
const f32 float_1_804210bc = 1.0f;
const f32 float_4_804210c0 = 4.0f;
const f32 float_neg1_804210c4 = -1.0f;
const f32 float_10_804210c8 = 10.0f;
const f32 float_15_804210cc = 15.0f;
const f32 float_0p5_804210d0 = 0.5f;
const f32 float_180_804210d4 = 180.0f;
const f32 float_100_804210d8 = 100.0f;
const f32 float_35_804210dc = 35.0f;
const f32 float_16_804210e0 = 16.0f;
const f32 float_500_804210e4 = 500.0f;
const f32 float_90_804210e8 = 90.0f;
const f32 float_30_804210ec = 30.0f;
const f32 float_0p67_804210f0 = 0.67f;
const f32 float_1E06_804210f4 = 1000000.0f;
const f32 float_6p2832_804210f8 = 6.28318548f;
const f32 float_360_804210fc = 360.0f;
const f32 float_neg980_80421100 = -980.0f;
const f32 float_0p25_80421104 = 0.25f;
const f32 float_neg1000_80421108 = -1000.0f;
const f32 float_0p7_8042110c = 0.7f;
const f32 float_8_80421110 = 8.0f;
const f32 float_1024_80421114 = 1024.0f;
const f32 float_50_80421118 = 50.0f;
const f32 float_62_8042111c = 62.0f;
const f32 float_85_80421120 = 85.0f;
const f32 float_120_80421124 = 120.0f;
const f32 float_12_80421128 = 12.0f;
const f32 float_1000_8042112c = 1000.0f;

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
