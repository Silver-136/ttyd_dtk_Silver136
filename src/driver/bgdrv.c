#include "driver/bgdrv.h"
#include "mario/mariost.h"
#include "driver/arcdrv.h"

// .bss
BackgroundWork work[2];

// Helper Macros
#define bgGetWork() (gp->inBattle ? &work[1] : &work[0])

#define FIFO_U16 (*(volatile u16*)0xCC008000)
#define FIFO_F32_BG (*(volatile f32*)0xCC008000)
#define READ_F32_BG(addr) (*(volatile const f32*)&(addr))

void bgMain(void);

extern void* camGetPtr(s32 cameraId);
extern void GXSetAlphaUpdate(s32 enable);
extern void GXSetCullMode(s32 mode);
extern void GXSetZCompLoc(s32 beforeTex);
extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
extern void mapGetBlend(void* color);
extern void GXSetTevColor(s32 reg, void* color);
extern void GXSetNumChans(s32 count);
extern void GXSetNumTevStages(s32 stages);
extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
extern void TEXGetGXTexObjFromPalette(s32 palette, void* texObj, s32 index);
extern void GXLoadTexObj(void* texObj, s32 mapId);
extern u32 GXGetTexObjHeight(void* texObj);
extern u32 GXGetTexObjWidth(void* texObj);
extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXConcat(void* a, void* b, void* ab);
extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
extern void GXSetNumTexGens(s32 count);
extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
extern void GXSetChanMatColor(s32 chan, void* color);
extern void GXLoadPosMtxImm(void* mtx, s32 id);
extern void GXSetCurrentMtx(s32 id);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32 attr, s32 type);
extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
extern f32 reviseAngle(f32 angle);
extern f64 sin(f64 angle);
extern f64 cos(f64 angle);
extern void dispEntry(s32 cameraId, s32 order, void* callback, s32 param, f32 z);
extern const f32 float_6p2832_8041f60c;
extern const f32 float_360_8041f610;
extern const f32 float_0p001_8041f614;
extern const f32 float_4_8041f618;
extern const f64 double_to_int_802bf538;
extern const f32 float_480_8041f600;
extern const f32 float_608_8041f5fc;
extern const f32 float_1_8041f604;
extern const f32 float_0_8041f608;
extern const f64 double_to_int_mask_802bf530;
extern void* mapalloc_base_ptr;
extern void _mapFree(void* heap, void* ptr);
extern void* _mapAlloc(void* heap, u32 size);
extern s32 getMarioStDvdRoot(void);
extern int sprintf(char* str, const char* format, ...);
extern void* DVDMgrOpen(void* path, s32 mode, s32 unk);
extern u32 DVDMgrGetLength(void* dvd);
extern void DVDMgrRead(void* dvd, void* dst, u32 size, s32 offset);
extern void DVDMgrClose(void* dvd);
extern void* UnpackTexPalette(void* data);
extern const char str_PCTs_PCTs_8041f61c[6];
extern u32 dat_8041f5f0;
extern u32 dat_8041f5f4;
extern u32 dat_8041f5f8;

void bgInit(void) {
    memset(&work[0], 0, sizeof(BackgroundWork));
    memset(&work[1], 0, sizeof(BackgroundWork));
}

void bgReInit(void) {
    BackgroundWork* wp = bgGetWork();

    memset(wp, 0, sizeof(BackgroundWork));
}

