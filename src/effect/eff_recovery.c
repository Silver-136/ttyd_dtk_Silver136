#include "effect/eff_recovery.h"

typedef f32 Mtx[3][4];

typedef struct RecoveryEffAnimDt {
    u32 unk0;
    f32 end_frame;
    void* rst_dt;
} RecoveryEffAnimDt;

extern void PSMTXTrans(Mtx, f32, f32, f32);
extern void PSMTXScale(Mtx, f32, f32, f32);
extern void PSMTXConcat(Mtx, Mtx, Mtx);
extern void PSMTXRotRad(Mtx, f32, char);
extern void GXLoadPosMtxImm(Mtx, s32);
extern void* gpGlobals;
extern void effDeleteMayaAnim(void* anim);
extern void effDelete(void* effect);
extern double cos(double);
extern double sin(double);
extern f32 float_0_80422dcc;
extern f32 float_2_80422dd8;
extern f32 float_6p2832_80422e20;
extern f32 float_360_80422e24;
extern void* camGetPtr(s32);
extern void effGetTexObj(s32, void*);
extern void GXLoadTexObj(void*, s32);
extern void GXSetCurrentMtx(u32);
extern void GXSetNumTexGens(u32);
extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
extern void GXSetNumChans(u32);
extern void GXSetNumTevStages(u32);
extern void GXSetTevOrder(u32, u32, u32, s32);
extern void GXSetCullMode(s32);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32, s32);
extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
extern void GXBegin(s32, s32, s16);

void* effRecoveryEntry(s32 type, s32 amount, f32 x, f32 y, f32 z) {
    typedef struct Vec { f32 x,y,z; } Vec;
    typedef struct Work {
        s8 type, amount, state, pad;
        s32 alpha;
        Vec pos, velocity, scale;
        s16 timer, pad2;
        f32 angle;
        s32 retrace;
        void* anim;
        s32 delay;
        f32 height;
    } Work;
    extern void* effEntry(void);
    extern void* __memAlloc(s32,u32);
    extern void* effMayaAnimAlloc(void*);
    extern void effPlayMayaAnim(void*);
    extern void psndSFXOn_3D(char*,Vec*);
    extern s32 irand(s32);
    extern u8 effRecoveryMain(void), effRecoveryMain2(void*);
    extern RecoveryEffAnimDt recovery_effAnimDt;
    extern f32 float_neg30_80422e38, float_neg15_80422e3c;
    extern f32 float_1p5_80422e40, float_0p3_80422e44, float_1p8_80422e48;
    extern f32 vec3_802f3b20[];
    void* entry;
    Work* work;
    Work* part;
    Vec sfxPos, pos;
    s32 i, angle, delay;
    f32 zero;
    entry=effEntry();
    switch(type) {
    case 2: case 3: amount=0; break;
    case 4: case 5: amount=20; break;
    case 6: amount=11; break;
    }
    *(char**)((s32)entry+0x14)="Recovery";
    *(s32*)((s32)entry+8)=amount+1;
    work=__memAlloc(3,*(s32*)((s32)entry+8)*0x44);
    *(Work**)((s32)entry+0xC)=work;
    switch(type) {
    case 6: *(void**)((s32)entry+0x10)=effRecoveryMain2; break;
    default: *(void**)((s32)entry+0x10)=effRecoveryMain; break;
    }
    work->anim=effMayaAnimAlloc(&recovery_effAnimDt);
    effPlayMayaAnim(work->anim);
    work->type=type; work->amount=amount;
    work->pos.x=x; work->pos.y=y; work->pos.z=z;
    work->timer=0;
    work->retrace=*(s32*)((s32)gpGlobals+0x1C);
    work->alpha=255; work->height=float_neg30_80422e38;
    pos=*(Vec*)vec3_802f3b20;
    pos.x=work->pos.x; pos.y=work->pos.y; pos.z=work->pos.z;
    sfxPos=pos;
    switch(work->type) {
    case 0: case 7:
        if(work->amount<6) psndSFXOn_3D("SFX_HP_RECOVER_SMALL1",&sfxPos);
        else if(work->amount<11) psndSFXOn_3D("SFX_HP_RECOVER_MID1",&sfxPos);
        else psndSFXOn_3D("SFX_HP_RECOVER_BIG1",&sfxPos);
        break;
    case 1: case 8:
        if(work->amount<6) psndSFXOn_3D("SFX_FP_RECOVER_SMALL1",&sfxPos);
        else if(work->amount<11) psndSFXOn_3D("SFX_FP_RECOVER_MID1",&sfxPos);
        else psndSFXOn_3D("SFX_FP_RECOVER_BIG1",&sfxPos);
        break;
    case 4: psndSFXOn_3D("SFX_HP_RECOVER_BIG1",&sfxPos); break;
    case 5: psndSFXOn_3D("SFX_FP_RECOVER_BIG1",&sfxPos); break;
    }
    if(type==6) { effDeleteMayaAnim(work->anim); work->timer=180; }
    zero=float_0_80422dcc;
    delay=8;
    part=work+1;
    for(i=1;i<*(s32*)((s32)entry+8);i++,part++,delay+=8) {
        part->pos.x=zero;
        if(type<6 && type>3) part->pos.y=i<10 ? zero : float_neg15_80422e3c;
        else part->pos.y=float_0_80422dcc;
        part->pos.z=zero;
        part->scale.x=zero; part->scale.y=zero; part->scale.z=zero;
        part->velocity.x=zero; part->velocity.y=zero; part->velocity.z=zero;
        part->timer=0; part->state=0;
        part->alpha=255; part->angle=zero;
        switch(type) {
        case 6:
            if(i==1) {
                part->scale.x=part->scale.y=part->scale.z=float_1p5_80422e40;
                part->alpha=0; part->delay=0;
            } else {
                part->scale.x=part->scale.y=part->scale.z=float_0p3_80422e44;
                if((i&1)==0) angle=-irand(80); else angle=irand(80);
                part->angle=(f32)(angle+90);
                part->velocity.x=float_1p8_80422e48*(f32)cos((float_6p2832_80422e20*part->angle)/float_360_80422e24);
                part->velocity.y=float_1p8_80422e48*(f32)sin((float_6p2832_80422e20*part->angle)/float_360_80422e24);
                part->alpha=255; part->delay=delay;
            }
            break;
        }
    }
    return entry;
}

