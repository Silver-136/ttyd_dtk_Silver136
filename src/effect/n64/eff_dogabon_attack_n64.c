#include "effect/n64/eff_dogabon_attack_n64.h"


#pragma optimize_for_size off
#pragma optimize_for_size off
void effDogabonAttackMain(void* effect) {
    extern void effDelete(void*);
    extern float dispCalcZ(void*);
    extern void dispEntry(int, int, void*, void*, float);
    extern void effDogabonAttackDisp(int, void*);
    extern unsigned char scale_data[];
    extern unsigned char scale_data_2[];
    extern unsigned char move_data_1[];
    extern unsigned char move_data_2[];
    extern unsigned char move_data_hazure[];
    extern signed char y_data[];
    extern double sin(double);
    extern double cos(double);
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* part = work;
    float pos[3];
    int type;
    int frame;
    int i;

    pos[0] = *(float*)(work + 4);
    pos[1] = *(float*)(work + 8);
    pos[2] = *(float*)(work + 0xC);
    type = *(int*)work;
    if ((*(unsigned int*)entry & 4) != 0) {
        *(unsigned int*)entry &= ~4;
        *(int*)(work + 0x10) = 0x40;
    }
    if (*(int*)(work + 0x10) < 1000) {
        (*(int*)(work + 0x10))--;
    }
    frame = ++*(int*)(work + 0x14);
    if (*(int*)(work + 0x10) < 0) {
        effDelete(effect);
        return;
    }

    for (i = 0; i < 25; i++, part += 4) {
        float startX = *(float*)(part + 0x158);
        float startY = *(float*)(part + 0x1BC);
        float startZ = *(float*)(part + 0x220);
        int state = *(int*)(part + 0x6D0);
        int tick = *(int*)(part + 0x734);
        float pulse = 0.01f * (float)scale_data[frame % 7];

        *(float*)(part + 0x414) = pulse;
        *(float*)(part + 0x3B0) = 2.0f - pulse;
        switch (state) {
            case 0:
                *(float*)(part + 0x2C) = startX;
                *(float*)(part + 0x90) = startY;
                *(float*)(part + 0xF4) = startZ;
                if (*(int*)(part + 0x66C) == 0) {
                    float intro = 0.01f * (float)scale_data_2[tick];
                    *(int*)(part + 0x6D0) = 1;
                    *(float*)(part + 0x414) = intro;
                    *(float*)(part + 0x3B0) = intro;
                    *(int*)(part + 0x734) = tick + 1;
                } else {
                    (*(int*)(part + 0x66C))--;
                }
                break;
            case 1: {
                float intro = 0.01f * (float)scale_data_2[tick];
                *(float*)(part + 0x414) = intro;
                *(float*)(part + 0x3B0) = intro;
                tick++;
                *(int*)(part + 0x734) = tick;
                if (tick > 7) {
                    *(int*)(part + 0x6D0) = type == 0 ? 2 : 20;
                    *(int*)(part + 0x734) = 0;
                }
                break;
            }
            case 2: {
                float t = 0.01f * (float)move_data_1[tick];
                *(float*)(part + 0x2C) = startX + t * (*(float*)(part + 0x284) - startX);
                *(float*)(part + 0x90) = startY + t * (*(float*)(part + 0x2E8) - startY);
                *(float*)(part + 0xF4) = startZ + t * (*(float*)(part + 0x34C) - startZ);
                *(float*)(part + 0x414) = 1.0f;
                *(float*)(part + 0x3B0) = 1.0f;
                if (++*(int*)(part + 0x734) > 14) {
                    *(int*)(part + 0x6D0) = 3;
                    *(int*)(part + 0x734) = 0;
                }
                break;
            }
            case 3:
                if (i == 24) {
                    int j;
                    for (j = 0; j < 25; j++) {
                        ((int*)(work + 0x6D0))[j] = 4;
                    }
                }
                break;
            case 4: {
                float t = 0.01f * (float)move_data_2[tick];
                float ring = (float)(i / 25);
                float angle = (6.283185f * (540.0f * (1.0f - t) *
                              (float)cos((6.283185f * ring) / 360.0f) + (float)i)) / 360.0f;
                float radius = 15.0f / t / t;
                *(float*)(part + 0x2C) = (float)sin(angle) * radius;
                *(float*)(part + 0x90) = (float)y_data[i] + 10.0f;
                *(float*)(part + 0xF4) = (float)cos(angle) * radius;
                tick++;
                *(int*)(part + 0x734) = tick;
                if (tick > 0x45) {
                    *(int*)(part + 0x6D0) = 5;
                    *(int*)(part + 0x734) = 0;
                    *(float*)(part + 0x540) = 0.5f * *(float*)(part + 0x284);
                    *(float*)(part + 0x5A4) = 0.5f * *(float*)(part + 0x2E8);
                    *(float*)(part + 0x608) = 0.5f * *(float*)(part + 0x34C);
                }
                break;
            }
            case 5:
                *(float*)(part + 0x2C) += *(float*)(part + 0x540);
                *(float*)(part + 0x90) += *(float*)(part + 0x5A4);
                *(float*)(part + 0xF4) += *(float*)(part + 0x608);
                *(float*)(part + 0x540) *= 0.99f;
                *(float*)(part + 0x5A4) = *(float*)(part + 0x5A4) * 0.99f - 0.1f;
                *(float*)(part + 0x608) *= 0.99f;
                if (++*(int*)(part + 0x734) > 16) {
                    *(int*)(part + 0x734) = 0;
                    *(int*)(part + 0x6D0) = 6;
                }
                break;
            case 20: {
                float t = 0.01f * (float)move_data_hazure[tick];
                *(float*)(part + 0x2C) = startX + t * (*(float*)(part + 0x284) - startX);
                *(float*)(part + 0x90) = startY + t * (*(float*)(part + 0x2E8) - startY);
                *(float*)(part + 0xF4) = startZ + t * (*(float*)(part + 0x34C) - startZ);
                if (++*(int*)(part + 0x734) > 19) {
                    *(int*)(part + 0x734) = 0;
                    *(int*)(part + 0x6D0) = 5;
                }
                break;
            }
        }
    }
    dispEntry(4, 1, effDogabonAttackDisp, effect, dispCalcZ(pos));
}
#pragma optimize_for_size on

