#include "driver/shadowdrv.h"

void* cswp;
void* pswp;
void* dswp;
u8 rampTex16[0x20];
u8 rampTex8[0x20];

extern u8 shadowConfig[];
extern void* smartTexObj(void*, s32);

u8 depthShadowEnd(void) {
    return 0;
}

void projShadowEnd(s32 param_1, u32* param_2) {
    extern void* camGetCurPtr(void);
    extern void PSMTXCopy(void* src, void* dst);
    extern void GXSetViewport(f32 left, f32 top, f32 width, f32 height, f32 nearz, f32 farz);
    extern void GXSetScissor(s32 left, s32 top, s32 width, s32 height);
    extern u32 GXGetTexBufferSize(u32 width, u32 height, s32 format, u8 mipmap, u32 max_lod);
    extern void* smartAlloc(u32 size, s32 align);
    extern void GXSetTexCopySrc(u32 left, u32 top, u32 width, u32 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, u32 format, u32 mipmap);
    extern void GXSetZMode(u32 enable, u32 func, u32 update_enable);
    extern void GXCopyTex(void* dest, u8 clear);
    extern void GXPixModeSync(void);
    extern void GXInitTexObj(void* obj, void* image_ptr, u16 width, u16 height, u32 format, u32 wrap_s, u32 wrap_t, u8 mipmap);
    extern void GXInitTexObjLOD(void* obj, s32 min_filt, s32 mag_filt, f32 min_lod, f32 max_lod, f32 lod_bias, u8 bias_clamp, u8 edge_lod, u32 max_aniso);
    extern void C_MTXLightFrustum(f32 t, f32 b, f32 l, f32 r, f32 n, f32 scale_s, f32 scale_t, f32 trans_s, void* mtx, f32 trans_t);
    extern void C_MTXLightOrtho(f32 t, f32 b, f32 l, f32 r, f32 scale_s, f32 scale_t, f32 trans_s, f32 trans_t, void* mtx);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern f32 tmp_view[3][4];
    extern s32 tmp_sci[];
    extern f32 tmp_vp[];
    extern u8 shadowConfig[];
    extern u16 ShadowSizeTbl[];
    extern f32 float_0_8041f9d4;
    extern f32 float_0p5_8041f9d8;
    f32 mtx[3][4];
    void* cam;
    void* config;
    u16 width;
    u32 flags;
    u32 size;

    cam = camGetCurPtr();
    PSMTXCopy(tmp_view, (void*)((s32)cam + 0x11C));
    GXSetViewport(tmp_vp[0], tmp_vp[1], tmp_vp[2], tmp_vp[3], tmp_vp[4], tmp_vp[5]);
    GXSetScissor(tmp_sci[0], tmp_sci[1], tmp_sci[2], tmp_sci[3]);

    if (param_2[1] != 0) {
        config = shadowConfig + param_2[2] * 0x48;
        flags = param_2[0];
        width = ShadowSizeTbl[*(s32*)((s32)config + 0x40)];
        if ((flags & 2) != 0) {
            if ((flags & 4) != 0) {
                size = GXGetTexBufferSize(width >> 1, width >> 1, 2, 0, 0);
            } else {
                size = GXGetTexBufferSize(width, width, 2, 0, 0);
            }
            param_2[0x3F] = (u32)smartAlloc(size, 3);
        }
        GXSetTexCopySrc(0, 0, width, width);
        if ((param_2[0] & 4) != 0) {
            GXSetTexCopyDst(width >> 1, width >> 1, 2, 1);
        } else {
            GXSetTexCopyDst(width, width, 2, 0);
        }
        GXSetZMode(1, 3, 1);
        GXCopyTex(*(void**)param_2[0x3F], 1);
        GXPixModeSync();
        if ((param_2[0] & 4) != 0) {
            GXInitTexObj((void*)((s32)param_2 + 0xDC), *(void**)param_2[0x3F], width >> 1, width >> 1, 2, 0, 0, 0);
        } else {
            GXInitTexObj((void*)((s32)param_2 + 0xDC), *(void**)param_2[0x3F], width, width, 2, 0, 0, 0);
        }
        GXInitTexObjLOD((void*)((s32)param_2 + 0xDC), 1, 1, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, 0, 0, 0);
        if ((void*)((s32)param_2 + 0x7C) != 0) {
            if (*(s32*)((s32)config + 0x38) == 0) {
                C_MTXLightFrustum(-*(f32*)((s32)config + 0x2C), *(f32*)((s32)config + 0x2C), *(f32*)((s32)config + 0x28), -*(f32*)((s32)config + 0x28), *(f32*)((s32)config + 0x30), float_0p5_8041f9d8, float_0p5_8041f9d8, float_0p5_8041f9d8, mtx, float_0p5_8041f9d8);
            } else {
                C_MTXLightOrtho(-*(f32*)((s32)config + 0x2C), *(f32*)((s32)config + 0x2C), *(f32*)((s32)config + 0x28), -*(f32*)((s32)config + 0x28), float_0p5_8041f9d8, float_0p5_8041f9d8, float_0p5_8041f9d8, float_0p5_8041f9d8, mtx);
            }
            PSMTXConcat(mtx, (void*)((s32)param_2 + 0xC), (void*)((s32)param_2 + 0x7C));
        }
        param_2[0] |= 1;
    }
}

