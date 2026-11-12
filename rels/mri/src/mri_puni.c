// mri_puni (巨木の中, The Great Tree): the Puni crowd system: every puni follows Mario or its group leader,
// walks the maze maps (puniMapdata), falls, fights and is saved/restored through the LSW bytes; driven by the
// evt_puni_* calls.

#include "version.h"
#include "eff/eff_fukidashi.h"
#include "evt/evt_story.h"
#include "eff/eff_puniballoon.h"
#include "mri_puni.h"
#include "party.h"
#include "party/party_cloud.h"
#include "mario/mario_party.h"
#include "driver/camdrv.h"
#include "memory.h"
#include <string.h>
#include "mario/mariost.h"
#include "mario/mario.h"
#include "driver/icondrv.h"
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "driver/hitdrv.h"

#include "driver/swdrv.h"

#include "evt/evt_seq.h"
#include "evt/evt_npc.h"
#include "pmario_sound.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"
#include "driver/punidrv.h"
#include "driver/mapdrv.h"
#include <dolphin/gx.h>
#include "evt/evt_shop.h"
#include "system.h"
#include "mri.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cmd.h"
#include "driver/animdrv.h"
#include "eff/eff_n64.h"
#include "driver/seqdrv.h"
#include "evt/evt_save_flags.h"
#include "icon_ids.h"
extern int abs(int);
void envSetYami(MapTevCallbackInfo* info);

/**
 * Animation frame sequences of the Puni, one per action, each a list of frame numbers ending with 65535 or 65534:
 * this one is the idle loop
 * (the walk, run, damage, down, roll, stray, jump and wander sequences below follow the same layout).
 */
static u16 puni_nom_patern_data[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,    21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0,  1,  2,  3,  4,  5,     6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,    31, 32, 33, 34,
    35, 36, 37, 38, 39, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,    16, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 51, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 65535,
};

/**
 * The remaining Puni action sequences: walking, running, damaged, knocked down, rolling, stray (hagure),
 * jumping up, jumping down and wandering.
 */
static u16 puni_wlk_patern_data[] = {
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 65535,
};
static u16 puni_run_patern_data[] = { 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 65535 };
static u16 puni_dmg_patern_data[] = { 72, 65535 };
static u16 puni_dwn_patern_data[] = { 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 65535 };
static u16 puni_rol_patern_data[] = { 83, 65535 };
static u16 puni_hag_patern_data[] = { 0, 65535 };
static u16 puni_jpu_patern_data[] = { 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 65534 };
static u16 puni_jpd_patern_data[] = { 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 65534 };
static u16 puni_wan_patern_data[] = { 104, 105, 106, 107, 108, 109, 110, 111, 65534 };

/**
 * The ten Puni action sequences in the order of the animation sets
 * (idle, walk, run, damage, down, roll, stray, jump up, jump down, wander); registered with
 * puniAnimationInit_inl.
 */
static u16* puni_patern_table[] = { puni_nom_patern_data, puni_wlk_patern_data, puni_run_patern_data, puni_dmg_patern_data,
                             puni_dwn_patern_data, puni_rol_patern_data, puni_hag_patern_data, puni_jpu_patern_data,
                             puni_jpd_patern_data, puni_wan_patern_data };

/**
 * Animation frame sequences of the Jabbi (toge): the idle loop, which has 24 frames.
 */
static u16 toge_nom_patern_data[25] = {
    84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 65535,
};

/**
 * The remaining Jabbi action sequences: walking, running, damaged, knocked down, rolling and stray (hagure).
 */
static u16 toge_wlk_patern_data[] = { 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 65535 };
static u16 toge_run_patern_data[] = { 108, 110, 112, 114, 116, 118, 65535 };
static u16 toge_dmg_patern_data[] = { 120, 65535 };
static u16 toge_dwn_patern_data[] = { 121, 122, 123, 124, 125, 126, 127, 128, 65535 };
static u16 toge_rol_patern_data[] = { 129, 65535 };
static u16 toge_hag_patern_data[] = { 0, 65535 };

/**
 * The ten Jabbi action sequences in the same order as puni_patern_table
 * (the stray sequence stands in for the last four).
 */
static u16* toge_patern_table[] = { toge_nom_patern_data, toge_wlk_patern_data, toge_run_patern_data, toge_dmg_patern_data,
                             toge_dwn_patern_data, toge_rol_patern_data, toge_hag_patern_data, toge_hag_patern_data,
                             toge_hag_patern_data, toge_hag_patern_data };

//.bss
static PuniData* Punidat;
static PuniWork* Punigp;
static PuniTexInfo* Punitex;
PuniMap* Punimap;

/**
 * Returns the table of Puni data (Punidat, PUNI_DATA_MAX slots: Mario's stand-in, the Punies 1 to 102,
 * the blockers 103 to 109 and the Jabbi 110 to 209). Used by the C code of mri.c, mri_01, mri_03, mri_13 and mri_16.
 */
PuniData* puniGetPunidat(void) {
    return Punidat;
}

/** Returns the shared Puni work area (Punigp: current floor, y mode, targets, sound handles and so on). */
PuniWork* puniGetPunigp(void) {
    return Punigp;
}

/** Returns the table of Puni texture frames built by evt_make_texbuf. */
PuniTexInfo* puniGetTex(void) {
    return Punitex;
}

/**
 * Draw callback for the status icons over the Puni on the two battle floors (9 and 12):
 * shows the dizzy stars circling a dizzy Jabbi and clears the healed flag of a Puni whose HP was just restored
 * after about 30 frames (nothing is drawn for that flag).
 */
void evt_icon_disp_func(CameraId camId, void* param) {
    PuniData* p;
    s32 i;
    if (Punigp->floor == 9 || Punigp->floor == 12) {
        for (i = 1; i < PUNI_DATA_MAX; i++) {
            p = &Punidat[i];
            if ((p->flags & PUNI_FLAG_ACTIVE) && !(p->flags & (PUNI_FLAG_DEAD | PUNI_FLAG_NOT_IN_PARTY))) {
                Vec a;
                Vec b;
                a = p->pos;
                if (p->flags2 & PUNI_FLAG2_HEALED) {
                    p->flag1Timer++;
                    if ((s8)p->flag1Timer > 30) {
                        p->flag1Timer = 0;
                        p->flags2 &= ~PUNI_FLAG2_HEALED;
                    }
                }
                if (p->flags2 & PUNI_FLAG2_DIZZY_ICON) {
                    b = a;
                    puniSinCosRet(p->index + gp->retraceCount * 10, &a.z, &a.x, 9.0f, 9.0f);
                    a.y += 13.0f;
                    iconDispGx(&a, 0, ICON_BTL_CMD_SUPER_ACTION, 0.6f);
                    puniSinCosRet(p->index + gp->retraceCount * 10 + 180, &b.z, &b.x, 9.0f, 9.0f);
                    b.y += 15.0f;
                    iconDispGx(&b, 0, ICON_BTL_CMD_SUPER_ACTION, 0.5f);
                }
            }
        }
    }
}

static PuniTexInfo* _make_texbuf(PuniTexInfo* texbuf, s32 count, const char* group, const char* anim, f32 frame) {
    s32 i;

    for (i = 0; i < count; i++) {
        texbuf->group = _mapAlloc(0xC);
        texbuf->anim = _mapAlloc(0xC);
        strcpy(texbuf->group, group);
        strcpy(texbuf->anim, anim);
        texbuf->frame = frame;
        frame += 1.0f;
        texbuf++;
    }
    return texbuf;
}

/**
 * Builds the table of Puni texture frames: the Puni stand, walk, damage, down and fall animations (c_puni),
 * then the Jabbi (c_toge) frames on the battle floors 9 and 12 or the Puni jump and balloon frames elsewhere,
 * ended by an empty record.
 */
static USER_FUNC(evt_make_texbuf) {
    PuniTexInfo* texbuf = Punitex;
    texbuf = _make_texbuf(texbuf, 40, "c_puni", "S_1", 0.0f);
    texbuf = _make_texbuf(texbuf, 12, "c_puni", "S_1", 97.0f);
    texbuf = _make_texbuf(texbuf, 20, "c_puni", "W_1", 0.0f);
    texbuf = _make_texbuf(texbuf, 1, "c_puni", "D_1", 0.0f);
    texbuf = _make_texbuf(texbuf, 10, "c_puni", "D_2", 0.0f);
    texbuf = _make_texbuf(texbuf, 1, "c_puni", "F_1", 0.0f);

    if (Punigp->floor == 9 || Punigp->floor == 12) {
        texbuf = _make_texbuf(texbuf, 24, "c_toge", "S_1", 0.0f);
        texbuf = _make_texbuf(texbuf, 12, "c_toge", "W_1", 0.0f);
        texbuf = _make_texbuf(texbuf, 1, "c_toge", "D_1", 0.0f);
        texbuf = _make_texbuf(texbuf, 8, "c_toge", "D_2", 0.0f);
        texbuf = _make_texbuf(texbuf, 1, "c_toge", "F_1", 0.0f);
    } else {
        texbuf = _make_texbuf(texbuf, 10, "c_puni", "J_1A", 0.0f);
        texbuf = _make_texbuf(texbuf, 10, "c_puni", "J_1B", 0.0f);
        texbuf = _make_texbuf(texbuf, 8, "c_puni", "B_1", 0.0f);
    }
    texbuf->group = 0;
    texbuf->anim = 0;
    texbuf->frame = 0.0f;
    return EVT_RETURN_DONE;
}

/**
 * Counts the frames of an animation pattern (a list of frame numbers ended by -1 or -2) into *count
 * and stores the terminating value in *end.
 */
static void puniAnimCount(s16* pattern, s16* count, s16* end) {
    s16 n = 0;
    s32 i = 0;

    while (TRUE) {
        if (pattern[i] == -1) {
            *end = pattern[i];
            break;
        }
        if (pattern[i] == -2) {
            *end = pattern[i];
            break;
        }
        n++;
        i++;
    }
    *count = n;
}

/**
 * Registers the ten animation patterns of the Puni (a[]) and of the Jabbi (b[]) with their frame counts
 * and terminator values in Punigp->anim[0] and [1].
 */
static void puniAnimationInit(s16** a, s16** b) {
    PuniAnimSet* s0 = &Punigp->anim[0];
    PuniAnimSet* s1 = &Punigp->anim[1];
    s32 j;
    s32 i;
    s16* p;
    s16 v;
    s32 k;

    for (i = 0; i < 10; i++) {
        s0->table[i] = a[i];
        puniAnimCount(a[i], &s0->count[i], &s0->end[i]);
        s1->table[i] = b[i];
        k = 0;
        j = 0;
        for (p = b[i];; j++, k++) {
            v = p[k];
            if (v == -1) {
                s1->end[i] = v;
                break;
            }
            if (v == -2) {
                s1->end[i] = v;
                break;
            }
        }
        s1->count[i] = j;
    }
}

/**
 * Fills Punigp->animNames with the pose names the guide NPCs use for each Puni animation id (S_1 stand,
 * W_1 walk, R_1 run, D_1/D_2 damage and down, F_1 fall, Z_1, J_1A/J_1B jump, B_1) for the guide, the sister,
 * the little Jabbi and the rival.
 */
static void puniAnimationInit2(void) {
    char (*n)[16];
    n = (char (*)[16])&Punigp->animNames[0];
    strcpy(n[0], "S_1");
    strcpy(n[1], "W_1");
    strcpy(n[2], "R_1");
    strcpy(n[3], "D_1");
    strcpy(n[4], "D_2");
    strcpy(n[5], "F_1");
    strcpy(n[6], "Z_1");
    strcpy(n[7], "J_1A");
    strcpy(n[8], "J_1B");
    strcpy(n[9], "B_1");
    n = (char (*)[16])&Punigp->animNames[160];
    strcpy(n[0], "S_1");
    strcpy(n[1], "W_1");
    strcpy(n[2], "R_1");
    strcpy(n[3], "D_1");
    strcpy(n[4], "D_2");
    strcpy(n[5], "F_1");
    strcpy(n[6], "Z_1");
    strcpy(n[7], "J_1A");
    strcpy(n[8], "J_1B");
    strcpy(n[9], "B_1");
    n = (char (*)[16])&Punigp->animNames[320];
    strcpy(n[0], "S_1");
    strcpy(n[1], "W_1");
    strcpy(n[2], "R_1");
    strcpy(n[3], "D_1");
    strcpy(n[4], "D_2");
    strcpy(n[5], "F_1");
    strcpy(n[6], "Z_1");
    strcpy(n[7], "J_1A");
    strcpy(n[8], "J_1B");
    strcpy(n[9], "B_1");
    n = (char (*)[16])&Punigp->animNames[480];
    strcpy(n[0], "S_1");
    strcpy(n[1], "W_1");
    strcpy(n[2], "R_1");
    strcpy(n[3], "D_2");
    strcpy(n[4], "D_2");
    strcpy(n[5], "F_1");
    strcpy(n[6], "Z_1");
    strcpy(n[7], "J_1A");
    strcpy(n[8], "J_1B");
    strcpy(n[9], "B_1");
}

/**
 * Per-frame animation and placement of one Puni.
 * For ordinary Punies it picks the texture frame from the pattern tables and writes position, scale, direction,
 * spin and offset into the draw pose t; for the four story Punies (Punio, his sister, the little Jabbi and the
 * rival), which are drawn by their stand-in NPCs, it sets the NPC's pose,
 * position and rotation instead and parks the draw pose out of view.
 * Inactive or hidden Punies are moved to y=-1000.
 */
static void puniAnimation(PuniData* p, PunidrvPose* t) {
    NpcEntry* npc;
    char* names;
    PuniAnimSet* set;
    s32 cnt;
    s16* tbl;
    s32 end;
    s32 show;
    s32 f;
    s32 ofs;

    if (!(p->flags & PUNI_FLAG_SCRIPT_CONTROL)) {
    if (p->flags & (PUNI_FLAG_NPC_GUIDE | PUNI_FLAG_NPC_GUIDE_SISTER | PUNI_FLAG_NPC_CHIBI_TOGE | PUNI_FLAG_NPC_RIVAL)) {
        if (p->flags & PUNI_FLAG_NPC_GUIDE) {
            npc = npcNameToPtr("\203K\203C\203hsp");
            names = (char*)( &Punigp->animNames[0]) ;
        } else if (p->flags & PUNI_FLAG_NPC_GUIDE_SISTER) {
            npc = npcNameToPtr("\203K\203C\203h\226\205sp");
            names = (char*)( &Punigp->animNames[160]) ;
        } else if (p->flags & PUNI_FLAG_NPC_CHIBI_TOGE) {
            npc = npcNameToPtr("\202\277\202\321\203g\203Qsp");
            names = (char*)( &Punigp->animNames[320]) ;
        } else if (p->flags & PUNI_FLAG_NPC_RIVAL) {
            npc = npcNameToPtr("\203\211\203C\203o\203\213sp");
            names = (char*)( &Punigp->animNames[480]) ;
        } else {
            npc = npcNameToPtr("\203\211\203C\203o\203\213sp");
            names = (char*)( &Punigp->animNames[480]) ;
        }
        ofs = p->marioDist;
        if (p->flags & PUNI_FLAG_OWN_ANIM_TIME) {
            p->animTime++;
        } else {
            if (p->mode != PUNI_MODE_PARENT) {
                ofs = p->index ;
            } else {
                ofs /= 8;
                if (p->anim == 0) {
                    ofs += p->index * 40;
                }
            }
            p->animTime = gp->retraceCount + ofs;
        }
        if (!(p->flags & PUNI_FLAG_SCRIPT_ANIM)) {
            animPoseSetAnim(npc->poseId, names + ( p->anim * 16) , 1);
            animPoseSetLocalTime(npc->poseId, p->animTime);
        }
        show = 0;
        if (p->flags & PUNI_FLAG_ACTIVE) { show = 1;
        }
        if (p->flags & PUNI_FLAG_HIDDEN) { show = 0;
        }
        if (show) {
            npc->position.x = p->pos.x + p->dispOfs.x;
            npc->position.y = p->pos.y + p->dispOfs.y;
            npc->position.z = p->pos.z + p->dispOfs.z;
        } else {
            npc->position.x = 0.0f;
            npc->position.y = -1000.0f;
            npc->position.z = 0.0f;
        }
        (*t->mtx)[0][3] = 0.0f;
        (*t->mtx)[1][3] = -1000.0f;
        (*t->mtx)[2][3] = 0.0f;
        npc->scale.x = p->scale;
        npc->scale.y = p->scale;
        npc->direction = p->dir;
        npc->rotation.z = p->spin;
        npc->rotationOffset.y = p->rotOffsetY;
    } else {
        set = &Punigp->anim[0];
        if (p->flags & PUNI_FLAG_SPRITE_PUNI) { set = &Punigp->anim[0];
        }
        if (p->flags & PUNI_FLAG_SPRITE_TOGE) { set = &Punigp->anim[1];
        }
        tbl = set->table[p->anim];
        cnt = set->count[p->anim];
        end = set->end[p->anim];
        ofs = p->marioDist;
        if (p->flags & PUNI_FLAG_OWN_ANIM_TIME) {
            f = p->animTime % cnt;
            if (( end == ( -2 )) && ( cnt <= p->animTime) ) { f = cnt - 1;
            }
            t->texId = tbl[f];
            p->animTime++;
        } else {
            if (p->mode != PUNI_MODE_PARENT) {
                ofs = *(( s16*)( &p->index)) ;
            } else {
                ofs /= 8;
                if (p->anim == 0) {
                    ofs += ( *( (s16*)( &p->index ))) * (cnt / 3);
                }
            }
            t->texId = tbl[(( u32)(gp->retraceCount + ofs) ) % ( (u32)set->count [p->anim])];
        }
        show = 0;
        if (p->flags & PUNI_FLAG_ACTIVE) { show = 1;
        }
        if (p->flags & PUNI_FLAG_HIDDEN) { show = 0;
        }
        if (show) {
            (*t->mtx)[0][3] = p->pos.x + p->dispOfs.x;
            (*t->mtx)[1][3] = p->pos.y + p->dispOfs.y;
            (*t->mtx)[2][3] = p->pos.z + p->dispOfs.z;
        } else {
            (*t->mtx)[0][3] = 0.0f;
            (*t->mtx)[1][3] = -1000.0f;
            (*t->mtx)[2][3] = 0.0f;
        }
        t->scale = p->scale;
        t->dir = -p->dir;
        t->rotZ = p->spin;
        t->offsetY = p->rotOffsetY;
    }
    p->prevAnim = p->anim;
    }
}

/**
 * "chukan hosei" (in-between correction): height on one stair of the sloped floors.
 * pos runs from start to start + 20; over the first 10 units the height falls from base + 10 to base,
 * then it stays at base.
 */
static f32 _chukanhosei(f32 a, f32 b, f32 c) {
    f32 t = (20.0f + b - a) / 20.0f;
    if (t > 0.5f) {
        t = (t - 0.5f) / 0.5f;
    } else {
        t = 0.0f;
    }
    return 10.0f * t + c;
}

/** Whether (px, pz) lies in the square of half-width r around (x, z). */
static BOOL _areacheck(f32 x, f32 z, f32 r, f32 px, f32 pz) {
    BOOL ret = TRUE;
    if (x - r > px) {
        ret = FALSE;
    }
    if (x + r < px) {
        ret = FALSE;
    }
    if (z - r > pz) {
        ret = FALSE;
    }
    if (z + r < pz) {
        ret = FALSE;
    }
    return ret;
}

#define STAIR_Y_BY_Z(thr, base)             \
    if (p->pos.z <= (thr)) {                \
        f32 t = ((thr) - p->pos.z) / 20.0f; \
        f32 u;                              \
        if (t > 0.5f) {                     \
            u = (t - 0.5f) / 0.5f;          \
        } else {                            \
            u = floorY;                     \
        }                                   \
        p->pos.y = 10.0f * u + (base);      \
    }

/**
 * Height of a Puni on the slanted floors of mri_09 (floor ymode 2):
 * on the stair slope east of x=200 it follows 10-unit steps down from 100 to 0 along z;
 * on the central area it is level at 0.
 */
static void _ymode_mri_09(PuniData* p) {
    float floorY;
    floorY = 0.0f;
    if (p->pos.x <= 400.0f && p->pos.x > 200.0f && p->pos.y <= 100.0f && p->pos.y >= floorY) {
        if (p->pos.z <= -55.0f) {
            p->pos.y = 100.0f;
        } else STAIR_Y_BY_Z(-35.0f, 90.0f)
        else STAIR_Y_BY_Z(-15.0f, 80.0f)
        else STAIR_Y_BY_Z(5.0f, 70.0f)
        else STAIR_Y_BY_Z(25.0f, 60.0f)
        else STAIR_Y_BY_Z(45.0f, 50.0f)
        else STAIR_Y_BY_Z(65.0f, 40.0f)
        else STAIR_Y_BY_Z(85.0f, 30.0f)
        else STAIR_Y_BY_Z(105.0f, 20.0f)
        else STAIR_Y_BY_Z(125.0f, 10.0f)
        else STAIR_Y_BY_Z(145.0f, floorY)
        else {
            p->pos.y = floorY;
        }
    }
    if (p->pos.x >= -170.0f && p->pos.x <= 170.0f && p->pos.z >= -80.0f && p->pos.z <= 140.0f) {
        p->pos.y = 0.0f;
    }
}

#define STAIR_Y_BY_X(thr, base)             \
    if (p->pos.x <= (thr)) {                \
        f32 t = ((thr) - p->pos.x) / 20.0f; \
        f32 u;                              \
        if (t > 0.5f) {                     \
            u = (t - 0.5f) / 0.5f;          \
        } else {                            \
            u = 0.0f;                       \
        }                                   \
        p->pos.y = 10.0f * u + (base);      \
    }

/**
 * Height of a Puni on the lower sloped floor of mri_16 (ymode 4):
 * the ramp from y=-320 down to y=-400 along x, with PUNI_FLAG2_ON_SLOPE set while on it.
 * A Puni leaving the ramp sideways is put back at its previous position so it cannot step over the ramp edge.
 */
static void _ymode_mri_16_00(PuniData* p) {
    u32 old = p->flags2;
    p->flags2 &= ~PUNI_FLAG2_ON_SLOPE;
    if (p->pos.y <= -200.0f && p->pos.y >= -400.0f) {
        if (p->pos.x <= 40.0f && p->pos.x >= -210.0f && p->pos.z <= -76.46314f && p->pos.z >= -200.0f) {
            if (p->pos.x <= -190.0f) {
                p->pos.y = -320.0f;
            } else STAIR_Y_BY_X(-170.0f, -330.0f)
            else STAIR_Y_BY_X(-150.0f, -340.0f)
            else STAIR_Y_BY_X(-130.0f, -350.0f)
            else STAIR_Y_BY_X(-110.0f, -360.0f)
            else STAIR_Y_BY_X(-90.0f, -370.0f)
            else STAIR_Y_BY_X(-70.0f, -380.0f)
            else STAIR_Y_BY_X(-50.0f, -390.0f)
            else STAIR_Y_BY_X(-30.0f, -400.0f)
            else {
                p->pos.y = -400.0f;
            }
            if (p->pos.y < -320.0f && p->pos.y > -400.0f) {
                p->flags2 |= PUNI_FLAG2_ON_SLOPE;
            }
        }
        if (p->pos.x <= 40.0f && p->pos.x >= -200.0f && p->pos.z > -76.46314f && p->pos.z < 200.0f) {
            p->pos.y = -400.0f;
        }
    }
    if (!(p->flags2 & PUNI_FLAG2_CROSSING_LEDGE) && !(p->flags2 & PUNI_FLAG2_ON_SLOPE) && (old & PUNI_FLAG2_ON_SLOPE)) {
        if (p->pos.z > -76.46314f && p->prevPos.z <= -76.46314f) {
            p->pos = p->prevPos;
            p->pos.z -= 0.0002f;
        }
    }
}

/**
 * Height of a Puni on the upper floor of mri_16 (ymode 5):
 * the ramp from y=80 down to y=0 along x, with PUNI_FLAG2_ON_SLOPE set while on it,
 * and the edge check that stops a Puni stepping off the ramp sideways.
 */
static void _ymode_mri_16_01(PuniData* p) {
    u32 old = p->flags2;
    f32 bz;
    p->flags2 &= ~PUNI_FLAG2_ON_SLOPE;
    if (p->pos.y <= 100.0f && p->pos.y >= 0.0f) {
        if (p->pos.x <= 40.0f && p->pos.x >= -200.0f && p->pos.z > -200.0f && p->pos.z < 200.0f) {
            p->pos.y = 0.0f;
        }
        if (p->pos.x <= 40.0f && p->pos.x >= -210.0f && p->pos.z <= 60.00006f && p->pos.z >= -10.00012f) {
            if (p->pos.x <= -190.0f) {
                p->pos.y = 80.0f;
            } else STAIR_Y_BY_X(-170.0f, 70.0f)
            else STAIR_Y_BY_X(-150.0f, 60.0f)
            else STAIR_Y_BY_X(-130.0f, 50.0f)
            else STAIR_Y_BY_X(-110.0f, 40.0f)
            else STAIR_Y_BY_X(-90.0f, 30.0f)
            else STAIR_Y_BY_X(-70.0f, 20.0f)
            else STAIR_Y_BY_X(-50.0f, 10.0f)
            else if (p->pos.x <= (-30.0f)) {
                f32 t = ((-30.0f) - p->pos.x) / 20.0f;
                f32 u;
                if (t > 0.5f) {
                    u = (t - 0.5f) / 0.5f;
                } else {
                    u = 0.0f;
                }
                bz = (0.0f);
                p->pos.y = 10.0f * u + bz;
            } else {
                p->pos.y = 0.0f;
            }
            if (p->pos.y < 80.0f && p->pos.y > 0.0f) {
                p->flags2 |= PUNI_FLAG2_ON_SLOPE;
            }
        }
    }
    if (!(p->flags2 & PUNI_FLAG2_CROSSING_LEDGE) && !(p->flags2 & PUNI_FLAG2_ON_SLOPE) && (old & PUNI_FLAG2_ON_SLOPE)) {
        if (p->pos.z > 60.00006f && p->prevPos.z <= 60.00006f) {
            p->pos = p->prevPos;
            p->pos.z -= 0.0002f;
        }
        if (p->pos.z < -10.00012f && p->prevPos.z >= -10.00012f) {
            p->pos = p->prevPos;
            p->pos.z += 0.0002f;
        }
    }
}

#define IN_RECT_XZ(p, x0, x1, z0, z1) \
    (_aok = 1, \
     _ax = (p)->pos.x, \
     _az = (p)->pos.z, \
     ((x0) > _ax ? _aok = 0 : 0), \
     ((x1) < _ax ? _aok = 0 : 0), \
     ((z0) > _az ? _aok = 0 : 0), \
     ((z1) < _az ? _aok = 0 : 0), \
     _aok)
/**
 * Sets a Puni's height from the map's floor mode (Punigp->ymode):
 * flat floors, the mri_02 weight floor, the stairs of mri_09, the broken floors of mri_09,
 * mri_12 and mri_17 (until it breaks, the breakable floor raises the Puni 6 units), and the ramps of mri_16.
 * In height modes 2 to 6 it never lets a Puni jump more than 11 units in one frame.
 */
static void puniSetYPosition(PuniData* p) {
    if (p->flags & PUNI_FLAG_PUNI) {
        BOOL _aok;
        f32 _ax, _az;
        switch (Punigp->ymode) {
            case 0:
                break;
            case 1:
                p->pos.y = 0.0f;
                if (IN_RECT_XZ(p, -49.0f, 49.0f, -69.0f, 29.0f)) {
                    p->pos.y = getMri02Yuka();
                }
                break;
            case 2:
                _ymode_mri_09(p);
                if (!swGet(GSWF_ID(GSWF_MRI_09_BREAKING_FLOOR_BROKEN))) {
                    if (IN_RECT_XZ(p, -35.0f, 35.0f, 40.0f, 110.0f)) {
                        p->pos.y = 6.0f;
                    }
                }
                break;
            case 3:
                p->pos.y = 0.0f;
                if (!swGet(GSWF_ID(GSWF_MRI_12_BREAKING_FLOOR_BROKEN))) {
                    if (IN_RECT_XZ(p, -80.0f, -10.0f, -99.0f, -29.0f)) {
                        p->pos.y = 6.0f;
                    }
                }
                break;
            case 4:
                _ymode_mri_16_00(p);
                break;
            case 5:
                _ymode_mri_16_01(p);
                break;
            case 6:
                p->pos.y = 0.0f;
                if (!swGet(GSWF_ID(GSWF_MRI_17_BREAKING_FLOOR_BROKEN))) {
                    if (IN_RECT_XZ(p, -35.0f, 35.0f, -70.0f, 0.0f)) {
                        p->pos.y = 6.0f;
                    }
                }
                break;
        }
        switch (Punigp->ymode) {
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                if (11.0 < __fabs(p->prevPos.y - p->pos.y)) {
                    p->pos.y = p->prevPos.y;
                }
                break;
        }
    }
}

/**
 * Cell map of the mri_06 maze (13 by 5 cells; used while a Puni's y is between -1 and 50) that the Puni movement
 * code checks to keep the Punies inside the maze.
 */
