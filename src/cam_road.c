#include "system.h"
#include "driver/arcdrv.h"
#include "driver/mapdrv.h"
#include "manager/dvdmgr.h"
#include "memory.h"
#include "mario/mariost.h"
#include "cam_shift.h"
#include "libc_decls.h"
#include "cam_road.h"
#include "addr_word.h"
#include <math.h>
typedef struct CamRoadHit {
    f32 t;                 // 0x00
    f32 f_04;              // 0x04
    f32 y;                 // 0x08
    Vec pos;               // 0x0c
    Vec dir;               // 0x18
    s32 f_24;              // 0x24
    u32 flags;             // 0x28
} CamRoadHit;
double acos(double);
static const Vec unk_vec0 = {0.0f, 1.0f, 0.0f};
static const Vec unk_vec1 = {90.0f, 110.0f, 13.0f};
static const Vec unk_vec2 = {-110.0f, -70.0f, -190.0f};
static CamRoadWork work;
static CamRoadWork* wp = &work;
void OSReport(const char* msg, ...);
void calcCurrentCamState(CameraParams* out);
void calcCamRoad(CamRoadState* out, CamRoadState* prev, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g);
u32 compareCamRoad(CamRoadState* a, CamRoadState* b, f32* speed);
void calcHokanCamRoad(s32 type, CamRoadState* from, CamRoadState* to, CamRoadState* unused, CamRoadState* out, f32 t);
static inline void crCheck(BOOL b) {}


// FAKE: stand-in for a debug function the linker stripped; its two strings are the last literals in the
// target .rodata pool (codegen runs bottom-up, so it sits before the first function). The JP demo map
// (MarioSt.MAP) names the original: the strings are @2397/@2398, numbered between calcCurrentCamState and
// camRoadSetup, where the map lists the stripped camRoadDraw (0x8b8 bytes, UNUSED in the demo as well), so
// no build contains its code.
static void camRoadStrippedReport(const char* p, s32 a, s32 b) {
    OSReport("%scam = %d\n", p, a);
    OSReport("%scurve = %d\n", p, b);
}

s32 camRoadSetup(const char* name) {
    char path[128];
    u32 size;
    DVDEntry* entry;
    memset(wp, 0, 0x210);
    wp->f_c8 = 0;
    wp->prev.on = 0;
    wp->prev.on = 0;
    wp->f_19c[0] = 0.005f;
    wp->f_19c[1] = 0.0f;
    wp->f_19c[2] = 0.004f;
    wp->f_19c[3] = 0.008f;
    wp->f_19c[4] = 1.0f;
    wp->f_19c[5] = 1.0f;
    wp->f_19c[6] = 2.0f;
    wp->f_19c[7] = 1.0f;
    wp->f_19c[8] = 1.0f;
    wp->f_19c[9] = 3.0f;
    wp->f_19c[10] = 1.0f;
    wp->f_204 = -1;
    wp->f_208 = 0;
    camShiftInit();
    arcOpen(name, (void**)&wp->map, &size);
    if (!wp->map) {
        sprintf(path, "%s/%s", getMapDataDvdRoot(), name);
        entry = DVDMgrOpen(path, 2, 0);
        if (entry) {
            size = DVDMgrGetLength(entry);
            wp->map = _mapAlloc((size + 0x1f) & ~0x1f);
            DVDMgrRead(entry, wp->map, (size + 0x1f) & ~0x1f, 0);
            DVDMgrClose(entry);
        } else {
            return 0;
        }
    }
    if ((u8*)wp->map->f_e8 < (u8*)wp->map) {
        ADDR_WORD(wp->map->f_e8) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->curves) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_f0) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_f4) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_f8) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_fc) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_100) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_104) += (AddrWord)wp->map;
        ADDR_WORD(wp->map->f_108) += (AddrWord)wp->map;
    }
    if (strcmp(wp->map->version, "1.01") == 0) {
        wp->state = 0x65;
    } else {
        wp->state = 0x64;
    }
    return 1;
}
void calcCurrentCamState(CameraParams* out) {
    Mtx m;
    Vec v;
    Vec res;
    Vec axis;
    f32 rot, x, y, z, dx, dz, dist, a, ox, oy, oz;
    CamRoadState* s;
    Vec* p;
    p = wp->map->f_e8;
    out->fovY = p->x;
    out->aspect = p->y;
    out->far = p->z;
    s = &wp->cur;
    if (!s->on) {
        out->pos.x = 0.0f;
        out->pos.y = 100.0f;
        out->pos.z = 100.0f;
        out->at.x = 0.0f;
        out->at.y = 0.0f;
        out->at.z = 0.0f;
        return;
    }
    a = 3.141592f * s->angle / 180.0f;
    x = s->pos.x;
    y = s->pos.y;
    z = s->pos.z;
    dx = s->dir.x;
    dz = s->dir.z;
    dist = s->dist;
    rot = s->rot;
    ox = dx * dist * (f32)cos(a);
    oy = dist * (f32)sin(a);
    oz = dz * dist * (f32)cos(a);
    out->pos.x = x + ox;
    out->pos.y = y + oy;
    out->pos.z = z + oz;
    axis.x = -dz;
    axis.y = 0.0f;
    axis.z = dx;
    PSMTXRotAxisRad(m, &axis, 0.017453292f * rot);
    v.x = -ox;
    v.y = -oy;
    v.z = -oz;
    PSMTXMultVec(m, &v, &res);
    out->at.x = out->pos.x + res.x;
    out->at.y = out->pos.y + res.y;
    out->at.z = out->pos.z + res.z;
}
static inline Vec crVecSub(Vec a, Vec b) {
    Vec r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}
