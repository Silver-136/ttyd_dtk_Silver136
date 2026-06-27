#include "manager/winmgr.h"

void* wp;
void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int val, unsigned long size);
void __memFree(s32 heap, void* ptr);
extern u8 itemDataTable[];
char* msgSearch(char* msg);
void winMgrHelpDraw(void* win);

/* Reorder pilot candidate: functions arranged in _main.map order for main/manager/winmgr. */

void select_disp3_mario(void* win) {
    extern void* pouchGetPtr(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern char* msgSearch(char* msg);
    extern void winMgrHelpDraw(void* win);
    extern char vec3_803003a0[];
    extern char str_help_80428008[];
    extern char* mario_status_name_table[];

    char buf[0x50];
    void* data;
    void* list;
    u16 flags;
    s32 index;
    char* label;
    u16 status;

    data = *(void**)((s32)win + 0x2C);
    pouchGetPtr();
    if ((*(u32*)((s32)*(void**)((s32)wp + 4) + *(s32*)((s32)data + 0x2C) * 0x44) & 4) == 0) {
        index = *(s32*)((s32)data + 0xC);
        list = *(void**)((s32)data + 0x30);
        flags = *(u16*)((s32)list + (index << 2));
        if (flags & 2) {
            if (flags & 4) {
                label = &vec3_803003a0[0x300];
            } else {
                label = &vec3_803003a0[0x30C];
            }
        } else {
            label = str_help_80428008;
        }
        status = *(u16*)((s32)list + (index << 2) + 2);
        if (*(s32*)((s32)data + 4) == 9) {
            sprintf(buf, &vec3_803003a0[0x314], label, mario_status_name_table[status]);
        } else {
            sprintf(buf, &vec3_803003a0[0x324], label, mario_status_name_table[status]);
        }
        *(char**)((s32)win + 0x34) = msgSearch(buf);
        winMgrHelpDraw(win);
    }
}

void select_disp3_party(void* win) {
    extern void* pouchGetPtr(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern char* msgSearch(char* msg);
    extern void winMgrHelpDraw(void* win);
    extern char vec3_803003a0[];
    extern char str_help_80428008[];
    extern u16 party_id_table[];
    extern char* party_labelname_table[];

    char buf[0x50];
    void* data;
    void* list;
    void* pouch;
    u16 flags;
    u16 party;
    char* label;
    char* state;
    s32 index;
    s32 count;

    data = *(void**)((s32)win + 0x2C);
    pouch = pouchGetPtr();
    if ((*(u32*)((s32)*(void**)((s32)wp + 4) + *(s32*)((s32)data + 0x2C) * 0x44) & 4) == 0) {
        index = *(s32*)((s32)data + 0xC);
        list = *(void**)((s32)data + 0x30);
        flags = *(u16*)((s32)list + (index << 2));
        label = str_help_80428008;
        if (flags & 2) {
            label = &vec3_803003a0[0x30C];
            if (*(s32*)((s32)data + 4) == 6) {
                state = &vec3_803003a0[0x334];
            } else {
                state = &vec3_803003a0[0x33C];
            }
            party = *(u16*)((s32)list + (index << 2) + 2);
            count = *(s16*)((s32)pouch + party_id_table[party] * 0xE + 0xC) + 1;
            sprintf(buf, &vec3_803003a0[0x348], label, state, count, party_labelname_table[party]);
            *(char**)((s32)win + 0x34) = msgSearch(buf);
        } else {
            *(char**)((s32)win + 0x34) = msgSearch(label);
        }
        winMgrHelpDraw(win);
    }
}

void select_disp3(void* win) {
    typedef struct SelectEntry {
        u16 unk0;
        u16 item;
    } SelectEntry;

    void* data = *(void**)((s32)win + 0x2C);
    s32 id = *(s32*)((s32)data + 0x2C);
    void* mgr = *(void**)((s32)wp + 4);

    if ((*(u32*)((s32)mgr + id * 0x44) & 4) == 0) {
        s32 index = *(s32*)((s32)data + 0xC);
        u8* table = (u8*)itemDataTable;
        SelectEntry* list = *(SelectEntry**)((s32)data + 0x30);
        u16 item = list[index].item;

        *(char**)((s32)win + 0x34) = msgSearch(*(char**)(table + item * 0x28 + 8));
        winMgrHelpDraw(win);
    }
}

void select_main3(void* win) {
    extern void FontGetMessageWidthLine(char* msg, u16* lines);
    extern u32 keyGetDirTrg(s32 controller);

    u16 lines;
    s32 b;
    s32 a;

    lines = 0;
    if ((*(u32*)win & 4) == 0) {
        if (*(char**)((s32)win + 0x34) != 0) {
            FontGetMessageWidthLine(*(char**)((s32)win + 0x34), &lines);
            *(s32*)((s32)win + 0x3C) = lines + 1;
        }
        if (*(s32*)((s32)win + 0x3C) > 2) {
            if (keyGetDirTrg(0) & 0x100) {
                *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) - 2;
                if (*(s32*)((s32)win + 0x38) < 0) {
                    *(s32*)((s32)win + 0x38) = 0;
                }
            } else if (keyGetDirTrg(0) & 0x200) {
                a = *(s32*)((s32)win + 0x3C) + 1;
                b = *(s32*)((s32)win + 0x38) + 1;
                if (((s32)(a + ((u32)a >> 31)) >> 1) - 1 > ((s32)(b + ((u32)b >> 31)) >> 1)) {
                    *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) + 2;
                }
            }
        }
    }
}

