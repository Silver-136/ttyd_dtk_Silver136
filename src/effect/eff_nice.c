#include "eff/eff_nice.h"
#include "driver/animdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <string.h>
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/gx.h>


//.sdata
const char* anim_jp[] = {
    "EFF_nice",
    "EFF_good",
    "EFF_great",
    "EFF_wonderful",
    "EFF_excellent",
    NULL,
    "EFF_lucky"
};
const char* anim_us[] = {
    "EFF_nice_us",
    "EFF_good_us",
    "EFF_great_us",
    "EFF_wonderful_us",
    "EFF_excellent_us",
    NULL,
    "EFF_lucky_us"
};
#ifdef VERSION_G8ME01
const char* anim_lang3[] = {
    "EFF_nice_D",
    "EFF_good_D",
    "EFF_great_D",
    "EFF_wonderful_D",
    "EFF_excellent_D",
    NULL,
    "EFF_lucky_D"
};
const char* anim_lang4[] = {
    "EFF_nice_F",
    "EFF_good_F",
    "EFF_great_F",
    "EFF_wonderful_F",
    "EFF_excellent_us",
    NULL,
    "EFF_lucky_F"
};
const char* anim_lang5[] = {
    "EFF_nice_Sp",
    "EFF_good_Sp",
    "EFF_great_Sp",
    "EFF_wonderful_Sp",
    "EFF_excellent_Sp",
    NULL,
    "EFF_lucky_Sp"
};
const char* anim_lang6[] = {
    "EFF_nice_I",
    "EFF_good_I",
    "EFF_great_D",
    "EFF_wonderful_I",
    "EFF_excellent_I",
    NULL,
    "EFF_lucky_I"
};
const char** anim_tbl[6] = { anim_jp, anim_us, anim_lang3, anim_lang4, anim_lang5, anim_lang6 };
s16 acrobat_jp[] = {151, 152, 153, 154, 155, 156, 157};
s16 acrobat_us[] = {161, 162, 163, 164, 165, 161, 166, 167};
s16 acrobat_lang3[] = {157, 158, 158, 159, 160};
s16 acrobat_lang4[] = {163, 164, 165, 166, 167, 168};
s16 acrobat_lang5[] = {166, 167, 168, 166, 169, 166, 170, 171, 166};
s16 acrobat_lang6[] = {197, 198, 155, -1, 151, 152, 153, 154, 155, 156};
typedef struct AcrobatSet {
    s16* tex;
    s32 count;
} AcrobatSet;
AcrobatSet acrobat_tbl[6] = { { acrobat_jp, 7 }, { acrobat_us, 8 }, { acrobat_lang3, 5 }, { acrobat_lang4, 6 }, { acrobat_lang5, 9 }, { acrobat_lang6, 10 } };
#else
static const char** anim_tbl[2] = { anim_jp, anim_us };
s32 acrobat_jp[] = {179, 180, 181, 182, 183, 184, 185};
s32 acrobat_us[] = {183, 184, 185, 186, 187, 188}; //includes nonexistant textures
s32* acrobat_tbl[2] = { acrobat_jp, acrobat_us };
#endif
 // unreferenced zero word in target

//forward declarations
static void rendermodeFunc(s32 wXluStage);
void effNiceMain(EffectEntry* effect);

BOOL effNiceAsync(s32 group) {
    int i;
    BOOL valid;
    const char* name;

    valid = TRUE;
    for (i = 0; i < 7; i++) {
        name = anim_tbl[gp->language][i];
        if (name && !animGroupBaseAsync(name, group, NULL)) {
            valid = FALSE;
        }
    }
    return valid;
}

static void rendermodeFunc(s32 wXluStage) {
    AnimationWork* wp = animGetPtr();

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    wp->currDispMode = wXluStage;
}

