#include "eff/eff_funemizu.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

void effFunemizuMain(EffectEntry* effect);
void effFunemizuDisp(CameraId cameraId, EffectEntry* effect);
void effFunemizuShibukiDisp(CameraId cameraId, EffectEntry* effect);

typedef struct ColorEntry {
    char* name;
    GXColor color;
} ColorEntry;

static const GXColor funemizu_default_color = {0xFF, 0xFF, 0xFF, 0xFF};


static s16 funemizu_vertex_tbl[] __attribute__((aligned(32))) = { 20079, -636, 0, 13766, 0, 0, 8902, 0, 0, 5574, 0, 0, 0, 0, 0, 13766, 1536, 0, 8902, 2560, 0, 5574, 2560, 0, 512, 0, 0, 18154, 899, 0, 13766, 3328, 0, 8902, 5376, 0, 5574, 5488, 0, 2246, 2982, 0, 14542, -396, 1249, 10016, 0, 1249, 6528, 0, 1249, 4142, 0, 1249, 512, 0, 1249, 10016, 957, 1249, 6528, 1596, 1249, 4142, 1596, 1249, 13161, 560, 1249, 10016, 2074, 1249, 6528, 3351, 1249, 4142, 3422, 1249, 1755, 1859, 1249};
static s16 funemizu_texcoord0_tbl[] __attribute__((aligned(32))) = { 1024, 4096, 1024, 2048, 0, 3072, 2048, 4096, 2048, 2048, 3072, 4096, 3072, 2048, 4096, 4096, 4096, 2048, 1024, 256, 0, 256, 2048, 256, 3072, 256, 4096, 256, -19456, 4096, -19456, 2048, -20480, 3072, -18432, 4096, -18432, 2048, -17408, 4096, -17408, 2048, -16384, 3072, -19456, 256, -20480, 256, -18432, 256, -17408, 256, -16384, 256};
static u8 funemizu_dl_0_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0A, 0x09, 0x00, 0x01, 0x0A, 0x00, 0x00, 0x00, 0x02, 0x0A, 0x00, 0x01, 0x09, 0x05, 0x00, 0x01, 0x01, 0x0B, 0x00, 0x01, 0x0B, 0x06, 0x00, 0x01, 0x04, 0x0C, 0x00, 0x01, 0x0C, 0x07, 0x00, 0x01, 0x06, 0x0D, 0x00, 0x01, 0x0D, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 funemizu_dl_1_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0A, 0x16, 0x00, 0x01, 0x17, 0x0E, 0x00, 0x00, 0x10, 0x17, 0x00, 0x01, 0x16, 0x13, 0x00, 0x01, 0x0F, 0x18, 0x00, 0x01, 0x18, 0x14, 0x00, 0x01, 0x12, 0x19, 0x00, 0x01, 0x19, 0x15, 0x00, 0x01, 0x14, 0x1A, 0x00, 0x01, 0x1A, 0x12, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static s16 gor_fune_vertex_tbl[] __attribute__((aligned(32))) = { 23605, 0, 0, 16368, 0, 0, 12081, 0, 0, 8184, 0, 0, 4287, 0, 0, 0, 0, 0, 24198, 333, 0, 17754, 3291, 0, 13886, 6565, 0, 10323, 9024, 0, 6398, 9314, 0, 0, 0, 0, 24139, 2048, 0, 18190, 4665, 0, 14467, 8397, 0, 10977, 11084, 0, 7124, 11603, 0, 2223, 9774, 0, 23605, 0, 0, 16368, 0, 0, 12081, 0, 0, 8184, 0, 0, 4287, 0, 0, 0, 0, 0, 24198, 200, 0, 17754, 1974, 0, 13886, 4553, 0, 10323, 6438, 0, 6398, 6612, 0, 0, 0, 0, 24139, 1229, 0, 18190, 2799, 0, 14467, 5652, 0, 10977, 7674, 0, 7124, 7986, 0, 2966, 6422, 0};
static s16 gor_fune_texcoord0_tbl[] __attribute__((aligned(32))) = { -3325, 16384, -3325, 6276, -11004, 6276, -11004, 16384, 4352, 16384, 4352, 6276, 12031, 16384, 12031, 6276, 19709, 16384, 19709, 6276, 27388, 16384, 27388, 6276, -3325, 1024, -11004, 512, 4352, 1024, 12031, 1024, 19709, 1024, 27388, 512, -3325, 15564, -11004, 15564, 4352, 15564, 12031, 15564, 19709, 15564, 27388, 15564 };
static u8 gor_fune_dl_0_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0C, 0x0C, 0x00, 0x01, 0x0D, 0x06, 0x00, 0x00, 0x02, 0x0D, 0x00, 0x01, 0x0C, 0x07, 0x00, 0x01, 0x01, 0x0E, 0x00, 0x01, 0x0E, 0x08, 0x00, 0x01, 0x05, 0x0F, 0x00, 0x01, 0x0F, 0x09, 0x00, 0x01, 0x07, 0x10, 0x00, 0x01, 0x10, 0x0A, 0x00, 0x01, 0x09, 0x11, 0x00, 0x01, 0x11, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 gor_fune_dl_0_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0C, 0x05, 0x00, 0x00, 0x0A, 0x0B, 0x00, 0x00, 0x0B, 0x04, 0x00, 0x00, 0x08, 0x0A, 0x00, 0x01, 0x09, 0x03, 0x00, 0x00, 0x06, 0x09, 0x00, 0x01, 0x07, 0x02, 0x00, 0x00, 0x04, 0x08, 0x00, 0x01, 0x05, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 gor_fune_dl_1_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0C, 0x1E, 0x00, 0x01, 0x0D, 0x18, 0x00, 0x00, 0x02, 0x1F, 0x00, 0x01, 0x0C, 0x19, 0x00, 0x01, 0x01, 0x20, 0x00, 0x01, 0x0E, 0x1A, 0x00, 0x01, 0x05, 0x21, 0x00, 0x01, 0x0F, 0x1B, 0x00, 0x01, 0x07, 0x22, 0x00, 0x01, 0x10, 0x1C, 0x00, 0x01, 0x09, 0x23, 0x00, 0x01, 0x11, 0x1D, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 gor_fune_dl_1_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0C, 0x17, 0x00, 0x00, 0x17, 0x1D, 0x00, 0x00, 0x0B, 0x16, 0x00, 0x00, 0x16, 0x1C, 0x00, 0x01, 0x09, 0x15, 0x00, 0x00, 0x15, 0x1B, 0x00, 0x01, 0x07, 0x14, 0x00, 0x00, 0x14, 0x1A, 0x00, 0x01, 0x05, 0x13, 0x00, 0x00, 0x12, 0x19, 0x00, 0x01, 0x01, 0x12, 0x00, 0x00, 0x13, 0x18, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static struct { ColorEntry e[32]; char* term; } funemizu_color_tbl = { {
    { "dou_02", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_03", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_04", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_05", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_07", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_08", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_09", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_10", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_11", {0x0E, 0x2C, 0xA2, 0x00} },
    { "dou_12", {0x0E, 0x2C, 0xA2, 0x00} },
    { "gon_01", {0x00, 0x61, 0x49, 0x00} },
    { "gor_00", {0x00, 0x30, 0x65, 0x00} },
    { "gor_02", {0x00, 0x30, 0x65, 0x00} },
    { "hei_01", {0x51, 0xC3, 0xCB, 0x00} },
    { "las_04", {0x08, 0x1C, 0x2C, 0x00} },
    { "las_09", {0x45, 0x79, 0x8E, 0x00} },
    { "mri_16", {0x00, 0xA6, 0xFF, 0x00} },
    { "muj_00", {0x0E, 0x2C, 0xA2, 0x00} },
    { "muj_02", {0x20, 0x30, 0x5D, 0x00} },
    { "muj_04", {0x0E, 0x2C, 0xA2, 0x00} },
    { "muj_10", {0x0E, 0x2C, 0xA2, 0x00} },
    { "nok_00", {0x69, 0xC3, 0xD7, 0x00} },
    { "nok_01", {0x69, 0xC3, 0xD7, 0x00} },
    { "pik_00", {0x82, 0xE3, 0xDF, 0x00} },
    { "pik_01", {0x6D, 0xCB, 0xFF, 0x00} },
    { "pik_04", {0x6D, 0xCB, 0xFF, 0x00} },
    { "tik_02", {0x04, 0x2C, 0x51, 0x00} },
    { "tik_15", {0x30, 0x4D, 0x59, 0x00} },
    { "tik_16", {0x30, 0x4D, 0x59, 0x00} },
    { "tik_17", {0x30, 0x4D, 0x59, 0x00} },
    { "tik_18", {0x30, 0x4D, 0x59, 0x00} },
    { "tik_20", {0x30, 0x4D, 0x59, 0x00} },
    }, NULL };

