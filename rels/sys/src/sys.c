#include "battle/battle_unit.h"
#include "eff/eff_itemget.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "eff/n64/eff_stardust_n64.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"

#include "data/mapdata.h"
#include "pmario_sound.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "sdk/texPalette.h"
#include "manager/fontmgr.h"
#include "driver/msgdrv.h"
#include "memory.h"
#include "string.h"
#include "mario/mariost.h"
#include "manager/filemgr.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "system.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
#ifdef VERSION_G8ME01
#endif
void display_func(CameraId cameraId, void* param);
typedef struct SysChar { f32 x; f32 y; f32 z; f32 rot; s32 timer; s32 state; s32 texid; } SysChar;
#ifdef VERSION_G8ME01
// US: room for 64 letters, plus the chapter subtitle rendered once into a texture
typedef struct SysWork {
    s32 state;
    FileEntry* file;
    f32 rot;
    s32 frame;
    s32 scroll;
    s32 type;
    s32 alpha;
    f32 angle;
    s32 count;
    SysChar chars[64];
    smartEntry* msgTex; // 0x724
    u16 msgTexW;        // 0x728
    u16 msgTexH;        // 0x72A
} SysWork;
#else
typedef struct SysWork { s32 state; FileEntry* file; f32 rot; s32 frame; s32 scroll; s32 type; s32 alpha; f32 angle; s32 count; SysChar chars[8]; } SysWork;
#endif
SysWork* g_wp;

USER_FUNC(bg);
USER_FUNC(display);
USER_FUNC(init);
#ifdef VERSION_G8ME01
USER_FUNC(sys_01_start_end_wait);
#endif
USER_FUNC(start);
#ifdef VERSION_G8ME01
void sys_01_msg_tex_capture_func(CameraId cameraId, void* param);
#endif
s32 getLightColor(f32 angle);
void sys_01_release(void);
static void _unresolved(void);
static void _epilog(void);
void _prolog(void);

EvtWord dummy_evt[2];
#ifdef VERSION_G8ME01
static u32 niji_tbl[6];
#else
static u32 niji_tbl[2];
#endif
static const char* msg_tbl[9];
s8 prologue_jp_tbl[6];
s8 stage1_jp_tbl[6];
s8 stage2_jp_tbl[6];
s8 stage3_jp_tbl[6];
s8 stage4_jp_tbl[6];
s8 stage5_jp_tbl[6];
s8 stage6_jp_tbl[6];
s8 stage7_jp_tbl[6];
s8 stage8_jp_tbl[6];
s8* texid_jp_tbl[9];
#ifdef VERSION_G8ME01
s8 prologue_us_tbl[9];
s8 stage1_us_tbl[10];
s8 stage2_us_tbl[10];
s8 stage3_us_tbl[10];
s8 stage4_us_tbl[10];
s8 stage5_us_tbl[10];
s8 stage6_us_tbl[10];
s8 stage7_us_tbl[10];
s8 stage8_us_tbl[10];
s8* texid_us_tbl[9];
s8 prologue_lang3_tbl[7];
s8 stage1_lang3_tbl[10];
s8 stage2_lang3_tbl[10];
s8 stage3_lang3_tbl[10];
s8 stage4_lang3_tbl[10];
s8 stage5_lang3_tbl[10];
s8 stage6_lang3_tbl[10];
s8 stage7_lang3_tbl[10];
s8 stage8_lang3_tbl[10];
s8* texid_lang3_tbl[9];
s8 prologue_lang4_tbl[9];
s8 stage1_lang4_tbl[11];
s8 stage2_lang4_tbl[11];
s8 stage3_lang4_tbl[11];
s8 stage4_lang4_tbl[11];
s8 stage5_lang4_tbl[11];
s8 stage6_lang4_tbl[11];
s8 stage7_lang4_tbl[11];
s8 stage8_lang4_tbl[11];
s8* texid_lang4_tbl[9];
s8 prologue_lang5_tbl[8];
s8 stage1_lang5_tbl[7];
s8 stage2_lang5_tbl[7];
s8 stage3_lang5_tbl[7];
s8 stage4_lang5_tbl[7];
s8 stage5_lang5_tbl[7];
s8 stage6_lang5_tbl[7];
s8 stage7_lang5_tbl[7];
s8 stage8_lang5_tbl[7];
s8* texid_lang5_tbl[9];
s8 prologue_lang6_tbl[8];
s8 stage1_lang6_tbl[8];
s8 stage2_lang6_tbl[8];
s8 stage3_lang6_tbl[8];
s8 stage4_lang6_tbl[8];
s8 stage5_lang6_tbl[8];
s8 stage6_lang6_tbl[8];
s8 stage7_lang6_tbl[8];
s8 stage8_lang6_tbl[8];
s8* texid_lang6_tbl[9];
s8** lang_tbl[6];
const char* lang_prefix[7];
#else
s8 prologue_us_tbl[9];
s8 stage1_us_tbl[7];
s8 stage2_us_tbl[7];
s8 stage3_us_tbl[7];
s8 stage4_us_tbl[7];
s8 stage5_us_tbl[7];
s8 stage6_us_tbl[7];
s8 stage7_us_tbl[7];
s8 stage8_us_tbl[7];
s8* texid_us_tbl[9];
s8** lang_tbl[2];
const char* lang_prefix[2];
#endif
EvtWord sys_01_init_evt[135];
#ifdef VERSION_G8ME01
EvtWord evt_start[191];
#else
EvtWord evt_start[189];
#endif

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("sys_00", dummy_evt);
    relSetEvtAddr("sys_01", sys_01_init_evt);
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) != 2 && evtGetValue(NULL, GSW_STORY_PROGRESS) != 125 && evtGetValue(NULL, GSW_STORY_PROGRESS) != 229 &&
        evtGetValue(NULL, GSW_STORY_PROGRESS) != 280) {
        psndStopAllFadeOut();
    }
    camGetPtr(CAMERA_2D)->flags |= 0x200;
    camGetPtr(CAMERA_2D)->flags &= ~0x100;
    camGetPtr(CAMERA_2D)->flags |= 0x400;
}