#pragma optimize_for_size on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effDogabonAttackDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern u32 unk_80429680;
    extern u32 unk_80429684;
    extern f32 float_0p03125_80425068;
    extern f32 float_0_8042506c;
    extern f32 float_deg2rad_80425070;
    extern f32 float_1_80425074;
    extern char size12x12_tex32x32_vtx[];
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    u8* work;
    u8* part;
    void* cam;
    u32 color;
    s32 i;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    PSMTXTrans(transMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x28), *(f32*)(work + 0x28), *(f32*)(work + 0x28));
    PSMTXConcat(transMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    color = unk_80429680;
    ((u8*)&color)[0] = *(u8*)(work + 0x18);
    ((u8*)&color)[1] = *(u8*)(work + 0x1C);
    ((u8*)&color)[2] = *(u8*)(work + 0x20);
    ((u8*)&color)[3] = *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    color = unk_80429684;
    GXSetTevColor(2, &color);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 8, 2, 4, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80425068, float_0p03125_80425068, float_0_8042506c);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    effGetTexObjN64(0x8C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(size12x12_tex32x32_vtx);

    part = work;
    for (i = 0; i < 0x19; i++, part += 4) {
        if ((*(s32*)(work + 0x6D0) != 0) && (*(s32*)(work + 0x6D0) != 7)) {
            PSMTXRotRad(rotMtx, float_deg2rad_80425070 * *(f32*)(part + 0x4DC), 'y');
            PSMTXTrans(transMtx, *(f32*)(part + 0x2C), *(f32*)(part + 0x90), *(f32*)(part + 0xF4));
            PSMTXConcat(rotMtx, transMtx, transMtx);
            PSMTXRotRad(rotMtx, float_deg2rad_80425070 * -*(f32*)(part + 0x4DC), 'y');
            PSMTXConcat(transMtx, transMtx, transMtx);
            PSMTXRotRad(rotMtx, float_deg2rad_80425070 * *(f32*)(part + 0x478), 'z');
            PSMTXConcat(transMtx, rotMtx, transMtx);
            PSMTXScale(scaleMtx, *(f32*)(part + 0x3B0), *(f32*)(part + 0x414), float_1_80425074);
            PSMTXConcat(transMtx, scaleMtx, transMtx);
            PSMTXConcat(baseMtx, transMtx, transMtx);
            GXLoadPosMtxImm(transMtx, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3);
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void* effDogabonAttackN64Entry(float x, float y, float z, float scale, int type, int timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(int, int);
    extern void effDogabonAttackMain(void*);
    extern int rand(void);
    extern double sin(double);
    extern double cos(double);
    extern char str_DogabonAttackN64_802fadf8[];
    extern signed char y_data[];
    extern float float_6p2832_80425084;
    extern float float_360_80425088;
    extern float float_15_80425090;
    extern float float_120_804250a0;
    extern float float_0_8042506c;
    unsigned char* entry = (unsigned char*)effEntry();
    int* work;
    int* part;
    int i;
    int phase = 0;
    int delay = 1;

    *(char**)(entry + 0x14) = str_DogabonAttackN64_802fadf8;
    *(int*)(entry + 8) = 1;
    work = (int*)__memAlloc(3, 0x798);
    *(int**)(entry + 0xC) = work;
    *(void**)(entry + 0x10) = effDogabonAttackMain;
    *(unsigned int*)entry |= 2;
    work[0] = type;
    work[5] = 0;
    work[4] = timer < 1 ? 1000 : timer;
    work[9] = 0xFF;
    *(float*)&work[1] = x;
    *(float*)&work[2] = y;
    *(float*)&work[3] = z;
    *(float*)&work[10] = scale;
    work[6] = 0x46;
    work[7] = 0xB4;
    work[8] = 0x78;

    part = work;
    for (i = 0; i < 25; i++, part++, phase += 0x438, delay += 2) {
        float angle = (float_6p2832_80425084 * (float)(phase / 25)) / float_360_80425088;
        float sine = (float)sin(angle);
        float cosine = (float)cos(angle);
        part[0x11E] = (int)(float)((rand() % 360) - 180);
        *(float*)&part[0xA1] = float_15_80425090 * sine;
        *(float*)&part[0xBA] = (float)y_data[i] + 10.0f;
        *(float*)&part[0xD3] = float_15_80425090 * cosine;
        *(float*)&part[0x56] = float_120_804250a0 * sine;
        *(float*)&part[0x6F] = (float)(y_data[i] * 8 + 0x28);
        *(float*)&part[0x88] = float_120_804250a0 * cosine;
        *(float*)&part[0x169] = float_0_8042506c;
        part[0x19B] = delay;
        part[0x1B4] = 0;
        part[0x1CD] = 0;
        *(float*)&part[0x137] = float_0_8042506c;
    }
    return entry;
}
