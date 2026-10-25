#include "driver/camdrv.h"
#include "mario/mariost.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"
#include <math.h>
#include "himo.h"


extern void spline_getvalue(Vec* out, s32 n, Vec* pts, f32* t1, f32* t2, f32 t);
extern void spline_maketable(s32 n, Vec* pts, f32* t1, f32* t2);

typedef struct HimoWork {
    /* 0x00 */ Vec p0;
    /* 0x0C */ Vec p1;
    /* 0x18 */ Mtx m0;
    /* 0x48 */ Mtx m1;
    /* 0x78 */ Vec pts[4];
    /* 0xA8 */ f32 t1[4];
    /* 0xB8 */ f32 t2[12];
} HimoWork;

s8 Pos[6] ATTRIBUTE_ALIGN(32) = { 1, 0, 0, -1, 0, 0 };
s8 Nrm[3] ATTRIBUTE_ALIGN(32) = { 0, 0, 127 };
u8 Col[4] ATTRIBUTE_ALIGN(32) = { 0xFF, 0xFF, 0xFF, 0xFF };


void vivihimoDisp(CameraId camId, HimoWork* w);

void vivihimo(Mtx a, Mtx b, f32 speed, f32 scale, f32 offA, f32 offB) {
    HimoWork w;
    HimoWork* pw = &w;
    f32 m0, m2, len, wob;
    Vec* pt;
    s32 i;
    Vec* pts;

    pw->p0.x = a[0][3];
    pw->p0.y = a[1][3] + offA;
    pw->p0.z = a[2][3];
    pw->p1.x = b[0][3];
    pw->p1.y = b[1][3] + offB;
    pw->p1.z = b[2][3];
    m0 = 0.5f * PSVECMag((Vec*)a[0]);
    m2 = 0.5f * PSVECMag((Vec*)a[2]);
    len = sqrtf(m0 * m0 + m2 * m2);
    scale *= 0.5f * len;
    PSMTXCopy(a, pw->m0);
    PSMTXCopy(b, pw->m1);
    {
    Vec dir = *(Vec*)a[0];
    if (dir.x != 0.0f || dir.y != 0.0f || dir.z != 0.0f) {
        Vec dn;
        Vec delta;
        PSVECNormalize(&dir, &dir);
        pts = pw->pts;
        {
        Vec off = { 0.0f, 0.0f, 0.0f };
        off.x = -dir.x;
        off.y = -dir.y;
        off.z = -dir.z;
        dn = off;
        }
        wob = (f32)sin(0.1f * ((f32)gp->retraceCount * speed));
        wob = wob * 2.5f;
        PSVECSubtract(&pw->p1, &pw->p0, &delta);
        pt = pts;
        for (i = 0; i < 4; i++) {
            f32 s = 10.0f * (f32)sin(3.1415927f * (f32)i / 4.0f) + wob;
            f32 k;
            if (i == 1) {
                k = scale * (0.2f * s);
            } else if (i == 2) {
                k = scale * s;
            } else {
                k = scale * (-1.0f * (f32)(i & 1) * s);
            }
            pt->x = pw->p0.x + 0.33333334f * (delta.x * (f32)i) + dn.x * k;
            pt->y = pw->p0.y + 0.33333334f * (delta.y * (f32)i) + dn.y * k;
            pt->z = pw->p0.z + 0.33333334f * (delta.z * (f32)i) + dn.z * k;
            pt++;
        }
        pts[0] = pw->p0;
        pts[3] = pw->p1;
        spline_maketable(4, pts, pw->t1, pw->t2);
        vivihimoDisp(camGetCurNo(), pw);
    }
    }
}


static inline void himoMtx(HimoWork* w, Mtx out, f32 t, Vec* p) {
    f32 u = 1.0f - t;
    out[0][0] = 10.0f * (u * w->m0[0][0] + t * w->m1[0][0]);
    out[0][1] = u * w->m0[0][1] + t * w->m1[0][1];
    out[0][2] = u * w->m0[0][2] + t * w->m1[0][2];
    out[0][3] = p->x;
    out[1][0] = 10.0f * (u * w->m0[1][0] + t * w->m1[1][0]);
    out[1][1] = u * w->m0[1][1] + t * w->m1[1][1];
    out[1][2] = u * w->m0[1][2] + t * w->m1[1][2];
    out[1][3] = p->y;
    out[2][0] = 10.0f * (u * w->m0[2][0] + t * w->m1[2][0]);
    out[2][1] = u * w->m0[2][1] + t * w->m1[2][1];
    out[2][2] = u * w->m0[2][2] + t * w->m1[2][2];
    out[2][3] = p->z;
}

void vivihimoDisp(CameraId camId, HimoWork* w) {
    CameraEntry* cam;
    Vec* pts;
    f32* t1;
    f32* t2;
    s32 i;
    MtxPtr pA;
    MtxPtr pB;
    MtxPtr tp;
    MtxPtr pT;
    Mtx mA, mB, tmp;
    Vec prev;
    Vec cur;
    f32 tanBase, k, t, pwid, wid;
    f32 ang0 = -1.288053f;

    pts = w->pts;
    t1 = w->t1;
    t2 = w->t2;
    cam = camGetPtr(camId);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_POS, Pos, 3);
    GXSetArray(GX_VA_NRM, Nrm, 3);
    GXSetArray(GX_VA_CLR0, Col, 4);

    tanBase = (f32)tan(1.288053035736084);
    k = 0.5f / tanBase;
    t = 0.06666667f;
    pwid = k * (tanBase + (f32)tan(ang0));
    pA = mA;
    pB = mB;
    spline_getvalue(&prev, 4, pts, t1, t2, 0.0f);
    himoMtx(w, tmp, 0.0f, &prev);
    PSMTXConcat(cam->view, tmp, pA);
    tp = tmp;
    for (i = 0; i < 15;) {
        spline_getvalue(&cur, 4, pts, t1, t2, t);
        GXLoadPosMtxImm(pA, 0);
        himoMtx(w, tp, t, &cur);
        PSMTXConcat(cam->view, tp, pB);
        GXLoadPosMtxImm(pB, 3);
        wid = k * (tanBase + (f32)tan(3.1415927f * (0.82f * t) - 1.288053f));
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 1;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.f32 = 0.0f; GXWGFifo.f32 = pwid;
        GXWGFifo.u8 = 3;
        GXWGFifo.u8 = 1;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.f32 = 0.0f; GXWGFifo.f32 = wid;
        GXWGFifo.u8 = 3;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.f32 = 1.0f; GXWGFifo.f32 = wid;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.u8 = 0;
        GXWGFifo.f32 = 1.0f; GXWGFifo.f32 = pwid;
        pwid = wid;
        pT = pA;
        pA = pB;
        pB = pT;
        prev = cur;
        i++;
        t = (f32)(i + 1) / 15.0f;
    }
}

