#include "eff/eff_fukidashi.h"
#include "eff/eff_puniballoon.h"
#include "version.h"
#include "mri_puni.h"
#include "party.h"
#include "mario/mario_party.h"
#include "driver/camdrv.h"
#include "memory.h"
#include <string.h>
#include "mario/mariost.h"
#include "mario/mario.h"
#include "driver/icondrv.h"
#include "evt/evt_sub.h"

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
void puniHagurePos(PuniData* p);
void puniMapdata_MRI_06_getidx(f32 x, f32 z, s32* bx, s32* bz);
void puniMapdata_MRI_09_getidx(f32 x, f32 z, s32* bx, s32* bz);
void puniMapdata_MRI_16_getidx(f32 x, f32 z, s32* bx, s32* bz);
void puniMapdata_MRI_09_getpos(s32 bx, s32 bz, f32* x, f32* z);
extern int abs(int);
void envSetYami(MapTevCallbackInfo* info);

u16 puni_nom_patern_data[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,    21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0,  1,  2,  3,  4,  5,     6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,    31, 32, 33, 34,
    35, 36, 37, 38, 39, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,    16, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 51, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 65535,
};
u16 puni_wlk_patern_data[] = {
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 65535,
};
u16 puni_run_patern_data[] = { 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 65535 };
u16 puni_dmg_patern_data[] = { 72, 65535 };
u16 puni_dwn_patern_data[] = { 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 65535 };
u16 puni_rol_patern_data[] = { 83, 65535 };
u16 puni_hag_patern_data[] = { 0, 65535 };
u16 puni_jpu_patern_data[] = { 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 65534 };
u16 puni_jpd_patern_data[] = { 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 65534 };
u16 puni_wan_patern_data[] = { 104, 105, 106, 107, 108, 109, 110, 111, 65534 };
u16* puni_patern_table[] = { puni_nom_patern_data, puni_wlk_patern_data, puni_run_patern_data, puni_dmg_patern_data,
                             puni_dwn_patern_data, puni_rol_patern_data, puni_hag_patern_data, puni_jpu_patern_data,
                             puni_jpd_patern_data, puni_wan_patern_data };

u16 toge_nom_patern_data[25] = {
    84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 65535,
};
u16 toge_wlk_patern_data[] = { 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 65535 };
u16 toge_run_patern_data[] = { 108, 110, 112, 114, 116, 118, 65535 };
u16 toge_dmg_patern_data[] = { 120, 65535 };
u16 toge_dwn_patern_data[] = { 121, 122, 123, 124, 125, 126, 127, 128, 65535 };
u16 toge_rol_patern_data[] = { 129, 65535 };
u16 toge_hag_patern_data[] = { 0, 65535 };
u16* toge_patern_table[] = { toge_nom_patern_data, toge_wlk_patern_data, toge_run_patern_data, toge_dmg_patern_data,
                             toge_dwn_patern_data, toge_rol_patern_data, toge_hag_patern_data, toge_hag_patern_data,
                             toge_hag_patern_data, toge_hag_patern_data };

//.bss
PuniData* Punidat;
PuniWork* Punigp;
PuniTexInfo* Punitex;
PuniMap* Punimap;


static inline s32 puni_bero_id_filter(s32 id) {
    return bero_id_filter(id);
}
PuniData* puniGetPunidat(void) {
    return Punidat;
}

PuniWork* puniGetPunigp(void) {
    return Punigp;
}

PuniTexInfo* puniGetTex(void) {
    return Punitex;
}

void evt_icon_disp_func(CameraId camId, void* param) {
    PuniData* p;
    s32 i;
    if (Punigp->floor == 9 || Punigp->floor == 12) {
        for (i = 1; i < 211; i++) {
            p = &Punidat[i];
            if ((p->flags & 0x400) && !(p->flags & 0x804000)) {
                Vec a;
                Vec b;
                a = p->pos;
                if (p->flags2 & 1) {
                    p->flag1Timer++;
                    if ((s8)p->flag1Timer > 30) {
                        p->flag1Timer = 0;
                        p->flags2 &= ~1;
                    }
                }
                if (p->flags2 & 2) {
                    b = a;
                    puniSinCosRet(p->index + gp->retraceCount * 10, &a.z, &a.x, 9.0f, 9.0f);
                    a.y += 13.0f;
                    iconDispGx(&a, 0, VERSION_PICK(0x14d, 0x155), 0.6f);
                    puniSinCosRet(p->index + gp->retraceCount * 10 + 180, &b.z, &b.x, 9.0f, 9.0f);
                    b.y += 15.0f;
                    iconDispGx(&b, 0, VERSION_PICK(0x14d, 0x155), 0.5f);
                }
            }
        }
    }
}