static u8 puniMapdata_MRI_06[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/** Cell map of mri_09 (7 by 6 cells; used while a Puni's y is between -1 and 150) while its wall panels are in place. */
static u8 puniMapdata_MRI_09[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/** Cell map of mri_09 after the wall panels have been rolled away. */
static u8 puniMapdata_MRI_09_01[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/** Cell map of mri_16 that the Puni movement code checks. */
static u8 puniMapdata_MRI_16[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/** Converts a world position in mri_06 into a maze block (column bx 1 to 11, row bz 1 to 3); -1 outside the room. */
static void puniMapdata_MRI_06_getidx(f32 x, f32 z, s32* bx, s32* bz) {
    if (x <= -420.0) {
        *bx = -1;
    } else if (x <= -350.0) {
        *bx = 1;
    } else if (x <= -250.0) {
        *bx = 2;
    } else if (x <= -200.0) {
        *bx = 3;
    } else if (x <= -100.0) {
        *bx = 4;
    } else if (x <= -50.0) {
        *bx = 5;
    } else if (x <= 50.0) {
        *bx = 6;
    } else if (x <= 100.0) {
        *bx = 7;
    } else if (x <= 200.0) {
        *bx = 8;
    } else if (x <= 250.0) {
        *bx = 9;
    } else if (x <= 350.0) {
        *bx = 10;
    } else if (x <= 400.0) {
        *bx = 11;
    } else {
        *bx = -1;
    }
    if (z <= -150.0) {
        *bz = -1;
    } else if (z <= -50.0) {
        *bz = 1;
    } else if (z <= 110.0) {
        *bz = 2;
    } else if (z <= 162.0) {
        *bz = 3;
    } else {
        *bz = -1;
    }
}

/** Gives the centre of the mri_06 maze block (bx, bz) as a world x and z; -1000 for an invalid block. */
static void puniMapdata_MRI_06_getpos(s32 bx, s32 bz, f32* x, f32* z) {
    switch (bx) {
        case 1:
            *x = -375.0f;
            break;
        case 2:
            *x = -300.0f;
            break;
        case 3:
            *x = -225.0f;
            break;
        case 4:
            *x = -150.0f;
            break;
        case 5:
            *x = -75.0f;
            break;
        case 6:
            *x = -0.0f;
            break;
        case 7:
            *x = 75.0f;
            break;
        case 8:
            *x = 150.0f;
            break;
        case 9:
            *x = 225.0f;
            break;
        case 10:
            *x = 300.0f;
            break;
        case 11:
            *x = 375.0f;
            break;
        default:
            *x = -1000.0f;
            break;
    }
    switch (bz) {
        case 1:
            *z = -100.0f;
            break;
        case 2:
            *z = 30.0f;
            break;
        case 3:
            *z = 135.0f;
            break;
        default:
            *z = -1000.0f;
            break;
    }
}

/** Converts a world position in mri_09 into a maze block (column bx 1 to 5, row bz 1 to 4); -1 outside the room. */
static void puniMapdata_MRI_09_getidx(f32 x, f32 z, s32* bx, s32* bz) {
    if (x <= -300.0) {
        *bx = -1;
    } else if (x <= -200.0) {
        *bx = 1;
    } else if (x <= -90.0) {
        *bx = 2;
    } else if (x <= 90.0) {
        *bx = 3;
    } else if (x <= 200.0) {
        *bx = 4;
    } else if (x <= 320.0) {
        *bx = 5;
    } else {
        *bx = -1;
    }
    if (z <= -150.0) {
        *bz = -1;
    } else if (z <= -100.0) {
        *bz = 1;
    } else if (z <= 100.0) {
        *bz = 2;
    } else if (z <= 120.0) {
        *bz = 3;
    } else if (z <= 200.0) {
        *bz = 4;
    } else {
        *bz = -1;
    }
}

/** Gives the centre of the mri_09 maze block (bx, bz) as a world x and z; -1000 for an invalid block. */
static void puniMapdata_MRI_09_getpos(s32 bx, s32 bz, f32* x, f32* z) {
    switch (bx) {
        case 1:
            *x = -250.0f;
            break;
        case 2:
            *x = -145.0f;
            break;
        case 3:
            *x = 0.0f;
            break;
        case 4:
            *x = 145.0f;
            break;
        case 5:
            *x = 260.0f;
            break;
        default:
            *x = -1000.0f;
            break;
    }
    switch (bz) {
        case 1:
            *z = -120.0f;
            break;
        case 2:
            *z = 0.0f;
            break;
        case 3:
            *z = 105.0f;
            break;
        case 4:
            *z = 160.0f;
            break;
        default:
            *z = -1000.0f;
            break;
    }
}

/** Converts a world position in mri_16 into a maze block (column bx 1 to 9, row bz 1 to 7); -1 outside the room. */
static void puniMapdata_MRI_16_getidx(f32 x, f32 z, s32* bx, s32* bz) {
    if (x <= -350.0) {
        *bx = -1;
    } else if (x <= -200.0) {
        *bx = 1;
    } else if (x <= -150.0) {
        *bx = 2;
    } else if (x <= -100.0) {
        *bx = 3;
    } else if (x <= -50.0) {
        *bx = 4;
    } else if (x <= 0.0) {
        *bx = 5;
    } else if (x <= 50.0) {
        *bx = 6;
    } else if (x <= 100.0) {
        *bx = 7;
    } else if (x <= 150.0) {
        *bx = 8;
    } else if (x <= 350.0) {
        *bx = 9;
    } else {
        *bx = -1;
    }
    if (z <= -150.0) {
        *bz = -1;
    } else if (z <= -80.0) {
        *bz = 1;
    } else if (z <= -10.0) {
        *bz = 2;
    } else if (z <= 60.0) {
        *bz = 3;
    } else if (z <= 90.0) {
        *bz = 4;
    } else if (z <= 120.0) {
        *bz = 5;
    } else if (z <= 150.0) {
        *bz = 6;
    } else if (z <= 180.0) {
        *bz = 7;
    } else {
        *bz = -1;
    }
}

/** Gives the centre of the mri_16 maze block (bx, bz) as a world x and z; -1000 for an invalid block. */
static void puniMapdata_MRI_16_getpos(s32 bx, s32 bz, f32* x, f32* z) {
    switch (bx) {
        case 1: *x = -220.0f; break;
        case 2: *x = -175.0f; break;
        case 3: *x = -125.0f; break;
        case 4: *x = -75.0f; break;
        case 5: *x = -25.0f; break;
        case 6: *x = 25.0f; break;
        case 7: *x = 75.0f; break;
        case 8: *x = 125.0f; break;
        case 9: *x = 170.0f; break;
        default: *x = -1000.0f; break;
    }
    switch (bz) {
        case 1: *z = -120.0f; break;
        case 2: *z = -45.0f; break;
        case 3: *z = 25.0f; break;
        case 4: *z = 75.0f; break;
        case 5: *z = 105.0f; break;
        case 6: *z = 135.0f; break;
        case 7: *z = 165.0f; break;
        default: *z = -1000.0f; break;
    }
}

/**
 * Maze block of a world position.
 * Map kind 0 (no maze) leaves *bx and *bz unwritten;
 * every caller first checks that a maze is loaded (Punimap->enabled, which only maze maps set).
 * This function has no direct callers: the compiler inlines it into puniMapFindbuff,
 * puniModeParent and puniModeSearch.
 */
static void puniMapPos2Blk(Vec* pos, s32* blk) {
    switch (Punigp->mapKind) {
        case 0:
            break;
        case 1:
            ((void (*)(Vec*, s32*))puniMapdata_MRI_06_getidx)(pos, blk);
            break;
        case 2:
            ((void (*)(Vec*, s32*))puniMapdata_MRI_09_getidx)(pos, blk);
            break;
        case 3:
            ((void (*)(Vec*, s32*))puniMapdata_MRI_16_getidx)(pos, blk);
            break;
    }
}

/** World x/z of the centre of a maze block (the inverse of puniMapPos2Blk); out-of-range blocks give -1000. */
static void puniMapBlk2Pos(s32 bx, s32 bz, f32* x, f32* z) {
    switch (Punigp->mapKind) {
        case 0:
            break;
        case 1:
            puniMapdata_MRI_06_getpos(bx, bz, x, z);
            break;
        case 2:
            switch (bx) {
                case 1: *x = -250.0f; break;
                case 2: *x = -145.0f; break;
                case 3: *x = 0.0f; break;
                case 4: *x = 145.0f; break;
                case 5: *x = 260.0f; break;
                default: *x = -1000.0f; break;
            }
            switch (bz) {
                case 1: *z = -120.0f; break;
                case 2: *z = 0.0f; break;
                case 3: *z = 105.0f; break;
                case 4: *z = 160.0f; break;
                default: *z = -1000.0f; break;
            }
            break;
        case 3:
            puniMapdata_MRI_16_getpos(bx, bz, x, z);
            break;
    }
}

/**
 * Reads the maze entry for block (x, y) from Punimap->data;
 * an entry whose distance byte is 0 (not yet reached) is returned as 0xFFFF.
 */
static u16 puniMapGetInfo(s32 x, s32 y) {
    u16 v = Punimap->data[x + y * Punimap->width];
    if (!(v & 0xFF)) {
        v |= 0xFFFF;
    }
    return v;
}

/** Allocates and clears the maze map record (Punimap) for the current map. */
static void puniMapInit(void) {
    Punimap = _mapAlloc(sizeof(PuniMap));
    memset(Punimap, 0, sizeof(PuniMap));
}

/**
 * Loads a w x h maze grid (u16 per block: low byte = distance, 0xF000 = height step) used
 * while a puni's y is between minY and maxY.
 * Two original slips, harmless in the game: the size is w * h * 4 although the grid has 2-byte cells
 * (puniMapFindbuff copies twice the table, reading past it), and the memcpy below copies src onto itself,
 * so data stays uninitialised until puniMapFindbuff fills it.
 */
static void puniMapDataSet(s32 w, s32 h, void* src, f32 x, f32 z) {
    Punimap->x = x;
    Punimap->z = z;
    if (src == NULL) {
        Punimap->enabled = 0;
        return;
    }
    Punimap->width = w;
    Punimap->height = h;
    Punimap->enabled = 1;
    Punimap->src = src;
    Punimap->inRange = 0;
    Punimap->size = h * (w * 4);
    Punimap->data = _mapAlloc(Punimap->size);
    memcpy(Punimap->src, src, Punimap->size);
}

/**
 * Floods the loaded maze grid with walking distances from the block under pos:
 * copies the clean grid, marks the start block and
 * then repeatedly gives each neighbouring block (within one height step) the distance plus one,
 * until nothing changes. Does nothing when no maze is loaded or pos is outside the maze's height range.
 */
static void puniMapFindbuff(Vec pos) {
    f32 px, pz;
    s32 bx, bz;
    u16* d;
    u16 lvl;
    s32 i;
    s32 idx;
    s32 changed;
    s32 n;
    if (Punimap->enabled) {
        if (Punimap->x < pos.y || Punimap->z > pos.y) {
            Punimap->inRange = 0;
        } else {
            memcpy(Punimap->data, Punimap->src, Punimap->size);
            Punimap->inRange = 1;
            pz = pos.z;
            px = pos.x;
            switch (Punigp->mapKind) {
                case 0:
                    break;
                case 1:
                    puniMapdata_MRI_06_getidx(px, pz, &bx, &bz);
                    break;
                case 2:
                    puniMapdata_MRI_09_getidx(px, pz, &bx, &bz);
                    break;
                case 3:
                    puniMapdata_MRI_16_getidx(px, pz, &bx, &bz);
                    break;
            }
            idx = bx + Punimap->width * bz;
            if (idx < 0) {
                Punimap->inRange = 0;
            }
            if (Punimap->inRange) {
                Punimap->data[idx] |= 1;
                lvl = 1;
                while (1) {
                    u16 l = lvl;
                    changed = 0;
                    n = Punimap->width * Punimap->height;
                    for (i = 0; i < n; i++) {
                        u16 v;
                        d = Punimap->data;
                        v = d[i];
                        if (v != 0xFFFF && l == (v & 0xFFF)) {
                            u16 h = v & 0xF000;
                            u16* k0 = &d[i - Punimap->width];
                            u16* k1 = &d[i + Punimap->width];
                            u16* k2 = &d[i - 1];
                            u16* k3 = &d[i + 1];
#define SPREAD_LEVEL(k)                           \
    if ((*k & 0xFF) == 0 && (*k & 0xFF) < l) {    \
        if (__abs((*k & 0xF000) - h) <= 0x1000) { \
            *k |= (u16)(l + 1);                   \
            changed = 1;                          \
        }                                         \
    }
                            SPREAD_LEVEL(k0)
                            SPREAD_LEVEL(k1)
                            SPREAD_LEVEL(k2)
                            SPREAD_LEVEL(k3)
#undef SPREAD_LEVEL
                        }
                    }
                    if (!changed) break;
                    lvl++;
                }
            }
        }
    }
}

/**
 * Picks the nearest gather target (PUNI_TARGET_FLAG_MASS)
 * and the nearest flee target (PUNI_TARGET_FLAG_AWAY) for a Puni among the ten targets;
 * only targets within 32 units of its height count.
 * Results go to massTarget and awayTarget (-1 for none) with the squared distances.
 */
static void puniTargetSearch(PuniData* p) {
    s32 i;
    PuniTarget* t = Punigp->targets;
    f32 d;

    p->massTarget = -1;
    p->massTargetDistSq = 1000000.0f;
    p->awayTarget = -1;
    p->awayTargetDistSq = 1000000.0f;
    for (i = 0; i < 10; i++, t++) {
        if (!(t->flags & PUNI_TARGET_FLAG_ON)) {
            continue;
        }
        if (32.0 < __fabs(p->pos.y - t->pos.y)) {
            continue;
        }
        if (t->flags & PUNI_TARGET_FLAG_AWAY) {
            d = PSVECSquareDistance(&p->pos, &t->pos);
            if (p->awayTargetDistSq > d) {
                p->awayTarget = i;
                p->awayTargetDistSq = d;
            }
        }
        if (t->flags & PUNI_TARGET_FLAG_MASS) {
            d = PSVECSquareDistance(&p->pos, &t->pos);
            if (p->massTargetDistSq > d) {
                p->massTarget = i;
                p->massTargetDistSq = d;
            }
        }
    }
}

/**
 * Puni position save slots: PUNI_LSW_SLOT_SIZE (7) LSW bytes per puni,
 * from the LSW() script word `base` (puniswGetindex/puniswSetindex)
 * or from LSW 0 (evt_puni_exit, puni_exit_swset):
 * [0] floor number (0: none saved), [1..2] x low/high, [3..4] y low/high, [5..6] z low/high,
 * each a signed 16-bit map coordinate.
 */
void puniswGetindex(s32 base, s32 idx, s32* val, Vec* pos) {
    u8 slotFloor, b1, b2, b3, b4, b5, b6;
    s32 off;
    s32 x;
    s32 y;
    s32 z;

    off = base + idx * PUNI_LSW_SLOT_SIZE;
    slotFloor = _swByteGet(off + EVTDAT_LSW_BASE);
    b1 = _swByteGet(off + 150000001);
    b2 = _swByteGet(off + 150000002);
    b3 = _swByteGet(off + 150000003);
    b4 = _swByteGet(off + 150000004);
    b5 = _swByteGet(off + 150000005);
    b6 = _swByteGet(off + 150000006);

    *val = slotFloor;
    x = b1 + (b2 << 8);
    if (x & 0x8000) {
        x |= 0xFFFF0000;
    }
    y = b3 + (b4 << 8);
    if (y & 0x8000) {
        y |= 0xFFFF0000;
    }
    z = b5 + (b6 << 8);
    if (z & 0x8000) {
        z |= 0xFFFF0000;
    }
    pos->x = x;
    pos->y = y;
    pos->z = z;
}

// Kept: the target needs dead assignments kept in this block; no natural form found (cleanup_notes/mweppc_pilot.md).
#pragma push
#pragma opt_dead_assignments off
/**
 * Writes a Puni's saved position into 7 LSW bytes (floor number, then x, y and z as signed 16-bit values),
 * the inverse of puniswGetindex. These slots are how the Puni crowd is kept when Mario changes maps.
 */
void puniswSetindex(s32 base, s32 idx, s32* val, Vec* pos) {
    u8 yh, xh;
    s32 off;
    s32 x;
    s32 y;
    s32 z;
    off = base + idx * PUNI_LSW_SLOT_SIZE;
    x = pos->x;
    y = pos->y;
    z = pos->z;
    xh = ((x & 0xFF00) >> 8) & 0xFFu;
    yh = (y & 0xFF00) >> 8;
    _swByteSet(off + EVTDAT_LSW_BASE, *val);
    _swByteSet(off + 150000001, x & 0xFF);
    _swByteSet(off + 150000002, xh);
    _swByteSet(off + 150000003, y & 0xFF);
    _swByteSet(off + 150000004, yh);
    _swByteSet(off + 150000005, z & 0xFF);
    _swByteSet(off + 150000006, (z & 0xFF00) >> 8);
}
#pragma pop

/**
 * Reads the trap bit of Puni number idx from the saved-bit area GSW_MRI_PUNI_TRAPPED_BITS into *out (1 if the
 * Puni is caught in a trap); the blocker slots from 103 up always give 0.
 */
void puniswTrapGetindex(s32 idx, s32* out) {
    if (idx >= PUNI_SOLID_FIRST) {
        *out = 0;
    } else {
        *out = (swByteGet(idx / 8 + GSW_MRI_PUNI_TRAPPED_BITS) >> (idx % 8)) & 1;
    }
}

/** Sets or clears the saved trap bit of Puni number idx to *val (bit 0); ignored for the blocker slots from 103 up. */
void puniswTrapSetindex(s32 idx, s32* val) {
    if (idx < PUNI_SOLID_FIRST) {
        s32 byte = idx / 8;
        s32 bit = idx % 8;
        s32 v = swByteGet(byte + GSW_MRI_PUNI_TRAPPED_BITS);
        v &= (1 << bit) ^ 0xFF;
        v |= *val << bit;
        swByteSet(byte + GSW_MRI_PUNI_TRAPPED_BITS, (u8)v);
    }
}

/** Random float between min and max from rand(). */
static f32 randf(f32 min, f32 max) {
    f32 r = (f32)rand() / 32767.0f;
    return r * (max - min) + min;
}

/** Random integer from min to max, both inclusive, from rand(). */
static s32 randi(s32 min, s32 max) {
    s32 r = rand() % (max - min + 1);
    return r + min;
}

/** Seeds the Puni fight's own random generator (Punigp->seed0 and seed1). */
static void qrndseed(s32 seed) {
    Punigp->seed0 = seed + 0x3A3B01F9;
    Punigp->seed1 = 0x21708331;
}

/**
 * Advances the Puni fight's own random generator and returns a number from 0 to n-1.
 * The Puni battles use this instead of rand().
 */
static s32 qrnd(s32 n) {
    s32 v;
    Punigp->seed0 += Punigp->seed1 + 0x1733AF91;
    Punigp->seed1 += Punigp->seed0 + 0x21273F9C;
    v = Punigp->seed1 < 0 ? -Punigp->seed1 : Punigp->seed1;
    return v % n;
}

/** Deletes the effect attached to a Puni (if any) and clears the pointer. */
static void puniDeleteEffect(PuniData* p) {
    if (p->effect) {
        effDelete(p->effect);
        p->effect = NULL;
    }
}

/**
 * Lowers the depth number of every Puni below p in its follow chain by one, recursively.
 * Used when a Puni leaves the chain.
 */
static void _daime_change(PuniData* p) {
    if (p->child1) {
        p->child1->depth--;
        _daime_change(p->child1);
    }
    if (p->child2) {
        p->child2->depth--;
        _daime_change(p->child2);
    }
}

/**
 * Takes a Puni out of its follow chain ("dokuritsu", independence):
 * clears the group, resets its depth and links its children to its former parent so the chain stays joined.
 */
void PuniDokuritsu(PuniData* p) {
    PuniData* c1;
    PuniData* c2;
    PuniData* par;
    p->groupId = -1;
    p->flags &= ~PUNI_FLAG_IN_CHAIN;
    p->depth = 0;
    c1 = p->child1;
    par = p->parent;
    c2 = p->child2;
    p->parent = NULL;
    p->child1 = NULL;
    p->child2 = NULL;
    if (c1) {
        c1->depth--;
        _daime_change(c1);
        c1->parent = par;
        if (c1->parent) {
            if (par->child1 == p) {
                par->child1 = c1;
            } else if (par->child2 == p) {
                par->child2 = c1;
            }
        }
        c1->child2 = c2;
        if (c1->child2) {
            c1->child2->parent = c1;
        }
    } else if (c2) {
        c2->depth--;
        _daime_change(c2);
        c2->parent = par;
        if (c2->parent) {
            if (par->child1 == p) {
                par->child1 = c2;
            } else if (par->child2 == p) {
                par->child2 = c2;
            }
        }
        c2->child1 = c1;
        if (c2->child1) {
            c2->child1->parent = c2;
        }
    } else if (par) {
        if (par->child1 == p) {
            par->child1 = NULL;
        } else if (par->child2 == p) {
            par->child2 = NULL;
        }
    }
}

/** Walks up the parent links to the head of the follow chain and returns it. */
static PuniData* PuniSakanobori(PuniData* p) {
    while (p->parent != NULL) {
        p = p->parent;
    }
    return p;
}

/**
 * Sends all the Punies behind p in the follow chain astray:
 * each descendant goes to the hagure (lost) mode and is detached from the chain.
 */
static void PuniChildHagure(PuniData *p) {
    if (p->child1 != NULL) {
        PuniData *c = p->child1;
        c->mode = PUNI_MODE_HAGURE;
        p->modeStep = 0;
        PuniChildHagure(c);
        PuniDokuritsu(c);
    }
    if (p->child2 != NULL) {
        PuniData *c = p->child2;
        c->mode = PUNI_MODE_HAGURE;
        p->modeStep = 0;
        PuniChildHagure(c);
        PuniDokuritsu(c);
    }
}

/**
 * Finds a place in a follow chain for p: the chain member with the lowest depth that has a free child slot (at
 * most two children, the second only below depth 2) and makes p its child.
 * Returns FALSE when no chain member has a free slot (p's parent is then left unchanged).
 */
BOOL searchParent(PuniData* p) {
    s32 level = 10000;
    PuniData* found;
    PuniData* q = Punidat;
    s32 i;

    for (i = 0; i < PUNI_DATA_MAX; i++, q++) {
        if ((q->flags & PUNI_FLAG_IN_CHAIN) && q->child2 == NULL && (q->child1 == NULL || q->depth < 2) && q != p &&
            (s16)level > q->depth) {
            found = q;
            level = q->depth + 1;
        }
    }
    if ((s16)level == 10000) {
        return FALSE;
    }
    p->parent = found;
    p->depth = level;
    p->groupId = found->groupId;
    if (found->child1 == NULL) {
        found->child1 = p;
    } else {
        found->child2 = p;
    }
    return TRUE;
}

/** TRUE when Mario is within 150 units of the Puni, the range at which it joins him. */
static BOOL searchMario(PuniData* p) {
    return p->marioDist < 150.0f;
}

/** Straight-down direction used to find the floor under a falling Puni. */
static const Vec puni_fall_dir = {0.0f, -1.0f, 0.0f};

/**
 * Checks that a Puni has not been pushed out of the map:
 * returns 1 if its height is outside both height bands (it has left the floor heights),
 * 2 if it is inside a band but there is no floor within 3 units below it,
 * and 0 if it is in a band with a floor beneath (all is well); callers put it back for 1 and 2.
 */
static s32 puniPositionCheck(PuniData* p) {
    f32* r = &Punigp->stuckTop0;
    s32 in = 0;
    Vec pos;
    Vec dir;
    Vec hit;
    Vec nrm;
    f32 dist;
    f32 hi, lo;
    hi = r[0];
    lo = r[1];
    if (-1000.0f != hi && p->pos.y < hi && p->pos.y > lo) {
        in = 1;
    }
    hi = r[2];
    lo = r[3];
    if (-1000.0f != hi && p->pos.y < hi && p->pos.y > lo) {
        in = 1;
    }
    if (in == 0) {
        return 1;
    }
    pos = p->pos;
    pos.y += 1.0f;
    dir = puni_fall_dir;
    dist = 3.0f;
    if (hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &dist, &nrm.x, &nrm.y, &nrm.z, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z) == NULL) {
        return 2;
    }
    return 0;
}

/**
 * Puts a Puni that fell out of the map back at the recovery position (Punigp->recoverPos) with a one-unit random
 * jitter, setting its previous position to the new one.
 */
static void puniPositionRecover(PuniData* p) {
    p->pos = Punigp->recoverPos;
    p->pos.x += randf(0.0f, 2.0f) - 1.0f;
    p->pos.z += randf(0.0f, 2.0f) - 1.0f;
    p->prevPos = p->pos;
}

static const Vec puni_fall_dir;

/**
 * Moves a Puni by its velocity with wall collision:
 * casts a ray from start along dir for dist units,
 * and on a hit slides the Puni along the wall (optionally reflecting the velocity),
 * undoing the move if the slid position would cross another wall.
 * Records the walls hit in hitMove and hitCheck and returns 0 if the velocity was reflected, 1 otherwise.
 */
int puniMoveAtari(PuniData* p, Vec start, Vec dir, int reflect, f32 dist) {
    Vec hit;
    Vec n;
    Vec diff;
    Vec old;
    Vec refl;
    f32 hx, hy, hz;
    f32 nx, ny, nz;
    f32 d;
    int ret = 1;
    f32 dp;
    f32 t;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & PUNI_FLAG_WALL_CHECK) {
        p->hitMove = hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &d, &n.x, &n.y, &n.z, start.x, start.y, start.z, dir.x, dir.y, dir.z);
    }
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else {
        old = p->pos;
        *&p->wallNrm = n;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            n.y = 0.0f;
        }
        PSVECNormalize(&n, &n);
        if (reflect) {
            C_VECReflect(&dir, &n, &refl);
            PSVECScale(&refl, &p->vel, dist);
            ret = 0;
        }
        dp = PSVECDotProduct(&dir, &n);
        t = -(dist - d);
        PSVECScale(&n, &n, dp * t + 0.1f);
        PSVECAdd(&p->pos, &n, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            start = old;
            start.y += 1.0f;
            p->hitCheck = hitCheckFilter(NULL, &hx, &hy, &hz, &d, &nx, &ny, &nz, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
    return ret;
}

static const Vec puni_fall_dir;

/* FAKE: hitCheckFilter called through a cast with the three start floats listed first. EABI assigns float and
   pointer args to f1-f6 and r3-r10 independently, so the registers and the call are identical; the reorder makes
   the start loads code-generate before the pointer args, so they read r4 directly (scheduling). */
typedef void* (*PuniHitCheckStartFirst)(f32, f32, f32, void*, f32*, f32*, f32*, f32*, f32*, f32*, f32*, f32, f32, f32);

// Descriptive name (not in the symbol maps); placeholder unk_mri_000095f0, JP mri .text+0x952C. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Move step of the limited-speed Puni (JP and later):
 * like puniMoveAtari but without reflecting the velocity, and on floor 18 a hit simply stops the step.
 */
void puniMoveHitLimited(PuniData* p, Vec start, Vec dir, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec old;
    f32 hitX, b2, b3;
    f32 hitNX, c2, c3;
    f32 d;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    p->hitMove = ((PuniHitCheckStartFirst)hitCheckFilter)(start.x, start.y, start.z, NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, dir.x, dir.y, dir.z);
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else if (Punigp->floor != 18) {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        {
            f32 dp = PSVECDotProduct(&dir, &hit);
            PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            p->hitCheck = hitCheckFilter(NULL, &hitX, &b2, &b3, &d, &hitNX, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
}
#endif

static const Vec puni_fall_dir;

// Descriptive name (not in the symbol maps); placeholder unk_mri_0000980c, JP mri .text+0x9748. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Move step with wall collision that reflects the velocity off the wall (JP and later):
 * the Puni slides along the wall, the velocity is mirrored,
 * and the move is undone if the slid position would cross another wall.
 */
void puniMoveHit(PuniData* p, Vec start, Vec dir, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec refl;
    Vec old;
    f32 hitX, b2, b3;
    f32 hitNX, c2, c3;
    f32 d;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & PUNI_FLAG_WALL_CHECK) {
        p->hitMove = hitCheckFilter(NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, start.x, start.y, start.z, dir.x, dir.y, dir.z);
    }
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        C_VECReflect(&dir, &hit, &refl);
        PSVECScale(&refl, &p->vel, dist);
        {
            f32 dp = PSVECDotProduct(&dir, &hit);
            PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            start = old;
            start.y += 1.0f;
            p->hitCheck = hitCheckFilter(NULL, &hitX, &b2, &b3, &d, &hitNX, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
}
#endif

void puniMoveDecel(PuniData* p) {
    Vec start;
    Vec dir;
    Vec s2;
    Vec d2;
    Vec old;
    Vec refl;
    Vec diff;
    Vec hit;
    Vec pa;
    f32 d;
    f32 c3, c2, c1;
    f32 b3, b2, b1;
    f32 dist = PSVECMag(&p->vel);
    if (!dist) {
        return;
    }
    if (dist > 1.0f) {
        if (p->vel.x < 0.0f) {
            p->dir = 270.0f;
        } else {
            p->dir = 90.0f;
        }
    }
    if (dist > p->maxSpeed) {
        PSVECScale(&p->vel, &p->vel, p->maxSpeed / dist);
        dist = p->maxSpeed;
    }
    start = p->pos;
    start.y += 1.0f;
    PSVECNormalize(&p->vel, &dir);
#if VERSION <= VERSION_DEMO_JP
    if (puniMoveAtari(p, start, dir, 1, dist)) {
        PSVECScale(&p->vel, &p->vel, 0.6f);
#else
    s2 = start;
    d2 = dir;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & PUNI_FLAG_WALL_CHECK) {
        p->hitMove = hitCheckFilter(NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, s2.x, s2.y, s2.z, d2.x, d2.y, d2.z);
    }
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        C_VECReflect(&d2, &hit, &refl);
        PSVECScale(&refl, &p->vel, dist);
        {
            f32 dot = PSVECDotProduct(&d2, &hit);
            PSVECScale(&hit, &hit, -(dist - d) * dot + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            p->hitCheck = hitCheckFilter(NULL, &b1, &b2, &b3, &d, &c1, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
#endif
    }
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
    } else {
#if VERSION >= VERSION_JP
        PSVECScale(&p->vel, &p->vel, 0.6f);
#endif
        p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
    }
}

#if VERSION >= VERSION_JP
void puniMoveLimitedNormal(PuniData* p) {
    Vec start;
    Vec dir;
    Vec s2;
    Vec d2;
    Vec old;
    Vec diff;
    Vec hit;
    Vec pa;
    f32 d;
    f32 c3, c2, c1;
    f32 b3, b2, b1;
    f32 dist = PSVECMag(&p->vel);
    if (!dist) {
        return;
    }
    if (dist > p->maxSpeed) {
        PSVECScale(&p->vel, &p->vel, p->maxSpeed / dist);
        dist = p->maxSpeed;
    }
    start = p->pos;
    start.y += 1.0f;
    PSVECNormalize(&p->vel, &dir);
    d2 = dir;
    s2 = start;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    p->hitMove = hitCheckFilter(NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, s2.x, s2.y, s2.z, d2.x, d2.y, d2.z);
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else if (Punigp->floor != 0x12) {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        {
            f32 dp = PSVECDotProduct(&d2, &hit);
            PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            p->hitCheck = hitCheckFilter(NULL, &b1, &b2, &b3, &d, &c1, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
    } else {
        p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
    }
}
#endif

#if VERSION >= VERSION_JP
void puniMoveNormal(PuniData* p) {
    Vec start;
    Vec dir;
    Vec s2;
    Vec d2;
    Vec old;
    Vec refl;
    Vec diff;
    Vec hit;
    Vec pa;
    f32 d;
    f32 c3, c2, c1;
    f32 b3, b2, b1;
    f32 dist = PSVECMag(&p->vel);
    if (!dist) {
        return;
    }
    start = p->pos;
    start.y += 1.0f;
    PSVECNormalize(&p->vel, &dir);
    s2 = start;
    d2 = dir;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & PUNI_FLAG_WALL_CHECK) {
        p->hitMove = hitCheckFilter(NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, s2.x, s2.y, s2.z, d2.x, d2.y, d2.z);
    }
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        C_VECReflect(&d2, &hit, &refl);
        PSVECScale(&refl, &p->vel, dist);
        {
            f32 dp = PSVECDotProduct(&d2, &hit);
            PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            p->hitCheck = hitCheckFilter(NULL, &b1, &b2, &b3, &d, &c1, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
    } else {
        p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
    }
}
#endif

/**
 * The early demos' two move steps (up to the JP demo): both move through puniMoveAtari with the velocity reflected off walls;
 * the limited one first caps the speed at maxSpeed.
 */
#if VERSION <= VERSION_DEMO_JP
static void puniMoveLimitedNormal(PuniData* p) {
    Vec start;
    Vec dir;
    f32 dist = PSVECMag(&p->vel);
    if (!dist) {
        return;
    }
    if (dist > p->maxSpeed) {
        PSVECScale(&p->vel, &p->vel, p->maxSpeed / dist);
        dist = p->maxSpeed;
    }
    start = p->pos;
    start.y += 1.0f;
    PSVECNormalize(&p->vel, &dir);
    puniMoveAtari(p, start, dir, 1, dist);
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
    } else {
        p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
    }
}
static void puniMoveNormal(PuniData* p) {
    Vec start;
    Vec dir;
    f32 dist = PSVECMag(&p->vel);
    if (!dist) {
        return;
    }
    start = p->pos;
    start.y += 1.0f;
    PSVECNormalize(&p->vel, &dir);
    puniMoveAtari(p, start, dir, 1, dist);
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
    } else {
        p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
    }
}
#endif
/** Moves a Puni by its velocity with no collision at all. */
static void puniMoveSimple(PuniData* p) {
    if (PSVECMag(&p->vel)) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    }
}

/** Calls the Puni's own move function (moveFunc), if it has one. */
static void puniMoveCustom(PuniData* p) {
    if (p->moveFunc) {
        p->moveFunc(p);
    }
}

/**
 * Resolves one fight exchange between Puni a and Puni b:
 * each side's hit chance (hitRate, or hitRateGuard against an escaping opponent) is rolled with qrnd;
 * the loser gets damage (the winner's attack minus its defense), loses its velocity,
 * is flagged for knockback and goes to PUNI_MODE_DAMAGE. A dizzy Jabbi always loses.
 * Both are marked attacked for this frame.
 */
static void puniParaAttack(PuniData* a, PuniData* b) {
    s16 x, y, r;
    s32 c;
    if (b->attacked == 0 && b->mode == PUNI_MODE_DAMAGE) {
        return;
    }
    if (b->mode != PUNI_MODE_ESCAPE) {
        x = a->hitRate;
        y = a->hitRate + b->hitRate;
    } else {
        x = a->hitRateGuard;
        y = a->hitRateGuard + (100 - b->hitRateGuard);
    }
    r = qrnd(y);
    c = r < x;
    if (b->flags & PUNI_FLAG_DIZZY) {
        c = 1;
    }
    if (c) {
        if (a->attacked == 0) {
            b->damage = a->attack - b->defense;
            {
                Vec paraVecA = { 0.0f, 0.0f, 0.0f };
                b->vel = paraVecA;
            }
            b->target = a;
            b->flags |= PUNI_FLAG_KNOCKED_BACK_UNUSED;
            b->flags &= ~PUNI_FLAG_DIZZY;
            b->flags2 &= ~PUNI_FLAG2_DIZZY_ICON;
            b->mode = PUNI_MODE_DAMAGE;
            b->modeStep = 0;
            b->flags &= ~PUNI_FLAG_UPDATED;
        }
    } else if (b->attacked == 0) {
        a->damage = b->attack - a->defense;
        {
            Vec paraVecB = { 0.0f, 0.0f, 0.0f };
            a->vel = paraVecB;
        }
        a->target = b;
        a->flags |= PUNI_FLAG_KNOCKED_BACK_UNUSED;
        a->flags &= ~PUNI_FLAG_DIZZY;
        a->flags2 &= ~PUNI_FLAG2_DIZZY_ICON;
        a->mode = PUNI_MODE_DAMAGE;
        a->modeStep = 0;
        a->flags &= ~PUNI_FLAG_UPDATED;
    }
    a->attacked = 1;
    b->attacked = 1;
}

/** Applies the pending damage to a Puni's hp; at 0 it is flagged defeated and killed. */
static void puniParaDamage(PuniData* p) {
    p->hp -= p->damage;
    if (p->hp <= 0) {
        p->hp = 0;
        p->flags |= PUNI_FLAG_DEAD;
        p->flags2 |= PUNI_FLAG2_DEATH_PENDING;
    }
}

/**
 * Regenerates a resting Puni: while it moves, is blown or dizzy the regen timer is reset;
 * otherwise every regenInterval frames it recovers regen hp and gets the healed marker.
 */
static void puniParaRecover(PuniData* p) {
    static PuniData* debug;
    s16 old;

    debug = p;
    if (p->moveDist > 0.001f || p->mode == PUNI_MODE_WIND || p->mode == PUNI_MODE_PIYORI) {
        p->regenTimer = p->regenInterval;
        return;
    }
    p->regenTimer--;
    if (p->regenTimer == 0) {
        if (p->hp < p->maxHp) {
            old = p->hp;
            p->hp = p->hp + p->regen;
            if (p->hp > p->maxHp) {
                p->hp = p->maxHp;
            }
            if (p->hp > old) {
                p->flags2 |= PUNI_FLAG2_HEALED;
                p->flag1Timer = 0;
            }
        }
        p->regenTimer = p->regenInterval;
    }
}

/** TRUE when the Puni is at 30 percent hp or less. */
static BOOL puniParaDangerCheck(PuniData* p) {
    return p->hp * 100 / p->maxHp <= 30;
}

/** TRUE when the Puni is at 80 percent hp or more. */
static BOOL puniParaSelfCheck(PuniData* p) {
    return p->hp * 100 / p->maxHp >= 80;
}

/** TRUE when the Puni is at full hp. */
static BOOL puniParaRecoverCheck(PuniData* p) {
    return p->hp >= p->maxHp;
}

// FAKE: qrnd with the abs as `v = seed1; if (v < 0) v = -seed1;` (this numbers the seed1 load web after the
// start + count temp, so start + count is coloured first and takes r7): gives the target registers here, but
// the standalone qrnd only matches with the ternary form, so puniSearchRandom gets its own copy. Same values.
static inline s32 qrnd_abs_reload(s32 n) {
    s32 v;
    Punigp->seed0 += Punigp->seed1 + 0x1733AF91;
    Punigp->seed1 += Punigp->seed0 + 0x21273F9C;
    v = Punigp->seed1;
    if (v < 0) {
        v = -Punigp->seed1;
    }
    return v % n;
}

/**
 * Returns a random living, active Puni among count slots starting at start (starting point from qrnd),
 * or NULL when none is left.
 */
static PuniData* puniSearchRandom(s32 start, s32 count) {
    s32 idx = qrnd_abs_reload(count - 1) + start;
    s32 i = 0;
    PuniData* p;

    while (TRUE) {
        if (i == count) {
            return NULL;
        }
        if (idx == start + count) {
            idx = start;
        }
        p = &Punidat[idx];
        if ((p->flags & PUNI_FLAG_ACTIVE) && !(p->flags & PUNI_FLAG_DEAD)) {
            break;
        }
        idx++;
        i++;
    }
    return p;
}

/** Random living Puni (slots 1 to 102). */
static PuniData* puniSearchRandom_Mikata(void) {
    return puniSearchRandom(1, PUNI_ALLY_LAST);
}

/** Random living Jabbi (slots 110 to 209). */
static PuniData* puniSearchRandom_Teki(void) {
    return puniSearchRandom(PUNI_ENEMY_FIRST, 100);
}

/**
 * Nearest active, living fighter among count slots from start that is not already knocked back or blown away;
 * returns it only if its squared distance to self is under maxDist, else NULL.
 */
static PuniData* searchBattle(PuniData* self, s32 start, s32 count, f32 maxDist) {
    s32 i;
    PuniData* q;
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;

    q = &Punidat[start];
    for (i = start; i < start + count; i++, q++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != PUNI_MODE_DAMAGE && q->mode != PUNI_MODE_WIND) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
    }
    if (minDist < maxDist) {
        return found;
    }
    return NULL;
}

/**
 * Nearest living Puni (slots 1 to 102) within 50 units (squared distance under 2500) of self that is not knocked
 * back or blown away, or NULL.
 */
static PuniData* searchMikata(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[1];
    s32 i;

    for (i = 1; i < PUNI_SOLID_FIRST; i++, q++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != PUNI_MODE_DAMAGE && q->mode != PUNI_MODE_WIND) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
    }
    if (minDist < 2500.0f) {
        return found;
    }
    return NULL;
}

/**
 * Nearest living Jabbi within 50 units (squared distance under 2500) of self that is not knocked back
 * or blown away, or NULL.
 */
static PuniData* searchTeki(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[PUNI_ENEMY_FIRST];
    s32 i;

    for (i = PUNI_ENEMY_FIRST; i < PUNI_ENEMY_END; i++, q++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != 9 && q->mode != 13) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
    }
    if (minDist < 2500.0f) {
        return found;
    }
    return NULL;
}

/** Nearest living Puni to self at any distance under 1000 units (squared 1000000), or NULL. */
static PuniData* searchBattleMikata(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[1];
    s32 i;

    for (i = 1; i < PUNI_SOLID_FIRST; i++, q++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != PUNI_MODE_DAMAGE && q->mode != PUNI_MODE_WIND) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
    }
    if (minDist < 1000000.0f) {
        return found;
    }
    return NULL;
}

/** Inline version of "nearest living Jabbi within 1000 units", used by puniSearch. */
static inline PuniData* searchBattleTeki_inl(PuniData* self) {
    s32 i;
    PuniData* q;
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* ret;

    q = &Punidat[PUNI_ENEMY_FIRST];
    for (i = PUNI_ENEMY_FIRST; i < PUNI_ENEMY_END; i++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != PUNI_MODE_DAMAGE && q->mode != PUNI_MODE_WIND) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
        q++;
    }
    if (minDist < 1000000.0f) {
        ret = found;
    } else {
        ret = NULL;
    }
    return ret;
}

/** Inline version of "nearest living Puni within 1000 units", used by puniSearch. */
static inline PuniData* searchBattleMikata_inl(PuniData* self) {
    s32 i;
    PuniData* q;
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* ret;

    q = &Punidat[1];
    for (i = 1; i < PUNI_SOLID_FIRST; i++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != PUNI_MODE_DAMAGE && q->mode != PUNI_MODE_WIND) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
        q++;
    }
    if (minDist < 1000000.0f) {
        ret = found;
    } else {
        ret = NULL;
    }
    return ret;
}

/** Nearest living Jabbi to self at any distance under 1000 units, or NULL. */
static PuniData* searchBattleTeki(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[PUNI_ENEMY_FIRST];
    s32 i;

    for (i = PUNI_ENEMY_FIRST; i < PUNI_ENEMY_END; i++, q++) {
        if ((q->flags & PUNI_FLAG_ACTIVE) && !(q->flags & PUNI_FLAG_DEAD) && q->mode != 9 && q->mode != 13) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
    }
    if (minDist < 1000000.0f) {
        return found;
    }
    return NULL;
}

/**
 * Every fourth frame (staggered by the Puni's index) picks the nearest living opponent as the Puni's target:
 * a Jabbi for a Puni, a Puni for a Jabbi.
 */
static void puniSearch(PuniData* p) {
    if ((s32)(gp->retraceCount & 3) == (*( s16*)( &p->index) & 3)) {
        if (p->flags & PUNI_FLAG_PUNI) {
            p->target = searchBattleTeki_inl(p);
        } else {
            p->target = searchBattleMikata_inl(p);
        }
    }
}

/**
 * Battle start (allMode 3): every idle, joining,
 * following or scripted Puni or Jabbi that has an opponent in range leaves its chain
 * and switches to PUNI_MODE_ATTACK against it.
 */
static void puniModeChangeAttack(void) {
    s32 i;
    PuniData* p = &Punidat[1];
    PuniData* f;
    for (i = 1; i < PUNI_DATA_MAX; i++, p++) {
        if (p->flags & PUNI_FLAG_ACTIVE) {
            switch (p->mode) {
                case PUNI_MODE_IDLE:
                case PUNI_MODE_JOIN:
                case 2:
                case 0x1d:
                    if (p->flags & PUNI_FLAG_PUNI) {
                        f = searchBattleTeki_inl(p);
                    } else {
                        f = searchBattleMikata_inl(p);
                    }
                    if (f) {
                        p->target = f;
                        p->mode = PUNI_MODE_ATTACK;
                        p->modeStep = 0;
                        p->depth = 0;
                        p->flags |= PUNI_FLAG_COLLIDE;
                        PuniDokuritsu(p);
                        if (p->effect) {
                            effDelete(p->effect);
                            p->effect = NULL;
                        }
                    }
                    break;
            }
        }
    }
}

/**
 * Flee start (allMode 2): when a flee target is near (Pider spider), every idle, joining, following,
 * gathering or lost Puni leaves its chain and switches to the away-jump.
 */
static void puniModeChangeAway(void) {
    s32 i;
    PuniData* p = &Punidat[1];

    for (i = 1; i <= PUNI_ALLY_LAST; i++, p++) {
        if (!(p->flags & PUNI_FLAG_ACTIVE)) {
            continue;
        }
        if (1000000.0f == p->awayTargetDistSq) {
            continue;
        }
        switch (p->mode) {
            case PUNI_MODE_IDLE:
            case PUNI_MODE_JOIN:
            case 2:
            case PUNI_MODE_MASS:
            case PUNI_MODE_HAGURE:
                PuniDokuritsu(p);
                p->prevMode = p->mode;
                p->mode = PUNI_MODE_AWAYJUMP;
                p->modeStep = 0;
                if (p->effect != NULL) {
                    effDelete(p->effect);
                    p->effect = NULL;
                }
                break;
        }
    }
}

/** Direction straight down, used when checking whether a Puni has ground under it. */
static const Vec puni_fallchk_dir = {0.0f, -1.0f, 0.0f};

/**
 * Puni (slots 1 to 102) that have no floor within 2 units under them leave their chain,
 * lose their velocity and switch to PUNI_MODE_FALL.
 */
static void puniModeChangeFallCheck(void) {
    Vec dir;
    Vec pos;
    Vec nrm;
    Vec hit;
    f32 dist;
    s32 i;
    PuniData* p = &Punidat[1];

    for (i = 1; i <= PUNI_ALLY_LAST; i++, p++) {
        if (!(p->flags & PUNI_FLAG_ACTIVE)) {
            continue;
        }
        pos = p->pos;
        pos.y = 1.0f + pos.y;
        dir = puni_fallchk_dir;
        dist = 2.0f;
        if (hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &dist, &nrm.x, &nrm.y, &nrm.z, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z) ==
            NULL) {
            PuniDokuritsu(p);
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                p->vel = v;
            }
            p->mode = PUNI_MODE_FALL;
            p->modeStep = 0;
            if (p->effect != NULL) {
                effDelete(p->effect);
                p->effect = NULL;
            }
        }
    }
}