EVT_BEGIN(dummy_evt)
    RETURN
EVT_END()

static u32 niji_tbl[] = {
#ifdef VERSION_G8ME01
    19, 26, 24, 27, 22, 22,
#else
    19, 22,
#endif
};

static const char* msg_tbl[] = {
    "msg_stg_seq_prolouge", "msg_stg_seq_stg1", "msg_stg_seq_stg2", "msg_stg_seq_stg3", "msg_stg_seq_stg4", "msg_stg_seq_stg5", "msg_stg_seq_stg6", "msg_stg_seq_stg7",
    "msg_stg_seq_stg8",
};

static void _epilog(void) {
    voidfunctionptr* dtor;

    if (strcmp(gp->currentMapName, "sys_01") == 0) {
        sys_01_release();
    }
    camGetPtr(CAMERA_2D)->flags &= ~0x200;
    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

static void _unresolved(void) {
}

void sys_01_release(void) {
    if (g_wp->file != NULL) {
        fileFree(g_wp->file);
    }
    g_wp = NULL;
}

s32 getLightColor(f32 angle) {
    f32 a = fmod(angle, 360.0);
    s32 c;

    if (a >= 270.0f) {
        c = 360.0f - a;
    } else if (a >= 180.0f) {
        c = a - 180.0f;
    } else if (a >= 90.0f) {
        c = 180.0f - a;
    } else {
        c = a;
    }
    return 255 - c * 2;
}

static inline void setTevColor(GXColor init) {
    GXColor color = init;

    GXSetTevColor(GX_TEVREG0, color);
}

static inline void setChanMatColor(GXColor init, s32 c) {
    GXColor color = init;

    color.r = c;
    color.g = c;
    color.b = c;
    GXSetChanMatColor(GX_COLOR0A0, color);
}

#ifdef VERSION_G8ME01

// US: draws the chapter subtitle once, flat, and copies it into a texture that display_func bends
void sys_01_msg_tex_capture_func(CameraId cameraId, void* param) {
    SysWork* wp = param;
    CameraEntry* camera = camGetPtr(cameraId);
    const char* msg = msgSearch(msg_tbl[wp->type]);
    f32 width = FontGetMessageWidth(msg);
    Vec eye;
    Vec up;
    Vec target;
    Mtx mtx;
    Mtx scale;
    Mtx saved;
    u16 left;
    u16 right;
    u16 top;
    u16 bottom;
    u16 w;
    u16 h;

    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.z = 240.0f / (f32)tan(MTXDegToRad(25.0f) * 0.5f);
        eye = pos;
    }
    {
        Vec u = {0.0f, 1.0f, 0.0f};
        up = u;
    }
    {
        Vec t = {0.0f, 0.0f, 0.0f};
        target = t;
    }
    if (wp->msgTex != NULL) {
        return;
    }
    PSMTXCopy(camera->view, saved);
    C_MTXLookAt(camera->view, &eye, &up, &target);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    {
        GXColor black = {0x00, 0x00, 0x00, 0xFF};
        GXSetChanMatColor(GX_COLOR0A0, black);
    }
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-gp->fbWidth / 2.0f, gp->efbHeight / 2.0f, 0.0f);
    GXPosition3f32(gp->fbWidth / 2.0f, gp->efbHeight / 2.0f, 0.0f);
    GXPosition3f32(gp->fbWidth / 2.0f, -gp->efbHeight / 2.0f, 0.0f);
    GXPosition3f32(-gp->fbWidth / 2.0f, -gp->efbHeight / 2.0f, 0.0f);
    GXEnd();
    GXSetAlphaUpdate(GX_TRUE);
    GXSetZCompLoc(GX_FALSE);
    sysWaitDrawSync();
    GXClearBoundingBox();
    if (width < 510.0f) {
        FontDrawStart();
        PSMTXTrans(mtx, -width / 2.0f, 0.0f, 0.0f);
        FontDrawMessageMtx(mtx, msg);
    } else {
        FontDrawStart();
        PSMTXScale(scale, 510.0f / width, 1.0f, 1.0f);
        PSMTXTrans(mtx, -255.0f, 0.0f, 0.0f);
        PSMTXConcat(mtx, scale, mtx);
        FontDrawMessageMtx(mtx, msg);
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    left -= left % 2u;
    top -= top % 2u;
    right += right % 2u;
    bottom += bottom % 2u;
    right += 2;
    left -= 2;
    if (right > gp->fbWidth) {
        right = gp->fbWidth;
    }
    if (bottom > gp->efbHeight) {
        bottom = gp->efbHeight;
    }
    w = right - left;
    h = bottom - top;
    wp->msgTexW = w;
    wp->msgTexH = h;
    wp->msgTex = smartAlloc(GXGetTexBufferSize(w, h, GX_TF_RGB5A3, GX_FALSE, 0), 1);
    GXSetTexCopySrc(left, top, w, h);
    GXSetTexCopyDst(w, h, GX_TF_RGB5A3, GX_FALSE);
    GXCopyTex(wp->msgTex->address, GX_FALSE);
    GXPixModeSync();
    PSMTXCopy(saved, camera->view);
}
#endif