static inline Vec crVecAdd(Vec a, Vec b) {
    Vec r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
static inline Vec crVecScale(Vec a, f32 s) {
    Vec r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    return r;
}
static inline Vec crVecCross(Vec a, Vec b) {
    Vec r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
static inline f32 crVecDot(Vec a, Vec b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static inline BOOL crVecIsZero(Vec a) {
    if (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f) {
        return TRUE;
    }
    return FALSE;
}
s32 collisionTri_simple(Vec P, Vec A, Vec B, Vec C, f32* dist) {
    Vec dir = {0.0f, -1.0f, 0.0f};
    Vec nz;
    Vec hit;
    Vec e2, e1, e3, n;
    f32 nd, t;
    e2 = crVecSub(C, A);
    e1 = crVecSub(A, B);
    e3 = crVecSub(B, C);
    n = crVecCross(e2, e1);
    nz = n;
    if (nz.x == 0.0f && nz.y == 0.0f && nz.z == 0.0f) {
        return 0;
    }
    nd = crVecDot(n, crVecSub(P, A));
    if (nd > 0.0f) {
        if (crVecDot(n, dir) >= 0.0) {
            return 0;
        }
        if (crVecDot(crVecSub(P, A), crVecCross(e2, dir)) > 0.0) {
            return 0;
        }
        if (crVecDot(crVecSub(P, B), crVecCross(e1, dir)) > 0.0) {
            return 0;
        }
        if (crVecDot(crVecSub(P, C), crVecCross(e3, dir)) > 0.0) {
            return 0;
        }
    } else {
        if (crVecDot(n, dir) <= 0.0) {
            return 0;
        }
        if (crVecDot(crVecSub(P, A), crVecCross(e2, dir)) < 0.0) {
            return 0;
        }
        if (crVecDot(crVecSub(P, B), crVecCross(e1, dir)) < 0.0) {
            return 0;
        }
        if (crVecDot(crVecSub(P, C), crVecCross(e3, dir)) < 0.0) {
            return 0;
        }
    }
    t = -nd / crVecDot(n, dir);
    { Vec h2 = crVecAdd(P, crVecScale(dir, t)); hit = h2; }
    if (hit.y < *dist) {
        return 0;
    }
    *dist = hit.y;
    return 1;
}
// CC_BEGIN
 static inline f32 crLen2(f32 x, f32 z) {
    f32 l = x * x + z * z;
    if (l != 0.0) {
        l = sqrtf(l);
    }
    return l;
}
s32 collisionCurve(s32 mode, void* curve, Vec pos, CamRoadHit* hit) {
    f32 lx0;
    f32 ang0, yA, yB, minDist, cx, side;
    f32 dpx, dpz, sgn, s, limA, limB, x0, y0, z0, x1, z1, cz, r0, r1, rp, r0x, r0z, c1;
    f32 u0x, u0z, u1x, u1z, t, c0, ang1, ratio, d, d0, yy, cross, n1x, n1z;
    f32 r1x, r1z;
    f32 lz0, px, pz, qx, qz, dx, dz, su1x, st, dist, dx2, dz2, cr;
    s32 ret;
    s32 i;
    s32 half;
    s32 idxA, idxB;
    s32 flag, isGrp;
    s32 fl;
    s32 ok;
    Vec* pt;
    Vec* nrm;
    minDist = 100000000.0f;
    ret = 0;
    if (mode == 0) {
        CamRoadCurve* c = (CamRoadCurve*)curve;
        isGrp = 0;
        half = c->vtxNum / 2;
        pt = &wp->map->f_f8[c->vtxOfs];
        nrm = &wp->map->f_f8[c->vtxOfs + half];
        limA = c->limA;
        limB = c->limB;
        idxA = c->idxA;
        idxB = c->idxB;
        flag = c->flag;
    } else {
        CamRoadGroup* g = (CamRoadGroup*)curve;
        idxA = 0;
        idxB = 1;
        half = g->stripNum / 2;
        limB = limA = 0.0f;
        pt = &wp->map->f_f8[g->stripOfs];
        nrm = &wp->map->f_f8[g->stripOfs + half];
        flag = 0;
        isGrp = 1;
    }
    yA = nrm[idxA].y;
    yB = nrm[idxB].y;
    if (__fabs(yB - yA) < 0.0001f) {
        yA = yB;
    }
    if (isGrp) {
        flag = 0;
    }
    for (i = 0; i < half - 1; i++) {
        fl = 0;
        x0 = pt[i].x;
        y0 = pt[i].y;
        z0 = pt[i].z;
        x1 = pt[i + 1].x;
        z1 = pt[i + 1].z;
        n1x = -nrm[i + 1].x;
        n1z = -nrm[i + 1].z;
        if (nrm[i].x * (pos.x - x0) + nrm[i].z * (pos.z - z0) < 0.0f) {
            continue;
        }
        if (n1x * (pos.x - x1) + n1z * (pos.z - z1) < 0.0f) {
            continue;
        }
        u0x = -nrm[i].z;
        u0z = nrm[i].x;
        u1x = n1z;
        u1z = -n1x;
        if (__fabs(u0x - u1x) < 0.01f && __fabs(u0z - u1z) < 0.01f) {
            u0x = u1x = 0.5f * (u0x + u1x);
            u0z = u1z = 0.5f * (u0z + u1z);
        }
        if (u0x != u1x || u0z != u1z) {
            side = sgn = 1.0f;
            if (u1x != 0.0) {
                t = (u1z * ((x0 - x1) / u1x) + z1 - z0) / (u0z - u1z * (u0x / u1x));
            } else if (u1z != 0.0) {
                t = (u1x * ((z0 - z1) / u1z) + x1 - x0) / (u0x - u1x * (u0z / u1z));
            } else {
                t = 0.0f;
            }
            cz = t * u0z + z0;
            cx = t * u0x + x0;
            r0z = z0 - cz;
            r0x = x0 - cx;
            r0 = r0x * r0x + r0z * r0z;
            if (r0 != 0.0) {
                r0 = sqrtf(r0);
            }
            dpz = pos.z - cz;
            dpx = pos.x - cx;
            rp = dpx * dpx + dpz * dpz;
            if (rp != 0.0) {
                rp = sqrtf(rp);
            }
            r1z = z1 - cz;
            r1x = x1 - cx;
            r1 = r1x * r1x + r1z * r1z;
            if (r1 != 0.0) {
                r1 = sqrtf(r1);
            }
            c0 = (r0x * dpx + r0z * dpz) / (r0 * rp);
            c1 = (r1x * dpx + r1z * dpz) / (r1 * rp);
            if (c0 < -1.0f) {
                c0 = -1.0f;
            } else if (c0 > 1.0f) {
                c0 = 1.0f;
            }
            if (c1 < -1.0f) {
                c1 = -1.0f;
            } else if (c1 > 1.0f) {
                c1 = 1.0f;
            }
            ang0 = acos(c0);
            ang1 = acos(c1);
            d0 = rp - r0;
            ratio = ang0 / (ang0 + ang1);
            d = ratio * ((rp - r1) - d0) + d0;
            if (d < 0.0f) {
                sgn = -1.0f;
                d = -d;
            }
            if (d >= minDist) {
                continue;
            }
            minDist = d;
            cross = r0x * -(z1 - z0) + r0z * (x1 - x0);
            yy = ratio * (nrm[i + 1].y - nrm[i].y) + nrm[i].y;
            if (flag) {
                if (yy < yA) {
                    if (cross > 0.0) {
                        if (sgn < 0.0f) {
                            if (d > limA) { d = limA; fl |= 2; }
                        } else {
                            if (d > limB) { d = limB; fl |= 2; }
                            d = -d;
                        }
                    } else {
                        if (sgn < 0.0f) {
                            if (d > limB) { d = limB; fl |= 2; }
                            d = -d;
                        } else {
                            if (d > limA) { d = limA; fl |= 2; }
                        }
                    }
                    ret = 1;
                    hit->t = yA;
                    hit->pos.x = pt[idxA].x + d * nrm[idxA].z;
                    hit->pos.y = 0.0f;
                    hit->pos.z = pt[idxA].z - d * nrm[idxA].x;
                    hit->dir.x = nrm[idxA].x;
                    hit->dir.y = 0.0f;
                    hit->dir.z = nrm[idxA].z;
                    hit->y = y0;
                    hit->f_24 = idxA;
                    hit->flags = fl | 1;
                    continue;
                }
                if (yy > yB) {
                    if (cross > 0.0) {
                        if (sgn < 0.0f) {
                            if (d > limA) { d = limA; fl |= 2; }
                        } else {
                            if (d > limB) { d = limB; fl |= 2; }
                            d = -d;
                        }
                    } else {
                        if (sgn < 0.0f) {
                            if (d > limB) { d = limB; fl |= 2; }
                            d = -d;
                        } else {
                            if (d > limA) { d = limA; fl |= 2; }
                        }
                    }
                    ret = 1;
                    hit->t = yB;
                    hit->pos.x = pt[idxB].x + d * nrm[idxB].z;
                    hit->pos.y = 0.0f;
                    hit->pos.z = pt[idxB].z - d * nrm[idxB].x;
                    hit->dir.x = nrm[idxB].x;
                    hit->dir.y = 0.0f;
                    hit->dir.z = nrm[idxB].z;
                    hit->y = y0;
                    hit->f_24 = idxB;
                    hit->flags = fl | 1;
                    continue;
                }
                if (cross > 0.0) {
                    if (sgn < 0.0f) {
                        if (d > limA) {
                            dpx /= rp; dpz /= rp; fl |= 2;
                            rp += d - limA;
                            dpx *= rp; dpz *= rp;
                        }
                    } else {
                        if (d > limB) {
                            dpx /= rp; dpz /= rp; fl |= 2;
                            rp -= d - limB;
                            dpx *= rp; dpz *= rp;
                        }
                    }
                } else {
                    if (sgn < 0.0f) {
                        if (d > limB) {
                            dpx /= rp; dpz /= rp; fl |= 2;
                            rp += d - limB;
                            dpx *= rp; dpz *= rp;
                        }
                    } else {
                        if (d > limA) {
                            dpx /= rp; dpz /= rp; fl |= 2;
                            rp -= d - limA;
                            dpx *= rp; dpz *= rp;
                        }
                    }
                    side = -1.0f;
                }
            } else if (isGrp) {
                if (cross > 0.0) {
                    if (!(sgn < 0.0f) && d > 0.0f) {
                        dpx /= rp; dpz /= rp; fl = 3;
                        rp -= d;
                        dpx *= rp; dpz *= rp;
                    }
                } else {
                    if (sgn < 0.0f && d > 0.0f) {
                        dpx /= rp; dpz /= rp; fl = 3;
                        rp += d;
                        dpx *= rp; dpz *= rp;
                    }
                    side = -1.0f;
                }
            } else {
                if (!(cross > 0.0)) {
                    side = -1.0f;
                }
            }
            hit->pos.x = cx + dpx;
            hit->pos.y = 0.0f;
            hit->pos.z = cz + dpz;
            if (side < 0.0f) {
                hit->dir.x = -dpz / rp;
                hit->dir.y = 0.0f;
                hit->dir.z = dpx / rp;
            } else {
                hit->dir.x = dpz / rp;
                hit->dir.y = 0.0f;
                hit->dir.z = -dpx / rp;
            }
            hit->t = yy;
            ret = 1;
            hit->y = y0;
            hit->f_24 = i;
            hit->flags = fl;
        } else {
            lz0 = pt[i].z;
            dz = pt[i + 1].z - lz0;
            lx0 = pt[i].x;
            dx = pt[i + 1].x - lx0;
            px = pos.x;
            pz = pos.z;
            su1x = -dz;
            if (__fabs(su1x) > __fabs(dx)) {
                st = (dx * ((lx0 - px) / su1x) + pz - lz0) / (dz - dx * (dx / su1x));
            } else if (dx != 0.0) {
                st = (su1x * ((lz0 - pz) / dx) + px - lx0) / (dx - su1x * (dz / dx));
            } else {
                st = 0.0f;
            }
            qx = st * dx + lx0;
            qz = st * dz + lz0;
            if (__fabs(dx) > __fabs(dz)) {
                s = (qx - lx0) / dx;
                ok = (s < -0.0000001 || s > 1.0000001);
            } else if (dz != 0.0) {
                s = (qz - lz0) / dz;
            }
            dz2 = pz - qz;
            dx2 = px - qx;
            yy = s * (nrm[i + 1].y - nrm[i].y) + nrm[i].y;
            dist = dx2 * dx2 + dz2 * dz2;
            if (dist > 0.0) {
                dist = sqrtf(dist);
            }
            if (dist >= minDist) {
                continue;
            }
            minDist = dist;
            if (flag) {
                if (yy < yA) {
                    cr = dx2 * su1x + dz2 * dx;
                    if (cr < 0.0f) {
                        if (dist > limA) { dist = limA; fl |= 2; }
                    } else {
                        if (dist > limB) { dist = limB; fl |= 2; }
                        dist = -dist;
                    }
                    ret = 1;
                    hit->t = yA;
                    hit->pos.x = pt[idxA].x + dist * nrm[idxA].z;
                    hit->pos.y = 0.0f;
                    hit->pos.z = pt[idxA].z - dist * nrm[idxA].x;
                    hit->dir.x = nrm[idxA].x;
                    hit->dir.y = 0.0f;
                    hit->dir.z = nrm[idxA].z;
                    hit->y = y0;
                    hit->f_24 = idxA;
                    hit->flags = fl | 1;
                    continue;
                }
                if (yy > yB) {
                    cr = dx2 * su1x + dz2 * dx;
                    if (cr < 0.0f) {
                        if (dist > limA) { dist = limA; fl |= 2; }
                    } else {
                        if (dist > limB) { dist = limB; fl |= 2; }
                        dist = -dist;
                    }
                    ret = 1;
                    hit->t = yB;
                    hit->pos.x = pt[idxB].x + dist * nrm[idxB].z;
                    hit->pos.y = 0.0f;
                    hit->pos.z = pt[idxB].z - dist * nrm[idxB].x;
                    hit->dir.x = nrm[idxB].x;
                    hit->dir.y = 0.0f;
                    hit->dir.z = nrm[idxB].z;
                    hit->y = y0;
                    hit->f_24 = idxB;
                    hit->flags = fl | 1;
                    continue;
                }
                cr = dx2 * su1x + dz2 * dx;
                if (cr < 0.0f) {
                    if (dist > limA) {
                        dx2 = limA * (dx2 / dist);
                        dz2 = limA * (dz2 / dist);
                        fl |= 2;
                    }
                } else {
                    if (dist > limB) {
                        dx2 = limB * (dx2 / dist);
                        dz2 = limB * (dz2 / dist);
                        fl |= 2;
                    }
                }
            }
            hit->t = yy;
            hit->pos.x = qx + dx2;
            hit->pos.y = 0.0f;
            hit->pos.z = qz + dz2;
            ret = 1;
            hit->dir.x = s * (nrm[i + 1].x - nrm[i].x) + nrm[i].x;
            hit->dir.y = 0.0f;
            hit->dir.z = s * (nrm[i + 1].z - nrm[i].z) + nrm[i].z;
            hit->y = y0;
            hit->f_24 = i;
            hit->flags = fl;
        }
    }
    return ret;
}
// CC_END
// FAKE: cos through an inline so the first-block cs is an inline @ temp (numbered above the hoisted +-1 constants, f15 as in the target)
static inline f32 camRoadCos(f32 x) { return (f32)cos(x); }
// FAKE: l2 through an inline so it is an inline @ temp (numbered with the sgn ?: temps) -> 2nd-web lim1/lim2 colour f27/f25 as in the target
static inline f32 crMul(f32 a, f32 b) { return a * b; }
// FAKE: the t/s solve through an inline (s via pointer) so t is the inline return @ temp, numbered above the CSE temps
// (v2.x load, frsp dB.x) -> t is simplified before the r1/r2 products and colours f5 as in the target
static inline f32 camRoadSolve(f32 ax, f32 az, Vec* dB, Vec* v2, f32* sp) {
    f32 t;
    if (__fabs(az) > __fabs(ax)) {
        t = (v2->x - ax * (v2->z / az)) / (dB->x - dB->z * (ax / az));
        *sp = (v2->z - t * dB->z) / az;
    } else {
        t = (v2->z - az * (v2->x / ax)) / (dB->z - az * (dB->x / ax));
        *sp = (v2->x - t * dB->x) / ax;
    }
    return t;
}
// FAKE: sgn1/sgn2 as if/else locals of an inline: inline locals get their @ labels at definition time, so the sgn webs
// are numbered above l2 (crMul) and below cs; with ax/az numbered low this gives the target l2 f14 / sgn1 f15 / sgn2 f28
static inline void camRoadSgn2(f32 s, f32 t, f32* p1, f32* p2) {
    f32 a, b;
    if (s < 0.0f) {
        a = -1.0f;
    } else {
        a = 1.0f;
    }
    if (t < 0.0f) {
        b = -1.0f;
    } else {
        b = 1.0f;
    }
    *p1 = a;
    *p2 = b;
}
void calcCamRoad(CamRoadState* out, CamRoadState* prev, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g) {
    f32 ax, az; // FAKE: named dA.x/dA.z copies declared first (numbered low, below l2) -> see camRoadSgn2
    f32 nx;
    f32 ny;
    f32 nz;
    f32 mnz;
    f32 cs, sn, w, ox, oz;
    s32 cnt;
    s32* idxp;
    s32 j;
    s32 l;
    CamRoadCurve* curve;
    void* hitCurve;
    s32 n;
    CamRoadGroup* grp;
    Vec* verts;
    s32 m;
    CamRoadStrip* strip;
    s32 k;
    s32* idx;
    f32 pz, qz, qx;
    CamRoadState** pp;
    f32 cs2;
    f32 angle;
    f32 rot;
    f32 f4c;
    f32 spd1;
    f32 spd2;
    CamRoadHit hit;
    CamRoadHit hit2;
    CamRoadHit hit3;
    Vec posA;
    Vec posB;
    Vec q;
    Vec p;
    Vec p2;
    Vec v1;
    Vec v2;
    Vec dA;
    Vec dB;
    Vec r1;
    Vec r2;
    f32 best;
    f32 zoom;
    f32 px;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    f32 s, t, sgn1, sgn2, len2, len;
    u32 flags;
    s32 found, found2;
    s32 i;
    f32 fov;
    f32 l1, l2, lim2, lim1;
    curve = wp->map->curves;
    best = -100000000.0f;
    hitCurve = NULL;
    posA.x = a;
    posA.y = b;
    posA.z = c;
    posB.x = d;
    posB.y = e;
    posB.z = f;
    for (i = 0; i < wp->map->curveNum; i++, curve++) {
        grp = &wp->map->f_f0[curve->grpOfs];
        n = curve->grpNum;
        if (wp->state == 0x65 && curve->off != 0) {
            continue;
        }
        if (n == 0) {
            if (curve->height > e) {
                continue;
            }
            if (curve->height > best) {
                best = curve->height;
                hitCurve = curve;
            }
            continue;
        }
        for (j = 0; j < n; j++, grp++) {
            verts = &wp->map->f_fc[grp->vtxOfs];
            m = grp->stripNum;
            strip = &wp->map->f_f4[grp->stripOfs];
            for (k = 0; k < m; k++, strip++) {
                cnt = strip->num;
        cnt = cnt - 1;
                idx = &wp->map->f_100[strip->idxOfs];
                idxp = idx + 1;
                for (l = 1; cnt > l; idxp++, l++) {
                    if (collisionTri_simple(posB, verts[idx[0]], verts[idxp[0]], verts[idxp[1]], &best)) {
                        hitCurve = curve;
                        break;
                    }
                }
            }
        }
    }
    if (hitCurve != NULL) {
        hit.y = -1000000.0f;
        hit.dir.x = 0.0f;
        hit.dir.y = 0.0f;
        hit.dir.z = 0.0f;
        if (!collisionCurve(0, hitCurve, posA, &hit)) {
            hitCurve = NULL;
        }
    }
    curve = hitCurve;
    if (curve == NULL) {
        curve = prev->on ? prev->cam : NULL;
        if (curve == NULL) {
            wp->out[0] = a;
            wp->out[1] = b;
            wp->out[2] = c;
            wp->out[3] = d;
            wp->out[4] = e;
            wp->out[5] = f;
            return;
        }
        x = a;
        if (wp->state == 0x65 && curve != NULL && curve->f_20 != 0) {
            y = curve->f_24;
        } else {
            y = b;
        }
        z = c;
        nx = -prev->dir.x;
        ny = -prev->dir.y;
        nz = -prev->dir.z;
        angle = prev->angle;
        dist = prev->dist;
        rot = prev->rot;
        f4c = prev->f_4c;
        flags = 0;
    } else {
        if (hit.t < 0.0f) {
            hit.t = 0.0f;
        }
        if (hit.t > 1.0f) {
            hit.t = 1.0f;
        }
        angle = curve->f_64;
        rot = curve->f_70;
        dist = curve->f_58;
        x = hit.pos.x;
        if (wp->state == 0x65 && curve->f_20 != 0) {
            y = curve->f_24;
        } else {
            y = b;
        }
        nx = -hit.dir.z;
        len2 = nx * nx + hit.dir.x * hit.dir.x;
        f4c = hit.f_24;
        nz = hit.dir.x;
        z = hit.pos.z;
        flags = hit.flags;
        ny = hit.dir.y;
        if (len2 != 0.0) {
            len = sqrtf(len2);
            nx = nx / len;
            ny = 0.0f;
            nz = nz / len;
        }
        nx *= -1.0f;
        ny *= -1.0f;
        nz *= -1.0f;
    }
    if (curve->f_b4 != 0) {
        grp = &wp->map->f_104[curve->f_b0];
        q.y = 100000000.0f;
        q.x = x;
        q.z = z;
        hit.y = -1000000.0f;
        if (collisionCurve(1, grp, q, &hit)) {
            flags |= hit.flags;
            x = hit.pos.x;
            z = hit.pos.z;
        }
    }
    if (curve != NULL) {
        fov = 3.141592f * (wp->map->f_e8->x / 2.0f / 180.0f);
        zoom = 1.0f;
        if (g && hitCurve != NULL && prev->on) {
            cs = camRoadCos(fov);
            sn = (f32)sin(fov);
            mnz = -nz;
            w = 0.01f * (prev->f_58 * (zoom * (640.0f * (dist * sn / cs) / 480.0f)));
            found = 0;
            p = posA;
            ox = w * mnz;
            oz = w * nx;
            p.x = p.x + ox;
            p.y = 1000000.0f;
            p.z = p.z + oz;
            hit2.y = -1000000.0f;
            hit2.dir.z = 0.0f;
            hit2.dir.y = 0.0f;
            hit2.dir.x = 0.0f;
            if (collisionCurve(0, curve, p, &hit2)) {
                found = 1;
                p.x = hit2.pos.x;
                p.y = y;
                p.z = hit2.pos.z;
            }
            if (curve->f_b4 != 0) {
                grp = &wp->map->f_104[curve->f_b0];
                hit2.y = -1000000.0f;
                if (collisionCurve(1, grp, p, &hit2)) {
                    found = 1;
                    p.x = hit2.pos.x;
                    p.y = y;
                    p.z = hit2.pos.z;
                }
            }
            if (found) {
                qx = p.x;
                qz = p.z;
            } else {
                qx = x;
                qz = z;
            }

            cs2 = (f32)cos(fov);
            sn = (f32)sin(fov);
            w = 0.01f * (g * (zoom * (640.0f * (dist * sn / cs2) / 480.0f)));
            found2 = 0;
            flags = 0;
            p2 = posA;
            ox = w * mnz;
            oz = w * nx;
            p2.x = p2.x + ox;
            p2.y = 1000000.0f;
            p2.z = p2.z + oz;
            hit3.y = -1000000.0f;
            hit3.dir.z = 0.0f;
            hit3.dir.y = 0.0f;
            hit3.dir.x = 0.0f;
            if (collisionCurve(0, curve, p2, &hit3)) {
                found2 = 1;
                p2.x = hit3.pos.x;
                // FAKE: volatile flags load + p2.y store order the lwz between the p2.x and p2.y stores (target 16c8)
                flags = *(volatile u32*)&hit3.flags;
                *(volatile f32*)&p2.y = y;
                p2.z = hit3.pos.z;
            }
            if (curve->f_b4 != 0) {
                grp = &wp->map->f_104[curve->f_b0];
                hit3.y = -1000000.0f;
                if (collisionCurve(1, grp, p2, &hit3)) {
                    found2 = 1;
                    p2.x = hit3.pos.x;
                    flags |= hit3.flags;
                    p2.z = hit3.pos.z;
                }
            }
            if (found2) {
                px = p2.x;
                pz = p2.z;
                if (!(hit3.flags & 1) && (prev->f_08 & 1)) {
                    wp->f_19c[4] = 0.0f;
                }
                if (!(hit3.flags & 2) && (prev->f_08 & 2)) {
                    wp->f_19c[5] = 0.0f;
                }
            } else {
                px = x;
                pz = z;
            }
            v1.x = qx - prev->pos.x;
            v1.y = 0.0f;
            v1.z = qz - prev->pos.z;
            v2.x = px - prev->pos.x;
            v2.y = 0.0f;
            v2.z = pz - prev->pos.z;
            lim1 = PSVECMag(&v1);
            lim2 = PSVECMag(&v2);
            lim1 += getV60FPS(wp->f_19c[0], prev->time, gp->renderFieldTime);
            lim2 = getV60FPS(wp->f_19c[1], prev->time, gp->renderFieldTime);
            l1 = lim1 * wp->f_19c[4];
            l2 = crMul(lim1, wp->f_19c[5]);
            spd1 = lim2 * wp->f_19c[4];
            spd2 = lim2 * wp->f_19c[5];
            dA.x = mnz;
            dA.z = nx;
            ax = dA.x;
            az = dA.z;
            dB.x = -az;
            dB.z = ax;
            dA.y = 0.0f;
            dB.y = 0.0f;
            r1.y = 0.0f;
            r2.y = 0.0f;
            t = camRoadSolve(ax, az, &dB, &v2, &s);
            r1.x = s * dA.x;
            r1.z = s * dA.z;
            r2.x = t * dB.x;
            r2.z = t * dB.z;
            camRoadSgn2(s, t, &sgn1, &sgn2);
            lim1 = PSVECMag(&r1);
            lim2 = PSVECMag(&r2);
            if (lim1 > l1 && lim1 > spd1) {
                PSVECScale(&dA, &r1, l1 * sgn1);
            }
            if (lim2 > l2 && lim2 > spd2) {
                PSVECScale(&dB, &r2, l2 * sgn2);
            }
            x = prev->pos.x;
            z = prev->pos.z;
            x += r1.x + r2.x;
            z += r1.z + r2.z;
        }
    }
    wp->f_19c[4] += wp->f_19c[2];
    if (wp->f_19c[4] > 1.0f) {
        wp->f_19c[4] = 1.0f;
    }
    wp->f_19c[5] += wp->f_19c[3];
    if (wp->f_19c[5] > 1.0f) {
        wp->f_19c[5] = 1.0f;
    }
    out->on = 1;
    out->f_04 = hitCurve != NULL;
    out->f_08 = flags;
    out->in[0] = a;
    out->in[1] = b;
    out->in[2] = c;
    out->in[3] = d;
    out->in[4] = e;
    out->in[5] = f;
    out->cam = curve;
    out->pos.x = x;
    out->pos.y = y;
    out->pos.z = z;
    out->dir.x = -nx;
    out->dir.y = -ny;
    out->dir.z = -nz;
    out->dist = dist;
    out->angle = angle;
    out->rot = rot;
    out->f_4c = f4c;
    out->time = gp->renderFieldTime;
    out->f_58 = g;
    out->shiftXRate = curve->f_7c;
}
static inline f32 crSq(f32 x) { return x * x; }
u32 compareCamRoad(CamRoadState* a, CamRoadState* b, f32* outSpeed) {
    u32 flags = 0;
    s32 d;
    s32 n;
    f32 dot, ang, speed, t, dx, dy, dz, d2, dist;
    if (!b->on) {
        return 0;
    }
    if (a->cam == b->cam && !(a->f_04 == 1 && b->f_04 == 0) && !(a->f_04 == 0 && b->f_08 == 1)) {
        d = (s32)a->f_4c - (s32)b->f_4c;
        if (__abs(d) <= 1) {
            return 0;
        }
        n = a->cam->idxB - a->cam->idxA - 1;
        if (__abs(d) >= n) {
            return 0;
        }
        n = a->cam->idxB - a->cam->idxA - 2;
        if (__abs(d) >= n) {
            flags |= 0x20;
        }
    }
    if (a->cam != b->cam) {
        flags |= 1;
    }
    if (a->f_04 == 1 && b->f_04 == 0) {
        flags |= 2;
    }
    if (a->f_04 == 0 && b->f_08 == 1) {
        flags |= 4;
    }
    if (!(__fabs(a->f_4c - b->f_4c) <= 1.0)) {
        flags |= 8;
    }
    dot = a->dir.x * b->dir.x + a->dir.z * b->dir.z;
    if (dot > 1.0f) {
        dot = 1.0f;
    }
    ang = __fabsf(57.29578f * (f32)acos(dot));
    if (ang > 180.0f) {
        ang = 360.0f - ang;
    }
    t = 6.2831855f * a->dist * ang / 360.0f;
    speed = t * wp->f_19c[6];
    ang = __fabsf(a->angle - b->angle);
    if (ang > 180.0f) {
        ang = 360.0f - ang;
    }
    t = 6.2831855f * a->dist * ang / 360.0f;
    t *= wp->f_19c[7];
    if (t > speed) {
        speed = t;
    }
    ang = __fabsf(a->rot - b->rot);
    if (ang > 180.0f) {
        ang = 360.0f - ang;
    }
    t = 6.2831855f * a->dist * ang / 360.0f;
    t *= wp->f_19c[8];
    if (t > speed) {
        speed = t;
    }
    t = __fabsf(a->dist - b->dist);
    t *= wp->f_19c[9];
    if (t > speed) {
        speed = t;
    }
    dx = a->pos.x - b->pos.x;
    dy = a->pos.y - b->pos.y;
    dz = a->pos.z - b->pos.z;
    dist = crSq(dx);
    dist += crSq(dy);
    dist += crSq(dz);
    if (dist) {
        if (dist > 0.0f) {
            f64 guess = __frsqrte(dist);
            guess = 0.5 * guess * (3.0 - guess * guess * dist);
            guess = 0.5 * guess * (3.0 - guess * guess * dist);
            guess = 0.5 * guess * (3.0 - guess * guess * dist);
            dist = dist * guess;
        } else if (dist < 0.0) {
            dist = NAN;
        } else if (isnan(dist)) {
            dist = NAN;
        }
    } else {
        dist = 0.0f;
    }
    if (speed < 0.01f && dist < 0.3f) {
        return 0;
    }
    t = dist * wp->f_19c[10];
    if (t > speed) {
        speed = t;
    }
    if (speed > 90.0f) {
        speed = 90.0f;
    }
    if (speed < 20.0f) {
        speed = 20.0f;
    }
    *outSpeed = 1.0f / speed;
    if (!flags) {
        flags |= 0x10;
    }
    return flags;
}
static inline f32 hokanRate(f32 t, f32 a, f32 b, f32 k, f32 o) {
    f32 c1, c0, c2, r;
    c0 = (f32)((double (*)(f32))cos)(a);
    c1 = (f32)((double (*)(f32))cos)(b);
    c2 = (f32)cos(3.141592f * t * k + o);
    r = (2.0f / (1.0f + c0)) * (0.5f * (1.0f - c1) + c2);
    r = 0.500001f * (2.0f - (1.0f + r));
    if (r > 1.0f) {
        r = 1.0f;
    }
    return r;
}
static inline f32 angleLerp(f32 a0, f32 a1, f32 r) {
    while (a0 - a1 > 3.1415927f) {
        a0 -= 6.2831855f;
    }
    while (a0 - a1 < -3.1415927f) {
        a0 += 6.2831855f;
    }
    return r * (a1 - a0) + a0;
}
#pragma dont_inline on
void calcHokanCamRoad(s32 type, CamRoadState* from, CamRoadState* to, CamRoadState* unused, CamRoadState* out, f32 t) {
    f32 r, a0, a1, a2;
    r = hokanRate(t, 0.0f, 0.0f, 1.0f, 0.0f);
    if (type) {
        out->pos.x = r * (to->pos.x - from->pos.x) + from->pos.x;
        out->pos.y = r * (to->pos.y - from->pos.y) + from->pos.y;
        out->pos.z = r * (to->pos.z - from->pos.z) + from->pos.z;
    } else {
        out->pos.x = to->pos.x;
        out->pos.y = to->pos.y;
        out->pos.z = to->pos.z;
    }
    a2 = (f32)atan2(from->dir.x, from->dir.z);
    a0 = a2;
    a1 = (f32)atan2(to->dir.x, to->dir.z);
    while (a0 - a1 > 3.1415927f) {
        a0 -= 6.2831855f;
    }
    while (a0 - a1 < -3.1415927f) {
        a0 += 6.2831855f;
    }
    a0 = r * (a1 - a0) + a0;
    out->dir.x = (f32)sin(a0);
    out->dir.z = (f32)cos(a0);
    out->dir.y = r * (to->dir.y - from->dir.y) + from->dir.y;
    out->dist = r * (to->dist - from->dist) + from->dist;
    out->angle = r * (to->angle - from->angle) + from->angle;
    out->rot = r * (to->rot - from->rot) + from->rot;
    out->shiftXRate = r * (to->shiftXRate - from->shiftXRate) + from->shiftXRate;
}
#pragma dont_inline reset
#define CAMROAD_HOKAN_START(src, flags)                                                                     \
    {                                                                                                         \
        wp->cmpFlags = flags;                                                                                 \
        wp->f_c8 = 1;                                                                                         \
        wp->time = gp->renderFieldTime;                                                                       \
        wp->hokanT = 0.0f;                                                                                    \
        wp->start = src;                                                                                      \
        wp->prev = wp->next;                                                                                  \
        if (wp->prev.f_08 || wp->start.f_08 ||                                                                \
            (wp->state == 0x65 &&                                                                             \
             ((wp->prev.cam && wp->prev.cam->f_20) || (wp->start.cam && wp->start.cam->f_20))) ||             \
            (wp->prev.cam && wp->prev.cam->f_7c) || (wp->start.cam && wp->start.cam->f_7c)) {                 \
            wp->hokanType = 1;                                                                                \
        } else {                                                                                              \
            wp->hokanType = 0;                                                                                \
        }                                                                                                     \
    }
void camRoadMain(CameraParams* out, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g) {
    u32 flags;
    wp->in[0] = a;
    wp->in[1] = b;
    wp->in[2] = c;
    wp->in[3] = d;
    wp->in[4] = e;
    wp->in[5] = f;
    calcCamRoad(&wp->next, &wp->prev, a / 10.0f, b / 10.0f, c / 10.0f, d / 10.0f, e / 10.0f, f / 10.0f, g);
    if (wp->f_c8) {
        flags = compareCamRoad(&wp->prev, &wp->next, &wp->hokanSpeed);
        if (flags) {
            CAMROAD_HOKAN_START(wp->cur, flags)
        } else {
            wp->prev = wp->next;
        }
    }
    if (!wp->f_c8) {
        flags = compareCamRoad(&wp->next, &wp->prev, &wp->hokanSpeed);
        if (flags) {
            CAMROAD_HOKAN_START(wp->prev, flags)
        }
    }
    if (wp->f_c8) {
        wp->hokanT += getV60FPS(wp->hokanSpeed, wp->time, gp->renderFieldTime);
        if (wp->hokanT > 1.0f) {
            wp->hokanT = 1.0f;
        }
        wp->time = gp->renderFieldTime;
        calcHokanCamRoad(wp->hokanType, &wp->start, &wp->prev, &wp->next, &wp->cur, wp->hokanT);
        if (wp->hokanType) {
            wp->cur.f_08 = 1;
        } else {
            wp->cur.f_08 = 0;
        }
        if (1.0f == wp->hokanT) {
            wp->cur = wp->prev;
            wp->f_c8 = 0;
        }
    } else {
        wp->cur = wp->prev = wp->next;
    }
    calcCurrentCamState(out);
    out->at.x *= 10.0f;
    out->at.y *= 10.0f;
    out->at.z *= 10.0f;
    out->pos.x *= 10.0f;
    out->pos.y *= 10.0f;
    out->pos.z *= 10.0f;
}
CamRoadWork* camRoadGetWorkPtr(void) {
    return wp;
}
f32 camRoadGetCurrentShiftXRate(void) {
    if (wp->next.on) {
        return wp->next.shiftXRate;
    }
    return 0.0f;
}
void camRoadReset(void) {
    s32 a = wp->f_204;
    CamRoadMap* m = wp->map;
    u32 s = wp->state;
    memset(wp, 0, 0x210);
    wp->f_c8 = 0;
    wp->prev.on = 0;
    wp->prev.on = 0;
    wp->f_19c[0] = 0.005f;
    wp->f_19c[1] = 0.0f;
    wp->f_19c[2] = 0.004f;
    wp->f_19c[3] = 0.008f;
    wp->f_19c[4] = 1.0f;
    wp->f_19c[5] = 1.0f;
    wp->f_19c[6] = 2.0f;
    wp->f_19c[7] = 1.0f;
    wp->f_19c[8] = 1.0f;
    wp->f_19c[9] = 3.0f;
    wp->f_19c[10] = 1.0f;
    wp->f_204 = -1;
    wp->f_208 = 0;
    wp->f_204 = a;
    wp->map = m;
    wp->state = s;
}
void camRoadCurveOn(const char* name) {
    CamRoadCurve* c = wp->map->curves;
    s32 i;
    if (wp->state == 0x65) {
        for (i = 0; i < wp->map->curveNum; i++, c++) {
            if (strcmp(c->name, name) == 0) {
                c->off = 0;
                break;
            }
        }
    }
}
void camRoadCurveOff(const char* name) {
    CamRoadCurve* c = wp->map->curves;
    s32 i;
    if (wp->state == 0x65) {
        for (i = 0; i < wp->map->curveNum; i++, c++) {
            if (strcmp(c->name, name) == 0) {
                c->off = 1;
                break;
            }
        }
    }
}

// forward declarations: these functions are defined further down