/**
 * Re-collects lost Punies: every Puni in the hagure (lost) mode that is within 30 units of self
 * and at about its height joins again, recursively picking up Punies near those.
 */
static void puniFukki(PuniData* self) {
    s32 i;
    PuniData* p = &Punidat[1];

    for (i = 1; i <= PUNI_ALLY_LAST; i++, p++) {
        if ((p->flags & PUNI_FLAG_ACTIVE) && p->mode == PUNI_MODE_HAGURE) {
            if (!(32.0 < __fabs(self->pos.y - p->pos.y)) && PSVECSquareDistance(&self->pos, &p->pos) < 900.0f) {
                p->mode = PUNI_MODE_JOIN;
                puniFukki(p);
                puniDeleteEffect(p);
            }
        }
    }
}

/** Runs the group commands requested through Punigp->allMode: 2 starts the flee, 3 starts the battle. */
static void puniModeALLCheck(void) {
    switch (Punigp->allMode) {
        case 2:
            puniModeChangeAway();
            break;
        case 3:
            puniModeChangeAttack();
            break;
        case 4:
        default:
            break;
    }
}

/** Ends the flee command after one frame and remembers the current allMode for the next frame. */
static void puniModeALLEndCheck(void) {
    switch (Punigp->allMode) {
        case 1:
            break;
        case 2:
            if (Punigp->prevAllMode == 2) {
                Punigp->allMode = 0;
            }
            break;
        case 3:
            break;
    }
    Punigp->prevAllMode = Punigp->allMode;
}

/**
 * Idle mode: a Puni blown by Flurrie's breath (breath power over 10) goes to the wind mode;
 * one with Mario within 150 units joins him.
 */
static void puniModeNormal(PuniData* p) {
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
    } else if (searchMario(p)) {
        p->mode = PUNI_MODE_JOIN;
    }
}

/**
 * Joining mode: finds a place in the follow chain (searchParent),
 * turns into a follower of that parent and starts with the walking animation.
 */
static void puniModeParentpre(PuniData* p) {
    Vec v;

    searchParent(p);
    p->mode = PUNI_MODE_PARENT;
    p->flags |= (PUNI_FLAG_IN_CHAIN | PUNI_FLAG_COLLIDE);
    PSVECSubtract(&p->parent->pos, &p->pos, &v);
    p->prevParentDist = p->parentDist = PSVECMag(&v);
    p->timer = 0;
    p->modeStep = 0;
    p->anim = 1;
}

/**
 * Follower mode: follows its parent Puni, choosing the stand
 * or walk animation from its speed (with a short hold so it does not flicker).
 * If it stays out of reach (more than 80 units or 32 units of height from its parent) for 30 frames it is lost
 * and goes to the hagure mode.
 * It switches to the wind, flee, gather or maze-search modes when those apply and
 * otherwise steps toward its parent when too far.
 */
static void puniModeParent(PuniData* p) {
    Vec v;
    s32 bx, bz;
    s32 flag;
    u16 info;
    switch (p->prevAnim) {
        case 0:
        case 1:
            if (p->moveDist < 1.5f) {
                p->anim = 0;
            } else {
                p->anim = 1;
            }
            p->animHoldCount = 0;
            break;
        default:
            if (p->moveDist < 1.5f) {
                p->anim = 0;
            } else {
                p->anim = 1;
            }
            if (p->prevAnim == p->anim) {
                p->animHoldCount = 0;
            } else if (++p->animHoldCount <= 8) {
                p->anim = p->prevAnim;
            } else {
                p->animHoldCount = 0;
            }
            break;
    }
    if (p->flags & PUNI_FLAG_MOVE_STOP) {
        return;
    }
    flag = 0;
    PSVECSubtract(&p->parent->pos, &p->pos, &v);
    p->parentDist = PSVECMag(&v);
    if (!(bero_get_BeroEXEC() & BERO_EXEC_BUSY)) {
        if (32.0 < __fabs(p->pos.y - p->parent->pos.y)) {
            flag = 1;
        } else if (p->parentDist > 80.0f) {
            flag = 1;
        } else {
            p->timer = 0;
        }
    }
    if (0.1f + p->parentDist <= p->prevParentDist) {
        flag = 0;
        p->timer = 0;
    }
    if (Punimap->inRange != 0) {
        flag = 0;
    }
    if (flag) {
        if (++p->timer > 30) {
            p->mode = PUNI_MODE_HAGURE;
            p->modeStep = 0;
            PuniChildHagure(p);
            PuniDokuritsu(p);
            return;
        }
    }
    p->prevParentDist = p->parentDist;
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        return;
    }
    if (p->awayTarget != -1 && p->awayTargetDistSq < 10000.0f) {
        Punigp->allMode = 2;
        return;
    }
    if (p->massTarget != -1) {
        PuniDokuritsu(p);
        p->mode = PUNI_MODE_MASS;
        p->modeStep = 0;
        return;
    }
    if (Punimap->enabled != 0 && Punimap->inRange != 0) {
        f32 z = p->pos.z;
        f32 x = p->pos.x;
        switch (Punigp->mapKind) {
            case 0:
                break;
            case 1:
                puniMapdata_MRI_06_getidx(x, z, &bx, &bz);
                break;
            case 2:
                puniMapdata_MRI_09_getidx(x, z, &bx, &bz);
                break;
            case 3:
                puniMapdata_MRI_16_getidx(x, z, &bx, &bz);
                break;
        }
        info = puniMapGetInfo(bx, bz);
        if (info != 0xFFFF && (info & 0xFFF) > 2) {
            PuniDokuritsu(p);
            p->mode = PUNI_MODE_SEARCH;
            p->modeStep = 0;
            return;
        }
    }
    if (p->parentDist > p->radius.x + p->parent->radius.x + p->radius.y + p->radius.z) {
        PSVECNormalize(&v, &v);
        v.y = 0.0f;
        PSVECScale(&v, &v, 1.0f);
        PSVECAdd(&p->vel, &v, &p->vel);
        p->maxSpeed = 3.0f;
    }
}

#define PUNI_MAPDATA_GETPOS(a, b)                                                          \
    { s32 _pa = (b); s32 _pb = (a); switch (Punigp->mapKind) {                             \
            case 0: break; case 1: puniMapdata_MRI_06_getpos(_pa, _pb, &tgx, &tgz); break; \
            case 2: puniMapdata_MRI_09_getpos(_pa, _pb, &tgx, &tgz); break;                \
            case 3: puniMapdata_MRI_16_getpos(_pa, _pb, &tgx, &tgz); break;                \
        } }

/**
 * Maze-search mode: a Puni that is too far from Mario on a maze map follows the downhill distance grid (the one
 * puniMapFindbuff flooded from Mario) block by block toward Mario,
 * preferring the direction given by bits of the block entry, and rejoins once within two blocks.
 * A Puni with no way back becomes lost.
 */
static void puniModeSearch(PuniData* p) {
    static u16 takasa;
    static u16 move;
    static u16 find;
    static u16 m0;
    static u16 m1;
    static u16 m2;
    static u16 m3;
    static u16 h0;
    static u16 h1;
    static u16 h2;
    static u16 h3;
    f32 px, pz;
    s32 bx;
    s32 bz;
    f32 tgx, tgz;
    Vec d;
    u16 c, cl, cu, cr, cd;
    s32 k;

    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        return;
    }
    if (Punimap->inRange == 0 && p->marioDist == 1000.0f) {
        p->mode = PUNI_MODE_HAGURE;
        p->anim = 0;
        p->modeStep = 0;
    }
    if (Punimap->enabled == 0) {
        return;
    }
    if (Punimap->inRange == 0) {
        return;
    }
    pz = p->pos.z;
    px = p->pos.x;
    switch (Punigp->mapKind) {
        case 0:
            break;
        case 1:
            puniMapdata_MRI_06_getidx(px, pz, &bx, &bz);
            break;
        case 2:
            puniMapdata_MRI_09_getidx(px, pz, &bx, &bz);
            break;
        case 3:
            puniMapdata_MRI_16_getidx(px, pz, &bx, &bz);
            break;
    }
    c = Punimap->data[bz * Punimap->width + bx];
    if (!(c & 0xFF)) {
        c |= 0xFFFF;
    }
    k = c & 0xFF;
    if (k <= 2) {
        p->mode = PUNI_MODE_JOIN;
        p->flags |= PUNI_FLAG_COLLIDE;
        return;
    }
    k--;
    takasa = c & 0xF000;
    find = c & 0x0F00;
    move = k;
    k = bz * Punimap->width;
    k = bx + k;
    k--;
    cl = Punimap->data[k];
    if (!(cl & 0xFF)) {
        cl |= 0xFFFF;
    }
    m0 = cl;
    cu = Punimap->data[(bz - 1) * Punimap->width + bx];
    if (!(cu & 0xFF)) {
        cu |= 0xFFFF;
    }
    m1 = cu;
    k = bz * Punimap->width;
    k = bx + k;
    k++;
    cr = Punimap->data[k];
    if (!(cr & 0xFF)) {
        cr |= 0xFFFF;
    }
    m2 = cr;
    cd = Punimap->data[(bz + 1) * Punimap->width + bx];
    if (!(cd & 0xFF)) {
        cd |= 0xFFFF;
    }
    h0 = cl & 0xF000;
    h1 = cu & 0xF000;
    h2 = cr & 0xF000;
    h3 = cd & 0xF000;
    m0 = cl & 0xFF;
    m1 = cu & 0xFF;
    m2 = cr & 0xFF;
    m3 = cd & 0xFF;
    switch (find) {
        case 0x000:
            if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx - 1);
            } else if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz - 1, bx);
            } else if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx + 1);
            } else if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz + 1, bx);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        case 0x100:
            if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz - 1, bx);
            } else if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx + 1);
            } else if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz + 1, bx);
            } else if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx - 1);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        case 0x200:
            if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx + 1);
            } else if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz + 1, bx);
            } else if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx - 1);
            } else if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz - 1, bx);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        case 0x300:
            if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz + 1, bx);
            } else if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx - 1);
            } else if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz - 1, bx);
            } else if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PUNI_MAPDATA_GETPOS(bz, bx + 1);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        default:
            tgx = 0.0f;
            tgz = 0.0f;
            break;
    }
    d.x = tgx;
    d.y = p->pos.y;
    d.z = tgz;
    PSVECSubtract(&d, &p->pos, &d);
    if (d.x != 0.0f || d.y != 0.0f || d.z != 0.0f) {
        PSVECNormalize(&d, &d);
    }
    d.y = 0.0f;
    PSVECScale(&d, &d, 1.0f);
    PSVECAdd(&p->vel, &d, &p->vel);
    p->maxSpeed = 3.0f;
}

/**
 * Gather mode: the Puni walks to a gather target (targets with PUNI_TARGET_FLAG_MASS, e.g. a Puni Orb pedestal),
 * nudging away when too close and jittering sideways on floors 13 and 18 when it stands still.
 */
#if VERSION <= VERSION_DEMO_JP
static void puniModeMass(PuniData* p) {
#else
void puniModeMass(PuniData* p) {
#endif
    Vec v;
    PuniTarget* t = &Punigp->targets[p->massTarget];
    s32 a, b;
    if (p->massTarget == -1) {
        if (p->prevMode == PUNI_MODE_TRAP) {
            p->mode = PUNI_MODE_TRAP;
        } else {
            p->mode = PUNI_MODE_JOIN;
        }
        p->anim = 0;
        p->modeStep = 0;
        return;
    }
    if (p->awayTarget != -1 && p->awayTargetDistSq < 10000.0f) {
        Punigp->allMode = 2;
        return;
    }
    a = gp->retraceCount & 0xF;
#if VERSION <= VERSION_DEMO_JP
    b = p->index & 0xF;
#else
    b = *(s16*)( &p->index ) & 0xF;
    if (Punigp->floor == 18) {
        a = b;
    }
#endif
    if (a == b && p->breathPower > 10.0f) {
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        return;
    }
    if (p->massTargetDistSq > 100.0f) {
        if (p->massTargetDistSq < 3600.0f && (qrnd(3) == 0)) {
            PSVECSubtract(&p->pos, &t->pos, &v);
            PSVECNormalize(&v, &v);
            v.y = 0.0f;
            PSVECScale(&v, &v, 1.5f);
            PSVECAdd(&p->vel, &v, &p->vel);
        } else {
            PSVECSubtract(&t->pos, &p->pos, &v);
            PSVECNormalize(&v, &v);
            v.y = 0.0f;
            PSVECScale(&v, &v, 5.0f);
            PSVECAdd(&p->vel, &v, &p->vel);
        }
    } else {
        Vec zv = {0.0f, 0.0f, 0.0f};
        p->vel = zv;
    }
    switch (p->modeStep) {
        case 0:
            if (p->vel.x < 0.0f) {
                p->dir = 270.0f;
            } else {
                p->dir = 90.0f;
            }
            p->modeStep = 1;
            if (Punigp->floor == 13) {
                p->modeStep = 10;
            }
            if (Punigp->floor == 18) {
                p->modeStep = 10;
            }
            p->modeVar0 = 0;
            p->modeVar1 = 0;
            break;
        case 1:
            break;
        case 10:
            if (p->modeVar0 == 120) {
                p->modeVar0 = 0;
                p->modeVar1 = 0;
                if (rand() % 2) {
                    p->moveStart = (Vec){0.0f, 0.0f, 2.0f};
                } else {
                    p->moveStart = (Vec){0.0f, 0.0f, -2.0f};
                }
                p->modeStep = 11;
            } else if (p->pos.x == p->prevPos.x && p->pos.y == p->prevPos.y) {
                p->modeVar0++;
            }
            break;
        case 11:
            if (p->modeVar1 == 30) {
                p->modeStep = 10;
            } else {
                PSVECAdd(&p->vel, &p->moveStart, &p->vel);
                p->modeVar1++;
            }
            break;
    }
}

/**
 * Flee jump: the Puni hops up (200 ms jump arc with the Puni or little-Jabbi jump sound) and
 * then goes to the away mode.
 */
static void puniModeAwayjump(PuniData* p) {
    u32 t;

    switch (p->modeStep) {
        case 0:
            p->startTime = gp->renderTime;
            p->modeVar1 = 0;
            p->modeF0 = p->pos.y;
            p->modeF1 = -72.0f;
            p->modeF2 = -p->modeF1;
            p->modeStep = 1;
            p->anim = 7;
            p->animTime = 0;
            p->flags |= PUNI_FLAG_OWN_ANIM_TIME;
            if (p->flags & PUNI_FLAG_NPC_CHIBI_TOGE) {
                psndSFXOn_3D("SFX_STG2_TOGETTI_JUMP1", &p->pos);
            } else {
                psndSFXOn_3D("SFX_STG2_PUNI_JUMP1", &p->pos);
            }
        case 1:
            t = OSTicksToMilliseconds((u32)gp->renderTime - p->startTimeLo);
            if ((s32)t > 100 && p->modeVar1 == 0) {
                p->modeVar1 = 1;
                p->anim = 8;
                p->animTime = 0;
            }
            if ((s32)t < 200) {
                f32 f = (f32)(s32)t / 200.0f;
                p->pos.y = f * (p->modeF1 * f + p->modeF2) + p->modeF0;
            } else {
                p->pos.y = p->modeF0;
                p->mode = PUNI_MODE_AWAY;
                p->anim = 2;
                p->flags &= ~PUNI_FLAG_OWN_ANIM_TIME;
                p->modeStep = 0;
            }
            break;
    }
}

/** The zero vector, used when an away target has no position. */
static const Vec puni_away_zero0 = {0.0f, 0.0f, 0.0f};

// Kept: the target calls puniModeAway_sub (REL map: called from puniModeAway); under -inline auto nothing else
// keeps a short function out of line.
#pragma dont_inline on
/**
 * Moves a fleeing Puni along its velocity with wall collision.
 * Returns 1 and sets the wall push direction when it hits a wall with attribute 0x4 (HIT_ATTR_NPC_ONLY),
 * 2 for any other wall (step 10), 0 when nothing was hit.
 */
static int puniModeAway_sub(PuniData* p) {
    Vec dir;
    Vec start;
    f32 mag = PSVECMag(&p->vel);
    start = p->pos;
    start.y += 1.0f;
    if (0.0f != mag) {
        PSVECNormalize(&p->vel, &dir);
    } else {
        dir = puni_away_zero0;
    }
    puniMoveAtari(p, start, dir, 0, mag);
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        if (p->hitMove != NULL && (p->hitMove->attributes & HIT_ATTR_NPC_ONLY)) {
            p->pushDir.x = -p->wallNrm.x;
            p->pushDir.y = -p->wallNrm.y;
            p->pushDir.z = -p->wallNrm.z;
            p->modeStep = 1;
            return 1;
        } else if (p->hitCheck != NULL && (p->hitCheck->attributes & HIT_ATTR_NPC_ONLY)) {
            p->pushDir.x = -p->wallNrm.x;
            p->pushDir.y = -p->wallNrm.y;
            p->pushDir.z = -p->wallNrm.z;
            p->modeStep = 1;
            return 1;
        } else {
            p->modeStep = 10;
            return 2;
        }
    }
    return 0;
}
#pragma dont_inline reset

/**
 * Flee mode: runs from the nearest flee target (a Pider spider), steering away from it with wall avoidance,
 * until the target is gone, then rejoins Mario.
 */
static void puniModeAway(PuniData* p) {
    Vec sub;
    Vec start;
    f32 nz, ny, nx, hz, hy, hx, dist;
    PuniTarget* t;
    s32 idx = p->awayTarget;
    t = &Punigp->targets[idx];
    if (idx == -1) {
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            p->vel = v;
        }
        p->mode = PUNI_MODE_JOIN;
        p->modeStep = 0;
        p->anim = 0;
        return;
    }
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        return;
    }
    switch (p->modeStep) {
        case 0:
            if (p->awayTargetDistSq < 90000.0f) {
                PSVECSubtract(&p->pos, &t->pos, &sub);
                PSVECNormalize(&sub, &sub);
                sub.y = 0.0f;
                PSVECScale(&sub, &sub, 1.4f);
                PSVECAdd(&p->vel, &sub, &p->vel);
                p->maxSpeed = 4.2f;
                puniModeAway_sub(p);
            }
            break;
        case 1:
            p->flags2 |= PUNI_FLAG2_CROSSING_LEDGE;
            PSVECNormalize((Vec*)&p->pushDir.x, &sub);
            PSVECScale(&sub, &p->vel, p->radius.x);
            PSVECAdd(&p->pos, &p->vel, &p->pos);
            start = p->pos;
            start.y = 1.0f + start.y;
            {
                Vec d = {0.0f, -1.0f, 0.0f};
                sub = d;
            }
            dist = 2.0f;
            start.y += 0.2f;
            if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, start.x, start.y, start.z, sub.x, sub.y, sub.z) == NULL) {
                {
                    Vec v = {0.0f, 0.0f, 0.0f};
                    p->vel = v;
                }
                p->mode = PUNI_MODE_FALL;
                p->anim = 0;
                p->modeStep = 0;
            } else {
                p->modeStep = 0;
            }
            break;
        case 10:
            break;
    }
}

/**
 * Attack mode: the fighter walks toward its target (re-chosen with puniSearch every few frames) at speed 3,
 * and when within the target's follow range starts a fight exchange with puniParaAttack.
 * Switches to the wind mode if Flurrie's breath hits it.
 */
static void puniModeAttack(PuniData* p) {
    Vec d;
    switch (p->modeStep) {
        case 0:
            p->anim = 1;
            p->modeStep = 1;
        case 1:
            if (p->breathPower > 10.0f) {
                p->prevMode = p->mode;
                PuniDokuritsu(p);
                p->target = Punigp->marioPuni;
                p->mode = PUNI_MODE_WIND;
                p->modeStep = 0;
                break;
            }
            puniSearch(p);
            if (p->target != NULL) {
                PSVECSubtract(&p->target->pos, &p->pos, &d);
                if (PSVECMag(&d) > p->target->followRange) {
                    PSVECNormalize(&d, &d);
                    d.y = 0.0f;
                    PSVECScale(&d, &d, 1.0f);
                    PSVECAdd(&p->vel, &d, &p->vel);
                    p->maxSpeed = 3.0f;
                } else {
                    puniParaAttack(p, p->target);
                }
            }
            break;
    }
}

/**
 * Knockback mode after losing an exchange: plays the fight sound,
 * is thrown away from the opponent with a small hop (5 units up, falling by 0.5 per frame),
 * then takes the pending damage.
 * A Puni down to 30 percent hp or less flees (escape mode), otherwise it goes back to idle;
 * at 0 hp it is defeated and killed.
 */
