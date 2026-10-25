#include "driver/animdrv.h"
#include "eff/eff_mahojin.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

void effMahojinMain(EffectEntry* effect);
void effMahojinDisp(CameraId cameraId, EffectEntry* effect);


s32 mahojin_rate;

static const char* name_tbl[] = {
    "A_2_1", "S_2_1", "S_3_1", "A_2_2", "S_2_2", "S_3_2", "A_2_3", "S_2_3", "S_3_3", "A_2_4", "S_2_4",
    "S_3_4", "A_2_5", "S_2_5", "S_3_5", "A_2_6", "S_2_6", "S_3_6", "A_2_7", "S_2_7", "S_3_7",
};

Vec pos_tbl[7] = {
    { 0.0f, 0.0f, -14.321f },
    { 10.7071f, 0.0f, -9.1648f },
    { 13.843f, 0.0f, 2.7905f },
    { 6.3653f, 0.0f, 12.7509f },
    { -6.0852f, 0.0f, 12.9425f },
    { -13.8747f, 0.0f, 3.0951f },
    { -11.1931f, 0.0f, -9.0435f },
};



static void callback(MapTevCallbackInfo* info) {
    s32 tevStage = info->tevStage;
    s32 texMap = info->texMap;
    s32 texCoord = info->texCoord;
    s32 indStage = info->indStage;

    GXSetTevColor(GX_TEVREG0, (GXColor){ 0xE8, 0x00, 0xE8, mahojin_rate });
    GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(tevStage, GX_CC_CPREV, GX_CC_C0, GX_CC_A0, GX_CC_ZERO);
    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    info->tevStage = tevStage + 1;
    info->texCoord = texCoord;
    info->indStage = indStage;
    info->texMap = texMap;
}

EffectEntry* effMahojinEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffMahojinWork* work;
    EffMahojinWork* p;
    s32 n;

    effect = effEntry();
    n = (type < 8 ? type : 7) + 2;
    effect->type = "Mahojin";
    effect->count = n;
    work = __memAlloc(HEAP_EFFECT, n * sizeof(EffMahojinWork));
    effect->userdata = work;
    effect->callback = effMahojinMain;
    effect->flags |= 2;

    p = work;
    p->type = type;
    p->position.x = x;
    p->position.y = y;
    p->position.z = z;
    p->timer = 1000;
    p->field_0x14 = 0;
    p->poseId[0] = -1;
    p->poseId[1] = -1;
    p->alpha = 255;
    p->state = 0;
    p->arg = NULL;
    p->func = NULL;

    p = work + 1;
    p->type = type;
    p->position.x = x;
    p->position.y = y;
    p->position.z = z;
    p->timer = 1000;
    p->field_0x14 = 0;
    p->poseId[0] = -1;
    p->poseId[1] = -1;
    p->alpha = 255;
    p->state = 0;
    p->arg = NULL;
    p->func = NULL;

    for (p = work + 2, n = 2; n < effect->count; n++, p++) {
        p->position.x = x;
        p->position.y = y;
        p->position.z = z;
        p->poseId[0] = -1;
        p->poseId[1] = -1;
        p->alpha = 0;
        p->state = 0;
        p->arg = NULL;
        p->func = NULL;
    }
    return effect;
}