void display_func(CameraId cameraId, void* param) {
    SysWork* wp = param;
    CameraEntry* camera = camGetPtr(cameraId);
    s32 i;
    s32 scroll;
    SysChar* chr;
    GXLightObj light;
    Vec lightPos;
    GXTexObj tex;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx proj;
    Mtx view;
    Vec eye;
    Vec up;
    Vec target;
#ifdef VERSION_G8ME01
    f32 h;
    f32 size;
    s32 full;
    s32 rem;
    f32 r;
    f32 v;
    f32 c;
    f32 rad;
    f32 w;
    f32 u;
    f32 deg;
    f32 s;
#else
    f32 w;
    f32 h;
    f32 size;
    const char* msg;
    char buf[3];
    s32 full;
    s32 rem;
#endif

    scroll = wp->scroll;
    wp->scroll++;
#ifndef VERSION_G8ME01
    if (scroll > 0x500) {
        scroll -= 0x500;
    }
#endif

    {
        GXColor amb = {0x80, 0x80, 0x80, 0xFF};
        GXSetChanAmbColor(GX_COLOR0A0, amb);
    }
    {
        Vec pos = {0.0f, 0.0f, 300.0f};
        lightPos = pos;
    }
    PSMTXMultVec(camera->view, &lightPos, &lightPos);
    GXInitLightPos(&light, lightPos.x, lightPos.y, lightPos.z);
    {
        GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
        GXInitLightColor(&light, white);
    }
    GXInitLightAttn(&light, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    GXLoadLightObjImm(&light, GX_LIGHT0);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    wp->rot += 2.5f;
    if (wp->rot > 180.0f) {
        wp->rot = 0.0f;
        wp->frame++;
        wp->frame %= 4;
    }
    setChanMatColor((GXColor){0x00, 0x00, 0x00, 0xFF}, getLightColor(90.0f + wp->rot));
    PSMTXTrans(mtx, 0.0f, -40.0f, 0.0f);
    PSMTXRotRad(rot, 'y', 0.017453292f * (90.0f + wp->rot));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    PSMTXInvXpose(mtx, mtx);
    GXLoadNrmMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    TEXGetGXTexObjFromPalette(*wp->file->data, &tex, (u16)wp->frame);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    w = (f32)GXGetTexObjWidth(&tex) / 2;
    h = (f32)GXGetTexObjHeight(&tex) / 2;
    GXPosition3f32(-w / 2, h / 2, 0.0f);
    GXNormal3f32(0.0f, 0.0f, 1.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w / 2, h / 2, 0.0f);
    GXNormal3f32(0.0f, 0.0f, 1.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w / 2, -h / 2, 0.0f);
    GXNormal3f32(0.0f, 0.0f, 1.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-w / 2, -h / 2, 0.0f);
    GXNormal3f32(0.0f, 0.0f, 1.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();

#ifdef VERSION_G8ME01
    // US: the subtitle texture from sys_01_msg_tex_capture_func is bent into a ring strip and faded in along it
    PSMTXTrans(mtx, 0.0f, -40.0f, 0.0f);
    PSMTXRotRad(rot, 'z', 0.017453292f * wp->angle);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(mtx, 1.05f * (512.0f / wp->msgTexW), 1.0f, 1.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXInitTexObj(&tex, wp->msgTex->address, wp->msgTexW, wp->msgTexH, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 64);
    for (i = 0; i < 64; i++) {
        if (i & 1) {
            r = 40.0f;
        } else {
            r = 54.0f;
        }
        deg = (360.0f / 31.0f) * (i / 2);
        u = 0.032258064f * (i / 2);
        if (i & 1) {
            v = 1.0f;
        } else {
            v = 0.0f;
        }
        rad = 6.2831855f * deg / 360.0f;
        full = wp->alpha / 256;
        rem = wp->alpha % 256;
        c = cos(rad);
        s = sin(rad);
        GXPosition3f32(r * s, r * c, 0.0f);
        if (i < full) {
            GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
        } else if (i == full) {
            GXColor4u8(0xFF, 0xFF, 0xFF, rem);
        } else {
            GXColor4u8(0xFF, 0xFF, 0xFF, 0);
        }
        GXTexCoord2f32(u, v);
    }
#else
    msg = msgSearch(msg_tbl[wp->type]);
    FontDrawStart();
    full = wp->alpha / 256;
    rem = wp->alpha % 256;
    for (i = 0; i < 44; i += 2) {
        if (i >= strlen(msg)) {
            break;
        }
        *(u16*)buf = *(u16*)&msg[i];
        buf[2] = 0;
        PSMTXTrans(mtx, 0.0f, -40.0f, 0.0f);
        PSMTXRotRad(rot, 'z', 0.017453292f * (-8.181818f * i + wp->angle));
        PSMTXConcat(mtx, rot, rot);
        PSMTXTrans(mtx, 0.0f, 57.5f, 0.0f);
        PSMTXConcat(rot, mtx, mtx);
        PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
        PSMTXConcat(mtx, scale, mtx);
        if (i / 2 < full) {
            FontDrawStart_alpha(0xFF);
            FontDrawStringMtx(mtx, buf);
        } else {
            FontDrawStart_alpha(rem);
            FontDrawStringMtx(mtx, buf);
            break;
        }
    }
#endif

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTevStages(4);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP1, GX_TEV_SWAP1);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    setTevColor((GXColor){0x36, 0x36, 0x36, 0xFF});
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_RASC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_RASA, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);

    size = 44.0f;
    for (i = 0; i < wp->count; i++) {
        chr = &wp->chars[i];
        if (chr->state == 0) {
            break;
        }
#ifdef VERSION_G8ME01
        if (chr->texid == -2) {
            continue;
        }
#endif
        TEXGetGXTexObjFromPalette(*wp->file->data, &tex, (u16)chr->texid);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        TEXGetGXTexObjFromPalette(*wp->file->data, &tex, (u16)niji_tbl[gp->language]);
        GXLoadTexObj(&tex, GX_TEXMAP1);
        setChanMatColor((GXColor){0x00, 0x00, 0x00, 0xFF}, getLightColor(chr->rot));
        if ((u32)gp->language != 0) {
            PSMTXScale(scale, 0.8f, 0.8f, 0.8f);
        } else {
            PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        }
        PSMTXTrans(mtx, chr->x, chr->y, chr->z);
        PSMTXRotRad(rot, 'y', 0.017453292f * chr->rot);
        PSMTXConcat(scale, mtx, mtx);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        PSMTXInvXpose(mtx, mtx);
        GXLoadNrmMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.z = 1.0f / (f32)tan(MTXDegToRad(25.0f) * 0.5f);
            eye = pos;
        }
        {
            Vec u = {0.0f, 1.0f, 0.0f};
            up = u;
        }
        {
            Vec t = {0.0f, 0.0f, 0.0f};
            target = t;
        }
        C_MTXLightPerspective(proj, 25.0f, 640.0f, 0.5f, 0.5f, 0.5f, 0.5f);
        C_MTXLookAt(view, &eye, &up, &target);
        PSMTXConcat(proj, view, proj);
        if ((u32)gp->language != 0) {
            PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
        } else {
            PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        }
        PSMTXTrans(mtx, chr->x, chr->y, chr->z);
        PSMTXConcat(scale, mtx, mtx);
        PSMTXConcat(proj, mtx, mtx);
        PSMTXTrans(scale, -scroll, 0.0f, 0.0f);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-size / 2, size / 2, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(size / 2, size / 2, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(size / 2, -size / 2, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-size / 2, -size / 2, 0.0f);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

s8 prologue_jp_tbl[] = {
    16, 17, 17, 6, 18, -1,
};

s8 stage1_jp_tbl[] = {
    4, 5, 6, 7, 8, -1,
};

s8 stage2_jp_tbl[] = {
    4, 5, 6, 7, 9, -1,
};

s8 stage3_jp_tbl[] = {
    4, 5, 6, 7, 10, -1,
};

s8 stage4_jp_tbl[] = {
    4, 5, 6, 7, 11, -1,
};

s8 stage5_jp_tbl[] = {
    4, 5, 6, 7, 12, -1,
};

s8 stage6_jp_tbl[] = {
    4, 5, 6, 7, 13, -1,
};

s8 stage7_jp_tbl[] = {
    4, 5, 6, 7, 14, -1,
};

s8 stage8_jp_tbl[] = {
    4, 5, 6, 7, 15, -1,
};

s8* texid_jp_tbl[] = {
    prologue_jp_tbl, stage1_jp_tbl, stage2_jp_tbl, stage3_jp_tbl, stage4_jp_tbl, stage5_jp_tbl, stage6_jp_tbl, stage7_jp_tbl, stage8_jp_tbl,
};

#ifdef VERSION_G8ME01
// US: one title texture set per language; -2 marks a word gap
s8 prologue_us_tbl[] = {
    19, 20, 21, 22, 21, 23, 24, 25, -1,
#else
s8 prologue_us_tbl[] = {
    17, 18, 19, 20, 19, 7, 21, 8, -1,
#endif
};

s8 stage1_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 11, -1,
#else
    4, 5, 6, 7, 8, 9, -1,
#endif
};

s8 stage2_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 12, -1,
#else
    4, 5, 6, 7, 8, 10, -1,
#endif
};

s8 stage3_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 13, -1,
#else
    4, 5, 6, 7, 8, 11, -1,
#endif
};

s8 stage4_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 14, -1,
#else
    4, 5, 6, 7, 8, 12, -1,
#endif
};

