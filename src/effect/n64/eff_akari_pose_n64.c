#include "effect/n64/eff_akari_pose_n64.h"

const char str_AkariPoseN64_802faaf8[] = "AkariPoseN64";

f32 scale_data[] = {
    0.9f, 1.0f, 1.0f, 0.9f, 0.85f, 0.8f, 0.75f, 0.7f,
    0.65f, 0.6f, 0.55f, 0.5f, 0.45f, 0.4f, 0.35f, 0.3f,
    0.25f, 0.2f, 0.15f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f,
};

f32 scale_data2[] = {
    0.9f, 1.0f, 1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f,
    0.4f, 0.3f, 0.2f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

s32 max_seq_num[] = { 24, 12 };
u8 a_data[] = {
    0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
u8 a_data2[] = {
    0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

extern f32 float_0_80424c50;

void* effAkariPoseN64Entry(s32 type, s32 count, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    typedef struct AkariPoseWork {
        s32 type; f32 x, y, z; f32 moveX, moveY; f32 targetX, targetY;
        s32 sequence; f32 angle; s32 timer; s32 frame; s32 alpha; f32 scale;
    } AkariPoseWork;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effAkariPoseMain(void);
    extern f32 float_0p5_80424c70;
    extern f32 float_neg1_80424c74;
    void* entry;
    AkariPoseWork* work;
    AkariPoseWork* item;
    s32 i;
    s32 blocks;
    s32 remainder;
    s32 value;
    f32 halfSeq;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_AkariPoseN64_802faaf8;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * sizeof(AkariPoseWork));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effAkariPoseMain;
    *(u32*)entry |= 2;
    work->type = type;
    work->frame = 0;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }
    work->alpha = 0xFF;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;

    halfSeq = (f32)max_seq_num[type] * float_0p5_80424c70;
    item = work + 1;
    i = 1;
    if (count + 1 > 1) {
        blocks = (u32)count >> 2;
        if (blocks != 0) {
            do {
                value = (s32)(float_neg1_80424c74 - halfSeq * (f32)(i & 1));
                item[0].sequence = value;
                value = (s32)(float_neg1_80424c74 - halfSeq * (f32)((i + 1) & 1));
                item[1].sequence = value;
                value = (s32)(float_neg1_80424c74 - halfSeq * (f32)((i + 2) & 1));
                item[2].sequence = value;
                value = (s32)(float_neg1_80424c74 - halfSeq * (f32)((i + 3) & 1));
                item[3].sequence = value;
                item += 4;
                i += 4;
            } while (--blocks != 0);
        }
        remainder = count & 3;
        if (remainder != 0) {
            do {
                value = (s32)(float_neg1_80424c74 - halfSeq * (f32)(i & 1));
                item->sequence = value;
                item++;
                i++;
            } while (--remainder != 0);
        }
    }
    return entry;
}


void effAkariPoseMain(void* entry) {
    extern void effDelete(void* entry);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effAkariPoseDisp(void);
    extern f32 float_0p1_80424c54;
    extern f32 float_0p2_80424c58;
    extern f32 float_360_80424c5c;
    extern f32 float_6p2832_80424c60;
    extern f32 float_16_80424c64;
    extern f32 float_45_80424c68;
    extern f32 float_1_80424c6c;
    s32* work = *(s32**)((s32)entry + 0xC);
    f32 pos[3];
    s32 type;
    s32 count;
    s32 tickParity;
    s32 i;
    f32 stepScale;
    f32 easeScale;
    u8* item;

    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    type = work[0];

    if ((*(u32*)entry & 4) != 0) {
        *(u32*)entry &= ~4;
        work[0xA] = 0x10;
    }
    if (work[0xA] < 1000) {
        work[0xA]--;
    }
    work[0xB]++;

    if (work[0xA] < 0) {
        effDelete(entry);
        return;
    }

    if (work[0xA] < 0x10) {
        work[0xC] = work[0xA] << 4;
    }

    if (type == 0) {
        stepScale = float_0p1_80424c54;
        easeScale = float_0p2_80424c58;
    } else {
        stepScale = float_0p2_80424c58;
        easeScale = float_0p2_80424c58;
    }

    count = *(s32*)((s32)entry + 8);
    tickParity = work[0xB] & 1;
    item = (u8*)work + 0x38;
    for (i = 1; i < count; i++, item += 0x38) {
        s32 seq;

        seq = *(s32*)(item + 0x20) + 1;
        *(s32*)(item + 0x20) = seq;
        if (seq >= max_seq_num[type]) {
            seq = 0;
            *(s32*)(item + 0x20) = 0;
        }

        if (*(s32*)(item + 0x20) >= 0) {
            if (*(s32*)(item + 0x20) == 0) {
                f32 angle;
                f32 radians;
                f32 sx;
                f32 cy;

                angle = (float_360_80424c5c * (f32)i) / (f32)(count - 1);
                radians = (float_6p2832_80424c60 * angle) / float_360_80424c5c;
                sx = float_16_80424c64 * (f32)sin((f64)radians);
                cy = float_16_80424c64 * (f32)cos((f64)radians);
                *(f32*)(item + 4) = sx;
                *(f32*)(item + 8) = cy;
                *(f32*)(item + 0x10) = sx * stepScale;
                *(f32*)(item + 0x14) = cy * stepScale;
                *(f32*)(item + 0x18) = float_0_80424c50;
                *(f32*)(item + 0x1C) = float_0_80424c50;
                *(f32*)(item + 0x24) = -angle - float_45_80424c68;
                *(f32*)(item + 0x34) = float_1_80424c6c;
            }

            if (type == 0) {
                *(s32*)(item + 0x30) = a_data[*(s32*)(item + 0x20)];
                *(f32*)(item + 0x34) = scale_data[*(s32*)(item + 0x20)];
            } else {
                *(s32*)(item + 0x30) = a_data2[*(s32*)(item + 0x20)];
                *(f32*)(item + 0x34) = scale_data2[*(s32*)(item + 0x20)];
            }

            if (tickParity == (i & 1)) {
                *(f32*)(item + 4) += *(f32*)(item + 0x10);
                *(f32*)(item + 8) += *(f32*)(item + 0x14);
            } else {
                *(f32*)(item + 4) += float_0p2_80424c58 * *(f32*)(item + 0x10);
                *(f32*)(item + 8) += float_0p2_80424c58 * *(f32*)(item + 0x14);
            }

            *(f32*)(item + 0x10) += easeScale * (*(f32*)(item + 0x18) - *(f32*)(item + 0x10));
            *(f32*)(item + 0x14) += easeScale * (*(f32*)(item + 0x1C) - *(f32*)(item + 0x14));
        }
    }

    dispEntry(4, 2, effAkariPoseDisp, entry, dispCalcZ(pos));
}

void effAkariPoseDisp(int param_1, void* param_2) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern f32 float_deg2rad_80424c48;
    extern f32 float_0p03125_80424c4c;
    extern u8 size16x16_tex32x32_vtx[];
    extern u32 unk_80429640;
    f32 baseMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 viewMtx[3][4];
    u8 texObj[0x20];
    void* cam;
    u8* work;
    u8* item;
    s32 baseAlpha;
    s32 i;
    u32 color;

    cam = camGetPtr(param_1);
    work = *(u8**)((s32)param_2 + 0xC);
    baseAlpha = *(s32*)(work + 0x30);

    PSMTXTrans(baseMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rotMtx, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114) * float_deg2rad_80424c48));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x34), *(f32*)(work + 0x34), *(f32*)(work + 0x34));
    PSMTXConcat(baseMtx, rotMtx, baseMtx);
    PSMTXConcat(baseMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, viewMtx);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80424c4c, float_0p03125_80424c4c, float_0_80424c50);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    effGetTexObjN64(0x76, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(size16x16_tex32x32_vtx);

    item = work + 0x38;
    for (i = 1; i < *(s32*)((s32)param_2 + 8); i++, item += 0x38) {
        if (*(s32*)(item + 0x20) >= 0) {
            PSMTXTrans(baseMtx, *(f32*)(item + 4), *(f32*)(item + 8), float_0_80424c50);
            PSMTXRotRad(rotMtx, 0x7A, *(f32*)(item + 0x24) * float_deg2rad_80424c48);
            PSMTXScale(scaleMtx, *(f32*)(item + 0x34), *(f32*)(item + 0x34), *(f32*)(item + 0x34));
            PSMTXConcat(baseMtx, rotMtx, baseMtx);
            PSMTXConcat(baseMtx, scaleMtx, baseMtx);
            PSMTXConcat(viewMtx, baseMtx, baseMtx);
            GXLoadPosMtxImm(baseMtx, 0);
            GXSetCurrentMtx(0);
            color = unk_80429640;
            ((u8*)&color)[3] = (baseAlpha * *(s32*)(item + 0x30)) >> 8;
            GXSetTevColor(1, &color);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}