int select_disp2() {
    return 0;
}

int select_disp_luigi() {
    return 0;
}

int select_disp_mario() {
    return 0;
}

int select_disp_party() {
    return 0;
}

int select_disp() {
    return 0;
}

int select_main() {
    return 0;
}

s32 unk_8023cf04(void* win) {
    return *(s32*)((s32)win + 0xC);
}

void winMgrSelectDelete(void* select) {
    register s32 keep = (s32)select;

    __memFree(0, *(void**)(keep + 0x30));
    __memFree(0, (void*)keep);
}

int winMgrSelectOther() {
    return 0;
}

s32 winMgrSelect(void* select) {
    u16 flags = *(u16*)select;

    if (flags & 0x1000) {
        if (flags & 0x2000) {
            return -1;
        }
        if (*(s32*)((s32)select + 0x38) != 0 && *(s32*)((s32)select + 0xC) == 0 && (flags & 0x100)) {
            return -1;
        }
        {
            s32 offset = *(s32*)((s32)select + 0xC) << 2;
            void* list = *(void**)((s32)select + 0x30);

            list = (void*)((s32)list + offset);
            return *(u16*)((s32)list + 2);
        }
    }
    return 0;
}

int unk_8023d524() {
    return 0;
}

s32 unk_8023d59c(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 av = *(s16*)((s32)values + ai * 0x28);
    s32 bv = *(s16*)((s32)values + bi * 0x28);

    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}

s32 unk_8023d5e4(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    u8* values = itemDataTable + 0x18;
    u16 av = *(u16*)(values + ai * 0x28);
    u16 bv = *(u16*)(values + bi * 0x28);
    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}

int* winMgrSelectEntry(u32 selectType, int newItem, int isCancellable) {
    return 0;
}

void winMgrHelpDraw(void* win) {
    return;
}

void winMgrHelpMain(void* win) {
    extern void FontGetMessageWidthLine(char* msg, u16* lines);
    extern u32 keyGetDirTrg(s32 controller);

    u16 lines;
    s32 b;
    s32 a;

    lines = 0;
    if ((*(u32*)win & 4) == 0) {
        if (*(char**)((s32)win + 0x34) != 0) {
            FontGetMessageWidthLine(*(char**)((s32)win + 0x34), &lines);
            *(s32*)((s32)win + 0x3C) = lines + 1;
        }
        if (*(s32*)((s32)win + 0x3C) > 2) {
            if (keyGetDirTrg(0) & 0x100) {
                *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) - 2;
                if (*(s32*)((s32)win + 0x38) < 0) {
                    *(s32*)((s32)win + 0x38) = 0;
                }
            } else if (keyGetDirTrg(0) & 0x200) {
                a = *(s32*)((s32)win + 0x3C) + 1;
                b = *(s32*)((s32)win + 0x38) + 1;
                if (((s32)(a + ((u32)a >> 31)) >> 1) - 1 > ((s32)(b + ((u32)b >> 31)) >> 1)) {
                    *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) + 2;
                }
            }
        }
    }
}

void winMgrHelpInit(void* win) {
    *(s32*)((s32)win + 0x38) = 0;
    *(s32*)((s32)win + 0x3C) = 0;
    *(s32*)((s32)win + 0x34) = 0;
}

void* winMgrGetPtr(s32 id) {
    return (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
}

void winMgrSetPriority(s32 id, s32 priority) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x30) = priority;
    }
}

void winMgrSetSize(s32 id, s32 x, s32 y, s32 w, s32 h) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x18) = x;
        *(s32*)((s32)win + 0x1C) = y;
        *(s32*)((s32)win + 0x20) = w;
        *(s32*)((s32)win + 0x24) = h;
    }
}

s32 winMgrAction(s32 id) {
    return (*(u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44) >> 2) & 1;
}