s8 stage5_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 15, -1,
#else
    4, 5, 6, 7, 8, 13, -1,
#endif
};

s8 stage6_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 16, -1,
#else
    4, 5, 6, 7, 8, 14, -1,
#endif
};

s8 stage7_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 17, -1,
#else
    4, 5, 6, 7, 8, 15, -1,
#endif
};

s8 stage8_us_tbl[] = {
#ifdef VERSION_G8ME01
    4, 5, 6, 7, 8, 9, 10, -2, 18, -1,
#else
    4, 5, 6, 7, 8, 16, -1,
#endif
};

s8* texid_us_tbl[] = {
    prologue_us_tbl, stage1_us_tbl, stage2_us_tbl, stage3_us_tbl, stage4_us_tbl, stage5_us_tbl, stage6_us_tbl, stage7_us_tbl, stage8_us_tbl,
};

#ifdef VERSION_G8ME01
s8 prologue_lang3_tbl[] = {
    19, 20, 21, 22, 21, 23, -1,
};

s8 stage1_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 11, -1,
};

s8 stage2_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 12, -1,
};

s8 stage3_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 13, -1,
};

s8 stage4_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 14, -1,
};

s8 stage5_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 15, -1,
};

s8 stage6_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 16, -1,
};

s8 stage7_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 17, -1,
};

