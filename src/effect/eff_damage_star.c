#include "eff/eff_damage_star.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_maya_data.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "libc_decls.h"
#include "poly_trig.h"
#include "eff/eff_number.h"

u32 strlen(const char* s);

typedef struct DamageStarWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    Vec velocity;   // 0x10
    f32 rotation;   // 0x1C
    s32 alpha;      // 0x20
    s32 life;       // 0x24
    s32 frame;      // 0x28
    Vec scale;      // 0x2C
    f32 angle;      // 0x38
    s32 number;     // 0x3C
    u32 tick;       // 0x40
    MayaAnim* anim; // 0x44
} DamageStarWork;

MayaHermitKey damageStar_effHermit_0[] = {
    {
        0, // time
        {
            {-640, 0, 8, 0, 0}, {0, 0, 12, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        8, // time
        {
            {-576, 5, 5, 0, 0}, {96, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        18, // time
        {
            {-544, 1, 1, 0, 0}, {0, -1, -1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        25, // time
        {
            {-542, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        30, // time
        {
            {-544, 6, 6, 0, 0}, {0, -4, -4, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        40, // time
        {
            {-448, 9, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        65, // time
        {
            {-448, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
};
MayaHermitKey damageStar_effHermit_1[] = {
    {
        0, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        1, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 7, 0, 0}, {0, 0, 7, 0, 0},
            {0, 0, 7, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        10, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 4, 4, 0, 0}, {64, 1, 1, 0, 0},
            {64, 4, 4, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        18, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {83, 0, 0, 0, 0}, {25, 1, 1, 0, 0},
            {83, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        25, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {51, -1, -1, 0, 0}, {89, 3, 3, 0, 0},
            {51, -1, -1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        30, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, -16, -16, 0, 0}, {64, 0, 0, 0, 0}, {64, -1, -1, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        40, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {-245, -11, -10, 0, 0}, {64, -1, -1, 0, 0}, {64, -1, -1, 0, 0},
            {64, -1, -1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        42, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {-141, 19, 18, 0, 0}, {44, 0, 0, 0, 0}, {44, 0, 0, 0, 0},
            {44, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        46, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {-134, 0, 0, 0, 0}, {64, -1, -1, 0, 0}, {64, -1, -1, 0, 0},
            {64, -1, -1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        65, // time
        {
            {0, 0, 0, 0, 0}, {256, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {-134, 0, 0, 0, 0}, {0, -3, 0, 0, 0}, {0, -3, 0, 0, 0},
            {0, -3, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
};

static u8 scale_data[] = {
    0x32, 0x37, 0x3C, 0x46, 0x50, 0x64, 0x96, 0xDC, 0xF0, 0xC8,
    0x8C, 0x64, 0x8C, 0xA0, 0x8C, 0x96, 0x9B, 0x96, 0x94, 0x96,
};
static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};
s16 damageStar_effFace_pPlane168_0[] = {
    4, // vertex count
    -1024, -1024, 0, 0, 512,
    1024, -1024, 0, 512, 512,
    1024, 1024, 0, 512, 0,
    -1024, 1024, 0, 0, 0,
};
s16* damageStar_effFaceDt_pPlane168 = damageStar_effFace_pPlane168_0;
MayaPoly damageStar_effAnimRST[] = {
    {7, damageStar_effHermit_0, 0, NULL},
    {10, damageStar_effHermit_1, 1, &damageStar_effFaceDt_pPlane168},
};
MayaKeyList damageStar_effAnimRSTDt = {2, damageStar_effAnimRST};
MayaAnimData damageStar_effAnimDt = { { 0 }, 65.0f, &damageStar_effAnimRSTDt };

void effDamageStarMain(EffectEntry* effect);
void effDamageStarDisp(CameraId cameraId, EffectEntry* effect);

#pragma opt_propagation off
#pragma opt_lifetimes off
EffectEntry* effDamageStarEntry(s32 type, s32 number, f32 x, f32 y, f32 z, f32 scale, f32 angle) {
    EffectEntry* effect;
    DamageStarWork* work;
    s32 size;
    s32 n;
    s32 i;
    f32 base;
    f32 a;
    f32 ang;

    effect = effEntry();
    n = number;
    if (number > 20) {
        number = 20;
    }
    effect->type = "DamageStar";
    effect->count = number + 2;
    work = __memAlloc(HEAP_EFFECT, size = (number + 2) * sizeof(DamageStarWork));
    effect->userdata = work;
    memset(work, 0, size);
    effect->callback = effDamageStarMain;
    work->anim = effMayaAnimAlloc(&damageStar_effAnimDt);
    effPlayMayaAnim(work->anim);
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->rotation = scale;
    work->angle = angle;
    work->life = 99;
    work->frame = 0;
    work->number = n;
    work->tick = gp->retraceCount;
    work++;
    base = 30.0f - (f32)(number + 2);
    for (i = 1; i < number + 2; i++, work++) {
        ang = base * (f32)(i * ((i % 2) * 2 - 1) / 2) + angle;
        work->angle = ang;
        work->position.x = 0.0f;
        work->position.y = 0.0f;
        work->position.z = 0.0f;
        a = 6.2831855f * ang / 360.0f;
        work->velocity.x = 1.5f * (scale * -polySinf(a));
        work->velocity.y = 1.5f * (scale * polyCosf(a));
        work->velocity.z = 0.0f;
        work->rotation = 0.0f;
        work->alpha = 255;
        work->scale.x = 0.2f;
        work->scale.y = 0.2f;
        work->scale.z = 0.2f;
    }
    return effect;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

void effDamageStarMain(EffectEntry* effect) {
    DamageStarWork* work = effect->userdata;
    Vec calcPos;
    Vec pos = { 0.0f, 0.0f, 0.0f };
    u32 n = 0;
    s32 i;
    s32 frame;
    f32 decay;

    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    calcPos = pos;
    effCalcMayaAnim(work->anim);
    if (gp->retraceCount > work->tick) {
        n = gp->retraceCount - work->tick;
    }
    if (n < 1) {
        n = 1;
    }
    decay = 0.75f;
    while (n-- != 0) {
        work = effect->userdata;
        if (work->life < 100) {
            work->life--;
        }
        work->frame++;
        if (work->life < 0) {
            effDeleteMayaAnim(work->anim);
            effDelete(effect);
            return;
        }
        frame = work->frame;
        work += 2;
        for (i = 2; i < effect->count; i++, work++) {
            if (frame < 20) {
                work->scale.x = work->scale.y = work->scale.z = 0.01f * scale_data[frame];
            } else if (frame < 40) {
                work->rotation = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 720.0f, frame - 20, 19);
            } else if (frame < 50) {
                work->scale.x = intplGetValue(EASING_QUADRATIC_OUT, 1.0f, 1.5f, frame - 40, 9);
                work->scale.y = intplGetValue(EASING_QUADRATIC_OUT, 1.0f, 0.5f, frame - 40, 9);
            } else if (frame < 60) {
                work->scale.x = intplGetValue(EASING_QUADRATIC_IN, 1.5f, 0.1f, frame - 50, 9);
                work->scale.y = intplGetValue(EASING_QUADRATIC_IN, 0.5f, 1.0f, frame - 50, 9);
                work->position.y = intplGetValue(EASING_QUADRATIC_IN, work->position.y, 10.0f + work->position.y, frame - 50, 9);
            } else {
                work->scale.x = work->scale.y = work->scale.z = 0.0f;
            }
            work->velocity.x *= decay;
            work->velocity.y *= decay;
            work->velocity.z *= decay;
            work->position.x += work->velocity.x;
            work->position.y += work->velocity.y;
            work->position.z += work->velocity.z;
        }
    }
    ((DamageStarWork*)effect->userdata)->tick = gp->retraceCount;
    dispEntry(CAMERA_3D, dispCalcZ(&calcPos), 8, (DispCallback)effDamageStarDisp, effect);
}

// One textured square of half-size h, u = horizontal texture repeat.
static inline void damageStarQuad(f32 h, f32 u) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-h, h, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(h, h, 0.0f);
    GXTexCoord2f32(u, 0.0f);
    GXPosition3f32(h, -h, 0.0f);
    GXTexCoord2f32(u, 1.0f);
    GXPosition3f32(-h, -h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
}

void effDamageStarDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* cam;
    GXColor color;
    DamageStarWork* work;
    s32 flip;
    DamageStarWork* part;
    GXTexObj texObj;
    Mtx baseMtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx transMtx;
    Mtx partMtx;
    Mtx partRot;
    Mtx partFinal;
    Mtx numMtx;
    Mtx finalMtx;
    Mtx invMtx;
    Mtx numRot;
    Mtx numTrans;
    Mtx numScale;
    char buf[0x20];
    s32 i;
    s32 frame;
    s32 number;
    s32 type;
    s32 len;
    f32 off;

    cam = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    number = work->number;
    flip = work->angle > 0.0f ? TRUE : FALSE;
    type = work->type;
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXTrans(transMtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scaleMtx, 0.8f, 0.8f, 0.8f);
    PSMTXConcat(transMtx, rotMtx, baseMtx);
    PSMTXConcat(baseMtx, scaleMtx, baseMtx);
    effGetTexObj(0, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    effGetTexObj(1, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    effGetTexObj(2, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP2);
    effGetTexObj(3, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP3);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    part = (DamageStarWork*)effect->userdata + 2;
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    if (type == 1) {
        effGetTexObj(21, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP2);
    }
    for (i = 2; i < effect->count; i++, part++) {
        color = (GXColor){ color_rotation_data[((i + frame) % 12) * 3],
                           color_rotation_data[((i + frame) % 12) * 3 + 1],
                           color_rotation_data[((i + frame) % 12) * 3 + 2],
                           part->alpha };
        GXSetChanMatColor(GX_COLOR0A0, color);
        PSMTXIdentity(partMtx);
        PSMTXScaleApply(partMtx, partMtx, part->scale.x, part->scale.y, part->scale.z);
        PSMTXTransApply(partMtx, partMtx, part->position.x, part->position.y, part->position.z);
        PSMTXRotRad(partRot, 'y', 0.017453292f * part->rotation);
        PSMTXConcat(partMtx, partRot, partMtx);
        PSMTXScale(partFinal, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(partMtx, partFinal, partMtx);
        PSMTXConcat(baseMtx, partMtx, partFinal);
        PSMTXConcat(camGetPtr(cameraId)->view, partFinal, partFinal);
        GXLoadPosMtxImm(partFinal, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetCullMode(GX_CULL_BACK);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        damageStarQuad(8.0f, 2.0f);
        GXSetCullMode(GX_CULL_FRONT);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, GX_TRUE, GX_TEVPREV);
        damageStarQuad(8.0f, 2.0f);
    }
    work = effect->userdata;
    PSMTXRotRad(numRot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXTrans(numTrans, work->position.x, work->position.y, work->position.z);
    PSMTXConcat(numTrans, numRot, numMtx);
    if (!flip) {
        PSMTXScale(numScale, 12.0f, 12.0f, 12.0f);
        PSMTXConcat(numMtx, numScale, numMtx);
    } else {
        PSMTXScale(numScale, -12.0f, 12.0f, 12.0f);
        PSMTXConcat(numMtx, numScale, numMtx);
    }
    PSMTXTrans(numTrans, 10.0f, -1.5f, 0.0f);
    PSMTXConcat(numMtx, numTrans, numMtx);
    PSMTXConcat(numMtx, work->anim->joints[0].mtx, numMtx);
    PSMTXConcat(numMtx, work->anim->joints[1].mtx, numMtx);
    GXSetCurrentMtx(GX_PNMTX0);
    PSMTXTransApply(numMtx, finalMtx, 2.5f, -2.5f, 0.0f);
    PSMTXConcat(cam->view, finalMtx, finalMtx);
    GXLoadPosMtxImm(finalMtx, GX_PNMTX0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetCullMode(GX_CULL_NONE);
    if (type == 1) {
        effGetTexObj(39, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP3);
        damageStarQuad(2.0f, 2.0f);
    } else {
        damageStarQuad(2.0f, 2.0f);
    }
    PSMTXConcat(cam->view, numMtx, finalMtx);
    GXLoadPosMtxImm(finalMtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0xFF, 0x00, 0xFF });
    if (type == 1) {
        effGetTexObj(19, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP2);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        damageStarQuad(2.0f, 1.0f);
    } else {
        damageStarQuad(2.0f, 2.0f);
    }
    mtxGetRotationElement(numMtx, invMtx, 'y', 'x');
    PSMTXInverse(invMtx, invMtx);
    PSMTXConcat(numMtx, invMtx, numMtx);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    sprintf(buf, "%d", number);
    len = strlen(buf);
    PSMTXScale(numScale, 0.04f, 0.04f, 0.04f);
    off = 24.0f * (f32)(-(len - 1));
    off *= 0.5f;
    PSMTXTrans(numTrans, off, 0.0f, 0.0f);
    PSMTXConcat(numScale, numTrans, numTrans);
    PSMTXConcat(numMtx, numTrans, finalMtx);
    effNumberGX(finalMtx, 2, number, 0xFF, 1);
}