static void puniModeDamage(PuniData* p) {
    Vec d;
    switch (p->modeStep) {
        case 0:
            if (Punigp->fightSfxCount1++ < 6) psndSFXOn_3D("SFX_STG2_FIGHT_TOGE1", &p->pos);
            p->modeStep = 1;
            break;
        case 1:
            if (Punigp->fightSfxCount2++ < 6) psndSFXOn_3D("SFX_STG2_FIGHT_TOGE2", &p->pos);
            p->modeStep = 2;
        case 2:
            PSVECSubtract(&p->pos, &p->target->pos, &d);
            if (0.0f == PSVECMag(&d)) {
                {
                    Vec v_ = {0.0f, 0.0f, 0.0f};
                    d = v_;
                }
            } else {
                PSVECNormalize(&d, &d);
            }
            d.y = 0.0f;
            PSVECScale(&d, &d, 3.0f);
            p->vel = d;
            p->accelStep = 5.0f;
            p->anim = 3;
            p->modeStep = 3;
            break;
        case 3:
            p->pos.y += p->accelStep;
            p->accelStep -= 0.5f;
            if (p->pos.y <= 0.001f) {
                p->flags &= ~PUNI_FLAG_KNOCKED_BACK_UNUSED;
                p->pos.y = 0.0f;
                p->accelStep = 0.0f;
                p->prevPos = p->pos;
                p->hp -= p->damage;
                if (p->hp <= 0) {
                    p->hp = 0;
                    p->flags |= PUNI_FLAG_DEAD;
                    p->flags2 |= PUNI_FLAG2_DEATH_PENDING;
                }
                if ((p->hp * 100) / p->maxHp <= 30) {
                    p->mode = PUNI_MODE_ESCAPE;
                    p->anim = 2;
                    p->modeStep = 0;
                } else {
                    p->mode = PUNI_MODE_IDLE;
                    p->anim = 0;
                    p->modeStep = 0;
                }
            }
            break;
    }
}

/**
 * Dizzy mode: a Jabbi blown away by Flurrie's breath
 * while all Punies fight (allMode 3) lies dizzy for 600 frames with the dizzy stars
 * and always loses the next exchange.
 * Afterwards it returns to its earlier mode or to escape mode when its hp is low.
 * A Puni goes straight to the afterwards step.
 */
static void puniModePiyori(PuniData* p) {
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->flags &= ~PUNI_FLAG_DIZZY;
        p->flags2 &= ~PUNI_FLAG2_DIZZY_ICON;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        return;
    }
    switch (p->modeStep) {
        case 0:
            if (p->flags & PUNI_FLAG_TOGE) {
                p->timer = 600;
                p->anim = 4;
                p->flags |= (PUNI_FLAG_COLLIDE | PUNI_FLAG_DIZZY);
                p->flags2 |= PUNI_FLAG2_DIZZY_ICON;
                p->modeStep = 2;
            } else {
                p->modeStep = 10;
            }
            break;
        case 2:
            if (p->timer-- == 0) {
                p->flags &= ~PUNI_FLAG_DIZZY;
                p->flags2 &= ~PUNI_FLAG2_DIZZY_ICON;
                p->modeStep = 10;
            }
            break;
        case 10:
            if (p->prevMode == PUNI_MODE_PIYORI) {
                if (p->hp * 100 / p->maxHp <= 30) {
                    p->mode = PUNI_MODE_ESCAPE;
                    p->anim = 2;
                    p->modeStep = 0;
                } else {
                    p->mode = PUNI_MODE_IDLE;
                    p->anim = 0;
                    p->modeStep = 0;
                }
            } else {
                p->mode = p->prevMode;
#if VERSION >= VERSION_US
                p->anim = 2;
#endif
                if (p->prevMode == PUNI_MODE_PARENT) {
                    p->mode = PUNI_MODE_JOIN;
                }
                p->modeStep = 0;
            }
            break;
    }
}

/**
 * Escape mode: a Puni with low hp runs away from its opponent (speed 2.4),
 * until it is 120 units away (then it rests in recover mode), or until its hp is back to 80 percent (then idle).
 * Flurrie's breath switches it to the wind mode.
 */
static void puniModeEscape(PuniData* p) {
    Vec d;
    f32 dist;

    switch (p->modeStep) {
        case 0:
            break;
        default:
            return;
    }
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        return;
    }
    if (p->hp * 100 / p->maxHp >= 80) {
        p->mode = PUNI_MODE_IDLE;
        p->anim = 0;
        p->modeStep = 0;
        return;
    }
    if ((s32)(gp->retraceCount & 3) == (p->index & 3)) {
        if (p->flags & PUNI_FLAG_PUNI) {
            p->target = searchBattleTeki_inl(p);
        } else {
            p->target = searchBattleMikata_inl(p);
        }
    }
    if (p->target == NULL) return;
    PSVECSubtract(&p->target->pos, &p->pos, &d);
    dist = PSVECMag(&d);
    if (dist >= 120.0f) {
        {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            p->vel = v_;
        }
        p->mode = PUNI_MODE_RECOVER;
        p->anim = 0;
        p->modeStep = 0;
        return;
    }
    if (0.0f == dist) {
        {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            d = v_;
        }
    } else {
        PSVECNormalize(&d, &d);
    }
    d.y = 0.0f;
    PSVECScale(&d, &d, 0.8f);
    PSVECSubtract(&p->vel, &d, &p->vel);
    p->maxSpeed = 2.4f;
}

/**
 * Recover mode: first checks the Puni is still inside the map and puts it back if not,
 * then rests until hp is 80 percent (back to idle);
 * an opponent coming within 80 units sends it back to escape mode and the wind takes it to wind mode.
 */
static void puniModeRecover(PuniData* p) {
    int r;

    switch (p->modeStep) {
        case 0:
            r = puniPositionCheck(p);
            if (r != 0) {
                p->recoverReason = r;
                p->recoverFromPos = p->pos;
                puniPositionRecover(p);
                p->recoverToPos = p->pos;
            }
            {
                Vec puni_recover_vel = {0.0f, 0.0f, 0.0f};
                p->vel = puni_recover_vel;
            }
            p->modeStep = 1;
            break;
        case 1:
            if (p->hp * 100 / p->maxHp >= 80) {
                p->mode = PUNI_MODE_IDLE;
                p->anim = 0;
                p->modeStep = 0;
            } else if (p->breathPower > 10.0f) {
                p->prevMode = p->mode;
                p->target = Punigp->marioPuni;
                p->mode = PUNI_MODE_WIND;
                p->modeStep = 0;
            } else {
                if (((s32)gp->retraceCount & 3) == (*(s16*)(&p->index) & 3)) {
                    if (p->flags & PUNI_FLAG_PUNI) {
                        p->target = searchBattleTeki_inl(p);
                    } else {
                        p->target = searchBattleMikata_inl(p);
                    }
                }
                if (p->target != NULL) {
                    Vec d;
                    PSVECSubtract(&p->target->pos, &p->pos, &d);
                    if (PSVECMag(&d) < 80.0f) {
                        p->mode = PUNI_MODE_ESCAPE;
                        p->anim = 2;
                        p->modeStep = 0;
                    }
                }
            }
            break;
    }
}

/** Defeated mode: plays the Puni fight death sound once and lies down in the down animation. */
#if VERSION <= VERSION_DEMO_JP
/** The early demos' puni main loop calls this directly with the sound name; their puniModeDead below calls it too. */
static inline void puniModeDeadSfx(PuniData* p, const char* sfx) {
    switch (p->modeStep) {
        case 0:
            psndSFXOn_3D(sfx, &p->pos);
#else
static void puniModeDead(PuniData* p) {
    switch (p->modeStep) {
        case 0:
            psndSFXOn_3D("SFX_STG2_FIGHT_TOGE3", &p->pos);
#endif
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                p->vel = v;
            }
            p->anim = 4;
            p->modeStep = 1;
            break;
        case 1:
            break;
    }
}

/** Defeated mode: plays the Puni fight death sound once and lies down in the down animation. */
#if VERSION <= VERSION_DEMO_JP
static void puniModeDead(PuniData* p) {
    puniModeDeadSfx(p, "SFX_STG2_FIGHT_TOGE3");
}
#endif

/**
 * Moves a Puni blown by the wind and handles what it hits:
 * returns 1 for a wall that pushes it out (attribute 0x4, HIT_ATTR_NPC_ONLY; remembers the push direction and goes to step 3),
 * 2 for any other wall hit (step 10; also for any hit while all Punies fight) and 0 if nothing was hit.
 */
static int puniModeWind_sub(PuniData* p) {
    Vec dir;
    Vec start;
    f32 mag = PSVECMag(&p->vel);
    start = p->pos;
    start.y += 1.0f;
    if (0.0f != mag) {
        PSVECNormalize(&p->vel, &dir);
    } else {
        {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            dir = v_;
        }
    }
    puniMoveAtari(p, start, dir, 0, mag);
    if (Punigp->allMode == 3 && (p->hitMove != NULL || p->hitCheck != NULL)) {
        p->modeStep = 10;
        return 2;
    }
    if (p->hitMove != NULL || p->hitCheck != NULL) {
        if (p->hitMove != NULL && (p->hitMove->attributes & HIT_ATTR_NPC_ONLY)) {
            p->pushDir.x = -p->wallNrm.x;
            p->pushDir.y = 0.0f;
            p->pushDir.z = -p->wallNrm.z;
            p->savedPos = p->pos;
            p->modeStep = 3;
            return 1;
        } else if (p->hitCheck != NULL && (p->hitCheck->attributes & HIT_ATTR_NPC_ONLY)) {
            p->pushDir.x = -p->wallNrm.x;
            p->pushDir.y = 0.0f;
            p->pushDir.z = -p->wallNrm.z;
            p->savedPos = p->pos;
            p->modeStep = 3;
            return 1;
        } else {
            p->modeStep = 10;
            return 2;
        }
    }
    return 0;
}

/**
 * Spins a Puni that is blown by the wind (10 degrees a frame),
 * clockwise or not depending on which side it is facing and which side of Mario it is on.
 */
static void puniModeWind_kaiten(PuniData* p) {
    BOOL dir;
    if (p->dir < 180.0f) {
        dir = p->modeVar0;
    } else {
        dir = p->modeVar0 ^ 1;
    }
    if (dir) {
        p->spin += 10.0f;
        if (p->spin >= 360.0f) {
            p->spin -= 360.0f;
        }
    } else {
        p->spin -= 10.0f;
        if (p->spin <= 0.0f) {
            p->spin += 360.0f;
        }
    }
}

static const Vec puni_fall_dir;

/**
 * Wind mode: Flurrie's breath blows the Puni away from Mario in a sliding, spinning arc:
 * it accelerates up to a speed set by the breath power, then slows down;
 * it is pushed off walls and falls if it ends over no floor.
 * When the breath stops it goes back to join Mario (or to dizzy mode during a battle).
 */
static void puniModeWind(PuniData* p) {
    MarioWork* mario = marioGetPtr();
    f32 hz, hy, hx;
    f32 nz, ny, nx;
    f32 dist;
    Vec d;
    Vec start;

    switch (p->modeStep) {
        case 0:
            if (p->flags & (PUNI_FLAG_SPRITE_PUNI | PUNI_FLAG_SPRITE_TOGE)) {
                p->rotOffsetY = 8.0f;
            } else {
                p->dispOfs.y = 8.0f;
            }
            p->flags &= ~PUNI_FLAG_COLLIDE;
            PSVECSubtract(&p->pos, &mario->position, &d);
            if (d.x < 0.0f) {
                p->modeVar0 = 0;
            } else {
                p->modeVar0 = 1;
            }
            d.x *= 1.5f;
            d.z *= 0.5f;
            dist = PSVECMag(&d);
            if (dist != 0.0f) {
                PSVECNormalize(&d, &d);
            } else {
                d = (Vec){0.0f, 0.0f, 0.0f};
            }
            d.y = 0.0f;
            p->modeF2 = 6.0f * (p->breathPower / 100.0f) + 2.0f;
            p->moveStart = d;
            p->modeF0 = 0.0f;
            p->modeF1 = 0.0f;
            if (p->flags & PUNI_FLAG_PUNI) {
                p->accelStep = 0.06f;
            } else {
                p->accelStep = 0.6f;
            }
            p->anim = 5;
            p->modeStep = 1;

            // fallthrough
        case 1:
            PSVECScale(&p->moveStart, &d, p->modeF0);
            d.z = 0.0f;
            p->vel = d;
            puniModeWind_kaiten(p);
            p->modeF0 += p->modeF1;
            p->modeF1 += p->accelStep;
            if (puniModeWind_sub(p) == 0 && p->modeF0 >= p->modeF2) {
                if (p->flags & PUNI_FLAG_PUNI) {
                    p->accelStep = 0.03f;
                } else {
                    p->accelStep = 0.003f;
                }
                p->modeF1 = 0.0f;
                p->modeStep = 2;
            }
            break;
        case 2:
            PSVECScale(&p->moveStart, &d, p->modeF0);
            d.z = 0.0f;
            p->vel = d;
            puniModeWind_kaiten(p);
            p->modeF0 -= p->modeF1;
            p->modeF1 += p->accelStep;
            if (puniModeWind_sub(p) == 0 && p->modeF0 < 0.0f) {
                p->modeStep = 10;
            }
            break;
        case 3:
            p->flags2 |= PUNI_FLAG2_CROSSING_LEDGE;
            PSVECNormalize(&p->pushDir, &d);
            PSVECScale(&d, &p->vel, p->radius.x);
            p->modeStep = 4;

            // fallthrough
        case 4:

            PSVECAdd(&p->pos, &p->vel, &p->pos);
            start = p->pos;
            dist = 2.0f;
            d = (Vec){0.0f, -1.0f, 0.0f};
            start.y += 0.2f;
            if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, start.x, start.y, start.z, d.x, d.y, d.z) == NULL) {
                p->vel = (Vec){0.0f, 0.0f, 0.0f};
                p->mode = PUNI_MODE_FALL;
                p->anim = 0;
                p->rotOffsetY = 0.0f;
                p->spin = 0.0f;
                p->dispOfs.y = 0.0f;
                p->modeStep = 0;
            } else {
                p->modeStep = 10;
            }
            break;
        case 10:
            puniModeWind_kaiten(p);
            if (!(p->breathPower > 0.0f)) {
                if (Punigp->allMode == 3) {
                    p->mode = PUNI_MODE_PIYORI;
                    p->rotOffsetY = 0.0f;
                    p->spin = 0.0f;
                    p->dispOfs.y = 0.0f;
                    p->vel = (Vec){0.0f, 0.0f, 0.0f};
                    p->modeStep = 0;
                } else {
                    p->rotOffsetY = 0.0f;
                    p->spin = 0.0f;
                    p->dispOfs.y = 0.0f;
                    p->vel = (Vec){0.0f, 0.0f, 0.0f};
                    p->mode = PUNI_MODE_JOIN;
                    p->modeStep = 0;
                }
            }
            break;
    }
}

/**
 * Fall mode: the Puni falls under gravity (0.45 per frame) until it hits something.
 * Depending on what it lands on it chooses the next mode: attribute 0x100 the updraft wait (preupdraft),
 * 0x200 the water mode, the floor of one of the first two targets the pedestal mode (dodai),
 * ordinary floor the position check; a wall with attribute 0x4 does not stop the fall.
 * A Puni that falls below y=-1000 is put back at the recovery position and becomes lost (hagure).
 */
#if VERSION <= VERSION_DEMO_JP
static void puniModeFall(PuniData* p) {
#else
void puniModeFall(PuniData* p) {
    Vec start;
#endif
    Vec dir;
#if VERSION <= VERSION_DEMO_JP
    Vec start;
#endif
    f32 mag;
    PuniTarget* t = Punigp->targets;
    HitEntry* h0;
    HitEntry* h1;
    s32 found;
    s32 i;

    switch (p->modeStep) {
        case 0:
            p->flags &= ~PUNI_FLAG_COLLIDE;
            {
                Vec v = {0.0f, -0.8f, 0.0f};
                p->vel = v;
            }
            p->modeStep = 1;
            break;
        case 1:
            p->vel.x = 0.0f;
            p->vel.y -= 0.45f;
#if VERSION <= VERSION_DEMO_JP
            PSVECNormalize(&p->vel, &dir);
#else
            {
                Vec v = {0.0f, -1.0f, 0.0f};
                dir = v;
            }
#endif
            mag = PSVECMag(&p->vel);
            start = p->pos;
            puniMoveAtari(p, start, dir, 0, mag);
            h0 = p->hitMove;
            if (h0 != NULL || p->hitCheck != NULL) {
                if (h0 != NULL && (h0->attributes & HIT_ATTR_NPC_ONLY)) {
                    break;
                }
                h1 = p->hitCheck;
                if (h1 != NULL && (h1->attributes & HIT_ATTR_NPC_ONLY)) {
                    break;
                }
                if (h0 != NULL && (h0->attributes & HIT_ATTR_SHALLOW_WATER)) {
                    p->modeVar0 = 0x12;
                    p->anim = 0;
                    p->modeStep = 2;
                    break;
                }
                if (h1 != NULL && (h1->attributes & HIT_ATTR_SHALLOW_WATER)) {
                    p->modeVar0 = 0x12;
                    p->anim = 0;
                    p->modeStep = 2;
                    break;
                }
                if (h0 != NULL && (h0->attributes & HIT_ATTR_WATER_HAZARD)) {
                    p->modeVar0 = 0x16;
                    p->anim = 3;
                    p->modeStep = 2;
                    break;
                }
                if (h1 != NULL && (h1->attributes & HIT_ATTR_WATER_HAZARD)) {
                    p->modeVar0 = 0x16;
                    p->anim = 3;
                    p->modeStep = 2;
                    break;
                }
                found = 0;
                for (i = 0; i < 2; i++, t++) {
                    if (t->hit == NULL) {
                        continue;
                    }
                    if ((void*)h0 == (void*)t->hit) {
                        found = 1;
                        p->moveStart = t->pos;
                        break;
                    }
                    if ((void*)h1 == (void*)t->hit) {
                        found = 1;
                        p->moveStart = t->pos;
                        break;
                    }
                }
                if (found) {
                    p->modeVar0 = 0x17;
                } else {
                    p->modeVar0 = 0xF;
                }
#if VERSION >= VERSION_JP
                if (Punigp->floor == 0x12 && p->pos.y < -300.0f) {
                    s32 zero = 0;
                    puniswTrapSetindex(*(s16*)(&p->index), &zero);
                    p->flags &= ~PUNI_FLAG_NOT_IN_PARTY;
                }
#endif
                {
                    Vec v = {0.0f, 0.0f, 0.0f};
                    p->vel = v;
                }
                p->modeStep = 2;
            } else if (p->pos.y <= -1000.0f) {
                p->recoverFromPos = p->pos;
                p->pos = Punigp->recoverPos;
                p->pos.x += randf(0.0f, 2.0f) - 1.0f;
                p->pos.z += randf(0.0f, 2.0f) - 1.0f;
                p->prevPos = p->pos;
                p->recoverToPos = p->pos;
                p->modeVar0 = 0x10;
                {
                    Vec v = {0.0f, 0.0f, 0.0f};
                    p->vel = v;
                }
                p->modeStep = 2;
            }
            break;
        case 2:
            p->mode = p->modeVar0;
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                p->vel = v;
            }
            p->modeStep = 0;
            break;
    }
}

/**
 * Waits (30 to 230 frames) with a balloon effect over the Puni before starting to ride an updraft;
 * its draw pose (p->npc is a pose view) then gets flag 4, which hides its shadow.
 */
static void puniModePreupdraft(PuniData* p) {
    Vec pos;

    switch (p->modeStep) {
        case 0:
            pos = p->pos;
            pos.y += 15.0f;
            pos.z += 3.0f;
            p->effect = effPuniBalloonEntry(0, pos.x, pos.y, pos.z, 1.5f);
            p->timer = rand() % 201 + 30;
            p->modeStep = 1;
        case 1:
            if (p->timer-- == 0) {
                p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
                p->mode = PUNI_MODE_UPDRAFT;
                p->modeStep = 0;
            }
            break;
    }
}

static const Vec puni_fall_dir;

/**
 * Updraft mode: the Puni floats upward inside a soap bubble (balloon effect following it,
 * with a gentle sideways wobble and a bubble sound), blown sideways by Flurrie's breath.
 * When it hits a wall (other than the pushing kind) or rises above y=0 the bubble pops
 * and it goes to the wonder mode.
 */
static void puniModeUpdraft(PuniData *p) {
    MarioWork *mario = marioGetPtr();
    Vec d;
    Vec save;
    Vec savev;
    f32 mag;
    s32 t;
    Vec q;
    if (p->modeStep == 0) {
        {
            Vec v_ = {0.0f, 0.94f, 0.0f};
            p->vel = v_;
        }
    } else {
        {
            Vec v_ = {0.0f, 0.66f, 0.0f};
            p->vel = v_;
        }
    }
    if ((gp->retraceCount & 0x1F) == 0) {
        psndSFXOn_3D("SFX_STG2_SOAPBALL_MOVE1", &p->pos);
    }
    t = (gp->retraceCount + p->index) & 0xF;
    if (t > 7) {
        t = 4 - (t & 7);
    } else {
        t = t - 3;
    }
    p->dispOfs.x = 0.5f * ((f32) t);
    switch (p->modeStep) {
        case 0:
            if (p->breathPower > 10.0f) {
                p->target = Punigp->marioPuni;
                PSVECSubtract(&p->pos, &mario->position, &d);
                mag = PSVECMag(&d);
                if (0.0f != mag) {
                    PSVECNormalize(&d, &d);
                }
                d.y = 0.0f;
                d.z = 0.0f;
                p->modeF2 = 1.2f + (p->breathPower / 100.0f);
                p->moveStart = d;
                p->modeF0 = 0.0f;
                p->modeF1 = 0.0f;
                p->modeStep = 1;
            }
            break;

        case 1:
            PSVECScale(&p->moveStart, &d, p->modeF0);
            PSVECAdd(&p->vel, &d, &p->vel);
            p->modeF0 += p->modeF1;
            p->modeF1 += 0.06f;
            if ((p->modeF0) >= (p->modeF2)) {
                p->modeF0 = p->modeF2;
            }
            break;

        case 2:
            PSVECScale(&p->moveStart, &d, p->modeF0);
            PSVECAdd(&p->vel, &d, &p->vel);
            p->modeF0 -= p->modeF1;
            p->modeF1 += 0.03f;
            if ((p->modeF0) < 0.0f) {
                p->modeStep = 0;
            }
            break;

    }

    mag = PSVECMag(&p->vel);
    if (0.0f != mag) {
        PSVECNormalize(&p->vel, &d);
    } else {
        {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            d = v_;
        }
    }
    save = p->pos;
    savev = p->vel;
    puniMoveAtari(p, save, d, 1, mag);
    q = p->pos;
    q.x += (p->dispOfs.x);
    q.y += 15.0f;
    q.z += 3.0f;
    ((PuniBalloonWork*)p->effect->userdata)->position.x = q.x;
    ((PuniBalloonWork*)p->effect->userdata)->position.y = q.y;
    ((PuniBalloonWork*)p->effect->userdata)->position.z = q.z;
    if ((p->hitMove != NULL) || (p->hitCheck != NULL)) {
        s32 f = 0;
        if ((p->hitMove != NULL) && (p->hitMove->attributes & HIT_ATTR_NPC_ONLY)) {
            f = 1;
        }
        if ((p->hitCheck != NULL) && (p->hitCheck->attributes & HIT_ATTR_NPC_ONLY)) {
            f = 1;
        }
        if (f == 0) {
            p->mode = PUNI_MODE_WONDER;
            p->modeStep = 0;
            p->dispOfs.x = 0.0f;
        } else {
            p->pos = save;
            p->vel = savev;
            PSVECAdd(&p->pos, &p->vel, &p->pos);
        }
    }
    if (p->pos.y >= 0.0f) {
        p->mode = PUNI_MODE_WONDER;
        p->modeStep = 0;
        p->dispOfs.x = 0.0f;
    }
}

/**
 * Wonder mode: pops the soap bubble (sound and burst effect),
 * starts animation 9 at once and after 20 frames goes to the funbari mode with the walk animation.
 * The demo build has this as a helper taking the sound name.
 */
