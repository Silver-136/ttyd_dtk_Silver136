#include "driver/imgdrv.h"

extern void* wp;

void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int value, u32 size);

void imgSetShadow(void* img, s32 shadow) {
    *(s32*)((s32)img + 0x100) = shadow;
}

void imgClearVirtualPoint(void* img) {
    *(u32*)((s32)img + 0xCC) |= 8;
}

void imgSetVirtualPoint(void* img, void* point) {
    u32 flags;
    u32 y;
    u32 x;
    u32 z;

    flags = *(u32*)((s32)img + 0xCC);
    x = *(u32*)((s32)point + 0);
    flags &= ~8;
    y = *(u32*)((s32)point + 4);
    *(u32*)((s32)img + 0xCC) = flags;
    z = *(u32*)((s32)point + 8);
    *(u32*)((s32)img + 0xF0) = x;
    *(u32*)((s32)img + 0xF4) = y;
    *(u32*)((s32)img + 0xF8) = z;
}

void imgInit(void) {
    *(s32*)wp = 10;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x158);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x158);
}


u8 imgDisp(void) {
    return 0;
}


u8 imgShadowDisp(void) {
    return 0;
}


u8 imgCapture_Prim(int param_1, int param_2) {
    return 0;
}


void zFill(void) {
    extern void* camGetCurPtr(void);
    extern f64 tan(f64 x);
    extern void GXInitTexObj(void* obj, void* image, u16 width, u16 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 stages);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetZTexture(s32 op, s32 fmt, u32 bias);
    extern void GXSetNumChans(s32 count);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetColorUpdate(s32 enable);
    extern void GXSetAlphaUpdate(s32 enable);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void PSMTXIdentity(void* mtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern const f32 float_0p5_80420394;
    extern const f32 float_6p2832_8042039c;
    extern const f32 float_360_804203a0;
    extern const f32 float_neg1000_804203a4;
    extern const u8 depth_image_444[];
    void* cam;
    u8 texObj[0x20];
    f32 mtx[3][4];
    f32 halfDepth;
    f32 angle;
    f32 z;
    f32 negHalfDepth;
    f32 negFarX;
    f32 farX;

    cam = camGetCurPtr();
    angle = (float_6p2832_8042039c * *(f32*)((s32)cam + 0x38)) / float_360_804203a0;
    halfDepth = (*(f32*)((s32)cam + 0x30) + *(f32*)((s32)cam + 0x34)) * float_0p5_80420394;
    halfDepth *= (f32)tan(float_0p5_80420394 * angle);
    farX = halfDepth * *(f32*)((s32)cam + 0x3C);
    negHalfDepth = -halfDepth;
    negFarX = -farX;

    GXInitTexObj(texObj, (void*)depth_image_444, 4, 4, 0x11, 1, 1, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 4);
    GXSetZTexture(2, 0x11, 0);
    GXSetNumChans(0);
    GXSetAlphaCompare(7, 0, 1, 7, 0);
    GXSetZCompLoc(1);
    GXSetBlendMode(2, 1, 0, 3);
    GXSetZMode(1, 7, 1);
    GXSetColorUpdate(0);
    GXSetAlphaUpdate(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 0, 0);
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    z = float_neg1000_804203a4;
    *(volatile f32*)0xCC008000 = negFarX;
    *(volatile f32*)0xCC008000 = halfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = farX;
    *(volatile f32*)0xCC008000 = halfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 1;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = farX;
    *(volatile f32*)0xCC008000 = negHalfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 1;
    *(volatile u8*)0xCC008000 = 1;
    *(volatile f32*)0xCC008000 = negFarX;
    *(volatile f32*)0xCC008000 = negHalfDepth;
    *(volatile f32*)0xCC008000 = z;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 1;

    GXSetZTexture(0, 0x11, 0);
    GXSetColorUpdate(1);
}

u8 imgDisp_ProjPlane(s32 param_1, int param_2) {
    extern void PSMTXIdentity(void* mtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void mapSetMaterialLight(s32 flag, void* pos);
    extern void mapSetMaterialLastStageBlend(u32 flags, void* color0, void* color1);
    extern void mapSetMaterialTev(s32 texCount, s32 drawMode, u32 flags, void* mtx);
    extern void* camGetCurPtr(void);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetAlphaUpdate(s32 enable);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern const f32 float_0_80420370;
    extern const f32 float_neg32_80420380;
    extern const f32 float_24_80420384;
    extern const f32 float_32_80420388;
    extern const f32 float_neg24_8042038c;
    extern const u32 vec3_802c2a98[];
    extern u32 dat_80420368;
    extern u32 dat_8042036c;
    u32 mode;
    u32 flags;
    f32 mtx[3][4];
    f32 outMtx[3][4];
    u32 color0;
    u32 color1;
    u32 src[3];
    u32 dst[3];
    void* cam;

    mode = *(s32*)(param_2 + 0x100);
    if (mode == 1) {
        flags = 0x20000;
        PSMTXIdentity(mtx);
        GXSetBlendMode(1, 4, 5, 0);
    } else if ((s32)mode < 1) {
        if ((s32)mode >= 0) {
            return 0;
        }
        flags = 0;
    } else if ((s32)mode < 3) {
        flags = 0x40000;
        PSMTXTrans(mtx, float_0_80420370, float_0_80420370, *(f32*)(param_2 + 0x11C));
        GXSetBlendMode(3, 4, 5, 0);
    } else {
        flags = 0;
    }

    src[0] = vec3_802c2a98[0];
    src[1] = vec3_802c2a98[1];
    src[2] = vec3_802c2a98[2];
    PSMTXMultVec(mtx, src, src);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    mapSetMaterialLight(0, dst);
    color1 = dat_8042036c;
    color0 = dat_80420368;
    mapSetMaterialLastStageBlend(flags, &color0, &color1);
    mapSetMaterialTev(0, 0, flags, mtx);
    cam = camGetCurPtr();
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, outMtx);
    GXLoadPosMtxImm(outMtx, 0);
    GXSetCurrentMtx(0);
    GXSetAlphaUpdate(1);
    GXSetZMode(0, 3, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetCullMode(0);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = float_neg32_80420380;
    *(volatile f32*)0xCC008000 = float_24_80420384;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = float_32_80420388;
    *(volatile f32*)0xCC008000 = float_24_80420384;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = float_32_80420388;
    *(volatile f32*)0xCC008000 = float_neg24_8042038c;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile f32*)0xCC008000 = float_neg32_80420380;
    *(volatile f32*)0xCC008000 = float_neg24_8042038c;
    *(volatile f32*)0xCC008000 = float_0_80420370;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0;
}

u8 imgMain(void) {
    return 0;
}


void imgEntry(s32 name, s32 flag) {
    ;
}


void imgAutoRelease(s32 value) {
    extern void* wp;
    extern void* memset(void* dst, s32 value, u32 size);
    extern void smartFree(void* ptr);
    s32 count;
    void* entry;
    s32 i;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 && *(s32*)((s32)entry + 0xD0) == value) {
            void* part = entry;
            s32 j = 0;
            while (j < 3) {
                u32 flags = *(u32*)part;
                if ((flags & 4) != 0) {
                    *(u32*)part = flags & ~0x10;
                    if ((*(u32*)part & 0x100) != 0) {
                        *(u32*)part = 0;
                    } else if (*(s32*)((s32)part + 0x40) == 0) {
                        *(u32*)part = 0;
                    } else {
                        *(u32*)part = 0;
                        smartFree(*(void**)((s32)part + 0x3C));
                    }
                }
                j++;
                part = (void*)((s32)part + 0x44);
            }
            *(u32*)((s32)entry + 0xCC) = 0;
            memset(entry, 0, 0x158);
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
}

void imgRelease(void* image) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void smartFree(void* ptr);
    void* part = image;
    s32 i = 0;

    while (i < 3) {
        u32 flags = *(u32*)part;
        if ((flags & 4) != 0) {
            *(u32*)part = flags & ~0x10;
            if ((*(u32*)part & 0x100) != 0) {
                *(u32*)part = 0;
            } else if (*(s32*)((s32)part + 0x40) == 0) {
                *(u32*)part = 0;
            } else {
                *(u32*)part = 0;
                smartFree(*(void**)((s32)part + 0x3C));
            }
        }
        i++;
        part = (void*)((s32)part + 0x44);
    }
    *(u32*)((s32)image + 0xCC) = 0;
    memset(image, 0, 0x158);
}