void* shadowGetProjShadowConfig(void) {
    return shadowConfig + (*(s32*)((s32)pswp + 8) * 0x48);
}

u32 shadowGetProjShadowColor(void) {
    return *(u32*)((s32)pswp + 0x100);
}

void* shadowGetProjShadowProjTexMtx(void) {
    return (void*)((s32)pswp + 0x7C);
}

void* shadowGetProjShadowTexObj(void) {
    void* wp = pswp;
    if ((*(u32*)wp & 1) != 0) {
        return smartTexObj((void*)((s32)wp + 0xDC), *(s32*)((s32)wp + 0xFC));
    }
    return 0;
}

void shadowProjShadowDrawCountUp(void) {
    (*(s32*)((s32)pswp + 4))++;
}

void* shadowGetDepthShadowConfig(void) {
    return shadowConfig + (*(s32*)((s32)dswp + 8) * 0x48);
}

u32 shadowGetDepthShadowColor(void) {
    void* wp;
    u8 color[4];
    s32 alpha;

    wp = dswp;
    color[3] = 0;
    alpha = *(u8*)((s32)wp + 0x103);
    color[0] = ((0xFF - *(u8*)((s32)wp + 0x100)) * alpha) / 0xFF;
    color[1] = ((0xFF - *(u8*)((s32)wp + 0x101)) * alpha) / 0xFF;
    color[2] = ((0xFF - *(u8*)((s32)wp + 0x102)) * alpha) / 0xFF;
    return *(u32*)color;
}

void* shadowGetDepthShadowProjTexMtx(void) {
    return (void*)((s32)dswp + 0x7C);
}

void* shadowGetDepthShadowDepthMtx(void) {
    return (void*)((s32)dswp + 0xAC);
}

void* shadowGetDepthShadowTexObj(void) {
    void* wp = dswp;
    if ((*(u32*)wp & 1) != 0) {
        return smartTexObj((void*)((s32)wp + 0xDC), *(s32*)((s32)wp + 0xFC));
    }
    return 0;
}

void shadowDepthShadowDrawCountUp(void) {
    (*(s32*)((s32)dswp + 4))++;
}

void* shadowGetRamp16TexObj(void) {
    return rampTex16;
}

void* shadowGetRamp8TexObj(void) {
    return rampTex8;
}

void* shadowGetCharShadowProjection(void) {
    return *(void**)((s32)cswp + 0x10C);
}

u32 shadowGetCharShadowColor(void) {
    return *(u32*)((s32)cswp + 0x100);
}

void* shadowGetCharShadowProjTexMtx(void) {
    return (void*)((s32)cswp + 0x7C);
}

void* shadowGetCharShadowTexObj(void) {
    void* wp = cswp;
    if ((*(u32*)wp & 1) != 0) {
        return smartTexObj((void*)((s32)wp + 0xDC), *(s32*)((s32)wp + 0xFC));
    }
    return 0;
}

void shadowFlagOn(s32 id, u16 flag) {
    u16* flags = (u16*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C);
    *flags |= flag;
}

void shadowSetCollision(s32 id, s32 on) {
    u16* flags = (u16*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C);
    if (on != 0) {
        *flags |= 1;
    } else {
        *flags &= ~1;
    }
}

void shadowSetCamId(s32 id, s16 camId) {
    *(s16*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 0x1A) = camId;
}

void shadowSetSize(s32 id, f32 size) {
    *(f32*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 0x10) = size;
}

void shadowSetVolumeDecay(s32 id, f32 a, f32 b, f32 c) {
    void* entry = (void*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C);
    *(f32*)((s32)entry + 0x1C) = a;
    *(f32*)((s32)entry + 0x20) = b;
    *(f32*)((s32)entry + 0x24) = c;
}

void U_shadowSetMode(s32 id, u8 mode) {
    *(u8*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 0x28) = mode;
}

void shadowSetType(s32 id, u8 type) {
    *(u8*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 3) = type;
}

int shadowEntryMode(double param_1, double param_2, double param_3, double param_4, u8 param_5) {
    return 0;
}