void bgDisp(s32 cameraId) {
    u8* bg;
    void* cam;
    u32 blendColor;
    u32 tevColor;
    u32 matColor;
    u8 texObj[0x20];
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    u32 height;
    u32 width;
    f32 scaleY;

    bg = (u8*)bgGetWork();
    cam = camGetPtr(cameraId);

    if ((*(u16*)bg & 1) == 0) {
        return;
    }

    GXSetAlphaUpdate(0);
    GXSetCullMode(2);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(0, 1, 0, 7);
    GXSetZMode(0, 7, 0);

    mapGetBlend(&blendColor);

    ((u8*)&blendColor)[1] = (s32)(((u8*)&blendColor)[1] * *(u8*)(bg + 0x15)) / 255;
    ((u8*)&blendColor)[2] = (s32)(((u8*)&blendColor)[2] * *(u8*)(bg + 0x16)) / 255;
    ((u8*)&blendColor)[3] = (s32)(((u8*)&blendColor)[3] * *(u8*)(bg + 0x17)) / 255;
    ((u8*)&blendColor)[0] = (s32)(((u8*)&blendColor)[0] * *(u8*)(bg + 0x14)) / 255;

    tevColor = blendColor;
    GXSetTevColor(1, &tevColor);

    if ((*(u16*)bg & 2) == 0) {
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 8, 2, 0xF);
        GXSetTevAlphaIn(0, 7, 4, 1, 7);

        TEXGetGXTexObjFromPalette(*(s32*)(bg + 0x1C), texObj, 0);
        GXLoadTexObj(texObj, 0);

        height = GXGetTexObjHeight(texObj);
        height &= 0xFFFF;
        scaleY = float_480_8041f600 / (f32)height;

        width = GXGetTexObjWidth(texObj);
        width &= 0xFFFF;

        PSMTXScale(
            scaleMtx,
            float_608_8041f5fc / (f32)width,
            scaleY,
            float_1_8041f604
        );

        if ((*(u16*)bg & 4) != 0) {
            PSMTXTrans(
                transMtx,
                *(f32*)(bg + 0x0C),
                *(f32*)(bg + 0x10),
                float_0_8041f608
            );
        } else {
            PSMTXTrans(
                transMtx,
                *(f32*)(bg + 0x04) + *(f32*)(bg + 0x0C),
                *(f32*)(bg + 0x08) + *(f32*)(bg + 0x10),
                float_0_8041f608
            );
        }

        PSMTXConcat(transMtx, scaleMtx, transMtx);
        GXLoadTexMtxImm(transMtx, 0x1E, 1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        GXSetNumTexGens(1);
    } else {
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);

        matColor = *(u32*)(bg + 0x18);
        GXSetChanMatColor(4, &matColor);

        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0xFF, 0xFF, 4);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xA, 2, 0xF);
        GXSetTevAlphaIn(0, 7, 5, 1, 7);
    }

    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);

    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    GXBegin(0x80, 0, 4);

    FIFO_U16 = 0;
    FIFO_U16 = 0;
    FIFO_U16 = 0;
    FIFO_F32_BG = READ_F32_BG(float_0_8041f608);
    FIFO_F32_BG = READ_F32_BG(float_0_8041f608);

    FIFO_U16 = 0x260;
    FIFO_U16 = 0;
    FIFO_U16 = 0;
    FIFO_F32_BG = READ_F32_BG(float_1_8041f604);
    FIFO_F32_BG = READ_F32_BG(float_0_8041f608);

    FIFO_U16 = 0x260;
    FIFO_U16 = 0x1E0;
    FIFO_U16 = 0;
    FIFO_F32_BG = READ_F32_BG(float_1_8041f604);
    FIFO_F32_BG = READ_F32_BG(float_1_8041f604);

    FIFO_U16 = 0;
    FIFO_U16 = 0x1E0;
    FIFO_U16 = 0;
    FIFO_F32_BG = READ_F32_BG(float_0_8041f608);
    FIFO_F32_BG = READ_F32_BG(float_1_8041f604);
}

void bgMain(void) {
    u8* bg;
    void* cam;
    f32 sinValue;
    f32 cosValue;
    f32 offset;
    s32 tileCount;
    u8 texObj[0x20];
    s32 width;
    f32 angle;

    bg = (u8*)bgGetWork();

    if ((*(u16*)bg & 1) == 0) {
        return;
    }

    cam = camGetPtr(4);

    cosValue = reviseAngle(-*(f32*)((s32)cam + 0x114));
    cosValue = (float_6p2832_8041f60c * cosValue) / float_360_8041f610;

    sinValue = (f32)sin(cosValue);
    cosValue = (f32)cos(cosValue);

    offset = float_0_8041f608;

    if ((*(u16*)bg & 2) == 0) {
        TEXGetGXTexObjFromPalette(*(s32*)(bg + 0x1C), texObj, 0);
        tileCount = (s32)*(u16*)((s32)gp + 0x170) / (s32)(u16)GXGetTexObjWidth(texObj);
    } else {
        tileCount = 0;
    }

    if ((*(u16*)bg & 8) == 0) {
        offset += float_0p001_8041f614 *
            ((sinValue * *(f32*)((s32)cam + 0x18)) -
             (cosValue * *(f32*)((s32)cam + 0x20)));
    }

    cam = camGetPtr(4);

    angle = *(f32*)((s32)cam + 0x114) / float_360_8041f610;
    angle = float_4_8041f618 * angle;

    offset += (f32)tileCount * angle;

    *(f32*)(bg + 0x04) = offset;

    dispEntry(3, 3, bgDisp, 0, float_0_8041f608);
}

