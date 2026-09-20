#include "sdk/texPalette.h"


void* UnpackTexPalette(void* data) {
    typedef struct LocalCLUTHeader {
        u16 numEntries;
        u8 unpacked;
        u8 pad;
        s32 format;
        void* data;
    } LocalCLUTHeader;
    typedef struct LocalTEXHeader {
        u16 height;
        u16 width;
        u32 format;
        void* data;
        s32 wrapS;
        s32 wrapT;
        s32 minFilter;
        s32 magFilter;
        f32 lodBias;
        u8 edgeLODEnable;
        u8 minLOD;
        u8 maxLOD;
        u8 unpacked;
    } LocalTEXHeader;
    typedef struct LocalTEXDescriptor {
        LocalTEXHeader* textureHeader;
        LocalCLUTHeader* clutHeader;
    } LocalTEXDescriptor;
    typedef struct LocalTEXPalette {
        u32 versionNumber;
        u32 numDescriptors;
        LocalTEXDescriptor* descriptorArray;
    } LocalTEXPalette;
    extern void OSPanic(char*, int, char*, ...);
    LocalTEXPalette* palette = data;
    u16 i;

    if (palette->versionNumber != 0x20AF30) {
        OSPanic("texPalette.c", 0x24, "invalid version number for texture palette");
    }
    if ((u32)palette->descriptorArray <= 0x80000000) {
        palette->descriptorArray = (LocalTEXDescriptor*)((u8*)palette + (u32)palette->descriptorArray);
        for (i = 0; i < palette->numDescriptors; i++) {
            if (palette->descriptorArray[i].textureHeader != 0) {
                palette->descriptorArray[i].textureHeader =
                    (LocalTEXHeader*)((u8*)palette + (u32)palette->descriptorArray[i].textureHeader);
                if (palette->descriptorArray[i].textureHeader->unpacked == 0) {
                    palette->descriptorArray[i].textureHeader->data =
                        (u8*)palette + (u32)palette->descriptorArray[i].textureHeader->data;
                    palette->descriptorArray[i].textureHeader->unpacked = 1;
                }
            }
            if (palette->descriptorArray[i].clutHeader != 0) {
                palette->descriptorArray[i].clutHeader =
                    (LocalCLUTHeader*)((u8*)palette + (u32)palette->descriptorArray[i].clutHeader);
                if (palette->descriptorArray[i].clutHeader->unpacked == 0) {
                    palette->descriptorArray[i].clutHeader->data =
                        (u8*)palette + (u32)palette->descriptorArray[i].clutHeader->data;
                    palette->descriptorArray[i].clutHeader->unpacked = 1;
                }
            }
        }
    }
    return data;
}

void* TEXGet(void* tpl, s32 id) {
    return (void*)((s32)*(void**)((s32)tpl + 8) + id * 8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