u8 shadowEntry(double param_1, double param_2, double param_3, double param_4) {
    return 0;
}

u8 cylinderShadowDraw(void) {
    return 0;
}

void cylinder(void* param_1) {
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXCallDisplayList(void* list, u32 nbytes);
    extern void* cswp;
    extern u8 color_tbl;
    extern u8 color_tbl_nodecay;
    extern u8 lbl_80304A20[];
    extern f32 float_0_8041f9d4;
    extern f32 float_1_8041f9e4;
    extern f32 float_255_8041f9f4;
    f32 decay;
    f32 alpha;
    f32 top;
    s32 colorAlpha;
    u8* dlBase;

    dlBase = lbl_80304A20;
    if ((*(u16*)param_1 & 4) != 0) {
        GXSetArray(0xB, &color_tbl_nodecay, 4);
        GXCallDisplayList(dlBase + 0x140, 0x40);
        GXCallDisplayList(dlBase + 0x180, 0x40);
        GXCallDisplayList(dlBase + 0x1C0, 0x40);
    } else {
        top = float_1_8041f9e4 - *(f32*)((s32)param_1 + 0x24);
        alpha = (((f32)*(u8*)((s32)cswp + 0x103)) * *(f32*)((s32)param_1 + 0x1C)) / float_255_8041f9f4;
        decay = alpha * *(f32*)((s32)param_1 + 0x20);
        if (top < float_0_8041f9d4) {
            top = float_0_8041f9d4;
        } else if (top > float_1_8041f9e4) {
            top = float_1_8041f9e4;
        }
        if (alpha > float_0_8041f9d4) {
            if (decay <= float_0_8041f9d4) {
                decay = float_0_8041f9d4;
            }
            if (decay >= float_1_8041f9e4) {
                decay = float_1_8041f9e4;
            }
            colorAlpha = (s32)(float_255_8041f9f4 * decay);
            GXSetArray(0xB, &color_tbl, 4);
            if (float_0_8041f9d4 != top) {
                GXCallDisplayList(dlBase + 0x200, 0x40);
            }
            if (float_1_8041f9e4 != top && ((colorAlpha & 0xFF) != 0)) {
                GXCallDisplayList(dlBase + 0x240, 0x40);
            }
            GXCallDisplayList(dlBase + 0x180, 0x40);
            GXCallDisplayList(dlBase + 0x1C0, 0x40);
        }
    }
}

u8 shadowCharShadowDisp_Texture(void) {
    return 0;
}

