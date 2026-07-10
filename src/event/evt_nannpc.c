#include "event/evt_nannpc.h"

extern void extLoadShadowMtx(void*);
extern u8 extLoadShadowRenderMode(void);
extern u8 extLoadShadowVertex(void);
extern u8 extLoadShadowTev(void);
extern u8 extDrawShadow(void);
extern void* nanNPCWork;
s32 evtGetValue(void* event, s32 arg);

typedef struct NanNpcSortEntry {
    f32 unk0;
    f32 z;
} NanNpcSortEntry;


u8 nanNPCOption(void) {
    return 0;
}


u8 evt_nannpc_jump_position(void) {
    return 0;
}


s32 evt_nannpc_init(void* param_1, int param_2) {
    return 0;
}


u8 nannpc_ext_disp(void) {
    return 0;
}


u8 evt_nannpc_move_position2(void) {
    return 0;
}


s32 evt_nannpc_entry(void* param_1) {
    return 0;
}


u8 nannpc_ext_main_sub_fast(void) {
    return 0;
}


s32 evt_nannpc_ppflag_onoff(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    u32 mask;
    s32 onoff;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    onoff = evtGetValue(pEvt, args[1]);
    mask = (u16)evtGetValue(pEvt, args[2]);
    if (onoff == 0) {
        **(u32**)(entry + 0x10) |= mask;
    } else {
        **(u32**)(entry + 0x10) &= ~mask;
    }
    return 2;
}

s32 evt_nannpc_flag_onoff(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    u16 mask;
    s32 onoff;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    onoff = evtGetValue(pEvt, args[1]);
    mask = (u16)evtGetValue(pEvt, args[2]);
    if (onoff == 0) {
        *(u16*)(entry + 0x14) |= mask;
    } else {
        *(u16*)(entry + 0x14) &= ~mask;
    }
    return 2;
}

s32 evt_nannpc_set_animtbl(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s16* tbl;
    s32 count;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    tbl = (s16*)evtGetValue(pEvt, args[1]);
    *(s16**)(entry + 0x58) = tbl;
    count = 0;
    while (*tbl != -1) {
        tbl++;
        count++;
    }
    *(s16*)(entry + 0x5C) = count;
    if (*(s16*)(entry + 0x5C) == 0) {
        *(s32*)(entry + 0x58) = 0;
    }
    return 2;
}

s32 evt_nannpc_set_color(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    entry[0xBC] = evtGetValue(pEvt, args[1]);
    entry[0xBD] = evtGetValue(pEvt, args[2]);
    entry[0xBE] = evtGetValue(pEvt, args[3]);
    entry[0xBF] = evtGetValue(pEvt, args[4]);
    return 2;
}

s32 evt_nannpc_set_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    *(f32*)(entry + 0x18) = x;
    *(f32*)(entry + 0x30) = x;
    *(f32*)(entry + 0x1C) = y;
    *(f32*)(entry + 0x34) = y;
    *(f32*)(entry + 0x20) = z;
    *(f32*)(entry + 0x38) = z;
    return 2;
}

s32 evt_nannpc_set_dir(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(f32*)(entry + 0x60) = (f32)evtGetValue(pEvt, args[1]);
    return 2;
}

s32 evt_nannpc_set_hosei_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(f32*)(entry + 0x24) = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x28) = evtGetFloat(pEvt, args[2]);
    *(f32*)(entry + 0x2C) = evtGetFloat(pEvt, args[3]);
    return 2;
}

s32 evt_nannpc_get_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern void evtSetFloat(void*, s32, f32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    evtSetFloat(pEvt, args[1], *(f32*)(entry + 0x18));
    evtSetFloat(pEvt, args[2], *(f32*)(entry + 0x1C));
    evtSetFloat(pEvt, args[3], *(f32*)(entry + 0x20));
    return 2;
}

s32 evt_nannpc_set_work(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 idx;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    idx = evtGetValue(pEvt, args[1]);
    *(s32*)(entry + idx * 4 + 0xA4) = evtGetValue(pEvt, args[2]);
    return 2;
}

