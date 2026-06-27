#include "driver/windowdrv.h"

void* winTpl;
void* wakuTexObj;
void* WinObjects;
s32 unk_8041ea1c;

void* __memAlloc(s32 heap, u32 size);

void* getWakuTexObj(s32 id) {
    return (void*)((s32)wakuTexObj + id * 0x20);
}

void* windowGetPointer(s32 id) {
    return (void*)((s32)WinObjects + id * 0x48);
}

s32 windowCheckID(s32 id) {
    return *(u16*)((s32)WinObjects + id * 0x48 + 2) & 2;
}

s32 windowDelete(void* win) {
    void (*cb)(void*) = *(void (**) (void*))((s32)win + 0x40);
    if (cb != 0) {
        cb(win);
    }
    *(u16*)((s32)win + 2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    return 1;
}

s32 windowDeleteID(s32 id) {
    void* win = (void*)((s32)WinObjects + id * 0x48);
    void (*cb)(void*);
    if ((*(u16*)((s32)win + 2) & 1) == 0) {
        return 0;
    }
    cb = *(void (**) (void*))((s32)win + 0x40);
    if (cb != 0) {
        cb(win);
    }
    *(u16*)((s32)win + 2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    return 1;
}

void windowReInit(void) {
    void* win = WinObjects;
    *(u16*)((s32)win + 0x2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    *(u16*)((s32)win + 0x4A) = 0;
    *(s32*)((s32)win + 0x70) = 0;
    *(u16*)((s32)win + 0x92) = 0;
    *(s32*)((s32)win + 0xB8) = 0;
    *(u16*)((s32)win + 0xDA) = 0;
    *(s32*)((s32)win + 0x100) = 0;
    *(u16*)((s32)win + 0x122) = 0;
    *(s32*)((s32)win + 0x148) = 0;
    *(u16*)((s32)win + 0x16A) = 0;
    *(s32*)((s32)win + 0x190) = 0;
    *(u16*)((s32)win + 0x1B2) = 0;
    *(s32*)((s32)win + 0x1D8) = 0;
}

void windowInit(void) {
    void* win = __memAlloc(0, 0x1F8);
    WinObjects = win;
    *(u16*)((s32)win + 0x2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    *(u16*)((s32)win + 0x4A) = 0;
    *(s32*)((s32)win + 0x70) = 0;
    *(u16*)((s32)win + 0x92) = 0;
    *(s32*)((s32)win + 0xB8) = 0;
    *(u16*)((s32)win + 0xDA) = 0;
    *(s32*)((s32)win + 0x100) = 0;
    *(u16*)((s32)win + 0x122) = 0;
    *(s32*)((s32)win + 0x148) = 0;
    *(u16*)((s32)win + 0x16A) = 0;
    *(s32*)((s32)win + 0x190) = 0;
    *(u16*)((s32)win + 0x1B2) = 0;
    *(s32*)((s32)win + 0x1D8) = 0;
    winTpl = 0;
    unk_8041ea1c = 0;
}


u8 _windowDispGX_Message(void) {
    return 0;
}


u8 windowDispGX_System(void) {
    return 0;
}


u8 windowDispGX_Kanban(void) {
    return 0;
}


u8 windowDispGX2_Waku_col(s64 param_1, s64 param_2, s64 param_3, s64 param_4, s64 param_5, s32 param_6, s16 param_7, s32 color) {
    return 0;
}


void windowDispGX_Waku_col(double x, double y, double width, double height, double curve, u16 gxTexMapID, u32* color) {
    ;
}


u8 windowDispGX_Message(void) {
    return 0;
}


void windowTexSetup(void) {
    extern void* arcOpen(const char* filename, void** address, u32* length);
    extern char* getMarioStDvdRoot(void);
    extern s32 sprintf(char* str, const char* format, ...);
    extern void* DVDMgrOpen(const char* path, s32 mode, s32 unk);
    extern u32 DVDMgrGetLength(void* entry);
    extern void DVDMgrReadAsync(void* entry, void* dst, u32 size, u32 offset, void* callback);
    extern void* UnpackTexPalette(void* data);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void unk_8007fcf0(void* param_1, void* param_2);
    extern const char str_font_msgWindow_tpl_802c3080[];
    extern const char str_PCTs_f_msgWindow_tpl_802c3094[];
    char path[128];
    void* entry;
    u32 length;
    u32 i;
    u32 offset;

    winTpl = NULL;
    unk_8041ea1c = 0;
    winTpl = arcOpen(str_font_msgWindow_tpl_802c3080, NULL, NULL);
    if (winTpl == NULL) {
        sprintf(path, str_PCTs_f_msgWindow_tpl_802c3094, getMarioStDvdRoot());
        entry = DVDMgrOpen(path, 2, 0);
        length = (DVDMgrGetLength(entry) + 0x1F) & ~0x1F;
        winTpl = __memAlloc(0, length);
        *(void**)((s32)entry + 0x6C) = entry;
        DVDMgrReadAsync(entry, winTpl, length, 0, unk_8007fcf0);
    } else {
        UnpackTexPalette(winTpl);
        wakuTexObj = __memAlloc(0, *(u32*)((s32)winTpl + 4) << 5);
        for (i = 0, offset = 0; i < *(u32*)((s32)winTpl + 4); offset += 0x20, i++) {
            TEXGetGXTexObjFromPalette(winTpl, (void*)((s32)wakuTexObj + offset), i);
        }
    }
}

int windowEntry(s16 param_1) {
    void* win = WinObjects;
    s32 i;

    for (i = 0; i < 7; i++, win = (void*)((s32)win + 0x48)) {
        if ((*(u16*)((s32)win + 2) & 1) == 0) {
            memset(win, 0, 0x48);
            *(u16*)((s32)win + 2) = 1;
            *(u16*)((s32)win + 4) = param_1;
            return i;
        }
    }
    return -1;
}

u8 windowMain(void) {
    return 0;
}

void unk_8007fcf0(void* param_1, void* param_2) {
    extern void* UnpackTexPalette(void* data);
    extern void DVDMgrClose(void* entry);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    void* entry = *(void**)((s32)param_2 + 0x2C);
    u32 offset;
    u32 i;

    UnpackTexPalette(winTpl);
    DVDMgrClose(entry);
    wakuTexObj = __memAlloc(0, *(u32*)((s32)winTpl + 4) << 5);
    i = 0;
    offset = 0;
    while (i < *(u32*)((s32)winTpl + 4)) {
        TEXGetGXTexObjFromPalette(winTpl, (void*)((s32)wakuTexObj + offset), i);
        offset += 0x20;
        i++;
    }
    unk_8041ea1c = 1;
}
