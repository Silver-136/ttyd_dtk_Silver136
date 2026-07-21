#include "manager/filemgr.h"

typedef struct {
    u32 reg_info;
    void* input_arg_area;
    void* reg_save_area;
} va_list[1];

extern void _fileAlloc();
extern void* afp;
extern char _filename[];
extern int vsprintf(char* buffer, const char* format, void* arg);
extern void* __memAlloc(s32 heap, u32 size);
extern void* memmove(void* dst, const void* src, u32 size);
extern void DVDMgrClose(void* entry);
char* fileAsync(char* name, s32 kind, s32 callback);
u8 fileInit(void) {
    void* mem;
    u8* file;
    s32 i;

    mem = __memAlloc(0, 0x2C000);
    *(void**)afp = mem;
    *(s32*)((s32)afp + 4) = 0;
    *(s32*)((s32)afp + 8) = 0;
    *(s32*)((s32)afp + 0xC) = 0;
    memset(*(void**)afp, 0, 0x2C000);

    file = *(u8**)afp;
    for (i = 0; i < 0x80; i++, file += 0x580) {
        *(u8**)(file + 0xA4) = file + 0xB0;
        *(u8**)(file + 0x154) = file + 0x160;
        *(u8**)(file + 0x204) = file + 0x210;
        *(u8**)(file + 0x2B4) = file + 0x2C0;
        *(u8**)(file + 0x364) = file + 0x370;
        *(u8**)(file + 0x414) = file + 0x420;
        *(u8**)(file + 0x4C4) = file + 0x4D0;
        *(u8**)(file + 0x574) = file + 0x580;
    }

    *(void**)((s32)afp + 0x10) = *(void**)afp;
    *(u8**)((s32)afp + 0x14) = (u8*)*(void**)afp + 0x2BF50;
    *(s32*)(*(s32*)((s32)afp + 0x14) + 0xA4) = 0;
}


void fileGarbageDataAdrClear(int fileEntry) {
    char* data = **(char***)(fileEntry + 0xA0);
    int kind = *(u8*)(fileEntry + 1);
    int i;
    int j;
    char* item;
    char** field;

    switch (kind) {
        case 1:
            field = (char**)(data + 0x14C);
            if (*field >= data) {
                field[0] -= (int)data;
                field[1] -= (int)data;
                field[2] -= (int)data;
                field[3] -= (int)data;
                field[4] -= (int)data;
                field[5] -= (int)data;
                field[6] -= (int)data;
                field[7] -= (int)data;
                field[8] -= (int)data;
                field[9] -= (int)data;
                field[10] -= (int)data;
                field[11] -= (int)data;
                field[12] -= (int)data;
                field[13] -= (int)data;
                field[14] -= (int)data;
                field[15] -= (int)data;
                field[16] -= (int)data;
                field[17] -= (int)data;
                field[18] -= (int)data;
                field[19] -= (int)data;
                field[20] -= (int)data;
                field[21] -= (int)data;
                field[22] -= (int)data;
                field[23] -= (int)data;
                field[24] -= (int)data;
            }
            break;
        case 5:
            if (kind == 5) {
                for (i = 0; i < *(int*)(data + 0x148); i++) {
                    field = (char**)(*(char**)(data + 0x1AC) + i * 0x40 + 0x3C);
                    item = *field;
                    if (*(char**)(item + 0x24) >= item) {
                        for (j = 0x24; j <= 0x40; j += 4) {
                            *(char**)(item + j) -= (int)item;
                        }
                    }
                    *field -= (int)data;
                }
            }
            field = (char**)(data + 0x14C);
            if (*field >= data) {
                for (i = 0; i < 25; i++, field++) {
                    *field -= (int)data;
                }
            }
            break;
        case 2:
            field = (char**)(data + 0x24);
            if (*field >= data) {
                for (i = 0; i < 8; i++, field++) {
                    *field -= (int)data;
                }
            }
            break;
        case 3:
        case 6:
        case 10:
            field = (char**)(data + 0x48);
            if (*field >= data) {
                *field -= (int)data;
            }
            break;
        case 4:
            field = (char**)(data + 8);
            if (*field >= data) {
                for (i = 0; (u16)i < *(u32*)(data + 4); i++) {
                    for (j = 0; j < 2; j++) {
                        field = (char**)(*(char**)(data + 8) + i * 8 + j * 4);
                        item = *field;
                        if (item != 0) {
                            if (*(u8*)(item + (j == 0 ? 0x23 : 2)) != 0) {
                                *(char**)(item + 8) -= (int)data;
                                *(u8*)(item + (j == 0 ? 0x23 : 2)) = 0;
                            }
                            *field -= (int)data;
                        }
                    }
                }
                *(char**)(data + 8) -= (int)data;
            }
            break;
        case 7:
            field = (char**)(data + 0x64);
            if (*field >= data) {
                for (i = 0; i < 3; i++, field++) {
                    *field -= (int)data;
                }
            }
            break;
        case 8:
            field = (char**)(data + 0xC8);
            if (*field >= data) {
                for (i = 0; i < 23; i++, field++) {
                    *field -= (int)data;
                }
            }
            break;
        case 9:
            field = (char**)(data + 0x58);
            if (*field >= data) {
                for (i = 0; i < 4; i++, field++) {
                    *field -= (int)data;
                }
            }
            break;
    }
}

