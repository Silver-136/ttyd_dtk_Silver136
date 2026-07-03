#include "effect/eff_peach_fukidashi.h"

extern f32 float_0_80426f94;

void effPeachFukidashiDisp(s32 cameraId, void* effect);
void effDelete(void* effect);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);

void effSetPeachFukiBatu(char* name) {
    extern void* effNameToPtr(char* name);

    void* effect = effNameToPtr(name);
    void* work = *(void**)((s32)effect + 0xC);
    *(u32*)work |= 0x80;
}

void effPeachFukidashiMain(void* effect) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 timer = *(s32*)((s32)work + 0x1C);

    if (timer < 1000) {
        *(s32*)((s32)work + 0x1C) = timer - 1;
    }
    if (*(s32*)((s32)work + 0x1C) < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effPeachFukidashiDisp, effect, float_0_80426f94);
    }
}

void effPeachFukidashiDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* camGetPtr(s32 cameraId);
    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern void PSMTXRotAxisRad(Mtx m, Vec* axis, f32 angle);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void iconDispGx2(void* mtx, s32 flags, s32 iconId);
    extern void FontDrawStart(void);
    extern void FontDrawColor(void* color);
    extern void FontDrawStringMtx(void* mtx, char* str);

    extern u32 vec3_802fe570[];
    extern u32 dat_802fe57c[];
    extern u32 dat_80426f68;
    extern f32 float_deg2rad_80426f6c;
    extern f32 float_0p5_80426f70;
    extern f32 float_16_80426f74;
    extern f32 float_0p4_80426f78;
    extern f32 float_0p34_80426f7c;
    extern f32 float_40_80426f80;
    extern f32 float_7p25_80426f84;
    extern f32 float_14_80426f88;
    extern f32 float_1p5_80426f8c;
    extern f32 float_90_80426f90;
    extern f32 float_0_80426f94;
    extern f32 float_1_80426f98;
    extern f32 float_0p75_80426f9c;
    extern f32 float_6_80426fa0;
    extern f32 float_11p5_80426fa4;
    extern f32 float_30_80426fa8;
    extern f32 float_8_80426fac;

    s32 work;
    void* cam;
    void* owner;
    Vec axis;
    Vec base;
    Mtx axisMtx;
    Mtx transMtx;
    Mtx rotMtx;
    Mtx viewMtx;
    Mtx scaleMtx;
    Mtx drawMtx;
    f32 dist;
    f32 ownerY;
    f32 rise;
    f32 tmp;
    f32 zero;
    u32 textWords[8];
    u32 color;
    s32 flags;

    work = *(s32*)((s32)effect + 0xC);

    camGetPtr(cameraId);
    cam = camGetPtr(4);

    axis.x = ((f32*)vec3_802fe570)[0];
    axis.y = ((f32*)vec3_802fe570)[1];
    axis.z = ((f32*)vec3_802fe570)[2];

    axis.x = (f32)sin((double)(float_deg2rad_80426f6c * *(f32*)((s32)cam + 0x114)));
    axis.z = -(f32)cos((double)(float_deg2rad_80426f6c * *(f32*)((s32)cam + 0x114)));

    dist = float_0p5_80426f70 * (float_16_80426f74 + *(f32*)(work + 0x14));
    owner = *(void**)(work + 0x20);
    if (owner != 0) {
        base.x = *(f32*)((s32)owner + 0x8C);
        base.y = *(f32*)((s32)owner + 0x90);
        base.z = *(f32*)((s32)owner + 0x94);
        ownerY = *(f32*)((s32)owner + 0x150);
    } else {
        owner = marioGetPtr();
        base.x = *(f32*)((s32)owner + 0x8C);
        base.y = *(f32*)((s32)owner + 0x90);
        base.z = *(f32*)((s32)owner + 0x94);
        ownerY = *(f32*)((s32)owner + 0x1BC);
    }

    PSMTXRotAxisRad(axisMtx, &axis, float_deg2rad_80426f6c * -*(f32*)(work + 0x10));

    *(f32*)(work + 0x4) = axisMtx[0][1] * dist + base.x;
    *(f32*)(work + 0x8) = axisMtx[1][1] * dist + base.y + ownerY;
    *(f32*)(work + 0xC) = axisMtx[2][1] * dist + base.z;
    *(f32*)(work + 0x18) = *(f32*)(work + 0x10);

    PSMTXTrans(
        transMtx,
        (double)*(f32*)(work + 0x4),
        (double)*(f32*)(work + 0x8),
        (double)*(f32*)(work + 0xC));

    cam = camGetPtr(cameraId);
    PSMTXRotRad(rotMtx, (double)(float_deg2rad_80426f6c * -*(f32*)((s32)cam + 0x114)), 'y');
    PSMTXConcat(transMtx, rotMtx, viewMtx);

    PSMTXScale(scaleMtx, float_0p4_80426f78, float_0p34_80426f7c, float_0p4_80426f78);
    PSMTXRotRad(rotMtx, (double)(float_deg2rad_80426f6c * *(f32*)(work + 0x18)), 'z');

    PSMTXConcat(viewMtx, scaleMtx, drawMtx);
    PSMTXConcat(drawMtx, rotMtx, drawMtx);
    iconDispGx2(drawMtx, 0, 0x1F9);

    cam = camGetPtr(4);

    axis.x = ((f32*)vec3_802fe570)[0];
    axis.y = ((f32*)vec3_802fe570)[1];
    axis.z = ((f32*)vec3_802fe570)[2];

    axis.x = (f32)sin((double)(float_deg2rad_80426f6c * *(f32*)((s32)cam + 0x114)));
    axis.z = -(f32)cos((double)(float_deg2rad_80426f6c * *(f32*)((s32)cam + 0x114)));

    dist = float_0p5_80426f70 * (float_40_80426f80 + *(f32*)(work + 0x14));
    owner = *(void**)(work + 0x20);
    if (owner != 0) {
        base.x = *(f32*)((s32)owner + 0x8C);
        base.y = *(f32*)((s32)owner + 0x90);
        base.z = *(f32*)((s32)owner + 0x94);
        ownerY = *(f32*)((s32)owner + 0x150);
    } else {
        owner = marioGetPtr();
        base.x = *(f32*)((s32)owner + 0x8C);
        base.y = *(f32*)((s32)owner + 0x90);
        base.z = *(f32*)((s32)owner + 0x94);
        ownerY = *(f32*)((s32)owner + 0x1BC);
    }

    PSMTXRotAxisRad(axisMtx, &axis, float_deg2rad_80426f6c * -*(f32*)(work + 0x10));

    *(f32*)(work + 0x4) = axisMtx[0][1] * dist + base.x;
    *(f32*)(work + 0x8) = axisMtx[1][1] * dist + base.y + ownerY;
    *(f32*)(work + 0xC) = axisMtx[2][1] * dist + base.z;
    *(f32*)(work + 0x18) = *(f32*)(work + 0x10);

    rise = (float_1p5_80426f8c * *(f32*)(work + 0x10)) / float_90_80426f90;
    if (rise < float_0_80426f94) {
        rise = -rise;
    }

    PSMTXTrans(
        transMtx,
        (double)*(f32*)(work + 0x4),
        (double)(float_7p25_80426f84 + (*(f32*)(work + 0x8) - float_14_80426f88) + rise),
        (double)*(f32*)(work + 0xC));

    cam = camGetPtr(cameraId);
    PSMTXRotRad(rotMtx, (double)(float_deg2rad_80426f6c * -*(f32*)((s32)cam + 0x114)), 'y');
    PSMTXConcat(transMtx, rotMtx, viewMtx);

    zero = float_0_80426f94;
    PSMTXTrans(transMtx, (double)zero, (double)zero, (double)float_1_80426f98);
    PSMTXConcat(viewMtx, transMtx, viewMtx);

    PSMTXScale(scaleMtx, float_0p4_80426f78, float_0p4_80426f78, float_0p4_80426f78);
    PSMTXConcat(viewMtx, scaleMtx, viewMtx);

    flags = *(s32*)work & 3;
    if (flags == 1) {
        iconDispGx2(viewMtx, 0, 0x1FA);
    } else if (flags == 0) {
        iconDispGx2(viewMtx, 0, 0x1FC);
    } else {
        iconDispGx2(viewMtx, 0, 0x1FB);
    }

    if ((*(u32*)work & 0x80) != 0) {
        textWords[0] = dat_802fe57c[0];
        textWords[1] = dat_802fe57c[1];
        textWords[2] = dat_802fe57c[2];
        textWords[3] = dat_802fe57c[3];
        textWords[4] = dat_802fe57c[4];
        textWords[5] = dat_802fe57c[5];
        textWords[6] = dat_802fe57c[6];
        textWords[7] = dat_802fe57c[7];
        color = dat_80426f68;

        PSMTXScale(scaleMtx, float_0p75_80426f9c, float_0p75_80426f9c, float_0p75_80426f9c);

        tmp = (float_1p5_80426f8c * *(f32*)(work + 0x10)) / float_90_80426f90;
        if (tmp < float_0_80426f94) {
            tmp = -tmp;
        }

        PSMTXTrans(
            transMtx,
            (double)(float_6_80426fa0 + (*(f32*)(work + 0x4) - float_11p5_80426fa4)),
            (double)((float_30_80426fa8 + (*(f32*)(work + 0x8) - float_14_80426f88) + tmp) - float_8_80426fac),
            (double)*(f32*)(work + 0xC));

        cam = camGetPtr(cameraId);
        PSMTXRotRad(rotMtx, (double)(float_deg2rad_80426f6c * -*(f32*)((s32)cam + 0x114)), 'y');
        PSMTXConcat(transMtx, rotMtx, viewMtx);

        PSMTXTrans(transMtx, (double)zero, (double)zero, (double)float_1_80426f98);
        PSMTXConcat(viewMtx, transMtx, viewMtx);
        PSMTXConcat(viewMtx, scaleMtx, viewMtx);

        FontDrawStart();
        FontDrawColor(&color);
        FontDrawStringMtx(viewMtx, (char*)textWords);
    }
}