#if VERSION <= VERSION_DEMO_JP
static inline void puniModeWonderSfx(PuniData* p, const char* sfx) {
#else
void puniModeWonder(PuniData* p) {
#endif
    Vec pos;

    switch (p->modeStep) {
        case 0:
#if VERSION <= VERSION_DEMO_JP
            psndSFXOn_3D(sfx, &p->pos);
#else
            psndSFXOn_3D("SFX_STG2_SOAPBALL_BREAK1", &p->pos);
#endif
            if (p->effect != NULL) {
                effDelete(p->effect);
                p->effect = NULL;
            }
            pos = p->pos;
            pos.y += 15.0f;
            pos.z += 3.0f;
#if VERSION >= VERSION_US
            effPuniBalloonEntry(1, pos.x, pos.y, pos.z, 1.5f);
#else
            // BUG: keeps the pop burst in p->effect, but that effect deletes itself after 30 frames and effEntry
            // reuses its slot, so a later effDelete(p->effect) deletes whichever effect took the slot (often another
            // Puni's balloon, whose work puniModeUpdraft keeps writing after it is freed); US and later do not keep it.
            p->effect = effPuniBalloonEntry(1, pos.x, pos.y, pos.z, 1.5f);
#endif
            p->timer = 20;
            p->anim = 9;
            p->animTime = 0;
            p->flags |= PUNI_FLAG_OWN_ANIM_TIME;
            p->modeStep = 1;
        case 1:
            if (p->timer-- == 0) {
                p->npc->flags &= ~NPC_FLAG_RECORD_DEFEAT;
                p->mode = PUNI_MODE_FUNBARI;
                p->anim = 1;
                p->flags &= ~PUNI_FLAG_OWN_ANIM_TIME;
                p->modeStep = 0;
            }
            break;
    }
}

/**
 * Wonder mode: pops the soap bubble (sound and burst effect),
 * starts animation 9 at once and after 20 frames goes to the funbari mode with the walk animation.
 * The demo build has this as a helper taking the sound name.
 */
#if VERSION <= VERSION_DEMO_JP
static void puniModeWonder(PuniData* p) {
    puniModeWonderSfx(p, "SFX_STG2_SOAPBALL_BREAK1");
}
#endif
/**
 * Inline copy of the speed-limited wall-collision move (see puniMoveLimitedNormal) taking the start
 * and direction by pointer; used by puniModeFunbari in the US and later releases.
 */
static inline void puniMoveHitLimitedInl(PuniData* p, Vec* s2, Vec* d2, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec old;
    f32 hitX, b2, b3;
    f32 hitNX, c2, c3;
    f32 d;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    p->hitMove = hitCheckFilter(NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, s2->x, s2->y, s2->z, d2->x, d2->y, d2->z);
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else if (Punigp->floor != 0x12) {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        {
            f32 dp = PSVECDotProduct(d2, &hit);
            PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            p->hitCheck = hitCheckFilter(NULL, &hitX, &b2, &b3, &d, &hitNX, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
}

/**
 * Funbari mode ("holding on"): the Puni that came down from the bubble moves toward the gather target with one hop
 * for 68 frames, then falls back to the floor (at once when no gather target is on).
 */
static void puniModeFunbari(PuniData* p) {
    Vec start;
    Vec s2;
    Vec d2;
    f32 dist;
    f32 s;
    PuniTarget* t;
    int i;

    switch (p->modeStep) {
        case 0:
            t = Punigp->targets;
            for (i = 0; i < 10; i++, t++) {
                if ((t->flags & PUNI_TARGET_FLAG_ON) && (t->flags & PUNI_TARGET_FLAG_MASS)) break;
            }
            if (i >= 10) {
                p->modeStep = 10;
                break;
            }
            if (p->pos.x > t->pos.x) {
                p->modeF2 = -1.0f;
                p->dir = 270.0f;
            } else {
                p->modeF2 = 1.0f;
                p->dir = 90.0f;
            }
            p->vel.x = 0.5f;
            p->timer = 68;
            p->modeF0 = 1.0f;
#if VERSION >= VERSION_US
            p->dispOfs.y = 0.0f;
#endif
            p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
            p->modeStep = 1;
            break;
        case 1:
        case 2:
            s = sin(3.1415927f * p->modeF0 / 180.0f);
            if (p->modeF0 <= 180.0f) {
                p->modeF1 = 0.5f * s;
            } else {
                p->modeF1 = 0.45f * s;
            }
            p->modeF0 += 7.0f;
            if (p->modeF0 >= 360.0f) p->modeF0 = 360.0f;
#if VERSION >= VERSION_US
            start = p->pos;
            dist = __fabs(p->modeF2);
            p->vel.x = 0.5f * p->modeF2;
            p->vel.z = p->vel.y = 0.0f;
            d2 = p->vel;
            s2 = start;
            puniMoveHitLimitedInl(p, &s2, &d2, dist);
            p->dispOfs.y += p->modeF1;
            if (p->timer-- == 0) {
                p->modeStep = 10;
                p->dispOfs.y = 0.0f;
            }
#else
            p->pos.y += p->modeF1;
            p->pos.x += p->vel.x * p->modeF2;
            if (p->timer-- == 0) p->modeStep = 10;
#endif
            break;
        case 10: {
                Vec v_ = {0.0f, 0.0f, 0.0f};
                p->vel = v_;
            }
            p->mode = PUNI_MODE_FALL;
            p->anim = 0;
            p->npc->flags &= ~NPC_FLAG_RECORD_DEFEAT;
            p->modeStep = 0;
            break;
    }
}

/**
 * Water mode: the Puni is hidden for 1 second, then reappears
 * and is tossed out of the water in a short arc (300 ms) away from where it fell in, and is then lost (hagure).
 */
static void puniModeWater(PuniData* p) {
    Vec d;
    u32 t;
    f32 x;
    f32 z;
    switch (p->modeStep) {
        case 0:
            p->flags |= PUNI_FLAG_HIDDEN;
            p->timer = 60;
            p->modeStep = 1;
        case 1:
            if (p->timer-- == 0) {
                p->flags &= ~PUNI_FLAG_HIDDEN;
                p->modeStep = 2;
            }
            break;
        case 2:
            p->startTime = gp->renderTime;
            p->moveStart = p->pos;
            p->modeF0 = -80.0f;
            p->modeF1 = (p->savedPos.y - p->moveStart.y) - p->modeF0;
            PSVECNormalize(&p->pushDir, &d);
            PSVECScale(&d, &d, -20.0f);
            x = d.x + p->pos.x;
            z = d.z + p->pos.z;
            p->savedPos.x = x;
            p->savedPos.z = z;
            p->modeStep = 3;
        case 3:
            t = OSTicksToMilliseconds((u32)gp->renderTime - p->startTimeLo);
            if ((s32)t < 300) {
                f32 f = (f32)(s32)t / 300.0f;
                p->pos.x = intplGetValue(EASING_LINEAR, p->moveStart.x, p->savedPos.x, t, 300);
                p->pos.z = intplGetValue(EASING_LINEAR, p->moveStart.z, p->savedPos.z, t, 300);
                p->pos.y = f * (p->modeF0 * f + p->modeF1) + p->moveStart.y;
                p->prevPos = p->pos;
            } else {
                p->pos = p->savedPos;
                p->prevPos = p->pos;
                p->modeStep = 4;
            }
            break;
        case 4:
            p->mode = PUNI_MODE_HAGURE;
            p->anim = 0;
            p->modeStep = 0;
            break;
    }
}

/**
 * Lost-start mode: checks whether the Puni has fallen outside the map and puts it back at the recovery position
 * if so, then goes to the hagure mode.
 */
static void puniModePrehagure(PuniData* p) {
    Vec nrm;
    Vec hit;
    Vec dir;
    Vec start;
    f32 dist;
    s32 res;
    f32 b, a;
    switch (p->modeStep) {
        case 0:
            res = 0;
            a = Punigp->stuckTop0;
            b = Punigp->stuckBottom0;
            if (-1000.0f != a && p->pos.y < a && p->pos.y > b) {
                res = 1;
            }
            a = Punigp->stuckTop1;
            b = Punigp->stuckBottom1;
            if (-1000.0f != a && p->pos.y < a && p->pos.y > b) {
                res = 1;
            }
            if (res == 0) {
                res = 1;
            } else {
                start = p->pos;
                start.y += 1.0f;
                dir = puni_fall_dir;
                dist = 3.0f;
                if (hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &dist, &nrm.x, &nrm.y, &nrm.z, start.x, start.y, start.z, dir.x, dir.y, dir.z) == NULL) {
                    res = 2;
                } else {
                    res = 0;
                }
            }
            if (res != 0) {
                p->recoverReason = res;
                p->recoverFromPos = p->pos;
                p->pos = Punigp->recoverPos;
                p->pos.x += randf(0.0f, 2.0f) - 1.0f;
                p->pos.z += randf(0.0f, 2.0f) - 1.0f;
                p->prevPos = p->pos;
                p->recoverToPos = p->pos;
            }
            p->modeStep = 1;
            break;
        case 1:
            p->mode = PUNI_MODE_HAGURE;
            p->modeStep = 0;
            break;
    }
}

/**
 * Lost (hagure) mode: the Puni turns left and right, then shows a speech bubble now and then, waiting for Mario.
 * It rejoins Mario (and re-collects other lost Punies near it) once he is within 150 units,
 * goes to the gather or flee modes for targets in range and to the wind mode when blown.
 */
static void puniModeHagure(PuniData* p) {
    switch (p->modeStep) {
        case 0:
            // the bubble follows an NpcEntry-shaped view whose position field is p->bubblePos
            p->effect = effFukidashiEntry(4, 0, (NpcEntry*)((u8*)&p->bubblePos - offsetof(NpcEntry, position)), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1);
            p->anim = 6;
            p->flags |= PUNI_FLAG_COLLIDE;
            p->modeStep = 1;
        case 1:
            p->modeVar0 = rand() % 21 + 30;
            p->modeVar1 = rand() % 6 + 5;
            p->timer = p->modeVar0;
            p->modeVar2 = p->modeVar1;
            p->modeStep = 10;
            p->bubblePos.x = 0.0f;
            p->bubblePos.y = -1000.0f;
            p->bubblePos.z = 0.0f;
        case 10:
            if (p->timer-- == 0) {
                if (p->modeVar2 & 1) {
                    p->dir = 90.0f;
                } else {
                    p->dir = 270.0f;
                }
                if (p->modeVar2-- == 0) {
                    p->modeStep = 20;
                } else {
                    p->timer = p->modeVar0;
                }
            }
            break;
        case 20:
            p->timer = 30;
            p->modeStep = 21;
        case 21:
            if (p->timer-- == 0) {
                p->modeStep = 30;
            }
            break;
        case 30:
            if (rand() % 2) {
                p->bubblePos.x = p->pos.x;
                p->bubblePos.y = 10.0f + p->pos.y;
                p->bubblePos.z = p->pos.z;
                ((EffFukidashiWork*)p->effect->userdata)->frame = 0;
            }
            p->timer = rand() % 41 + 100;
            p->modeStep = 31;
        case 31:
            if (p->timer-- == 0) {
                p->modeStep = 1;
            }
            break;
    }
    if (p->awayTarget != -1 && p->awayTargetDistSq < 10000.0f) {
        Punigp->allMode = 2;
        p->modeStep = 0;
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    } else if (p->massTarget != -1) {
        PuniDokuritsu(p);
        p->mode = PUNI_MODE_MASS;
        p->modeStep = 0;
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    } else if (p->breathPower > 10.0f) {
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    } else if (p->marioDist < 150.0f) {
        p->mode = PUNI_MODE_JOIN;
        puniFukki(p);
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    }
}

/** Pedestal mode: the Puni walks toward a point (moveStart) and, once within 10 units, becomes lost (hagure) again. */
static void puniModeDodai(PuniData* p) {
    Vec v;

    switch (p->modeStep) {
        case 0:
            PSVECSubtract(&p->pos, &p->moveStart, &v);
            if (PSVECMag(&v) < 10.0f) {
                {
                    Vec zero = {0.0f, 0.0f, 0.0f};
                    p->vel = zero;
                }
                p->mode = PUNI_MODE_HAGURE;
                p->modeStep = 0;
            } else {
                PSVECNormalize(&v, &v);
                v.y = 0.0f;
                PSVECScale(&v, &v, 1.0f);
                PSVECSubtract(&p->vel, &v, &p->vel);
                p->maxSpeed = 3.0f;
            }
            break;
    }
}

/**
 * Frees a trapped Puni from its stand-in: for the guide, the sister and the rival, shows the Puni again,
 * makes it collide and moves the stand-in NPC away.
 */
static void puniModeTrap_sub(PuniData* p) {
    NpcEntry* n;
    if (p->flags & PUNI_FLAG_NPC_GUIDE) {
        n = npcNameToPtr("\203K\203C\203h");
        p->flags &= ~PUNI_FLAG_HIDDEN;
        p->flags |= PUNI_FLAG_COLLIDE;
        n->flags &= ~NPC_FLAG_NO_HIT;
        n->position.x = 0.0f;
        n->position.y = -1000.0f;
        n->position.z = 0.0f;
    }
    if (p->flags & PUNI_FLAG_NPC_GUIDE_SISTER) {
        n = npcNameToPtr("\203K\203C\203h\226\205");
        p->flags &= ~PUNI_FLAG_HIDDEN;
        p->flags |= PUNI_FLAG_COLLIDE;
        n->flags &= ~NPC_FLAG_NO_HIT;
        n->position.x = 0.0f;
        n->position.y = -1000.0f;
        n->position.z = 0.0f;
    }
    if (p->flags & PUNI_FLAG_NPC_RIVAL) {
        n = npcNameToPtr("\203\211\203C\203o\203\213");
        p->flags &= ~PUNI_FLAG_HIDDEN;
        p->flags |= PUNI_FLAG_COLLIDE;
        n->flags &= ~NPC_FLAG_NO_HIT;
        n->position.x = 0.0f;
        n->position.y = -1000.0f;
        n->position.z = 0.0f;
    }
}

#define TRAP_NPC(name, setpos)        \
    {                                 \
        n = npcNameToPtr(name);       \
        p->flags |= PUNI_FLAG_HIDDEN; \
        n->position.x = p->pos.x;     \
        n->position.y = p->pos.y;     \
        n->position.z = p->pos.z;     \
        n->direction = p->dir;        \
        n->flags |= NPC_FLAG_NO_HIT;  \
    }
#define TRAP_HIDE(name)                \
    {                                  \
        n = npcNameToPtr(name);        \
        p->flags &= ~PUNI_FLAG_HIDDEN; \
        p->flags |= PUNI_FLAG_COLLIDE; \
        n->flags &= ~NPC_FLAG_NO_HIT;  \
        n->position.x = 0.0f;          \
        n->position.y = -1000.0f;      \
        n->position.z = 0.0f;          \
    }

/**
 * Trapped mode: the Puni is caught in the cage trap of mri_17; the guide, sister or rival NPC stands in for it.
 * Flurrie's breath or a gather target frees it again.
 */
static void puniModeTrap(PuniData* p) {
    NpcEntry* n;
    if (p->modeStep == 0) {
        p->flags |= PUNI_FLAG_COLLIDE;
        if (p->flags & PUNI_FLAG_NPC_GUIDE) TRAP_NPC("\203K\203C\203h", 0)
        if (p->flags & PUNI_FLAG_NPC_GUIDE_SISTER) TRAP_NPC("\203K\203C\203h\226\205", 0)
        if (p->flags & PUNI_FLAG_NPC_RIVAL) TRAP_NPC("\203\211\203C\203o\203\213", 0) {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            p->vel = v_;
        }
        p->prevMode = p->mode;
    }
    if (p->breathPower > 10.0f) {
        if (p->flags & PUNI_FLAG_NPC_GUIDE) TRAP_HIDE("\203K\203C\203h")
        if (p->flags & PUNI_FLAG_NPC_GUIDE_SISTER) TRAP_HIDE("\203K\203C\203h\226\205")
        if (p->flags & PUNI_FLAG_NPC_RIVAL) TRAP_HIDE("\203\211\203C\203o\203\213")
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->prevMode = p->mode;
        p->mode = PUNI_MODE_WIND;
        p->modeStep = 0;
    } else if (p->massTarget != -1) {
        if (p->flags & PUNI_FLAG_NPC_GUIDE) TRAP_HIDE("\203K\203C\203h")
        if (p->flags & PUNI_FLAG_NPC_GUIDE_SISTER) TRAP_HIDE("\203K\203C\203h\226\205")
        if (p->flags & PUNI_FLAG_NPC_RIVAL) TRAP_HIDE("\203\211\203C\203o\203\213")
        PuniDokuritsu(p);
        p->prevMode = p->mode;
        p->mode = PUNI_MODE_MASS;
        p->modeStep = 0;
    }
}

/**
 * Starts the scale-up when a Puni walks in through a left or right loading zone:
 * resets the delay counter, scale 0 and sets flag 4 on its draw pose (p->npc is a pose view; hides its shadow).
 */
static void puniModeEnterSubInit(PuniData* p) {
    if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
        p->modeVar0 = 0;
        p->scale = 0.0f;
        p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
    }
}

/**
 * Scale-up step for a Puni entering through a left or right loading zone:
 * after 10 frames it grows by 0.1 a frame up to full size.
 */
static void puniModeEnterSub(PuniData* p) {
    if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
        p->modeVar0++;
        if (p->modeVar0 >= 10) {
            p->scale += 0.1f;
            if (p->scale >= 1.0f) {
                p->scale = 1.0f;
            }
        }
    }
}

/**
 * Entering mode: a Puni walks in through a loading zone after Mario,
 * starting after a delay (200 ms, 700 ms for the down pipe, plus a random 0 to 500),
 * walks to its entry point and then joins the follow chain behind Mario.
 */
static void puniModeEnter(PuniData* p) {
    Vec d;
    u32 t;
    switch (p->modeStep) {
        case 0:
            p->startTime = gp->renderTime;
            p->timer = 200;
            if ((Punigp->flags & PUNI_WORK_FLAG_PIPE_DOWN) && !(Punigp->flags & PUNI_WORK_FLAG_NO_NEXT_MAP)) {
                p->timer = 700;
            }
            p->timer += qrnd(500);
            if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
                p->modeVar0 = 0;
                p->scale = 0.0f;
                p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
            }
            p->modeStep = 1;
        case 1:
            t = OSTicksToMilliseconds((u32)gp->renderTime - p->startTimeLo);
            if ((s32)t > p->timer) {
                p->modeStep = 2;
            }
            break;
        case 2:
            if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
                p->modeVar0++;
                if (p->modeVar0 >= 10) {
                    p->scale += 0.1f;
                    if (p->scale >= 1.0f) {
                        p->scale = 1.0f;
                    }
                }
            }
            if (PSVECSquareDistance(&p->moveStart, &p->pos) > 100.0f) {
                PSVECSubtract(&p->moveStart, &p->pos, &d);
                PSVECNormalize(&d, &d);
                d.y = 0.0f;
                PSVECScale(&d, &d, 1.0f);
                PSVECAdd(&p->vel, &d, &p->vel);
            } else {
                p->flags |= PUNI_FLAG_WALL_CHECK;
                {
                    Vec puni_enter_dir = {0.0f, 0.0f, 0.0f};
                    p->vel = puni_enter_dir;
                }
                p->scale = 1.0f;
                p->npc->flags &= ~NPC_FLAG_RECORD_DEFEAT;
                p->modeStep = 3;
            }
            break;
        case 3:
            searchParent(p);
            p->mode = PUNI_MODE_PARENT;
            p->flags |= (PUNI_FLAG_IN_CHAIN | PUNI_FLAG_COLLIDE);
            PSVECSubtract(&p->parent->pos, &p->pos, &d);
            p->prevParentDist = p->parentDist = PSVECMag(&d);
            p->timer = 0;
            p->modeStep = 0;
            p->anim = 1;
            break;
    }
}

/** Starts the shrink-out when a Puni walks out through a left or right loading zone. */
static void puniModeExitSubInit(PuniData* p) {
    if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
        p->modeVar0 = 0;
        p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
    }
}

/**
 * Shrink-out step for a Puni leaving through a left or right loading zone:
 * after 10 frames it shrinks by 0.3 a frame down to zero.
 */
static void puniModeExitSub(PuniData* p) {
    if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
        p->modeVar0++;
        if (p->modeVar0 >= 10) {
            p->scale -= 0.3f;
            if (p->scale <= 0.0f) {
                p->scale = 0.0f;
            }
        }
    }
}

/**
 * Exiting mode: a Puni walks to the near end of the loading zone (beroStart),
 * then to the far end (beroEnd) while shrinking, and is then parked below the map and turned inert.
 */
static void puniModeExit(PuniData* p) {
    Vec d;
    switch (p->modeStep) {
        case 0:
            if (PSVECSquareDistance(&Punigp->beroStart, &p->pos) > Punigp->beroStartRange) {
                PSVECSubtract(&Punigp->beroStart, &p->pos, &d);
                PSVECNormalize(&d, &d);
                d.y = 0.0f;
                PSVECScale(&d, &d, 1.5f);
                PSVECAdd(&p->vel, &d, &p->vel);
            } else {
                {
                    Vec puniModeExit_v0 = {0.0f, 0.0f, 0.0f};
                    p->vel = puniModeExit_v0;
                }
                p->flags &= ~PUNI_FLAG_COLLIDE & ~PUNI_FLAG_WALL_CHECK;
                if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
                    p->modeVar0 = 0;
                    p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
                }
                p->modeStep = 1;
            }
            break;
        case 1:
            if (Punigp->flags & (PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST)) {
                p->modeVar0++;
                if (p->modeVar0 >= 10) {
                    p->scale -= 0.3f;
                    if (p->scale <= 0.0f) {
                        p->scale = 0.0f;
                    }
                }
            }
            if (PSVECSquareDistance(&Punigp->beroEnd, &p->pos) > Punigp->beroEndRange) {
                PSVECSubtract(&Punigp->beroEnd, &p->pos, &d);
                PSVECNormalize(&d, &d);
                d.y = 0.0f;
                PSVECScale(&d, &d, 2.0f);
                PSVECAdd(&p->vel, &d, &p->vel);
            } else {
                p->mode = PUNI_MODE_INERT;
                p->flags &= ~PUNI_FLAG_COLLIDE;
                {
                    Vec puniModeExit_v1 = {0.0f, 0.0f, 0.0f};
                    p->vel = puniModeExit_v1;
                }
                {
                    Vec puniModeExit_v2 = {0.0f, -1000.0f, 0.0f};
                    p->pos = puniModeExit_v2;
                }
                p->prevPos = p->pos;
                p->modeStep = 0;
            }
            break;
        case 2:
        default:
            break;
    }
}

/**
 * Point mode: the same walk as the exit mode (to beroStart then beroEnd) but without scaling;
 * the Puni ends parked below the map, inert. Used by scripts to walk the crowd across a room.
 */
static void puniModePoint(PuniData* p) {
    Vec d;
    switch (p->modeStep) {
        case 0:
            if (PSVECSquareDistance(&Punigp->beroStart, &p->pos) > Punigp->beroStartRange) {
                PSVECSubtract(&Punigp->beroStart, &p->pos, &d);
                PSVECNormalize(&d, &d);
                d.y = 0.0f;
                PSVECScale(&d, &d, 1.5f);
                PSVECAdd(&p->vel, &d, &p->vel);
            } else {
                p->vel = (Vec){0.0f, 0.0f, 0.0f};
                p->flags &= ~PUNI_FLAG_COLLIDE & ~PUNI_FLAG_WALL_CHECK;
                p->modeStep = 1;
            }
            break;
        case 1:
            if (PSVECSquareDistance(&Punigp->beroEnd, &p->pos) > Punigp->beroEndRange) {
                PSVECSubtract(&Punigp->beroEnd, &p->pos, &d);
                PSVECNormalize(&d, &d);
                d.y = 0.0f;
                PSVECScale(&d, &d, 2.0f);
                PSVECAdd(&p->vel, &d, &p->vel);
            } else {
                p->mode = PUNI_MODE_INERT;
                p->flags &= ~PUNI_FLAG_COLLIDE;
                p->vel = (Vec){0.0f, 0.0f, 0.0f};
                p->pos = (Vec){0.0f, -1000.0f, 0.0f};
                p->prevPos = p->pos;
                p->modeStep = 0;
            }
            break;
        case 2:
            break;
    }
}

/** Pipe mode: hides the Puni and stops it; used while the crowd is inside a pipe. */
static void puniModeDokan(PuniData* p) {
    p->modeStep = 0;
    p->flags |= PUNI_FLAG_HIDDEN;
    p->npc->flags |= NPC_FLAG_RECORD_DEFEAT;
    {
        Vec v = {0.0f, 0.0f, 0.0f};
        p->vel = v;
    }
}

/**
 * Custom mode: calls the Puni's own mode function (modeFunc), if it has one.
 * Scripts use it with PUNI_MODE_FUNC for scripted behaviors.
 */
static void puniModeCustom(PuniData* p) {
    if (p->modeFunc) {
        p->modeFunc(p);
    }
}

/**
 * Draw callback for the Punies: loads the Puni draw state and draws every Puni pose;
 * while the dark view (area flag
 * 3, LSWF_MRI_00_DARKNESS_ON) is on, it sets up the dark lighting for each pose first.
 */
static void puni_puni_disp(CameraId camId, void* param) {
    MapTevCallbackInfo info;
    s32 off;
    MapTevCallbackInfo* ip;
    s32 i;
    PunidrvPose* pose;

    puniLoadRenderMode();
    puniLoadVertex();
    puniLoadTexture();
    puniLoadTev();
    if (!_swGet(3)) { // LSWF_MRI_00_DARKNESS_ON
        puniDraw();
    } else {
        for (ip = &info, i = 0, off = 0; i < puniGetPoseNum(); i++, off += sizeof(PunidrvPose)) {
            pose = (PunidrvPose*)((u8*)puniGetPosePtr() + off);
            ip->tevStage = 1;
            ip->texMap = 1;
            ip->texCoord = 1;
            ip->indStage = 0;
            ip->mtx = *pose->mtx;
            envSetYami(ip);
            GXSetNumTexGens(ip->texCoord);
            GXSetNumTevStages(ip->tevStage);
            GXSetNumIndStages(ip->indStage);
            puniPoseDraw(i);
        }
    }
    puniLoadTextureExit();
}

static u8 shadow_dl[32] ATTRIBUTE_ALIGN(32) = {
    0x80, 0x00, 0x04, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03,
};

/** The four corners of the square shadow drawn under each Puni (2.4 units wide, on the ground). */
static f32 shadow_vtx[12] ATTRIBUTE_ALIGN(32) = {
    -1.2f, 0.0f, -1.2f, -1.2f, 0.0f, 1.2f, 1.2f, 0.0f, 1.2f, 1.2f, 0.0f, -1.2f,
};

/** Texture coordinates of the shadow square, one pair per corner. */
static f32 shadow_coord[8] ATTRIBUTE_ALIGN(32) = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

/**
 * Draw callback for one Puni's shadow: a dark translucent shadow quad (texture 0x2C) placed under the pose,
 * at the floor height when the pose has a shadow height; skipped for poses with flag 4.
 */
static void puni_puni_shadow_disp(CameraId camId, void* param) {
    static Mtx shadow_mt = { { 10.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 10.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 10.0f, 0.0f } };
    GXColor color;
    GXTexObj obj;
    Mtx mtx;
    CameraEntry* cam = camGetPtr(camId);
    PunidrvPose* pose = param;

    if (!(pose->flags & 4)) {
        static const GXColor c = { 0, 0, 0, 0xC0 };
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        color = c;
        GXSetChanMatColor(GX_COLOR0A0, color);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        effGetTexObjN64(0x2C, &obj);
        GXLoadTexObj(&obj, GX_TEXMAP0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetArray(GX_VA_POS, shadow_vtx, 12);
        GXSetArray(GX_VA_TEX0, shadow_coord, 8);
        GXSetCullMode(GX_CULL_NONE);
        PSMTXConcat(cam->view, *pose->mtx, mtx);
        PSMTXConcat(mtx, shadow_mt, mtx);
        if (pose->flags & 0x10) {
            PSMTXTransApply(mtx, mtx, 0.0f, 1.0f + (-(*pose->mtx)[1][3] + pose->shadowY), 0.0f);
        }
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXCallDisplayList(shadow_dl, 0x20);
    }
}

/**
 * Sets up the Puni system: seeds the Puni random generator,
 * makes Punidat[0] the stand-in for Mario and clears the other 210 slots with default speed 3,
 * follow range 10 and an empty pose each, shows the counter and allocates the maze record.
 */
static void puni_puni_init(void) {
    s32 i;
    PuniData* p;
    MarioWork* mario;
    PunidrvPose* pose;
    f32 r;
    f32 lo = 0.0f;

    pose = puniGetPosePtr();
    mario = marioGetPtr();
    r = (f32)rand() / 32767.0f;
    Punigp->seed0 = (s32)(lo + r * (100000000.0f - lo)) + (u32)gp->renderTime + 0x3A3B01F9;
    Punigp->seed1 = 0x21708331;
    p = Punidat;
    Punigp->marioPuni = p;
    p->flags |= (PUNI_FLAG_MARIO_UNUSED | PUNI_FLAG_ACTIVE | PUNI_FLAG_IN_CHAIN | PUNI_FLAG_COLLIDE);
    p->pos = mario->position;
    p->prevPos = p->pos;
    p->radius.x = 10.0f;
    p->mode = PUNI_MODE_INERT;
    p->index = 0;
    p->groupId = 0;
    p = &Punidat[1];
    for (i = 1; i < PUNI_DATA_MAX; i++) {
        pose->flags = 1;
        pose->flags |= 2;
        pose->dir = 0.0f;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = -1000.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->offsetY = 0.0f;

        // This field aliases a billboard pose; its flags are not NpcEntry flags.
        p->npc = (NpcEntry*)pose;
        pose++;
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
            p->dispOfs = zero;
        }
        p->dir = 0.0f;
        p->rotOffsetY = 0.0f;
        p->scale = 1.0f;
        p->followRange = 10.0f;
        p->flags = 0;
        p->index = i;
        p->groupId = -1;
        p->maxSpeed = 3.0f;
        p++;
    }
    Punigp->showPuniCounter = 1;
    Punigp->puniCounterAlpha = 0xFF;
    Punigp->showRemaining = 0;
    Punimap = _mapAlloc(sizeof(PuniMap));
    memset(Punimap, 0, sizeof(PuniMap));
}

static const Vec puni_fall_dir;

/**
 * Checks that a stray Puni has not been pushed out of the map and puts it back at the recovery position
 * if needed, then makes it lost (hagure). Written as an inline helper for puniModeMain.
 */
static inline void puniModeCheckPos(PuniData* p) {
    s32 r;

    switch (p->modeStep) {
        case 0:
            r = puniPositionCheck(p);
            if (r != 0) {
                p->recoverReason = r;
                p->recoverFromPos = p->pos;
                puniPositionRecover(p);
                p->recoverToPos = p->pos;
            }
            p->modeStep = 1;
            break;
        case 1:
            p->mode = PUNI_MODE_HAGURE;
            p->modeStep = 0;
            break;
    }
}

// Descriptive name (not in the symbol maps); placeholder unk_mri_00012d58, JP mri .text+0x12C94. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
void puniModeMain(PuniData* p) {
    f32 m;
    Vec d;

    switch (p->mode) {
        case PUNI_MODE_IDLE:
            p->flags &= ~PUNI_FLAG_STRAY;
            if (p->breathPower > 10.0f) {
                p->prevMode = p->mode;
                PuniDokuritsu(p);
                p->target = Punigp->marioPuni;
                p->mode = PUNI_MODE_WIND;
                p->modeStep = 0;
            } else if (p->marioDist < 150.0f) {
                p->mode = PUNI_MODE_JOIN;
            }
            break;
        case PUNI_MODE_JOIN:
            p->flags &= ~PUNI_FLAG_STRAY;
            searchParent(p);
            p->mode = PUNI_MODE_PARENT;
            p->flags |= (PUNI_FLAG_IN_CHAIN | PUNI_FLAG_COLLIDE);
            PSVECSubtract(&p->parent->pos, &p->pos, &d);
            m = PSVECMag(&d);
            p->parentDist = m;
            p->prevParentDist = m;
            p->timer = 0;
            p->modeStep = 0;
            p->anim = 1;
            break;
        case PUNI_MODE_PARENT:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeParent(p);
            break;
        case PUNI_MODE_SEARCH:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeSearch(p);
            break;
        case PUNI_MODE_MASS:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeMass(p);
            break;
        case PUNI_MODE_AWAYJUMP:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeAwayjump(p);
            break;
        case PUNI_MODE_AWAY:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeAway(p);
            break;
        case PUNI_MODE_ATTACK:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeAttack(p);
            break;
        case PUNI_MODE_DAMAGE:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeDamage(p);
            break;
        case PUNI_MODE_PIYORI:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModePiyori(p);
            break;
        case PUNI_MODE_ESCAPE:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeEscape(p);
            break;
        case PUNI_MODE_RECOVER:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeRecover(p);
            break;
        case PUNI_MODE_DEAD:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeDead(p);
            break;
        case PUNI_MODE_WIND:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeWind(p);
            break;
        case PUNI_MODE_STOP:
            break;
        case PUNI_MODE_CHECKPOS:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeCheckPos(p);
            break;
        case PUNI_MODE_HAGURE:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeHagure(p);
            break;
        case PUNI_MODE_PREUPDRAFT:
            p->flags |= PUNI_FLAG_STRAY;
            puniModePreupdraft(p);
            break;
        case PUNI_MODE_UPDRAFT:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeUpdraft(p);
            break;
        case PUNI_MODE_WONDER:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeWonder(p);
            break;
        case PUNI_MODE_FUNBARI:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeFunbari(p);
            break;
        case PUNI_MODE_WATER:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeWater(p);
            break;
        case PUNI_MODE_FALL:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeFall(p);
            break;
        case PUNI_MODE_DODAI:
            p->flags |= PUNI_FLAG_STRAY;
            puniModeDodai(p);
            break;
        case PUNI_MODE_TRAP:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeTrap(p);
            break;
        case PUNI_MODE_ENTER:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeEnter(p);
            break;
        case PUNI_MODE_EXIT:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeExit(p);
            break;
        case PUNI_MODE_DOKAN:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModeDokan(p);
            break;
        case PUNI_MODE_POINT:
            p->flags &= ~PUNI_FLAG_STRAY;
            puniModePoint(p);
            break;
        case PUNI_MODE_FUNC:
            p->flags &= ~PUNI_FLAG_STRAY;
            if (p->modeFunc != NULL) {
                p->modeFunc(p);
            }
            break;
    }
}
#endif

static const Vec puni_fall_dir;

/**
 * Inline copy of the wall-collision move that reflects the velocity (puniMoveHit),
 * taking start and direction by pointer; used by puniMoveMain.
 */
static inline void puniMoveHitInl(PuniData* p, Vec* s2, Vec* d2, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec refl;
    Vec old;
    f32 hitX, b2, b3;
    f32 hitNX, c2, c3;
    f32 d;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & PUNI_FLAG_WALL_CHECK) {
        p->hitMove = hitCheckFilter(NULL, &pa.x, &pa.y, &pa.z, &d, &hit.x, &hit.y, &hit.z, s2->x, s2->y, s2->z, d2->x, d2->y, d2->z);
    }
    if (p->hitMove == NULL) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    } else {
        old = p->pos;
        *&p->wallNrm = hit;
        PSVECAdd(&p->pos, &p->vel, &p->pos);
        if (p->vel.y == 0.0f) {
            hit.y = 0.0f;
        }
        PSVECNormalize(&hit, &hit);
        C_VECReflect(d2, &hit, &refl);
        PSVECScale(&refl, &p->vel, dist);
        {
            f32 dot = PSVECDotProduct(d2, &hit);
            PSVECScale(&hit, &hit, -(dist - d) * dot + 0.1f);
        }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            p->hitCheck = hitCheckFilter(NULL, &hitX, &b2, &b3, &d, &hitNX, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
}

// Descriptive name (not in the symbol maps); placeholder fn_19_13A90, JP mri .text+0x13A90. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Moves one Puni according to its mode (JP and later):
 * the following, fighting and walking modes use the normal move with speed limit and slowdown,
 * the gather mode the speed-limited move, damage the reflecting move without speed limit or slowdown,
 * the scripted mode its own moveFunc; flee and wind modes have already moved themselves.
 * All these modes then set the Puni's height; the other modes (updraft, fall and so on) are left alone.
 */
void puniMoveMain(PuniData* p) {
    Vec d2;
    Vec s2;
    Vec dir;
    Vec start;
    f32 dist;

    switch (p->mode) {
        case PUNI_MODE_IDLE:
        case PUNI_MODE_JOIN:
        case 2:
        case PUNI_MODE_SEARCH:
        case PUNI_MODE_ATTACK:
        case PUNI_MODE_ESCAPE:
        case PUNI_MODE_PIYORI:
        case PUNI_MODE_STOP:
        case PUNI_MODE_HAGURE:
        case PUNI_MODE_DODAI:
        case PUNI_MODE_ENTER:
        case PUNI_MODE_EXIT:
        case PUNI_MODE_POINT:
            dist = PSVECMag(&p->vel);
            if (!dist) {
                goto mv_end1;
            }
            if (dist > 1.0f) {
                if (p->vel.x < 0.0f) {
                    p->dir = 270.0f;
                } else {
                    p->dir = 90.0f;
                }
            }
            if (dist > p->maxSpeed) {
                PSVECScale(&p->vel, &p->vel, p->maxSpeed / dist);
                dist = p->maxSpeed;
            }
            start = p->pos;
            start.y += 1.0f;
            PSVECNormalize(&p->vel, &dir);
            s2 = start;
            d2 = dir;
            puniMoveHitInl(p, &s2, &d2, dist);
            if (p->hitMove != NULL || p->hitCheck != NULL) {
                p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
            } else {
                PSVECScale(&p->vel, &p->vel, 0.6f);
                p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
            }
            mv_end1:
                puniSetYPosition(p);
                break;
            case PUNI_MODE_MASS: {
                Vec d2;
                Vec s2;
                Vec dir;
                Vec start;
                f32 dist = PSVECMag(&p->vel);
                if (!dist) {
                    goto mv_end2;
                }
                if (dist > p->maxSpeed) {
                    PSVECScale(&p->vel, &p->vel, p->maxSpeed / dist);
                    dist = p->maxSpeed;
                }
                start = p->pos;
                start.y += 1.0f;
                PSVECNormalize(&p->vel, &dir);
                d2 = dir;
                s2 = start;
                puniMoveHitLimitedInl(p, &s2, &d2, dist);
                if (p->hitMove != NULL || p->hitCheck != NULL) {
                    p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
                } else {
                    p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
                }
            }
            mv_end2:
                puniSetYPosition(p);
                break;
            case PUNI_MODE_DAMAGE: {
                Vec d2;
                Vec s2;
                Vec dir;
                Vec start;
                f32 dist = PSVECMag(&p->vel);
                if (!dist) {
                    goto mv_end3;
                }
                start = p->pos;
                start.y += 1.0f;
                PSVECNormalize(&p->vel, &dir);
                s2 = start;
                d2 = dir;
                puniMoveHitInl(p, &s2, &d2, dist);
                if (p->hitMove != NULL || p->hitCheck != NULL) {
                    p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
                } else {
                    p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
                }
            }
            mv_end3:
                puniSetYPosition(p);
                break;
        case PUNI_MODE_FUNC:
            if (p->moveFunc) {
                p->moveFunc(p);
            }
                puniSetYPosition(p);
                break;
        case PUNI_MODE_AWAYJUMP:
        case PUNI_MODE_AWAY:
        case PUNI_MODE_WIND:
            puniSetYPosition(p);
            break;
        case PUNI_MODE_TRAP: {
            Vec d2;
            Vec s2;
            Vec dir;
            Vec start;
            f32 dist = PSVECMag(&p->vel);
            if (!dist) {
                goto mv_end5;
            }
            if (dist > 1.0f) {
                if (p->vel.x < 0.0f) {
                    p->dir = 270.0f;
                } else {
                    p->dir = 90.0f;
                }
            }
            if (dist > p->maxSpeed) {
                PSVECScale(&p->vel, &p->vel, p->maxSpeed / dist);
                dist = p->maxSpeed;
            }
            start = p->pos;
            start.y += 1.0f;
            PSVECNormalize(&p->vel, &dir);
            s2 = start;
            d2 = dir;
            puniMoveHitInl(p, &s2, &d2, dist);
            if (p->hitMove != NULL || p->hitCheck != NULL) {
                p->flags |= PUNI_FLAG_HIT_WALL_UNUSED;
            } else {
                PSVECScale(&p->vel, &p->vel, 0.6f);
                p->flags &= ~PUNI_FLAG_HIT_WALL_UNUSED;
            }
        }
        mv_end5:
            puniSetYPosition(p);
            break;
    }
}

void puniOption(void) {
#else
static void puniOption(void) {
#endif
    PuniData* p;
    PuniData* q;
    PuniData* start;
    PuniData* end;
    Vec diff;
    f32 dist, r2, r;
    for (p = Punidat; p < Punidat + PUNI_DATA_MAX; p++) {
        p->flags &= ~PUNI_FLAG_NEAR;
#if VERSION <= VERSION_DEMO_JP
    }
    for (p = Punidat; p < Punidat + PUNI_DATA_MAX; p++) {
#endif
        if (!(p->flags & PUNI_FLAG_ACTIVE) || !(p->flags & PUNI_FLAG_COLLIDE)) {
            continue;
        }
        if (p < Punidat + PUNI_ENEMY_FIRST) {
            start = Punidat;
            end = Punidat + 109;
        } else {
            start = Punidat + PUNI_ENEMY_FIRST;
            end = Punidat + 209;
        }
        for (q = start; q <= end; q++) {
            if (!(q->flags & PUNI_FLAG_ACTIVE) || !(q->flags & PUNI_FLAG_COLLIDE) || p == q) {
                continue;
            }
            dist = PSVECDistance(&p->pos, &q->pos);
            r = p->radius.x + q->radius.x;
            r2 = p->radius.z + (r + p->radius.y);
            if (q->flags2 & PUNI_FLAG2_OBSTACLE) {
                if (dist < r) {
                    PSVECSubtract(&p->pos, &q->pos, &diff);
                    if (0.0f == PSVECMag(&diff)) {
                        diff.x = 1.0f;
                        dist = 1.0f;
                    }
                    PSVECNormalize(&diff, &diff);
                    PSVECScale(&diff, &diff, r - dist);
                    PSVECAdd(&p->vel, &diff, &p->vel);
                }
            } else if (dist <= r2) {
                if (dist < r2 - p->radius.y) {
                    p->flags |= PUNI_FLAG_NEAR;
                }
                if (!(p->flags & PUNI_FLAG_PUSHING) && dist < p->radius.x + q->radius.x) {
                    p->flags |= PUNI_FLAG_PUSHING;
                }
                if (p->flags & PUNI_FLAG_PUSHING) {
                    PSVECSubtract(&p->pos, &q->pos, &diff);
                    if (0.0f == PSVECMag(&diff)) {
                        diff.x = 1.0f;
                        dist = 1.0f;
                    }
                    PSVECNormalize(&diff, &diff);
                    diff.y = 0.0f;
                    PSVECScale(&diff, &diff, 0.85f * (0.3f * ((r2 - dist) / r2)) + 0.044999994f);
                    PSVECAdd(&p->vel, &diff, &p->vel);
                    PSVECSubtract(&q->vel, &diff, &q->vel);
                }
            }
        }
    }
    for (p = Punidat; p < Punidat + PUNI_DATA_MAX; p++) {
        if (!(p->flags & PUNI_FLAG_ACTIVE) || !(p->flags & PUNI_FLAG_COLLIDE)) {
            continue;
        }
        if (!(p->flags & PUNI_FLAG_NEAR)) {
            p->flags &= ~PUNI_FLAG_PUSHING;
        }
#if VERSION >= VERSION_JP
        if (PSVECMag(&p->vel) >= 8.0f) {
            PSVECNormalize(&p->vel, &p->vel);
            PSVECScale(&p->vel, &p->vel, 8.0f);
        }
    }
}
#else
    }
}
#endif

