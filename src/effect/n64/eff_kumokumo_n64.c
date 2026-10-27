#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "version.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "system.h"
// US effect texture ids are 0x1C lower for these entries.
#define EFF_TEX(id) ((id) - VERSION_PICK(0, 0x1C))



#include "eff/n64/eff_kumokumo_n64.h"

#ifdef __MWERKS__
#endif

s16 cloud_vertex_tbl[] __attribute__((aligned(32))) = {
    25023, 2076, 0, 27200, 6605, 0,
    27743, 12288, 0, 26383, 16490, 0,
    24751, 17685, 0, 22303, -522, 0,
    22303, 3416, 0, 22303, 8213, 0,
    22303, 13010, 0, 22303, 16949, 0,
    17408, -1592, 0, 17408, 1498, 0,
    17408, 5077, 0, 17408, 8656, 0,
    17408, 11747, 0, 12240, -1914, 0,
    12240, 291, 0, 12240, 2687, 0,
    12240, 5084, 0, 12240, 7290, 0,
    7344, -1979, 0, 7344, -598, 0,
    7344, 768, 0, 7344, 2134, 0,
    7344, 3515, 0, 3535, -1560, 0,
    3535, -729, 0, 3536, 0, 0,
    3536, 729, 0, 3536, 1560, 0,
    0, -788, 0, 0, -351, 0,
    0, 0, 0, 0, 351, 0,
    0, 788, 0,
};
s16 cloud_texcoord0_tbl[] __attribute__((aligned(32))) = {
    0, -13107, 4096, -13107, 4096, -16383, 0, -16383,
    8192, -13107, 8192, -16383, 12288, -13107, 12288, -16383,
    16384, -13107, 16384, -16383, 0, -10649, 4096, -10649,
    8192, -10649, 12288, -10649, 16384, -10649, 0, -8192,
    4096, -8192, 8192, -8192, 12288, -8192, 16384, -8192,
    0, -5734, 4096, -5734, 8192, -5734, 12288, -5734,
    16384, -5734, 0, -3276, 4096, -3276, 8192, -3276,
    12288, -3276, 16384, -3276, 0, 0, 4096, 0,
    8192, 0, 12288, 0, 16384, 0,
};
s16 cloud_texcoord1_tbl[] __attribute__((aligned(32))) = {
    0, -13107, 4096, -13107, 4096, -16383, 0, -16383,
    8192, -13107, 8192, -16383, 12288, -13107, 12288, -16383,
    16384, -13107, 16384, -16383, 0, -10649, 4096, -10649,
    8192, -10649, 12288, -10649, 16384, -10649, 0, -8192,
    4096, -8192, 8192, -8192, 12288, -8192, 16384, -8192,
    0, -5734, 4096, -5734, 8192, -5734, 12288, -5734,
    16384, -5734, 0, -3276, 4096, -3276, 8192, -3276,
    12288, -3276, 16384, -3276, 0, 0, 4096, 0,
    8192, 0, 12288, 0, 16384, 0,
};
/* GX display lists (GX_TRIANGLESTRIP 0x98 with u8 position/normal/texcoord indices, 32-byte padded), drawn with GXCallDisplayList */
u32 cloud_dl_0_0[] __attribute__((aligned(32))) = {0x98000E1E, 0x00001E1E, 0x1F00001F, 0x1F190001, 0x19191A00, 0x031A1A14, 0x00011414, 0x15000315, 0x150F0001, 0x0F0F1000, 0x0310100A, 0x00010A0A, 0x0B00030B, 0x0B050000, 0x00000600, 0x01010100, 0x00000303, 0x01000002, 0x02000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 cloud_dl_0_1[] __attribute__((aligned(32))) = {0x98000E1F, 0x00001F1F, 0x20000020, 0x201A0003, 0x1A1A1B00, 0x041B1B15, 0x00031515, 0x16000416, 0x16100003, 0x10101100, 0x0411110B, 0x00030B0B, 0x0C00040C, 0x0C060001, 0x01010700, 0x02040401, 0x00000202, 0x02000005, 0x05000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 cloud_dl_0_2[] __attribute__((aligned(32))) = {0x98000E20, 0x00002020, 0x21000021, 0x211B0004, 0x1B1B1C00, 0x031C1C16, 0x00041616, 0x17000317, 0x17110004, 0x11111200, 0x0312120C, 0x00040C0C, 0x0D00030D, 0x0D070002, 0x04040800, 0x01060602, 0x00000505, 0x03000007, 0x07000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 cloud_dl_0_3[] __attribute__((aligned(32))) = {0x98000E21, 0x00002121, 0x22000022, 0x221C0003, 0x1C1C1D00, 0x011D1D17, 0x00031717, 0x18000118, 0x18120003, 0x12121300, 0x0113130D, 0x00030D0D, 0x0E00010E, 0x0E080001, 0x06060900, 0x00080803, 0x00000707, 0x04000009, 0x09000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
s16 batten_vertex_tbl[] __attribute__((aligned(32))) = {
    -14192, -15455, 0, -8515, -16128, 0,
    0, -16800, 0, 8515, -16128, 0,
    14192, -15455, 0, -10866, -12768, 0,
    -6519, -12768, 0, 0, -12768, 0,
    6519, -12768, 0, 10866, -12768, 0,
    -8512, -10080, 0, -5107, -10080, 0,
    0, -10080, 0, 5107, -10080, 0,
    8512, -10080, 0, -7277, -8064, 0,
    -4366, -8064, 0, 0, -8064, 0,
    4366, -8064, 0, 7277, -8064, 0,
    -6205, -6048, 0, -3723, -6048, 0,
    0, -6048, 0, 3723, -6048, 0,
    6205, -6048, 0, -4761, -2688, 0,
    -2857, -2688, 0, 0, -2688, 0,
    2857, -2688, 0, 4761, -2688, 0,
    -4285, 0, 0, -2571, 0, 0,
    0, 0, 0, 2571, 0, 0,
    4285, 0, 0,
};
s16 batten_texcoord0_tbl[] __attribute__((aligned(32))) = {
    0, 10922, 4096, 10922, 4096, 12288, 0, 12288,
    8192, 10922, 8192, 12288, 12288, 10922, 12288, 12288,
    16384, 10922, 16384, 12288, 0, 9557, 4096, 9557,
    8192, 9557, 12288, 9557, 16384, 9557, 0, 8192,
    4096, 8192, 8192, 8192, 12288, 8192, 16384, 8192,
    0, 6826, 4096, 6826, 8192, 6826, 12288, 6826,
    16384, 6826, 0, 5461, 4096, 5461, 8192, 5461,
    12288, 5461, 16384, 5461, 0, 4096, 4096, 4096,
    8192, 4096, 12288, 4096, 16384, 4096,
};
s16 batten_texcoord1_tbl[] __attribute__((aligned(32))) = {
    0, 10922, 4096, 10922, 4096, 12288, 0, 12288,
    8192, 10922, 8192, 12288, 12288, 10922, 12288, 12288,
    16384, 10922, 16384, 12288, 0, 9557, 4096, 9557,
    8192, 9557, 12288, 9557, 16384, 9557, 0, 8192,
    4096, 8192, 8192, 8192, 12288, 8192, 16384, 8192,
    0, 6826, 4096, 6826, 8192, 6826, 12288, 6826,
    16384, 6826, 0, 5461, 4096, 5461, 8192, 5461,
    12288, 5461, 16384, 5461, 0, 4096, 4096, 4096,
    8192, 4096, 12288, 4096, 16384, 4096,
};
/* GX display lists (GX_TRIANGLESTRIP 0x98 with u8 position/normal/texcoord indices, 32-byte padded), drawn with GXCallDisplayList */
u32 batten_dl_0_0[] __attribute__((aligned(32))) = {0x98000E1E, 0x00021E1E, 0x1F00021F, 0x1F190000, 0x19191A00, 0x041A1A14, 0x00001414, 0x15000415, 0x150F0000, 0x0F0F1000, 0x0410100A, 0x00000A0A, 0x0B00040B, 0x0B050000, 0x00000600, 0x01010100, 0x00020303, 0x01000202, 0x02000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 batten_dl_0_1[] __attribute__((aligned(32))) = {0x98000E1F, 0x00021F1F, 0x20000220, 0x201A0004, 0x1A1A1B00, 0x051B1B15, 0x00041515, 0x16000516, 0x16100004, 0x10101100, 0x0511110B, 0x00040B0B, 0x0C00050C, 0x0C060001, 0x01010700, 0x03040401, 0x00020202, 0x02000205, 0x05000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 batten_dl_0_2[] __attribute__((aligned(32))) = {0x98000E20, 0x00022020, 0x21000221, 0x211B0005, 0x1B1B1C00, 0x041C1C16, 0x00051616, 0x17000417, 0x17110005, 0x11111200, 0x0412120C, 0x00050C0C, 0x0D00040D, 0x0D070003, 0x04040800, 0x01060602, 0x00020505, 0x03000207, 0x07000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 batten_dl_0_3[] __attribute__((aligned(32))) = {0x98000E21, 0x00022121, 0x22000222, 0x221C0004, 0x1C1C1D00, 0x001D1D17, 0x00041717, 0x18000018, 0x18120004, 0x12121300, 0x0013130D, 0x00040D0D, 0x0E00000E, 0x0E080001, 0x06060900, 0x00080803, 0x00020707, 0x04000209, 0x09000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
s8 cloud_normal_tbl[] __attribute__((aligned(32))) = {
    0, 0, 64,
};
GXColor cloud_color0_tbl[] __attribute__((aligned(32))) = {
    {255, 255, 255, 0}, {255, 255, 255, 51}, {255, 255, 255, 102}, {255, 255, 255, 127},
    {255, 255, 255, 204},
};
void* cloud_dl_0_tbl[] = {cloud_dl_0_0, cloud_dl_0_1, cloud_dl_0_2, cloud_dl_0_3};
u8 cloud_dl_0_size_tbl[] = {0x03, 0x03, 0x03, 0x03};
s8 batten_normal_tbl[] __attribute__((aligned(32))) = {
    0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
GXColor batten_color0_tbl[] __attribute__((aligned(32))) = {
    {255, 255, 255, 51}, {255, 255, 255, 102}, {255, 255, 255, 0}, {255, 255, 255, 153},
    {255, 255, 255, 127}, {255, 255, 255, 204},
};
void* batten_dl_0_tbl[] = {batten_dl_0_0, batten_dl_0_1, batten_dl_0_2, batten_dl_0_3};
u8 batten_dl_0_size_tbl[] = {0x03, 0x03, 0x03, 0x03};
u8 rz_tbl[] = {0x00, 0x09, 0x12, 0x1C, 0x25, 0x2F, 0x39, 0x43, 0x4D, 0x57, 0x61, 0x6B, 0x74, 0x7D, 0x86, 0x8F, 0x97, 0x9F, 0xA6, 0xAD, 0xB4, 0xB8, 0xB9, 0xB9, 0xB6, 0xB4, 0xB1, 0xAE, 0xAE, 0xAF, 0xB4};

void effKumokumoMain(EffectEntry* effect);
void effKumokumoDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effKumokumoN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 rot, f32 unused, f32 scale) {
    EffectEntry* effect;
    EffKumokumoN64Particle* work;
    EffKumokumoN64Particle* p;
    s32 count;
    s32 i;

    effect = effEntry();
    count = 11;
    if (type == 1) {
        count = 1;
    }
    if (type == 2) {
        rot = 0.0f;
    }
    effect->type = "KumokumoN64";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffKumokumoN64Particle));
    effect->userdata = work;
    effect->callback = effKumokumoMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->frame = 0;
    if (time <= 0) {
        work->delay = 1000;
    } else {
        work->delay = time;
    }
    work->alpha = 0;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->a2 = 255;
    work->rot = rot;
    work->camera = 4;
    work->s1 = 0.0f;
    work->t1 = 0.0f;
    work->s2 = 0.0f;
    work->t2 = 0.0f;
    work->ds1 = 0.0f;
    work->dt1 = 0.018181818f;
    work->ds2 = 0.0f;
    work->dt2 = 0.04f;
    if (type == 1 || type == 3) {
        work->ds1 *= -1.0f;
        work->dt1 *= -1.0f;
        work->ds2 *= -1.0f;
        work->dt2 *= -1.0f;
    }
    switch (type) {
    case 0:
        work->r1 = 255;
        work->g1 = 255;
        work->b1 = 150;
        work->r2 = 215;
        work->g2 = 210;
        work->b2 = 10;
        break;
    case 1:
        work->r1 = 230;
        work->g1 = 97;
        work->b1 = 44;
        work->r2 = 104;
        work->g2 = 47;
        work->b2 = 149;
        break;
    case 2:
        work->r1 = 255;
        work->g1 = 255;
        work->b1 = 255;
        work->r2 = 255;
        work->g2 = 255;
        work->b2 = 255;
        break;
    case 3:
    case 4:
    default:
        work->r1 = 0;
        work->g1 = 153;
        work->b1 = 255;
        work->r2 = 0;
        work->g2 = 0;
        work->b2 = 0;
        break;
    }
    p = work + 1;
    for (i = 1; i < effect->count; i++, p++) {
        switch (type) {
        case 0:
        case 1:
            p->x = 0.0f;
            p->y = 3.0f;
            p->z = 0.0f;
            p->scale = 0.0f;
            p->alpha = 204;
            p->delay = i * 30;
            p->rot = (f32)-irand(20);
            p->s1 = 8.0f;
            break;
        case 2:
            p->x = (f32)(200 - irand(400));
            p->y = -(f32)(irand(50) + 200);
            p->z = 0.0f;
            p->scale = 1.0f;
            p->alpha = 0;
            p->delay = i * 15;
            p->rot = 0.0f;
            break;
        case 3:
            rot = (f32)irand(360);
            scale = 200.0f + (f32)irand(200);
            rot = 6.2831855f * rot / 360.0f;
            p->x = scale * (f32)sin(rot);
            p->y = scale * (f32)cos(rot);
            p->z = 0.0f;
            p->scale = 2.0f;
            p->alpha = 0;
            p->delay = i * 15;
            p->rot = 0.0f;
            break;
        case 4:
        default:
            p->x = 0.0f;
            p->y = 3.0f;
            p->z = 0.0f;
            p->scale = 0.0f;
            p->alpha = 204;
            p->delay = i * 30;
            p->rot = (f32)-irand(20);
            p->s1 = 8.0f;
            break;
        }
    }
    return effect;
}

void effKumokumoMain(EffectEntry* effect) {
    EffKumokumoN64Particle* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;
    CameraId camera;
    s32 fr;
    f32 ang;
    f32 rad;
    f32 tmp2;

    tmp.x = work->x;
    tmp.y = work->y;
    tmp.z = work->z;
    position = tmp;
    camera = work->camera;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->delay = 16;
    }
    if (work->delay < 1000) {
        work->delay--;
    }
    work->frame++;
    if (work->delay < 0) {
        effDelete(effect);
        return;
    }
    fr = work->frame;
    if (work->delay < 16) {
        work->alpha = work->delay * 16;
    }
    if (fr < 16) {
        work->alpha = fr * 16 + 15;
    }
    work->s1 += work->ds1;
    work->t1 += work->dt1;
    work->s2 += work->ds2;
    work->t2 += work->dt2;
    if (type == 3) {
        work->rot -= 2.0f;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            work->delay--;
            continue;
        }
        switch (type) {
        case 0:
            work->alpha -= 2;
            if (work->alpha < 0) {
                work->x = 0.0f;
                work->y = 3.0f;
                work->z = 0.0f;
                work->scale = 0.0f;
                work->alpha = 204;
                work->delay = i * 30;
                work->rot = (f32)-irand(20);
                work->s1 = 8.0f;
            } else {
                work->scale += (1.5f - work->scale) / 30.0f;
                work->rot += (30.0f - work->rot) / 30.0f;
                work->x += work->s1 * (f32)cos(6.2831855f * work->rot / 360.0f);
                tmp2 = (f32)sin(6.2831855f * work->rot / 360.0f);
                work->y += work->s1 * (2.0f * tmp2);
                work->s1 += (2.0f - work->s1) / 30.0f;
            }
            break;
        case 2:
            work->alpha += (255 - work->alpha) / 4;
            work->scale += -work->scale / 32.0f;
            work->x += -work->x / 32.0f;
            work->y += -work->y / 32.0f;
            work->z += -work->z / 32.0f;
            if (work->scale < 0.1f) {
                work->x = (f32)(200 - irand(400));
                work->y = -(f32)(irand(50) + 200);
                work->z = 0.0f;
                work->scale = 1.0f;
                work->alpha = 0;
                work->delay = i * 15;
                work->rot = 0.0f;
            }
            work->rot = (f32)(u32)(rz_tbl[(u32)work->frame % 31] + (u32)(work->frame * 180) / 31);
            work->rot = fmod(work->rot, 360.0);
            break;
        case 3:
            work->alpha += (192 - work->alpha) / 4;
            work->scale += -work->scale / 32.0f;
            work->x += -work->x / 32.0f;
            work->y += -work->y / 32.0f;
            work->z += -work->z / 32.0f;
            if (work->scale < 0.1f) {
                ang = (f32)irand(360);
                rad = 200.0f + (f32)irand(200);
                ang = 6.2831855f * ang / 360.0f;
                work->x = rad * (f32)sin(ang);
                work->y = rad * (f32)cos(ang);
                work->z = 0.0f;
                work->scale = 3.0f;
                work->alpha = 0;
                work->delay = i * 15;
                work->rot = 0.0f;
            }
            break;
        case 4:
            work->alpha -= 2;
            if (work->alpha < 0) {
                work->x = 0.0f;
                work->y = 3.0f;
                work->z = 0.0f;
                work->scale = 0.0f;
                work->alpha = 204;
                work->delay = i * 30;
                work->rot = (f32)-irand(20);
                work->s1 = 8.0f;
            } else {
                work->scale += (1.0f - work->scale) / 30.0f;
                work->rot += (30.0f - work->rot) / 30.0f;
                work->x += work->s1 * (f32)cos(6.2831855f * work->rot / 360.0f);
                tmp2 = (f32)sin(6.2831855f * work->rot / 360.0f);
                work->y += work->s1 * (2.0f * tmp2);
                work->s1 += (2.0f - work->s1) / 30.0f;
            }
            break;
        }
        work->frame++;
    }
    dispEntry(camera, dispCalcZ(&position), 2, (DispCallback)effKumokumoDisp, effect);
}

static inline void kkDraw(void* vtx, void* tex0, void* tex1, void* norm, void* clr, void** dl, u8* sz) {
    u32 i;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 6);
    GXSetArray(GX_VA_POS, vtx, 6);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetArray(GX_VA_NRM, norm, 3);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetArray(GX_VA_CLR0, clr, 4);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
    GXSetArray(GX_VA_TEX0, tex0, 4);
    GXSetVtxDesc(GX_VA_TEX1, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_S16, 14);
    GXSetArray(GX_VA_TEX1, tex1, 4);
    for (i = 0; i < 4; i++) {
        GXCallDisplayList(dl[i], sz[i] << 5);
    }
}

void effKumokumoDisp(CameraId cameraId, EffectEntry* effect) {
    EffKumokumoN64Particle* work;
    s32 alpha;
    s32 type;
    CameraEntry* camera;
    s32 k;
    s32 j;
    f32 w;
    f32 h;
    f32 tmax;
    Mtx mtx;
    Mtx scl;
    Mtx rot;
    Mtx mtx2;
    GXTexObj texObj;

    work = effect->userdata;
    camera = camGetPtr(cameraId);
    alpha = work->alpha;
    type = work->type;
    PSMTXTrans(mtx, work->x, work->y, work->z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
    PSMTXConcat(mtx, rot, mtx);
    if (type != 3) {
        if (work->rot < 90.0f) {
            PSMTXScale(scl, work->scale, work->scale, work->scale);
        } else {
            PSMTXScale(scl, work->scale, -1.0f * work->scale, work->scale);
        }
        PSMTXConcat(mtx, scl, mtx);
        PSMTXScale(scl, 0.4f, 0.4f, 0.4f);
        PSMTXConcat(mtx, scl, mtx);
    } else {
        PSMTXScale(scl, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, scl, mtx);
        PSMTXScale(scl, 0.1f, 0.1f, 0.1f);
        PSMTXConcat(mtx, scl, mtx);
    }
    PSMTXConcat(camera->view, mtx, mtx2);
    GXLoadPosMtxImm(mtx2, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetTevColor(GX_TEVREG0, (GXColor){work->r1, work->g1, work->b1, alpha});
    GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, work->a2});
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    effGetTexObj(EFF_TEX(0x84), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObj(EFF_TEX(0x85), &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    PSMTXTrans(mtx, work->s1, work->t1, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(mtx, work->s2, work->t2, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    switch (type) {
    case 0:
    case 1:
        kkDraw(cloud_vertex_tbl, cloud_texcoord0_tbl, cloud_texcoord1_tbl, cloud_normal_tbl, cloud_color0_tbl, cloud_dl_0_tbl, cloud_dl_0_size_tbl);
        break;
    case 2:
        kkDraw(batten_vertex_tbl, batten_texcoord0_tbl, batten_texcoord1_tbl, batten_normal_tbl, batten_color0_tbl, batten_dl_0_tbl, batten_dl_0_size_tbl);
        break;
    case 3:
        for (j = 0; j < 5; j++) {
            PSMTXRotRad(rot, 'z', 0.017453292f * (360.0f * j / 5.0f));
            PSMTXConcat(mtx2, rot, rot);
            GXLoadPosMtxImm(rot, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            kkDraw(cloud_vertex_tbl, cloud_texcoord0_tbl, cloud_texcoord1_tbl, cloud_normal_tbl, cloud_color0_tbl, cloud_dl_0_tbl, cloud_dl_0_size_tbl);
        }
        break;
    case 4:
    default:
        kkDraw(cloud_vertex_tbl, cloud_texcoord0_tbl, cloud_texcoord1_tbl, cloud_normal_tbl, cloud_color0_tbl, cloud_dl_0_tbl, cloud_dl_0_size_tbl);
        break;
    }
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    switch (type) {
    case 0:
    case 1:
        effGetTexObj(0x34, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        break;
    case 2:
        effGetTexObj(0xB, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        break;
    case 3:
    case 4:
    default:
        effGetTexObjN64(7, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        break;
    }
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (k = 1; k < effect->count; k++, work++) {
        if (work->delay == 0) {
            if (type != 3 && type != 4) {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xDB, 0xDF, 0xE7, work->alpha * alpha / 255});
            } else {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, work->alpha * alpha / 255});
            }
            PSMTXTrans(mtx, work->x, work->y, work->z);
            PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
            PSMTXScale(scl, work->scale, work->scale, work->scale);
            PSMTXConcat(mtx2, mtx, mtx);
            PSMTXConcat(mtx, rot, mtx);
            PSMTXConcat(mtx, scl, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            w = (u16)GXGetTexObjWidth(&texObj);
            h = (u16)GXGetTexObjHeight(&texObj);
            tmax = 1.0f;
            if (type == 3 || type == 4) {
                tmax = 2.0f;
            }
            GXPosition3f32(-w / 2.0f, h / 2.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(w / 2.0f, h / 2.0f, 0.0f);
            GXTexCoord2f32(tmax, 0.0f);
            GXPosition3f32(w / 2.0f, -h / 2.0f, 0.0f);
            GXTexCoord2f32(tmax, tmax);
            GXPosition3f32(-w / 2.0f, -h / 2.0f, 0.0f);
            GXTexCoord2f32(0.0f, tmax);
        }
    }
}



