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

void fileAlloc(void* name, unsigned long size) {
    _fileAlloc(name, size, 0);
}

void fileSetCurrentArchiveType(s32 type) {
    extern void* afp;
    ((s32*)afp)[1] = type;
}


u8 fileGarbageDataAdrClear(int param_1) {
    return 0;
}


u8 fileGarbageDataAdrSet(char* pFileData, int fileKind) {
    return 0;
}


char* fileAsync(char* param_1, s32 param_2, s32 param_3) {
    return 0;
}


void _fileAlloc() {
}


u8 _fileGarbage(int param_1) {
    return 0;
}


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


void fileFree(void*) {
}