// Descriptive name (not in the symbol maps); placeholder unk_mri_00014ad4, JP mri .text+0x14A10. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Same push-apart pass as puniOption for slots 0 to 109 (JP and later),
 * used outside the two battle floors where there are no Jabbi slots:
 * separates overlapping Punies and blockers and caps the speed at 8.
 */
void puniCollideAll(void) {
    PuniData* p;
    PuniData* q;
    Vec diff;
    f32 dist, r2, r;

    for (p = Punidat; p <= Punidat + 109; p++) {
        p->flags &= ~PUNI_FLAG_NEAR;
        if (!(p->flags & PUNI_FLAG_ACTIVE) || !(p->flags & PUNI_FLAG_COLLIDE)) {
            continue;
        }
        for (q = Punidat; q <= Punidat + 109; q++) {
            if (!(q->flags & PUNI_FLAG_ACTIVE) || !(q->flags & PUNI_FLAG_COLLIDE) || p == q) {
                continue;
            }
            dist = PSVECDistance(&p->pos, &q->pos);
            r = p->radius.x + q->radius.x;
            r2 = p->radius.z + (r + p->radius.y);
            if (q->flags2 & PUNI_FLAG2_OBSTACLE) {
                if (dist < r) {
                    PSVECSubtract(&p->pos, &q->pos, &diff);
                    if (0.0f == PSVECMag(&diff)) {
                        diff.x = 1.0f;
                        dist = 1.0f;
                    }
                    PSVECNormalize(&diff, &diff);
                    PSVECScale(&diff, &diff, r - dist);
                    PSVECAdd(&p->vel, &diff, &p->vel);
                }
            } else if (dist <= r2) {
                if (dist < r2 - p->radius.y) {
                    p->flags |= PUNI_FLAG_NEAR;
                }
                if (!(p->flags & PUNI_FLAG_PUSHING) && dist < p->radius.x + q->radius.x) {
                    p->flags |= PUNI_FLAG_PUSHING;
                }
                if (p->flags & PUNI_FLAG_PUSHING) {
                    PSVECSubtract(&p->pos, &q->pos, &diff);
                    if (0.0f == PSVECMag(&diff)) {
                        diff.x = 1.0f;
                        dist = 1.0f;
                    }
                    PSVECNormalize(&diff, &diff);
                    diff.y = 0.0f;
                    PSVECScale(&diff, &diff, 0.85f * (0.3f * ((r2 - dist) / r2)) + 0.044999994f);
                    PSVECAdd(&p->vel, &diff, &p->vel);
                    PSVECSubtract(&q->vel, &diff, &q->vel);
                }
            }
        }
    }
    for (p = Punidat; p <= Punidat + 109; p++) {
        if (!(p->flags & PUNI_FLAG_ACTIVE) || !(p->flags & PUNI_FLAG_COLLIDE)) {
            continue;
        }
        if (!(p->flags & PUNI_FLAG_NEAR)) {
            p->flags &= ~PUNI_FLAG_PUSHING;
        }
        if (PSVECMag(&p->vel) >= 8.0f) {
            PSVECNormalize(&p->vel, &p->vel);
            PSVECScale(&p->vel, &p->vel, 8.0f);
        }
    }
}
#endif

/**
 * Plays the crowd sounds: counts the Punies that are walking or running (anim 1 or 2),
 * picks the loudness level 0 to 6 from the count
 * and every few frames plays one of the six Puni footstep sounds at a random walking Puni;
 * keeps the little Jabbi's own looping sound following slot 3;
 * and switches the Jabbi marching loop (levels 1 to 3 by count). Also resets the per-frame fight sound counters.
 */
static void puniSound(void) {
    int i;
    PuniData *p;
    PuniData *d;
    int n;
    int special;
    int div;
    int k;
    int m;
    Vec pos;
    k = 0;
    special = 0;
    for (i = 1; i <= PUNI_ALLY_LAST; i++) {
        p = &Punidat[i];
        if ((p->flags & PUNI_FLAG_ACTIVE) && (((p->anim) == 1) || ((p->anim) == 2))) {
            if (i == 3) {
                special++;
            } else {
                k++;
            }
        }
    }

    Punigp->puniMoveCount = k;
    if (k == 0) {
        Punigp->puniMoveLevel = 0;
        div = 5;
    } else
        if (k <= 2) {
        Punigp->puniMoveLevel = 1;
        div = 5;
    } else
        if (k <= 11) {
        Punigp->puniMoveLevel = 2;
        div = 4;
    } else
        if (k <= 30) {
        Punigp->puniMoveLevel = 3;
        div = 3;
    } else
        if (k <= 50) {
        Punigp->puniMoveLevel = 4;
        div = 2;
    } else
        if (k <= 70) {
        Punigp->puniMoveLevel = 5;
        div = 2;
    } else {
        Punigp->puniMoveLevel = 6;
        div = 2;
    }
    if (((Punigp->puniMoveLevel) != 0) && ((gp->retraceCount % div) == 0)) {
        m = Punigp->puniMoveCount;
        n = (rand() % m) + 1;
        k = 0;
        for (i = 1; i <= PUNI_ALLY_LAST; i++) {
            p = &Punidat[i];
            if ((p->flags & PUNI_FLAG_ACTIVE) && (((p->anim) == 1) || ((p->anim) == 2))) {
                k++;
                if (k == n) {
                    break;
                }
            }
        }

        pos = p->pos;
        m = Punigp->puniMoveLevel;
        switch ((rand() % m) + 1) {
            case 1:
                psndSFXOn_3D("SFX_STG2_PUNI_MOVE2_1", &pos);
                break;

            case 2:
                psndSFXOn_3D("SFX_STG2_PUNI_MOVE2_2", &pos);
                break;

            case 3:
                psndSFXOn_3D("SFX_STG2_PUNI_MOVE2_3", &pos);
                break;

            case 4:
                psndSFXOn_3D("SFX_STG2_PUNI_MOVE2_4", &pos);
                break;

            case 5:
                psndSFXOn_3D("SFX_STG2_PUNI_MOVE2_5", &pos);
                break;

            case 6:
                psndSFXOn_3D("SFX_STG2_PUNI_MOVE2_6", &pos);
                break;

        }

    }
    d = Punidat;
    if (special != 0) {
        if (((u32) ((s32)Punigp->sfxTogetti)) == 0) {
            Punigp->sfxTogetti = psndSFXOn_3D("SFX_STG2_TOGETTI_MOVE1", &d[3].pos);
        }
    } else
        if (((u32) ((s32)Punigp->sfxTogetti)) != 0) {
        psndSFXOff((s32)Punigp->sfxTogetti);
        Punigp->sfxTogetti = 0;
    }
    if (((u32) ((s32)Punigp->sfxTogetti)) != 0) {
        psndSFX_pos((s32)Punigp->sfxTogetti, &d[3].pos);
    }
    k = 0;
    for (i = PUNI_ENEMY_FIRST; i < PUNI_ENEMY_END; i++) {
        if ((Punidat[i].flags & PUNI_FLAG_ACTIVE) && ((Punidat[i].anim == 1) || (Punidat[i].anim == 2))) {
            k++;
        }
    }

    Punigp->togeMoveCount = k;
    if (k == 0) {
        Punigp->togeMoveLevel = 0;
    } else
        if (k <= 2) {
        Punigp->togeMoveLevel = 1;
    } else
        if (k <= 11) {
        Punigp->togeMoveLevel = 2;
    } else {
        Punigp->togeMoveLevel = 3;
    }
    if ((Punigp->togeMoveLevel) != (Punigp->togeMoveLevelPrev)) {
        if (((u32) ((s32)Punigp->sfx468)) != 0) {
            psndSFXOff((s32)Punigp->sfx468);
        }
        switch (Punigp->togeMoveLevel) {
            case 1:
                Punigp->sfx468 = psndSFXOn("SFX_STG2_TOGE_MOVE1");
                break;

            case 2:
                Punigp->sfx468 = psndSFXOn("SFX_STG2_TOGE_MOVE2");
                break;

            case 3:
                Punigp->sfx468 = psndSFXOn("SFX_STG2_TOGE_MOVE3");
                break;

        }

    }
    Punigp->togeMoveLevelPrev = Punigp->togeMoveLevel;
    Punigp->fightSfxCount1 = 0;
    Punigp->fightSfxCount2 = 0;
}

extern double fabs(double);

#if VERSION <= VERSION_DEMO_JP
static void puni_puni_main(void) {
#else
void puni_puni_main(void) {
#endif
    PuniTarget* tg;
    s32 i, k, j;
    MarioWork* mario;
    PuniData* p;
    PuniData* t;
    Vec diff;

    mario = marioGetPtr();
    t = Punigp->marioPuni;
    t->pos = mario->position;
    puniMapFindbuff(t->pos);
    puniModeALLCheck();
#if VERSION <= VERSION_DEMO_JP
    for (i = 0, p = Punidat + 1; i < 210; i++, p++) {
        p->modeHistIdx++;
        p->modeHistIdx &= 0x7F;
        p->modeHist[p->modeHistIdx] = p->mode;
    }
#else
    if (Punigp->flags & PUNI_WORK_FLAG_BATTLE_FLOOR) {
#endif
        for (i = 1, p = Punidat + 1; i < PUNI_DATA_MAX; i++, p++) {
            PSVECSubtract(&p->pos, &p->prevPos, &diff);
            p->moveDist = PSVECMag(&diff);
            if (!(p->flags & PUNI_FLAG_ACTIVE)) continue;
            p->flags &= ~(PUNI_FLAG_UPDATED | PUNI_FLAG_SENSED);
            p->flags2 &= ~PUNI_FLAG2_CROSSING_LEDGE;
            p->attacked = 0;
            puniParaRecover(p);
        }
#if VERSION >= VERSION_JP
    }
    if (Punigp->flags & PUNI_WORK_FLAG_BATTLE_FLOOR) {
#endif
        for (k = 0; k < 2; k++) {
            for (i = 1, p = Punidat + 1; i < PUNI_DATA_MAX; i++, p++) {
                if (!(p->flags & PUNI_FLAG_ACTIVE)) continue;
                if (k == 0) {
                    if (p->mode != PUNI_MODE_ATTACK) continue;
                    p->flags |= PUNI_FLAG_UPDATED;
                } else {
                    if (p->flags & PUNI_FLAG_UPDATED) continue;
                    p->flags |= PUNI_FLAG_UPDATED;
                }
                if (!(p->flags & PUNI_FLAG_SENSED)) {
                    p->flags |= PUNI_FLAG_SENSED;
                    if (p->flags2 & PUNI_FLAG2_DEATH_PENDING) {
                        p->flags2 &= ~PUNI_FLAG2_DEATH_PENDING;
                        p->mode = PUNI_MODE_DEAD;
                        p->modeStep = 0;
                    }
                    {
                        PartyEntry* party = partyGetPtr(marioGetPartyId());
                        p->breathPower = 0.0f;
                        if (Punigp->windDisabled == 0 && party != NULL) {
                            p->breathPower = cloudGetBreathPower(p->pos, 10.0f);
                        }
                    }
                    if (32.0 < __fabs(p->pos.y - mario->position.y)) {
                        p->marioDist = 1000.0f;
                    } else {
                        p->marioDist = PSVECDistance(&p->pos, &mario->position);
                    }
                    tg = Punigp->targets;
                    p->massTarget = -1;
                    p->massTargetDistSq = 1000000.0f;
                    p->awayTarget = -1;
                    p->awayTargetDistSq = 1000000.0f;
                    for (j = 0; j < 10; j++, tg++) {
                        if (tg->flags & PUNI_TARGET_FLAG_ON) {
                            if (!(32.0 < __fabs(p->pos.y - tg->pos.y))) {
                                if (tg->flags & PUNI_TARGET_FLAG_AWAY) {
                                    f32 d = PSVECSquareDistance(&p->pos, &tg->pos);
                                    if (p->awayTargetDistSq > d) {
                                        p->awayTarget = j;
                                        p->awayTargetDistSq = d;
                                    }
                                }
                                if (tg->flags & PUNI_TARGET_FLAG_MASS) {
                                    f32 d = PSVECSquareDistance(&p->pos, &tg->pos);
                                    if (p->massTargetDistSq > d) {
                                        p->massTarget = j;
                                        p->massTargetDistSq = d;
                                    }
                                }
                            }
                        }
                    }
                }
#if VERSION <= VERSION_DEMO_JP
                switch (p->mode) {
                    case PUNI_MODE_IDLE:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeNormal(p);
                        break;
                    case PUNI_MODE_JOIN:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeParentpre(p);
                        break;
                    case PUNI_MODE_PARENT:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeParent(p);
                        break;
                    case PUNI_MODE_SEARCH:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeSearch(p);
                        break;
                    case PUNI_MODE_MASS:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeMass(p);
                        break;
                    case PUNI_MODE_AWAYJUMP:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeAwayjump(p);
                        break;
                    case PUNI_MODE_AWAY:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeAway(p);
                        break;
                    case PUNI_MODE_ATTACK:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeAttack(p);
                        break;
                    case PUNI_MODE_DAMAGE:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeDamage(p);
                        break;
                    case PUNI_MODE_PIYORI:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModePiyori(p);
                        break;
                    case PUNI_MODE_ESCAPE:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeEscape(p);
                        break;
                    case PUNI_MODE_RECOVER:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeRecover(p);
                        break;
                    case PUNI_MODE_DEAD:
                        p->flags &= ~PUNI_FLAG_STRAY;

                        // FAKE: the (u32) round trip keeps the name out of constant propagation, so its address is hoisted into a saved
                        // register before the loop as in the demo.
                        puniModeDeadSfx(p, (const char*)(u32)"SFX_STG2_FIGHT_TOGE3");
                        break;
                    case PUNI_MODE_WIND:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeWind(p);
                        break;
                    case PUNI_MODE_STOP:
                        break;
                    case PUNI_MODE_CHECKPOS:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeCheckPos(p);
                        break;
                    case PUNI_MODE_HAGURE:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeHagure(p);
                        break;
                    case PUNI_MODE_PREUPDRAFT:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModePreupdraft(p);
                        break;
                    case PUNI_MODE_UPDRAFT:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeUpdraft(p);
                        break;
                    case PUNI_MODE_WONDER:
                        p->flags |= PUNI_FLAG_STRAY;

                        // FAKE: same (u32) round trip as PUNI_MODE_DEAD.
                        puniModeWonderSfx(p, (const char*)(u32) "SFX_STG2_SOAPBALL_BREAK1");
                        break;
                    case PUNI_MODE_FUNBARI:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeFunbari(p);
                        break;
                    case PUNI_MODE_WATER:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeWater(p);
                        break;
                    case PUNI_MODE_FALL:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeFall(p);
                        break;
                    case PUNI_MODE_DODAI:
                        p->flags |= PUNI_FLAG_STRAY;
                        puniModeDodai(p);
                        break;
                    case PUNI_MODE_TRAP:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeTrap(p);
                        break;
                    case PUNI_MODE_ENTER:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeEnter(p);
                        break;
                    case PUNI_MODE_EXIT:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeExit(p);
                        break;
                    case PUNI_MODE_DOKAN:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeDokan(p);
                        break;
                    case PUNI_MODE_POINT:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModePoint(p);
                        break;
                    case PUNI_MODE_FUNC:
                        p->flags &= ~PUNI_FLAG_STRAY;
                        puniModeCustom(p);
                        break;
#else
                puniModeMain(p);
            }
        }
        puniOption();
    } else {

        for (i = 1, p = Punidat + 1; i <= 109; i++, p++) {
            if (p->flags & PUNI_FLAG_ACTIVE) {
                PartyEntry* party;
                PuniTarget* tt;
                s32 jj;
                party = partyGetPtr(marioGetPartyId());

                p->breathPower = 0.0f;
                if (Punigp->windDisabled == 0 && party != NULL) {
                    p->breathPower = cloudGetBreathPower(p->pos, 10.0f);
                }
                if (32.0 < __fabs(p->pos.y - mario->position.y)) {
                    p->marioDist = 1000.0f;
                } else {
                    p->marioDist = PSVECDistance(&p->pos, &mario->position);
                }
                tt = Punigp->targets;
                p->massTarget = -1;
                p->massTargetDistSq = 1000000.0f;
                p->awayTarget = -1;
                p->awayTargetDistSq = 1000000.0f;
                for (jj = 0; jj < 10; jj++, tt++) {
                    if (tt->flags & 1) {
                        if (!(32.0 < __fabs(p->pos.y - tt->pos.y))) {
                            if (tt->flags & 4) {
                                f32 d = PSVECSquareDistance(&p->pos, &tt->pos);
                                if (p->awayTargetDistSq > d) {
                                    p->awayTarget = jj;
                                    p->awayTargetDistSq = d;
                                }
                            }
                            if (tt->flags & 2) {
                                f32 d = PSVECSquareDistance(&p->pos, &tt->pos);
                                if (p->massTargetDistSq > d) {
                                    p->massTarget = jj;
                                    p->massTargetDistSq = d;
#endif
                                }
                            }
                        }
#if VERSION <= VERSION_DEMO_JP
                        puniOption();
                        {
#else
                    }
                }
                puniModeMain(p);
            }
        }
        puniCollideAll();
    }
    if (Punigp->flags & PUNI_WORK_FLAG_BATTLE_FLOOR) {
#endif
        s32 ii;
        PuniData* q;
        PunidrvPose* ps;
        ps = puniGetPosePtr();
        for (ii = 1, q = Punidat + 1; ii < PUNI_DATA_MAX; ii++, ps++, q++) {
            if (q->flags & PUNI_FLAG_ACTIVE) {
                if (!(q->flags & PUNI_FLAG_MOVE_STOP)) {
                    q->prevPos = q->pos;
#if VERSION <= VERSION_DEMO_JP
                    switch (q->mode) {
                        case PUNI_MODE_IDLE:
                        case PUNI_MODE_JOIN:
                        case PUNI_MODE_PARENT:
                        case PUNI_MODE_SEARCH:
                        case PUNI_MODE_ATTACK:
                        case PUNI_MODE_ESCAPE:
                        case PUNI_MODE_PIYORI:
                        case PUNI_MODE_STOP:
                        case PUNI_MODE_HAGURE:
                        case PUNI_MODE_DODAI:
                        case PUNI_MODE_ENTER:
                        case PUNI_MODE_EXIT:
                        case PUNI_MODE_POINT:
                            puniMoveDecel(q);
                            puniSetYPosition(q);
                            break;
                        case PUNI_MODE_MASS:
                            puniMoveLimitedNormal(q);
                            puniSetYPosition(q);
                            break;
                        case PUNI_MODE_DAMAGE:
                            puniMoveNormal(q);
                            puniSetYPosition(q);
                            break;
                        case PUNI_MODE_FUNC:
                            puniMoveCustom(q);
                            puniSetYPosition(q);
                            break;
                        case PUNI_MODE_AWAYJUMP:
                        case PUNI_MODE_AWAY:
                        case PUNI_MODE_WIND:
                            puniSetYPosition(q);
                            break;
                        case PUNI_MODE_TRAP:
                            puniMoveDecel(q);
                            puniSetYPosition(q);
                            break;
                    }
#else
                    puniMoveMain(q);
                }
                puniAnimation(q, ps);
            }
        }
    } else {
        s32 ii;
        PuniData* q;
        PunidrvPose* ps;
        ps = puniGetPosePtr();

        for (ii = 1, q = Punidat + 1; ii <= 109; ii++, ps++, q++) {
            if (q->flags & PUNI_FLAG_ACTIVE) {
                if (!(q->flags & PUNI_FLAG_MOVE_STOP)) {
                    q->prevPos = q->pos;
                    puniMoveMain(q);
#endif
                }
                puniAnimation(q, ps);
            }
        }
    }
    puniSound();
    switch (Punigp->allMode) {
        case 2:
            if (Punigp->prevAllMode == 2) Punigp->allMode = 0;
            break;
        case 1: break;
        case 3: break;
    }
    Punigp->prevAllMode = Punigp->allMode;
}

/**
 * Queues the draw callbacks of the Puni system for the frame:
 * the Puni models, one shadow per Puni pose and the status icons.
 */
static void puni_puni_dispent(void) {
    PunidrvPose* pose;
    s32 i;
    Vec v;
    Vec tmp;

    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, puni_puni_disp, NULL);
    pose = puniGetPosePtr();
    for (i = 0; i < 210; i++) {
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
            tmp = zero;
        }
        tmp.x = (*pose->mtx)[0][3];
        tmp.y = (*pose->mtx)[1][3];
        tmp.z = (*pose->mtx)[2][3];
        v = tmp;
        dispEntry(CAMERA_3D, dispCalcZ(&v), DISP_MODE_XLU, puni_puni_shadow_disp, pose);
        pose++;
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, evt_icon_disp_func, NULL);
}

/**
 * Allocates and clears the Puni work area, the Puni data (211 slots) and the Puni texture-frame table.
 * Called at the start of every map's init script.
 */
USER_FUNC(evt_puni_mem_init) {
    Punigp = _mapAlloc(sizeof(PuniWork));
    memset(Punigp, 0, sizeof(PuniWork));
    Punidat = _mapAlloc(sizeof(PuniData) * PUNI_DATA_MAX);
    memset(Punidat, 0, sizeof(PuniData) * PUNI_DATA_MAX);
    Punitex = _mapAlloc(sizeof(PuniTexInfo) * PUNI_TEX_INFO_MAX);
    memset(Punitex, 0, sizeof(PuniTexInfo) * PUNI_TEX_INFO_MAX);
    return EVT_RETURN_DONE;
}

// FAKE: second copy of puniAnimationInit in its natural form (puniAnimCount for both tables); evt_puni_init
// inlines it and reaches 100%, while the out-of-line puniAnimationInit above only matches with its hand-written
// second loop. Same effect as the old open-coded loops (count up to the first -1/-2, store it in end[i]).
static inline void puniAnimationInit_inl(s16** a, s16** b) {
    PuniAnimSet* s0 = &Punigp->anim[0];
    PuniAnimSet* s1 = &Punigp->anim[1];
    s32 i;

    for (i = 0; i < 10; i++) {
        s0->table[i] = a[i];
        puniAnimCount(a[i], &s0->count[i], &s0->end[i]);
        s1->table[i] = b[i];
        puniAnimCount(b[i], &s1->count[i], &s1->end[i]);
    }
}

/**
 * Script call that starts the Puni drawing system:
 * on its first call it initializes the Puni renderer (puniInit);
 * afterwards it registers the animation patterns, pose names and the texture table, and finishes.
 * args[0] to args[3] are passed to puniEntry (draw and update hooks).
 */
s32 evt_puni_init(EventEntry *event, BOOL isFirstCall) {
    EvtWord *args = event->args;
    s32 a = evtGetValue(event, args[0]);
    PuniTexInfo *tex = Punitex;
    EvtWord b = evtGetValue(event, args[1]);
    EvtWord c = evtGetValue(event, args[2]);
    EvtWord d = evtGetValue(event, args[3]);
    if (isFirstCall) {
        puniInit();
        return EVT_RETURN_BLOCK;
    }
    puniAnimationInit_inl((s16**)puni_patern_table, (s16**)toge_patern_table);
    puniAnimationInit2();
    puniEntry(a, (PunidrvTexInfo*)tex, (void*)b, (void*)c, (void*)d);
    return EVT_RETURN_DONE;
}

/**
 * Declares this map's floor data to the Puni system.
 * args[0] = map number (stored as floor = number + 1 and written to GSW_MRI_PUNI_ROOM),
 * args[1] to args[4] = the heights of up to four floors, args[5] = height mode for puniSetYPosition (ymode),
 * args[6] = maze map kind (see evt_puni_set_maze).
 */
USER_FUNC(evt_puni_set_yuka) {
    EvtWord* args = event->args;
    Punigp->floor = evtGetValue(event, args[0]) + 1;
    Punigp->posY[0] = evtGetValue(event, args[1]);
    Punigp->posY[1] = evtGetValue(event, args[2]);
    Punigp->posY[2] = evtGetValue(event, args[3]);
    Punigp->posY[3] = evtGetValue(event, args[4]);
    Punigp->ymode = evtGetValue(event, args[5]);
    Punigp->mapKind = evtGetValue(event, args[6]);
    swByteSet(GSW_ID(GSW_MRI_PUNI_ROOM), Punigp->floor);
    return EVT_RETURN_DONE;
}

/**
 * Sets where a Puni that fell out of the map is put back (args[0] to args[2] = x, y, z) and the two height bands
 * around the floor heights a Puni must stay within (args[3], args[4] top and bottom of band 0, args[5],
 * args[6] of band 1; -1000 for none); see puniPositionCheck.
 */
USER_FUNC(evt_puni_set_hamari) {
    EvtWord* args = event->args;
    f32* v = &Punigp->recoverPos.x;
    Punigp->recoverPos.x = evtGetValue(event, args[0]);
    Punigp->recoverPos.y = evtGetValue(event, args[1]);
    Punigp->recoverPos.z = evtGetValue(event, args[2]);
    Punigp->stuckTop0 = evtGetValue(event, args[3]);
    Punigp->stuckBottom0 = evtGetValue(event, args[4]);
    Punigp->stuckTop1 = evtGetValue(event, args[5]);
    Punigp->stuckBottom1 = evtGetValue(event, args[6]);
    return EVT_RETURN_DONE;
}

static inline void puniMapDataSetI(s32 w, s32 h, u8* src, f32 x, f32 z) {
    Punimap->x = x;
    Punimap->z = z;
    if (src == NULL) {
        Punimap->enabled = 0;
    } else {
        Punimap->width = w;
        Punimap->height = h;
        Punimap->enabled = 1;
        Punimap->src = src;
        Punimap->inRange = 0;
        Punimap->size = h * (w * 4);
        Punimap->data = _mapAlloc(Punimap->size);

        // copies src onto itself (Punimap->src is src), a no-op; puniMapFindbuff fills Punimap->data from src
        // before it reads it
        memcpy(Punimap->src, src, Punimap->size);
    }
}

/**
 * Loads the maze map of the current room if it has one:
 * mri_06 (13 by 5), mri_09 (7 by 6; a second layout once the wall panels have rolled away)
 * and mri_16 (11 by 9, only after the water has been drained).
 */
USER_FUNC(evt_puni_set_maze) {
    switch (Punigp->mapKind) {
        case 0:
            break;
        case 1:
            puniMapDataSetI(13, 5, puniMapdata_MRI_06, 50.0f, -1.0f);
            break;
        case 2:
            if (swGet(GSWF_ID(GSWF_MRI_09_WALL_PUZZLE_SOLVED))) {
                puniMapDataSetI(7, 6, puniMapdata_MRI_09_01, 150.0f, -1.0f);
            } else {
                puniMapDataSetI(7, 6, puniMapdata_MRI_09, 150.0f, -1.0f);
            }
            break;
        case 3:
            if (swGet(GSWF_ID(GSWF_MRI_16_WATER_SWITCH_PRESSED))) {
                puniMapDataSetI(11, 9, puniMapdata_MRI_16, -269.0f, -320.0f);
            }
            break;
    }
    return EVT_RETURN_DONE;
}

/** Event that runs the Puni system's per-frame update (puniMain) forever. */
static USER_FUNC(evt_puni_main) {
    puniMain();
    return EVT_RETURN_BLOCK;
}

/** Returns whichever of the map's four floor heights (Punigp->posY) is closest to y. */
static f32 puni_init_posy(f32 y) {
    f32 best = Punigp->posY[0];
    f32 d = __fabs(y - best);
    f32 d2;

    d2 = __fabs(y - Punigp->posY[1]);
    if (d > d2) {
        d = d2;
        best = Punigp->posY[1];
    }
    d2 = __fabs(y - Punigp->posY[2]);
    if (d > d2) {
        d = d2;
        best = Punigp->posY[2];
    }
    d2 = __fabs(y - Punigp->posY[3]);
    if (d > d2) {
        best = Punigp->posY[3];
    }
    return best;
}

/**
 * Sets a new Puni up as a living ally at x, y, z:
 * hp 60, attack 20, defense 10, regen 10 per second, hit rate 50 (70 when guarding), radius 6 by 2 by 3;
 * no parent or children yet.
 */
static void puni_init_para(PuniData* p, f32 x, f32 y, f32 z) {
    p->modeStep = 0;
    p->anim = 0;
    p->prevAnim = 1000;
    p->pos.x = x;
    p->pos.y = y;
    p->pos.z = z;
    p->prevPos = p->pos;
    p->radius.x = 6.0f;
    p->radius.y = 2.0f;
    p->radius.z = 3.0f;
    p->hp = 60;
    p->maxHp = 60;
    p->regen = 10;
    p->regenTimer = 60;
    p->regenInterval = 60;
    p->attack = 20;
    p->defense = 10;
    p->reserved_582 = 10;
    p->damage = 0;
    p->attacked = 0;
    p->hitRate = 50;
    p->hitRateGuard = 70;
    p->parent = NULL;
    p->child1 = NULL;
    p->child2 = NULL;
    PuniDokuritsu(p);
    p->flags |= (PUNI_FLAG_PUNI | PUNI_FLAG_ACTIVE | PUNI_FLAG_WALL_CHECK);
}

/**
 * Sets a new Jabbi up as an enemy at x, y, z with the same stats as a Puni:
 * hp 60, attack 20, defense 10, collision on.
 */
static void toge_init_para(PuniData* p, f32 x, f32 y, f32 z) {
    p->anim = 0;
    p->modeStep = 0;
    p->prevAnim = 1000;
    p->pos.x = x;
    p->pos.y = y;
    p->pos.z = z;
    p->prevPos = p->pos;
    p->radius.x = 6.0f;
    p->radius.y = 2.0f;
    p->radius.z = 3.0f;
    p->hp = 60;
    p->maxHp = 60;
    p->regen = 10;
    p->regenTimer = 60;
    p->regenInterval = 60;
    p->attack = 20;
    p->defense = 10;
    p->reserved_582 = 10;
    p->damage = 0;
    p->attacked = 0;
    p->hitRate = 50;
    p->hitRateGuard = 70;
    p->flags |= (PUNI_FLAG_TOGE | PUNI_FLAG_ACTIVE | PUNI_FLAG_WALL_CHECK | PUNI_FLAG_COLLIDE);
}

/**
 * Script helper that scatters a position: args[0] = 0 for plus or minus 10 units,
 * 1 for plus or minus 50 units around the given x and z (args[1] and args[3]);
 * y becomes the closest floor height. Results are written back to args[1] to args[3].
 */
USER_FUNC(evt_puni_rnd_pos) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 px = args[1];
    s32 py = args[2];
    s32 pz = args[3];
    f32 x = evtGetFloat(event, px);
    f32 y = evtGetFloat(event, py);
    f32 ry, rx, rz, oy;
    f32 d, d2;
    f32 z;
    ry = Punigp->posY[0];
    d = __fabs(y - ry);
    d2 = __fabs(y - Punigp->posY[1]);
    if (d > d2) {
        d = d2;
        ry = Punigp->posY[1];
    }
    d2 = __fabs(y - Punigp->posY[2]);
    if (d > d2) {
        d = d2;
        ry = Punigp->posY[2];
    }
    d2 = __fabs(y - Punigp->posY[3]);
    if (d > d2) {
        ry = Punigp->posY[3];
    }
    z = evtGetFloat(event, pz);
    switch (type) {
        case 0:
            rx = randf(x - 10.0f, x + 10.0f);
            oy = ry;
            rz = randf(z - 10.0f, z + 10.0f);
            break;
        case 1:
            rx = randf(x - 50.0f, x + 50.0f);
            oy = ry;
            rz = randf(z - 50.0f, z + 50.0f);
            break;
    }
    evtSetFloat(event, px, rx);
    evtSetFloat(event, py, oy);
    evtSetFloat(event, pz, rz);
    return EVT_RETURN_DONE;
}

