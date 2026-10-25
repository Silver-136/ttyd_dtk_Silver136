#include "driver/effdrv.h"
#include "hwaccess.h"
#include <dolphin/os/OSFastCast.h>
#include "driver/arcdrv.h"
#include "eff/eff_n64.h"
#include "manager/dvdmgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include "libc_decls.h"

//.data
EffectSet eff_set_table[] = {
    {0, "kemuri"},
    {1, "confetti"},
    {2, "fukidashi"},
    {3, "butterfly"},
    {4, "hit"},
    {5, "damage_star"},
    {6, "small_star"},
    {7, "mario_balloon"},
    {8, "mugi"},
    {9, "stardust"},
    {10, "recovery"},
    {11, "miss_star"},
    {12, "nice"},
    {13, "breath_fire"},
    {14, "torch"},
    {15, "confusion"},
    {16, "spark"},
    {17, "updown"},
    {18, "charge"},
    {19, "toge_flush"},
    {20, "ice"},
    {21, "fire"},
    {22, "bomb"},
    {23, "ripple"},
    {24, "coin_fukidashi"},
    {25, "star_point"},
    {26, "puni_balloon"},
    {27, "p_fukidashi"},
    {28, "kemuri_test"},
    {29, "starstone"},
    {30, "itemget"},
    {31, "status"},
    {32, "pointget"},
    {33, "levelup"},
    {34, "stageclear"},
    {35, "fall"},
    {36, "fpdamage"},
    {37, "break"},
    {38, "mobj_broken"},
    {39, "mahojin"},
    {40, "nozle"},
    {41, "mizutama"},
    {42, "minigame"},
    {43, "splash"},
    {44, "treasure_map"},
    {45, "kiss"},
    {46, "funemizu"},
    {47, "mist"},
    {48, "teresa"},
    {49, "batten"},
    {50, "sheep"},
    {51, "naniga"},
    {52, "sandars"},
    {53, "boomerang"},
    {54, "irekae"},
    {55, "rankup"},
    {56, "scanning"},
    {57, "toiki"},
    {58, "mist2"},
    {59, "particle"},
    {60, "ibuki"},
    {61, "mahorn"},
    {62, "spirit"},
    {63, "indirect"},
    {64, "syuryou"},
    {65, "uranoko"},
    {66, "hibashira"},
    {67, "number"},
    {68, "laser"},
    {69, "machingegun"},
    {70, "las_mon"},
    {71, "sleep"},
    {72, "energy"},
    {73, "biribirikinoko"},
    {74, "nokotarou"},
    {75, "queen"},
    {76, "queen2"},
    {77, "gonbaba_breath"},
    {78, "majinai"},
    {79, "mahorn2"},
    {80, "ultra_hammer"},
    {-1, NULL}
};

//.sbss
static EffectWork work;

//.sdata
static EffectWork* wp = &work;
#ifdef VERSION_G8ME01
const char* prefix_tbl[7] = {"jp", "us", "ge", "fr", "sp", "it", "du"};
#else
const char* prefix_tbl[2] = {"jp", "us"};
#endif

//local prototypes
static void _callback_tpl(s32 error, DVDFileInfo* info);

static void _callback_tpl(s32 error, DVDFileInfo* info) {
    DVDEntry* entry = info->cb.userData;
    UnpackTexPalette(wp->texture);
    DVDMgrClose(entry);
    wp->texLoaded = TRUE;
}

void effInit(void) {
#ifdef VERSION_G8ME01
    wp->count = 0x200;
#else
    wp->count = 0x100;
#endif
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(EffectEntry));
    memset(wp->entries, 0, wp->count * sizeof(EffectEntry));
    wp->handle = NULL;
    wp->language = -1;
    wp->texture = NULL;
    wp->texLoaded = FALSE;
    effInit64();
}

#ifdef VERSION_G8ME01
#define EFF_NTEX 118
#else
#define EFF_NTEX 146
#endif