PuniTexInfo* _make_texbuf(PuniTexInfo* texbuf, s32 count, const char* group, const char* anim, f32 frame) {
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

USER_FUNC(evt_make_texbuf) {
    PuniTexInfo* texbuf = Punitex;

    texbuf = Punitex;
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

void puniAnimCount(s16* pattern, s16* count, s16* end) {
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

void puniAnimationInit(s16** a, s16** b) {
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
        k = 0; j = 0;
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

void puniAnimationInit2(void) {
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



void puniAnimation(PuniData* p, PunidrvPose* t) {
    NpcEntry* npc;
    char* names;
    PuniAnimSet* set;
    s32 cnt;
    s16* tbl;
    s32 end;
    s32 show;
    s32 f;
    s32 ofs;

    if (!(p->flags & 0x40000)) {
    if (p->flags & 0x1E0) {
        if (p->flags & 0x20) {
            npc = npcNameToPtr("\203K\203C\203hsp");
            names = (char*)( &Punigp->animNames[0]) ;
        } else if (p->flags & 0x40) {
            npc = npcNameToPtr("\203K\203C\203h\226\205sp");
            names = (char*)( &Punigp->animNames[160]) ;
        } else if (p->flags & 0x80) {
            npc = npcNameToPtr("\202\277\202\321\203g\203Qsp");
            names = (char*)( &Punigp->animNames[320]) ;
        } else if (p->flags & 0x100) {
            npc = npcNameToPtr("\203\211\203C\203o\203\213sp");
            names = (char*)( &Punigp->animNames[480]) ;
        } else {
            npc = npcNameToPtr("\203\211\203C\203o\203\213sp");
            names = (char*)( &Punigp->animNames[480]) ;
        }
        ofs = p->marioDist;
        if (p->flags & 0x1000000) {
            p->animTime++;
        } else {
            if (p->mode != 2) {
                ofs = p->index ;
            } else {
                ofs /= 8;
                if (p->anim == 0) {
                    ofs += p->index * 40;
                }
            }
            p->animTime = gp->retraceCount + ofs;
        }
        if (!(p->flags & 0x20000)) {
            animPoseSetAnim(npc->poseId, names + ( p->anim * 16) , 1);
            animPoseSetLocalTime(npc->poseId, p->animTime);
        }
        show = 0;
        if (p->flags & 0x400) { show = 1;
        }
        if (p->flags & 0x800) { show = 0;
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
        if (p->flags & 0x8) { set = &Punigp->anim[0];
        }
        if (p->flags & 0x10) { set = &Punigp->anim[1];
        }
        tbl = set->table[p->anim];
        cnt = set->count[p->anim];
        end = set->end[p->anim];
        ofs = p->marioDist;
        if (p->flags & 0x1000000) {
            f = p->animTime % cnt;
            if (( end == ( -2 )) && ( cnt <= p->animTime) ) { f = cnt - 1;
            }
            t->texId = tbl[f];
            p->animTime++;
        } else {
            if (p->mode != 2) {
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
        if (p->flags & 0x400) { show = 1;
        }
        if (p->flags & 0x800) { show = 0;
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

f32 _chukanhosei(f32 a, f32 b, f32 c) {
    f32 t = (20.0f + b - a) / 20.0f;
    if (t > 0.5f) {
        t = (t - 0.5f) / 0.5f;
    } else {
        t = 0.0f;
    }
    return 10.0f * t + c;
}

BOOL _areacheck(f32 x, f32 z, f32 r, f32 px, f32 pz) {
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

#define YSTEP(thr, base)                          \
    if (p->pos.z <= (thr)) {                      \
        f32 t = ((thr) - p->pos.z) / 20.0f;       \
        f32 u;                                    \
        if (t > 0.5f) {                           \
            u = (t - 0.5f) / 0.5f;                \
        } else {                                  \
            u = floorY;                             \
        }                                         \
        p->pos.y = 10.0f * u + (base);            \
    }

void _ymode_mri_09(PuniData* p) {
    float floorY;
    floorY = 0.0f;
    if (p->pos.x <= 400.0f && p->pos.x > 200.0f && p->pos.y <= 100.0f && p->pos.y >= floorY) {
        if (p->pos.z <= -55.0f) {
            p->pos.y = 100.0f;
        } else YSTEP(-35.0f, 90.0f)
        else YSTEP(-15.0f, 80.0f)
        else YSTEP(5.0f, 70.0f)
        else YSTEP(25.0f, 60.0f)
        else YSTEP(45.0f, 50.0f)
        else YSTEP(65.0f, 40.0f)
        else YSTEP(85.0f, 30.0f)
        else YSTEP(105.0f, 20.0f)
        else YSTEP(125.0f, 10.0f)
        else YSTEP(145.0f, floorY)
        else {
            p->pos.y = floorY;
        }
    }
    if (p->pos.x >= -170.0f && p->pos.x <= 170.0f && p->pos.z >= -80.0f && p->pos.z <= 140.0f) {
        p->pos.y = 0.0f;
    }
}

#define YSTEPX(thr, base)                   \
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
#define YSTEPZ(thr, base)                               \
    if (p->pos.x <= (thr)) {                            \
        f32 t = ((thr) - p->pos.x) / 20.0f;             \
        f32 u;                                          \
        if (t > 0.5f) {                                 \
            u = (t - 0.5f) / 0.5f;                      \
        } else {                                        \
            u = 0.0f;                                   \
        }                                               \
        { f32 bz = (base); p->pos.y = 10.0f * u + bz; } \
    }

void _ymode_mri_16_00(PuniData* p) {
    u32 old = p->flags2;
    p->flags2 &= ~8;
    if (p->pos.y <= -200.0f && p->pos.y >= -400.0f) {
        if (p->pos.x <= 40.0f && p->pos.x >= -210.0f && p->pos.z <= -76.46314f && p->pos.z >= -200.0f) {
            if (p->pos.x <= -190.0f) {
                p->pos.y = -320.0f;
            } else YSTEPX(-170.0f, -330.0f)
            else YSTEPX(-150.0f, -340.0f)
            else YSTEPX(-130.0f, -350.0f)
            else YSTEPX(-110.0f, -360.0f)
            else YSTEPX(-90.0f, -370.0f)
            else YSTEPX(-70.0f, -380.0f)
            else YSTEPX(-50.0f, -390.0f)
            else YSTEPX(-30.0f, -400.0f)
            else {
                p->pos.y = -400.0f;
            }
            if (p->pos.y < -320.0f && p->pos.y > -400.0f) {
                p->flags2 |= 8;
            }
        }
        if (p->pos.x <= 40.0f && p->pos.x >= -200.0f && p->pos.z > -76.46314f && p->pos.z < 200.0f) {
            p->pos.y = -400.0f;
        }
    }
    if (!(p->flags2 & 4) && !(p->flags2 & 8) && (old & 8)) {
        if (p->pos.z > -76.46314f && p->prevPos.z <= -76.46314f) {
            p->pos = p->prevPos;
            p->pos.z -= 0.0002f;
        }
    }
}

void _ymode_mri_16_01(PuniData* p) {
    u32 old = p->flags2;
    p->flags2 &= ~8;
    if (p->pos.y <= 100.0f && p->pos.y >= 0.0f) {
        if (p->pos.x <= 40.0f && p->pos.x >= -200.0f && p->pos.z > -200.0f && p->pos.z < 200.0f) {
            p->pos.y = 0.0f;
        }
        if (p->pos.x <= 40.0f && p->pos.x >= -210.0f && p->pos.z <= 60.00006f && p->pos.z >= -10.00012f) {
            if (p->pos.x <= -190.0f) {
                p->pos.y = 80.0f;
            } else YSTEPX(-170.0f, 70.0f)
            else YSTEPX(-150.0f, 60.0f)
            else YSTEPX(-130.0f, 50.0f)
            else YSTEPX(-110.0f, 40.0f)
            else YSTEPX(-90.0f, 30.0f)
            else YSTEPX(-70.0f, 20.0f)
            else YSTEPX(-50.0f, 10.0f)
            else YSTEPZ(-30.0f, 0.0f)
            else {
                p->pos.y = 0.0f;
            }
            if (p->pos.y < 80.0f && p->pos.y > 0.0f) {
                p->flags2 |= 8;
            }
        }
    }
    if (!(p->flags2 & 4) && !(p->flags2 & 8) && (old & 8)) {
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

#define AC(p, x0, x1, z0, z1) (_aok = 1, _ax = (p)->pos.x, _az = (p)->pos.z, ((x0) > _ax ? _aok = 0 : 0), ((x1) < _ax ? _aok = 0 : 0), ((z0) > _az ? _aok = 0 : 0), ((z1) < _az ? _aok = 0 : 0), _aok)
//(x0,x1,z0,z1) (ok = 1, (p->pos.x < (x0) ? ok = 0 : 0), (p->pos.x > (x1) ? ok = 0 : 0), (p->pos.z < (z0) ? ok = 0 : 0), (p->pos.z > (z1) ? ok = 0 : 0), ok)
void puniSetYPosition(PuniData* p) {
    if (p->flags & 1) {
        BOOL _aok;
        f32 _ax, _az;
        switch (Punigp->ymode) {
            case 0:
                break;
            case 1:
                p->pos.y = 0.0f;
                if (AC(p, -49.0f, 49.0f, -69.0f, 29.0f)) {
                    p->pos.y = getMri02Yuka();
                }
                break;
            case 2:
                _ymode_mri_09(p);
                if (!swGet(0xB23)) {
                    if (AC(p, -35.0f, 35.0f, 40.0f, 110.0f)) {
                        p->pos.y = 6.0f;
                    }
                }
                break;
            case 3:
                p->pos.y = 0.0f;
                if (!swGet(0xB2A)) {
                    if (AC(p, -80.0f, -10.0f, -99.0f, -29.0f)) {
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
                if (!swGet(0xB37)) {
                    if (AC(p, -35.0f, 35.0f, -70.0f, 0.0f)) {
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
u8 puniMapdata_MRI_06[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
};
u8 puniMapdata_MRI_09[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
u8 puniMapdata_MRI_09_01[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
u8 puniMapdata_MRI_16[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
};


void puniMapdata_MRI_06_getidx(f32 x, f32 z, s32* bx, s32* bz) {
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

void puniMapdata_MRI_06_getpos(s32 bx, s32 bz, f32* x, f32* z) {
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

void puniMapdata_MRI_09_getidx(f32 x, f32 z, s32* bx, s32* bz) {
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

void puniMapdata_MRI_09_getpos(s32 bx, s32 bz, f32* x, f32* z) {
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

void puniMapdata_MRI_16_getidx(f32 x, f32 z, s32* bx, s32* bz) {
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

void puniMapdata_MRI_16_getpos(s32 bx, s32 bz, f32* x, f32* z) {
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

void puniMapPos2Blk(Vec* pos, s32* blk) {
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

void puniMapBlk2Pos(s32 bx, s32 bz, f32* x, f32* z) {
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

u16 puniMapGetInfo(s32 x, s32 y) {
    u16 v = Punimap->data[x + y * Punimap->width];
    if (!(v & 0xFF)) {
        v |= 0xFFFF;
    }
    return v;
}

void puniMapInit(void) {
    Punimap = _mapAlloc(sizeof(PuniMap));
    memset(Punimap, 0, sizeof(PuniMap));
}

void puniMapDataSet(s32 w, s32 h, void* src, f32 x, f32 z) {
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

void puniMapFindbuff(Vec pos) {
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
#define CHK(k) if ((*k & 0xFF) == 0 && (*k & 0xFF) < l) { if (__abs((*k & 0xF000) - h) <= 0x1000) { *k |= (u16)(l + 1); changed = 1; } }
                            CHK(k0)
                            CHK(k1)
                            CHK(k2)
                            CHK(k3)
#undef CHK
                        }
                    }
                    if (!changed) break;
                    lvl++;
                }
            }
        }
    }
}

void puniTargetSearch(PuniData* p) {
    s32 i;
    PuniTarget* t = Punigp->targets;
    f32 d;

    p->massTarget = -1;
    p->massTargetDistSq = 1000000.0f;
    p->awayTarget = -1;
    p->awayTargetDistSq = 1000000.0f;
    for (i = 0; i < 10; i++, t++) {
        if (!(t->flags & 1)) {
            continue;
        }
        if (32.0 < __fabs(p->pos.y - t->pos.y)) {
            continue;
        }
        if (t->flags & 4) {
            d = PSVECSquareDistance(&p->pos, &t->pos);
            if (p->awayTargetDistSq > d) {
                p->awayTarget = i;
                p->awayTargetDistSq = d;
            }
        }
        if (t->flags & 2) {
            d = PSVECSquareDistance(&p->pos, &t->pos);
            if (p->massTargetDistSq > d) {
                p->massTarget = i;
                p->massTargetDistSq = d;
            }
        }
    }
}

void puniswGetindex(s32 base, s32 idx, s32* val, Vec* pos) {
    u8 b0, b1, b2, b3, b4, b5, b6;
    s32 off;
    s32 x;
    s32 y;
    s32 z;

    off = base + idx * 7;
    b0 = _swByteGet(off + EVTDAT_LSW_BASE);
    b1 = _swByteGet(off + 150000001);
    b2 = _swByteGet(off + 150000002);
    b3 = _swByteGet(off + 150000003);
    b4 = _swByteGet(off + 150000004);
    b5 = _swByteGet(off + 150000005);
    b6 = _swByteGet(off + 150000006);

    *val = b0;
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

#pragma push
#pragma opt_dead_assignments off
void puniswSetindex(s32 base, s32 idx, s32* val, Vec* pos) {
    u8 yh, xh;
    s32 off;
    s32 x;
    s32 y;
    s32 z;
    off = base + idx * 7;
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

void puniswTrapGetindex(s32 idx, s32* out) {
    if (idx >= 103) {
        *out = 0;
    } else {
        *out = (swByteGet(idx / 8 + 0x28C) >> (idx % 8)) & 1;
    }
}

void puniswTrapSetindex(s32 idx, s32* val) {
    if (idx < 103) {
        s32 byte = idx / 8;
        s32 bit = idx % 8;
        s32 v = swByteGet(byte + 0x28C);
        v &= (1 << bit) ^ 0xFF;
        v |= *val << bit;
        swByteSet(byte + 0x28C, (u8)v);
    }
}

static f32 randf(f32 min, f32 max) {
    f32 r = (f32)rand() / 32767.0f;
    return r * (max - min) + min;
}

s32 randi(s32 min, s32 max) {
    s32 r = rand() % (max - min + 1);
    return r + min;
}

void qrndseed(s32 seed) {
    Punigp->seed0 = seed + 0x3A3B01F9;
    Punigp->seed1 = 0x21708331;
}

s32 qrnd(s32 n) {
    s32 v;
    Punigp->seed0 += Punigp->seed1 + 0x1733AF91;
    Punigp->seed1 += Punigp->seed0 + 0x21273F9C;
    v = Punigp->seed1 < 0 ? -Punigp->seed1 : Punigp->seed1;
    return v % n;
}

void puniDeleteEffect(PuniData* p) {
    if (p->effect) {
        effDelete(p->effect);
        p->effect = NULL;
    }
}

void _daime_change(PuniData* p) {
    if (p->child1) {
        p->child1->depth--;
        _daime_change(p->child1);
    }
    if (p->child2) {
        p->child2->depth--;
        _daime_change(p->child2);
    }
}

void PuniDokuritsu(PuniData* p) {
    PuniData* c1;
    PuniData* c2;
    PuniData* par;
    p->groupId = -1;
    p->flags &= ~0x80000;
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

PuniData* PuniSakanobori(PuniData* p) {
    while (p->parent != NULL) {
        p = p->parent;
    }
    return p;
}

void PuniChildHagure(PuniData *p)
{
    if (p->child1 != NULL)
    {
        PuniData *c = p->child1;
        c->mode = 16;
        p->modeStep = 0;
        PuniChildHagure(c);
        PuniDokuritsu(c);
    }
    if (p->child2 != NULL)
    {
        PuniData *c = p->child2;
        c->mode = 16;
        p->modeStep = 0;
        PuniChildHagure(c);
        PuniDokuritsu(c);
    }
}

BOOL searchParent(PuniData* p) {
    s32 level = 10000;
    PuniData* found;
    PuniData* q = Punidat;
    s32 i;

    for (i = 0; i < PUNI_DATA_MAX; i++, q++) {
        if ((q->flags & 0x80000) && q->child2 == NULL && (q->child1 == NULL || q->depth < 2) && q != p &&
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

BOOL searchMario(PuniData* p) {
    return p->marioDist < 150.0f;
}
static const Vec puni_fall_dir = {0.0f, -1.0f, 0.0f};

s32 puniPositionCheck(PuniData* p) {
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
    dir = puni_fall_dir;
    pos.y += 1.0f;
    dist = 3.0f;
    if (hitCheckFilter(NULL, &hit.x, &hit.y, &hit.z, &dist, &nrm.x, &nrm.y, &nrm.z, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z) == NULL) {
        return 2;
    }
    return 0;
}

void puniPositionRecover(PuniData* p) {
    p->pos = Punigp->recoverPos;
    p->pos.x += randf(0.0f, 2.0f) - 1.0f;
    p->pos.z += randf(0.0f, 2.0f) - 1.0f;
    p->prevPos = p->pos;
}


static const Vec puni_fall_dir;
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
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & 0x1000) {
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
        {
            f32 dp = PSVECDotProduct(&dir, &n);
            f32 t = -(dist - d);
            PSVECScale(&n, &n, dp * t + 0.1f);
        }
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
void puniMoveHitLimited(PuniData* p, Vec start, Vec dir, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec old;
    f32 b1, b2, b3;
    f32 c1, c2, c3;
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
        { f32 dp = PSVECDotProduct(&dir, &hit); PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f); }
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
}

static const Vec puni_fall_dir;
void puniMoveHit(PuniData* p, Vec start, Vec dir, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec refl;
    Vec old;
    f32 b1, b2, b3;
    f32 c1, c2, c3;
    f32 d;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & 0x1000) {
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
        { f32 dp = PSVECDotProduct(&dir, &hit); PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f); }
        PSVECAdd(&p->pos, &hit, &p->pos);
        PSVECSubtract(&p->pos, &old, &diff);
        d = PSVECMag(&diff);
        if (d > 0.0f) {
            PSVECNormalize(&diff, &diff);
            start = old;
            start.y += 1.0f;
            p->hitCheck = hitCheckFilter(NULL, &b1, &b2, &b3, &d, &c1, &c2, &c3, old.x, old.y + 1.0f, old.z, diff.x, diff.y, diff.z);
            if (p->hitCheck != NULL) {
                p->pos = old;
            }
        }
    }
}

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
    s2 = start;
    d2 = dir;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & 0x1000) {
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
        { f32 dot = PSVECDotProduct(&d2, &hit); PSVECScale(&hit, &hit, -(dist - d) * dot + 0.1f); }
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
        p->flags |= 0x400000;
    } else {
        PSVECScale(&p->vel, &p->vel, 0.6f);
        p->flags &= ~0x400000;
    }
}

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
        { f32 dp = PSVECDotProduct(&d2, &hit); PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f); }
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
        p->flags |= 0x400000;
    } else {
        p->flags &= ~0x400000;
    }
}

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
    if (p->flags & 0x1000) {
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
        { f32 dp = PSVECDotProduct(&d2, &hit); PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f); }
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
        p->flags |= 0x400000;
    } else {
        p->flags &= ~0x400000;
    }
}

void puniMoveSimple(PuniData* p) {
    if (PSVECMag(&p->vel)) {
        PSVECAdd(&p->pos, &p->vel, &p->pos);
    }
}

void puniMoveCustom(PuniData* p) {
    if (p->moveFunc) {
        p->moveFunc(p);
    }
}


void puniParaAttack(PuniData* a, PuniData* b) {
    s16 x, y, r;
    s32 c;
    if (b->attacked == 0 && b->mode == 9) {
        return;
    }
    if (b->mode != 8) {
        x = a->hitRate;
        y = a->hitRate + b->hitRate;
    } else {
        x = a->hitRateGuard;
        y = a->hitRateGuard + (100 - b->hitRateGuard);
    }
    r = qrnd(y);
    c = r < x;
    if (b->flags & 0x8000000) {
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
            b->flags |= 0x2000;
            b->flags &= ~0x8000000;
            b->flags2 &= ~2;
            b->mode = 9;
            b->modeStep = 0;
            b->flags &= ~0x2000000;
        }
    } else if (b->attacked == 0) {
        a->damage = b->attack - a->defense;
        {
            Vec paraVecB = { 0.0f, 0.0f, 0.0f };
            a->vel = paraVecB;
        }
        a->target = b;
        a->flags |= 0x2000;
        a->flags &= ~0x8000000;
        a->flags2 &= ~2;
        a->mode = 9;
        a->modeStep = 0;
        a->flags &= ~0x2000000;
    }
    a->attacked = 1;
    b->attacked = 1;
}

void puniParaDamage(PuniData* p) {
    p->hp -= p->damage;
    if (p->hp <= 0) {
        p->hp = 0;
        p->flags |= 0x4000;
        p->flags2 |= 0x10;
    }
}

void puniParaRecover(PuniData* p) {
    static PuniData* debug;
    s16 old;

    debug = p;
    if (p->moveDist > 0.001f || p->mode == 13 || p->mode == 10) {
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
                p->flags2 |= 1;
                p->flag1Timer = 0;
            }
        }
        p->regenTimer = p->regenInterval;
    }
}

BOOL puniParaDangerCheck(PuniData* p) {
    return p->hp * 100 / p->maxHp <= 30;
}

BOOL puniParaSelfCheck(PuniData* p) {
    return p->hp * 100 / p->maxHp >= 80;
}

BOOL puniParaRecoverCheck(PuniData* p) {
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

PuniData* puniSearchRandom(s32 start, s32 count) {
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
        if ((p->flags & 0x400) && !(p->flags & 0x4000)) {
            break;
        }
        idx++;
        i++;
    }
    return p;
}

PuniData* puniSearchRandom_Mikata(void) {
    return puniSearchRandom(1, 102);
}

PuniData* puniSearchRandom_Teki(void) {
    return puniSearchRandom(110, 100);
}

PuniData* searchBattle(PuniData* self, s32 start, s32 count, f32 maxDist) {
    s32 i;
    PuniData* q;
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;

    q = &Punidat[start];
    for (i = start; i < start + count; i++, q++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
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

PuniData* searchMikata(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[1];
    s32 i;

    for (i = 1; i < 103; i++, q++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
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

PuniData* searchTeki(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[110];
    s32 i;

    for (i = 110; i < 210; i++, q++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
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

PuniData* searchBattleMikata(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[1];
    s32 i;

    for (i = 1; i < 103; i++, q++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
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

static inline PuniData* searchBattleTeki_inl(PuniData* self) {
    s32 i;
    PuniData* q;
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;

    q = &Punidat[110];
    for (i = 110; i < 210; i++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
        q++;
    }
    {
    PuniData* ret;
    if (minDist < 1000000.0f) {
        ret = found;
    } else {
        ret = NULL;
    }
    return ret;
    }
}

static inline PuniData* searchBattleMikata_inl(PuniData* self) {
    s32 i;
    PuniData* q;
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;

    q = &Punidat[1];
    for (i = 1; i < 103; i++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
            f32 d = PSVECSquareDistance(&self->pos, &q->pos);
            if (d < minDist) {
                minDist = d;
                found = q;
            }
        }
        q++;
    }
    {
    PuniData* ret;
    if (minDist < 1000000.0f) {
        ret = found;
    } else {
        ret = NULL;
    }
    return ret;
    }
}

PuniData* searchBattleTeki(PuniData* self) {
    f32 minDist = 1000000.0f;
    PuniData* found = NULL;
    PuniData* q = &Punidat[110];
    s32 i;

    for (i = 110; i < 210; i++, q++) {
        if ((q->flags & 0x400) && !(q->flags & 0x4000) && q->mode != 9 && q->mode != 13) {
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

void puniSearch(PuniData* p) {
    if ((s32)(gp->retraceCount & 3) == (*( s16*)( &p->index) & 3)) {
        if (p->flags & 1) {
            p->target = searchBattleTeki_inl(p);
        } else {
            p->target = searchBattleMikata_inl(p);
        }
    }
}

void puniModeChangeAttack(void) {
    s32 i;
    PuniData* p = &Punidat[1];
    for (i = 1; i < 211; i++, p++) {
        if (p->flags & 0x400) {
            switch (p->mode) {
            case 0:
            case 1:
            case 2:
            case 0x1d: {
                PuniData* f;
                if (p->flags & 1) {
                    f = searchBattleTeki_inl(p);
                } else {
                    f = searchBattleMikata_inl(p);
                }
                if (f) {
                    p->target = f;
                    p->mode = 7;
                    p->modeStep = 0;
                    p->depth = 0;
                    p->flags |= 0x100000;
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
}

void puniModeChangeAway(void) {
    s32 i;
    PuniData* p = &Punidat[1];

    for (i = 1; i <= 102; i++, p++) {
        if (!(p->flags & 0x400)) {
            continue;
        }
        if (1000000.0f == p->awayTargetDistSq) {
            continue;
        }
        switch (p->mode) {
            case 0:
            case 1:
            case 2:
            case 4:
            case 16:
                PuniDokuritsu(p);
                p->prevMode = p->mode;
                p->mode = 5;
                p->modeStep = 0;
                if (p->effect != NULL) {
                    effDelete(p->effect);
                    p->effect = NULL;
                }
                break;
        }
    }
}

static const Vec puni_fallchk_dir = {0.0f, -1.0f, 0.0f};

void puniModeChangeFallCheck(void) {
    Vec dir;
    Vec pos;
    Vec nrm;
    Vec hit;
    f32 dist;
    s32 i;
    PuniData* p = &Punidat[1];

    for (i = 1; i <= 102; i++, p++) {
        if (!(p->flags & 0x400)) {
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
            p->mode = 17;
            p->modeStep = 0;
            if (p->effect != NULL) {
                effDelete(p->effect);
                p->effect = NULL;
            }
        }
    }
}

void puniFukki(PuniData* self) {
    s32 i;
    PuniData* p = &Punidat[1];

    for (i = 1; i <= 102; i++, p++) {
        if ((p->flags & 0x400) && p->mode == 16) {
            if (!(32.0 < __fabs(self->pos.y - p->pos.y)) && PSVECSquareDistance(&self->pos, &p->pos) < 900.0f) {
                p->mode = 1;
                puniFukki(p);
                puniDeleteEffect(p);
            }
        }
    }
}

void puniModeALLCheck(void) {
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

void puniModeALLEndCheck(void) {
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

void puniModeNormal(PuniData* p) {
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = 13;
        p->modeStep = 0;
    } else if (searchMario(p)) {
        p->mode = 1;
    }
}

void puniModeParentpre(PuniData* p) {
    Vec v;

    searchParent(p);
    p->mode = 2;
    p->flags |= 0x180000;
    PSVECSubtract(&p->parent->pos, &p->pos, &v);
    p->prevParentDist = p->parentDist = PSVECMag(&v);
    p->timer = 0;
    p->modeStep = 0;
    p->anim = 1;
}

void puniModeParent(PuniData* p) {
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
    if (p->flags & 0x200) {
        return;
    }
    flag = 0;
    PSVECSubtract(&p->parent->pos, &p->pos, &v);
    p->parentDist = PSVECMag(&v);
    if (!(bero_get_BeroEXEC() & 1)) {
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
            p->mode = 16;
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
        p->mode = 13;
        p->modeStep = 0;
        return;
    }
    if (p->awayTarget != -1 && p->awayTargetDistSq < 10000.0f) {
        Punigp->allMode = 2;
        return;
    }
    if (p->massTarget != -1) {
        PuniDokuritsu(p);
        p->mode = 4;
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
            p->mode = 3;
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

#define PSEARCH_POS(a, b)                                                                  \
    { s32 _pa = (b); s32 _pb = (a); switch (Punigp->mapKind) {                             \
            case 0: break; case 1: puniMapdata_MRI_06_getpos(_pa, _pb, &tgx, &tgz); break; \
            case 2: puniMapdata_MRI_09_getpos(_pa, _pb, &tgx, &tgz); break;                \
            case 3: puniMapdata_MRI_16_getpos(_pa, _pb, &tgx, &tgz); break;                \
        } }

void puniModeSearch(PuniData* p) {
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
        p->mode = 13;
        p->modeStep = 0;
        return;
    }
    if (Punimap->inRange == 0 && p->marioDist == 1000.0f) {
        p->mode = 16;
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
        p->mode = 1;
        p->flags |= 0x100000;
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
    if (!(cl & 0xFF)) { cl |= 0xFFFF; }
    m0 = cl;
    cu = Punimap->data[(bz - 1) * Punimap->width + bx];
    if (!(cu & 0xFF)) { cu |= 0xFFFF; }
    m1 = cu;
    k = bz * Punimap->width;
    k = bx + k;
    k++;
    cr = Punimap->data[k];
    if (!(cr & 0xFF)) { cr |= 0xFFFF; }
    m2 = cr;
    cd = Punimap->data[(bz + 1) * Punimap->width + bx];
    if (!(cd & 0xFF)) { cd |= 0xFFFF; }
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
                PSEARCH_POS(bz, bx - 1);
            } else if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz - 1, bx);
            } else if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx + 1);
            } else if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz + 1, bx);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        case 0x100:
            if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz - 1, bx);
            } else if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx + 1);
            } else if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz + 1, bx);
            } else if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx - 1);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        case 0x200:
            if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx + 1);
            } else if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz + 1, bx);
            } else if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx - 1);
            } else if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz - 1, bx);
            } else {
                tgx = 0.0f;
                tgz = 0.0f;
            }
            break;
        case 0x300:
            if (m3 == move && __abs((s32)h3 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz + 1, bx);
            } else if (m0 == move && __abs((s32)h0 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx - 1);
            } else if (m1 == move && __abs((s32)h1 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz - 1, bx);
            } else if (m2 == move && __abs((s32)h2 - (s32)takasa) <= 0x1000) {
                PSEARCH_POS(bz, bx + 1);
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

void puniModeMass(PuniData* p) {
    Vec v;
    PuniTarget* t = &Punigp->targets[p->massTarget];
    s32 a, b;
    if (p->massTarget == -1) {
        if (p->prevMode == 24) {
            p->mode = 24;
        } else {
            p->mode = 1;
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
    b = *(s16*)( &p->index ) & 0xF;
    if (Punigp->floor == 18) {
        a = b;
    }
    if (a == b && p->breathPower > 10.0f) {
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = 13;
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

void puniModeAwayjump(PuniData* p) {
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
            p->flags |= 0x1000000;
            if (p->flags & 0x80) {
                psndSFXOn_3D("SFX_STG2_TOGETTI_JUMP1", &p->pos);
            } else {
                psndSFXOn_3D("SFX_STG2_PUNI_JUMP1", &p->pos);
            }
        case 1: {
            u32 t = OSTicksToMilliseconds((u32)gp->renderTime - p->startTimeLo);
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
                p->mode = 6;
                p->anim = 2;
                p->flags &= ~0x1000000;
                p->modeStep = 0;
            }
            break;
        }
    }
}


static const Vec puni_away_zero0 = {0.0f, 0.0f, 0.0f};
#pragma dont_inline on
int puniModeAway_sub(PuniData* p) {
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
        if (p->hitMove != NULL && (p->hitMove->attributes & 4)) {
            p->pushDir.x = -p->wallNrm.x;
            p->pushDir.y = -p->wallNrm.y;
            p->pushDir.z = -p->wallNrm.z;
            p->modeStep = 1;
            return 1;
        } else if (p->hitCheck != NULL && (p->hitCheck->attributes & 4)) {
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

void puniModeAway(PuniData* p) {
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
        p->mode = 1;
        p->modeStep = 0;
        p->anim = 0;
        return;
    }
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        p->target = Punigp->marioPuni;
        p->mode = 13;
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
            p->flags2 |= 4;
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
                p->mode = 17;
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

void puniModeAttack(PuniData* p) {
    PuniData* best;
    Vec d;
    f32 m;
    s32 i;
    PuniData* q;
    switch (p->modeStep) {
        case 0:
            p->anim = 1;
            p->modeStep = 1;
        case 1:
            if (p->breathPower > 10.0f) {
                p->prevMode = p->mode;
                PuniDokuritsu(p);
                p->target = Punigp->marioPuni;
                p->mode = 13;
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

void puniModeDamage(PuniData* p) {
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
                p->flags &= ~0x2000;
                p->pos.y = 0.0f;
                p->accelStep = 0.0f;
                p->prevPos = p->pos;
                p->hp -= p->damage;
                if (p->hp <= 0) {
                    p->hp = 0;
                    p->flags |= 0x4000;
                    p->flags2 |= 0x10;
                }
                if ((p->hp * 100) / p->maxHp <= 30) {
                    p->mode = 8;
                    p->anim = 2;
                    p->modeStep = 0;
                } else {
                    p->mode = 0;
                    p->anim = 0;
                    p->modeStep = 0;
                }
            }
            break;
    }
}


void puniModePiyori(PuniData* p) {
    if (p->breathPower > 10.0f) {
        p->prevMode = p->mode;
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->flags &= ~0x8000000;
        p->flags2 &= ~2;
        p->mode = 13;
        p->modeStep = 0;
        return;
    }
    switch (p->modeStep) {
        case 0:
            if (p->flags & 2) {
                p->timer = 600;
                p->anim = 4;
                p->flags |= 0x8100000;
                p->flags2 |= 2;
                p->modeStep = 2;
            } else {
                p->modeStep = 10;
            }
            break;
        case 2:
            if (p->timer-- == 0) {
                p->flags &= ~0x8000000;
                p->flags2 &= ~2;
                p->modeStep = 10;
            }
            break;
        case 10:
            if (p->prevMode == 10) {
                if (p->hp * 100 / p->maxHp <= 30) {
                    p->mode = 8;
                    p->anim = 2;
                    p->modeStep = 0;
                } else {
                    p->mode = 0;
                    p->anim = 0;
                    p->modeStep = 0;
                }
            } else {
                p->mode = p->prevMode;
#ifdef VERSION_G8ME01
                p->anim = 2;
#endif
                if (p->prevMode == 2) {
                    p->mode = 1;
                }
                p->modeStep = 0;
            }
            break;
    }
}


void puniModeEscape(PuniData* p) {
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
        p->mode = 13;
        p->modeStep = 0;
        return;
    }
    if (p->hp * 100 / p->maxHp >= 80) {
        p->mode = 0;
        p->anim = 0;
        p->modeStep = 0;
        return;
    }
    if ((s32)(gp->retraceCount & 3) == (p->index & 3)) {
        if (p->flags & 1) {
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
        p->mode = 11;
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


void puniModeRecover(PuniData* p) {
    switch (p->modeStep) {
        case 0: {
            int r = puniPositionCheck(p);
            if (r != 0) {
                p->reserved_58C = r;
                p->reserved_590 = p->pos;
                puniPositionRecover(p);
                p->reserved_59C = p->pos;
            }
            {
                Vec puni_recover_vel = {0.0f, 0.0f, 0.0f};
                p->vel = puni_recover_vel;
            }
            p->modeStep = 1;
            break;
        }
        case 1: {
            if (p->hp * 100 / p->maxHp >= 80) {
                p->mode = 0;
                p->anim = 0;
                p->modeStep = 0;
            } else if (p->breathPower > 10.0f) {
                p->prevMode = p->mode;
                p->target = Punigp->marioPuni;
                p->mode = 13;
                p->modeStep = 0;
            } else {
                if (((s32)gp->retraceCount & 3) == (*(s16 *) (&p->index) & 3)) {
                    if (p->flags & 1) {
                        p->target = searchBattleTeki_inl(p);
                    } else {
                        p->target = searchBattleMikata_inl(p);
                    }
                }
                if (p->target != NULL) {
                    Vec d;
                    PSVECSubtract(&p->target->pos, &p->pos, &d);
                    if (PSVECMag(&d) < 80.0f) {
                        p->mode = 8;
                        p->anim = 2;
                        p->modeStep = 0;
                    }
                }
            }
            break;
        }
    }
}

void puniModeDead(PuniData* p) {
    switch (p->modeStep) {
        case 0:
            psndSFXOn_3D("SFX_STG2_FIGHT_TOGE3", &p->pos);
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

int puniModeWind_sub(PuniData* p) {
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
        if (p->hitMove != NULL && (p->hitMove->attributes & 4)) {
            p->pushDir.x = -p->wallNrm.x;
            p->pushDir.y = 0.0f;
            p->pushDir.z = -p->wallNrm.z;
            p->savedPos = p->pos;
            p->modeStep = 3;
            return 1;
        } else if (p->hitCheck != NULL && (p->hitCheck->attributes & 4)) {
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

void puniModeWind_kaiten(PuniData* p) {
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



void puniModeWind(PuniData *p);
static const Vec puni_fall_dir;
void puniModeWind(PuniData* p) {
    MarioWork* mario = marioGetPtr();
    f32 hz, hy, hx;
    f32 nz, ny, nx;
    f32 dist;
    Vec d;

    switch (p->modeStep) {
        case 0:
            if (p->flags & 0x18) {
                p->rotOffsetY = 8.0f;
            } else {
                p->dispOfs.y = 8.0f;
            }
            p->flags &= ~0x100000;
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
            if (p->flags & 1) {
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
                if (p->flags & 1) {
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
            p->flags2 |= 4;
            PSVECNormalize(&p->pushDir, &d);
            PSVECScale(&d, &p->vel, p->radius.x);
            p->modeStep = 4;
            // fallthrough
        case 4: {
            Vec start;

            PSVECAdd(&p->pos, &p->vel, &p->pos);
            start = p->pos;
            dist = 2.0f;
            d = (Vec){0.0f, -1.0f, 0.0f};
            start.y += 0.2f;
            if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, start.x, start.y, start.z, d.x, d.y, d.z) == NULL) {
                p->vel = (Vec){0.0f, 0.0f, 0.0f};
                p->mode = 17;
                p->anim = 0;
                p->rotOffsetY = 0.0f;
                p->spin = 0.0f;
                p->dispOfs.y = 0.0f;
                p->modeStep = 0;
            } else {
                p->modeStep = 10;
            }
            break;
        }
        case 10:
            puniModeWind_kaiten(p);
            if (!(p->breathPower > 0.0f)) {
                if (Punigp->allMode == 3) {
                    p->mode = 10;
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
                    p->mode = 1;
                    p->modeStep = 0;
                }
            }
            break;
    }
}

void puniModeFall(PuniData* p) {
    Vec start;
    Vec dir;
    f32 mag;
    PuniTarget* t = Punigp->targets;
    HitEntry* h0;
    HitEntry* h1;
    s32 found;
    s32 i;

    switch (p->modeStep) {
    case 0:
        p->flags &= ~0x00100000;
        {
            Vec v = {0.0f, -0.8f, 0.0f};
            p->vel = v;
        }
        p->modeStep = 1;
        break;
    case 1:
        p->vel.x = 0.0f;
        p->vel.y -= 0.45f;
        {
            Vec v = {0.0f, -1.0f, 0.0f};
            dir = v;
        }
        mag = PSVECMag(&p->vel);
        start = p->pos;
        puniMoveAtari(p, start, dir, 0, mag);
        h0 = p->hitMove;
        if (h0 != NULL || p->hitCheck != NULL) {
            if (h0 != NULL && (h0->attributes & 4)) {
                break;
            }
            h1 = p->hitCheck;
            if (h1 != NULL && (h1->attributes & 4)) {
                break;
            }
            if (h0 != NULL && (h0->attributes & 0x100)) {
                p->modeVar0 = 0x12;
                p->anim = 0;
                p->modeStep = 2;
                break;
            }
            if (h1 != NULL && (h1->attributes & 0x100)) {
                p->modeVar0 = 0x12;
                p->anim = 0;
                p->modeStep = 2;
                break;
            }
            if (h0 != NULL && (h0->attributes & 0x200)) {
                p->modeVar0 = 0x16;
                p->anim = 3;
                p->modeStep = 2;
                break;
            }
            if (h1 != NULL && (h1->attributes & 0x200)) {
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
            if (Punigp->floor == 0x12 && p->pos.y < -300.0f) {
                s32 zero = 0;
                puniswTrapSetindex(*(s16 *) (&p->index), &zero);
                p->flags &= ~0x00800000;
            }
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                p->vel = v;
            }
            p->modeStep = 2;
        } else if (p->pos.y <= -1000.0f) {
            p->reserved_590 = p->pos;
            p->pos = Punigp->recoverPos;
            p->pos.x += randf(0.0f, 2.0f) - 1.0f;
            p->pos.z += randf(0.0f, 2.0f) - 1.0f;
            p->prevPos = p->pos;
            p->reserved_59C = p->pos;
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

void puniModePreupdraft(PuniData* p) {
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
                p->npc->flags |= 4;
                p->mode = 19;
                p->modeStep = 0;
            }
            break;
    }
}


static const Vec puni_fall_dir;
void puniModeUpdraft(PuniData *p)
{
    MarioWork *mario = marioGetPtr();
    Vec d;
    Vec save;
    Vec savev;
    Vec dir;
    f32 mag;
    f32 fx;
    f32 fz;
    s32 a;
    s32 b;
    s32 t;
    if (p->modeStep == 0)
    {
        {
            Vec v_ = {0.0f, 0.94f, 0.0f};
            p->vel = v_;
        }
    }
    else
    {
        {
            Vec v_ = {0.0f, 0.66f, 0.0f};
            p->vel = v_;
        }
    }
    if ((gp->retraceCount & 0x1F) == 0)
    {
        psndSFXOn_3D("SFX_STG2_SOAPBALL_MOVE1", &p->pos);
    }
    t = (gp->retraceCount + p->index) & 0xF;
    if (t > 7)
    {
        t = 4 - (t & 7);
    }
    else
    {
        t = t - 3;
    }
    p->dispOfs.x = 0.5f * ((f32) t);
    switch (p->modeStep)
    {
        case 0:
            if (p->breathPower > 10.0f)
        {
            p->target = Punigp->marioPuni;
            PSVECSubtract(&p->pos, &mario->position, &d);
            mag = PSVECMag(&d);
            if (0.0f != mag)
            {
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
            if ((p->modeF0) >= (p->modeF2))
        {
            p->modeF0 = p->modeF2;
        }
            break;

        case 2:
            PSVECScale(&p->moveStart, &d, p->modeF0);
            PSVECAdd(&p->vel, &d, &p->vel);
            p->modeF0 -= p->modeF1;
            p->modeF1 += 0.03f;
            if ((p->modeF0) < 0.0f)
        {
            p->modeStep = 0;
        }
            break;

    }

    mag = PSVECMag(&p->vel);
    if (0.0f != mag)
    {
        PSVECNormalize(&p->vel, &d);
    }
    else
    {
        {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            d = v_;
        }
    }
    save = p->pos;
    savev = p->vel;
    puniMoveAtari(p, save, d, 1, mag);
    {
        Vec q = p->pos;
        q.x += (p->dispOfs.x);
        q.y += 15.0f;
        q.z += 3.0f;
        ((PuniBalloonWork*)p->effect->userdata)->position.x = q.x;
        ((PuniBalloonWork*)p->effect->userdata)->position.y = q.y;
        ((PuniBalloonWork*)p->effect->userdata)->position.z = q.z;
        if ((p->hitMove != NULL) || (p->hitCheck != NULL))
        {
            s32 f = 0;
            if ((p->hitMove != NULL) && (p->hitMove->attributes & 4))
            {
                f = 1;
            }
            if ((p->hitCheck != NULL) && (p->hitCheck->attributes & 4))
            {
                f = 1;
            }
            if (f == 0)
            {
                p->mode = 20;
                p->modeStep = 0;
                p->dispOfs.x = 0.0f;
            }
            else
            {
                p->pos = save;
                p->vel = savev;
                PSVECAdd(&p->pos, &p->vel, &p->pos);
            }
        }
        if (p->pos.y >= 0.0f)
        {
            p->mode = 20;
            p->modeStep = 0;
            p->dispOfs.x = 0.0f;
        }
    }
}

void puniModeWonder(PuniData* p) {
    Vec pos;

    switch (p->modeStep) {
        case 0:
            psndSFXOn_3D("SFX_STG2_SOAPBALL_BREAK1", &p->pos);
            if (p->effect != NULL) {
                effDelete(p->effect);
                p->effect = NULL;
            }
            pos = p->pos;
            pos.y += 15.0f;
            pos.z += 3.0f;
#ifdef VERSION_G8ME01
            effPuniBalloonEntry(1, pos.x, pos.y, pos.z, 1.5f);
#else
            p->effect = effPuniBalloonEntry(1, pos.x, pos.y, pos.z, 1.5f);
#endif
            p->timer = 20;
            p->anim = 9;
            p->animTime = 0;
            p->flags |= 0x1000000;
            p->modeStep = 1;
        case 1:
            if (p->timer-- == 0) {
                p->npc->flags &= ~4;
                p->mode = 21;
                p->anim = 1;
                p->flags &= ~0x1000000;
                p->modeStep = 0;
            }
            break;
    }
}

static inline void puniMoveHitLimitedInl(PuniData* p, Vec* s2, Vec* d2, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec old;
    f32 b1, b2, b3;
    f32 c1, c2, c3;
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
        { f32 dp = PSVECDotProduct(d2, &hit); PSVECScale(&hit, &hit, dp * -(dist - d) + 0.1f); }
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
}

void puniModeFunbari(PuniData* p) {
    switch (p->modeStep) {
        case 0: {
            PuniTarget* t;
            int i;
            t = Punigp->targets;
            for (i = 0; i < 10; i++, t++) {
                if ((t->flags & 1) && (t->flags & 2)) break;
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
#ifdef VERSION_G8ME01
            p->dispOfs.y = 0.0f;
#endif
            p->npc->flags |= 4;
            p->modeStep = 1;
            break;
        }
        case 1:
        case 2:
            {
                f32 s = sin(3.1415927f * p->modeF0 / 180.0f);
                if (p->modeF0 <= 180.0f) {
                    p->modeF1 = 0.5f * s;
                } else {
                    p->modeF1 = 0.45f * s;
                }
            }
            p->modeF0 += 7.0f;
            if (p->modeF0 >= 360.0f) p->modeF0 = 360.0f;
#ifdef VERSION_G8ME01
            {
                Vec start;
                Vec s2;
                Vec d2;
                f32 dist;
                start = p->pos;
                dist = __fabs(p->modeF2);
                p->vel.x = 0.5f * p->modeF2;
                p->vel.z = p->vel.y = 0.0f;
                d2 = p->vel;
                s2 = start;
                puniMoveHitLimitedInl(p, &s2, &d2, dist);
            }
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
        case 10:
            {
                Vec v_ = {0.0f, 0.0f, 0.0f};
                p->vel = v_;
            }
            p->mode = 17;
            p->anim = 0;
            p->npc->flags &= ~4;
            p->modeStep = 0;
            break;
    }
}

void puniModeWater(PuniData* p) {
    Vec d;
    switch (p->modeStep) {
        case 0:
            p->flags |= 0x800;
            p->timer = 60;
            p->modeStep = 1;
        case 1:
            if (p->timer-- == 0) {
                p->flags &= ~0x800;
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
            {
                f32 x = d.x + p->pos.x;
                f32 z = d.z + p->pos.z;
                p->savedPos.x = x;
                p->savedPos.z = z;
            }
            p->modeStep = 3;
        case 3: {
            u32 t = OSTicksToMilliseconds((u32)gp->renderTime - p->startTimeLo);
            if ((s32)t < 300) {
                f32 f = (f32)(s32)t / 300.0f;
                p->pos.x = intplGetValue(0, p->moveStart.x, p->savedPos.x, t, 300);
                p->pos.z = intplGetValue(0, p->moveStart.z, p->savedPos.z, t, 300);
                p->pos.y = f * (p->modeF0 * f + p->modeF1) + p->moveStart.y;
                p->prevPos = p->pos;
            } else {
                p->pos = p->savedPos;
                p->prevPos = p->pos;
                p->modeStep = 4;
            }
            break;
        }
        case 4:
            p->mode = 16;
            p->anim = 0;
            p->modeStep = 0;
            break;
    }
}

void puniModePrehagure(PuniData* p) {
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
                p->reserved_58C = res;
                p->reserved_590 = p->pos;
                p->pos = Punigp->recoverPos;
                p->pos.x += randf(0.0f, 2.0f) - 1.0f;
                p->pos.z += randf(0.0f, 2.0f) - 1.0f;
                p->prevPos = p->pos;
                p->reserved_59C = p->pos;
            }
            p->modeStep = 1;
            break;
        case 1:
            p->mode = 16;
            p->modeStep = 0;
            break;
    }
}

extern EffectEntry* effFukidashiEntry(s32 a, s32 b, void* c, s32 d, f32 e, f32 f, f32 g, f32 h, f32 i);

void puniModeHagure(PuniData* p) {
    switch (p->modeStep) {
        case 0:
            p->effect = effFukidashiEntry(4, 0, (u8*)&p->bubblePos - offsetof(NpcEntry, position), -1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            p->anim = 6;
            p->flags |= 0x100000;
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
        p->mode = 4;
        p->modeStep = 0;
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    } else if (p->breathPower > 10.0f) {
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->mode = 13;
        p->modeStep = 0;
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    } else if (p->marioDist < 150.0f) {
        p->mode = 1;
        puniFukki(p);
        if (p->effect != NULL) {
            effDelete(p->effect);
            p->effect = NULL;
        }
    }
}

void puniModeDodai(PuniData* p) {
    Vec v;

    switch (p->modeStep) {
        case 0:
            PSVECSubtract(&p->pos, &p->moveStart, &v);
            if (PSVECMag(&v) < 10.0f) {
                {
                    Vec zero = {0.0f, 0.0f, 0.0f};
                    p->vel = zero;
                }
                p->mode = 16;
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

void puniModeTrap_sub(PuniData* p) {
    NpcEntry* n;
    if (p->flags & 0x20) {
        n = npcNameToPtr("\203K\203C\203h");
        p->flags &= ~0x800;
        p->flags |= 0x100000;
        n->flags &= ~0x20;
        n->position.x = 0.0f;
        n->position.y = -1000.0f;
        n->position.z = 0.0f;
    }
    if (p->flags & 0x40) {
        n = npcNameToPtr("\203K\203C\203h\226\205");
        p->flags &= ~0x800;
        p->flags |= 0x100000;
        n->flags &= ~0x20;
        n->position.x = 0.0f;
        n->position.y = -1000.0f;
        n->position.z = 0.0f;
    }
    if (p->flags & 0x100) {
        n = npcNameToPtr("\203\211\203C\203o\203\213");
        p->flags &= ~0x800;
        p->flags |= 0x100000;
        n->flags &= ~0x20;
        n->position.x = 0.0f;
        n->position.y = -1000.0f;
        n->position.z = 0.0f;
    }
}

#define TRAP_NPC(name, setpos)                \
    {                                         \
        n = npcNameToPtr(name);               \
        p->flags |= 0x800;                     \
        n->position.x = p->pos.x;             \
        n->position.y = p->pos.y;             \
        n->position.z = p->pos.z;             \
        n->direction = p->dir;                   \
        n->flags |= 0x20;                     \
    }
#define TRAP_HIDE(name)                       \
    {                                         \
        n = npcNameToPtr(name);               \
        p->flags &= ~0x800;                    \
        p->flags |= 0x100000;                  \
        n->flags &= ~0x20;                    \
        n->position.x = 0.0f;                 \
        n->position.y = -1000.0f;             \
        n->position.z = 0.0f;                 \
    }


void puniModeTrap(PuniData* p) {
    NpcEntry* n;
    if (p->modeStep == 0) {
        p->flags |= 0x100000;
        if (p->flags & 0x20) TRAP_NPC("\203K\203C\203h", 0)
        if (p->flags & 0x40) TRAP_NPC("\203K\203C\203h\226\205", 0)
        if (p->flags & 0x100) TRAP_NPC("\203\211\203C\203o\203\213", 0)
        {
            Vec v_ = {0.0f, 0.0f, 0.0f};
            p->vel = v_;
        }
        p->prevMode = p->mode;
    }
    if (p->breathPower > 10.0f) {
        if (p->flags & 0x20) TRAP_HIDE("\203K\203C\203h")
        if (p->flags & 0x40) TRAP_HIDE("\203K\203C\203h\226\205")
        if (p->flags & 0x100) TRAP_HIDE("\203\211\203C\203o\203\213")
        PuniDokuritsu(p);
        p->target = Punigp->marioPuni;
        p->prevMode = p->mode;
        p->mode = 13;
        p->modeStep = 0;
    } else if (p->massTarget != -1) {
        if (p->flags & 0x20) TRAP_HIDE("\203K\203C\203h")
        if (p->flags & 0x40) TRAP_HIDE("\203K\203C\203h\226\205")
        if (p->flags & 0x100) TRAP_HIDE("\203\211\203C\203o\203\213")
        PuniDokuritsu(p);
        p->prevMode = p->mode;
        p->mode = 4;
        p->modeStep = 0;
    }
}

void puniModeEnterSubInit(PuniData* p) {
    if (Punigp->flags & 3) {
        p->modeVar0 = 0;
        p->scale = 0.0f;
        p->npc->flags |= 4;
    }
}

void puniModeEnterSub(PuniData* p) {
    if (Punigp->flags & 3) {
        p->modeVar0++;
        if (p->modeVar0 >= 10) {
            p->scale += 0.1f;
            if (p->scale >= 1.0f) {
                p->scale = 1.0f;
            }
        }
    }
}

void puniModeEnter(PuniData* p) {
    Vec d;
    switch (p->modeStep) {
        case 0:
            {
                p->startTime = gp->renderTime;
            }
            p->timer = 200;
            if ((Punigp->flags & 8) && !(Punigp->flags & 0x10)) {
                p->timer = 700;
            }
            p->timer += qrnd(500);
            if (Punigp->flags & 3) {
                p->modeVar0 = 0;
                p->scale = 0.0f;
                p->npc->flags |= 4;
            }
            p->modeStep = 1;
        case 1: {
            u32 t = OSTicksToMilliseconds((u32)gp->renderTime - p->startTimeLo);
            if ((s32)t > p->timer) {
                p->modeStep = 2;
            }
            break;
        }
        case 2:
            if (Punigp->flags & 3) {
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
                p->flags |= 0x1000;
                {
                    Vec puni_enter_dir = {0.0f, 0.0f, 0.0f};
                    p->vel = puni_enter_dir;
                }
                p->scale = 1.0f;
                p->npc->flags &= ~4;
                p->modeStep = 3;
            }
            break;
        case 3: {
            searchParent(p);
            p->mode = 2;
            p->flags |= 0x180000;
            PSVECSubtract(&p->parent->pos, &p->pos, &d);
            p->prevParentDist = p->parentDist = PSVECMag(&d);
            p->timer = 0;
            p->modeStep = 0;
            p->anim = 1;
            break;
        }
    }
}

void puniModeExitSubInit(PuniData* p) {
    if (Punigp->flags & 3) {
        p->modeVar0 = 0;
        p->npc->flags |= 4;
    }
}

void puniModeExitSub(PuniData* p) {
    if (Punigp->flags & 3) {
        p->modeVar0++;
        if (p->modeVar0 >= 10) {
            p->scale -= 0.3f;
            if (p->scale <= 0.0f) {
                p->scale = 0.0f;
            }
        }
    }
}

void puniModeExit(PuniData* p) {
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
                p->flags &= ~0x100000 & ~0x1000;
                if (Punigp->flags & 3) {
                    p->modeVar0 = 0;
                    p->npc->flags |= 4;
                }
                p->modeStep = 1;
            }
            break;
        case 1:
            if (Punigp->flags & 3) {
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
                p->mode = 30;
                p->flags &= ~0x100000;
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

void puniModePoint(PuniData* p) {
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
                p->flags &= ~0x100000 & ~0x1000;
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
                p->mode = 30;
                p->flags &= ~0x100000;
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

void puniModeDokan(PuniData* p) {
    p->modeStep = 0;
    p->flags |= 0x800;
    p->npc->flags |= 4;
    {
        Vec v = {0.0f, 0.0f, 0.0f};
        p->vel = v;
    }
}

void puniModeCustom(PuniData* p) {
    if (p->modeFunc) {
        p->modeFunc(p);
    }
}

void puni_puni_disp(CameraId camId, void* param) {
    MapTevCallbackInfo info;
    s32 off;
    MapTevCallbackInfo* ip;
    s32 i;
    PunidrvPose* pose;

    puniLoadRenderMode();
    puniLoadVertex();
    puniLoadTexture();
    puniLoadTev();
    if (!_swGet(3)) {
        puniDraw();
    } else {
        for (ip = &info, i = 0, off = 0; i < puniGetPoseNum(); i++, off += sizeof(PunidrvPose)) {
            pose = (PunidrvPose*)((u8*)puniGetPosePtr() + off);
            ip->tevStage = 1;
            ip->texCoord = 1;
            ip->texMap = 1;
            ip->indStage = 0;
            ip->mtx = *pose->mtx;
            envSetYami(ip);
            GXSetNumTexGens(ip->texMap);
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
static f32 shadow_vtx[16] = {
    -1.2f, 0.0f, -1.2f, -1.2f, 0.0f, 1.2f, 1.2f, 0.0f, 1.2f, 1.2f, 0.0f, -1.2f,
};
static f32 shadow_coord[8] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

void puni_puni_shadow_disp(CameraId camId, void* param) {
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
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        GXCallDisplayList(shadow_dl, 0x20);
    }
}

void puni_puni_init(void) {
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
    p->flags |= 0x180404;
    p->pos = mario->position;
    p->prevPos = p->pos;
    p->radius.x = 10.0f;
    p->mode = 30;
    p->index = 0;
    p->groupId = 0;
    p = &Punidat[1];
    for (i = 1; i < 211; i++) {
        pose->flags = 1;
        pose->flags |= 2;
        pose->dir = 0.0f;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = -1000.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->offsetY = 0.0f;
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
    Punimap = _mapAlloc(0x24);
    memset(Punimap, 0, 0x24);
}



void puniModeMain(PuniData* p);
static const Vec puni_fall_dir;
static inline void puniModeCheckPos(PuniData* p) {
    switch (p->modeStep) {
        case 0: {
            s32 r = puniPositionCheck(p);
            if (r != 0) {
                p->reserved_58C = r;
                p->reserved_590 = p->pos;
                puniPositionRecover(p);
                p->reserved_59C = p->pos;
            }
            p->modeStep = 1;
            break;
        }
        case 1:
            p->mode = 16;
            p->modeStep = 0;
            break;
    }
}

void puniModeMain(PuniData* p) {
    switch (p->mode) {
        case 0:
            p->flags &= ~0x8000;
            if (p->breathPower > 10.0f) {
                p->prevMode = p->mode;
                PuniDokuritsu(p);
                p->target = Punigp->marioPuni;
                p->mode = 13;
                p->modeStep = 0;
            } else if (p->marioDist < 150.0f) {
                p->mode = 1;
            }
            break;
        case 1: {
            Vec d;
            p->flags &= ~0x8000;
            searchParent(p);
            p->mode = 2;
            p->flags |= 0x180000;
            PSVECSubtract(&p->parent->pos, &p->pos, &d);
            {
                f32 m = PSVECMag(&d);
                p->parentDist = m;
                p->prevParentDist = m;
            }
            p->timer = 0;
            p->modeStep = 0;
            p->anim = 1;
            break;
        }
        case 2:
            p->flags &= ~0x8000;
            puniModeParent(p);
            break;
        case 3:
            p->flags &= ~0x8000;
            puniModeSearch(p);
            break;
        case 4:
            p->flags |= 0x8000;
            puniModeMass(p);
            break;
        case 5:
            p->flags |= 0x8000;
            puniModeAwayjump(p);
            break;
        case 6:
            p->flags |= 0x8000;
            puniModeAway(p);
            break;
        case 7:
            p->flags &= ~0x8000;
            puniModeAttack(p);
            break;
        case 9:
            p->flags &= ~0x8000;
            puniModeDamage(p);
            break;
        case 10:
            p->flags &= ~0x8000;
            puniModePiyori(p);
            break;
        case 8:
            p->flags &= ~0x8000;
            puniModeEscape(p);
            break;
        case 11:
            p->flags &= ~0x8000;
            puniModeRecover(p);
            break;
        case 12:
            p->flags &= ~0x8000;
            puniModeDead(p);
            break;
        case 13:
            p->flags &= ~0x8000;
            puniModeWind(p);
            break;
        case 14:
            break;
        case 15:
            p->flags |= 0x8000;
            puniModeCheckPos(p);
            break;
        case 16:
            p->flags |= 0x8000;
            puniModeHagure(p);
            break;
        case 18:
            p->flags |= 0x8000;
            puniModePreupdraft(p);
            break;
        case 19:
            p->flags |= 0x8000;
            puniModeUpdraft(p);
            break;
        case 20:
            p->flags |= 0x8000;
            puniModeWonder(p);
            break;
        case 21:
            p->flags |= 0x8000;
            puniModeFunbari(p);
            break;
        case 22:
            p->flags |= 0x8000;
            puniModeWater(p);
            break;
        case 17:
            p->flags |= 0x8000;
            puniModeFall(p);
            break;
        case 23:
            p->flags |= 0x8000;
            puniModeDodai(p);
            break;
        case 24:
            p->flags &= ~0x8000;
            puniModeTrap(p);
            break;
        case 25:
            p->flags &= ~0x8000;
            puniModeEnter(p);
            break;
        case 26:
            p->flags &= ~0x8000;
            puniModeExit(p);
            break;
        case 28:
            p->flags &= ~0x8000;
            puniModeDokan(p);
            break;
        case 27:
            p->flags &= ~0x8000;
            puniModePoint(p);
            break;
        case 29:
            p->flags &= ~0x8000;
            if (p->modeFunc != NULL) {
                p->modeFunc(p);
            }
            break;
    }
}


void puniMoveMain(PuniData* p);
static const Vec puni_fall_dir;
static inline void puniMoveHitInl(PuniData* p, Vec* s2, Vec* d2, f32 dist) {
    Vec pa;
    Vec hit;
    Vec diff;
    Vec refl;
    Vec old;
    f32 b1, b2, b3;
    f32 c1, c2, c3;
    f32 d;
    p->hitCheck = NULL;
    p->hitMove = NULL;
    d = dist;
    if (p->flags & 0x1000) {
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
        { f32 dot = PSVECDotProduct(d2, &hit); PSVECScale(&hit, &hit, -(dist - d) * dot + 0.1f); }
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
}

void puniMoveMain(PuniData* p) {
    switch (p->mode) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 7:
        case 8:
        case 10:
        case 14:
        case 16:
        case 23:
        case 25:
        case 26:
        case 27:
        {
            Vec d2;
            Vec s2;
            Vec dir;
            Vec start;
            f32 dist = PSVECMag(&p->vel);
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
                p->flags |= 0x400000;
            } else {
                PSVECScale(&p->vel, &p->vel, 0.6f);
                p->flags &= ~0x400000;
            }
        }
        mv_end1:
            puniSetYPosition(p);
            break;
        case 4:
        {
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
                p->flags |= 0x400000;
            } else {
                p->flags &= ~0x400000;
            }
        }
        mv_end2:
            puniSetYPosition(p);
            break;
        case 9:
        {
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
                p->flags |= 0x400000;
            } else {
                p->flags &= ~0x400000;
            }
        }
        mv_end3:
            puniSetYPosition(p);
            break;
        case 29:
        {
            if (p->moveFunc) {
                p->moveFunc(p);
            }
        }
            puniSetYPosition(p);
            break;
        case 5:
        case 6:
        case 13:
            puniSetYPosition(p);
            break;
        case 24:
        {
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
                p->flags |= 0x400000;
            } else {
                PSVECScale(&p->vel, &p->vel, 0.6f);
                p->flags &= ~0x400000;
            }
        }
        mv_end5:
            puniSetYPosition(p);
            break;
    }
}

void puniOption(void) {
    PuniData* p;
    PuniData* q;
    PuniData* start;
    PuniData* end;
    Vec diff;
    f32 dist, r2, r;

    for (p = Punidat; p < Punidat + PUNI_DATA_MAX; p++) {
        p->flags &= ~0x20000000;
        if (!(p->flags & 0x400) || !(p->flags & 0x100000)) {
            continue;
        }
        if (p < Punidat + 110) {
            start = Punidat;
            end = Punidat + 109;
        } else {
            start = Punidat + 110;
            end = Punidat + 209;
        }
        for (q = start; q <= end; q++) {
            if (!(q->flags & 0x400) || !(q->flags & 0x100000) || p == q) {
                continue;
            }
            dist = PSVECDistance(&p->pos, &q->pos);
            r = p->radius.x + q->radius.x;
            r2 = p->radius.z + (r + p->radius.y);
            if (q->flags2 & 0x20) {
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
                    p->flags |= 0x20000000;
                }
                if (!(p->flags & 0x10000000) && dist < p->radius.x + q->radius.x) {
                    p->flags |= 0x10000000;
                }
                if (p->flags & 0x10000000) {
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
        if (!(p->flags & 0x400) || !(p->flags & 0x100000)) {
            continue;
        }
        if (!(p->flags & 0x20000000)) {
            p->flags &= ~0x10000000;
        }
        if (PSVECMag(&p->vel) >= 8.0f) {
            PSVECNormalize(&p->vel, &p->vel);
            PSVECScale(&p->vel, &p->vel, 8.0f);
        }
    }
}

void puniCollideAll(void) {
    PuniData* p;
    PuniData* q;
    PuniData* start;
    PuniData* end;
    Vec diff;
    f32 dist, r2, r;

    for (p = Punidat; p <= Punidat + 109; p++) {
        p->flags &= ~0x20000000;
        if (!(p->flags & 0x400) || !(p->flags & 0x100000)) {
            continue;
        }
        for (q = Punidat; q <= Punidat + 109; q++) {
            if (!(q->flags & 0x400) || !(q->flags & 0x100000) || p == q) {
                continue;
            }
            dist = PSVECDistance(&p->pos, &q->pos);
            r = p->radius.x + q->radius.x;
            r2 = p->radius.z + (r + p->radius.y);
            if (q->flags2 & 0x20) {
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
                    p->flags |= 0x20000000;
                }
                if (!(p->flags & 0x10000000) && dist < p->radius.x + q->radius.x) {
                    p->flags |= 0x10000000;
                }
                if (p->flags & 0x10000000) {
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
        if (!(p->flags & 0x400) || !(p->flags & 0x100000)) {
            continue;
        }
        if (!(p->flags & 0x20000000)) {
            p->flags &= ~0x10000000;
        }
        if (PSVECMag(&p->vel) >= 8.0f) {
            PSVECNormalize(&p->vel, &p->vel);
            PSVECScale(&p->vel, &p->vel, 8.0f);
        }
    }
}


void puniSound(void)
{
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
    for (i = 1; i <= 102; i++)
    {
        p = &Punidat[i];
        if ((p->flags & 0x400) && (((p->anim) == 1) || ((p->anim) == 2)))
        {
            if (i == 3)
            {
                special++;
            }
            else
            {
                k++;
            }
        }
    }

    Punigp->puniMoveCount = k;
    if (k == 0)
    {
        Punigp->puniMoveLevel = 0;
        div = 5;
    }
    else
        if (k <= 2)
    {
        Punigp->puniMoveLevel = 1;
        div = 5;
    }
    else
        if (k <= 11)
    {
        Punigp->puniMoveLevel = 2;
        div = 4;
    }
    else
        if (k <= 30)
    {
        Punigp->puniMoveLevel = 3;
        div = 3;
    }
    else
        if (k <= 50)
    {
        Punigp->puniMoveLevel = 4;
        div = 2;
    }
    else
        if (k <= 70)
    {
        Punigp->puniMoveLevel = 5;
        div = 2;
    }
    else
    {
        Punigp->puniMoveLevel = 6;
        div = 2;
    }
    if (((Punigp->puniMoveLevel) != 0) && ((gp->retraceCount % div) == 0))
    {
        m = Punigp->puniMoveCount;
        n = (rand() % m) + 1;
        k = 0;
        for (i = 1; i <= 102; i++)
        {
            p = &Punidat[i];
            if ((p->flags & 0x400) && (((p->anim) == 1) || ((p->anim) == 2)))
            {
                k++;
                if (k == n)
                {
                    break;
                }
            }
        }

        pos = p->pos;
        m = Punigp->puniMoveLevel;
        switch ((rand() % m) + 1)
        {
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
    if (special != 0)
    {
        if (((u32) ((s32)Punigp->sfxTogetti)) == 0)
        {
            Punigp->sfxTogetti = psndSFXOn_3D("SFX_STG2_TOGETTI_MOVE1", &d[3].pos);
        }
    }
    else
        if (((u32) ((s32)Punigp->sfxTogetti)) != 0)
    {
        psndSFXOff((s32)Punigp->sfxTogetti);
        Punigp->sfxTogetti = 0;
    }
    if (((u32) ((s32)Punigp->sfxTogetti)) != 0)
    {
        psndSFX_pos((s32)Punigp->sfxTogetti, &d[3].pos);
    }
    k = 0;
    for (i = 110; i < 210; i++)
    {
        if ((Punidat[i].flags & 0x400) && ((Punidat[i].anim == 1) || (Punidat[i].anim == 2)))
        {
            k++;
        }
    }

    Punigp->togeMoveCount = k;
    if (k == 0)
    {
        Punigp->togeMoveLevel = 0;
    }
    else
        if (k <= 2)
    {
        Punigp->togeMoveLevel = 1;
    }
    else
        if (k <= 11)
    {
        Punigp->togeMoveLevel = 2;
    }
    else
    {
        Punigp->togeMoveLevel = 3;
    }
    if ((Punigp->togeMoveLevel) != (Punigp->togeMoveLevelPrev))
    {
        if (((u32) ((s32)Punigp->sfx468)) != 0)
        {
            psndSFXOff((s32)Punigp->sfx468);
        }
        switch (Punigp->togeMoveLevel)
        {
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


f32 cloudGetBreathPower(Vec pos, f32 r);
extern double fabs(double);

void puni_puni_main(void) {
    PuniTarget* tg; s32 i, k, j;
    MarioWork* mario;
    PuniData* p;
    PuniData* t;
    PunidrvPose* pose;
    Vec diff;
    Vec v;

    mario = marioGetPtr();
    t = Punigp->marioPuni;
    t->pos = mario->position;
    puniMapFindbuff(t->pos);
    puniModeALLCheck();
    if (Punigp->flags & 0x20) {
        
        for (i = 1, p = Punidat + 1; i < 211; i++, p++) {
            PSVECSubtract(&p->pos, &p->prevPos, &diff);
            p->moveDist = PSVECMag(&diff);
            if (!(p->flags & 0x400)) continue;
            p->flags &= ~0x6000000;
            p->flags2 &= ~4;
            p->attacked = 0;
            puniParaRecover(p);
        }
    }
    if (Punigp->flags & 0x20) {
        for (k = 0; k < 2; k++) {
            
            for (i = 1, p = Punidat + 1; i < 211; i++, p++) {
                if (!(p->flags & 0x400)) continue;
                if (k == 0) {
                    if (p->mode != 7) continue;
                    p->flags |= 0x2000000;
                } else {
                    if (p->flags & 0x2000000) continue;
                    p->flags |= 0x2000000;
                }
                if (!(p->flags & 0x4000000)) {

                    p->flags |= 0x4000000;
                    if (p->flags2 & 0x10) {
                        p->flags2 &= ~0x10;
                        p->mode = 12;
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
                        if (tg->flags & 1) {
                            if (!(32.0 < __fabs(p->pos.y - tg->pos.y))) {
                                if (tg->flags & 4) {
                                    f32 d = PSVECSquareDistance(&p->pos, &tg->pos);
                                    if (p->awayTargetDistSq > d) {
                                        p->awayTarget = j;
                                        p->awayTargetDistSq = d;
                                    }
                                }
                                if (tg->flags & 2) {
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
                puniModeMain(p);
            }
        }
        puniOption();
    } else {
        
        for (i = 1, p = Punidat + 1; i <= 109; i++, p++) {
            if (p->flags & 0x400) {
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
                                }
                            }
                        }
                    }
                }
                puniModeMain(p);
            }
        }
        puniCollideAll();
    }
    if (Punigp->flags & 0x20) {
        s32 ii;
        PuniData* q;
        PunidrvPose* ps;
        ps = puniGetPosePtr();
        
        for (ii = 1, q = Punidat + 1; ii < 211; ii++, ps++, q++) {
            if (q->flags & 0x400) {
                if (!(q->flags & 0x200)) {
                    q->prevPos = q->pos;
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
            if (q->flags & 0x400) {
                if (!(q->flags & 0x200)) {
                    q->prevPos = q->pos;
                    puniMoveMain(q);
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

void puni_puni_dispent(void) {
    PunidrvPose* pose;
    s32 i;
    Vec v;
    Vec tmp;

    dispEntry(CAMERA_3D, 0.0f, 1, puni_puni_disp, NULL);
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
        dispEntry(CAMERA_3D, dispCalcZ(&v), 2, puni_puni_shadow_disp, pose);
        pose++;
    }
    dispEntry(CAMERA_3D, 0.0f, 1, evt_icon_disp_func, NULL);
}

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
s32 evt_puni_init(EventEntry *event, BOOL isFirstCall)
{
    EvtWord *args = event->args;
    s32 a = evtGetValue(event, args[0]);
    PuniTexInfo *tex = Punitex;
    EvtWord b = evtGetValue(event, args[1]);
    EvtWord c = evtGetValue(event, args[2]);
    EvtWord d = evtGetValue(event, args[3]);
    if (isFirstCall)
    {
        puniInit();
        return 0;
    }
    puniAnimationInit_inl((s16 **) puni_patern_table, (s16 **) toge_patern_table);
    puniAnimationInit2();
    puniEntry(a, (PunidrvTexInfo *) tex, (void *) b, (void *) c, (void *) d);
    return 2;
}

USER_FUNC(evt_puni_set_yuka) {
    EvtWord* args = event->args;
    Punigp->floor = evtGetValue(event, args[0]) + 1;
    Punigp->posY[0] = evtGetValue(event, args[1]);
    Punigp->posY[1] = evtGetValue(event, args[2]);
    Punigp->posY[2] = evtGetValue(event, args[3]);
    Punigp->posY[3] = evtGetValue(event, args[4]);
    Punigp->ymode = evtGetValue(event, args[5]);
    Punigp->mapKind = evtGetValue(event, args[6]);
    swByteSet(0x299, Punigp->floor);
    return EVT_RETURN_DONE;
}

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

#define puniMapDataSetI(w_, h_, src_, x_, z_)            \
    do {                                                 \
        s32 w = (w_);                                    \
        s32 h = (h_);                                    \
        Punimap->x = (x_);                               \
        Punimap->z = (z_);                               \
        if ((src_) == NULL) {                            \
            Punimap->enabled = 0;                        \
        } else {                                         \
            Punimap->width = w;                          \
            Punimap->height = h;                         \
            Punimap->enabled = 1;                        \
            Punimap->src = (src_);                       \
            Punimap->inRange = 0;                        \
            Punimap->size = h * (w * 4);                 \
            Punimap->data = _mapAlloc(Punimap->size);    \
            memcpy(Punimap->src, (src_), Punimap->size); \
        }                                                \
    } while (0)

USER_FUNC(evt_puni_set_maze) {
    switch (Punigp->mapKind) {
        case 0:
            break;
        case 1:
            puniMapDataSetI(13, 5, puniMapdata_MRI_06, 50.0f, -1.0f);
            break;
        case 2:
            if (swGet(0xB22)) {
                puniMapDataSetI(7, 6, puniMapdata_MRI_09_01, 150.0f, -1.0f);
            } else {
                puniMapDataSetI(7, 6, puniMapdata_MRI_09, 150.0f, -1.0f);
            }
            break;
        case 3:
            if (swGet(0xB33)) {
                puniMapDataSetI(11, 9, puniMapdata_MRI_16, -269.0f, -320.0f);
            }
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_main) {
    puniMain();
    return EVT_RETURN_BLOCK;
}

f32 puni_init_posy(f32 y) {
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

void puni_init_para(PuniData* p, f32 x, f32 y, f32 z) {
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
    p->flags |= 0x1401;
}

void toge_init_para(PuniData* p, f32 x, f32 y, f32 z) {
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
    p->flags |= 0x101402;
}

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

USER_FUNC(evt_puni_get_systemmode) {
    evtSetValue(event, event->args[0], Punigp->systemMode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_set_systemmode) {
    Punigp->systemMode = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_wait_systemmode) {
    s32 mode = evtGetValue(event, event->args[0]);
    return Punigp->systemMode != mode ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

static inline void puni_swget_idx(s32 idx, s32* val, Vec* pos) {
    s32 off;
    u8 b6, b5, b4, b3, b2, b1, b0;
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
    b6 = _swByteGet(off + 6);

    *val = b0;
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

s32 evt_puni_debug_info(EventEntry *event, BOOL isFirstCall)
{
    char buf[256];
    s32 i;
    for (i = 1; i <= 102; i++)
    {
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
        if (i >= 103)
        {
            a = 0;
        }
        else
        {
            a = 8;
            trap = swByteGet((i / 8) + 0x28C);
            a = (trap >> (i % a)) & 1;
        }
        trap = 0;
        if (i == 1)
        {
            if (!swGet(0xB3C))
            {
                trap = 1;
            }
        }
        else
            if (i == 2)
        {
            if (!swGet(0xB3D))
            {
                trap = 1;
            }
        }
        else
            if (i == 4)
        {
            if (!swGet(0xB3F))
            {
                trap = 1;
            }
        }
        else
            if (i == 3)
        {
            if (!swGet(0xB3E))
            {
                trap = 1;
            }
        }
        else
            if ((swByteGet(0x28B) + 5) <= i)
        {
            trap = 1;
        }
        if (a == 0)
        {
            if (trap == 0)
            {
                if (b0 == 0)
                {
                    sprintf(buf, "No%03d,ついて来ています\n", i);
                }
                else
                {
                    sprintf(buf, "No%03d,はぐれています area = mri_%02d, x = %04d, y = %04d, z = %04d,\n", i, b0 - 1, (s32) fx, (s32) fy, (s32) fz);
                }
            }
            else
            {
                sprintf(buf, "No%03d,＜まだ仲間になっていません＞\n", i);
            }
        }
        else
        {
            sprintf(buf, "No%03d,＜トラップにかかっています＞\n", i);
        }
        if ((i == 1) && swGet(0xB14))
        {
            sprintf(buf, "No%03d,＜mri_03の牢屋に滞在しています＞\n", i);
        }
    }

    return 2;
}

USER_FUNC(evt_puni_setup) {
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
        Punigp->flags &= ~0xF;
        switch (a1) {
            case 6:
                Punigp->flags |= 1;
                break;
            case 2:
                Punigp->flags |= 2;
                break;
            case 8:
                Punigp->flags |= 4;
                break;
            case 9:
                Punigp->flags |= 8;
                break;
        }
    }
    if (Punigp->floor == 9 || Punigp->floor == 12) {
        Punigp->flags |= 0x20;
    }
    if (swByteGet(0x299) == 1) {
        return EVT_RETURN_DONE;
    }
    p = &Punidat[1];
    for (i = 1; i <= 109; i++, p++) {
        s32 b0;
        Vec pos;
        f32 fx, fy, fz;
        s32 a;
        f32 sx, sy, sz;
        if (p->flags & 0x40000000) {
            p->flags &= ~0x40000000;
            continue;
        }
        if (i >= 103) {
            p->flags2 |= 0x20;
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
            if (!swGet(0xB3C)) continue;
            if (swGet(0xB14)) continue;
            Punigp->reserved_364 = p;
            p->flags |= 0x20;
        } else if (i == 2) {
            if (!swGet(0xB3D)) continue;
            Punigp->reserved_368 = p;
            p->flags |= 0x40;
        } else if (i == 4) {
            if (!swGet(0xB3F)) continue;
            Punigp->reserved_370 = p;
            p->flags |= 0x100;
        } else if (i == 3) {
            if (!swGet(0xB3E)) continue;
            Punigp->reserved_36C = p;
            p->flags |= 0x80;
        } else {
            if (swByteGet(0x28B) + 5 <= i) continue;
            p->flags |= 8;
        }
        if (b0 == 0) {
            if (gp->flags & 1) {
                sx = randf(mario->position.x - 10.0f, mario->position.x + 10.0f);
                sy = mario->position.y;
                sz = randf(mario->position.z - 10.0f, mario->position.z + 10.0f);
                p->mode = 1;
            } else {
                f32 ex, ey, ez;
                f32 d, e;
                sx = bero_get_BeroSX();
                sy = puni_init_posy(bero_get_BeroSY());
                sz = bero_get_BeroSZ();
                ex = bero_get_BeroEX();
                ey = puni_init_posy(bero_get_BeroEY());
                ez = bero_get_BeroEZ();
                if (!(Punigp->flags & 8)) {
                    sx = randf(sx - 10.0f, sx + 10.0f);
                    sz = randf(sz - 10.0f, sz + 10.0f);
                } else {
                    ez += 60.0f;
                }
                p->moveStart.x = randf(ex - 10.0f, ex + 10.0f);
                p->moveStart.y = ey;
                p->moveStart.z = randf(ez - 10.0f, ez + 10.0f);
                p->mode = 25;
                p->flags &= ~0x1000;
            }
        } else {
            sx = fx;
            sy = fy;
            sz = fz;
            p->mode = 15;
            p->flags |= 0x8000;
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
        p->flags |= 0x1401;
    }
    bero = bero_get_ptr();
    n = bero_get_BeroNUM();
    {
    s32 j;
    for (j = 0; j < n; j++) {
        if (bero[j]->directionId == 9) {
            PuniData* q = Punidat + j + 103;
            q->flags |= 0x100400;
            q->prevMode = 28;
            q->mode = 28;
            q->radius.x = 25.0f;
            hitObjGetPos(bero[j]->hitName, &q->pos);
            q->pos.y = puni_init_posy(q->pos.y);
        }
    }
    }
    return EVT_RETURN_DONE;
}

s32 evt_toge_setup(EventEntry *event, BOOL isFirstCall)
{
    EvtWord *args = event->args;
    s32 end = 1;
    s32 kind = evtGetValue(event, args[0]);
    f32 x = evtGetValue(event, args[1]);
    f32 y = evtGetValue(event, args[2]);
    f32 z = evtGetValue(event, args[3]);
    PuniData *p;
    switch (kind)
    {
        case 0:
            return 2;

        case 1:
            end = 210;
            break;

        case 2:
            end = 120;
            break;

    }

    for (kind = 110; kind < end; kind++)
    {
        f32 rx;
        f32 rz;
        p = &Punidat[kind];
        rx = randf(x - 50.0f, x + 50.0f);
        rz = randf(z - 120.0f, z + 120.0f);
        p->flags |= 0x10;
        p->mode = 0;
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
        p->attacked = kind * 0;
        p->hitRate = 50;
        p->hitRateGuard = 70;
        p->flags |= 0x101402;
    }

    return 2;
}

BOOL _puniVAtari(PuniData* p, Vec* from, Vec* to) {
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
#define HAGURE_FLOOR(dst, v) do { (dst) = (s32)(v); if ((v) < 0.0f) { (dst)--; } } while (0)
void puniHagurePos(PuniData* p) {
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
        case 17:
        case 18:
        case 19:
        case 21:
        case 22:
            {
                Vec v_ = {0.0f, -1000.0f, 0.0f};
                p->pos = v_;
            }
            return;
        default:
            break;
    }
    cur = p->pos;
    HAGURE_FLOOR(ix, cur.x);
    ix1 = ix + 1;
    HAGURE_FLOOR(iz, cur.z);
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

static const Vec exit_zero0 = {0.0f, 0.0f, 0.0f};
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
#pragma push
#pragma opt_dead_assignments off
static const Vec puni_fall_dir;
USER_FUNC(evt_puni_exit) {
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
        Punigp->flags &= ~0xF;
        switch (i) {
            case 6:
                Punigp->flags |= 1;
                break;
            case 2:
                Punigp->flags |= 2;
                break;
            case 8:
                Punigp->flags |= 4;
                break;
            case 9:
                Punigp->flags |= 8;
                break;
        }
    }
    if (map == NULL) {
        Punigp->flags |= 0x10;
    } else {
        Punigp->flags &= ~0x10;
    }
    if (!(Punigp->flags & 8)) {
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
    for (i = 1, p = &Punidat[1]; i <= 102; i++, p++) {
        if (!(p->flags & 0x400) || (p->flags & 0x800000)) {
            continue;
        }
        if (p->flags & 0x8000) {
            if (map == NULL) {
                p->flags |= 0x40000000;
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
                p->mode = 14;
                p->modeStep = 0;
                p->anim = 0;
            } else {
                p->mode = 26;
                p->modeStep = 0;
                p->anim = 2;
            }
        }
    }
    if (map != NULL && strcmp(map, "mri_00") == 0) {
        for (j = 1; j <= 102; j++) {
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
        if (chk != 0 && swGet(0xB3C) == 0) {
            return EVT_RETURN_DONE;
        }
        if (chk != 0 && swGet(0xB14) != 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->reserved_364 = p;
        p->flags |= 0x20;
    } else if (idx == 2) {
        if (chk != 0 && swGet(0xB3D) == 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->reserved_368 = p;
        p->flags |= 0x40;
    } else if (idx == 4) {
        if (chk != 0 && swGet(0xB3F) == 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->reserved_370 = p;
        p->flags |= 0x100;
    } else if (idx == 3) {
        if (chk != 0 && swGet(0xB3E) == 0) {
            return EVT_RETURN_DONE;
        }
        Punigp->reserved_36C = p;
        p->flags |= 0x80;
    } else {
        if (chk != 0 && swByteGet(0x28B) + 5 <= idx) {
            return EVT_RETURN_DONE;
        }
        p->flags |= 8;
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
    p->flags |= 0x1401;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_init_teki) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    PuniData* p = &Punidat[idx];
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);

    p->flags |= 0x10;
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
    p->flags |= 0x101402;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_get_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], (s32)p->pos.x);
    evtSetValue(event, args[2], (s32)p->pos.y);
    evtSetValue(event, args[3], (s32)p->pos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_set_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    p->pos.x = evtGetValue(event, args[1]);
    p->pos.y = evtGetValue(event, args[2]);
    p->pos.z = evtGetValue(event, args[3]);
    p->prevPos = p->pos;
    return EVT_RETURN_DONE;
}



USER_FUNC(evt_puni_get_dir) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], (s32)Punidat[evtGetValue(event, args[0])].dir);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_set_dir) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    p->dir = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_get_anim) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], Punidat[evtGetValue(event, args[0])].anim);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_set_anim) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    p->anim = evtGetValue(event, args[1]);
    p->prevAnim = 1000;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_korokoro) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];

    switch (p->anim) {
        case 5:
            if (isFirstCall) {
                if (p->flags & 8) {
                    p->rotOffsetY = 8.0f;
                } else {
                    p->dispOfs.y = 8.0f;
                }
            }
            p->spin += evtGetValue(event, args[1]);
            return EVT_RETURN_BLOCK;
        default:
            if (p->flags & 8) {
                p->rotOffsetY = 0.0f;
            } else {
                p->dispOfs.y = 0.0f;
            }
            p->spin = evtGetValue(event, args[2]);
            return EVT_RETURN_DONE;
    }
}

USER_FUNC(evt_puni_move_position) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    f32 x = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    s32 frames = evtGetValue(event, args[3]);
    u32 t;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        p->moveStart = p->pos;
        p->moveEnd.x = x;
        p->moveEnd.y = p->pos.y;
        p->moveEnd.z = z;
    }
    if ((s32)(t = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime)) < frames) {
        p->pos.x = intplGetValue(0, p->moveStart.x, p->moveEnd.x, t, frames);
        p->pos.z = intplGetValue(0, p->moveStart.z, p->moveEnd.z, t, frames);
        p->prevPos = p->pos;
        return EVT_RETURN_BLOCK;
    }
    p->pos = p->moveEnd;
    p->prevPos = p->pos;
    return EVT_RETURN_DONE;
}

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
        r = (f32)t / (f32)frames;
        p->pos.x = intplGetValue(0, p->moveStart.x, x, (s32)t, frames);
        p->pos.z = intplGetValue(0, p->moveStart.z, z, (s32)t, frames);
        p->pos.y = r * (p->modeF0 * r + p->modeF1) + p->moveStart.y;
        return EVT_RETURN_BLOCK;
    }
    p->pos.x = x;
    p->pos.y = y;
    p->pos.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_get_mode) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], Punidat[evtGetValue(event, args[0])].mode);
    return EVT_RETURN_DONE;
}

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

USER_FUNC(evt_puni_setsudan) {
    PuniData* p = &Punidat[evtGetValue(event, event->args[0])];
    if (p->flags & 0x400) {
        PuniDokuritsu(p);
        puniDeleteEffect(p);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_get_flag) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], Punidat[evtGetValue(event, args[0])].flags);
    return EVT_RETURN_DONE;
}

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

USER_FUNC(evt_puni_dokan_onoff) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    PuniData* p = &Punidat[puni_bero_id_filter(id)] + 103;
    if (on == 0) {
        p->flags |= 0x100000;
    } else {
        p->flags &= ~0x100000;
    }
    return EVT_RETURN_DONE;
}

static inline void puniswGetindex_inl(s32 base, s32 idx, s32* val, Vec* pos) {
    u8 b6, b5, b4, b3, b2, b1, b0;
    s32 off;
    s32 x;
    s32 y;
    s32 z;

    off = base + idx * 7;
    b0 = _swByteGet(off + EVTDAT_LSW_BASE);
    b1 = _swByteGet(off + 150000001);
    b2 = _swByteGet(off + 150000002);
    b3 = _swByteGet(off + 150000003);
    b4 = _swByteGet(off + 150000004);
    b5 = _swByteGet(off + 150000005);
    b6 = _swByteGet(off + 150000006);

    *val = b0;
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

USER_FUNC(evt_puni_get_lsw_index) {
    EvtWord* args = event->args;
    s32 base = args[0];
    s32 idx = evtGetValue(event, args[1]);
    s32 val;
    Vec pos;
    base += idx * 7;
    puniswGetindex_inl(base, 0, &val, &pos);
    evtSetValue(event, args[2], val);
    evtSetValue(event, args[3], (s32)pos.x);
    evtSetValue(event, args[4], (s32)pos.y);
    evtSetValue(event, args[5], (s32)pos.z);
    return EVT_RETURN_DONE;
}

#pragma push
#pragma opt_dead_assignments off
static const Vec puni_fall_dir;
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
    off = base + idx * 7;
    puniswSetindex(off, 0, &val, &pos);
    return EVT_RETURN_DONE;
}
#pragma pop

USER_FUNC(evt_puni_get_trap_index) {
    EvtWord* args = event->args;
    s32 v;
    puniswTrapGetindex(evtGetValue(event, args[0]), &v);
    evtSetValue(event, args[1], v);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_set_trap_index) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 val = evtGetValue(event, args[1]);
    puniswTrapSetindex(idx, &val);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_set_ymode) {
    Punigp->ymode = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_all_fall_check) {
    puniModeChangeFallCheck();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_all_movestop) {
    s32 stop = evtGetValue(event, event->args[0]);
    s32 i;
    for (i = 1; i < 103; i++) {
        PuniData* p = &Punidat[i];
        if (p->flags & 0x400) {
            if (stop == 0) {
                p->flags &= ~0x200;
            } else {
                p->flags |= 0x200;
            }
        }
    }
    return EVT_RETURN_DONE;
}

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
        if ((Punidat[i].flags & 0x400) && !(Punidat[i].flags & 0x8000)) {
            count++;
        }
    }
    evtSetValue(event, args[1], count);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_count_area) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    f32 r = evtGetValue(event, args[3]);
    s32 count;
    s32 i;
    for (i = 1, count = 0; i < 103; i++) {
        PuniData* p = &Punidat[i];
        if ((p->flags & 0x400) && p->mode != 16) {
            if (!(10.0 < __fabs(y - p->pos.y)) && !(r < __fabs(x - p->pos.x)) && !(r < __fabs(z - p->pos.z))) {
                count++;
            }
        }
    }
    evtSetValue(event, args[4], count);
    return EVT_RETURN_DONE;
}

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
        if ((Punidat[i].flags & 0x400) && !(Punidat[i].flags & 0x4000)) {
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
        if ((p->flags & 0x400) && p->pos.y > y) {
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

USER_FUNC(evt_puni_pera_onoff) {
    EvtWord* args = event->args;
    PuniData* p = &Punidat[evtGetValue(event, args[0])];
    if (evtGetValue(event, args[1])) {
        p->npc->flags &= ~2;
    } else {
        p->npc->flags |= 2;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_fade_onoff) {
    puniFadeOff(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_attack_on) {
    Punigp->allMode = 3;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_puninum_onoff) {
    Punigp->showPuniCounter = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_puninum_alpha) {
    Punigp->puniCounterAlpha = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_winddisable_onoff) {
    Punigp->windDisabled = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_puni_force_se_off) {
    if (Punigp->sfx458) {
        psndSFXOff(Punigp->sfx458);
    }
    if (Punigp->sfx468) {
        psndSFXOff(Punigp->sfx468);
    }
    return EVT_RETURN_DONE;
}

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
USER_FUNC(evt_puni_all_save) {
    s32 off;
    s32 i;
    PuniData* p = Punidat + 1;
    Vec pos;
    for (i = 1; i <= 102; i++, p++) {
        if ((p->flags & 0x400) && !(p->flags & 0x800000)) {
            if (p->flags & 0x8000) {
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

USER_FUNC(evt_target_set_position) {
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);
    PuniTarget* t = &Punigp->targets[i];
    t->pos.x = evtGetValue(event, args[1]);
    t->pos.y = evtGetValue(event, args[2]);
    t->pos.z = evtGetValue(event, args[3]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_target_set_soko) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    PuniTarget* t = &Punigp->targets[idx];
    t->hit = hitNameToPtr((const char*)evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

const char* npcGetTribeName(const char** name) {
    return *name;
}

#define PIDERS_Y(n)                              \
    if (50.0 >= __fabs(v.y - Punigp->posY[n])) { \
        found = TRUE;                            \
        v.y = Punigp->posY[n];                   \
    }

static const Vec puni_fall_dir;
s32 puni_piders_check_func(EventEntry *event, BOOL isFirstCall)
{
    s32 k;
    s32 count;
    NpcEntry *npc;
    s32 i;
    NpcWork *work;
    work = npcGetWorkPtr();
    count = work->total;
    npc = work->entries;
    for (i = 2; i < 10; i++)
    {
        Punigp->targets[i].flags &= ~5;
    }
    k = 2;
    for (i = 0; i < count; i++, npc++)
    {
        if ((npc->flags & 1) && (npc->flags & 2) && npc->tribe != NULL && strcmp(npc->tribe->nameJp, "\203p\203C\203_\201[\203X") == 0)
        {
            Vec v;
            BOOL found = 0;
            v = npc->position;
            if (50.0 >= __fabs(v.y - Punigp->posY[0]))
            {
                found = 1;
                v.y = Punigp->posY[0];
            }
            if (50.0 >= __fabs(v.y - Punigp->posY[1]))
            {
                found = 1;
                v.y = Punigp->posY[1];
            }
            if (50.0 >= __fabs(v.y - Punigp->posY[2]))
            {
                found = 1;
                v.y = Punigp->posY[2];
            }
            if (50.0 >= __fabs(v.y - Punigp->posY[3]))
            {
                found = 1;
                v.y = Punigp->posY[3];
            }
            if (found)
            {
                PuniTarget *t = &Punigp->targets[k];
                t->pos = v;
                t->flags |= 5;
            }
            k++;
        }
    }
    return 2;
}

USER_FUNC(puni_disp_info_kassencheck) {
    EvtWord* args = event->args;
    BOOL ret = FALSE;
    if (Punigp->floor == 9 || Punigp->floor == 12) {
        ret = TRUE;
    }
    evtSetValue(event, args[0], ret);
    return EVT_RETURN_DONE;
}

void puniCountDisp(CameraId unused, void* param) {
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
    iconDispGxCol(m, 0x10, VERSION_PICK(0x203, 0x20A), c1);
    PSMTXTrans(m, -232.0f, -178.0f, 0.0f);
    iconDispGxCol(m, 0x10, VERSION_PICK(0x204, 0x20B), c2);
    PSMTXTrans(m, -160.0f, -178.0f, 0.0f);
    if (Punigp->showRemaining) {
        if (gp->retraceCount & 0x10) {
            iconNumberDispGx(m, num2, FALSE, c3);
        }
    } else {
        iconNumberDispGx(m, num2, FALSE, c3);
    }
    PSMTXTrans(m, -140.0f, -178.0f, 0.0f);
    iconDispGxCol(m, 0x10, VERSION_PICK(0x1D8, 0x1E0), c3);
    PSMTXTrans(m, -90.0f, -178.0f, 0.0f);
    iconNumberDispGx(m, num1, TRUE, c3);
}

USER_FUNC(puni_disp_info_sub) {
    EvtWord* args = event->args;
    s32 count = 0;
    s32 total;
    BOOL done;
    s32 i;
    EvtWord packed;
    for (i = 1; i < 103; i++) {
        if ((Punidat[i].flags & 0x400) && !(Punidat[i].flags & 0x804000) && !(Punidat[i].flags & 0x80)) {
            if (Punidat[i].mode != 16 && Punidat[i].mode != 15) {
                count++;
            }
        }
    }
    total = swGet(0xB3C);
    total += swGet(0xB3D);
    total += swGet(0xB3F);
    total += swByteGet(0x28B);
    done = FALSE;
    if (Punigp->floor == 9 && !swGet(0xB21)) {
        done = TRUE;
    }
    if (Punigp->floor == 12 && !swGet(0xB29)) {
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
        count = swByteGet(0x29B);
        total = swByteGet(0x29C);
        Punigp->showRemaining = 0;
    } else {
        swByteSet(0x29B, (u8)count);
        swByteSet(0x29C, (u8)total);
    }
    packed = (count << 16) | total;
    if (Punigp->showPuniCounter && total >= 10) {
        dispEntry((CameraId)8, 1000.0f, 0, puniCountDisp, (void*)packed);
    }
    return EVT_RETURN_DONE;
}


EVT_BEGIN(puni_piders_check)
    CALL(evt_seq_wait, 2)
    LOOP(0)
        CALL(puni_piders_check_func)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(puni_disp_info)
    IF_INT_GREATER_OR_EQUAL(4124967296, 118)
        RETURN
    END_IF
    LOOP(0)
        CALL(puni_disp_info_sub, 4264967296, 4264967297)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_puninum_fadeout)
    WAIT_MS(1200)
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_puni_puninum_alpha, 4264967296)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(4264967297, 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_puninum_fadein)
    CALL(evt_sub_intpl_msec_init, 11, 0, 255, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_puni_puninum_alpha, 4264967296)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(4264967297, 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_position_init)
    CALL(evt_puni_set_systemmode, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_puni_puninum_alpha, 0)
        WAIT_FRAMES(2)
        CALL(evt_bero_get_now_number, 4264967306)
        CALL(evt_bero_get_info_kinddir, 4264967306, 4264967296, 4264967297, 4264967298)
        CALL(evt_bero_get_info_nextarea, 4264967306, 4264967299, 4264967300)
        SET_VALUE(4224967296, 0)
        IF_INT_EQUAL(4264967296, 2)
            IF_INT_NOT_EQUAL(4264967299, 0)
                SET_VALUE(4224967296, 1)
            END_IF
        END_IF
        IF_INT_EQUAL(4224967296, 0)
            CALL(evt_puni_puninum_alpha, 255)
        END_IF
    END_INLINE_EVENT
    DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\217\211\212\372\202\314\216\300\215s\222\206\221\322\202\277"))
    CALL(evt_bero_exec_wait, 1)
    LOOP(0)
        CALL(evt_puni_set_systemmode, 1)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\215\300\225W\203Z\203b\203g\221\322\202\277"))
        CALL(evt_bero_exec_wait, 2)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\217\254\223\256\225\250\212\326\214W\217\211\212\372\211\273"))
        DEBUG_PUT_REG(4124967961)
        DEBUG_PUT_REG(4124967962)
        CALL(evt_bero_get_now_number, 4264967306)
        CALL(evt_bero_get_info_kinddir, 4264967306, 4264967296, 4264967297, 4264967298)
        CALL(evt_bero_get_info_nextarea, 4264967306, 4264967299, 4264967300)
        IF_INT_EQUAL(4264967296, 2)
            IF_INT_NOT_EQUAL(4264967299, 0)
                SPAWN_EVENT(mri_puninum_fadein)
            END_IF
        END_IF
        CALL(evt_puni_setup, 4264967296, 4264967297, 4264967298)
        CALL(evt_toge_setup, 4264967302, 4264967303, 4264967304, 4264967305)
        CALL(evt_puni_set_systemmode, 2)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\217I\227\271\221\322\202\277"))
        CALL(evt_bero_exec_wait, 65536)
        CALL(evt_puni_set_systemmode, 3)
        DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\216\237\202\314\216\300\215s\222\206\221\322\202\277"))
        CALL(evt_bero_exec_wait, 1)
        SET_VALUE(4124967962, 4124967961)
        CALL(evt_bero_get_now_number, 4264967306)
        CALL(evt_bero_get_info_kinddir, 4264967306, 4264967296, 4264967297, 4264967298)
        CALL(evt_bero_get_info_nextarea, 4264967306, 4264967299, 4264967300)
        IF_INT_EQUAL(4264967296, 1)
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\224\340\221\322\202\277"))
            CALL(evt_bero_exec_wait, 268435456)
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\224\340\221\322\202\277\217I\202\355\202\350"))
            CALL(evt_bero_exec_onoff, 1, 268435456)
        END_IF
        IF_INT_EQUAL(4264967296, 2)
            IF_INT_NOT_EQUAL(4264967299, 0)
                SPAWN_EVENT(mri_puninum_fadeout)
            END_IF
        END_IF
        BEGIN_INLINE_EVENT
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\203G\203\212\203A\203`\203F\203\223\203W\222\274\221O\221\322\202\277"))
            CALL(evt_bero_exec_wait, 8)
            DEBUG_PUT_MSG(STRING("\203v\203j\212\326\214W\201F\203v\203j\221\260\203g\203Q\221\260SE\213\255\220\247\203I\203t"))
            CALL(evt_puni_force_se_off)
        END_INLINE_EVENT
        CALL(evt_puni_exit, 4264967296, 4264967297, 4264967298, 4264967299)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_main)
    SET_TYPE(0)
    CALL(evt_puni_main)
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_init_dvd)
    CALL(evt_npc_entry, STRING("\203v\203jsp"), STRING("c_puni"))
    CALL(evt_npc_set_tribe, STRING("\203v\203jsp"), STRING("\203v\203j\221\260"))
    CALL(evt_npc_entry, STRING("\203g\203Qsp"), STRING("c_toge"))
    CALL(evt_npc_set_tribe, STRING("\203g\203Qsp"), STRING("\203g\203Q\221\260"))
    CALL(evt_npc_entry, STRING("\203K\203C\203hsp"), STRING("c_pguide"))
    CALL(evt_npc_set_tribe, STRING("\203K\203C\203hsp"), STRING("\203K\203C\203h"))
    CALL(evt_npc_entry, STRING("\203K\203C\203h\226\205sp"), STRING("c_psister"))
    CALL(evt_npc_set_tribe, STRING("\203K\203C\203h\226\205sp"), STRING("\203K\203C\203h\226\205"))
    CALL(evt_npc_entry, STRING("\202\277\202\321\203g\203Qsp"), STRING("c_tchibi"))
    CALL(evt_npc_set_tribe, STRING("\202\277\202\321\203g\203Qsp"), STRING("\202\277\202\321\203g\203Q"))
    CALL(evt_npc_entry, STRING("\203\211\203C\203o\203\213sp"), STRING("c_prival"))
    CALL(evt_npc_set_tribe, STRING("\203\211\203C\203o\203\213sp"), STRING("\203\211\203C\203o\203\213"))
    CALL(evt_npc_set_position, STRING("\203v\203jsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203g\203Qsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203K\203C\203hsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203K\203C\203h\226\205sp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\202\277\202\321\203g\203Qsp"), 0, 4294966296, 0)
    CALL(evt_npc_set_position, STRING("\203\211\203C\203o\203\213sp"), 0, 4294966296, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("\203v\203jsp"), 1074266144)
    CALL(evt_npc_flag_onoff, 1, STRING("\203g\203Qsp"), 1074266144)
    CALL(evt_npc_flag_onoff, 1, STRING("\203K\203C\203hsp"), 1074266144)
    CALL(evt_npc_flag_onoff, 1, STRING("\203K\203C\203h\226\205sp"), 1074266144)
    CALL(evt_npc_flag_onoff, 1, STRING("\202\277\202\321\203g\203Qsp"), 1074266144)
    CALL(evt_npc_flag_onoff, 1, STRING("\203\211\203C\203o\203\213sp"), 1074266144)
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_init)
    CALL_CHILD(mri_puni_init_dvd)
    SWITCH(4124967296)
        CASE_LESS(88)
        RETURN
        CASE_GREATER_OR_EQUAL(403)
        RETURN
        CASE_EQUAL(400)
        CALL(evt_sub_get_mapname, 4264967296)
        IF_STR_NOT_EQUAL(4264967296, STRING("mri_00"))
            RETURN
        END_IF
        SET_VALUE(4224967296, 0)
        CALL(evt_bero_get_entername, 4264967296)
        IF_STR_EQUAL(4264967296, STRING("sekai_yami1"))
            SET_VALUE(4224967296, 1)
        END_IF
        IF_STR_EQUAL(4264967296, STRING("minnnanokoe"))
            SET_VALUE(4224967296, 1)
        END_IF
        IF_INT_EQUAL(4224967296, 0)
            RETURN
        END_IF
        CASE_BETWEEN(118, 399)
        CALL(evt_sub_get_mapname, 4264967296)
        IF_STR_NOT_EQUAL(4264967296, STRING("mri_00"))
            RETURN
        END_IF
        IF_INT_EQUAL(4164970180, 1)
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

EVT_BEGIN(mri_puni_init_none)
    CALL_CHILD(mri_puni_init_dvd)
    CALL(evt_make_texbuf)
    CALL(evt_puni_init, 210, PTR(puni_puni_init), PTR(puni_puni_main), PTR(puni_puni_dispent))
    CALL(evt_puni_set_maze)
    RETURN
EVT_END()









