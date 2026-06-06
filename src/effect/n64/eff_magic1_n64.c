#include "effect/n64/eff_magic1_n64.h"

typedef f32 Mtx[3][4];

void* camGetPtr(s32 cameraId);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx mtx, s32 axis, f32 rad);
void PSMTXConcat(void* a, void* b, void* ab);

extern f32 float_0_804259cc;
extern f32 float_deg2rad_804259d0;


u8 main_dl(void* effEntry, Mtx mtx) {
    return 0;
}


u8 effMagic1Main(u32* param_1) {
    return 0;
}


void* effMagic1N64Entry(double param_1, double param_2, double param_3, double param_4, double param_5, double param_6, int param_7, u32 param_8) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Disp(s32 cameraId, void* effEntry) {
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    Mtx loopRot;
    register s32* work;
    register void* cam;
    register s32 type;
    f32 angle;
    s32 i;
    s32 rotAngle;

    cam = camGetPtr(cameraId);
    work = *(s32**)((s32)effEntry + 0xC);
    type = work[0];

    switch (type) {
        case 0:
            angle = float_0_804259cc;
            break;
        default:
            angle = -*(f32*)((s32)camGetPtr(4) + 0x114);
            break;
    }

    PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_804259d0 * angle);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);

    switch (type) {
        case 0:
            main_dl(effEntry, mtx);
            break;
        case 1:
            angle = float_deg2rad_804259d0;
            i = 0;
            rotAngle = 0;
            while (i < 12) {
                PSMTXRotRad(loopRot, 0x7A, angle * (f32)rotAngle);
                PSMTXConcat(mtx, loopRot, loopRot);
                main_dl(effEntry, loopRot);
                i++;
                rotAngle += 30;
            }
            break;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