/* CHATGPT STUB FILL: main/effect/eff_peach_fukidashi 20260624_184128 */

/* stub-fill: effPeachFukidashiEntry | missing_definition | ghidra_signature */
void* effPeachFukidashiEntry(s32 flags, void* owner, s32 timer, double unused0, double unused1, double unused2, double radius, double angle) {
    typedef f32 Mtx[3][4];
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* camGetPtr(s32 cameraId);
    extern void* marioGetPtr(void);
    extern double sin(double x);
    extern double cos(double x);
    extern void PSMTXRotAxisRad(Mtx m, Vec* axis, f32 angle);
    extern void effPeachFukidashiMain(void* effect);
    extern const char str_PeachFukidashi_802fe59c[];
    extern u32 vec3_802fe570[];
    extern f32 float_deg2rad_80426f6c;
    extern f32 float_0p5_80426f70;
    extern f32 float_16_80426f74;
    void* entry;
    s32 work;
    void* cam;
    void* baseObj;
    Vec axis;
    Vec base;
    Mtx axisMtx;
    f32 dist;
    f32 ownerY;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_PeachFukidashi_802fe59c;
    *(s32*)((s32)entry + 8) = 1;
    work = (s32)__memAlloc(3, *(s32*)((s32)entry + 8) * 0x24);
    *(s32*)((s32)entry + 0xC) = work;
    *(void (**)(void*))((s32)entry + 0x10) = effPeachFukidashiMain;

    *(s32*)work = flags;
    *(f32*)(work + 0x14) = (f32)radius;
    *(f32*)(work + 0x10) = (f32)angle;
    *(s32*)(work + 0x1C) = timer;
    *(void**)(work + 0x20) = owner;

    cam = camGetPtr(4);
    axis.x = ((f32*)vec3_802fe570)[0];
    axis.y = ((f32*)vec3_802fe570)[1];
    axis.z = ((f32*)vec3_802fe570)[2];
    axis.x = (f32)sin((double)(float_deg2rad_80426f6c * *(f32*)((s32)cam + 0x114)));
    axis.z = -(f32)cos((double)(float_deg2rad_80426f6c * *(f32*)((s32)cam + 0x114)));

    dist = float_0p5_80426f70 * (float_16_80426f74 + *(f32*)(work + 0x14));
    baseObj = *(void**)(work + 0x20);
    if (baseObj != 0) {
        base.x = *(f32*)((s32)baseObj + 0x8C);
        base.y = *(f32*)((s32)baseObj + 0x90);
        base.z = *(f32*)((s32)baseObj + 0x94);
        ownerY = *(f32*)((s32)baseObj + 0x150);
    } else {
        baseObj = marioGetPtr();
        base.x = *(f32*)((s32)baseObj + 0x8C);
        base.y = *(f32*)((s32)baseObj + 0x90);
        base.z = *(f32*)((s32)baseObj + 0x94);
        ownerY = *(f32*)((s32)baseObj + 0x1BC);
    }

    PSMTXRotAxisRad(axisMtx, &axis, float_deg2rad_80426f6c * -*(f32*)(work + 0x10));
    *(f32*)(work + 4) = axisMtx[0][1] * dist + base.x;
    *(f32*)(work + 8) = axisMtx[1][1] * dist + base.y + ownerY;
    *(f32*)(work + 0xC) = axisMtx[2][1] * dist + base.z;
    *(f32*)(work + 0x18) = *(f32*)(work + 0x10);
    return entry;
}

