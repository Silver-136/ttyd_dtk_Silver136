#include "effect/eff_stageclear.h"

s32 effStageClearEndChk(void* effect) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 value = *(s32*)((s32)work + 0x28);
    return (u32)(-value | value) >> 31;
}

void effStageClearMain(void* effect) {
    typedef struct VecRaw {
        f32 x;
        f32 y;
        f32 z;
    } VecRaw;
    typedef struct StageClearWork {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        s32 unk10;
        f32 scale;
        s32 timer;
        s32 frame;
        void* shakeEvt;
        s32 confettiCount;
        s32 done;
    } StageClearWork;

    extern VecRaw vec3_802fed18;
    extern u8 scale_dt[];
    extern s32 evt_shake[];
    extern u8 shake_dt[];
    extern u8 shake_dt2[];
    extern void* gp;
    extern f32 float_0p9_80427450;
    extern f32 float_100_80427478;
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    extern void psndSFXOn(s32 id);
    extern s32 irand(s32 range);
    extern s32 rand(void);
    extern void effConfettiEntry(f32 x, f32 y, f32 z, s32 type, s32 life);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);
    extern void effStageClearDisp(void);

    StageClearWork* baseWork;
    StageClearWork* work;
    VecRaw pos;
    VecRaw drawPos;
    s32 finishedCount;
    s32 i;
    s32 type;
    s32 retrace;
    s32 confettiCount;
    f32 cx;
    f32 cy;
    f32 z;
    void* shake;
    s32 last;

    baseWork = *(StageClearWork**)((s32)effect + 0xC);
    pos = vec3_802fed18;
    pos.x = baseWork->x;
    pos.y = baseWork->y;
    pos.z = baseWork->z;
    drawPos = pos;

    finishedCount = 0;
    i = 1;
    type = baseWork->type;
    baseWork->frame++;
    work = baseWork + 1;

    while (i < *(s32*)((s32)effect + 0x8)) {
        if (work->timer != 0) {
            work->timer--;
            if (work->timer == 0) {
                if (type == 0) {
                    psndSFXOn(0x29B);
                } else {
                    psndSFXOn(0x82E);
                }
            }
        } else {
            if ((u32)work->frame < 11U) {
                work->scale = (f32)scale_dt[work->frame] / float_100_80427478;
            }
            work->frame++;
            if (work->scale > float_0p9_80427450) {
                if (work->shakeEvt == 0) {
                    shake = evtEntry(evt_shake, 0, 0);
                    work->shakeEvt = shake;
                    last = *(s32*)((s32)effect + 0x8) - 1;
                    if (i == last) {
                        *(void**)((s32)shake + 0x9C) = (void*)shake_dt2;
                    } else {
                        *(void**)((s32)shake + 0x9C) = (void*)shake_dt;
                    }
                    *(s32*)((s32)shake + 0xA0) = (u32)(last == i);
                }
                finishedCount++;
            }
        }
        i++;
        work++;
    }

    if (*(s32*)((s32)effect + 0x8) - 1 <= finishedCount) {
        (*(StageClearWork**)((s32)effect + 0xC))->done = 1;
    }

    if (type % 10 == 1 && *(s32*)((s32)effect + 0x8) - 1 <= finishedCount) {
        retrace = *(s32*)((s32)gp + 0x1C);
        if (retrace == (u32)(retrace / 60) * 60) {
            baseWork = *(StageClearWork**)((s32)effect + 0xC);
            confettiCount = baseWork->confettiCount;
            if (confettiCount > 0) {
                cx = drawPos.x + (f32)(100 - irand(200));
                cy = drawPos.y + (f32)(200 - irand(400));
                effConfettiEntry(cx, cy, drawPos.z, (rand() % 3) + 0x18, 0x3C);
                psndSFXOn(0x82F);
                baseWork->confettiCount = confettiCount - 1;
            }
        }
    }

    if (type < 10) {
        z = dispCalcZ(&drawPos);
        dispEntry(4, 2, effStageClearDisp, effect, z);
    } else {
        z = dispCalcZ(&drawPos);
        dispEntry(5, 8, effStageClearDisp, effect, z);
    }
}