static s8 funemizu_normal_tbl[] __attribute__((aligned(32))) = { 0, 0, 64};
static u8 funemizu_color0_tbl[] __attribute__((aligned(32))) = { 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static u8 funemizu_dl_0_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x07, 0x04, 0x00, 0x00, 0x07, 0x08, 0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x05, 0x07, 0x00, 0x01, 0x06, 0x02, 0x00, 0x00, 0x03, 0x06, 0x00, 0x01, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00 };
static u8 funemizu_dl_0_2[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01, 0x01, 0x06, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8* funemizu_dl_0_tbl[] = { funemizu_dl_0_0, funemizu_dl_0_1, funemizu_dl_0_2 };
static u8 funemizu_dl_0_size_tbl[] = { 0x02, 0x01, 0x01};
static u8 funemizu_dl_1_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x04, 0x12, 0x00, 0x00, 0x15, 0x15, 0x00, 0x01, 0x14, 0x11, 0x00, 0x00, 0x13, 0x10, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 funemizu_dl_1_2[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x04, 0x0E, 0x00, 0x00, 0x10, 0x0F, 0x00, 0x00, 0x0E, 0x13, 0x00, 0x01, 0x0F, 0x14, 0x00, 0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 funemizu_dl_1_3[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x04, 0x15, 0x00, 0x01, 0x14, 0x14, 0x00, 0x01, 0x12, 0x10, 0x00, 0x00, 0x11, 0x0F, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8* funemizu_dl_1_tbl[] = { funemizu_dl_1_0, funemizu_dl_1_1, funemizu_dl_1_2, funemizu_dl_1_3 };
static u8 funemizu_dl_1_size_tbl[] = { 0x02, 0x01, 0x01, 0x01 };
static s8 gor_fune_normal_tbl[] __attribute__((aligned(32))) = { 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static u8 gor_fune_color0_tbl[] __attribute__((aligned(32))) = { 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static u8* gor_fune_dl_0_tbl[] = { gor_fune_dl_0_0, gor_fune_dl_0_1 };
static u8 gor_fune_dl_0_size_tbl[] = { 0x02, 0x02};
static u8* gor_fune_dl_1_tbl[] = { gor_fune_dl_1_0, gor_fune_dl_1_1 };
static u8 gor_fune_dl_1_size_tbl[] = { 0x02, 0x02};

GXColor effFunemizuGetMapColor(void) {
    ColorEntry* e;
    for (e = funemizu_color_tbl.e; e != NULL; e++) {
        if (e->name == NULL) {
            break;
        }
        if (strcmp(e->name, gp->currentMapName) == 0) {
            return e->color;
        }
    }
    for (e = funemizu_color_tbl.e; e != NULL; e++) {
        if (e->name == NULL) {
            break;
        }
        if (strncmp(e->name, gp->currentMapName, 3) == 0) {
            return e->color;
        }
    }
    return funemizu_default_color;
}


EffectEntry* effFunemizuEntry(s32 type, f32 x, f32 y, f32 z, f32 size, f32 yaw) {
    EffectEntry* effect;
    EffFunemizuWork* work;
    EffFunemizuWork* p;
    s32 i;
    s32 count;

    effect = effEntry();
    count = 1;
    if (type == 1) {
        count = (s32)size * 3 + 1;
    }
    if (count > 10) {
        count = 10;
    }
    effect->type = "funemizu";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(EffFunemizuWork));
    effect->userdata = work;
    effect->callback = effFunemizuMain;
    work->type = type;

    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->wave0Width = 0.0f;
    work->wave1Width = 0.0f;
    work->wave0Height = 0.0f;
    work->wave1Height = 0.0f;
    work->wave1Scroll = 0.0f;
    work->wave0Scroll = 0.0f;
    work->timer = 0;
    work->scale = 1.0f;
    work->cam = CAMERA_3D;
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->size = size / 2;
        p->yaw = yaw;
        p->speedY = p->size + irand(2);
        p->gravity = 0.1f + irand(10) / 100.0f;
        p->delay = irand(5);
        p->alpha = 255;
    }
    return effect;
}

void effFunemizuMain(EffectEntry* effect) {
    EffFunemizuWork* work = effect->userdata;
    Vec position;
    Vec v = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    CameraId cam;
    s32 dead = 0;
    f32 a, b, s, c, t, f;

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    type = work->type;
    cam = work->cam;
    work->timer++;
    if (work->timer > 1000) {
        work->timer -= 1000;
    }
    if (work->type == 0 || work->type == 2) {
        f = work->size;
        a = f;
        b = f - 1.0f;
        if (a < 0.0f) {
            a = 0.0f;
        }
        if (a > 1.0f) {
            a = 1.0f;
        }
        if (b < 0.0f) {
            b = 0.0f;
        }
        if (b > 1.0f) {
            b = 1.0f;
        }
        work->wave0Width += (a - work->wave0Width) / 10.0f;
        work->wave1Width += (b - work->wave1Width) / 10.0f;
        if (work->size > 1.0f) {
            work->wave0Height += (a - work->wave0Height) / 10.0f;
            work->wave1Height += (b - work->wave1Height) / 10.0f;
        } else {
            work->wave0Height += (a - work->wave0Height) / 2;
            work->wave1Height += (b - work->wave1Height) / 2;
        }
        work->wave0Scroll += 0.05f * work->wave0Width;
        work->wave1Scroll += 0.1f * work->wave1Width;
        if (work->size > 0.1f && work->timer % 10 == 0) {
            t = 6.2831855f * (work->yaw - camGetPtr(CAMERA_3D)->viewYaw) / 360.0f;
            s = sin(t);
            c = cos(t);
            effFunemizuEntry(1, work->position.x - 14.0f * c, work->position.y, work->position.z - 14.0f * s, work->size, work->yaw);
        }
    } else {
        work->wave0Width = 1.0f;
        for (i = 1, work++; i < effect->count; i++, work++) {
            if (work->position.y < 0.0f) {
                dead++;
            } else if (work->delay) {
                work->delay--;
            } else {
                c = cos(6.2831855f * work->yaw / 360.0f);
                work->position.x += work->size * c;
                s = sin(6.2831855f * work->yaw / 360.0f);
                work->position.z += work->size * -s;
                work->position.y += work->speedY;
                work->speedY -= work->gravity;
                work->alpha -= 8;
                if (work->alpha < 0) {
                    work->alpha = 0;
                }
            }
        }
        if (dead >= effect->count - 1) {
            effDelete(effect);
            return;
        }
    }
    if (type == 0 || type == 2) {
        dispEntry(cam, dispCalcZ(&position), 2, (DispCallback)effFunemizuDisp, effect);
    } else {
        dispEntry(cam, dispCalcZ(&position), 2, (DispCallback)effFunemizuShibukiDisp, effect);
    }
}

static inline void funemizuCallDL(u32 *p_u) {
    u32 u = *p_u;
    for (u = 0; u < 3; u++) {
        GXCallDisplayList(funemizu_dl_0_tbl[u], funemizu_dl_0_size_tbl[u] * 32);
    }
    *p_u = u;
}
void effFunemizuDisp(CameraId cameraId, EffectEntry* effect) {
    EffFunemizuWork* work = effect->userdata;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;
    GXColor color;
    CameraEntry* cam;
    f32 z;
    s32 type;
    s32 i;
    u32 u;

    cam = camGetPtr(cameraId);
    type = work->type;
    color = effFunemizuGetMapColor();
    if (type == 0) {
        funemizu_color0_tbl[0] = color.r;
        funemizu_color0_tbl[1] = color.g;
        funemizu_color0_tbl[2] = color.b;
    } else {
        gor_fune_color0_tbl[0] = color.r;
        gor_fune_color0_tbl[1] = color.g;
        gor_fune_color0_tbl[2] = color.b;
    }
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(work->yaw - camGetPtr(cameraId)->viewYaw));
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scale, trans);
    PSMTXTrans(rot, -15.0f, 0.0f, 0.0f);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(cam->view, trans, mtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    effGetTexObj(0x43, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            GXSetCullMode(GX_CULL_BACK);
        } else {
            GXSetCullMode(GX_CULL_FRONT);
        }
        z = 1.0f - 2.0f * i;
        PSMTXTrans(trans, 0.0f, 0.0f, z);
        PSMTXConcat(mtx, trans, trans);
        PSMTXScale(scale, 1.0f, 1.0f, z);
        PSMTXConcat(trans, scale, trans);
        PSMTXScale(scale, work->wave0Width, work->wave0Height, work->wave0Width);
        PSMTXConcat(trans, scale, scale);
        GXLoadPosMtxImm(scale, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXTrans(scale, work->wave0Scroll, 0.0f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        if (type == 0) {
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 9);
            GXSetArray(GX_VA_POS, funemizu_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, funemizu_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, funemizu_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 12);
            GXSetArray(GX_VA_TEX0, funemizu_texcoord0_tbl, 4);
            funemizuCallDL(&u);
        } else {
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
            GXSetArray(GX_VA_POS, gor_fune_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, gor_fune_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, gor_fune_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
            GXSetArray(GX_VA_TEX0, gor_fune_texcoord0_tbl, 4);
            for (u = 0; u < 2; u++) {
                GXCallDisplayList(gor_fune_dl_0_tbl[u], gor_fune_dl_0_size_tbl[u] * 32);
            }
        }
        PSMTXScale(scale, work->wave1Width, work->wave1Height, work->wave1Width);
        PSMTXConcat(trans, scale, scale);
        GXLoadPosMtxImm(scale, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXTrans(scale, work->wave1Scroll, 0.0f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        if (type == 0) {
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 9);
            GXSetArray(GX_VA_POS, funemizu_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, funemizu_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, funemizu_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 12);
            GXSetArray(GX_VA_TEX0, funemizu_texcoord0_tbl, 4);
            for (u = 0; u < 4; u++) {
                GXCallDisplayList(funemizu_dl_1_tbl[u], funemizu_dl_1_size_tbl[u] * 32);
            }
        } else {
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
            GXSetArray(GX_VA_POS, gor_fune_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, gor_fune_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, gor_fune_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
            GXSetArray(GX_VA_TEX0, gor_fune_texcoord0_tbl, 4);
            for (u = 0; u < 2; u++) {
                GXCallDisplayList(gor_fune_dl_1_tbl[u], gor_fune_dl_1_size_tbl[u] * 32);
            }
        }
    }
}

void effFunemizuShibukiDisp(CameraId cameraId, EffectEntry* effect) {
    EffFunemizuWork* work = effect->userdata;
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;
    CameraEntry* cam;
    s32 i;
    f32 w = 3.0f;
    f32 nw;

    cam = camGetPtr(cameraId);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(work->yaw - camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(cam->view, trans, mtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    effGetTexObj(0x40, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    nw = -w;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay == 0 && !(work->position.y < 0.0f)) {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, work->alpha});
            PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
            PSMTXConcat(mtx, trans, trans);
            GXLoadPosMtxImm(trans, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(nw / 2, w * 0.5f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w * 0.5f, w * 0.5f, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(w * 0.5f, nw / 2, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(nw / 2, nw / 2, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