void bgTransOffsetOn(void) {
    *(u16*)bgGetWork() &= ~8;
}

void bgTransOffsetOff(void) {
    *(u16*)bgGetWork() |= 8;
}

void bgAutoScrollOn(void) {
    *(u16*)bgGetWork() &= ~4;
}

void bgAutoScrollOff(void) {
    *(u16*)bgGetWork() |= 4;
}

void bgSetScrlOffset(f32 x, f32 y) {
    void* wp = bgGetWork();

    *(f32*)((s32)wp + 0xC) = x;
    *(f32*)((s32)wp + 0x10) = y;
}

void bgDispOff(void) {
    *(u16*)bgGetWork() &= ~1;
}

void bgDispOn(void) {
    *(u16*)bgGetWork() |= 1;
}

void bgSetColor(void* color) {
    void* wp;

    wp = bgGetWork();
    *(u32*)((s32)wp + 0x14) = *(volatile u32*)color;
}

void bgReEntry(char* name) {
    typedef struct BgEntryWork {
        u16 flags;          // 0x00
        u8 pad_02[0x12];    // 0x02
        u32 color;          // 0x14
        u32 failColor;      // 0x18
        void* texData;      // 0x1C
    } BgEntryWork;

    BgEntryWork* wp;
    unsigned long length;
    s32 root;
    char path[0x100];

    wp = (BgEntryWork*)bgGetWork();
    _mapFree(mapalloc_base_ptr, wp->texData);

    wp = (BgEntryWork*)bgGetWork();
    length = 0;

    if (name == NULL) {
        wp->flags &= ~1;
        return;
    }

    wp->texData = NULL;

    arcOpen(name, &wp->texData, &length);

    if (wp->texData == NULL) {
        root = getMarioStDvdRoot();
        sprintf(path, str_PCTs_PCTs_8041f61c, root, name);

        name = DVDMgrOpen(path, 2, 0);
        if (name != NULL) {
            length = DVDMgrGetLength(name);

            wp->texData = _mapAlloc(mapalloc_base_ptr, (length + 0x1F) & ~0x1F);

            DVDMgrRead(
                name,
                wp->texData,
                (length + 0x1F) & ~0x1F,
                0
            );

            DVDMgrClose(name);
        }
    }

    if (wp->texData != NULL) {
        UnpackTexPalette(wp->texData);

        wp->flags |= 1;
        wp->flags &= ~2;
        wp->flags &= ~4;
        wp->color = dat_8041f5f0;
    } else {
        wp->flags |= 2;
        wp->failColor = dat_8041f5f4;
        wp->flags |= 1;
        wp->color = dat_8041f5f8;
    }
}

void bgEntry(char* name) {
    typedef struct BgEntryWork {
        u16 flags;          // 0x00
        u8 pad_02[0x12];    // 0x02
        u32 color;          // 0x14
        u32 failColor;      // 0x18
        void* texData;      // 0x1C
    } BgEntryWork;

    BgEntryWork* wp;
    unsigned long length;
    s32 root;
    char path[0x100];

    wp = (BgEntryWork*)bgGetWork();
    length = 0;

    if (name == NULL) {
        wp->flags &= ~1;
        return;
    }

    wp->texData = NULL;

    arcOpen(name, &wp->texData, &length);

    if (wp->texData == NULL) {
        root = getMarioStDvdRoot();
        sprintf(path, str_PCTs_PCTs_8041f61c, root, name);

        name = DVDMgrOpen(path, 2, 0);
        if (name != NULL) {
            length = DVDMgrGetLength(name);

            wp->texData = _mapAlloc(mapalloc_base_ptr, (length + 0x1F) & ~0x1F);

            DVDMgrRead(
                name,
                wp->texData,
                (length + 0x1F) & ~0x1F,
                0
            );

            DVDMgrClose(name);
        }
    }

    if (wp->texData != NULL) {
        UnpackTexPalette(wp->texData);

        wp->flags |= 1;
        wp->flags &= ~2;
        wp->flags &= ~4;
        wp->color = dat_8041f5f0;
    } else {
        wp->flags |= 2;
        wp->failColor = dat_8041f5f4;
        wp->flags |= 1;
        wp->color = dat_8041f5f8;
    }
}