void shadowCharShadowDisp_Polygon(s32 param_1) {
    extern void* camGetCurPtr(void);
    extern void GXSetNumChans(u32 n);
    extern void GXSetChanCtrl(s32 chan, u8 enable, s32 amb_src, s32 mat_src, s32 light_mask, s32 diff_fn, s32 attn_fn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(u32 n);
    extern void GXSetNumTevStages(u32 n);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 regid);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 regid);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearz, f32 farz, void* color);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetZCompLoc(s32 before_tex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
    extern void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);
    extern void* cswp;
    extern u8 testKagePosArray[];
    extern u32 unk_80429538;
    extern f32 float_0_8041f9d4;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    f32 mtx[3][4];
    u32 matColor;
    u32 fogColor;
    void* cam;
    void* entry;
    s32 i;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;

    entry = *(void**)((s32)cswp + 0x108);
    cam = camGetCurPtr();
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    matColor = *(u32*)((s32)cswp + 0x100);
    GXSetChanMatColor(4, &matColor);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 1, 5, 7);
    fogColor = unk_80429538;
    GXSetFog(0, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, &fogColor);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZMode(1, 3, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXSetArray(9, testKagePosArray, 0xC);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);

    i = 0;
    while (i < *(s32*)((s32)cswp + 0x104)) {
        if (*(f32*)((s32)entry + 0x10) != float_0_8041f9d4
            && *(u8*)((s32)entry + 0x2) == 0
            && (*(u16*)entry & 2) != 0
            && *(s16*)((s32)entry + 0x1A) == param_1) {
            if (param_1 == 1) {
                sysWaitDrawSync();
                GXClearBoundingBox();
            }
            PSMTXConcat((void*)((s32)cam + 0x11C), (void*)((s32)entry + 0x2C), mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXBegin(0x98, 0, 6);
            *fifo = 0;
            *fifo = 1;
            *fifo = 2;
            *fifo = 3;
            *fifo = 4;
            *fifo = 5;
            if (param_1 == 1) {
                sysWaitDrawSync();
                GXReadBoundingBox(&left, &top, &right, &bottom);
                offscreenAddBoundingBox(*(s16*)((s32)entry + 0x18), left, top, right, bottom);
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x5C);
    }
}

u8 shadowCharShadowDisp_Projection(void) {
    extern void* camGetCurPtr(void);
    extern void GXSetNumChans(u32 n);
    extern void GXSetChanCtrl(s32 chan, u8 enable, s32 amb_src, s32 mat_src, s32 light_mask, s32 diff_fn, s32 attn_fn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(u32 n);
    extern void GXSetNumTevStages(u32 n);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 regid);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 regid);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearz, f32 farz, void* color);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetZCompLoc(s32 before_tex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void C_MTXOrtho(double t, double b, double l, double r, double n, double f, void* mtx);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void C_MTXLookAt(void* dst, void* camPos, void* camUp, void* target);
    extern void* cswp;
    extern void* gp;
    extern u8 shadowConfig[];
    extern u8 testKagePosArray[];
    extern f32 vec3_802bfd10[];
    extern u32 unk_80429538;
    extern u32 dat_8041f9c4;
    extern u32 dat_8041f9c8;
    extern f32 float_0_8041f9d4;
    extern f32 float_0p2_8041f9ec;
    volatile f32* fifoF = (volatile f32*)0xCC008000;
    volatile u8* fifoB = (volatile u8*)0xCC008000;
    void* wp;
    u8* config;
    void* cam;
    void* entry;
    f32 mtx[3][4];
    f32 proj[4][4];
    f32 lookAt[3][4];
    u32 pos[3];
    u32 up[3];
    u32 target[3];
    u32 color;
    u32 fogColor;
    s32 i;
    s32 configIndex;
    u8 alpha;
    f32 zero;
    f32 zRadius;
    f32 xRadius;
    f32 negZ;
    f32 xInset;
    f32 zInset;

    wp = cswp;
    entry = *(void**)((s32)wp + 0x108);
    configIndex = *(s32*)((s32)wp + 8);
    config = shadowConfig + configIndex * 0x48;
    cam = camGetCurPtr();
    alpha = *(u8*)((s32)wp + 0x103);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    color = *(u32*)((s32)wp + 0x100);
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 1, 5, 7);
    fogColor = unk_80429538;
    GXSetFog(0, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, &fogColor);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZMode(1, 3, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXSetArray(9, testKagePosArray, 0xC);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetBlendMode(0, 4, 5, 0);

    if (*(s32*)((s32)gp + 0x14) != 0) {
        color = dat_8041f9c4;
        GXSetChanMatColor(4, &color);
    } else {
        color = dat_8041f9c8;
        ((u8*)&color)[0] = alpha;
        ((u8*)&color)[1] = alpha;
        ((u8*)&color)[2] = alpha;
        GXSetChanMatColor(4, &color);
    }

    GXSetZMode(0, 3, 0);
    zero = float_0_8041f9d4;

    i = 0;
    while (i < *(s32*)((s32)cswp + 0x104)) {
        if (*(f32*)((s32)entry + 0x10) != zero
            && *(u8*)((s32)entry + 0x2) == 3
            && (*(u16*)entry & 2) != 0) {
            PSMTXConcat((void*)((s32)cam + 0x11C), (void*)((s32)entry + 0x2C), mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXBegin(0x98, 0, 6);
            *fifoB = 0;
            *fifoB = 1;
            *fifoB = 2;
            *fifoB = 3;
            *fifoB = 4;
            *fifoB = 5;
            (*(s32*)((s32)wp + 4))++;
        }
        i++;
        entry = (void*)((s32)entry + 0x5C);
    }

    if (*(s32*)((s32)wp + 4) != 0) {
        C_MTXOrtho(*(f32*)(config + 0x2C), -*(f32*)(config + 0x2C),
                   *(f32*)(config + 0x28), -*(f32*)(config + 0x28),
                   *(f32*)(config + 0x30), *(f32*)(config + 0x34), proj);
        GXSetProjection(proj, 1);
        GXSetBlendMode(1, 4, 5, 0);
        GXSetChanCtrl(4, 0, 0, 1, 0, 2, 2);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxDesc(0xB, 1);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);

        ((u32*)pos)[0] = ((u32*)vec3_802bfd10)[3];
        ((u32*)pos)[1] = ((u32*)vec3_802bfd10)[4];
        ((u32*)pos)[2] = ((u32*)vec3_802bfd10)[5];
        ((u32*)up)[0] = ((u32*)vec3_802bfd10)[6];
        ((u32*)up)[1] = ((u32*)vec3_802bfd10)[7];
        ((u32*)up)[2] = ((u32*)vec3_802bfd10)[8];
        ((u32*)target)[0] = ((u32*)vec3_802bfd10)[9];
        ((u32*)target)[1] = ((u32*)vec3_802bfd10)[10];
        ((u32*)target)[2] = ((u32*)vec3_802bfd10)[11];
        C_MTXLookAt(lookAt, pos, up, target);
        GXLoadPosMtxImm(lookAt, 0);

        zRadius = *(f32*)(config + 0x28);
        xRadius = *(f32*)(config + 0x2C);
        negZ = -zRadius;
        xInset = float_0p2_8041f9ec * xRadius;
        zInset = float_0p2_8041f9ec * negZ;

        GXBegin(0x80, 0, 0x10);

        *fifoF = zero - negZ;               *fifoF = zero; *fifoF = zero - xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zero + negZ;               *fifoF = zero; *fifoF = zero - xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zero + negZ;               *fifoF = zero; *fifoF = xInset + (zero - xRadius);    *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;
        *fifoF = zero - negZ;               *fifoF = zero; *fifoF = xInset + (zero - xRadius);    *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;

        *fifoF = zero - negZ;               *fifoF = zero; *fifoF = zero + xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zero + negZ;               *fifoF = zero; *fifoF = zero + xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zero + negZ;               *fifoF = zero; *fifoF = (zero + xRadius) - xInset;    *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;
        *fifoF = zero - negZ;               *fifoF = zero; *fifoF = (zero + xRadius) - xInset;    *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;

        *fifoF = zero - negZ;               *fifoF = zero; *fifoF = zero - xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zero - negZ;               *fifoF = zero; *fifoF = zero + xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zInset + (zero - negZ);    *fifoF = zero; *fifoF = zero + xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;
        *fifoF = zInset + (zero - negZ);    *fifoF = zero; *fifoF = zero - xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;

        *fifoF = zero + negZ;               *fifoF = zero; *fifoF = zero - xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = zero + negZ;               *fifoF = zero; *fifoF = zero + xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0xFF;
        *fifoF = (zero + negZ) - zInset;    *fifoF = zero; *fifoF = zero + xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;
        *fifoF = (zero + negZ) - zInset;    *fifoF = zero; *fifoF = zero - xRadius;               *fifoB = 0; *fifoB = 0; *fifoB = 0; *fifoB = 0;
    }

    return 0;
}