void effTexSetup(void) {
    DVDEntry* entry;
    char path[128];
    u32 size;

    wp->texture = NULL;
    wp->texLoaded = FALSE;
    wp->texture = arcOpen("effect.tpl", 0, 0);
    if (!wp->texture) {
#ifdef VERSION_G8ME01
        sprintf(path, "%s/e/%s/effect.tpl", getMarioStDvdRoot(), gp->language == 0U ? "jp" : "us");
#else
        sprintf(path, "%s/e/jp/effect.tpl", getMarioStDvdRoot());
#endif
        entry = DVDMgrOpen(path, 2, 0);
        size = OSRoundUp32B(DVDMgrGetLength(entry));
        wp->texture = __memAlloc(HEAP_DEFAULT, size);
        entry->info.cb.userData = entry;
        DVDMgrReadAsync(entry, wp->texture, size, 0, _callback_tpl);
    } else {
        UnpackTexPalette(wp->texture);
        wp->texLoaded = TRUE;
    }
    effTexSetupN64();
}

#pragma explicit_zero_data on
void effGetTexObj(u32 id, GXTexObj* obj) {
#ifdef __MWERKS__
    if ((s32)id < EFF_NTEX) { //number of images in effect.tpl
        if (!wp->texLoaded) {
            static char dummy ATTRIBUTE_ALIGN(32) = '\0';
            GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        } else {
            TEXGetGXTexObjFromPalette(wp->texture, obj, id);
        }
    } else {
        static char dummy ATTRIBUTE_ALIGN(32) = '\0';
        if (!wp->handle) {
            GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        } else {
            TEXGetGXTexObjFromPalette(*wp->handle->data, obj, id - EFF_NTEX);
        }
    }
#endif
}
#pragma explicit_zero_data reset

void effAutoRelease(BOOL inBattle) {
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (entry->flags & 1 && (s32)entry->inBattle == inBattle && entry && entry->flags) {
            __memFree(HEAP_EFFECT, entry->userdata);
            entry->flags = 0;
        }
    }
}

EffectEntry* effEntry(void) {
    EffectEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (!entry->flags) {
            break;
        }
    }
    entry->flags = 1;
    entry->inBattle = gp->inBattle != 0;
    entry->type = 0;
    entry->count = 0;
    entry->userdata = NULL;
    entry->callback = NULL;
    entry->name[0] = '\0';
    return entry;
}

void effSetName(EffectEntry* effect, const char* name) {
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (entry->flags && !strcmp(entry->name, name)) {
            break;
        }
    }
    strcpy(effect->name, name);
}

void effMain(void) {
    const char *dvdroot, *prefix;
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (gp->inBattle) {
            if ((s32)entry->inBattle != 1) {
                continue;
            }
        } else if ((s32)entry->inBattle != 0) {
            continue;
        }
        if (entry->flags & 1) {
            if (entry->callback) {
                entry->callback(entry);
            }
        }
    }
    if ((u32)wp->language != (u32)gp->language) {
        if (wp->handle) {
            fileFree(wp->handle);
            wp->handle = NULL;
        }
        prefix = prefix_tbl[gp->language];
        dvdroot = getMarioStDvdRoot();
        if (fileAsyncf(4, 0, "%s/e/%s/effect_%s.tpl", dvdroot, prefix, prefix) != 0U) {
            prefix = prefix_tbl[gp->language];
            wp->handle = fileAllocf(4, "%s/e/%s/effect_%s.tpl", getMarioStDvdRoot(), prefix, prefix);
            wp->language = gp->language;
        }
    }
}

void effDelete(EffectEntry* effect) {
    if (effect && effect->flags) {
        __memFree(HEAP_EFFECT, effect->userdata);
        effect->flags = 0;
    }
}

void effSoftDelete(EffectEntry* effect) {
    if (effect && effect->flags) {
        if (effect->flags & 2) {
            effect->flags |= 4; // "deleted"
        } else {
            effDelete(effect); //inlined
        }
    }
}