s32 imgCapture(s32 layer) {
    extern void* wp;
    extern void* gp;
    extern void imgCapture_Prim(void* img, s32 index);
    s32 i = 0;
    s32 count;
    void* entry;
    s32 flag;
    s32 localFlag;

    flag = *(s32*)((s32)gp + 0x14);
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    localFlag = ((u32)(-flag) | (u32)flag) >> 31;

    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 &&
            *(s32*)((s32)entry + 0x104) != -1) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if ((entryFlag == 2 || entryFlag == localFlag) &&
                *(s32*)((s32)entry + 0x154) == layer) {
                s32 j = 0;
                while (j < 3) {
                    imgCapture_Prim(entry, j);
                    j++;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
    return 0;
}

void* imgNameToPtr(s32 name, s32 flag) {
    extern void* wp;
    extern s32 strcmp(const char* a, const char* b);
    s32 count;
    s32 i;
    void* entry;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if (entryFlag == 2 || entryFlag == flag) {
                if (strcmp((char*)((s32)entry + 0xD4), (char*)name) == 0) {
                    break;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
    return entry;
}

void imgFreeCapture(void* img, s32 index) {
    extern void smartFree(void* ptr);
    s32 part = (s32)img + index * 0x44;
    u32 flags = *(u32*)part;

    if ((flags & 4) != 0) {
        *(u32*)part = flags & ~0x10;
        if ((*(u32*)part & 0x100) != 0) {
            *(u32*)part = 0;
        } else if (*(s32*)(part + 0x40) == 0) {
            *(u32*)part = 0;
        } else {
            *(u32*)part = 0;
            smartFree(*(void**)(part + 0x3C));
        }
    }
}