void effMahojinMain(EffectEntry* effect) {
    EffMahojinWork* p;
    s32 i;
    EffMahojinWork* work = effect->userdata;
    s32 type;
    s32 inBattle;
    f32 loops;
    Vec position;
    Vec tmp = { 0.0f, 0.0f, 0.0f };

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    loops = 0.0f;
    inBattle = gp->inBattle != 0;
    type = work->type;

    if (work->poseId[0] == -1) {
        if (!animGroupBaseAsync("MOBJ_EFF_mahojin_omote", inBattle, NULL)) {
            return;
        }
        if (!animGroupBaseAsync("MOBJ_EFF_mahojin_ura", inBattle, NULL)) {
            return;
        }
        work->poseId[0] = animPoseEntry("MOBJ_EFF_mahojin_omote", inBattle);
        work->poseId[1] = animPoseEntry("MOBJ_EFF_mahojin_ura", inBattle);
        animPoseSetAnim(work->poseId[0], "A_1", TRUE);
        animPoseSetAnim(work->poseId[1], "A_1", TRUE);
        work[1].poseId[0] = animPoseEntry("MOBJ_EFF_mahojin_omote", inBattle);
        work[1].poseId[1] = animPoseEntry("MOBJ_EFF_mahojin_ura", inBattle);
        animPoseSetAnim(work[1].poseId[0], "A_4", TRUE);
        animPoseSetAnim(work[1].poseId[1], "A_4", TRUE);
        work = effect->userdata;
    }

    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 16 && work->alpha > work->timer * 16) {
        work->alpha = work->timer * 16;
    }
    if (work->timer < 0) {
        {
            EffMahojinWork* r = effect->userdata;
            s32 k;
            for (k = 0; k < effect->count; k++, r++) {
                if (r->poseId[0] != -1) {
                    animPoseRelease(r->poseId[0]);
                }
                if (r->poseId[1] != -1) {
                    animPoseRelease(r->poseId[1]);
                }
            }
        }
        effDelete(effect);
        return;
    }

    p = effect->userdata;
    switch (p->state) {
        case 0:
            loops = animPoseGetLoopTimes(p->poseId[0]);
            if (loops > 1.0f) {
                animPoseSetAnim(p->poseId[0], "S_1", TRUE);
                animPoseSetAnim(p->poseId[1], "S_1", TRUE);
                animPoseSetAnim(p[1].poseId[0], "S_4", TRUE);
                animPoseSetAnim(p[1].poseId[1], "S_4", TRUE);
                p->state++;
            }
            break;
        case 1:
            if (type == 7) {
                if (animPoseGetLoopTimes(p[effect->count - 1].poseId[0]) > 1.0f) {
                    mapSetTevCallback(10, callback);
                    {
                        EffMahojinWork* q = p;
                        s32 k;
                        for (k = 0; k < effect->count; q++, k++) {
                            animPoseSetMaterialFlagOn(q->poseId[0], 0xA000000);
                            animPoseSetMaterialFlagOn(q->poseId[1], 0xA000000);
                        }
                    }
                    mahojin_rate = 0;
                    p->state++;
                }
            }
            break;
        case 2:
            mahojin_rate += 2;
            if (mahojin_rate > 255) {
                mahojin_rate = 255;
                p->state++;
            }
            break;
    }

    p = effect->userdata;
    p += 2;
    for (i = 2; i < effect->count; i++, p++) {
        switch (p->state) {
            case 0:
                if (i - 1 < type) {
                    if (loops > 0.6f) {
                        p->poseId[0] = animPoseEntry("MOBJ_EFF_mahojin_omote", inBattle);
                        p->poseId[1] = animPoseEntry("MOBJ_EFF_mahojin_ura", inBattle);
                        if (type != 7 && type != 8) {
                            animPoseSetAnim(p->poseId[0], name_tbl[(i - 2) * 3 + 2], TRUE);
                            animPoseSetAnim(p->poseId[1], name_tbl[(i - 2) * 3 + 2], TRUE);
                        } else {
                            animPoseSetAnim(p->poseId[0], name_tbl[(i - 2) * 3 + 1], TRUE);
                            animPoseSetAnim(p->poseId[1], name_tbl[(i - 2) * 3 + 1], TRUE);
                        }
                        p->alpha = 0;
                        p->state = 1;
                    }
                } else if (loops > 0.8f) {
                    p->poseId[0] = animPoseEntry("MOBJ_EFF_mahojin_omote", inBattle);
                    p->poseId[1] = animPoseEntry("MOBJ_EFF_mahojin_ura", inBattle);
                    animPoseSetAnim(p->poseId[0], name_tbl[(i - 2) * 3], TRUE);
                    animPoseSetAnim(p->poseId[1], name_tbl[(i - 2) * 3], TRUE);
                    p->alpha = 255;
                    p->state = 10;
                }
                break;
            case 1:
                p->alpha += 10;
                if (p->alpha > 255) {
                    p->alpha = 255;
                    p->state = 2;
                }
                break;
            case 10:
                if (animPoseGetLoopTimes(p->poseId[0]) > 1.0f) {
                    animPoseSetAnim(p->poseId[0], name_tbl[(i - 2) * 3 + 1], TRUE);
                    animPoseSetAnim(p->poseId[1], name_tbl[(i - 2) * 3 + 1], TRUE);
                    p->state = 11;
                }
                break;
            case 2:
            case 11:
                break;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMahojinDisp, effect);
}

void effMahojinDisp(CameraId cameraId, EffectEntry* effect) {
    Mtx mtx;
    f32 z[16];
    s32 idx[16];
    f32* zp;
    MapWork* map;
    s32* idxList; // FAKE: alias of idx forces z/idx base register order
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    EffMahojinWork* work = effect->userdata;
    s32 pass;
    s32 alpha;
    u16 flags;

    camGetPtr(cameraId);
    alpha = work->alpha;
    map = mapGetWork();
    zp = z;
    flags = map->entries[0].flags;



    for (pass = 0; pass < 2; pass++) {
        n = 0;
        if (pass == 0) {
            zp[0] = -9999.0f;
            idx[n++] = 0;
            z[n] = -10000.0f;
            idx[n++] = 1;
        } else {
            zp[0] = -10000.0f;
            idx[n++] = 0;
            z[n] = 10000.0f;
            idx[n++] = 1;
        }
        for (i = 2; i < effect->count; i++) {
            f32 d = dispCalcZ(&pos_tbl[i - 2]);
            z[n] = d;
            idx[n++] = i;
        }
        idxList = idx; // FAKE
        for (i = 0; i < effect->count - 1; i++) {
            for (j = i + 1; j < effect->count; j++) {
                if (zp[i] > z[j]) {
                    s32 ti = idx[i];
                    f32 tz = zp[i];
                    zp[i] = z[j];
                    idx[i] = idx[j];
                    z[j] = tz;
                    idx[j] = ti;
                }
            }
        }
        map->entries[0].flags &= ~1;
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        for (k = 0; k < effect->count; k++) {
            EffMahojinWork* p = &work[idxList[k]];
            if (pass == 1 && p->arg != NULL) {
                p->func(cameraId, p->arg);
            }
            if (p->poseId[pass] != -1) {
                animPoseMain(p->poseId[pass]);
                animPoseSetMaterialFlagOn(p->poseId[pass], 0x40);
                animPoseSetMaterialEvtColor(p->poseId[pass], (GXColor){ 0xFF, 0xFF, 0xFF, p->alpha * alpha / 255 });
                animPoseDrawMtx(p->poseId[pass], mtx, 3, 0.0f, 10.0f);
            }
        }
        map->entries[0].flags = flags;
    }
}