void winMgrDelete(s32 id) {
    u32* flags = (u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    *flags &= ~1;
}

void winMgrCloseAutoDelete(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
        *(u32*)win |= 8;
    }
}

void winMgrClose(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
    }
}

void winMgrOpen(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 1;
        *(s32*)((s32)win + 8) = 0;
        *(s32*)((s32)win + 0x38) = 0;
        *(s32*)((s32)win + 0x3C) = 0;
        *(s32*)((s32)win + 0x34) = 0;
    }
}

void winMgrSetParam(s32 id, s32 param) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x2C) = param;
    }
}

s32 winMgrEntry(void* tpl) {
    void* win;
    s32 id;
    s32 zero;
    void* savedTpl;

    id = 0;
    win = *(void**)((s32)wp + 4);
    for (id = 0; id < *(s32*)wp; id++, win = (void*)((s32)win + 0x44)) {
        if ((*(u32*)win & 1) == 0) {
            break;
        }
    }

    zero = 0;
    *(s32*)((s32)win + 0) = zero;
    *(u32*)win |= 1;
    *(s32*)((s32)win + 4) = zero;
    *(s32*)((s32)win + 8) = zero;
    *(void**)((s32)win + 0x28) = tpl;
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x18) = *(s32*)((s32)savedTpl + 0xC);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x1C) = *(s32*)((s32)savedTpl + 0x10);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x20) = *(s32*)((s32)savedTpl + 0x14);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x24) = *(s32*)((s32)savedTpl + 0x18);
    *(s32*)((s32)win + 0x30) = zero;
    *(s32*)((s32)win + 0x2C) = zero;
    *(s32*)((s32)win + 0x38) = zero;
    *(s32*)((s32)win + 0x3C) = zero;
    return id;
}

void winMgrSeq(void* win) {
    return;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winMgrMain(void) {
    extern void winMgrSeq(void* win);
    extern void winMgrDisp(s32 cameraId, s32 unused, void* win);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* param, f32 z);

    void* win;
    void* tpl;
    void (*mainFunc)(void*);
    s32 i;

    i = 0;
    win = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        if ((*(u32*)win & 1) != 0) {
            winMgrSeq(win);
            tpl = *(void**)((s32)win + 0x28);
            mainFunc = *(void (**)(void*))((s32)tpl + 0x20);
            if (mainFunc != 0) {
                mainFunc(win);
            }
            if ((*(u32*)win & 2) != 0) {
                tpl = *(void**)((s32)win + 0x28);
                dispEntry(*(s32*)((s32)tpl + 8), 0, winMgrDisp, win, (f32)(*(s32*)((s32)win + 0x30) + 0x12C));
            }
        }
        i++;
        win = (void*)((s32)win + 0x44);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void winMgrDisp(s32 cameraId, s32 unused, void* win) {
    return;
}

void winMgrReInit(void) {
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

void winMgrInit(void) {
    *(s32*)wp = 0x20;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x44);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

s32 unk_8023f79c(void* event, s32 isFirstCall) {
    extern void* pouchGetPtr(void);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern u16 party_id_table[];

    s32* args;
    void* pouch;
    u16* party;
    s32 joined;
    s32 missing;
    s32 hasValue;
    s32 threshold;
    s32 value;

    args = *(s32**)((s32)event + 0x18);
    pouch = pouchGetPtr();
    joined = 0;
    missing = 0;
    hasValue = 0;
    party = party_id_table;
    while (*party != 0) {
        if (partyChkJoin(*party) != 0) {
            joined++;
            value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
            if (value != 0) {
                hasValue = 1;
                if (evtGetValue(event, 0xF8406022) == 0) {
                    threshold = value >= 1 ? 1 : 0;
                } else {
                    threshold = value >= 2 ? 2 : 0;
                }
                if (threshold == 0) {
                    missing = 1;
                }
            }
        }
        party++;
    }
    if (joined > 1 && missing != 0 && hasValue != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

s32 unk_8023f8d0(void* event, s32 isFirstCall) {
    extern void* pouchGetPtr(void);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern u16 party_id_table[];

    s32* args;
    void* pouch;
    u16* party;
    s32 value;
    s32 threshold;

    args = *(s32**)((s32)event + 0x18);
    pouch = pouchGetPtr();
    party = party_id_table;
    while (*party != 0) {
        if (partyChkJoin(*party) != 0) {
            if (evtGetValue(event, 0xF8406022) == 0) {
                value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
                threshold = value >= 1 ? 1 : 0;
            } else {
                value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
                threshold = value >= 2 ? 2 : 0;
            }
            if (threshold == 0) {
                evtSetValue(event, args[0], 1);
                return 2;
            }
        }
        party++;
    }
    evtSetValue(event, args[0], 0);
    return 2;
}