void* effStageClearEntry(s32 type, f32 x, f32 y, f32 z) {
    typedef struct EffEntryRaw {
        u32 flags;
        u32 battle;
        s32 count;
        void* work;
        void* main;
        const char* name;
        char unk18[0x10];
    } EffEntryRaw;
    typedef struct StageClearWork {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        s32 unk10;
        f32 scale;
        s32 timer;
        s32 frame;
        void* shakeEvt;
        s32 confettiCount;
        s32 done;
    } StageClearWork;
    typedef struct TexIdEntry {
        s16* ids;
        s32 unused;
        s32 count;
    } TexIdEntry;

    extern void* gp;
    extern void* effMiniGameEntry(s32 type);
    extern EffEntryRaw* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 irand(s32 range);
    extern void effGetTexObj(s32 id, void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern TexIdEntry texid_tbl[];
    extern const char str_StageClear_802fed60[];
    extern f32 float_1_80427464;
    extern f32 float_0_80427460;
    extern f32 float_0p2_80427488;
    extern f32 float_96_8042748c;
    extern f32 float_60_80427490;
    extern f32 float_0p8_8042747c;
    extern f32 float_16_80427480;
    extern f32 float_0p5_80427470;
    extern f32 float_8_80427484;

    EffEntryRaw* entry;
    StageClearWork* work;
    StageClearWork* cur;
    TexIdEntry* texEntry;
    s32 mode;
    s32 count;
    s32 lang;
    s32 i;
    s32 j;
    s32 offset;
    s32 texId;
    s32 width;
    s32 fps;
    f32 zero;
    f32 fullBefore;
    f32 center;
    f32 totalHalf;
    f32 value;
    u8 texObj[0x20];

    if (type == 1 && *(u32*)((s32)gp + 0x16C) != 0) {
        return effMiniGameEntry(1);
    }

    entry = effEntry();
    mode = type % 10;
    switch (mode) {
        case 0:
            lang = *(s32*)((s32)gp + 0x16C);
            count = *(s32*)((s32)texid_tbl + lang * 0xC + 8) + 1;
            break;
        case 1:
            count = 7;
            break;
    }

    entry->name = str_StageClear_802fed60;
    entry->count = count;
    work = __memAlloc(3, count * 0x2C);
    entry->work = work;
    entry->main = effStageClearMain;

    zero = float_0_80427460;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = float_1_80427464;
    work->timer = 0;
    work->frame = 0;
    work->confettiCount = irand(5) + 1;
    work->done = 0;

    cur = work + 1;
    i = 1;
    while (i < entry->count) {
        fullBefore = zero;
        j = 0;
        offset = 0;
        while (j < i - 1) {
            lang = *(s32*)((s32)gp + 0x16C);
            texId = *(s16*)(*(s32*)((s32)texid_tbl + lang * 0xC) + offset);
            if (texId != -1) {
                effGetTexObj(texId, texObj);
                width = GXGetTexObjWidth(texObj) & 0xFFFF;
                fullBefore += float_0p8_8042747c * (f32)width;
            } else {
                fullBefore += float_16_80427480;
            }
            offset += 2;
            j++;
        }

        lang = *(s32*)((s32)gp + 0x16C);
        texId = *(s16*)(*(s32*)((s32)texid_tbl + lang * 0xC) + (j * 2));
        if (texId != -1) {
            effGetTexObj(texId, texObj);
            width = GXGetTexObjWidth(texObj) & 0xFFFF;
            center = float_0p8_8042747c * ((f32)width * float_0p5_80427470);
        } else {
            center = float_8_80427484;
        }

        totalHalf = zero;
        j = 0;
        offset = 0;
        while (j < entry->count - 1) {
            lang = *(s32*)((s32)gp + 0x16C);
            texId = *(s16*)(*(s32*)((s32)texid_tbl + lang * 0xC) + offset);
            if (texId != -1) {
                effGetTexObj(texId, texObj);
                width = GXGetTexObjWidth(texObj) & 0xFFFF;
                totalHalf += float_0p8_8042747c * ((f32)width * float_0p5_80427470);
            } else {
                totalHalf += float_8_80427484;
            }
            offset += 2;
            j++;
        }

        cur->x = (fullBefore + center) - totalHalf;
        cur->y = zero;
        cur->z = zero;
        cur->scale = zero;

        fps = *(s32*)((s32)gp + 4);
        value = (f32)i * float_0p2_80427488 * (f32)fps;
        cur->timer = (s32)value;
        lang = *(s32*)((s32)gp + 0x16C);
        cur->timer = (s32)(((f32)fps * ((float_96_8042748c * (f32)i) / (f32)*(s32*)((s32)texid_tbl + lang * 0xC + 8))) / float_60_80427490);
        cur->frame = 0;
        cur->shakeEvt = 0;

        cur++;
        i++;
    }

    return entry;
}



/* CHATGPT STUB FILL: main/effect/eff_stageclear 20260624_184823 */

/* stub-fill: effStageClearDisp | prototype_only | source_prototype */
void effStageClearDisp(void) {
    return;
}
