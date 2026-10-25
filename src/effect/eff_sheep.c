#include "eff/eff_kemu_test.h"
#include "battle/battle.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/mtx.h>

typedef struct EffSheepWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10
    s32 field_0x14; // 0x14
    f32 field_0x18; // 0x18
    f32 field_0x1C; // 0x1C
    s32 poseId;     // 0x20
    s32 state;      // 0x24
    s32 timer;      // 0x28
    f32 speed;      // 0x2C
    s32 sfxId;      // 0x30
} EffSheepWork;

f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z);

void effSheepDisp(CameraId cameraId, EffectEntry* effect);
void effSheepMain(EffectEntry* effect);

EffectEntry* effSheepEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffSheepWork* work;
    EffSheepWork* p;
    s32 i;

    effect = effEntry();
    if (gp->inBattle != 0) {
        y = BattleGetFloorHeight(_battleWorkPointer, x, y, z);
    }
    effect->type = "sheep";
    effect->count = 0x33;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(EffSheepWork));
    effect->userdata = work;
    effect->callback = effSheepMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->state = 0;
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->type = irand(100) < 20;
        p->position.x = -500.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->poseId = -1;
        p->state = 0;
        p->scale = (irand(5) + 8) / 10.0f;
        p->speed = (irand(20) + 20) / 10.0f;
        p->timer = i + irand(30);
        p->position.z = 100 - irand(200);
        if (p->type != 0) {
            p->field_0x14 = (s32)((irand(50) + 50) / p->speed);
            p->field_0x1C = 0.2f;
            p->field_0x18 = p->field_0x1C * p->field_0x14 / 2.0f;
        }
        if (effect->count - 1 == i) {
            p->position.z = 0.0f;
            p->type = 0;
            p->scale = 1.0f;
            p->timer = effect->count + 200;
        }
    }
    return effect;
}

void effSheepMain(EffectEntry* effect) {
    EffSheepWork* work = effect->userdata;
    const char* sfx1;
    const char* sfx2;
    EffSheepWork* p;
    s32 i;
    s32 done = 0;
    s32 inBattle;
    Vec position;
    Vec tmp = { 0.0f, 0.0f, 0.0f };

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    inBattle = gp->inBattle != 0;

    if (effect->flags & 4) {
        effect->flags &= ~4;
        {
            EffSheepWork* q;
            s32 k;

            q = effect->userdata;
            q++;
            for (k = 1; k < effect->count; k++, q++) {
                if (q->poseId != -1) {
                    animPoseRelease(q->poseId);
                }
            }
        }
        psndSFXOff(((EffSheepWork*)effect->userdata)[1].sfxId);
        psndSFXOff(((EffSheepWork*)effect->userdata)[effect->count - 1].sfxId);
        effDelete(effect);
        return;
    }
    sfx1 = "SFX_ITEM_SLEEP1";
    sfx2 = "SFX_ITEM_SLEEP2";
    p = work + 1;
    for (i = 1; i < effect->count; i++, p++) {
        switch (p->state) {
            case 0:
                if (animGroupBaseAsync("EFF_hituji", inBattle, NULL)) {
                    p->poseId = animPoseEntry("EFF_hituji", inBattle);
                    animPoseSetAnim(p->poseId, "A_1", TRUE);
                    p->state++;
                }
                break;
            case 1:
                if (--p->timer < 0) {
                    p->timer = 0;
                    if (i == 1) {
                        Vec v = { 0.0f, 0.0f, 0.0f };
                        v.x = position.x + p->position.x;
                        v.y = position.y + p->position.y;
                        v.z = position.z + p->position.z;
                        p->sfxId = psndSFXOn_3D(sfx1, &v);
                    } else if (i == effect->count - 1) {
                        Vec v = { 0.0f, 0.0f, 0.0f };
                        v.x = position.x + p->position.x;
                        v.y = position.y + p->position.y;
                        v.z = position.z + p->position.z;
                        p->sfxId = psndSFXOn_3D(sfx2, &v);
                    }
                    p->state++;
                }
                break;
            case 2:
                if (i == 1 || i == effect->count - 1) {
                    Vec v = { 0.0f, 0.0f, 0.0f };
                    v.x = position.x + p->position.x;
                    v.y = position.y + p->position.y;
                    v.z = position.z + p->position.z;
                    psndSFX_pos(p->sfxId, &v);
                }
                if (p->position.x < 500.0f) {
                    p->position.x += p->speed;
                    if (p->type == 1) {
                        p->position.y += p->field_0x18;
                        p->field_0x18 -= p->field_0x1C;
                        if (p->position.y < 0.0f) {
                            p->position.y = 0.0f;
                            p->field_0x18 *= -1.0f;
                        }
                    }
                } else {
                    p->state++;
                }
                if ((gp->retraceCount + i) % 10 == 0) {
                    effKemuTestEntry(0, position.x + p->position.x, 10.0f + (position.y + p->position.y),
                                     position.z + p->position.z, 0.3f + irand(30) / 100.0f);
                }
                break;
            case 3:
                if (i == effect->count - 2) {
                    psndSFXOff(((EffSheepWork*)effect->userdata)[1].sfxId);
                }
                if (i == effect->count - 1) {
                    psndSFXOff(p->sfxId);
                }
                animPoseRelease(p->poseId);
                p->poseId = -1;
                p->state++;
                break;
            case 4:
                done++;
                break;
        }
        if (p->poseId != -1) {
            animPoseMain(p->poseId);
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 1, (DispCallback)effSheepDisp, effect);
    }
}

void effSheepDisp(CameraId cameraId, EffectEntry* effect) {
    EffSheepWork* work = effect->userdata;
    Mtx trans;
    Mtx scale;
    Mtx base;
    s32 i;

    PSMTXTrans(base, work->position.x, work->position.y, work->position.z);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->poseId != -1) {
            PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
            PSMTXScale(scale, work->scale, work->scale, work->scale);
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(base, trans, trans);
            animPoseDrawMtx(work->poseId, trans, 1, 0.0f, 10.0f);
            animPoseDrawMtx(work->poseId, trans, 2, 0.0f, 10.0f);
            animPoseDrawMtx(work->poseId, trans, 3, 0.0f, 10.0f);
        }
    }
}