/** Returns the Puni system mode (Punigp->systemMode) in args[0]. */
USER_FUNC(evt_puni_get_systemmode) {
    evtSetValue(event, event->args[0], Punigp->systemMode);
    return EVT_RETURN_DONE;
}

/** Sets the Puni system mode (Punigp->systemMode) to args[0]. */
USER_FUNC(evt_puni_set_systemmode) {
    Punigp->systemMode = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/**
 * Blocks the script until the Puni system mode equals args[0];
 * scripts wait for mode 2 (the Puni system is up and running) before moving Punies.
 */
USER_FUNC(evt_puni_wait_systemmode) {
    s32 mode = evtGetValue(event, event->args[0]);
    return Punigp->systemMode != mode ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/**
 * Reads Puni number idx's saved position slot (7 bytes from LSW 0):
 * the floor number and x, y, z as signed 16-bit values.
 */
static inline void puni_swget_idx(s32 idx, s32* val, Vec* pos) {
    s32 off;
    u8 zHi, b5, b4, b3, b2, b1, b0;
    s32 x;
    s32 y;
    s32 z;

    off = (idx - 1) * 7;
    b0 = _swByteGet(off);
    b1 = _swByteGet(off + 1);
    b2 = _swByteGet(off + 2);
    b3 = _swByteGet(off + 3);
    b4 = _swByteGet(off + 4);
    b5 = _swByteGet(off + 5);
    zHi = _swByteGet(off + 6);

    *val = b0;
    x = b1 + (b2 << 8);
    if (x & 0x8000) {
        x |= 0xFFFF0000;
    }
    y = b3 + (b4 << 8);
    if (y & 0x8000) {
        y |= 0xFFFF0000;
    }
    z = b5 + (zHi << 8);
    if (z & 0x8000) {
        z |= 0xFFFF0000;
    }
    pos->x = x;
    pos->y = y;
    pos->z = z;
}

/**
 * Debug listing: formats a line per Puni (following Mario, lost in some map with its position, not yet joined,
 * caught in a trap, or waiting at the cell in mri_03) into a buffer; the text is never shown.
 */
static s32 evt_puni_debug_info(EventEntry *event, BOOL isFirstCall) {
    char buf[256];
    s32 i;
    for (i = 1; i <= PUNI_ALLY_LAST; i++) {
        s32 b0;
        Vec pos;
        s32 a;
        s32 trap;
        f32 fx;
        f32 fy;
        f32 fz;
        puni_swget_idx(i, &b0, &pos);
        fx = pos.x;
        fy = pos.y;
        fz = pos.z;
        if (i >= PUNI_SOLID_FIRST) {
            a = 0;
        } else {
            a = 8;
            trap = swByteGet((i / 8) + GSW_MRI_PUNI_TRAPPED_BITS);
            a = (trap >> (i % a)) & 1;
        }
        trap = 0;
        if (i == 1) {
            if (!swGet(GSWF_ID(GSWF_MRI_PUNIO_JOINED))) {
                trap = 1;
            }
        } else
            if (i == 2) {
            if (!swGet(GSWF_ID(GSWF_MRI_PETUNI_JOINED))) {
                trap = 1;
            }
        } else
            if (i == 4) {
            if (!swGet(GSWF_ID(GSWF_MRI_PUNIPER_JOINED))) {
                trap = 1;
            }
        } else
            if (i == 3) {
            if (!swGet(GSWF_ID(GSWF_MRI_JABBLE_JOINED))) {
                trap = 1;
            }
        } else
            if ((swByteGet(GSW_ID(GSW_MRI_PUNI_FOLLOWER_COUNT)) + 5) <= i) {
            trap = 1;
        }
        if (a == 0) {
            if (trap == 0) {
                if (b0 == 0) {
                    sprintf(buf, "No%03d,ついて来ています\n", i);
                } else {
                    sprintf(buf, "No%03d,はぐれています area = mri_%02d, x = %04d, y = %04d, z = %04d,\n", i, b0 - 1, (s32) fx, (s32) fy, (s32) fz);
                }
            } else {
                sprintf(buf, "No%03d,＜まだ仲間になっていません＞\n", i);
            }
        } else {
            sprintf(buf, "No%03d,＜トラップにかかっています＞\n", i);
        }
        if ((i == 1) && swGet(GSWF_ID(GSWF_MRI_PUNIO_WAITING_AT_JAIL))) {
            sprintf(buf, "No%03d,＜mri_03の牢屋に滞在しています＞\n", i);
        }
    }

    return 2;
}

/**
 * Sets up the Puni crowd when a map starts (called through mri_puni_init).
 * args[0] = 2 means the map was entered through a loading zone, whose direction in args[2] sets the entry flags;
 * Punies that Mario had with him (those the story has recruited: Punio, his sister, the little Jabbi,
 * the rival and the others numbered 5 to GSW_MRI_PUNI_FOLLOWER_COUNT+4) are placed either beside Mario after a save load,
 * walking in from the loading zone, or at their saved lost position; trapped ones are skipped.
 * The blockers 103 to 109 are placed at the down pipes.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(evt_puni_setup) {
#else
USER_FUNC(evt_puni_setup) {
#endif
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    s32 a0 = evtGetValue(event, args[0]);
    s32 a1;
    s32 i;
    PuniData* p;
    BeroEntry** bero;
    s32 n;
    evtGetValue(event, args[1]);
    a1 = evtGetValue(event, args[2]);

    Punigp->allMode = 0;
    if (a0 == 2) {
        Punigp->flags &= ~(PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST | PUNI_WORK_FLAG_PIPE_UP | PUNI_WORK_FLAG_PIPE_DOWN);
        switch (a1) {
            case BERO_DIR_W:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_WEST;
                break;
            case BERO_DIR_E:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_EAST;
                break;
            case 8:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_UP;
                break;
            case 9:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_DOWN;
                break;
        }
    }
#if VERSION >= VERSION_JP
    if (Punigp->floor == 9 || Punigp->floor == 12) {
        Punigp->flags |= PUNI_WORK_FLAG_BATTLE_FLOOR;
    }
#endif
    if (swByteGet(GSW_ID(GSW_MRI_PUNI_ROOM)) == 1) {
        return EVT_RETURN_DONE;
    }
    p = &Punidat[1];
    for (i = 1; i <= 109; i++, p++) {
        s32 b0;
        Vec pos;
        f32 fx, fy, fz;
        s32 a;
        f32 sx, sy, sz;
        if (p->flags & PUNI_FLAG_KEEP) {
            p->flags &= ~PUNI_FLAG_KEEP;
            continue;
        }
        if (i >= PUNI_SOLID_FIRST) {
            p->flags2 |= PUNI_FLAG2_OBSTACLE;
            continue;
        }
        puni_swget_idx(i, &b0, &pos);
        fx = pos.x;
        fy = pos.y;
        fz = pos.z;
        if (b0 != 0 && b0 != Punigp->floor) continue;
        puniswTrapGetindex(i, &a);
        if (a != 0) continue;
        if (i == 1) {
            if (!swGet(GSWF_ID(GSWF_MRI_PUNIO_JOINED))) continue;
            if (swGet(GSWF_ID(GSWF_MRI_PUNIO_WAITING_AT_JAIL))) continue;
            Punigp->guidePuni = p;
            p->flags |= PUNI_FLAG_NPC_GUIDE;
        } else if (i == 2) {
            if (!swGet(GSWF_ID(GSWF_MRI_PETUNI_JOINED))) continue;
            Punigp->sisterPuni = p;
            p->flags |= PUNI_FLAG_NPC_GUIDE_SISTER;
        } else if (i == 4) {
            if (!swGet(GSWF_ID(GSWF_MRI_PUNIPER_JOINED))) continue;
            Punigp->rivalPuni = p;
            p->flags |= PUNI_FLAG_NPC_RIVAL;
        } else if (i == 3) {
            if (!swGet(GSWF_ID(GSWF_MRI_JABBLE_JOINED))) continue;
            Punigp->jabbiPuni = p;
            p->flags |= PUNI_FLAG_NPC_CHIBI_TOGE;
        } else {
            if (swByteGet(GSW_ID(GSW_MRI_PUNI_FOLLOWER_COUNT)) + 5 <= i) continue;
            p->flags |= PUNI_FLAG_SPRITE_PUNI;
        }
        if (b0 == 0) {
            if (gp->flags & GW_FLAG_SAVE_LOADED) {
                sx = randf(mario->position.x - 10.0f, mario->position.x + 10.0f);
                sy = mario->position.y;
                sz = randf(mario->position.z - 10.0f, mario->position.z + 10.0f);
                p->mode = PUNI_MODE_JOIN;
            } else {
                f32 ex, ey, ez;
                f32 d, e;
                sx = bero_get_BeroSX();
                sy = puni_init_posy(bero_get_BeroSY());
                sz = bero_get_BeroSZ();
                ex = bero_get_BeroEX();
                ey = puni_init_posy(bero_get_BeroEY());
                ez = bero_get_BeroEZ();
                if (!(Punigp->flags & PUNI_WORK_FLAG_PIPE_DOWN)) {
                    sx = randf(sx - 10.0f, sx + 10.0f);
                    sz = randf(sz - 10.0f, sz + 10.0f);
                } else {
                    ez += 60.0f;
                }
                p->moveStart.x = randf(ex - 10.0f, ex + 10.0f);
                p->moveStart.y = ey;
                p->moveStart.z = randf(ez - 10.0f, ez + 10.0f);
                p->mode = PUNI_MODE_ENTER;
                p->flags &= ~PUNI_FLAG_WALL_CHECK;
            }
        } else {
            sx = fx;
            sy = fy;
            sz = fz;
            p->mode = PUNI_MODE_CHECKPOS;
            p->flags |= PUNI_FLAG_STRAY;
        }
        p->modeStep = 0;
        p->anim = 0;
        p->prevAnim = 1000;
        p->pos.x = sx;
        p->pos.y = sy;
        p->pos.z = sz;
        p->prevPos = p->pos;
        p->radius.x = 6.0f;
        p->radius.y = 2.0f;
        p->radius.z = 3.0f;
        p->hp = 60;
        p->maxHp = 60;
        p->regen = 10;
        p->regenTimer = 60;
        p->regenInterval = 60;
        p->attack = 20;
        p->defense = 10;
        p->reserved_582 = 10;
        p->damage = 0;
        p->attacked = 0;
        p->hitRate = 50;
        p->hitRateGuard = 70;
        p->parent = NULL;
        p->child1 = NULL;
        p->child2 = NULL;
        PuniDokuritsu(p);
        p->flags |= (PUNI_FLAG_PUNI | PUNI_FLAG_ACTIVE | PUNI_FLAG_WALL_CHECK);
    }
    bero = bero_get_ptr();
    n = bero_get_BeroNUM();
    {
    s32 j;
    for (j = 0; j < n; j++) {
        if (bero[j]->directionId == BERO_DIR_DOWN) {
            PuniData* q = Punidat + j + PUNI_SOLID_FIRST;
            q->flags |= (PUNI_FLAG_ACTIVE | PUNI_FLAG_COLLIDE);
            q->prevMode = PUNI_MODE_DOKAN;
            q->mode = PUNI_MODE_DOKAN;
            q->radius.x = 25.0f;
            hitObjGetPos(bero[j]->hitName, &q->pos);
            q->pos.y = puni_init_posy(q->pos.y);
        }
    }
    }
    return EVT_RETURN_DONE;
}

/**
 * Sets up the Jabbi for a map: args[0] = 0 none, 1 all 100 Jabbi, 2 the first 10;
 * args[1] to args[3] give the centre (x, y, z).
 * Each Jabbi is placed at a random spot within 50 units in x and 120 in z, with the stats of toge_init_para.
 */
static s32 evt_toge_setup(EventEntry *event, BOOL isFirstCall) {
    EvtWord *args = event->args;
    s32 end = 1;
    s32 kind = evtGetValue(event, args[0]);
    f32 x = evtGetValue(event, args[1]);
    f32 y = evtGetValue(event, args[2]);
    f32 z = evtGetValue(event, args[3]);
    PuniData *p;
    switch (kind) {
        case 0:
            return 2;

        case 1:
            end = 210;
            break;

        case 2:
            end = 120;
            break;

    }

    for (kind = PUNI_ENEMY_FIRST; kind < end; kind++) {
        f32 rx;
        f32 rz;
        p = &Punidat[kind];
        rx = randf(x - 50.0f, x + 50.0f);
        rz = randf(z - 120.0f, z + 120.0f);
        p->flags |= PUNI_FLAG_SPRITE_TOGE;
        p->mode = PUNI_MODE_IDLE;
        p->index = kind;
        p->anim = 0;
        p->modeStep = 0;
        p->prevAnim = 1000;
        p->pos.x = rx;
        p->pos.y = y;
        p->pos.z = rz;
        p->prevPos = p->pos;
        p->radius.x = 6.0f;
        p->radius.y = 2.0f;
        p->radius.z = 3.0f;
        p->hp = 60;
        p->maxHp = 60;
        p->regen = 10;
        p->regenTimer = 60;
        p->regenInterval = 60;
        p->attack = 20;
        p->defense = 10;
        p->reserved_582 = 10;
        p->damage = 0;
        p->attacked = 0;
        p->hitRate = 50;
        p->hitRateGuard = 70;
        p->flags |= (PUNI_FLAG_TOGE | PUNI_FLAG_ACTIVE | PUNI_FLAG_WALL_CHECK | PUNI_FLAG_COLLIDE);
    }

    return 2;
}

/**
 * Tests the straight line from one point to another against the map:
 * if nothing is in the way the Puni is moved to the end point;
 * if the two points coincide in x and z it is moved to the start point.
 * Returns TRUE when it moved the Puni, FALSE if a wall is in the way.
 */
static BOOL _puniVAtari(PuniData* p, Vec* from, Vec* to) {
    Vec start;
    Vec dir;
    Vec hit;
    Vec nrm;
    f32 dist;

    PSVECSubtract(to, from, &dir);
    if (0.0f == dir.x && 0.0f == dir.z) {
        p->pos = *from;
        return TRUE;
    }
    start = *from;
    PSVECNormalize(&dir, &dir);
    dist = PSVECMag(&dir);
    if (hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &dist, &nrm.x, &nrm.y, &nrm.z, start.x, start.y, start.z, dir.x, dir.y, dir.z) ==
        NULL) {
        p->pos = *to;
        return TRUE;
    }
    return FALSE;
}

static const Vec puni_fall_dir;

// Integer part of v, one lower for negative v (so not a true floor for negative whole numbers).
static inline s32 puniFloor(f32 v) {
    s32 i = v;
    if (v < 0.0f) {
        i--;
    }
    return i;
}

/**
 * Finds a stray Puni's resting spot: for Puni in the falling, bubble or water modes it is parked below the map,
 * otherwise it snaps to the first of the four whole grid points around its position that has a clear line to it.
 */
static void puniHagurePos(PuniData* p) {
    Vec cur;
    Vec a;
    Vec c;
    Vec b;
    Vec d;
    s32 ix;
    s32 iz;
    s32 ix1;
    s32 iz1;

    switch (p->mode) {
        case PUNI_MODE_FALL:
        case PUNI_MODE_PREUPDRAFT:
        case PUNI_MODE_UPDRAFT:
        case PUNI_MODE_FUNBARI:
            case PUNI_MODE_WATER: {
                    Vec v_ = {0.0f, -1000.0f, 0.0f};
                    p->pos = v_;
                }
                return;
        default:
            break;
    }
    cur = p->pos;
    ix = puniFloor(cur.x);
    ix1 = ix + 1;
    iz = puniFloor(cur.z);
    iz1 = iz + 1;
    a.x = ix;
    a.y = cur.y;
    a.z = iz;
    b.x = ix1;
    b.y = cur.y;
    b.z = iz1;
    c.x = ix1;
    c.y = cur.y;
    c.z = iz;
    d.x = ix;
    d.y = cur.y;
    d.z = iz1;
    if (_puniVAtari(p, &cur, &a)) return;
    if (_puniVAtari(p, &cur, &c)) return;
    if (_puniVAtari(p, &cur, &b)) return;
    if (_puniVAtari(p, &cur, &d)) return;
}

/** The zero vector, used as the default exit position. */
static const Vec exit_zero0 = {0.0f, 0.0f, 0.0f};

/**
 * Writes a Puni's save slot (floor number and x, y, z as signed 16-bit values,
 * 7 bytes starting at LSW offset off); the exit routine's copy of puniswSetindex.
 */
static inline void puni_exit_swset(s32 off, s32 val, Vec* pos) {
    u8 yh, xh;
    s32 x;
    s32 y;
    s32 z;
    x = pos->x;
    y = pos->y;
    z = pos->z;
    xh = ((x & 0xFF00) >> 8) & 0xFFu;
    yh = (y & 0xFF00) >> 8;
    _swByteSet(off, val);
    _swByteSet(off + 1, x & 0xFF);
    _swByteSet(off + 2, xh);
    _swByteSet(off + 3, y & 0xFF);
    _swByteSet(off + 4, yh);
    _swByteSet(off + 5, z & 0xFF);
    _swByteSet(off + 6, (z & 0xFF00) >> 8);
}

// Kept: the target needs dead assignments kept in this block; no natural form found (cleanup_notes/mweppc_pilot.md).
#pragma push
#pragma opt_dead_assignments off
static const Vec puni_fall_dir;

/**
 * Called when Mario leaves the map (args[0] = 2 for a loading zone, args[2] its direction,
 * args[3] the name of the next map or NULL).
 * It prepares the exit walk (loading-zone start and end points),
 * saves the positions of stray Punies in their LSW slots,
 * and sends the others walking out through the zone (or just stops them when the next map is mri_00);
 * when going to mri_00 all saved slots are cleared.
 */
static USER_FUNC(evt_puni_exit) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    PuniData* p;
    const char* map;
    f32 sx, sy, sz;
    f32 ex, ey, ez;
    s32 notMri00;
    s32 i;
    Vec pos;
    s32 o1;
    s32 o2;
    s32 j;

    evtGetValue(event, args[1]);

    // FAKE: the exit direction is read into i, so the first loop's counter is a second web of i (register colouring)
    i = evtGetValue(event, args[2]);
    map = (const char*)evtGetValue(event, args[3]);

    Punigp->allMode = 5;
    if (mode == 2) {
        Punigp->flags &= ~(PUNI_WORK_FLAG_PIPE_WEST | PUNI_WORK_FLAG_PIPE_EAST | PUNI_WORK_FLAG_PIPE_UP | PUNI_WORK_FLAG_PIPE_DOWN);
        switch (i) {
            case BERO_DIR_W:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_WEST;
                break;
            case BERO_DIR_E:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_EAST;
                break;
            case 8:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_UP;
                break;
            case 9:
                Punigp->flags |= PUNI_WORK_FLAG_PIPE_DOWN;
                break;
        }
    }
    if (map == NULL) {
        Punigp->flags |= PUNI_WORK_FLAG_NO_NEXT_MAP;
    } else {
        Punigp->flags &= ~PUNI_WORK_FLAG_NO_NEXT_MAP;
    }
    if (!(Punigp->flags & PUNI_WORK_FLAG_PIPE_DOWN)) {
        sx = bero_get_BeroSX();
        sy = puni_init_posy(bero_get_BeroSY());
        sz = bero_get_BeroSZ();
        ex = bero_get_BeroEX();
        ey = puni_init_posy(bero_get_BeroEY());
        ez = bero_get_BeroEZ();
    } else {
        sx = bero_get_BeroSX();
        sy = puni_init_posy(bero_get_BeroSY());
        sz = bero_get_BeroSZ();
        ex = sx;
        ey = sy;
        ez = sz;
        sz += 60.0f;
    }
    Punigp->beroMode = 2;
    Punigp->beroStart.x = sx;
    Punigp->beroStart.y = sy;
    Punigp->beroStart.z = sz;
    Punigp->beroStartRange = 100.0f;
    Punigp->beroEnd.x = ex;
    Punigp->beroEnd.y = ey;
    Punigp->beroEnd.z = ez;
    Punigp->beroEndRange = 100.0f;
    if (map != NULL) {
        s32 r = strcmp(map, "mri_00");
        notMri00 = r;
    } else {
        notMri00 = 1;
    }
    for (i = 1, p = &Punidat[1]; i <= PUNI_ALLY_LAST; i++, p++) {
        if (!(p->flags & PUNI_FLAG_ACTIVE) || (p->flags & PUNI_FLAG_NOT_IN_PARTY)) {
            continue;
        }
        if (p->flags & PUNI_FLAG_STRAY) {
            if (map == NULL) {
                p->flags |= PUNI_FLAG_KEEP;
            } else {
                puniHagurePos(p);
                pos = p->pos;

                // FAKE: slot offsets computed by the caller into named locals o1/o2 (puni_exit_swset takes the offset)
                o1 = (p->index - 1) * 7;
                puni_exit_swset(o1, (u8)Punigp->floor, &pos);
            }
        } else {
            pos = exit_zero0;
            o2 = (p->index - 1) * 7;
            puni_exit_swset(o2, 0, &pos);
            PuniDokuritsu(p);
            if (notMri00 == 0) {
                p->mode = PUNI_MODE_STOP;
                p->modeStep = 0;
                p->anim = 0;
            } else {
                p->mode = PUNI_MODE_EXIT;
                p->modeStep = 0;
                p->anim = 2;
            }
        }
    }
    if (map != NULL && strcmp(map, "mri_00") == 0) {
        for (j = 1; j <= PUNI_ALLY_LAST; j++) {
            Vec zero = {0.0f, 0.0f, 0.0f};

            // FAKE: the dead mode variable reused for this loop's slot offset (register colouring)
            mode = (j - 1) * 7;
            puni_exit_swset(mode, 0, &zero);
        }
    }
    return EVT_RETURN_DONE;
}
#pragma pop

USER_FUNC(evt_puni_init_puni) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    PuniData* p = &Punidat[idx];
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    s32 chk = evtGetValue(event, args[5]);

    if (idx == 1) {
        if (chk != 0 && swGet(GSWF_ID(GSWF_MRI_PUNIO_JOINED)) == 0) {
            return EVT_RETURN_DONE;
        }
        if (chk != 0 && swGet(GSWF_ID(GSWF_MRI_PUNIO_WAITING_AT_JAIL)) != 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->guidePuni = p;
        p->flags |= PUNI_FLAG_NPC_GUIDE;
    } else if (idx == 2) {
        if (chk != 0 && swGet(GSWF_ID(GSWF_MRI_PETUNI_JOINED)) == 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->sisterPuni = p;
        p->flags |= PUNI_FLAG_NPC_GUIDE_SISTER;
    } else if (idx == 4) {
        if (chk != 0 && swGet(GSWF_ID(GSWF_MRI_PUNIPER_JOINED)) == 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->rivalPuni = p;
        p->flags |= PUNI_FLAG_NPC_RIVAL;
    } else if (idx == 3) {
        if (chk != 0 && swGet(GSWF_ID(GSWF_MRI_JABBLE_JOINED)) == 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->jabbiPuni = p;
        p->flags |= PUNI_FLAG_NPC_CHIBI_TOGE;
    } else {
        if (chk != 0 && swByteGet(GSW_ID(GSW_MRI_PUNI_FOLLOWER_COUNT)) + 5 <= idx) {
            return EVT_RETURN_DONE;
        }
        p->flags |= PUNI_FLAG_SPRITE_PUNI;
    }
    p->mode = type;
    p->modeStep = 0;
    p->anim = 0;
    p->prevAnim = 1000;
    p->pos.x = x;
    p->pos.y = y;
    p->pos.z = z;
    p->prevPos = p->pos;
    p->radius.x = 6.0f;
    p->radius.y = 2.0f;
    p->radius.z = 3.0f;
    p->hp = 60;
    p->maxHp = 60;
    p->regen = 10;
    p->regenTimer = 60;
    p->regenInterval = 60;
    p->attack = 20;
    p->defense = 10;
    p->reserved_582 = 10;
    p->damage = 0;
    p->attacked = 0;
    p->hitRate = 50;
    p->hitRateGuard = 70;
    p->parent = NULL;
    p->child1 = NULL;
    p->child2 = NULL;
    PuniDokuritsu(p);
    p->flags |= (PUNI_FLAG_PUNI | PUNI_FLAG_ACTIVE | PUNI_FLAG_WALL_CHECK);
    return EVT_RETURN_DONE;
}

/**
 * Script call that brings Jabbi number args[0] (mode args[1]) to life at x, y, z (args[2] to args[4]):
 * an enemy with the same stats as a Puni and collision on.
 */
USER_FUNC(evt_puni_init_teki) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    PuniData* p = &Punidat[idx];
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);

    p->flags |= PUNI_FLAG_SPRITE_TOGE;
    p->mode = type;
    p->anim = 0;
    p->modeStep = 0;
    p->prevAnim = 1000;
    p->pos.x = x;
    p->pos.y = y;
    p->pos.z = z;
    p->prevPos = p->pos;
    p->radius.x = 6.0f;
    p->radius.y = 2.0f;
    p->radius.z = 3.0f;
    p->hp = 60;
    p->maxHp = 60;
    p->regen = 10;
    p->regenTimer = 60;
    p->regenInterval = 60;
    p->attack = 20;
    p->defense = 10;
    p->reserved_582 = 10;
    p->damage = 0;
    p->attacked = 0;
    p->hitRate = 50;
    p->hitRateGuard = 70;
    p->flags |= (PUNI_FLAG_TOGE | PUNI_FLAG_ACTIVE | PUNI_FLAG_WALL_CHECK | PUNI_FLAG_COLLIDE);
    return EVT_RETURN_DONE;
}

/** Script helper: returns the position (x, y, z as integers) of Puni args[0] in args[1] to args[3]. */
USER_FUNC(evt_puni_get_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], (s32)p->pos.x);
    evtSetValue(event, args[2], (s32)p->pos.y);
    evtSetValue(event, args[3], (s32)p->pos.z);
    return EVT_RETURN_DONE;
}

/** Script helper: moves Puni args[0] to x, y, z (args[1] to args[3]) at once, also resetting its previous position. */
USER_FUNC(evt_puni_set_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    p->pos.x = evtGetValue(event, args[1]);
    p->pos.y = evtGetValue(event, args[2]);
    p->pos.z = evtGetValue(event, args[3]);
    p->prevPos = p->pos;
    return EVT_RETURN_DONE;
}

/** Script helper: returns the facing direction of Puni args[0] in args[1]. */
USER_FUNC(evt_puni_get_dir) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], (s32)Punidat[evtGetValue(event, args[0])].dir);
    return EVT_RETURN_DONE;
}

/** Script helper: sets the facing direction of Puni args[0] to args[1]. */
USER_FUNC(evt_puni_set_dir) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    p->dir = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: returns the animation number of Puni args[0] in args[1]. */
USER_FUNC(evt_puni_get_anim) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], Punidat[evtGetValue(event, args[0])].anim);
    return EVT_RETURN_DONE;
}

/** Script helper: sets the animation of Puni args[0] to args[1] and forces it to restart. */
USER_FUNC(evt_puni_set_anim) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    p->anim = evtGetValue(event, args[1]);
    p->prevAnim = 1000;
    return EVT_RETURN_DONE;
}

/**
 * Rolling animation (anim 5): while Puni args[0] has that animation,
 * lifts it by 8 units and spins it by args[1] a frame, blocking the script.
 * When the animation changes it puts the height back and sets the spin to args[2].
 */
USER_FUNC(evt_puni_korokoro) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];

    switch (p->anim) {
        case 5:
            if (isFirstCall) {
                if (p->flags & PUNI_FLAG_SPRITE_PUNI) {
                    p->rotOffsetY = 8.0f;
                } else {
                    p->dispOfs.y = 8.0f;
                }
            }
            p->spin += evtGetValue(event, args[1]);
            return EVT_RETURN_BLOCK;
        default:
            if (p->flags & PUNI_FLAG_SPRITE_PUNI) {
                p->rotOffsetY = 0.0f;
            } else {
                p->dispOfs.y = 0.0f;
            }
            p->spin = evtGetValue(event, args[2]);
            return EVT_RETURN_DONE;
    }
}

/**
 * Blocking move: slides Puni args[0] in a straight line to x, z (args[1], args[2]) over args[3] ms,
 * keeping its height.
 */
USER_FUNC(evt_puni_move_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    f32 x = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    s32 durationMs = evtGetValue(event, args[3]);
    u32 t;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        p->moveStart = p->pos;
        p->moveEnd.x = x;
        p->moveEnd.y = p->pos.y;
        p->moveEnd.z = z;
    }
    if ((s32)(t = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime)) < durationMs) {
        p->pos.x = intplGetValue(EASING_LINEAR, p->moveStart.x, p->moveEnd.x, t, durationMs);
        p->pos.z = intplGetValue(EASING_LINEAR, p->moveStart.z, p->moveEnd.z, t, durationMs);
        p->prevPos = p->pos;
        return EVT_RETURN_BLOCK;
    }
    p->pos = p->moveEnd;
    p->prevPos = p->pos;
    return EVT_RETURN_DONE;
}

/**
 * Blocking move at a set speed: slides Puni args[0] in a straight line to x,
 * z (args[1], args[2]) at args[3] units per second, keeping its height.
 */
USER_FUNC(evt_puni_move_position2) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    f32 x = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    f32 speed = evtGetFloat(event, args[3]);
    f32 dist;
    s32 t;
    Vec dir;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        p->moveStart = p->pos;
        p->moveEnd.x = x;
        p->moveEnd.y = p->pos.y;
        p->moveEnd.z = z;
    }
    speed = sysFrame2SecFloat(speed);
    dist = PSVECDistance(&p->moveStart, &p->moveEnd);
    t = (s32)(0.5f + sysMsec2FrameFloat((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime)));
    PSVECSubtract(&p->moveEnd, &p->moveStart, &dir);
    PSVECNormalize(&dir, &dir);
    if (t < (s32)(dist / speed)) {
        PSVECScale(&dir, &dir, speed * t);
        PSVECAdd(&p->moveStart, &dir, &p->pos);
        return EVT_RETURN_BLOCK;
    }
    p->pos = p->moveEnd;
    p->prevPos = p->pos;
    return EVT_RETURN_DONE;
}

/**
 * Blocking jump: Puni args[0] jumps along a parabola to x, y, z (args[1] to args[3]) over args[4] ms,
 * rising args[5] units at the top, with the jump sound at the start.
 */
USER_FUNC(evt_puni_jump_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    f32 x = evtGetValue(event, args[1]);
    f32 y = evtGetValue(event, args[2]);
    f32 z = evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    f32 h = evtGetValue(event, args[5]);
    s32 t;
    f32 r;

    if (isFirstCall) {
        p->startTime = gp->renderTime;
        p->moveStart = p->pos;
        p->modeF0 = -4.0f * h;
        p->modeF1 = (y - p->moveStart.y) - p->modeF0;
        psndSFXOn_3D("SFX_STG2_PUNI_JUMP1", &p->pos);
    }
    t = OSTicksToMilliseconds((u32)gp->renderTime - (u32)p->startTime);
    if (t < frames) {
        r = (f32)t / (f32)frames;
        p->pos.x = intplGetValue(EASING_LINEAR, p->moveStart.x, x, (s32)t, frames);
        p->pos.z = intplGetValue(EASING_LINEAR, p->moveStart.z, z, (s32)t, frames);
        p->pos.y = r * (p->modeF0 * r + p->modeF1) + p->moveStart.y;
        return EVT_RETURN_BLOCK;
    }
    p->pos.x = x;
    p->pos.y = y;
    p->pos.z = z;
    return EVT_RETURN_DONE;
}

