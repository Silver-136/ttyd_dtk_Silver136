#include "driver/camdrv.h"
#include "eff/eff_recovery.h"
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
#include "pmario_sound.h"
#include "eff/eff_number.h"

double sin(double);
double cos(double);
EffectEntry* effStardustEntry(s32, f32, f32, f32, f32, f32, s32, s32);
u32 strlen(const char* s);

typedef struct MayaPoly {
    s32 keyCount;          // 0x0
    MayaHermitKey* keys;   // 0x4
    s32 count;
    s16** strips;
} MayaPoly;

typedef struct MayaKeyList {
    s32 count;
    MayaPoly* polys;
} MayaKeyList;

typedef struct MayaAnimData {
    u8 pad0[4];
    f32 endTime;
    MayaKeyList* lists;
} MayaAnimData;

typedef struct MayaJoint {
    Mtx mtx;
    f32 alpha;
} MayaJoint;

typedef struct MayaAnim {
    u16 flags;
    f32 time;
    OSTime startTime;
    MayaAnimData* data;
    MayaJoint joints[1];
} MayaAnim;

MayaAnim* effMayaAnimAlloc(MayaAnimData* data);
void effDeleteMayaAnim(MayaAnim* anim);
void effPlayMayaAnim(MayaAnim* anim);
void effCalcMayaAnim(MayaAnim* anim);

#define QUAD(l, r, t, b, u) \
    GXBegin(GX_QUADS, GX_VTXFMT0, 4); \
    GXPosition3f32((l), (t), 0.0f); GXTexCoord2f32(0.0f, 0.0f); \
    GXPosition3f32((r), (t), 0.0f); GXTexCoord2f32((u), 0.0f); \
    GXPosition3f32((r), (b), 0.0f); GXTexCoord2f32((u), 1.0f); \
    GXPosition3f32((l), (b), 0.0f); GXTexCoord2f32(0.0f, 1.0f);