s8 stage8_lang3_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, -2, 18, -1,
};

s8* texid_lang3_tbl[] = {
    prologue_lang3_tbl, stage1_lang3_tbl, stage2_lang3_tbl, stage3_lang3_tbl, stage4_lang3_tbl, stage5_lang3_tbl, stage6_lang3_tbl, stage7_lang3_tbl, stage8_lang3_tbl,
};

s8 prologue_lang4_tbl[] = {
    20, 21, 22, 23, 22, 24, 25, 26, -1,
};

s8 stage1_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 12, -1,
};

s8 stage2_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 13, -1,
};

s8 stage3_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 14, -1,
};

s8 stage4_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 15, -1,
};

s8 stage5_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 16, -1,
};

s8 stage6_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 17, -1,
};

s8 stage7_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 18, -1,
};

s8 stage8_lang4_tbl[] = {
    4, 5, 6, 7, 8, 9, 10, 11, -2, 19, -1,
};

s8* texid_lang4_tbl[] = {
    prologue_lang4_tbl, stage1_lang4_tbl, stage2_lang4_tbl, stage3_lang4_tbl, stage4_lang4_tbl, stage5_lang4_tbl, stage6_lang4_tbl, stage7_lang4_tbl, stage8_lang4_tbl,
};

s8 prologue_lang5_tbl[8] = {
    16, 17, 18, 19, 20, 21, 20, -1,
};

