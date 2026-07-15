#include "driver/casedrv.h"

extern void* wp;
extern void* __memAlloc();

void caseMain(void) {
    extern void* wp;
    extern void* gp;
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern u32 marioStGetSystemLevel(void);
    extern s32 winCheck(void);
    extern s32 N_fbatPreventMarioEventChk(void);
    extern s32 marioCaseEventValidChk(void);
    extern s32 yoshiGetStatus(void);
    extern const char* hitGetName(void* hit);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 keyGetDirTrg(s32 chan);
    extern void* fbatNpcTalkCheck(void);
    extern s32 swGet(s32 flag);
    extern s32 _swGet(s32 flag);
    extern s32 evtCheckID(s32 id);
    extern void* evtEntry(s32 evtCode, s32 evtArg, s32 flags);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void npcStopForEvent(void);

    void* player;
    s32 buttonsA;
    void* entry;
    s32 i;
    void* objStandOn;
    void* objUnder;
    void* objJumpFrom;
    void* objInteract;
    void* objPush;
    void* mobjInteract;
    void* objHead;
    void* party;
    void* evt;
    u16 flags;
    u32 conditionId;
    u32 motion;
    s32 (*activeFunc)(void*);

    player = marioGetPtr();
    partyGetPtr(marioGetPartyId());

    objStandOn = *(void**)((s32)player + 0x1E8);
    objUnder = *(void**)((s32)player + 0x1FC);
    objJumpFrom = *(void**)((s32)player + 0x1EC);
    objInteract = *(void**)((s32)player + 0x1E4);
    objPush = *(void**)((s32)player + 0x1F0);
    mobjInteract = *(void**)((s32)player + 0x200);
    objHead = *(void**)((s32)player + 0x1F8);
    i = *(u16*)((s32)player + 0x24C);

    if (marioStGetSystemLevel() == 0 &&
        winCheck() == 0 &&
        *(s32*)((s32)gp + 0x14) == 0 &&
        N_fbatPreventMarioEventChk() == 0 &&
        marioCaseEventValidChk() != 0) {
        motion = *(u16*)((s32)player + 0x2E);
        if (motion != 0 && motion != 1 && motion != 2 && yoshiGetStatus() != 1) {
            i = 0;
        }

        buttonsA = i & 0x100;
        entry = *(void**)((s32)wp + 4);

        for (i = 0; i < *(s32*)wp; i++, entry = (void*)((s32)entry + 0xA0)) {
            flags = *(u16*)entry;
            if ((flags & 1) == 0) {
                continue;
            }

            conditionId = *(u32*)((s32)entry + 4);
            switch (conditionId) {
                case 0:
                    if (objStandOn != 0 &&
                        strcmp(hitGetName(objStandOn), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 1:
                    if (objStandOn != 0) {
                        if (strcmp(hitGetName(objStandOn), (const char*)((s32)entry + 8)) != 0) {
                            *(u16*)entry |= 2;
                        }
                    } else {
                        *(u16*)entry = flags | 2;
                    }
                    break;

                case 2:
                    if (objUnder != 0 &&
                        strcmp(hitGetName(objUnder), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 3:
                    if (objUnder != 0) {
                        if (strcmp(hitGetName(objUnder), (const char*)((s32)entry + 8)) != 0) {
                            *(u16*)entry |= 2;
                        }
                    } else {
                        *(u16*)entry = flags | 2;
                    }
                    break;

                case 8:
                    if (objInteract != 0 &&
                        strcmp(hitGetName(objInteract), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 4:
                    if (objJumpFrom != 0 &&
                        *(s16*)((s32)player + 0x50) == 1 &&
                        fbatNpcTalkCheck() == 0 &&
                        strcmp(hitGetName(objJumpFrom), (const char*)((s32)entry + 8)) == 0 &&
                        (*(u16*)((s32)player + 0x24C) & 0x100) != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 5:
                    if (objStandOn != 0 &&
                        fbatNpcTalkCheck() == 0 &&
                        strcmp(hitGetName(objStandOn), (const char*)((s32)entry + 8)) == 0 &&
                        buttonsA != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 6:
                    if (objStandOn != 0 &&
                        strcmp(hitGetName(objStandOn), (const char*)((s32)entry + 8)) == 0 &&
                        (s8)*(u8*)((s32)player + 0x253) > 0x1E) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 7:
                    if (objStandOn != 0 &&
                        strcmp(hitGetName(objStandOn), (const char*)((s32)entry + 8)) == 0 &&
                        (keyGetDirTrg(0) & 0x1000) != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 9:
                    if (objInteract != 0 &&
                        fbatNpcTalkCheck() == 0 &&
                        strcmp(hitGetName(objInteract), (const char*)((s32)entry + 8)) == 0 &&
                        buttonsA != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 10:
                    if (objPush != 0 &&
                        strcmp(hitGetName(objPush), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 11:
                    if (objPush != 0 &&
                        strcmp(hitGetName(objPush), (const char*)((s32)entry + 8)) == 0) {
                        break;
                    }
                    *(u16*)entry |= 2;
                    break;

                case 12:
                    if (objPush != 0 &&
                        fbatNpcTalkCheck() == 0 &&
                        strcmp(hitGetName(objPush), (const char*)((s32)entry + 8)) == 0 &&
                        buttonsA != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 13:
                    if (mobjInteract != 0 &&
                        strcmp(hitGetName(mobjInteract), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 14:
                    if (mobjInteract != 0 &&
                        *(u16*)((s32)player + 0x2E) == 0x13 &&
                        strcmp(hitGetName(mobjInteract), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 15:
                    party = partyGetPtr(marioGetPartyId());
                    if (party != 0 &&
                        (s32)*(u8*)((s32)party + 0x31) == 3 &&
                        *(void**)((s32)party + 0x130) != 0 &&
                        strcmp(hitGetName(*(void**)((s32)party + 0x130)), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 16:
                    if (objHead != 0 &&
                        strcmp(hitGetName(objHead), (const char*)((s32)entry + 8)) == 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 18:
                    if (swGet(*(s32*)((s32)entry + 0x4C)) != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 19:
                    if (_swGet(*(s32*)((s32)entry + 0x4C)) != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;

                case 17:
                    activeFunc = *(s32 (**)(void*))((s32)entry + 0x50);
                    if (activeFunc != 0 && activeFunc(entry) != 0) {
                        *(u16*)entry |= 2;
                    }
                    break;
            }
        }

        entry = *(void**)((s32)wp + 4);
        for (i = 0; i < *(s32*)wp; i++, entry = (void*)((s32)entry + 0xA0)) {
            flags = *(u16*)entry;
            if ((flags & 1) != 0 && (flags & 2) != 0) {
                if (evtCheckID(*(s32*)((s32)entry + 0x94)) == 0) {
                    *(s32*)((s32)entry + 0x94) = 0;
                }

                if (*(s32*)((s32)entry + 0x94) == 0) {
                    evt = evtEntry(*(s32*)((s32)entry + 0x98), *(s32*)((s32)entry + 0x9C), 0x20);
                    *(s32*)((s32)evt + 0x16C) = *(s32*)((s32)entry + 0x48);
                    *(s32*)((s32)entry + 0x94) = *(s32*)((s32)evt + 0x15C);
                    memcpy((void*)((s32)evt + 0x9C), (void*)((s32)entry + 0x54), 0x40);
                    if (*(s32*)((s32)entry + 0x9C) == 0x14) {
                        npcStopForEvent();
                    }
                }

                *(u16*)entry &= ~2;
            }
        }
    }
}

u8 caseInit(void) {
    *(s32*)wp = 0x40;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0xA0);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0xA0);
}


void caseReInit(void) {
    void* entry = *(void**)((s32)wp + 4);
    s32 i = 0;

    while (i < *(s32*)wp) {
        *(u16*)entry &= ~1;
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
}


void caseDelete(s32 id) {
    void* entry = (void*)((s32)*(void**)((s32)wp + 4) + id * 0xA0);

    *(u16*)entry &= ~1;
}


s32 caseEntry(void* param) {
    extern void* wp;
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern char* strcpy(char* dst, const char* src);
    void* work = wp;
    s32 count = *(s32*)work;
    void* entry = *(void**)((s32)work + 4);
    s32 id;

    for (id = 0; id < count; id++, entry = (void*)((s32)entry + 0xA0)) {
        if ((*(u16*)entry & 1) == 0) {
            break;
        }
    }

    memset(entry, 0, 0xA0);
    *(s32*)((s32)entry + 0x48) = id;
    *(u16*)entry = 0;
    *(s32*)((s32)entry + 4) = *(u16*)param & ~0x8000;
    if ((*(u16*)param & 0x8000) != 0) {
        *(u16*)entry |= 4;
    }
    *(u16*)entry |= 1;
    strcpy((char*)((s32)entry + 8), *(char**)((s32)param + 4));
    *(s32*)((s32)entry + 0x4C) = *(s32*)((s32)param + 8);
    *(s32*)((s32)entry + 0x50) = *(s32*)((s32)param + 0xC);
    *(s32*)((s32)entry + 0x98) = *(s32*)((s32)param + 0x54);
    *(s32*)((s32)entry + 0x9C) = *(s32*)((s32)param + 0x58);
    *(s32*)((s32)entry + 0x94) = 0;
    memcpy((void*)((s32)entry + 0x54), (void*)((s32)param + 0x10), 0x40);
    return id;
}

void* caseCheckHitObj2(u32 pHit) {
    extern void* gp;
    extern void* wp;
    extern const char* hitGetName(void* hit);
    extern int strcmp(const char* a, const char* b);
    void* entry;
    s32 i;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        return 0;
    }

    i = 0;
    entry = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        s32 type;
        if ((*(u16*)entry & 1) != 0) {
            type = *(s32*)((s32)entry + 4);
            if (type != 0x11 && (u32)(type - 0x12) > 1 && type == 0xF) {
                if (strcmp(hitGetName((void*)pHit), (const char*)((s32)entry + 8)) == 0) {
                    return entry;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
    return 0;
}

void* caseCheckHitObj(void* pHit) {
    extern void* gp;
    extern void* wp;
    extern const char* hitGetName(void* hit);
    extern int strcmp(const char* a, const char* b);
    void* entry;
    s32 i;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        return 0;
    }

    i = 0;
    entry = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        s32 type;
        if ((*(u16*)entry & 1) != 0) {
            type = *(s32*)((s32)entry + 4);
            if (type != 0x11 && (u32)(type - 0x12) > 1) {
                if (strcmp(hitGetName(pHit), (const char*)((s32)entry + 8)) == 0) {
                    return entry;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
    return 0;
}

void* caseIdToPtr(s32 id) {
    void* entry = (void*)((s32)*(void**)((s32)wp + 4) + id * 0xA0);

    if (*(u16*)entry & 1) {
        return entry;
    }
    return NULL;
}