MayaHermitKey recovery_effHermit_0[] = {
    {
        0, // time
        {
            {0, 0, 0, 0, 0}, {320, 0, -35, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 7, 0, 0}, {0, 0, 7, 0, 0},
            {0, 0, 7, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        9, // time
        {
            {0, 0, 0, 0, 0}, {0, -22, -22, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 5, 5, 0, 0}, {64, 2, 2, 0, 0},
            {64, 4, 4, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        14, // time
        {
            {0, 0, 0, 0, 0}, {0, 4, 4, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {83, -1, -1, 0, 0}, {32, 1, 1, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        19, // time
        {
            {0, 0, 0, 0, 0}, {44, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {51, -1, -1, 0, 0}, {76, 3, 3, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        24, // time
        {
            {0, 0, 0, 0, 0}, {0, -3, -2, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        34, // time
        {
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {-11, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        49, // time
        {
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {11, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, 0, 0, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        54, // time
        {
            {0, 0, 0, 0, 0}, {0, -1, -1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {64, 0, 0, 0, 0}, {64, -1, -1, 0, 0},
            {64, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        69, // time
        {
            {0, 0, 0, 0, 0}, {-32, 20, 20, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {76, -2, -2, 0, 0}, {38, 1, 1, 0, 0},
            {64, -2, -2, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
    {
        79, // time
        {
            {0, 0, 0, 0, 0}, {512, 54, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, -7, 0, 0, 0}, {96, 5, 0, 0, 0},
            {0, -6, 0, 0, 0}, {0, 0, 0, 0, 0}, {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {-95, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
        },
    },
};
static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};
s16 recovery_effFace_pPlane2_q007_0[] = {
    4, // vertex count
    -768, -768, 0, 0, 512,
    768, -768, 0, 512, 512,
    768, 768, 0, 512, 0,
    -768, 768, 0, 0, 0,
};
s16* recovery_effFaceDt_pPlane2_q007 = recovery_effFace_pPlane2_q007_0;
MayaPoly recovery_effAnimRST[] = {
    {10, recovery_effHermit_0, 1, &recovery_effFaceDt_pPlane2_q007},
};
MayaKeyList recovery_effAnimRSTDt = {1, recovery_effAnimRST};
MayaAnimData recovery_effAnimDt = { { 0 }, 79.0f, &recovery_effAnimRSTDt };


void effRecoveryMain(EffectEntry* effect);
void effRecoveryMain2(EffectEntry* effect);

void effRecoveryDisp(CameraId cameraId, EffectEntry* effect);
void effRecoveryDisp2(CameraId cameraId, EffectEntry* effect);
void effRecoveryDisp3(CameraId cameraId, EffectEntry* effect);
void effRecoveryDisp4(CameraId cameraId, EffectEntry* effect);

EffectEntry* effRecoveryEntry(s32 type, s32 amount, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    RecoveryWork* work;
    s32 delay;
    RecoveryWork* pp;
    s32 size;
    f32 zero;
    s32 i;

    effect = effEntry();
    switch (type) {
    case 2:
    case 3:
        amount = 0;
        break;
    case 4:
    case 5:
        amount = 20;
        break;
    case 6:
        amount = 11;
        break;
    }
    effect->type = "Recovery";
    effect->count = amount + 1;
    size = effect->count * sizeof(RecoveryWork);
    work = __memAlloc(HEAP_EFFECT, size);
    effect->userdata = work;
    switch (type) {
    case 6:
        effect->callback = effRecoveryMain2;
        break;
    default:
        effect->callback = effRecoveryMain;
        break;
    }
    work->anim = effMayaAnimAlloc(&recovery_effAnimDt);
    effPlayMayaAnim(work->anim);
    work->type = type;
    work->amount = amount;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->timer = 0;
    work->tick = gp->retraceCount;
    work->alpha = 255;
    work->baseY = -30.0f;
    {
    Vec sfxPos;
    Vec pos = { 0.0f, 0.0f, 0.0f };
    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    sfxPos = pos;
    switch (work->type) {
    case 0:
    case 7:
        if (work->amount <= 5) {
            psndSFXOn_3D("SFX_HP_RECOVER_SMALL1", &sfxPos);
        } else if (work->amount <= 10) {
            psndSFXOn_3D("SFX_HP_RECOVER_MID1", &sfxPos);
        } else {
            psndSFXOn_3D("SFX_HP_RECOVER_BIG1", &sfxPos);
        }
        break;
    case 1:
    case 8:
        if (work->amount <= 5) {
            psndSFXOn_3D("SFX_FP_RECOVER_SMALL1", &sfxPos);
        } else if (work->amount <= 10) {
            psndSFXOn_3D("SFX_FP_RECOVER_MID1", &sfxPos);
        } else {
            psndSFXOn_3D("SFX_FP_RECOVER_BIG1", &sfxPos);
        }
        break;
    case 4:
        psndSFXOn_3D("SFX_HP_RECOVER_BIG1", &sfxPos);
        break;
    case 5:
        psndSFXOn_3D("SFX_FP_RECOVER_BIG1", &sfxPos);
        break;
    }
    }
    switch (type) {
    case 6:
        effDeleteMayaAnim(work->anim);
        work->timer = 180;
        break;
    }
    zero = 0.0f;
    pp = work + 1;
    for (i = 1, delay = 8; i < effect->count; delay += 8, i++, pp++) {
        work = pp;
        work->position.x = zero;
        switch (type) {
        case 4:
        case 5:
            work->position.y = (i < 10) ? zero : -15.0f;
            break;
        default:
            work->position.y = 0.0f;
            break;
        }
        work->position.z = zero;
        work->scale.x = zero;
        work->scale.y = zero;
        work->scale.z = zero;
        work->velocity.x = zero;
        work->velocity.y = zero;
        work->velocity.z = zero;
        work->timer = 0;
        work->state = 0;
        work->alpha = 255;
        work->angle = zero;
        switch (type) {
        case 6:
            if (i == 1) {
                work->scale.x = work->scale.y = work->scale.z = 1.5f;
                work->alpha = 0;
                work->delay = 0;
            } else {
                work->scale.x = work->scale.y = work->scale.z = 0.3f;
                work->angle = (f32)(((i & 1) ? irand(80) : -irand(80)) + 90);
                work->velocity.x = 1.8f * (f32)cos(6.2831855f * work->angle / 360.0f);
                work->velocity.y = 1.8f * (f32)sin(6.2831855f * work->angle / 360.0f);
                work->alpha = 255;
                work->delay = delay;
            }
            break;
        }
    }
    return effect;
}

void effRecoveryMain(EffectEntry* effect) {
    RecoveryWork* work = effect->userdata;
    RecoveryWork* p;
    s32 i;
    s32 go = 0;
    s32 amount;
    s32 done = 0;
    s32 type = work->type;
    u32 n = 0;
    f32 x, y, z;
    f32 baseY = work->baseY;
    f32 a, d;

    work = effect->userdata;
    type = work->type;
    effCalcMayaAnim(work->anim);
    if (gp->retraceCount > work->tick) {
        n = gp->retraceCount - work->tick;
    }
    if (n < 1) {
        n = 1;
    }
    while (n-- != 0) {
        work = p = effect->userdata;
        amount = work->amount;
        x = work->position.x;
        y = work->position.y;
        z = work->position.z;
        if (type == 4 || type == 5) {
            amount = 10;
        }
        work->timer++;
        if (work->timer > 10) {
            go = 1;
        }
        p++;
        for (i = 1; i < effect->count; i++, p++) {
            switch (p->state) {
            case 0:
                if (go) {
                    a = 6.2831855f * (90.0f + 360.0f * (f32)(i - 1) / (f32)amount) / 360.0f;
                    p->velocity.x = 12.0f * (f32)sin(a);
                    p->velocity.y = 0.0f;
                    a = cos(a);
                    p->velocity.z = 12.0f * -a;
                    p->timer = 0;
                    p->state++;
                }
                break;
            case 1:
                if (p->timer < 10) {
                    p->velocity.x *= 0.7f;
                    p->velocity.y *= 0.7f;
                    p->velocity.z *= 0.7f;
                    p->position.x += p->velocity.x;
                    p->position.y += p->velocity.y;
                    p->position.z += p->velocity.z;
                    p->scale.x = p->scale.y = p->scale.z = 1.0f;
                    p->timer++;
                } else {
                    p->timer = 10;
                    p->state++;
                }
                break;
            case 2:
                if (--p->timer <= 0) {
                    p->timer = 0;
                    p->state++;
                }
                break;
            case 3:
                if (p->timer < 40) {
                    a = intplGetValue(11, 0.0f, 360.0f, ++p->timer, 40);
                    d = distABf(0.0f, 0.0f, p->position.x, p->position.z);
                    d *= 0.99f;
                    a = 6.2831855f * (90.0f + (a + 360.0f * (f32)(i - 2) / (f32)amount)) / 360.0f;
                    p->position.x = d * (f32)sin(a);
                    p->position.z = d * -(f32)cos(a);
                } else {
                    p->timer = rand() % 20;
                    p->state++;
                }
                break;
            case 4:
                if (--p->timer <= 0) {
                    p->timer = 0;
                    p->state++;
                }
                break;
            case 5:
                if (p->timer < 16) {
                    p->timer++;
                    p->position.x = intplGetValue(1, p->position.x, 0.0f, p->timer, 16);
                    p->position.y = intplGetValue(1, p->position.y, baseY, p->timer, 16);
                    p->position.z = intplGetValue(1, p->position.z, 0.0f, p->timer, 20);
                    p->scale.x = p->scale.y = p->scale.z = intplGetValue(1, 1.0f, 0.0f, p->timer, 16);
                } else {
                    p->state++;
                }
                break;
            case 6:
                done++;
                break;
            }
        }
        if (type <= 1u || type == 7 || type == 8 || type == 4 || type == 5) {
            if (done >= amount) {
                work = effect->userdata;
                if (type != 7 && type != 8) {
                    Vec sfxPos;
                    effStardustEntry(2, x, y - 20.0f, z, 30.0f, 30.0f, 2, 30);
                    {
                    Vec pos = { 0.0f, 0.0f, 0.0f };
                    pos.x = work->position.x;
                    pos.y = work->position.y;
                    pos.z = work->position.z;
                    sfxPos = pos;
                    switch (work->type) {
                    case 0:
                        if (work->amount <= 5) {
                            psndSFXOn_3D("SFX_HP_RECOVER_SHINE1", &sfxPos);
                        } else if (work->amount <= 10) {
                            psndSFXOn_3D("SFX_HP_RECOVER_SHINE2", &sfxPos);
                        } else {
                            psndSFXOn_3D("SFX_HP_RECOVER_SHINE3", &sfxPos);
                        }
                        break;
                    case 1:
                        if (work->amount <= 5) {
                            psndSFXOn_3D("SFX_FP_RECOVER_SHINE1", &sfxPos);
                        } else if (work->amount <= 10) {
                            psndSFXOn_3D("SFX_FP_RECOVER_SHINE2", &sfxPos);
                        } else {
                            psndSFXOn_3D("SFX_FP_RECOVER_SHINE3", &sfxPos);
                        }
                        break;
                    case 4:
                        psndSFXOn_3D("SFX_HP_RECOVER_SHINE3", &sfxPos);
                        break;
                    case 5:
                        psndSFXOn_3D("SFX_FP_RECOVER_SHINE3", &sfxPos);
                        break;
                    }
                    }
                }
                effDeleteMayaAnim(work->anim);
                effDelete(effect);
                return;
            }
        }
    }
    work = effect->userdata;
    work->tick = gp->retraceCount;
    {
    Vec calc = { 0.0f, 0.0f, 0.0f };
    calc.x = work->position.x;
    calc.y = work->position.y;
    calc.z = work->position.z;
    dispEntry(CAMERA_3D, dispCalcZ(&calc), 1, (DispCallback)effRecoveryDisp, effect);
    }
    if (type <= 1u || type == 7 || type == 8) {
        Vec calc = { 0.0f, 0.0f, 0.0f };
        calc.x = work->position.x;
        calc.y = work->position.y;
        calc.z = work->position.z;
        dispEntry(CAMERA_3D, dispCalcZ(&calc), 2, (DispCallback)effRecoveryDisp2, effect);
    }
    if (type == 4 || type == 5) {
        Vec calc = { 0.0f, 0.0f, 0.0f };
        calc.x = work->position.x;
        calc.y = work->position.y;
        calc.z = work->position.z;
        dispEntry(CAMERA_3D, dispCalcZ(&calc), 2, (DispCallback)effRecoveryDisp3, effect);
    }
}

void effRecoveryMain2(EffectEntry* effect) {
    RecoveryWork* work = effect->userdata;
    Vec calc;
    Vec pos = { 0.0f, 0.0f, 0.0f };
    s32 i;

    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    calc = pos;
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 32) {
        work->alpha = work->timer << 3;
    }
    if (work->timer < 0) {
        effStardustEntry(2, work->position.x, work->position.y, work->position.z, 40.0f, 45.0f, 6, 30);
        effDelete(effect);
        return;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            work->delay--;
            continue;
        }
        work->position.x += work->velocity.x;
        work->position.y += work->velocity.y;
        work->position.z += work->velocity.z;
        work->velocity.x *= 0.95f;
        work->velocity.y *= 0.95f;
        work->velocity.z *= 0.95f;
        if (i == 1) {
            work->alpha += 5;
            if (work->alpha > 128) {
                work->alpha = 128;
            }
        } else {
            work->alpha -= 8;
            if (work->alpha < 0) {
                work->position.x = 0.0f;
                work->position.y = 0.0f;
                work->position.z = 0.0f;
                work->velocity.x = 2.0f * (f32)cos(6.2831855f * work->angle / 360.0f);
                work->velocity.y = 2.0f * (f32)sin(6.2831855f * work->angle / 360.0f);
                work->alpha = 255;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&calc), 2, (DispCallback)effRecoveryDisp4, effect);
}

void effRecoveryDisp(CameraId cameraId, EffectEntry* effect) {
    RecoveryWork* work = effect->userdata;
    RecoveryWork* p;
    CameraEntry* cam;
    s32 type;
    s32 i;
    u8* c;
    GXColor color;
    GXTexObj texObj;
    Mtx transMtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx base;

    cam = camGetPtr(cameraId);
    type = work->type;
    PSMTXTrans(transMtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(transMtx, rotMtx, base);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    switch (type) {
    case 0:
    case 2:
    case 4:
    case 7:
        effGetTexObj(18, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObj(20, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        break;
    case 1:
    case 3:
    case 5:
    case 8:
        effGetTexObj(19, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        effGetTexObj(21, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP1);
        break;
    }
    p = effect->userdata;
    PSMTXTrans(transMtx, p->position.x, p->position.y, p->position.z);
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXScale(scaleMtx, 10.0f, 10.0f, 10.0f);
    PSMTXConcat(transMtx, scaleMtx, transMtx);
    PSMTXTrans(scaleMtx, 0.0f, 2.0f, 0.0f);
    PSMTXConcat(transMtx, scaleMtx, transMtx);
    if (type <= 1u || (u32)(type - 7) <= 1u || type == 4 || type == 5) {
        PSMTXConcat(transMtx, p->anim->joints[0].mtx, transMtx);
    }
    PSMTXConcat(cam->view, transMtx, transMtx);
    PSMTXScale(scaleMtx, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(transMtx, scaleMtx, transMtx);
    GXLoadPosMtxImm(transMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    QUAD(-16.0f, 16.0f, 16.0f, -16.0f, 1.0f)
    if (type == 2 || type == 3) {
        return;
    }
    {
        p++;
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        for (i = 1; i < effect->count; i++, p++) {
            c = &color_rotation_data[((i + gp->retraceCount) % 12) * 3];
            color = (GXColor){ c[0], c[1], c[2], p->alpha };
            GXSetTevColor(GX_TEVREG0, color);
            PSMTXTrans(transMtx, p->position.x, p->position.y, p->position.z);
            PSMTXScale(scaleMtx, p->scale.x, p->scale.y, p->scale.z);
            PSMTXConcat(transMtx, scaleMtx, transMtx);
            PSMTXConcat(base, transMtx, transMtx);
            PSMTXConcat(camGetPtr(cameraId)->view, transMtx, transMtx);
            GXLoadPosMtxImm(transMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            QUAD(-6.0f, 6.0f, 6.0f, -6.0f, 2.0f)
        }
    }
}

void effRecoveryDisp2(CameraId cameraId, EffectEntry* effect) {
    RecoveryWork* work = effect->userdata;
    CameraEntry* cam;
    s32 type;
    s32 amount;
    s32 len;
    char buf[0x20];
    Mtx mtx;
    Mtx rot;
    Mtx tmp;
    f32 off;

    cam = camGetPtr(cameraId);
    amount = work->amount;
    type = work->type;
    sprintf(buf, "%d", amount);
    len = strlen(buf);
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXTrans(tmp, 0.0f, 0.0f, 1.0f);
    PSMTXConcat(mtx, tmp, mtx);
    PSMTXScale(tmp, 10.0f, 10.0f, 10.0f);
    PSMTXConcat(mtx, tmp, mtx);
    PSMTXTrans(tmp, 0.0f, 2.0f, 0.0f);
    PSMTXConcat(mtx, tmp, mtx);
    PSMTXConcat(mtx, work->anim->joints[0].mtx, mtx);
    PSMTXScale(tmp, 0.035f, 0.035f, 0.035f);
    PSMTXConcat(mtx, tmp, mtx);
    off = 24.0f * (f32)(-(len - 1));
    off *= 0.5f;
    PSMTXTrans(tmp, off, 0.0f, 0.0f);
    PSMTXConcat(mtx, tmp, mtx);
    switch (type) {
    case 0:
    case 2:
    case 7:
        effNumberGX(mtx, 0, amount, 0xFF, 1);
        break;
    case 1:
    case 3:
    case 8:
        effNumberGX(mtx, 1, amount, 0xFF, 1);
        break;
    }
}

void effRecoveryDisp3(CameraId cameraId, EffectEntry* effect) {
    RecoveryWork* work = effect->userdata;
    CameraEntry* cam;
    s32 type;
    s32 i;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx tmp;
    Mtx base;
    f32 w;
    f32 left;
    f32 right;
    f32 top;
    f32 bot;

    cam = camGetPtr(cameraId);
    type = work->type;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXTrans(tmp, 0.0f, 0.0f, 1.0f);
    PSMTXConcat(mtx, tmp, mtx);
    PSMTXScale(tmp, 10.0f, 10.0f, 10.0f);
    PSMTXConcat(mtx, tmp, mtx);
    PSMTXTrans(tmp, 0.0f, 2.0f, 0.0f);
    PSMTXConcat(mtx, tmp, mtx);
    PSMTXConcat(mtx, work->anim->joints[0].mtx, base);
    PSMTXConcat(cam->view, base, base);
    PSMTXScale(tmp, 0.05f, 0.05f, 0.05f);
    PSMTXConcat(base, tmp, base);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    bot = -20.0f;
    top = 20.0f;
    w = 108.0f;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
#ifdef VERSION_G8ME01
            if ((u32)gp->language == 4) {
                effGetTexObj(173, &texObj);
            } else {
                effGetTexObj(111, &texObj);
            }
#else
            effGetTexObj(0x8B, &texObj);
#endif
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x00, 0x00, 0x00, 0xA0 });
        } else {
#ifdef VERSION_G8ME01
            if ((u32)gp->language == 4) {
                effGetTexObj(172, &texObj);
            } else {
                effGetTexObj(110, &texObj);
            }
#else
            effGetTexObj(0x8A, &texObj);
#endif
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            if (type == 4) {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xF5, 0x98, 0x9D, 0xFF });
            } else {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0x5D, 0xC1, 0x00, 0xFF });
            }
        }
        PSMTXTrans(mtx, (f32)((1 - i) * 2), (f32)(-(1 - i) * 2), 0.0f);
        PSMTXConcat(base, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        left = -w;
        left *= 0.5f;
        right = left + w;
        QUAD(left, right, top, bot, 1.0f)
    }
}

void effRecoveryDisp4(CameraId cameraId, EffectEntry* effect) {
    RecoveryWork* work = effect->userdata;
    CameraEntry* cam;
    s32 i;
    s32 alpha;
    GXColor color;
    Mtx mtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx base;
    GXTexObj texObj;
    u8* c;
    f32 w;
    f32 nw;
    f32 l;
    f32 r;
    f32 v;
    f32 u;
    f32 off;

    cam = camGetPtr(cameraId);
    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rotMtx, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(mtx, rotMtx, base);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    w = 32.0f;
    nw = -w;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay == 0) {
            if (i == 1) {
                effGetTexObj(18, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, alpha * work->alpha / 255 });
            } else {
                c = &color_rotation_data[((i + gp->retraceCount) % 12) * 3];
                color = (GXColor){ c[0], c[1], c[2], alpha * work->alpha / 255 };
                effGetTexObj(20, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetChanMatColor(GX_COLOR0A0, color);
            }
            PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
            PSMTXScale(scaleMtx, work->scale.x, work->scale.y, work->scale.z);
            if (i != 1) {
                PSMTXRotRad(rotMtx, 'z', 0.017453292f * (work->angle - 90.0f));
            } else {
                PSMTXRotRad(rotMtx, 'z', 0.0f);
            }
            PSMTXConcat(base, mtx, mtx);
            PSMTXConcat(mtx, scaleMtx, mtx);
            PSMTXConcat(mtx, rotMtx, mtx);
            PSMTXConcat(cam->view, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            u = 1.0f;
            off = 0.0f;
            if (i == 1) {
                off = 8.0f;
            }
            if (i != 1) {
                u = 2.0f;
            }
            l = nw / 2.0f;
            v = l + off;
            r = 16.0f;
            w = 1.0f;
            GXPosition3f32(l, r + off, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(r, r + off, 0.0f);
            GXTexCoord2f32(u, 0.0f);
            GXPosition3f32(r, v, 0.0f);
            GXTexCoord2f32(u, w);
            GXPosition3f32(l, v, 0.0f);
            GXTexCoord2f32(0.0f, w);
        }
    }
}