EffectEntry* effNiceEntry(s32 type, f32 x, f32 y, f32 z) {
    NiceEffectWork *userdata, *ptr;
    EffectEntry* effect;
    u32 size, count;
    int i;

    effect = effEntry();
    if (type == 5) { //guard for unused slot
        type = 0 & 0xFFu;
    }
    switch (type) {
        case 7:
#ifdef VERSION_G8ME01
            count = acrobat_tbl[gp->language].count + 1;
#else
            if ((u32)gp->language == 0) {
                count = 8;
            } else {
                count = 7;
            }
#endif
            break;
        default:
            count = 1;
            break;
    }
    effect->type = "Nice";
    size = count * sizeof(NiceEffectWork);
    effect->count = count;
    userdata = __memAlloc(HEAP_EFFECT, size);
    effect->userdata = userdata;
    memset(userdata, 0, size);
    effect->callback = effNiceMain;
    effect->flags |= 2;
    userdata->type = type;
    userdata->position.x = x;
    userdata->position.y = y;
    userdata->position.z = z;
    userdata->scaleX = 1.0f;
    userdata->scaleY = 1.0f;
    userdata->scaleZ = 1.0f;
    userdata->overallScale = 1.0f;
    userdata->poseId = -1;
    switch (type) {
    case 7:
        ptr = userdata + 1;
        for (i = 1; i < effect->count; i++, ptr++) {
            ptr->position.x = 1000.0f;
            ptr->position.y = 0.0f;
            ptr->position.z = 0.0f;
            ptr->scaleX = 1.0f;
            ptr->scaleY = 1.0f;
            ptr->scaleZ = 1.0f;
            ptr->state = 0;
            ptr->timer = 0;
        }
        break;
    default:
        size = gp->inBattle != 0;
        userdata->poseId = animPoseEntry(anim_tbl[gp->language][type], size);
        animPoseSetAnim(userdata->poseId, "A_1", TRUE);
        animPoseSetMaterialFlagOn(userdata->poseId, 0x1800);
        animPoseSetGXFunc(userdata->poseId, rendermodeFunc, FALSE);
        break;
    }
    return effect;
}

void effNiceDisp(CameraId cameraId, void* param);
void acrobatMain(EffectEntry* effect);

void effNiceMain(EffectEntry* effect) {
    NiceEffectWork* wp = effect->userdata;
    Vec pos;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    AnimationPose* pose;
    AnimData* data;
#ifdef VERSION_G8ME01
    f32 len, start, t;
#else
    f32 start, len, t;
#endif

    tmp.x = wp->position.x;
    tmp.y = wp->position.y;
    tmp.z = wp->position.z;
    pos = tmp;
    type = wp->type;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        if (wp->poseId != -1) {
            animPoseRelease(wp->poseId);
        }
        effDelete(effect);
        return;
    }
    if (wp->poseId != -1) {
        pose = animPoseGetAnimPosePtr(wp->poseId);
        data = animPoseGetAnimDataPtr(wp->poseId);
        animPoseMain(wp->poseId);
        switch (type) {
        case 0:
#ifdef VERSION_G8ME01
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            len = data->loopInfo[2];
            {
            f32 tbl[7][6] = {
                { 32.0f, 34.0f, 34.0f, 34.0f, 34.0f, 36.0f },
                { 32.0f, 34.0f, 39.0f, 34.0f, 34.0f, 41.0f },
                { 35.0f, 37.0f, 39.0f, 35.0f, 35.0f, 39.0f },
                { 45.0f, 40.0f, 42.0f, 46.0f, 39.0f, 40.0f },
                { 48.0f, 40.0f, 80.0f, 40.0f, 58.0f, 58.0f },
                { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
                { 1000.0f, 37.0f, 46.0f, 42.0f, 39.0f, 62.0f },
            };
            start = tbl[type][gp->language];
            if (pose->localFrame > start) {
                t = 1.0f - (pose->localFrame - start) / (len - start);
                if (t < 0.0f) t = 0.0f;
                animPoseSetMaterialFlagOn(wp->poseId, 0x40);
                animPoseSetMaterialEvtColor(wp->poseId, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * t});
            }
            }
#else
        case 1: {
            len = data->loopInfo[2];
            start = ((u32)gp->language == 0) ? 32.0f : 34.0f;
            if (pose->localFrame > start) {
                t = 1.0f - (pose->localFrame - start) / (len - start);
                if (t < 0.0f) t = 0.0f;
                animPoseSetMaterialFlagOn(wp->poseId, 0x40);
                animPoseSetMaterialEvtColor(wp->poseId, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * t});
            }
            break;
        }
        case 2: {
            len = data->loopInfo[2];
            start = ((u32)gp->language == 0) ? 35.0f : 37.0f;
            if (pose->localFrame > start) {
                t = 1.0f - (pose->localFrame - start) / (len - start);
                if (t < 0.0f) t = 0.0f;
                animPoseSetMaterialFlagOn(wp->poseId, 0x40);
                animPoseSetMaterialEvtColor(wp->poseId, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * t});
            }
            break;
        }
        case 3: {
            len = data->loopInfo[2];
            start = ((u32)gp->language == 0) ? 45.0f : 40.0f;
            if (pose->localFrame > start) {
                t = 1.0f - (pose->localFrame - start) / (len - start);
                if (t < 0.0f) t = 0.0f;
                animPoseSetMaterialFlagOn(wp->poseId, 0x40);
                animPoseSetMaterialEvtColor(wp->poseId, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * t});
            }
            break;
        }
        case 4: {
            len = data->loopInfo[2];
            start = ((u32)gp->language == 0) ? 48.0f : 40.0f;
            if (pose->localFrame > start) {
                t = 1.0f - (pose->localFrame - start) / (len - start);
                if (t < 0.0f) t = 0.0f;
                animPoseSetMaterialFlagOn(wp->poseId, 0x40);
                animPoseSetMaterialEvtColor(wp->poseId, (GXColor){0xFF, 0xFF, 0xFF, 255.0f * t});
            }
            break;
        }
        case 6:
#endif
            break;
        }
        if (animPoseGetLoopTimes(wp->poseId) >= 1.0f) {
            animPoseRelease(wp->poseId);
            effDelete(effect);
            return;
        }
    }
    if (type == 7) {
        acrobatMain(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 8, effNiceDisp, effect);
    }
}

void effNiceDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    NiceEffectWork* wp = effect->userdata;
    Mtx trans, rot, scale, mtx;

    PSMTXTrans(trans, wp->position.x, wp->position.y, wp->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(scale, wp->overallScale, wp->overallScale, wp->overallScale);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXScale(scale, 8.0f, 8.0f, 8.0f);
    PSMTXConcat(mtx, scale, mtx);
#ifdef VERSION_G8ME01
    if (((u32)gp->language == 1 && wp->type == 3) || ((u32)gp->language == 1 && wp->type == 4) || ((u32)gp->language == 5 && wp->type == 6)) {
        PSMTXScale(scale, 0.7f, 0.7f, 0.7f);
        PSMTXConcat(mtx, scale, mtx);
    }
#endif
    animPoseDrawMtx(wp->poseId, mtx, 1, 0.0f, 1.0f);
    animPoseDrawMtx(wp->poseId, mtx, 2, 0.0f, 1.0f);
    animPoseDrawMtx(wp->poseId, mtx, 3, 0.0f, 1.0f);
}

void effAcrobatDisp(CameraId cameraId, void* param);

void acrobatMain(EffectEntry* effect) {
    NiceEffectWork* wp = effect->userdata;
    int i;
    int done = 0;
    Vec pos;
    Vec tmp = {0.0f, 0.0f, 0.0f};

    tmp.x = wp->position.x;
    tmp.y = wp->position.y;
    tmp.z = wp->position.z;
    pos = tmp;
    for (i = 1, wp++; i < effect->count; i++, wp++) {
        // FAKE: integer cast makes the literal a CSE/loop-invariant @ temp (numbers below the SR temps)
        const char* sfx2 = SFX_NAME_WORD("SFX_SYSTEM_ACROBAT1");
        switch (wp->state) {
        case 0:
            wp->timer = i * 4;
            wp->state++;
            if (i == 1) {
                psndSFXOn(sfx2);
            }
            break;
        case 1:
            wp->timer--;
            if (wp->timer < 0) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 2:
            wp->timer++;
            wp->position.x = intplGetValue(12, wp->timer, 30, 500.0f, i * 4);
            if (wp->timer >= 30) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 3:
            wp->timer++;
            wp->scaleX = intplGetValue(12, wp->timer, 8, 1.0f, 0.4f);
            wp->scaleY = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(12, 1.0f, 1.5f, wp->timer, 8);
            if (wp->timer >= 8) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 4:
            wp->timer++;
            if (wp->timer > (effect->count - i - 1) * 4) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 5:
            wp->timer++;
            wp->position.x = intplGetValue(8, wp->timer, 30, i * 4, i * 12);
            wp->scaleX = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(12, 0.2f, 1.0f, wp->timer, 30);
            wp->scaleY = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(12, 1.5f, 1.0f, wp->timer, 30);
            if (wp->timer >= 30) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 6:
            wp->timer++;
            if (wp->timer > 16) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 7:
            wp->timer++;
            wp->scaleX = intplGetValue(12, wp->timer, 15, 1.0f, 1.5f);
            wp->scaleY = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(12, 1.0f, 0.1f, wp->timer, 15);
            if (wp->timer >= 15) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 8:
            wp->timer++;
            wp->scaleX = intplGetValue(12, wp->timer, 15, 1.5f, 0.1f);
            wp->scaleY = ((f32 (*)(s32, f32, f32, s32, s32))intplGetValue)(12, 0.1f, 2.0f, wp->timer, 15);
            if (wp->timer >= 15) {
                wp->timer = 0;
                wp->state++;
            }
            break;
        case 9:
            done++;
            wp->scaleX = wp->scaleY = wp->scaleZ = 0.0f;
            break;
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 8, effAcrobatDisp, effect);
    }
}

void effAcrobatDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    NiceEffectWork* wp = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    Mtx trans, rot, scale, mtx;
    GXTexObj tex;
    f32 size, half, ofsX, ofsY, ten;
    int i;
#ifdef VERSION_G8ME01
    s32 id;
#endif

    PSMTXTrans(trans, wp->position.x, wp->position.y, wp->position.z);
    PSMTXScale(scale, wp->overallScale, wp->overallScale, wp->overallScale);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    {
        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
        GXSetChanMatColor(GX_COLOR0A0, color);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    size = 20.0f;
#ifdef VERSION_G8ME01
    size = -size;
    wp++;
    for (i = 1; i < effect->count; i++, wp++) {
        if (wp->state < 6) {
            ofsX = size / 2.0f;
            ofsY = 0.0f;
        } else {
            ofsX = 0.0f;
            ofsY = size / 2.0f;
        }
        PSMTXTrans(trans, wp->position.x, wp->position.y, wp->position.z);
        PSMTXTrans(rot, ofsX, ofsY, 0.0f);
        PSMTXScale(scale, wp->scaleX, wp->scaleY, wp->scaleZ);
        PSMTXConcat(rot, scale, scale);
        PSMTXTrans(rot, -ofsX, -ofsY, 0.0f);
        PSMTXConcat(scale, rot, scale);
        PSMTXConcat(trans, scale, scale);
        PSMTXConcat(mtx, scale, scale);
        PSMTXConcat(camera->view, scale, scale);
        GXLoadPosMtxImm(scale, 0);
        GXSetCurrentMtx(0);
        id = acrobat_tbl[gp->language].tex[i - 1];
        if (id != -1) {
            effGetTexObj(id, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            half = size / 2.0f;
            ten = 10.0f;
            GXPosition3f32(half, ten, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(ten, ten, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(ten, half, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(half, half, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
#else
    ten = 10.0f;
    size = -size;
    half = size; half *= 0.5f;
    wp++;
    for (i = 1; i < effect->count; i++, wp++) {
        if (wp->state < 6) {
            ofsX = size / 2.0f;
            ofsY = 0.0f;
        } else {
            ofsX = 0.0f;
            ofsY = size / 2.0f;
        }
        PSMTXTrans(trans, wp->position.x, wp->position.y, wp->position.z);
        PSMTXTrans(rot, ofsX, ofsY, 0.0f);
        PSMTXScale(scale, wp->scaleX, wp->scaleY, wp->scaleZ);
        PSMTXConcat(rot, scale, scale);
        PSMTXTrans(rot, -ofsX, -ofsY, 0.0f);
        PSMTXConcat(scale, rot, scale);
        PSMTXConcat(trans, scale, scale);
        PSMTXConcat(mtx, scale, scale);
        PSMTXConcat(camera->view, scale, scale);
        GXLoadPosMtxImm(scale, 0);
        GXSetCurrentMtx(0);
        effGetTexObj(acrobat_tbl[gp->language][i - 1], &tex);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(half, ten, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(ten, ten, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(ten, half, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(half, half, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
#endif
    }
}

