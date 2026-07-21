#include "sdk/texPalette.h"


void* UnpackTexPalette(void* data) {
    extern void OSPanic(char*, int, char*, ...);
    void* table;
    u16 i;

    if (*(u32*)data != 0x20AF30) {
        OSPanic("texPalette.c", 0x24, "invalid version numb");
    }

    table = *(void**)((s32)data + 8);
    if ((u32)table <= 0x80000000) {
        *(void**)((s32)data + 8) = (void*)((s32)data + (s32)table);
        for (i = 0; i < *(u32*)((s32)data + 4); i++) {
            void** pair;
            void* tex;
            void* pal;

            table = *(void**)((s32)data + 8);
            pair = (void**)((s32)table + i * 8);
            tex = pair[0];
            if (tex != 0) {
                tex = (void*)((s32)data + (s32)tex);
                pair[0] = tex;
                if (*(u8*)((s32)tex + 0x23) == 0) {
                    *(void**)((s32)tex + 8) = (void*)((s32)data + *(s32*)((s32)tex + 8));
                    *(u8*)((s32)pair[0] + 0x23) = 1;
                }
            }

            table = *(void**)((s32)data + 8);
            pair = (void**)((s32)table + i * 8);
            pal = pair[1];
            if (pal != 0) {
                pal = (void*)((s32)data + (s32)pal);
                pair[1] = pal;
                if (*(u8*)((s32)pal + 2) == 0) {
                    *(void**)((s32)pal + 8) = (void*)((s32)data + *(s32*)((s32)pal + 8));
                    *(u8*)((s32)pair[1] + 2) = 1;
                }
            }
        }
    }
    return data;
}

void* TEXGet(void* tpl, s32 id) {
    return (void*)((s32)*(void**)((s32)tpl + 8) + id * 8);
}

void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id) {
    extern void GXInitTexObj(void*, void*, u16, u16, s32, s32, s32, u8);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, u8, u8, s32);
    void* table;
    void* desc;
    u8 mipmap;

    table = *(void**)((s32)palette + 8);
    desc = *(void**)((s32)table + id * 8);
    if (*(u8*)((s32)desc + 0x21) == *(u8*)((s32)desc + 0x22)) {
        mipmap = 0;
    } else {
        mipmap = 1;
    }
    GXInitTexObj(texObj,
                 *(void**)((s32)desc + 8),
                 *(u16*)((s32)desc + 2),
                 *(u16*)desc,
                 *(s32*)((s32)desc + 4),
                 *(s32*)((s32)desc + 0xC),
                 *(s32*)((s32)desc + 0x10),
                 mipmap);
    desc = *(void**)((s32)table + id * 8);
    GXInitTexObjLOD(texObj,
                    *(s32*)((s32)desc + 0x14),
                    *(s32*)((s32)desc + 0x18),
                    (f32)*(u8*)((s32)desc + 0x21),
                    (f32)*(u8*)((s32)desc + 0x22),
                    *(f32*)((s32)desc + 0x1C),
                    0,
                    0,
                    *(u8*)((s32)desc + 0x20));
}