s8 stage1_lang5_tbl[] = {
    4, 5, 6, 7, -2, 8, -1,
};

s8 stage2_lang5_tbl[] = {
    4, 5, 6, 7, -2, 9, -1,
};

s8 stage3_lang5_tbl[] = {
    4, 5, 6, 7, -2, 10, -1,
};

s8 stage4_lang5_tbl[] = {
    4, 5, 6, 7, -2, 11, -1,
};

s8 stage5_lang5_tbl[] = {
    4, 5, 6, 7, -2, 12, -1,
};

s8 stage6_lang5_tbl[] = {
    4, 5, 6, 7, -2, 13, -1,
};

s8 stage7_lang5_tbl[] = {
    4, 5, 6, 7, -2, 14, -1,
};

s8 stage8_lang5_tbl[] = {
    4, 5, 6, 7, -2, 15, -1,
};

s8* texid_lang5_tbl[] = {
    prologue_lang5_tbl, stage1_lang5_tbl, stage2_lang5_tbl, stage3_lang5_tbl, stage4_lang5_tbl, stage5_lang5_tbl, stage6_lang5_tbl, stage7_lang5_tbl, stage8_lang5_tbl,
};

s8 prologue_lang6_tbl[8] = {
    17, 18, 19, 20, 19, 21, 19, -1,
};

s8 stage1_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 9, -1,
};

s8 stage2_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 10, -1,
};

s8 stage3_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 11, -1,
};

s8 stage4_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 12, -1,
};

s8 stage5_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 13, -1,
};

s8 stage6_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 14, -1,
};

s8 stage7_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 15, -1,
};

s8 stage8_lang6_tbl[8] = {
    4, 5, 6, 7, 8, -2, 16, -1,
};

s8* texid_lang6_tbl[] = {
    prologue_lang6_tbl, stage1_lang6_tbl, stage2_lang6_tbl, stage3_lang6_tbl, stage4_lang6_tbl, stage5_lang6_tbl, stage6_lang6_tbl, stage7_lang6_tbl, stage8_lang6_tbl,
};