void fileGarbageDataAdrSet(char* data, int kind) {
    extern void OSPanic(char*, int, char*);
    extern char str_filemgr_c_802f9470[];
    extern char str_invalid_version_numb_802f947c[];
    int i;
    int j;
    char* item;
    char** field;

    switch (kind) {
        case 1:
            field = (char**)(data + 0x14C);
            if (*field < data) {
                field[0] += (int)data;
                field[1] += (int)data;
                field[2] += (int)data;
                field[3] += (int)data;
                field[4] += (int)data;
                field[5] += (int)data;
                field[6] += (int)data;
                field[7] += (int)data;
                field[8] += (int)data;
                field[9] += (int)data;
                field[10] += (int)data;
                field[11] += (int)data;
                field[12] += (int)data;
                field[13] += (int)data;
                field[14] += (int)data;
                field[15] += (int)data;
                field[16] += (int)data;
                field[17] += (int)data;
                field[18] += (int)data;
                field[19] += (int)data;
                field[20] += (int)data;
                field[21] += (int)data;
                field[22] += (int)data;
                field[23] += (int)data;
                field[24] += (int)data;
            }
            break;
        case 5:
            field = (char**)(data + 0x14C);
            if (*field < data) {
                for (i = 0; i < 25; i++, field++) {
                    *field += (int)data;
                }
                if (kind == 5) {
                    for (i = 0; i < *(int*)(data + 0x148); i++) {
                        field = (char**)(*(char**)(data + 0x1AC) + i * 0x40 + 0x3C);
                        *field += (int)data;
                        item = *field;
                        if (*(char**)(item + 0x24) < item) {
                            for (j = 0x24; j <= 0x40; j += 4) {
                                *(char**)(item + j) += (int)item;
                            }
                        }
                    }
                }
            }
            break;
        case 2:
            field = (char**)(data + 0x24);
            if (*field < data) {
                for (i = 0; i < 8; i++, field++) {
                    *field += (int)data;
                }
            }
            break;
        case 3:
        case 6:
        case 10:
            field = (char**)(data + 0x48);
            if (*field < data) {
                *field += (int)data;
            }
            break;
        case 4:
            if (*(int*)data != 0x20AF30) {
                OSPanic(str_filemgr_c_802f9470, 0x70, str_invalid_version_numb_802f947c);
            }
            field = (char**)(data + 8);
            if (*field < data) {
                *field += (int)data;
                for (i = 0; (u16)i < *(u32*)(data + 4); i++) {
                    for (j = 0; j < 2; j++) {
                        field = (char**)(*(char**)(data + 8) + i * 8 + j * 4);
                        if (*field != 0) {
                            *field += (int)data;
                            item = *field;
                            if (*(u8*)(item + (j == 0 ? 0x23 : 2)) == 0) {
                                *(char**)(item + 8) += (int)data;
                                *(u8*)(item + (j == 0 ? 0x23 : 2)) = 1;
                            }
                        }
                    }
                }
            }
            break;
        case 7:
            field = (char**)(data + 0x64);
            if (*field < data) {
                for (i = 0; i < 3; i++, field++) {
                    *field += (int)data;
                }
            }
            break;
        case 8:
            field = (char**)(data + 0xC8);
            if (*field < data) {
                for (i = 0; i < 23; i++, field++) {
                    *field += (int)data;
                }
            }
            break;
        case 9:
            field = (char**)(data + 0x58);
            if (*field < data) {
                for (i = 0; i < 4; i++, field++) {
                    *field += (int)data;
                }
            }
            break;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void fileGarbageMoveMem(void* param_1, char* param_2) {
    void* data;
    void* dvd;

    if (*(u8*)param_2 == 3) {
        if (*(void**)((s32)param_2 + 0xAC) == NULL) {
            fileGarbageDataAdrClear((s32)param_2);
        }
    }
    else {
        fileGarbageDataAdrClear((s32)param_2);
    }

    memmove(param_1, **(void***)((s32)param_2 + 0xA0), *(u32*)(*(s32*)((s32)param_2 + 0xA0) + 4));
    data = *(void**)((s32)param_2 + 0xA0);
    *(void**)data = param_1;

    if (*(u8*)param_2 == 3) {
        dvd = *(void**)((s32)param_2 + 0xAC);
        if (dvd != NULL) {
            *(void**)((s32)dvd + 0x7C) = param_1;
        }
        else {
            fileGarbageDataAdrSet(**(char***)((s32)param_2 + 0xA0), *(s8*)(param_2 + 1));
        }
    }
    else {
        fileGarbageDataAdrSet(**(char***)((s32)param_2 + 0xA0), *(s8*)(param_2 + 1));
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 _fileGarbage(int mode) {
    extern void* afp;
    extern char dvdmgr_thread[];
    extern void* OSGetCurrentThread(void);
    extern void OSYieldThread(void);
    extern void smartFree(void*);

    u8* file;
    u8* prev;
    u8* first;
    u8* next;

    if (mode == 0) {
        file = *(u8**)((s32)afp + 8);
        while (file != NULL) {
            if (*file == 3 && *(void**)(file + 0xAC) != NULL) {
                if (OSGetCurrentThread() == dvdmgr_thread) {
                    *(s16*)((s32)*(void**)(file + 0xAC) + 0x90) = 8;
                }
                else {
                    while (*(void**)(file + 0xAC) != NULL) {
                        OSYieldThread();
                    }
                }
                *file = 2;
            }
            file = *(u8**)(file + 0xA4);
        }
    }
    else if (mode == 1) {
        file = *(u8**)((s32)afp + 8);
        while (file != NULL) {
            if (*file == 3) {
                if (*(void**)(file + 0xAC) != NULL) {
                    if (OSGetCurrentThread() != dvdmgr_thread) {
                        while (*(void**)(file + 0xAC) != NULL) {
                            OSYieldThread();
                        }
                    }
                }
                else {
                    (*(s16*)(file + 2))--;
                    if (*(s16*)(file + 2) <= -2) {
                        *file = 2;
                    }
                }
            }
            file = *(u8**)(file + 0xA4);
        }
    }

    prev = NULL;
    first = NULL;
    file = *(u8**)((s32)afp + 8);
    while (file != NULL) {
        next = *(u8**)(file + 0xA4);
        if (*file == 2) {
            *file = 0;
            if (file + 4 != *(u8**)(file + 0xA0)) {
                smartFree(*(void**)(file + 0xA0));
            }
            if (prev != NULL) {
                *(u8**)(prev + 0xA4) = *(u8**)(file + 0xA4);
            }
            if (*(void**)((s32)afp + 0x10) == NULL) {
                *(u8**)((s32)afp + 0x10) = file;
            }
            else {
                *(u8**)(*(s32*)((s32)afp + 0x14) + 0xA4) = file;
            }
            *(u8**)(file + 0xA4) = NULL;
            *(u8**)((s32)afp + 0x14) = file;
        }
        else {
            if (prev == NULL) {
                first = file;
            }
            prev = file;
        }
        file = next;
    }
    *(u8**)((s32)afp + 8) = first;
    *(u8**)((s32)afp + 0xC) = prev;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* fileAllocf(s32 size, const char* fmt, ...) {
    va_list args;
    void* ap;

    ap = args;
    args[0].reg_info = 0x02000000;
    args[0].input_arg_area = (u8*)args + 0x30;
    args[0].reg_save_area = (u8*)args - 0x60;
    vsprintf(_filename, fmt, ap);
    _fileAlloc(_filename, size, 0);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void fileAlloc(void* name, unsigned long size) {
    _fileAlloc(name, size, 0);
}

void _fileAlloc(char* name, s32 kind, s32 unused) {
    extern void* afp;
    extern s32 strcmp(char*, char*);
    extern void OSYieldThread(void);
    extern void _fileGarbage(s32);
    extern void* arcOpen(char*, void*, void*);
    extern void fileGarbageDataAdrSet(char*, s32);
    extern void* DVDMgrOpen(char*, s32, s32);
    extern u32 DVDMgrGetLength(void*);
    extern void DVDMgrClose(void*);
    extern void* smartAlloc(u32, s32);
    extern char* strcpy(char*, const char*);
    extern void DVDMgrRead(void*, void*, u32, s32);

    char* file;
    char* next;
    char* data;
    void* dvd;
    void* alloc;
    u32 len;
    u32 aligned;
    s32 archiveType;

    for (file = *(char**)((s32)afp + 8); file != 0; file = *(char**)(file + 0xA4)) {
        if (*file != 0 && strcmp(file + 0x20, name) == 0) {
            while (*(void**)(file + 0xAC) != 0) {
                OSYieldThread();
            }
            if (*file == 3) {
                *file = 1;
                *(s16*)(file + 2) = 0;
            } else if (*file == 2) {
                *file = 1;
            }
            (*(s16*)(file + 2))++;
            return;
        }
    }

    file = *(char**)((s32)afp + 0x10);
    if (file == 0) {
        _fileGarbage(1);
        file = *(char**)((s32)afp + 0x10);
    }
    if (file == 0) {
        _fileGarbage(0);
        file = *(char**)((s32)afp + 0x10);
    }

    data = 0;
    next = *(char**)(file + 0xA4);
    archiveType = *(s32*)((s32)afp + 4);
    if (archiveType == 1) {
        data = arcOpen(name, 0, 0);
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
    } else if (archiveType < 1) {
        if (archiveType > -1) {
            data = arcOpen(name, 0, 0);
        }
    } else if (archiveType < 3) {
        data = arcOpen(name, 0, 0);
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
    }

    if (data == 0) {
        dvd = DVDMgrOpen(name, 2, 0);
        if (dvd == 0) {
            return;
        }
        len = DVDMgrGetLength(dvd);
        aligned = len & 0x1F;
        aligned = len + ((0x20 - aligned) & ((s32)(-aligned | aligned) >> 31));
        if (aligned == 0) {
            DVDMgrClose(dvd);
            return;
        }
        if ((aligned & 0x1F) != 0) {
            aligned += 0x20 - (aligned & 0x1F);
        }
        alloc = smartAlloc(len, 0);
        *(void**)((s32)alloc + 8) = file;
        DVDMgrRead(dvd, *(void**)alloc, aligned, 0);
        DVDMgrClose(dvd);
        *(void**)(file + 0xA0) = alloc;
        *file = 1;
        *(s16*)(file + 2) = 1;
        *(s8*)(file + 1) = kind;
        *(void**)(file + 0xA4) = 0;
        strcpy(file + 0x20, name);
        *(void**)(file + 0xAC) = 0;
        fileGarbageDataAdrSet(*(char**)alloc, kind);
    } else {
        fileGarbageDataAdrSet(data, kind);
        *file = 1;
        *(void**)(file + 0xA0) = file + 4;
        **(void***)(file + 0xA0) = data;
        *(s16*)(file + 2) = 1;
        *(s8*)(file + 1) = kind;
        *(void**)(file + 0xA4) = 0;
        strcpy(file + 0x20, name);
        *(void**)(file + 0xAC) = 0;
    }

    *(char**)((s32)afp + 0x10) = next;
    if (*(void**)((s32)afp + 0x10) == 0) {
        *(void**)((s32)afp + 0x14) = 0;
    }
    if (*(void**)((s32)afp + 8) == 0) {
        *(void**)((s32)afp + 8) = file;
        *(void**)((s32)afp + 0xC) = file;
    } else {
        *(void**)(*(s32*)((s32)afp + 0xC) + 0xA4) = file;
        *(void**)((s32)afp + 0xC) = file;
    }
}


void fileFree(void* file) {
    if (file == 0) {
        return;
    }
    if (*(void**)((s32)file + 0xA0) == 0) {
        return;
    }
    if (*(u8*)file != 1) {
        return;
    }
    (*(s16*)((s32)file + 2))--;
    if (*(s16*)((s32)file + 2) == 0) {
        *(u8*)file = 2;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 dvdReadDoneCallBack(int param_1, u32 param_2) {
    char* file;
    void* dvd;
    void (*callback)(void*);

    file = *(char**)((s32)afp + 8);
    while (file != NULL) {
        dvd = *(void**)((s32)file + 0xAC);
        if ((s32)dvd + 0x40 == param_2) {
            break;
        }
        file = *(char**)((s32)file + 0xA4);
    }

    dvd = *(void**)((s32)file + 0xAC);
    if (param_1 == -3) {
        *(void**)((s32)file + 0xAC) = NULL;
        file[0] = 2;
        file[1] = 0xB;
        DVDMgrClose(dvd);
    }
    else {
        *(void**)((s32)file + 0xAC) = NULL;
        fileGarbageDataAdrSet(**(char***)((s32)file + 0xA0), *(s8*)(file + 1));
        DVDMgrClose(dvd);
        callback = *(void (**)(void*))((s32)file + 0xA8);
        if (callback != NULL) {
            callback(file);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 fileAsyncf(s32 id, s32 flags, const char* fmt, ...) {
    va_list args;
    void* ap;

    ap = args;
    args[0].reg_info = 0x03000000;
    args[0].input_arg_area = (u8*)args + 0x30;
    args[0].reg_save_area = (u8*)args - 0x60;
    vsprintf(_filename, fmt, ap);
    fileAsync(_filename, id, flags);
}

char* fileAsync(char* name, s32 kind, s32 callback) {
    extern void* afp;
    extern s32 strcmp(char*, char*);
    extern void _fileGarbage(s32);
    extern void* arcOpen(char*, void*, void*);
    extern void fileGarbageDataAdrSet(char*, s32);
    extern void* DVDMgrOpen(char*, s32, s32);
    extern u32 DVDMgrGetLength(void*);
    extern void DVDMgrClose(void*);
    extern void* smartAlloc(u32, s32);
    extern char* strcpy(char*, const char*);
    extern void DVDMgrReadAsync(void*, void*, u32, s32, void*);
    extern void dvdReadDoneCallBack(void);

    char* file;
    char* next;
    char* data;
    void* dvd;
    void* alloc;
    u32 len;
    u32 aligned;
    s32 archiveType;

    file = *(char**)((s32)afp + 8);
    while (file != 0) {
        if (*file != 0 && strcmp(file + 0x20, name) == 0) {
            if (*(void**)(file + 0xAC) != 0) {
                return 0;
            }
            if (*file == 3) {
                return file;
            }
            if (*file == 2) {
                *file = 3;
            }
            return file;
        }
        file = *(char**)(file + 0xA4);
    }

    file = *(char**)((s32)afp + 0x10);
    if (file == 0) {
        _fileGarbage(1);
        file = *(char**)((s32)afp + 0x10);
    }
    if (file == 0) {
        _fileGarbage(0);
        file = *(char**)((s32)afp + 0x10);
    }

    data = 0;
    next = *(char**)(file + 0xA4);
    archiveType = *(s32*)((s32)afp + 4);
    if (archiveType == 1) {
        data = arcOpen(name, 0, 0);
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
    } else if (archiveType < 1) {
        if (archiveType > -1) {
            data = arcOpen(name, 0, 0);
        }
    } else if (archiveType < 3) {
        data = arcOpen(name, 0, 0);
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
        if (data == 0) {
            data = arcOpen(name, 0, 0);
        }
    }

    if (data == 0) {
        dvd = DVDMgrOpen(name, 2, 0);
        if (dvd == 0) {
            return (char*)-1;
        }
        len = DVDMgrGetLength(dvd);
        aligned = len & 0x1F;
        aligned = len + ((0x20 - aligned) & ((s32)(-aligned | aligned) >> 31));
        if (aligned == 0) {
            DVDMgrClose(dvd);
            return 0;
        }
        DVDMgrClose(dvd);
        if ((aligned & 0x1F) != 0) {
            aligned += 0x20 - (aligned & 0x1F);
        }
        alloc = smartAlloc(len, 0);
        *(void**)((s32)alloc + 8) = file;
        *(void**)(file + 0xA0) = alloc;
        *file = 3;
        *(s16*)(file + 2) = 0;
        *(s8*)(file + 1) = kind;
        *(void**)(file + 0xA4) = 0;
        *(void**)(file + 0xA8) = (void*)callback;
        strcpy(file + 0x20, name);
        *(char**)((s32)afp + 0x10) = next;
        if (*(void**)((s32)afp + 0x10) == 0) {
            *(void**)((s32)afp + 0x14) = 0;
        }
        if (*(void**)((s32)afp + 8) == 0) {
            *(void**)((s32)afp + 8) = file;
            *(void**)((s32)afp + 0xC) = file;
        } else {
            *(void**)(*(s32*)((s32)afp + 0xC) + 0xA4) = file;
            *(void**)((s32)afp + 0xC) = file;
        }
        dvd = DVDMgrOpen(name, 2, 0);
        *(void**)(file + 0xAC) = dvd;
        DVDMgrReadAsync(dvd, *(void**)alloc, aligned, 0, dvdReadDoneCallBack);
        return 0;
    }

    fileGarbageDataAdrSet(data, kind);
    *file = 3;
    *(void**)(file + 0xA0) = file + 4;
    **(void***)(file + 0xA0) = data;
    *(s16*)(file + 2) = 0;
    *(s8*)(file + 1) = kind;
    *(void**)(file + 0xA4) = 0;
    strcpy(file + 0x20, name);
    *(void**)(file + 0xAC) = 0;
    *(char**)((s32)afp + 0x10) = *(char**)(*(s32*)((s32)afp + 0x10) + 0xA4);
    if (*(void**)((s32)afp + 0x10) == 0) {
        *(void**)((s32)afp + 0x14) = 0;
    }
    if (*(void**)((s32)afp + 8) == 0) {
        *(void**)((s32)afp + 8) = file;
        *(void**)((s32)afp + 0xC) = file;
    } else {
        *(void**)(*(s32*)((s32)afp + 0xC) + 0xA4) = file;
        *(void**)((s32)afp + 0xC) = file;
    }
    return file;
}

void fileSetCurrentArchiveType(s32 type) {
    extern void* afp;
    ((s32*)afp)[1] = type;
}