void shadowCharShadowDisp_Collision(void) {
    extern void* camGetCurPtr(void);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern s32 hitCheckVecFilter(void* data, void* filter);
    extern s32 _filter(s32, int);
    extern void* cswp;
    extern f32 float_0_8041f9d4;
    extern f32 float_1_8041f9e4;
    extern f32 float_1p5_8041fa04;
    extern f32 float_150_8041fa00;
    extern f32 vec3_802bfd10[];
    u8 data[0x40];
    void* entry;
    void* mtx;
    s32 i;
    f32 size;
    f32 nx;
    f32 ny;
    f32 nz;

    entry = *(void**)((s32)cswp + 0x108);
    camGetCurPtr();
    i = 0;
    while (1) {
        if (*(s32*)((s32)cswp + 0x104) <= i) {
            return;
        }
        mtx = (void*)((s32)entry + 0x2C);
        if (*(f32*)((s32)entry + 0x10) != float_0_8041f9d4 && (*(u16*)entry & 2) == 0) {
            if (*(u8*)((s32)entry + 2) == 3 || (*(u16*)entry & 1) == 0) {
                PSMTXTrans(
                    mtx,
                    *(f32*)((s32)entry + 4),
                    float_1p5_8041fa04 + *(f32*)((s32)entry + 8),
                    *(f32*)((s32)entry + 0xC)
                );
                *(f32*)((s32)entry + 0x54) = *(f32*)((s32)entry + 0x10);
                *(f32*)((s32)entry + 0x2C) = *(f32*)((s32)entry + 0x10);
                *(u32*)((s32)entry + 0x40) = *(u32*)((s32)entry + 0x14);
            } else {
                *(u32*)((s32)data + 0x0C) = *(u32*)((s32)entry + 4);
                *(f32*)((s32)data + 0x10) = *(f32*)((s32)entry + 8) + float_1_8041f9e4;
                *(u32*)((s32)data + 0x14) = *(u32*)((s32)entry + 0xC);
                *(u32*)((s32)data + 0x18) = *(u32*)((s32)vec3_802bfd10 + 0);
                *(u32*)((s32)data + 0x1C) = *(u32*)((s32)vec3_802bfd10 + 4);
                *(u32*)((s32)data + 0x20) = *(u32*)((s32)vec3_802bfd10 + 8);
                *(f32*)((s32)data + 0x3C) = float_150_8041fa00;
                if (hitCheckVecFilter(data, _filter) != 0) {
                    size = *(f32*)((s32)entry + 0x10);
                    nx = *(f32*)((s32)data + 0x30);
                    ny = *(f32*)((s32)data + 0x34);
                    nz = *(f32*)((s32)data + 0x38);
                    *(f32*)((s32)entry + 0x38) = *(f32*)((s32)data + 0x24) + nx;
                    *(f32*)((s32)entry + 0x48) = *(f32*)((s32)data + 0x28) + ny;
                    *(f32*)((s32)entry + 0x58) = *(f32*)((s32)data + 0x2C) + nz;
                    *(f32*)((s32)entry + 0x3C) = nx * size;
                    *(f32*)((s32)entry + 0x40) = ny * *(f32*)((s32)entry + 0x14);
                    *(f32*)((s32)entry + 0x44) = nz * size;
                    *(f32*)((s32)entry + 0x4C) = float_0_8041f9d4;
                    *(f32*)((s32)entry + 0x50) = nz * size;
                    *(f32*)((s32)entry + 0x54) = -ny * size;
                    *(f32*)((s32)entry + 0x2C) = size * (-ny * ny - nz * nz);
                    *(f32*)((s32)entry + 0x30) = size * nx * ny;
                    *(f32*)((s32)entry + 0x34) = size * nx * nz;
                } else {
                    i++;
                    entry = (void*)((s32)entry + 0x5C);
                    continue;
                }
            }
            *(u16*)entry |= 2;
        }
        i++;
        entry = (void*)((s32)entry + 0x5C);
    }
}