s8** lang_tbl[] = {
    texid_jp_tbl, texid_us_tbl, texid_lang3_tbl, texid_lang4_tbl, texid_lang5_tbl, texid_lang6_tbl,
#else
s8** lang_tbl[] = {
    texid_jp_tbl, texid_us_tbl,
#endif
};

const char* lang_prefix[] = {
#ifdef VERSION_G8ME01
    "jp", "us", "ge", "fr", "sp", "it", "du",
#else
    "jp", "us",
#endif
};


EVT_BEGIN(sys_01_init_evt)
    CALL(init)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, 0, 0)
    CALL(evt_party_set_pos, 0, 0, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 0, 500, 0, 0, 0, 0, 11)
    CALL(bg)
    CALL(evt_fade_entry, 32, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        CALL(evt_bero_get_entername, LW(1))
        IF_STR_EQUAL(LW(1), STRING("prologue"))
            SET_VALUE(LW(0), 0)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_1"))
            SET_VALUE(LW(0), 1)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_2"))
            SET_VALUE(LW(0), 2)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_3"))
            SET_VALUE(LW(0), 3)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_4"))
            SET_VALUE(LW(0), 4)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_5"))
            SET_VALUE(LW(0), 5)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_6"))
            SET_VALUE(LW(0), 6)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_7"))
            SET_VALUE(LW(0), 7)
        END_IF
        IF_STR_EQUAL(LW(1), STRING("stg_8"))
            SET_VALUE(LW(0), 8)
        END_IF
        CALL(display, LW(0))
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_fade_set_mapchange_type, 0, 17, 1800, 16, 1800)
    CALL(evt_fade_set_mapchange_type, 1, 17, 1800, 16, 1800)
    SPAWN_EVENT(PTR(evt_start))
    RETURN
EVT_END()

EVT_BEGIN(evt_start)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(evt_bero_get_entername, LW(1))
    IF_STR_NOT_EQUAL(LW(1), STRING("stg_3"))
        IF_STR_NOT_EQUAL(LW(1), STRING("stg_5"))
            IF_STR_NOT_EQUAL(LW(1), STRING("stg_6"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_FF_STG_START1"))
            END_IF
        END_IF
    END_IF
    CALL(evt_fade_entry, 35, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(start)
#ifdef VERSION_G8ME01
    CALL(sys_01_start_end_wait)
    WAIT_MS(3000)
#else
    WAIT_MS(5000)
#endif
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_BITS_SET(LW(0), 4864)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_fade_set_mapchange_type, 0, 19, 500, 18, 500)
    CALL(evt_fade_set_mapchange_type, 1, 19, 500, 18, 500)
    CALL(evt_bero_get_entername, LW(1))
    IF_STR_EQUAL(LW(1), STRING("prologue"))
        SET_VALUE(LW(0), 0)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_1"))
        SET_VALUE(LW(0), 1)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_2"))
        SET_VALUE(LW(0), 2)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_3"))
        SET_VALUE(LW(0), 3)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_4"))
        SET_VALUE(LW(0), 4)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_5"))
        SET_VALUE(LW(0), 5)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_6"))
        SET_VALUE(LW(0), 6)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_7"))
        SET_VALUE(LW(0), 7)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("stg_8"))
        SET_VALUE(LW(0), 8)
    END_IF
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_bero_mapchange, STRING("gor_10"), 0)
        CASE_EQUAL(1)
        CALL(evt_bero_mapchange, STRING("hei_00"), STRING("dokan_2"))
        CASE_EQUAL(2)
        CALL(evt_bero_mapchange, STRING("win_06"), STRING("dokan1"))
        CASE_EQUAL(3)
        CALL(evt_bero_mapchange, STRING("tou_20"), 0)
        CASE_EQUAL(4)
        CALL(evt_bero_mapchange, STRING("usu_00"), STRING("dokan_1"))
        CASE_EQUAL(5)
        CALL(evt_bero_mapchange, STRING("muj_20"), 0)
        CASE_EQUAL(6)
        CALL(evt_bero_mapchange, STRING("rsh_02_a"), 0)
        CASE_EQUAL(7)
        CALL(evt_bero_mapchange, STRING("bom_00"), STRING("dokan"))
        CASE_EQUAL(8)
        CALL(evt_bero_mapchange, STRING("las_00"), STRING("w_bero"))
    END_SWITCH
    RETURN
EVT_END()