EffectEntry* effNameToPtr(const char* name) {
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (entry->flags && !strcmp(entry->name, name)) {
            break;
        }
    }
    if (i >= count) {
        return NULL;
    } else {
        return entry; //found a match
    }
}

EffectSet* effGetSet(const char* name) {
    s32 i;

    for (i = 0; eff_set_table[i].id != -1; i++) { //run until end of table
        if (!strcmp(eff_set_table[i].name, name)) {
            return &eff_set_table[i];
        }
    }
    return NULL;
}

typedef struct MayaKey {
    s16 time;
    u8 data[0xb0];
} MayaKey;

typedef struct MayaKeyList {
    s32 count;
    MayaKey* keys;
    s32 pad[2];
} MayaKeyList;

typedef struct MayaListHdr {
    s32 count;
    MayaKeyList* entries;
} MayaListHdr;

typedef struct MayaAnimData {
    u8 pad0[4];
    f32 endTime;
    MayaListHdr* lists;
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

typedef struct MayaPoly {
    s32 pad0[2];
    s32 count;
    s16** strips;
} MayaPoly;

// per-key interpolation and the GQR5 setup were inlines in the original: inline locals take the low registers,
// last declared lowest (qv0 f9 .. qm1 f12 above dt f8 / s f7 / s3 f6 / s2 f5; gqr r9 under the key-offset temp r10)
static inline void effMayaKeyInterp(MayaKey* prev, MayaKey* curr, f32 t, f32* buf) {
    s32 i;
    f32* out;
    register s8* pb;
    register s8* pa;
    pa = (s8*)prev;
    pb = (s8*)curr;
    out = buf;
    for (i = 0; i < 22; i++) {
        register f32 qd0;
        register f32 qm1;
        f32 u;
        register f32 qv1;
        register f32 qm0;
        register f32 qv0;
        f32 dt;
        f32 s;
        f32 s3;
        f32 s2;
        f32 div;
        qv0 = __OSs16tof32((s16*)(pa + 2));
        qd0 = __OSs16tof32((s16*)(pa + 4));
        qm0 = __OSs16tof32((s16*)(pa + 6));
        qv1 = __OSs16tof32((s16*)(pb + 2));
        qm1 = __OSs16tof32((s16*)(pb + 4));
        qd0 = __OSs16tof32((s16*)(pb + 6));
        if (pa[9] != 0) {
            *out = qv0;
        } else {
            u = t - prev->time;
            dt = curr->time - prev->time;
            div = (!dt) ? ( 0.00000001f ) : ( dt) ;
            s = u / div;
            s2 = s * s;
            s3 = s2 * s;
            *out = ((( qv0 * (1.0f + (( 2.0f * s3 ) - ( 3.0f * s2)) )) + ( qm0 * (dt * (s + (s3 - ( 2.0f * s2))) ))) + ( qv1 * ((( -2.0f ) * s3 ) + ( 3.0f * s2) ))) + ( qm1 * (dt * (s3 - s2) ));
        }
        pa += 8;
        pb += 8;
        out++;
    }
}

static inline void effMayaSetGQR(void) {
    register u32 gqr;
    gqr = 0x06070607;
    HW_SET_GQR5(gqr);
}

void effCalcMayaAnimMatrix(MayaAnim* anim, MayaKeyList* list, f32* mtx, f32* alpha) {
    f32 buf[22];
    Mtx trans;
    Mtx scale;
    Mtx tmp2;
    Mtx pivot2;
    Mtx pivot2Inv;
    Mtx trans3;
    Mtx pivot1;
    Mtx pivot1Inv;
    Mtx trans2;
    Mtx rotX;
    Mtx rotY;
    Mtx rotZ;
    Mtx tmp;

    s32 n = list->count;
    s32 k;
    f32 t;

    for (k = 1; k < n; k++) {
        MayaKey* prev;
        MayaKey* curr;
        curr = &list->keys[k];
        prev = &list->keys[k - 1];
        effMayaSetGQR();
        if (anim->time <= curr->time) {
            t = anim->time;
        } else if (k != ( n - 1) ) {
            continue;
        } else {
            t = curr->time;
        }
        effMayaKeyInterp(prev, curr, t, buf);
        PSMTXTrans(trans, buf[0], buf[1], buf[2]);
        PSMTXRotRad(rotX, 'x', buf[3]);
        PSMTXRotRad(rotY, 'y', buf[4]);
        PSMTXRotRad(rotZ, 'z', buf[5]);
        PSMTXScale(scale, buf[6], buf[7], buf[8]);
        PSMTXTrans(pivot1, buf[9], buf[10], buf[11]);
        PSMTXTrans(pivot1Inv, -buf[9], -buf[10], -buf[11]);
        PSMTXTrans(trans2, buf[12], buf[13], buf[14]);
        PSMTXTrans(pivot2, buf[15], buf[16], buf[17]);
        PSMTXTrans(pivot2Inv, -buf[15], -buf[16], -buf[17]);
        PSMTXTrans(trans3, buf[18], buf[19], buf[20]);
        PSMTXConcat(trans3, pivot2, tmp);
        PSMTXConcat(tmp, scale, tmp);
        PSMTXConcat(tmp, pivot2Inv, scale);
        PSMTXConcat(trans2, pivot1, tmp);
        PSMTXConcat(tmp, rotZ, tmp);
        PSMTXConcat(tmp, rotY, tmp);
        PSMTXConcat(tmp, rotX, tmp);
        PSMTXConcat(tmp, pivot1Inv, tmp2);
        PSMTXConcat(trans, tmp2, tmp);
        PSMTXConcat(tmp, scale, (MtxPtr)mtx);
        *alpha = ( buf[21] > 1.0f ) ? ( 1.0f ) : ((buf [21] < ( (buf[21] < 0.0f ) * 0.0f)) ? ( 0.0f ) : ( buf[21]) );
        return;
    }
}

MayaAnim* effMayaAnimAlloc(MayaAnimData* data) {
    s32 size = data->lists->count * sizeof(MayaJoint) + 0x18;
    MayaAnim* anim = __memAlloc(HEAP_EFFECT, size);
    memset(anim, 0, size);
    anim->data = data;
    return anim;
}

void effDeleteMayaAnim(MayaAnim* anim) {
    __memFree(HEAP_EFFECT, anim);
}

void effPlayMayaAnim(MayaAnim* anim) {
    OSTime now = gp->inBattle ? gp->renderTime : gp->renderFieldTime;
    anim->startTime = OSTicksToMilliseconds(now);
    anim->time = 0.0f;
    anim->flags |= 1;
}

void effCalcMayaAnim(MayaAnim* anim) {
    if ((anim->flags & 1) && !(anim->flags & 2)) {
        OSTime now = gp->inBattle ? gp->renderTime : gp->renderFieldTime;
        s32 i;
        now = OSTicksToMilliseconds(now);
        anim->time = (f32)(u64)(now - anim->startTime) * gp->framerate / 1000.0f;
        if (anim->time >= anim->data->endTime) {
            anim->time = anim->data->endTime;
            anim->flags |= 2;
        }
        for (i = 0; i < anim->data->lists->count; i++) {
            effCalcMayaAnimMatrix(anim, &anim->data->lists->entries[i], (f32*)anim->joints[i].mtx, &anim->joints[i].alpha);
        }
    }
}

void effDrawMayaPoly(MayaPoly* poly) {
    s32 i;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 9);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 9);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < poly->count; i++) {
        s16* strip = poly->strips[i];
        u16 n = strip[0];
        s16* v;
        s32 j;
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, n);
        v = strip + 1;
        for (j = 0; j < n; j++, v += 5) {
            GXPosition3s16(v[0], v[1], v[2]);
            GXTexCoord2s16(v[3], v[4]);
        }
    }
}

// forward declarations: these functions are defined further down