s32 _filter(s32 param_1, int param_2) {
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(void*);
    void* hit;

    hit = (void*)param_2;
    if ((hitGetAttr(hit) & 0x80000000) != 0) {
        if ((*(u32*)mobjHitObjPtrToPtr(hit) & 0x800) != 0) {
            return 0;
        }
    }
    return 1;
}

void shadowDisp(s32 param_1, s32 param_2) {
    extern void* dispGetCurWork(void);
    extern void* camGetCurPtr(void);
    extern void* camGetPtr(s32 id);
    extern void GXSetNumChans(u32 n);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 amb_src, s32 mat_src, s32 light_mask, s32 diff_fn, s32 attn_fn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(u32 n);
    extern void GXSetNumTevStages(u32 n);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 regid);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 regid);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearz, f32 farz, void* color);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetZCompLoc(s32 before_tex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXInvalidateTexAll(void);
    extern void GXGetViewportv(void* vp);
    extern void GXGetScissor(s32* left, s32* top, s32* width, s32* height);
    extern void GXSetViewport(f32 left, f32 top, f32 width, f32 height, f32 nearz, f32 farz);
    extern void GXSetScissor(s32 left, s32 top, s32 width, s32 height);
    extern void GXSetProjection(void* mtx, s32 type);
    extern void GXSetColorUpdate(s32 enable);
    extern void C_MTXLookAt(void* dst, void* camPos, void* camUp, void* target);
    extern void C_MTXFrustum(double t, double b, double l, double r, double n, double f, void* mtx);
    extern void C_MTXOrtho(double t, double b, double l, double r, double n, double f, void* mtx);
    extern void PSMTXCopy(void* src, void* dst);
    extern void projShadowEnd(s32 param_1, u32* param_2);
    extern void* cswp;
    extern void* pswp;
    extern void* dswp;
    extern u8 lbl_80304A20[];
    extern u16 ShadowSizeTbl[];
    extern s32 tmp_sci[];
    extern f32 tmp_vp[];
    extern f32 tmp_view[3][4];
    extern u8 color_tbl;
    extern f32 float_0_8041f9d4;
    extern f32 float_1_8041f9e4;
    extern f32 float_1000_8041fa0c;
    extern f32 float_neg1_8041f9e8;
    extern u32 unk_80429538;

    void* dispWork;
    void* wp;
    void* cam;
    u8* base;
    u8* cfg;
    u8* cfgBase;
    s32 cfgIndex;
    s32 cfgOff;
    s32 mode;
    u16 shadowSize;
    f32 shadowSizeF;
    u32 fogColor;
    u32 matColor;
    u32 pos_case5[3];
    u32 pos_case0[3];
    u32 pos_case2[3];

    base = lbl_80304A20;
    cfgBase = base + 0x40;
    dispWork = dispGetCurWork();

    if ((param_1 == 1) || ((u32)(param_1 - 4) <= 1) || (param_1 == 7)) {
        ((void (*)(s32, s32))shadowCharShadowDisp_Polygon)(param_1, param_2);
        camGetCurPtr();

        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
        matColor = *(u32*)((s32)cswp + 0x100);
        GXSetChanMatColor(4, &matColor);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0xFF, 0xFF, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
        GXSetTevAlphaIn(0, 7, 1, 5, 7);
        fogColor = unk_80429538;
        GXSetFog(0, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, float_0_8041f9d4, &fogColor);
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZMode(1, 3, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetCurrentMtx(0);
        GXSetCullMode(0);
        GXSetArray(9, base + 0xE0, 0xC);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxDesc(0xB, 2);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetArray(9, base + 0x2C0, 0xC);
        GXSetArray(0xB, &color_tbl, 4);
        ((void (*)(s32, s32))cylinderShadowDraw)(param_1, param_2);
        ((void (*)(s32, s32))shadowCharShadowDisp_Texture)(param_1, param_2);
    } else if (param_1 == 2) {
        mode = *(u8*)((s32)dispWork + 1);
        switch (mode) {
            case 5:
                ((void (*)(s32, s32))shadowCharShadowDisp_Collision)(param_1, param_2);
                wp = cswp;
                cfgIndex = *(s32*)((s32)wp + 8);
                cfgOff = cfgIndex * 0x48;
                cfg = cfgBase + cfgOff;
                if (*(s32*)cfg == 1) {
                    cam = camGetPtr(4);
                    *(f32*)(cfg + 4) = *(f32*)((s32)cam + 0x18);
                    *(f32*)(cfg + 8) = float_1000_8041fa0c + *(f32*)((s32)cam + 0x1C);
                    *(f32*)(cfg + 0xC) = *(f32*)((s32)cam + 0x20);
                    *(f32*)(cfg + 0x10) = float_0_8041f9d4;
                    *(f32*)(cfg + 0x14) = float_0_8041f9d4;
                    *(f32*)(cfg + 0x18) = float_neg1_8041f9e8;
                    *(f32*)(cfg + 0x1C) = *(f32*)((s32)cam + 0x18);
                    *(f32*)(cfg + 0x20) = *(f32*)((s32)cam + 0x1C);
                    *(f32*)(cfg + 0x24) = *(f32*)((s32)cam + 0x20);
                }

                wp = cswp;
                GXInvalidateTexAll();
                cfgIndex = *(s32*)((s32)wp + 8);
                cfgOff = cfgIndex * 0x48;
                cfg = cfgBase + cfgOff;
                shadowSize = ShadowSizeTbl[*(s32*)(cfg + 0x40)];
                GXGetViewportv(tmp_vp);
                GXGetScissor(&tmp_sci[0], &tmp_sci[1], &tmp_sci[2], &tmp_sci[3]);
                shadowSizeF = (f32)shadowSize;
                GXSetViewport(float_0_8041f9d4, float_0_8041f9d4, shadowSizeF, shadowSizeF, float_0_8041f9d4, float_1_8041f9e4);
                GXSetScissor(1, 1, shadowSize - 2, shadowSize - 2);
                pos_case5[0] = *(u32*)(cfg + 4);
                pos_case5[1] = *(u32*)(cfg + 8);
                pos_case5[2] = *(u32*)(cfg + 0xC);
                C_MTXLookAt((void*)((s32)wp + 0xC), pos_case5, cfg + 0x10, cfg + 0x1C);
                if (*(s32*)(cfg + 0x38) == 0) {
                    C_MTXFrustum(*(f32*)(cfg + 0x2C), -*(f32*)(cfg + 0x2C), *(f32*)(cfg + 0x28), -*(f32*)(cfg + 0x28), *(f32*)(cfg + 0x30), *(f32*)(cfg + 0x34), (void*)((s32)wp + 0x3C));
                } else {
                    C_MTXOrtho(*(f32*)(cfg + 0x2C), -*(f32*)(cfg + 0x2C), *(f32*)(cfg + 0x28), -*(f32*)(cfg + 0x28), *(f32*)(cfg + 0x30), *(f32*)(cfg + 0x34), (void*)((s32)wp + 0x3C));
                }
                GXSetProjection((void*)((s32)wp + 0x3C), *(s32*)(cfg + 0x38));
                cam = camGetCurPtr();
                PSMTXCopy((void*)((s32)cam + 0x11C), tmp_view);
                cam = camGetCurPtr();
                PSMTXCopy((void*)((s32)wp + 0xC), (void*)((s32)cam + 0x11C));
                ((void (*)(s32, s32))shadowCharShadowDisp_Projection)(param_1, param_2);
                projShadowEnd(param_1, (u32*)cswp);
                break;

            case 0:
                wp = pswp;
                GXInvalidateTexAll();
                cfgIndex = *(s32*)((s32)wp + 8);
                cfgOff = cfgIndex * 0x48;
                cfg = cfgBase + cfgOff;
                shadowSize = ShadowSizeTbl[*(s32*)(cfg + 0x40)];
                GXGetViewportv(tmp_vp);
                GXGetScissor(&tmp_sci[0], &tmp_sci[1], &tmp_sci[2], &tmp_sci[3]);
                shadowSizeF = (f32)shadowSize;
                GXSetViewport(float_0_8041f9d4, float_0_8041f9d4, shadowSizeF, shadowSizeF, float_0_8041f9d4, float_1_8041f9e4);
                GXSetScissor(1, 1, shadowSize - 2, shadowSize - 2);
                pos_case0[0] = *(u32*)(cfg + 4);
                pos_case0[1] = *(u32*)(cfg + 8);
                pos_case0[2] = *(u32*)(cfg + 0xC);
                C_MTXLookAt((void*)((s32)wp + 0xC), pos_case0, cfg + 0x10, cfg + 0x1C);
                if (*(s32*)(cfg + 0x38) == 0) {
                    C_MTXFrustum(*(f32*)(cfg + 0x2C), -*(f32*)(cfg + 0x2C), *(f32*)(cfg + 0x28), -*(f32*)(cfg + 0x28), *(f32*)(cfg + 0x30), *(f32*)(cfg + 0x34), (void*)((s32)wp + 0x3C));
                } else {
                    C_MTXOrtho(*(f32*)(cfg + 0x2C), -*(f32*)(cfg + 0x2C), *(f32*)(cfg + 0x28), -*(f32*)(cfg + 0x28), *(f32*)(cfg + 0x30), *(f32*)(cfg + 0x34), (void*)((s32)wp + 0x3C));
                }
                GXSetProjection((void*)((s32)wp + 0x3C), *(s32*)(cfg + 0x38));
                cam = camGetCurPtr();
                PSMTXCopy((void*)((s32)cam + 0x11C), tmp_view);
                cam = camGetCurPtr();
                PSMTXCopy((void*)((s32)wp + 0xC), (void*)((s32)cam + 0x11C));
                break;

            case 2:
                projShadowEnd(param_1, (u32*)pswp);
                wp = dswp;
                GXInvalidateTexAll();
                cfgIndex = *(s32*)((s32)wp + 8);
                cfgOff = cfgIndex * 0x48;
                cfg = cfgBase + cfgOff;
                shadowSize = ShadowSizeTbl[*(s32*)(cfg + 0x40)];
                GXSetColorUpdate(0);
                GXGetViewportv(tmp_vp);
                GXGetScissor(&tmp_sci[0], &tmp_sci[1], &tmp_sci[2], &tmp_sci[3]);
                shadowSizeF = (f32)shadowSize;
                GXSetViewport(float_0_8041f9d4, float_0_8041f9d4, shadowSizeF, shadowSizeF, float_0_8041f9d4, float_1_8041f9e4);
                GXSetScissor(1, 1, shadowSize - 2, shadowSize - 2);
                pos_case2[0] = *(u32*)(cfg + 4);
                pos_case2[1] = *(u32*)(cfg + 8);
                pos_case2[2] = *(u32*)(cfg + 0xC);
                C_MTXLookAt((void*)((s32)wp + 0xC), pos_case2, cfg + 0x10, cfg + 0x1C);
                if (*(s32*)(cfg + 0x38) == 0) {
                    C_MTXFrustum(*(f32*)(cfg + 0x2C), -*(f32*)(cfg + 0x2C), *(f32*)(cfg + 0x28), -*(f32*)(cfg + 0x28), *(f32*)(cfg + 0x30), *(f32*)(cfg + 0x34), (void*)((s32)wp + 0x3C));
                } else {
                    C_MTXOrtho(*(f32*)(cfg + 0x2C), -*(f32*)(cfg + 0x2C), *(f32*)(cfg + 0x28), -*(f32*)(cfg + 0x28), *(f32*)(cfg + 0x30), *(f32*)(cfg + 0x34), (void*)((s32)wp + 0x3C));
                }
                GXSetProjection((void*)((s32)wp + 0x3C), *(s32*)(cfg + 0x38));
                cam = camGetCurPtr();
                PSMTXCopy((void*)((s32)cam + 0x11C), tmp_view);
                cam = camGetCurPtr();
                PSMTXCopy((void*)((s32)wp + 0xC), (void*)((s32)cam + 0x11C));
                ((void (*)(s32, void*))depthShadowEnd)(param_1, dswp);
                break;

            case 7:
                ((void (*)(s32, void*))depthShadowEnd)(param_1, dswp);
                break;
        }
    }
}

u8 shadowMain(void) {
    return 0;
}

u8 shadowInit(void) {
    return 0;
}
