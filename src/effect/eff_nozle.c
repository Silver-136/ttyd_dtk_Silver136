#include "effect/eff_nozle.h"

void effNozleMain(void* effEntry) {
    typedef struct VecNozle_s {
        f32 x;
        f32 y;
        f32 z;
    } VecNozle;

    typedef struct EffEntryNozle_s {
        u32 flags;
        u32 battle;
        s32 count;
        void* work;
    } EffEntryNozle;

    typedef struct NozleWork_s {
        s32 mode;
        f32 x;
        f32 y;
        f32 z;
        f32 scaleX;
        f32 scaleY;
        f32 rotA;
        f32 rotB;
        f32 rotC;
        f32 rotD;
        f32 baseScale;
        s32 timer;
        s32 frame;
        s32 alpha;
    } NozleWork;

    typedef struct NozleChildWork_s {
        f32 x;
        f32 y;
        f32 z;
        f32 scaleX;
        f32 scaleY;
        f32 rotA;
        f32 rotB;
        f32 rotC;
        f32 rotD;
        f32 baseScale;
        s32 timer;
        s32 frame;
        s32 alpha;
        s32 unk34;
        s32 state;
        s32 unk3C;
        s32 unk40;
    } NozleChildWork;

    extern VecNozle vec3_802fefc0[];
    extern u8 scale_dt[42];
    extern f32 float_100_80427794;
    extern f32 float_neg0p041667_80427798;
    extern f32 float_neg0p083333_8042779c;
    extern f32 float_300_804277a0;
    extern f32 float_0p002_804277b4;
    extern f32 float_0p004_804277b8;
    extern f32 float_1p3_804277a4;
    extern f32 float_500_804277a8;
    extern f32 float_1p2_804277ac;
    extern f32 float_0p8_804277b0;
    extern void effNozleDisp(void);
    extern void effDelete(void* eff);
    extern f32 dispCalcZ(VecNozle* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void (*callback)(void), void* param, f32 order);

    EffEntryNozle* eff;
    NozleWork* work;
    VecNozle zpos;
    VecNozle pos;
    s32 timer;
    s32 i;
    NozleChildWork* child;
    f32 value;

    eff = (EffEntryNozle*)effEntry;
    work = (NozleWork*)eff->work;

    pos = vec3_802fefc0[0];
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    zpos = pos;

    if (eff->flags & 4) {
        eff->flags &= ~4;
        work->timer = 0x20;
    }

    switch (work->mode) {
        case 0:
        case 2:
            if ((u32)work->frame < 0x15U) {
                i = work->frame << 1;
                work->scaleX = (f32)scale_dt[i] / float_100_80427794;
                work->scaleY = (f32)scale_dt[i + 1] / float_100_80427794;
            }

            if ((u32)work->timer < 0x15U) {
                i = work->timer << 1;
                work->scaleX = (f32)scale_dt[i] / float_100_80427794;
                work->scaleY = (f32)scale_dt[i + 1] / float_100_80427794;
            }

            work->rotB += float_neg0p041667_80427798;
            work->rotD += float_neg0p083333_8042779c;
            break;

        case 3: {
            f32 div300;
            f32 addSmall;
            f32 addLarge;

            child = (NozleChildWork*)((s32)work + 0x44);
            div300 = float_300_804277a0;
            addSmall = float_0p002_804277b4;
            addLarge = float_0p004_804277b8;
            for (i = 1; i < eff->count; i++) {
                value = child->y;
                child->y = value + (-value / div300);

                if (child->state == 0) {
                    child->scaleY += (float_1p3_804277a4 - child->scaleY) / float_500_804277a8;
                    if (child->scaleY > float_1p2_804277ac) {
                        child->state = 1;
                    }
                } else {
                    child->scaleY += (float_0p8_804277b0 - child->scaleY) / div300;
                }

                child->rotB += addSmall;
                child->rotC += addSmall;
                child->rotD += addLarge;
                child = (NozleChildWork*)((s32)child + 0x44);
            }
            break;
        }
    }

    timer = work->timer;
    if (timer < 1000) {
        timer--;
        work->timer = timer;
    }

    if (work->timer < 0x10) {
        work->alpha = work->timer << 4;
    }

    work->frame++;

    if (work->timer < 0) {
        effDelete(eff);
    } else if (work->mode == 3) {
        value = dispCalcZ(&zpos);
        dispEntry(4, 8, effNozleDisp, eff, value);
    } else {
        value = dispCalcZ(&zpos);
        dispEntry(4, 2, effNozleDisp, eff, value);
    }
}



/* CHATGPT STUB FILL: main/effect/eff_nozle 20260624_184823 */

/* stub-fill: effNozleDisp | prototype_only | source_prototype */
void effNozleDisp(void) {
    return;
}

/* stub-fill: effNozleEntry | missing_definition | ghidra_signature */
u8 effNozleEntry(void) {
    return 0;
}