/** Script helper: returns the mode of Puni args[0] in args[1]. */
USER_FUNC(evt_puni_get_mode) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], Punidat[evtGetValue(event, args[0])].mode);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: forces Puni args[0] into mode args[1] and detaches it from the follow chain,
 * removing its effects.
 */
USER_FUNC(evt_puni_set_mode) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    s32 mode = evtGetValue(event, args[1]);
    p->mode = mode;
    p->prevMode = mode;
    p->modeStep = 0;
    p->prevAnim = 1000;
    PuniDokuritsu(p);
    puniDeleteEffect(p);
    return EVT_RETURN_DONE;
}

/** Script helper: cuts the follow chain and effect of Puni args[0] if it is alive (setsudan = "cut off"). */
USER_FUNC(evt_puni_setsudan) {
    PuniData* p = &Punidat[evtGetValue(event, event->args[0])];
    if (p->flags & PUNI_FLAG_ACTIVE) {
        PuniDokuritsu(p);
        puniDeleteEffect(p);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: returns the flag word of Puni args[0] in args[1]. */
USER_FUNC(evt_puni_get_flag) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], Punidat[evtGetValue(event, args[0])].flags);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: sets or clears the flags in mask args[2] of Puni args[0];
 * args[1] = 0 sets them, otherwise clears them.
 */
USER_FUNC(evt_puni_flag_onoff) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    s32 on = evtGetValue(event, args[1]);
    s32 mask = evtGetValue(event, args[2]);
    if (on) {
        p->flags &= ~mask;
    } else {
        p->flags |= mask;
    }
    return EVT_RETURN_DONE;
}

/**
 * Turns a down-pipe blocker on or off: args[0] is the pipe id, args[1] = 0 turns its collision on,
 * anything else turns it off. The blockers are the solid slots after the real Punies.
 */
USER_FUNC(evt_puni_dokan_onoff) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    s32 slot = bero_id_filter(id);
    PuniData* p = &Punidat[slot] + PUNI_SOLID_FIRST;
    if (on == 0) {
        p->flags |= PUNI_FLAG_COLLIDE;
    } else {
        p->flags &= ~PUNI_FLAG_COLLIDE;
    }
    return EVT_RETURN_DONE;
}

/**
 * Inline helper that reads a Puni's save slot: the floor byte and the x, y,
 * z shorts (sign-extended) stored 7 bytes per slot from LSW base.
 */
static inline void puniswGetindex_inl(s32 base, s32 idx, s32* val, Vec* pos) {
    u8 zHi, b5, b4, b3, b2, b1, b0;
    s32 off;
    s32 x;
    s32 y;
    s32 z;

    off = base + idx * PUNI_LSW_SLOT_SIZE;
    b0 = _swByteGet(off + EVTDAT_LSW_BASE);
    b1 = _swByteGet(off + 150000001);
    b2 = _swByteGet(off + 150000002);
    b3 = _swByteGet(off + 150000003);
    b4 = _swByteGet(off + 150000004);
    b5 = _swByteGet(off + 150000005);
    zHi = _swByteGet(off + 150000006);

    *val = b0;
    x = b1 + (b2 << 8);
    if (x & 0x8000) {
        x |= 0xFFFF0000;
    }
    y = b3 + (b4 << 8);
    if (y & 0x8000) {
        y |= 0xFFFF0000;
    }
    z = b5 + (zHi << 8);
    if (z & 0x8000) {
        z |= 0xFFFF0000;
    }
    pos->x = x;
    pos->y = y;
    pos->z = z;
}

/** Script helper: reads LSW slot args[1] from base args[0]: floor into args[2] and x, y, z into args[3] to args[5]. */
USER_FUNC(evt_puni_get_lsw_index) {
    EvtWord* args = event->args;
    s32 base = args[0];
    s32 idx = evtGetValue(event, args[1]);
    s32 val;
    Vec pos;
    base += idx * PUNI_LSW_SLOT_SIZE;
    puniswGetindex_inl(base, 0, &val, &pos);
    evtSetValue(event, args[2], val);
    evtSetValue(event, args[3], (s32)pos.x);
    evtSetValue(event, args[4], (s32)pos.y);
    evtSetValue(event, args[5], (s32)pos.z);
    return EVT_RETURN_DONE;
}

// Kept: the target needs dead assignments kept in this block; no natural form found (cleanup_notes/mweppc_pilot.md).
#pragma push
#pragma opt_dead_assignments off
static const Vec puni_fall_dir;

/** Script helper: writes LSW slot args[1] from base args[0]: floor args[2] and position args[3] to args[5]. */
USER_FUNC(evt_puni_set_lsw_index) {
    s32 off;
    EvtWord* args = event->args;
    s32 base = args[0];
    s32 idx = evtGetValue(event, args[1]);
    s32 val = evtGetValue(event, args[2]);
    Vec pos;
    pos.x = evtGetValue(event, args[3]);
    pos.y = evtGetValue(event, args[4]);
    pos.z = evtGetValue(event, args[5]);

    // FAKE: slot offset computed in the caller (named local) and passed with idx 0; same slot, numbers off below the inlined extract temps (register colouring)
    off = base + idx * PUNI_LSW_SLOT_SIZE;
    puniswSetindex(off, 0, &val, &pos);
    return EVT_RETURN_DONE;
}
#pragma pop

/** Script helper: reads the trap flag of Puni args[0] (see puniswTrapGetindex) into args[1]. */
USER_FUNC(evt_puni_get_trap_index) {
    EvtWord* args = event->args;
    s32 v;
    puniswTrapGetindex(evtGetValue(event, args[0]), &v);
    evtSetValue(event, args[1], v);
    return EVT_RETURN_DONE;
}

/** Script helper: sets the trap flag of Puni args[0] to args[1]. */
USER_FUNC(evt_puni_set_trap_index) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 val = evtGetValue(event, args[1]);
    puniswTrapSetindex(idx, &val);
    return EVT_RETURN_DONE;
}

/** Script helper: sets the height mode used by puniSetYPosition (args[0]). */
USER_FUNC(evt_puni_set_ymode) {
    Punigp->ymode = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Script helper: checks all Punies for ones that have fallen out of the map and puts them into the fall mode. */
USER_FUNC(evt_puni_all_fall_check) {
    puniModeChangeFallCheck();
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stops (args[0] nonzero) or releases (0) every active Puni from moving,
 * e.g. while a cutscene plays.
 */
USER_FUNC(evt_puni_all_movestop) {
    s32 stop = evtGetValue(event, event->args[0]);
    s32 i;
    for (i = 1; i < PUNI_SOLID_FIRST; i++) {
        PuniData* p = &Punidat[i];
        if (p->flags & PUNI_FLAG_ACTIVE) {
            if (stop == 0) {
                p->flags &= ~PUNI_FLAG_MOVE_STOP;
            } else {
                p->flags |= PUNI_FLAG_MOVE_STOP;
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Counts Punies that are active and not stray: args[0] = 0 counts the ally slots 1 to 102,
 * anything else the Jabbi slots 110 to 209. Result in args[1].
 */
USER_FUNC(evt_puni_count) {
    EvtWord* args = event->args;
    s32 start, end, i;
    s32 count;

    if (evtGetValue(event, args[0]) == 0) {
        start = 1;
        end = 103;
    } else {
        start = 110;
        end = 210;
    }
    count = 0;
    for (i = start; i < end; i++) {
        if ((Punidat[i].flags & PUNI_FLAG_ACTIVE) && !(Punidat[i].flags & PUNI_FLAG_STRAY)) {
            count++;
        }
    }
    evtSetValue(event, args[1], count);
    return EVT_RETURN_DONE;
}

/**
 * Counts the Punies standing near a point: those within a square of half-width args[3] around x,
 * z (args[0], args[2]) and within 10 units of height y (args[1]); lost ones are skipped. Result in args[4].
 */
USER_FUNC(evt_puni_count_area) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    f32 r = evtGetValue(event, args[3]);
    s32 count;
    s32 i;
    for (i = 1, count = 0; i < PUNI_SOLID_FIRST; i++) {
        PuniData* p = &Punidat[i];
        if ((p->flags & PUNI_FLAG_ACTIVE) && p->mode != PUNI_MODE_HAGURE) {
            if (!(10.0 < __fabs(y - p->pos.y)) && !(r < __fabs(x - p->pos.x)) && !(r < __fabs(z - p->pos.z))) {
                count++;
            }
        }
    }
    evtSetValue(event, args[4], count);
    return EVT_RETURN_DONE;
}

/**
 * Checks whether a side is wiped out: args[0] = 0 for the Punies, otherwise the Jabbi.
 * args[1] = 1 if none of that side is still standing, otherwise 0.
 */
USER_FUNC(evt_puni_zenmetsu_check) {
    EvtWord* args = event->args;
    s32 start;
    s32 end;
    s32 i;
    s32 count;

    if (evtGetValue(event, args[0]) == 0) {
        start = 1;
        end = 103;
    } else {
        start = 110;
        end = 210;
    }
    count = 0;
    for (i = start; i < end; i++) {
        if ((Punidat[i].flags & PUNI_FLAG_ACTIVE) && !(Punidat[i].flags & PUNI_FLAG_DEAD)) {
            count++;
        }
    }
    if (count == 0) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Checks whether every Puni of one side is lower than height args[1] (args[0] picks Puni or Jabbi).
 * args[2] = 1 if no one is higher, otherwise 0 (the early demos: 1 only if everyone is exactly at that height).
 * mri_08 and mri_11 use it to wait until the Punies are down at height 0.
 */
USER_FUNC(evt_puni_landon_check) {
    EvtWord* args = event->args;
    s32 kind = evtGetValue(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    s32 start;
    s32 end;
    s32 i;
    s32 count;
    PuniData* p;

    if (kind == 0) {
        start = 1;
        end = 103;
    } else {
        start = 110;
        end = 210;
    }
    count = 0;
    for (i = start; i < end; i++) {
        p = &Punidat[i];
#if VERSION <= VERSION_DEMO_JP
        if ((p->flags & PUNI_FLAG_ACTIVE) && p->pos.y != y) {
#else
        if ((p->flags & PUNI_FLAG_ACTIVE) && p->pos.y > y) {
#endif
            count++;
        }
    }
    if (count == 0) {
        evtSetValue(event, args[2], 1);
    } else {
        evtSetValue(event, args[2], 0);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: turns the stand-in NPC of Puni args[0] on (args[1] = 0) or off. */
USER_FUNC(evt_puni_pera_onoff) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    if (evtGetValue(event, args[1])) {
        p->npc->flags &= ~NPC_FLAG_READY;
    } else {
        p->npc->flags |= NPC_FLAG_READY;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: fades the Puni drawing in or out (args[0]). */
USER_FUNC(evt_puni_fade_onoff) {
    puniFadeOff(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/** Script helper: makes all Punies attack (allMode 3). */
USER_FUNC(evt_puni_attack_on) {
    Punigp->allMode = 3;
    return EVT_RETURN_DONE;
}

/** Script helper: shows or hides the Puni counter on screen (args[0]). */
USER_FUNC(evt_puni_puninum_onoff) {
    Punigp->showPuniCounter = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Script helper: sets the opacity of the Puni counter (args[0]). */
USER_FUNC(evt_puni_puninum_alpha) {
    Punigp->puniCounterAlpha = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Script helper: turns Flurrie's wind effect on the Punies off (args[0] nonzero) or back on. */
USER_FUNC(evt_puni_winddisable_onoff) {
    Punigp->windDisabled = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Script helper: stops the Jabbi march loop (sfx468) and the sound handle sfx458, which nothing in this file sets. */
USER_FUNC(evt_puni_force_se_off) {
    if (Punigp->sfx458) {
        psndSFXOff(Punigp->sfx458);
    }
    if (Punigp->sfx468) {
        psndSFXOff(Punigp->sfx468);
    }
    return EVT_RETURN_DONE;
}

/** The zero vector, used as the default saved position. */
static const Vec puni_save_zero = { 0.0f, 0.0f, 0.0f };
#pragma push
static const Vec puni_fall_dir;

// FAKE: the zero-position branch of evt_puni_all_save inlines its own copy of puni_exit_swset with another local
// declaration order (register colouring only)
static inline void puni_set3(s32 off, s32 val, Vec* pos) {
    u8 yh;
    s32 y;
    s32 z;
    s32 x;
    u8 xh;
    x = pos->x;
    y = pos->y;
    z = pos->z;
    xh = ((x & 0xFF00) >> 8) & 0xFFu;
    yh = (y & 0xFF00) >> 8;
    _swByteSet(off, val);
    _swByteSet(off + 1, x & 0xFF);
    _swByteSet(off + 2, xh);
    _swByteSet(off + 3, y & 0xFF);
    _swByteSet(off + 4, yh);
    _swByteSet(off + 5, z & 0xFF);
    _swByteSet(off + 6, (z & 0xFF00) >> 8);
}

// FAKE: opt_propagation off for evt_puni_all_save (keeps the branch-2 zero argument in its own temp, register colouring)
#pragma opt_propagation off
/**
 * Saves every living Puni when Mario leaves the map:
 * lost Punies get their current floor and position written to their save slot,
 * the others a zero slot (so they come back with Mario).
 */
USER_FUNC(evt_puni_all_save) {
    s32 off;
    s32 i;
    PuniData* p = Punidat + 1;
    Vec pos;
    for (i = 1; i <= PUNI_ALLY_LAST; i++, p++) {
        if ((p->flags & PUNI_FLAG_ACTIVE) && !(p->flags & PUNI_FLAG_NOT_IN_PARTY)) {
            if (p->flags & PUNI_FLAG_STRAY) {
                puniHagurePos(p);
                pos = p->pos;
                off = (p->index- 1) * 7;
                puni_exit_swset(off, (u8)Punigp->floor, &pos);
            } else {
                pos = puni_save_zero;
                off = (p->index- 1) * 7;
                puni_set3(off, 0, &pos);
            }
        }
    }
    return EVT_RETURN_DONE;
}
#pragma pop

/**
 * Script helper: sets (args[1] = 0) or clears the flags in mask args[2] of Puni target number args[0] (the
 * points that attract or repel the crowd).
 */
USER_FUNC(evt_target_flag_onoff) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    s32 mask = evtGetValue(event, args[2]);
    PuniTarget* t = &Punigp->targets[idx];
    if (on) {
        t->flags &= ~mask;
    } else {
        t->flags |= mask;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: moves Puni target args[0] to x, y, z (args[1] to args[3]). */
USER_FUNC(evt_target_set_position) {
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);
    PuniTarget* t = &Punigp->targets[i];
    t->pos.x = evtGetValue(event, args[1]);
    t->pos.y = evtGetValue(event, args[2]);
    t->pos.z = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

/** Script helper: sets the floor collision (hit name args[1]) that Puni target args[0] sits on. */
USER_FUNC(evt_target_set_soko) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    PuniTarget* t = &Punigp->targets[idx];
    t->hit = hitNameToPtr((const char*)evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Returns the first string of the name table passed in; used to read an NPC tribe's Japanese name. */
static const char* npcGetTribeName(const char** name) {
    return *name;
}

static const Vec puni_fall_dir;

/**
 * Per-frame callback of the spider check: turns each live spider-tribe NPC of the map (Piders)
 * into a Puni target (slots 2 to 9) at its position,
 * snapped to the nearest floor height within 50 units, flagged to repel the crowd.
 * Targets of NPCs that are not near any floor are left off.
 */
static s32 puni_piders_check_func(EventEntry *event, BOOL isFirstCall) {
    s32 k;
    s32 count;
    NpcEntry *npc;
    s32 i;
    NpcWork *work;
    work = npcGetWorkPtr();
    count = work->total;
    npc = work->entries;
    for (i = 2; i < 10; i++) {
        Punigp->targets[i].flags &= ~(PUNI_TARGET_FLAG_ON | PUNI_TARGET_FLAG_AWAY);
    }
    k = 2;
    for (i = 0; i < count; i++, npc++) {
        if ((npc->flags & NPC_FLAG_IN_USE) && (npc->flags & NPC_FLAG_READY) && npc->tribe != NULL && strcmp(npc->tribe->nameJp, "\203p\203C\203_\201[\203X") == 0) {
            Vec v;
            BOOL found = 0;
            v = npc->position;
            if (50.0 >= __fabs(v.y - Punigp->posY[0])) {
                found = 1;
                v.y = Punigp->posY[0];
            }
            if (50.0 >= __fabs(v.y - Punigp->posY[1])) {
                found = 1;
                v.y = Punigp->posY[1];
            }
            if (50.0 >= __fabs(v.y - Punigp->posY[2])) {
                found = 1;
                v.y = Punigp->posY[2];
            }
            if (50.0 >= __fabs(v.y - Punigp->posY[3])) {
                found = 1;
                v.y = Punigp->posY[3];
            }
            if (found) {
                PuniTarget *t = &Punigp->targets[k];
                t->pos = v;
                t->flags |= PUNI_TARGET_FLAG_ON | PUNI_TARGET_FLAG_AWAY;
            }
            k++;
        }
    }
    return 2;
}

/** Script helper: args[0] = TRUE on the two battle floors (9 and 12), where Punies fight Jabbi, otherwise FALSE. */
static USER_FUNC(puni_disp_info_kassencheck) {
    EvtWord* args = event->args;
    BOOL ret = FALSE;
    if (Punigp->floor == 9 || Punigp->floor == 12) {
        ret = TRUE;
    }
    evtSetValue(event, args[0], ret);
    return EVT_RETURN_DONE;
}

/**
 * Draw callback for the Puni counter at the lower left of the screen: skipped while a shop menu is open;
 * draws the pair of numbers (the count with Mario, blinking while some are missing, and the story total,
 * packed in the parameter) with the icon for the counter background, faded by the counter opacity.
 */
#if VERSION <= VERSION_DEMO_JP
static void puniCountDisp(CameraId unused, void* param) {
#else
void puniCountDisp(CameraId unused, void* param) {
#endif
    EvtWord packed = (EvtWord)param;
    Mtx m;
    GXColor c1 = {0xFF, 0xFF, 0xFF, 0x99};
    GXColor c2 = {0, 0, 0, 0xFF};
    GXColor c3 = {0xFF, 0xFF, 0xFF, 0xFF};
    s32 num2 = packed >> 16;
    s32 num1 = packed & 0xFFFF;
    if (evtShopIsActive()) {
        return;
    }
    c1.a = c1.a * Punigp->puniCounterAlpha / 255;
    c2.a = c2.a * Punigp->puniCounterAlpha / 255;
    c3.a = c3.a * Punigp->puniCounterAlpha / 255;
    PSMTXTrans(m, -160.0f, -180.0f, 0.0f);
    iconDispGxCol(m, ICON_FLAG_2D, ICON_PUNI_BAR_WHITE, c1);
    PSMTXTrans(m, -232.0f, -178.0f, 0.0f);
    iconDispGxCol(m, ICON_FLAG_2D, ICON_PUNI_WHITE, c2);
    PSMTXTrans(m, -160.0f, -178.0f, 0.0f);
    if (Punigp->showRemaining) {
        if (gp->retraceCount & 0x10) {
            iconNumberDispGx(m, num2, FALSE, c3);
        }
    } else {
        iconNumberDispGx(m, num2, FALSE, c3);
    }
    PSMTXTrans(m, -140.0f, -178.0f, 0.0f);
    iconDispGxCol(m, ICON_FLAG_2D, ICON_SLASH, c3);
    PSMTXTrans(m, -90.0f, -178.0f, 0.0f);
    iconNumberDispGx(m, num1, TRUE, c3);
}

/**
 * Per-frame counter update: counts the living Punies that are with Mario (not defeated, not saved aside,
 * not the little Jabbi, not lost: modes 15 and 16) and the total the story has gathered (Punio, his sister,
 * the rival, plus the recruited followers). On the Jabbi fight floors, until the fight has been won, the
 * missing number (args[1]) is 0 and the count does not blink.
 * Keeps the totals in the save so the counter shows the right values while the map loads,
 * and draws the counter when there are at least 10 Punies.
 */
static USER_FUNC(puni_disp_info_sub) {
    EvtWord* args = event->args;
    s32 count = 0;
    s32 total;
    BOOL done;
    s32 i;
    EvtWord packed;
    for (i = 1; i < PUNI_SOLID_FIRST; i++) {
        if ((Punidat[i].flags & PUNI_FLAG_ACTIVE) && !(Punidat[i].flags & (PUNI_FLAG_NOT_IN_PARTY | PUNI_FLAG_DEAD)) &&
            !(Punidat[i].flags & PUNI_FLAG_NPC_CHIBI_TOGE)) {
            if (Punidat[i].mode != 16 && Punidat[i].mode != 15) {
                count++;
            }
        }
    }
    total = swGet(GSWF_ID(GSWF_MRI_PUNIO_JOINED));
    total += swGet(GSWF_ID(GSWF_MRI_PETUNI_JOINED));
    total += swGet(GSWF_ID(GSWF_MRI_PUNIPER_JOINED));
    total += swByteGet(GSW_ID(GSW_MRI_PUNI_FOLLOWER_COUNT));
    done = FALSE;
    if (Punigp->floor == 9 && !swGet(GSWF_ID(GSWF_MRI_08_JABBI_SQUAD_DEFEATED))) {
        done = TRUE;
    }
    if (Punigp->floor == 12 && !swGet(GSWF_ID(GSWF_MRI_11_JABBI_SQUAD_DEFEATED))) {
        done = TRUE;
    }
    if (done) {
        evtSetValue(event, args[0], count);
        evtSetValue(event, args[1], 0);
        Punigp->showRemaining = 0;
    } else {
        evtSetValue(event, args[0], count);
        evtSetValue(event, args[1], total - count);
        if (count == total) {
            Punigp->showRemaining = 0;
        } else {
            Punigp->showRemaining = 1;
        }
    }
    if (!(Punigp->systemMode & 2)) {
        count = swByteGet(GSW_MRI_PUNI_COUNTER_HERE);
        total = swByteGet(GSW_MRI_PUNI_COUNTER_TOTAL);
        Punigp->showRemaining = 0;
    } else {
        swByteSet(GSW_MRI_PUNI_COUNTER_HERE, (u8)count);
        swByteSet(GSW_MRI_PUNI_COUNTER_TOTAL, (u8)total);
    }
    packed = (count << 16) | total;
    if (Punigp->showPuniCounter && total >= 10) {
        dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, puniCountDisp, (void*)packed);
    }
    return EVT_RETURN_DONE;
}

/**
 * Event started by maps with spiders: waits for the game sequence (SEQ_GAME) and
 * then keeps the spider NPCs registered as Puni targets, every frame.
 */
EVT_BEGIN(puni_piders_check)
    CALL(evt_seq_wait, SEQ_GAME)
    LOOP(0)
        CALL(puni_piders_check_func)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Event that keeps the Puni counter updated every frame.
 * Does not run at all once the Punies have been praised (STG2_26_PRAISE_FROM_THE_PUNI)
 * and the counter is no longer needed.
 */
EVT_BEGIN(puni_disp_info)
    IF_INT_GREATER_OR_EQUAL(4124967296, 118)
        RETURN
    END_IF
    LOOP(0)
        CALL(puni_disp_info_sub, LW(0), LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Fades the Puni counter out after waiting 1.2 seconds (opacity 255 to 0 over 0.5 seconds);
 * started when Mario leaves through a loading zone.
 */
static EVT_BEGIN(mri_puninum_fadeout)
    WAIT_MS(1200)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_puni_puninum_alpha, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/** Fades the Puni counter in (opacity 0 to 255 over 0.5 seconds) when Mario arrives through a loading zone. */
static EVT_BEGIN(mri_puninum_fadein)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_puni_puninum_alpha, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Drives the Puni system through a map visit with the loading-zone (bero) steps.
 * Shows the counter on entry at once unless coming in through a zone from another area (then it fades in),
 * sets the system mode 1 (position setup), waits for the entry step, then sets up the Punies
 * and Jabbi (evt_puni_setup, evt_toge_setup) in mode 2 (running).
 * When the map ends it goes to mode 3, notes the previous map, waits for doors, fades the counter,
 * silences the crowd's looping sounds and sends the Punies out (evt_puni_exit). Loops for every map change.
 */
static EVT_BEGIN(mri_puni_position_init)
    CALL(evt_puni_set_systemmode, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_puninum_alpha, 0)
        WAIT_FRAMES(2)
        CALL(evt_bero_get_now_number, LW(10))
        CALL(evt_bero_get_info_kinddir, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_bero_get_info_nextarea, LW(10), LW(3), LW(4))
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(LW(0), 2)
            IF_INT_NOT_EQUAL(LW(3), 0)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            CALL(evt_puni_puninum_alpha, 255)
        END_IF
    END_INLINE_EVENT
    DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\217\211\212\372\202\314\216\300\215s\222\206\221\322\202\277"))
    CALL(evt_bero_exec_wait, BERO_EXEC_BUSY)
    LOOP(0)
        CALL(evt_puni_set_systemmode, 1)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\215\300\225W\203Z\203b\203g\221\322\202\277"))
        CALL(evt_bero_exec_wait, BERO_EXEC_MARIO_MOVING)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\217\254\223\256\225\250\212\326\214W\217\211\212\372\211\273"))
        DEBUG_PUT_REG(GSW_MRI_PUNI_ROOM)
        DEBUG_PUT_REG(GSW_MRI_PUNI_PREV_ROOM)
        CALL(evt_bero_get_now_number, LW(10))
        CALL(evt_bero_get_info_kinddir, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_bero_get_info_nextarea, LW(10), LW(3), LW(4))
        IF_INT_EQUAL(LW(0), 2)
            IF_INT_NOT_EQUAL(LW(3), 0)
                SPAWN_EVENT(mri_puninum_fadein)
            END_IF
        END_IF
        CALL(evt_puni_setup, LW(0), LW(1), LW(2))
        CALL(evt_toge_setup, LW(6), LW(7), LW(8), LW(9))
        CALL(evt_puni_set_systemmode, 2)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\217I\227\271\221\322\202\277"))
        CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
        CALL(evt_puni_set_systemmode, 3)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\216\237\202\314\216\300\215s\222\206\221\322\202\277"))
        CALL(evt_bero_exec_wait, BERO_EXEC_BUSY)
        SET_VALUE(GSW_MRI_PUNI_PREV_ROOM, GSW_MRI_PUNI_ROOM)
        CALL(evt_bero_get_now_number, LW(10))
        CALL(evt_bero_get_info_kinddir, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_bero_get_info_nextarea, LW(10), LW(3), LW(4))
        IF_INT_EQUAL(LW(0), 1)
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\224\340\221\322\202\277"))
            CALL(evt_bero_exec_wait, BERO_EXEC_MRI_DOOR_GO)
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\224\340\221\322\202\277\217I\202\355\202\350"))
            CALL(evt_bero_exec_onoff, 1, BERO_EXEC_MRI_DOOR_GO)
        END_IF
        IF_INT_EQUAL(LW(0), 2)
            IF_INT_NOT_EQUAL(LW(3), 0)
                SPAWN_EVENT(mri_puninum_fadeout)
            END_IF
        END_IF
        BEGIN_INLINE_EVENT
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\203G\203\212\203A\203`\203F\203\223\203W\222\274\221O\221\322\202\277"))
            CALL(evt_bero_exec_wait, BERO_EXEC_EXIT_DONE)
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\203v\203j\221\260\203g\203Q\221\260SE\213\255\220\247\203I\203t"))
            CALL(evt_puni_force_se_off)
        END_INLINE_EVENT
        CALL(evt_puni_exit, LW(0), LW(1), LW(2), LW(3))
    END_LOOP
    RETURN
EVT_END()

/** Event that runs the Puni system's per-frame update for the map. */
static EVT_BEGIN(mri_puni_main)
    SET_TYPE(0)
    CALL(evt_puni_main)
    RETURN
EVT_END()

/**
 * Registers the six stand-in NPCs for the Puni system (a Puni, a Jabbi, the guide, his sister,
 * the little Jabbi and the rival), parks them out of sight at y -1000 and makes them untouchable,
 * without wall collision and with shadows not projected to the floor.
 */
EVT_BEGIN(mri_puni_init_dvd)
    CALL(evt_npc_entry, STRING("\203v\203jsp"), STRING("c_puni")) // hidden Puni model stand-in for the Puni system
    CALL(evt_npc_set_tribe, STRING("\203v\203jsp"), STRING("\203v\203j\221\260"))
    CALL(evt_npc_entry, STRING("\203g\203Qsp"), STRING("c_toge")) // hidden Jabbi model stand-in for the Puni system
    CALL(evt_npc_set_tribe, STRING("\203g\203Qsp"), STRING("\203g\203Q\221\260"))
    CALL(evt_npc_entry, STRING("\203K\203C\203hsp"), STRING("c_pguide")) // hidden Punio stand-in for the Puni system
    CALL(evt_npc_set_tribe, STRING("\203K\203C\203hsp"), STRING("\203K\203C\203h"))
    CALL(evt_npc_entry, STRING("\203K\203C\203h\226\205sp"), STRING("c_psister")) // hidden Petuni stand-in for the Puni system
    CALL(evt_npc_set_tribe, STRING("\203K\203C\203h\226\205sp"), STRING("\203K\203C\203h\226\205"))
    CALL(evt_npc_entry, STRING("\202\277\202\321\203g\203Qsp"), STRING("c_tchibi")) // hidden Jabble stand-in for the Puni system
    CALL(evt_npc_set_tribe, STRING("\202\277\202\321\203g\203Qsp"), STRING("\202\277\202\321\203g\203Q"))
    CALL(evt_npc_entry, STRING("\203\211\203C\203o\203\213sp"), STRING("c_prival")) // hidden Puniper stand-in for the Puni system
    CALL(evt_npc_set_tribe, STRING("\203\211\203C\203o\203\213sp"), STRING("\203\211\203C\203o\203\213"))
    CALL(evt_npc_set_position, STRING("\203v\203jsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203g\203Qsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203K\203C\203hsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203K\203C\203h\226\205sp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\202\277\202\321\203g\203Qsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203\211\203C\203o\203\213sp"), 0, 4294966296, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("\203v\203jsp"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("\203g\203Qsp"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("\203K\203C\203hsp"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("\203K\203C\203h\226\205sp"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("\202\277\202\321\203g\203Qsp"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("\203\211\203C\203o\203\213sp"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Map init call that starts the Puni system if it is needed at the current story point.
 * The stand-ins are always registered; before STG2_10_SECRET_ENTRANCE_REVEALED and
 * from the epilogue on there are no Punies.
 * At STG8_15_GRODUS they exist only in mri_00
 * when entered at the ending scenes (sekai_yami1 or minnnanokoe);
 * between the praise and the queen's chamber they exist only in mri_00 and only
 * while the player has not left the tree.
 * Otherwise it allocates the system, loads the maze and starts the position driver and the update loop.
 */
EVT_BEGIN(mri_puni_init)
    CALL_CHILD(mri_puni_init_dvd)
    SWITCH(4124967296)
        CASE_LESS(STG2_10_SECRET_ENTRANCE_REVEALED)
            RETURN
        CASE_GREATER_OR_EQUAL(STG9_00_EPILOGUE)
            RETURN
        CASE_EQUAL(STG8_15_GRODUS)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_NOT_EQUAL(LW(0), STRING("mri_00"))
                RETURN
            END_IF
            SET_VALUE(LF(0), 0)
            CALL(evt_bero_get_entername, LW(0))
            IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                RETURN
            END_IF
        CASE_BETWEEN(STG2_26_PRAISE_FROM_THE_PUNI, STG8_14_01_QUEENS_CHAMBER)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_NOT_EQUAL(LW(0), STRING("mri_00"))
                RETURN
            END_IF
            IF_INT_EQUAL(GSWF_MRI_LEFT_AFTER_BOSS_BATTLE, 1)
                RETURN
            END_IF
        CASE_DEFAULT
    END_SWITCH
    CALL(evt_make_texbuf)
    CALL(evt_puni_init, 210, PTR(puni_puni_init), PTR(puni_puni_main), PTR(puni_puni_dispent))
    CALL(evt_puni_set_maze)
    SPAWN_EVENT(mri_puni_position_init)
    SPAWN_EVENT(mri_puni_main)
    RETURN
EVT_END()

/**
 * Version of the map init call for rooms that only need the Puni system loaded (not driven):
 * the stand-in NPCs, the allocation and the maze.
 */
EVT_BEGIN(mri_puni_init_none)
    CALL_CHILD(mri_puni_init_dvd)
    CALL(evt_make_texbuf)
    CALL(evt_puni_init, 210, PTR(puni_puni_init), PTR(puni_puni_main), PTR(puni_puni_dispent))
    CALL(evt_puni_set_maze)
    RETURN
EVT_END()
