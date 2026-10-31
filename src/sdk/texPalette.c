#include <charPipeline/texPalette.h>
#include <dolphin/gx/GXTexture.h>
#include <dolphin/os.h>
#include <macros.h>

#define PALETTE_VERSION 0x0020AF30

/**
 * Turns a TPL loaded in memory into usable form:
 * checks the version and converts the descriptor, texture and CLUT offsets to pointers (only once).
 * Used by the effect and background drivers after loading a .tpl.
 */
void UnpackTexPalette(TEXPalettePtr pal) {
    u16 i;

    if (pal->versionNumber != PALETTE_VERSION) {
        OSPanic(__FILE__, 0x24, "invalid version number for texture palette\0");
    }

    if ((u32)pal->descriptorArray <= 0x80000000) {
        pal->descriptorArray = (TEXDescriptorPtr)((Ptr)pal->descriptorArray + (u32)pal);
        for (i = 0; i < pal->numDescriptors; i++) {
            if (pal->descriptorArray[i].textureHeader) {
                pal->descriptorArray[i].textureHeader =
                    (TEXHeaderPtr)((Ptr)pal + (u32)pal->descriptorArray[i].textureHeader);
                if (!pal->descriptorArray[i].textureHeader->unpacked) {
                    pal->descriptorArray[i].textureHeader->data =
                        (Ptr)pal + (u32)pal->descriptorArray[i].textureHeader->data;
                    pal->descriptorArray[i].textureHeader->unpacked = TRUE;
                }
            }
            if (pal->descriptorArray[i].CLUTHeader) {
                pal->descriptorArray[i].CLUTHeader =
                    (CLUTHeaderPtr)((Ptr)pal + (u32)pal->descriptorArray[i].CLUTHeader);
                if (!pal->descriptorArray[i].CLUTHeader->unpacked) {
                    pal->descriptorArray[i].CLUTHeader->data = (Ptr)pal + (u32)pal->descriptorArray[i].CLUTHeader->data;
                    pal->descriptorArray[i].CLUTHeader->unpacked = TRUE;
                }
            }
        }
    }
}

/** Returns the descriptor for texture id (the range check is a debug-only assert). */
TEXDescriptorPtr TEXGet(TEXPalettePtr pal, u32 id) {
    ASSERTMSGLINE(0x90, id < pal->numDescriptors, "GetTexture():  Texture Not Found ");
    return &pal->descriptorArray[id];
}

/**
 * Fills a GXTexObj from texture id of a TPL: size, format, wrap,
 * filters and LOD range (mipmapped when the LODs differ).
 */
void TEXGetGXTexObjFromPalette(TEXPalettePtr pal, GXTexObj* to, u32 id) {
    TEXDescriptorPtr tdp;
    GXBool mipMap;

    tdp = TEXGet(pal, id);
    if (tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD) {
        mipMap = GX_FALSE;
    } else {
        mipMap = GX_TRUE;
    }
    GXInitTexObj(to, tdp->textureHeader->data, tdp->textureHeader->width, tdp->textureHeader->height,
                 tdp->textureHeader->format, tdp->textureHeader->wrapS, tdp->textureHeader->wrapT, mipMap);
    GXInitTexObjLOD(to, tdp->textureHeader->minFilter, tdp->textureHeader->magFilter, tdp->textureHeader->minLOD,
                    tdp->textureHeader->maxLOD, tdp->textureHeader->LODBias, GX_DISABLE,
                    tdp->textureHeader->edgeLODEnable, GX_ANISO_1);
}