USER_FUNC(display) {
    SysWork* wp;
#ifdef VERSION_G8ME01
    s32 type = evtGetValue(event, event->args[0]);
    s32 i;
    SysChar* chr;
#else
    SysChar* chr;
    s32 type = evtGetValue(event, event->args[0]);
    s32 i;
#endif

    if (isFirstCall) {
        wp = _mapAlloc(sizeof(SysWork));
        event->userdatap[0] = wp;
        memset(wp, 0, sizeof(SysWork));
        wp->type = type;
        wp->angle = 0.0f;
        wp->count = 0;
        while (lang_tbl[gp->language][type][wp->count] != -1) {
            wp->count++;
        }
        for (i = 0; i < wp->count; i++) {
#ifdef VERSION_G8ME01
            // US: letters are laid out by width (gaps marked -2 are half width), centred on the title
            f32 left = 0.0f;
            f32 total = left;
            s32 j;
            GXTexObj tex;

            for (j = 0; j < i; j++) {
                if (lang_tbl[gp->language][type][j] != -2) {
                    effGetTexObj(lang_tbl[gp->language][type][j], &tex);
                    left += 32.0f;
                } else {
                    left += 16.0f;
                }
            }
            if (lang_tbl[gp->language][type][j] != -2) {
                effGetTexObj(lang_tbl[gp->language][type][j], &tex);
                left += 16.0f;
            } else {
                left += 8.0f;
            }
            for (j = 0; j < wp->count; j++) {
                if (lang_tbl[gp->language][type][j] != -2) {
                    effGetTexObj(lang_tbl[gp->language][type][j], &tex);
                    total += 16.0f;
                } else {
                    total += 8.0f;
                }
            }
            wp->chars[i].x = left - total;
            wp->chars[i].y = 50.0f;
#else
            wp->chars[i].x = 36.0f * i - (f32)((wp->count - 1) * 36) / 2;
            wp->chars[i].y = 40.0f;
#endif
            wp->chars[i].z = 0.0f;
            wp->chars[i].rot = 90.0f;
            wp->chars[i].timer = i * 20 + 1;
            wp->chars[i].state = 0;
            wp->chars[i].texid = lang_tbl[gp->language][type][i];
        }
    }
    wp = event->userdatap[0];
    g_wp = wp;
    switch (wp->state) {
        case 0:
            if (fileAsyncf(4, NULL, "%s/e/%s/effect_sys.tpl", getMarioStDvdRoot(), lang_prefix[gp->language]) != 0U) {
                wp->file = fileAllocf(4, "%s/e/%s/effect_sys.tpl", getMarioStDvdRoot(), lang_prefix[gp->language]);
                wp->state++;
            }
            break;
        case 2:
            for (i = 0; i < wp->count; i++) {
                chr = &wp->chars[i];
                switch (chr->state) {
                    case 0:
                        chr->timer--;
                        if (chr->timer < 0) {
                            chr->timer = 0;
                            chr->state++;
                            effStardustN64SetDrawCam(effStardustN64Entry(4, chr->x, chr->y, chr->z, 40.0f), 5);
                        }
                        break;
                    case 1:
                        chr->rot = intplGetValue(5, 90.0f, 1800.0f, chr->timer, 90);
                        chr->timer++;
                        if (chr->timer > 90) {
                            chr->state++;
                        }
                        break;
                    case 2:
                        if (i == wp->count - 1) {
                            wp->angle += 0.8f;
                            if (wp->angle > 360.0f) {
                                wp->angle -= 360.0f;
                            }
                            wp->alpha += 0x30;
#ifdef VERSION_G8ME01
                            if (wp->alpha > 0x4000) {
                                wp->alpha = 0x4000;
                            }
#else
                            if (wp->alpha > 10000) {
                                wp->alpha = 10000;
                            }
#endif
                        }
                        break;
                }
            }
            break;
        case 3:
            break;
    }
    if (wp->state != 0) {
#ifdef VERSION_G8ME01
        dispEntry((CameraId)4, 0.0f, 2, sys_01_msg_tex_capture_func, wp);
#endif
        dispEntry(CAMERA_3D_EFFECTA, 0.0f, 2, display_func, wp);
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(start) {
    SysWork* wp = g_wp;

    gp->flags |= 0x10;
    if (wp->state < 1) {
        return EVT_RETURN_BLOCK;
    }
    wp->state++;
    return EVT_RETURN_DONE;
}

USER_FUNC(bg) {
    EffectEntry* eff = effItemGetEntry(0, 0.0f, 0.0f, 0.0f);

    ((ItemGetWork*)eff->userdata)->state = 1;
    ((ItemGetWork*)eff->userdata)->camera = 5;
    return EVT_RETURN_DONE;
}

USER_FUNC(init) {
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
// US: waits until every title letter has finished spinning in
USER_FUNC(sys_01_start_end_wait) {
    SysWork* wp = g_wp;
    s32 i;

    for (i = 0; i < wp->count; i++) {
        if (wp->chars[i].state != 2) {
            return EVT_RETURN_BLOCK;
        }
    }
    return EVT_RETURN_DONE;
}
#endif