s32 evt_nannpc_set_shadow_position(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    void* pp;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    pp = *(void**)(entry + 0x10);
    *(u32*)pp |= 0x10;
    *(f32*)((s32)pp + 8) = evtGetFloat(pEvt, args[1]);
    return 2;
}

s32 evt_nannpc_get_work(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern void evtSetValue(void*, s32, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 idx;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    idx = evtGetValue(pEvt, args[1]);
    evtSetValue(pEvt, args[2], *(s32*)(entry + idx * 4 + 0xA4));
    return 2;
}

s32 evt_nannpc_set_size(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern f32 evtGetFloat(void*, s32);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(f32*)(entry + 0x48) = evtGetFloat(pEvt, args[1]);
    *(f32*)(entry + 0x4C) = evtGetFloat(pEvt, args[2]);
    return 2;
}

s32 evt_nannpc_set_func(void* pEvt) {
    extern s32 sprintf(char*, char*, ...);
    extern s32 strcmp(char*, char*);
    extern char makestring[];
    extern char str_PCT06x_8042285c[];
    s32* args;
    s32* work;
    char* name;
    char* entry;
    s32 i;
    s32 offset;

    args = *(s32**)((s32)pEvt + 0x18);
    name = (char*)evtGetValue(pEvt, args[0]);
    work = (s32*)nanNPCWork;
    offset = 0;
    entry = NULL;
    for (i = 0; i < work[3]; i++, offset += 0xC0) {
        entry = (char*)(work[0] + offset);
        if ((s32)name >= 0) {
            sprintf(makestring, str_PCT06x_8042285c, name);
            name = makestring;
        }
        if (strcmp(entry, name) == 0) {
            break;
        }
        entry = NULL;
    }

    *(u32*)(entry + 0x64) = evtGetValue(pEvt, args[1]);
    return 2;
}

u8 nannpc_ext_init(void) {
    return 0;
}


s32 evt_nannpc_set_subfunc(void* pEvt) {
    s32* args;
    s32 type;

    args = *(s32**)((s32)pEvt + 0x18);
    type = evtGetValue(pEvt, args[0]);
    if (type == 0) {
        *(u32*)((s32)nanNPCWork + 0x14) = evtGetValue(pEvt, args[1]);
    } else if (type == 1) {
        *(u32*)((s32)nanNPCWork + 0x18) = evtGetValue(pEvt, args[1]);
    } else if (type == 2) {
        *(u32*)((s32)nanNPCWork + 0x1C) = evtGetValue(pEvt, args[1]);
    }
    return 2;
}

u8 nannpc_ext_dispent(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_nannpcwork_flag_onoff(void* pEvt) {
    s32* args;
    void* work;
    s32 onoff;
    u32 mask;

    args = *(s32**)((s32)pEvt + 0x18);
    onoff = evtGetValue(pEvt, args[0]);
    work = nanNPCWork;
    mask = (u16)evtGetValue(pEvt, args[1]);

    if (onoff != 0) {
        *(u32*)((s32)work + 0x10) &= ~mask;
    } else {
        *(u32*)((s32)work + 0x10) |= mask;
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 nannpc_ext_main_sub(void) {
    return 0;
}


u8 nannpc_ext_main(void) {
    return 0;
}


u8 evt_nannpc_set_animtbl_sub(int param_1, short* param_2) {
    s32 count = 0;

    *(short**)(param_1 + 0x58) = param_2;
loop:
    if (*param_2 == -1) {
        goto done;
    }
    param_2++;
    count++;
    goto loop;
done:
    *(s16*)(param_1 + 0x5C) = count;
    if (*(s16*)(param_1 + 0x5C) == 0) {
        *(s32*)(param_1 + 0x58) = 0;
    }
}


u8 nannpc_ext_shadow_disp(void) {
    static f32 mt[3][4];

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    return extDrawShadow();
}


s32 nannpc_zcompare(NanNpcSortEntry* param_1, NanNpcSortEntry* param_2) {
    if (param_1->z < param_2->z) {
        return 1;
    }
    if (param_1->z > param_2->z) {
        return -1;
    }
    return 0;
}