void effRecoveryMain(void* effect) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void effCalcMayaAnim(void* anim);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 camera,s32 layer,void* callback,void* param,f32 z);
    extern void effRecoveryDisp(void);
    extern void effRecoveryDisp2(void);
    extern void effRecoveryDisp3(s32 cameraId, void* effect);
    extern f32 intplGetValue(f32 start,f32 end,s32 mode,s32 current,s32 total);
    extern f32 distABf(f32,f32,f32,f32);
    extern s32 rand(void);
    extern void* effStardustEntry(f64 x, f64 y, f64 z, f64 a, f64 b, s32 c, s32 d, s32 e);
    extern void psndSFXOn_3D(char* sound, Vec* pos);
    extern f32 vec3_802f3b2c[];
    extern f32 float_20_80422df0;
    extern f32 float_30_80422e34;
    char* work;
    char* part;
    Vec pos;
    Vec pos2;
    Vec pos3;
    Vec shinePos;
    Vec sfxPos;
    u32 type;
    s32 delta;
    s32 i;
    s32 finished;
    s32 count;
    s32 doneCount = 0;
    f32 angle;
    f32 radius;
    f32 x;
    f32 y;
    f32 z;
    f32 height;

    work = *(char**)((char*)effect + 0x0C);
    type = *(u8*)work;
    height = *(f32*)(work + 0x40);
    effCalcMayaAnim(*(void**)(work + 0x38));
    delta = 0;
    if (*(u32*)(work + 0x34) < *(u32*)((char*)gpGlobals + 4)) {
        delta = *(u32*)((char*)gpGlobals + 4) - *(u32*)(work + 0x34);
    }
    if (delta == 0) delta = 1;
    finished = 0;

    while (delta-- > 0) {
        work = *(char**)((char*)effect + 0xC);
        x = *(f32*)(work + 0x8);
        y = *(f32*)(work + 0xC);
        z = *(f32*)(work + 0x10);
        if (++*(s16*)(work + 0x2C) > 10) {
            finished = 1;
        }
        count = (type == 4 || type == 5) ? 10 : *(u8*)(work + 1);
        part = work;
        for (i = 1; i < *(s32*)((char*)effect + 8); i++, part += 0x44) {
            switch (*(s8*)(part + 0x46)) {
                case 0:
                    if (finished) {
                        angle = 0.017453292f * (90.0f + 360.0f * (f32)(i - 1) / (f32)count);
                        *(f32*)(part + 0x58) = 12.0f * (f32)sin(angle);
                        *(f32*)(part + 0x5C) = 0.0f;
                        *(f32*)(part + 0x60) = -12.0f * (f32)cos(angle);
                        *(s16*)(part + 0x70) = 0;
                        *(s8*)(part + 0x46) += 1;
                    }
                    break;
                case 1:
                    if (*(s16*)(part + 0x70) < 10) {
                        *(f32*)(part + 0x58) *= 0.7f;
                        *(f32*)(part + 0x5C) *= 0.7f;
                        *(f32*)(part + 0x60) *= 0.7f;
                        *(f32*)(part + 0x4C) += *(f32*)(part + 0x58);
                        *(f32*)(part + 0x50) += *(f32*)(part + 0x5C);
                        *(f32*)(part + 0x54) += *(f32*)(part + 0x60);
                        *(f32*)(part + 0x64) = 1.0f;
                        *(f32*)(part + 0x68) = 1.0f;
                        *(f32*)(part + 0x6C) = 1.0f;
                        *(s16*)(part + 0x70) += 1;
                    } else {
                        *(s16*)(part + 0x70) = 10;
                        *(s8*)(part + 0x46) += 1;
                    }
                    break;
                case 2:
                    if (--*(s16*)(part + 0x70) < 1) {
                        *(s16*)(part + 0x70) = 0;
                        *(s8*)(part + 0x46) += 1;
                    }
                    break;
                case 3:
                    if (*(s16*)(part + 0x70) < 40) {
                        *(s16*)(part + 0x70) += 1;
                        angle = intplGetValue(0.0f,360.0f,11,*(s16*)(part+0x70),40);
                        radius = distABf(0.0f,0.0f,*(f32*)(part+0x4C),*(f32*)(part+0x54)) * 0.99f;
                        angle = 0.017453292f * (90.0f + angle + 360.0f * (f32)(i-2)/(f32)count);
                        *(f32*)(part+0x4C)=radius*(f32)sin(angle);
                        *(f32*)(part+0x54)=-radius*(f32)cos(angle);
                    } else {
                        *(s16*)(part+0x70)=rand()%20;
                        *(s8*)(part+0x46)+=1;
                    }
                    break;
                case 4:
                    if (--*(s16*)(part+0x70)<1) {
                        *(s16*)(part+0x70)=0;
                        *(s8*)(part+0x46)+=1;
                    }
                    break;
                case 5:
                    if (*(s16*)(part+0x70)<16) {
                        *(s16*)(part+0x70)+=1;
                        *(f32*)(part+0x4C)=intplGetValue(*(f32*)(part+0x4C),0.0f,1,*(s16*)(part+0x70),16);
                        *(f32*)(part+0x50)=intplGetValue(*(f32*)(part+0x50),height,1,*(s16*)(part+0x70),16);
                        *(f32*)(part+0x54)=intplGetValue(*(f32*)(part+0x54),0.0f,1,*(s16*)(part+0x70),20);
                        *(f32*)(part+0x64)=intplGetValue(1.0f,0.0f,1,*(s16*)(part+0x70),16);
                        *(f32*)(part+0x68)=*(f32*)(part+0x64);
                        *(f32*)(part+0x6C)=*(f32*)(part+0x64);
                    } else {
                        *(s8*)(part+0x46)+=1;
                    }
                    break;
                case 6:
                    doneCount++;
                    break;
            }
        }
        if ((type < 2 || type - 7 < 2 || type == 4 || type == 5) && doneCount >= count) break;
    }

    if ((type < 2 || type == 7 || type == 8 || type == 4 || type == 5) && doneCount >= count) {
        work = *(char**)((char*)effect + 0xC);
        if (type != 7 && type != 8) {
            effStardustEntry(x, y - float_20_80422df0, z,
                             float_30_80422e34, float_30_80422e34, 2, 2, 0x1E);

            shinePos.x = *(f32*)(work + 8);
            shinePos.y = *(f32*)(work + 0xC);
            shinePos.z = *(f32*)(work + 0x10);
            sfxPos = shinePos;

            switch (*(u8*)work) {
                case 0:
                    if (*(s8*)(work + 1) < 6) {
                        psndSFXOn_3D("SFX_HP_RECOVER_SHINE1", &sfxPos);
                    } else if (*(s8*)(work + 1) < 11) {
                        psndSFXOn_3D("SFX_HP_RECOVER_SHINE2", &sfxPos);
                    } else {
                        psndSFXOn_3D("SFX_HP_RECOVER_SHINE3", &sfxPos);
                    }
                    break;
                case 1:
                    if (*(s8*)(work + 1) < 6) {
                        psndSFXOn_3D("SFX_FP_RECOVER_SHINE1", &sfxPos);
                    } else if (*(s8*)(work + 1) < 11) {
                        psndSFXOn_3D("SFX_FP_RECOVER_SHINE2", &sfxPos);
                    } else {
                        psndSFXOn_3D("SFX_FP_RECOVER_SHINE3", &sfxPos);
                    }
                    break;
                case 4:
                    psndSFXOn_3D("SFX_HP_RECOVER_SHINE3", &sfxPos);
                    break;
                case 5:
                    psndSFXOn_3D("SFX_FP_RECOVER_SHINE3", &sfxPos);
                    break;
            }
        }

        effDeleteMayaAnim(*(void**)(work + 0x38));
        effDelete(effect);
        return;
    }

    work = *(char**)((char*)effect + 0xC);
    *(u32*)(work + 0x34) = *(u32*)((char*)gpGlobals + 4);
    pos.x=*(f32*)(work+8); pos.y=*(f32*)(work+0xC); pos.z=*(f32*)(work+0x10);
    dispEntry(4,1,effRecoveryDisp,effect,dispCalcZ(&pos));
    if (type<2 || type==7 || type==8) {
        pos2.x=*(f32*)(work+8); pos2.y=*(f32*)(work+0xC); pos2.z=*(f32*)(work+0x10);
        dispEntry(4,2,effRecoveryDisp2,effect,dispCalcZ(&pos2));
    }
    if (type==4 || type==5) {
        pos3.x=*(f32*)(work+8); pos3.y=*(f32*)(work+0xC); pos3.z=*(f32*)(work+0x10);
        dispEntry(4,2,effRecoveryDisp3,effect,dispCalcZ(&pos3));
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effRecoveryMain2(void* effEntry) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct Work {
        s32 type, alpha;
        Vec pos, velocity, scale;
        s16 timer, pad;
        f32 angle;
        s32 start;
        void* anim;
        s32 delay, unk40;
    } Work;
    extern void* effStardustEntry(f64, f64, f64, f64, f64, s32, s32, s32);
    extern f32 dispCalcZ(Vec*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern u8 effRecoveryDisp4(void);
    extern f32 float_40_80422e14, float_45_80422e18, float_0p95_80422e1c;
    extern f32 vec3_802f3b5c[];
    Vec drawPos, pos;
    Work* work;
    s32 i;
    f32 friction, zero;
    work = *(Work**)((s32)effEntry + 0xC);
    pos = *(Vec*)vec3_802f3b5c;
    pos.x = work->pos.x; pos.y = work->pos.y; pos.z = work->pos.z;
    drawPos = pos;
    if (work->timer < 1000) work->timer--;
    if (work->timer < 32) work->alpha = work->timer << 3;
    if (work->timer < 0) {
        effStardustEntry(work->pos.x, work->pos.y, work->pos.z, float_40_80422e14, float_45_80422e18, 2, 6, 30);
        effDelete(effEntry);
    } else {
        work++;
        for (i = 1; i < *(s32*)((s32)effEntry + 8); i++, work++) {
            if (work->delay != 0) {
                work->delay--;
            } else {
                friction = float_0p95_80422e1c;
                work->pos.x += work->velocity.x;
                work->pos.y += work->velocity.y;
                work->pos.z += work->velocity.z;
                work->velocity.x *= friction;
                work->velocity.y *= friction;
                work->velocity.z *= friction;
                if (i == 1) {
                    work->alpha += 5;
                    if (work->alpha > 128) work->alpha = 128;
                } else {
                    work->alpha -= 8;
                    if (work->alpha < 0) {
                        zero = float_0_80422dcc;
                        work->pos.x = zero; work->pos.y = zero; work->pos.z = zero;
                        work->velocity.x = float_2_80422dd8 * (f32)cos((float_6p2832_80422e20 * work->angle) / float_360_80422e24);
                        work->velocity.y = float_2_80422dd8 * (f32)sin((float_6p2832_80422e20 * work->angle) / float_360_80422e24);
                        work->alpha = 255;
                    }
                }
            }
        }
        dispEntry(4, 2, effRecoveryDisp4, effEntry, dispCalcZ(&drawPos));
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 effRecoveryDisp(int cameraId, int effect) {
    typedef struct Tex { u32 data[8]; } Tex;
    typedef struct Color { u8 r, g, b, a; } Color;
    extern void GXSetTevColor(s32, void*);
    extern u8 color_rotation_data[];
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    u8* work;
    void* cam;
    Tex tex;
    Color color, colorCopy;
    Mtx billboard, scale, rot, trans;
    volatile f32* fifo;
    u32 type;
    s32 i;
    u32 colorIndex;

    work = *(u8**)(effect + 0xC);
    cam = camGetPtr(cameraId);
    type = *work;
    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 0.017453292f * -*(f32*)((s32)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(trans, rot, billboard);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumChans(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    switch (type) {
    case 0: case 2: case 4: case 7:
        effGetTexObj(0x12, &tex); GXLoadTexObj(&tex, 0);
        effGetTexObj(0x14, &tex); GXLoadTexObj(&tex, 1);
        break;
    case 1: case 3: case 5: case 8:
        effGetTexObj(0x13, &tex); GXLoadTexObj(&tex, 0);
        effGetTexObj(0x15, &tex); GXLoadTexObj(&tex, 1);
        break;
    }

    work = *(u8**)(effect + 0xC);
    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 0.017453292f * -*(f32*)((s32)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(trans, rot, trans);
    PSMTXScale(scale, 10.0f, 10.0f, 10.0f);
    PSMTXConcat(trans, scale, trans);
    PSMTXTrans(scale, 0.0f, 2.0f, 0.0f);
    PSMTXConcat(trans, scale, trans);
    if ((type < 2) || (type - 7 < 2) || (type == 4) || (type == 5)) {
        PSMTXConcat(trans, (void*)(*(s32*)(work + 0x38) + 0x14), trans);
    }
    PSMTXConcat((void*)((s32)cam + 0x48), trans, trans);
    PSMTXScale(scale, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(trans, scale, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXBegin(0x80, 0, 4);
    fifo = (volatile f32*)0xCC008000;
    *fifo=-16.0f; *fifo=16.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
    *fifo=16.0f; *fifo=16.0f; *fifo=0.0f; *fifo=1.0f; *fifo=0.0f;
    *fifo=16.0f; *fifo=-16.0f; *fifo=0.0f; *fifo=1.0f; *fifo=1.0f;
    *fifo=-16.0f; *fifo=-16.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;

    if ((type != 2) && (type != 3)) {
        GXSetBlendMode(0, 1, 0, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(1, 3, 1);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 1, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
        GXSetTevAlphaIn(0, 7, 0, 4, 7);
        for (i = 1; i < *(s32*)(effect + 8); i++, work += 0x44) {
            colorIndex = ((u32)(i + *(s32*)((s32)gpGlobals + 4)) % 12) * 3;
            color.r = color_rotation_data[colorIndex];
            color.g = color_rotation_data[colorIndex + 1];
            color.b = color_rotation_data[colorIndex + 2];
            color.a = *(s32*)(work + 0x48);
            colorCopy = color;
            GXSetTevColor(1, &color);
            PSMTXTrans(trans, *(f32*)(work + 0x4C), *(f32*)(work + 0x50), *(f32*)(work + 0x54));
            PSMTXScale(scale, *(f32*)(work + 0x64), *(f32*)(work + 0x68), *(f32*)(work + 0x6C));
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(billboard, trans, trans);
            PSMTXConcat((void*)((s32)camGetPtr(cameraId) + 0x48), trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);
            *fifo=-6.0f; *fifo=6.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
            *fifo=6.0f; *fifo=6.0f; *fifo=0.0f; *fifo=2.0f; *fifo=0.0f;
            *fifo=6.0f; *fifo=-6.0f; *fifo=0.0f; *fifo=2.0f; *fifo=1.0f;
            *fifo=-6.0f; *fifo=-6.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
        }
    }
    return 0;
}

void effRecoveryDisp2(s32 cameraId, void* effEntry) {
    typedef f32 LocalMtx[3][4];
    extern s32 sprintf(char* str, char* fmt, ...);
    extern u32 strlen(char* str);
    extern void PSMTXRotRad(LocalMtx mtx, f32 rad, s32 axis);
    extern void effNumberGX(LocalMtx mtx, s32 colorType, s32 value, s32 alpha, s32 flag);
    extern char str_PCTd_80422df8[];
    extern f32 float_deg2rad_80422dc0;
    extern f32 float_1_80422dd0;
    extern f32 float_0p5_80422ddc;
    extern f32 float_10_80422de4;
    extern f32 float_0p035_80422dfc;
    extern f32 float_24_80422e00;
    LocalMtx textMtx;
    LocalMtx rotMtx;
    LocalMtx tmpMtx;
    void* work;
    char buf[32];
    s32 type;
    s32 value;
    u32 len;

    work = *(void**)((s32)effEntry + 0xC);
    camGetPtr(cameraId);
    value = *(s8*)((s32)work + 0x1);
    type = *(s8*)((s32)work + 0x0);

    sprintf(buf, str_PCTd_80422df8, value);
    len = strlen(buf);

    PSMTXTrans(textMtx, *(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10));
    PSMTXRotRad(rotMtx, float_deg2rad_80422dc0 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(textMtx, rotMtx, textMtx);
    PSMTXTrans(tmpMtx, float_0_80422dcc, float_0_80422dcc, float_1_80422dd0);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXScale(tmpMtx, float_10_80422de4, float_10_80422de4, float_10_80422de4);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXTrans(tmpMtx, float_0_80422dcc, float_2_80422dd8, float_0_80422dcc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXConcat(textMtx, (void*)(*(s32*)((s32)work + 0x38) + 0x14), textMtx);
    PSMTXScale(tmpMtx, float_0p035_80422dfc, float_0p035_80422dfc, float_0p035_80422dfc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXTrans(tmpMtx, float_24_80422e00 * (f32)-(s32)(len - 1) * float_0p5_80422ddc, float_0_80422dcc, float_0_80422dcc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);

    switch (type) {
        case 0:
        case 2:
        case 7:
            effNumberGX(textMtx, 0, value, 0xFF, 1);
            break;
        case 1:
        case 3:
        case 8:
            effNumberGX(textMtx, 1, value, 0xFF, 1);
            break;
    }
}

void effRecoveryDisp3(s32 cameraId, void* effect) {
    typedef struct Tex {
        u32 data[8];
    } Tex;
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern u32 dat_80422db0;
    extern u32 dat_80422db4;
    extern u32 dat_80422db8;
    extern f32 float_deg2rad_80422dc0;
    extern f32 float_1_80422dd0;
    extern f32 float_0p5_80422ddc;
    extern f32 float_10_80422de4;
    extern f32 float_0p05_80422de8;
    extern f32 float_108_80422dec;
    extern f32 float_20_80422df0;
    extern f32 float_neg20_80422df4;
    Mtx trans;
    Mtx rotate;
    Mtx temp;
    Mtx model;
    Tex tex;
    volatile f32* fifo;
    char* work;
    char* camera;
    s32 type;
    s32 i;
    u32 color;
    f32 left;
    f32 right;
    f32 top;
    f32 bottom;

    work = *(char**)((char*)effect + 0xC);
    camera = (char*)camGetPtr(cameraId);
    type = *(s8*)work;

    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rotate, float_deg2rad_80422dc0 * -*(f32*)((char*)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(trans, rotate, trans);
    PSMTXTrans(temp, float_0_80422dcc, float_0_80422dcc, float_1_80422dd0);
    PSMTXConcat(trans, temp, trans);
    PSMTXScale(temp, float_10_80422de4, float_10_80422de4, float_10_80422de4);
    PSMTXConcat(trans, temp, trans);
    PSMTXTrans(temp, float_0_80422dcc, float_2_80422dd8, float_0_80422dcc);
    PSMTXConcat(trans, temp, trans);
    PSMTXConcat(trans, (f32 (*)[4])(*(s32*)(work + 0x38) + 0x14), model);
    PSMTXConcat((f32 (*)[4])(camera + 0x11C), model, model);
    PSMTXScale(temp, float_0p05_80422de8, float_0p05_80422de8, float_0p05_80422de8);
    PSMTXConcat(model, temp, model);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xA, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    fifo = (volatile f32*)0xCC008000;
    left = -float_108_80422dec * float_0p5_80422ddc;
    right = left + float_108_80422dec;
    top = float_20_80422df0;
    bottom = float_neg20_80422df4;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            if (*(s32*)((char*)gpGlobals + 0x16C) == 4) {
                effGetTexObj(0xAD, &tex);
            } else {
                effGetTexObj(0x6F, &tex);
            }
            GXLoadTexObj(&tex, 0);
            color = dat_80422db0;
        } else {
            if (*(s32*)((char*)gpGlobals + 0x16C) == 4) {
                effGetTexObj(0xAC, &tex);
            } else {
                effGetTexObj(0x6E, &tex);
            }
            GXLoadTexObj(&tex, 0);
            if (type == 4) {
                color = dat_80422db4;
            } else {
                color = dat_80422db8;
            }
        }
        GXSetChanMatColor(4, &color);

        PSMTXTrans(temp, (f32)((1 - i) * 2), (f32)((1 - i) * -2), float_0_80422dcc);
        PSMTXConcat(model, temp, temp);
        GXLoadPosMtxImm(temp, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);

        *fifo = left;
        *fifo = top;
        *fifo = float_0_80422dcc;
        *fifo = float_0_80422dcc;
        *fifo = float_0_80422dcc;
        *fifo = right;
        *fifo = top;
        *fifo = float_0_80422dcc;
        *fifo = float_1_80422dd0;
        *fifo = float_0_80422dcc;
        *fifo = right;
        *fifo = bottom;
        *fifo = float_0_80422dcc;
        *fifo = float_1_80422dd0;
        *fifo = float_1_80422dd0;
        *fifo = left;
        *fifo = bottom;
        *fifo = float_0_80422dcc;
        *fifo = float_0_80422dcc;
        *fifo = float_1_80422dd0;
    }
}


void effRecoveryDisp4(s32 cameraId, void* effect) {
    typedef struct Tex { u32 data[8]; } Tex;
    typedef struct Color { u8 r, g, b, a; } Color;
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetChanMatColor(s32, void*);
    extern u32 dat_80422dbc;
    extern u8 color_rotation_data[];
    extern f32 float_deg2rad_80422dc0;
    extern f32 float_32_80422dc4;
    Mtx trans;
    Mtx rotate;
    Mtx scale;
    Mtx model;
    Tex tex;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    char* part = *(char**)((char*)effect + 0x0C);
    char* camera = (char*)camGetPtr(cameraId);
    s32 baseAlpha = *(s32*)(part + 4);
    Color color0;
    Color color1;
    Color color2;
    Color color3;
    Color color4;
    s32 alpha;
    s32 i;
    f32 u;
    f32 yoff;
    f32 left;

    PSMTXTrans(trans, *(f32*)(part + 8), *(f32*)(part + 0xC), *(f32*)(part + 0x10));
    PSMTXRotRad(rotate, float_deg2rad_80422dc0 * -*(f32*)((char*)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(trans, rotate, model);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    left = -float_32_80422dc4;
    part += 0x44;
    for (i = 1; i < *(s32*)((char*)effect + 8); i++, part += 0x44) {
        if (*(s32*)(part + 0x3C) != 0) {
            continue;
        }
        if (i == 1) {
            effGetTexObj(0x12, &tex);
            GXLoadTexObj(&tex, 0);
            alpha = baseAlpha * *(s32*)(part + 4) / 255;
            color0 = *(Color*)&dat_80422dbc;
            color0.a = alpha;
            color1 = color0;
            GXSetChanMatColor(4, &color1);
        } else {
            s32 index;
            alpha = baseAlpha * *(s32*)(part + 4) / 255;
            index = ((i + *(u32*)((char*)gpGlobals + 4)) % 12) * 3;
            color2.r = color_rotation_data[index];
            color2.g = color_rotation_data[index + 1];
            color2.b = color_rotation_data[index + 2];
            color2.a = alpha;
            color3 = color2;
            effGetTexObj(0x14, &tex);
            GXLoadTexObj(&tex, 0);
            color4 = color3;
            GXSetChanMatColor(4, &color4);
        }
        PSMTXTrans(trans, *(f32*)(part + 8), *(f32*)(part + 0xC), *(f32*)(part + 0x10));
        PSMTXScale(scale, *(f32*)(part + 0x20), *(f32*)(part + 0x24), *(f32*)(part + 0x28));
        if (i == 1) {
            PSMTXRotRad(rotate, 0.0f, 'z');
        } else {
            PSMTXRotRad(rotate, 0.017453292f * (*(f32*)(part + 0x30) - 90.0f), 'z');
        }
        PSMTXConcat(model, trans, trans);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(trans, rotate, trans);
        PSMTXConcat((f32 (*)[4])(camera + 0x48), trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);
        u = i == 1 ? 1.0f : 2.0f;
        yoff = i == 1 ? 8.0f : 0.0f;
        *fifo=left*0.5f; *fifo=16.0f+yoff; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
        *fifo=16.0f; *fifo=16.0f+yoff; *fifo=0.0f; *fifo=u; *fifo=0.0f;
        *fifo=16.0f; *fifo=left*0.5f+yoff; *fifo=0.0f; *fifo=u; *fifo=1.0f;
        *fifo=left*0.5f; *fifo=left*0.5f+yoff; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
    }
}

/* Exact Recovery Maya/Hermit payload. */
u8 recovery_effHermit_0[1780] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0xFF, 0xDD,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xEA,
    0xFF, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x40, 0x00, 0x02,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x20,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x4C, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFD, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0x00, 0x14, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x4C, 0xFF, 0xFE, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x26, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x40, 0xFF, 0xFE, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x36,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

/* Exact Recovery small-data tables and nested Maya descriptors. */
__declspec(section ".sdata") static u8 color_rotation_data[36] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE, 0xB4, 0xB4,
    0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xB4, 0xFE, 0xB4, 0xD5,
    0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC
};

__declspec(section ".sdata") u8 recovery_effFace_pPlane2_q007_0[42] = {
    0x00, 0x04, 0xFD, 0x00, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00,
    0xFD, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") void* recovery_effFaceDt_pPlane2_q007 =
    recovery_effFace_pPlane2_q007_0;

__declspec(section ".sdata") u32 recovery_effAnimRST[4] = {
    10,
    (u32)recovery_effHermit_0,
    1,
    (u32)&recovery_effFaceDt_pPlane2_q007,
};

__declspec(section ".sdata") u32 recovery_effAnimRSTDt[2] = {
    1,
    (u32)recovery_effAnimRST,
};

__declspec(section ".sdata") RecoveryEffAnimDt recovery_effAnimDt = {
    0,
    79.0f,
    recovery_effAnimRSTDt,
};

/* Exact Recovery zero-Vec3 read-only prefix. */
__declspec(section ".rodata") f32 vec3_802f3b20[3] = {
    0.0f, 0.0f, 0.0f,
};

__declspec(section ".rodata") f32 vec3_802f3b2c[3] = {
    0.0f, 0.0f, 0.0f,
};

__declspec(section ".rodata") f32 vec3_802f3b38[3] = {
    0.0f, 0.0f, 0.0f,
};

__declspec(section ".rodata") f32 vec3_802f3b44[3] = {
    0.0f, 0.0f, 0.0f,
};

__declspec(section ".rodata") f32 vec3_802f3b50[3] = {
    0.0f, 0.0f, 0.0f,
};

__declspec(section ".rodata") f32 vec3_802f3b5c[3] = {
    0.0f, 0.0f, 0.0f,
};

