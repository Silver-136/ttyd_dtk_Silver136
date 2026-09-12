#include "effect/eff_stageclear.h"

void effStageClearMain(void* effect);
extern void* gp;
extern s32 irand(s32 range);

typedef struct StageClearVecCatalog {
    f32 x;
    f32 y;
    f32 z;
} StageClearVecCatalog;

/*
 * Target .rodata prefix.  Keep the compiler-generated signed-int conversion
 * doubles immediately after this prefix; they correspond to the target
 * double_to_int_802fed50 / double_to_int_mask_802fed58 objects.
 */
const StageClearVecCatalog vec3_802fed18 = { 0.0f, 0.0f, 0.0f };
const StageClearVecCatalog vec3_802fed24 = { 0.0f, 0.0f, 0.0f };
const StageClearVecCatalog vec3_802fed30 = { 0.0f, 1.0f, 0.0f };
const StageClearVecCatalog vec3_802fed3c = { 0.0f, 0.0f, 0.0f };
const f64 double_0p21817_802fed48 = 0.21816615760326385;


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

    extern void* effMiniGameEntry(s32 type);
    extern EffEntryRaw* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effGetTexObj(s32 id, void* texObj);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern TexIdEntry texid_tbl[];
    extern const char str_StageClear_802fed60[];
    extern const f32 float_1_80427464;
    extern const f32 float_0_80427460;
    extern const f32 float_0p2_80427488;
    extern const f32 float_96_8042748c;
    extern const f32 float_60_80427490;
    extern const f32 float_0p8_8042747c;
    extern const f32 float_16_80427480;
    extern const f32 float_0p5_80427470;
    extern const f32 float_8_80427484;

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

void effStageClearMain(void* effect) {
    typedef StageClearVecCatalog VecRaw;
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

    extern u8 scale_dt[];
    extern s32 evt_shake[];
    extern u8 shake_dt[];
    extern u8 shake_dt2[];
    extern const f32 float_0p9_80427450;
    extern const f32 float_100_80427478;
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    extern void psndSFXOn(s32 id);
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



/* CHATGPT STUB FILL: main/effect/eff_stageclear 20260624_184823 */

/* stub-fill: effStageClearDisp | prototype_only | source_prototype */
void effStageClearDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; typedef u8 TexObj[0x20];
    extern void* camGetPtr(s32); extern void PSMTXTrans(Mtx,f64,f64,f64); extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*); extern void GXSetBlendMode(s32,s32,s32,s32); extern void GXSetZMode(s32,s32,s32);
    extern void GXSetZCompLoc(s32); extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXGetProjectionv(void*); extern void GXGetViewportv(void*); extern void GXSetProjection(void*,s32);
    extern void GXSetProjectionv(void*); extern void GXSetViewport(f32,f32,f32,f32,f32,f32);
    extern void GXSetNumChans(s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevOp(s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32,void*);
    extern void GXSetTevSwapModeTable(s32,s32,s32,s32,s32); extern void GXSetTevSwapMode(s32,s32,s32);
    extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void effGetTexObj(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXBegin(s32,s32,s32); extern volatile f32 DAT_cc008000;
    extern void C_MTXLightPerspective(Mtx,f32,f32,f32,f32,f32,f32);
    extern void C_MTXLookAt(Mtx,void*,void*,void*); extern f64 tan(f64);
    typedef struct TexIdEntry { s16* ids; s32 unused; s32 count; } TexIdEntry;
    extern TexIdEntry texid_tbl[]; extern s32 niji_tbl[];
    extern const u32 dat_80427448; extern const u32 dat_8042744c;
    extern const f64 double_0p21817_802fed48;
    extern const f32 float_0_80427460; extern const f32 float_0p5_80427470; extern const f32 float_0p6_80427458;
    extern const f32 float_0p9_80427450; extern const f32 float_1p7_80427454; extern const f32 float_1_80427464;
    extern const f32 float_24_80427474; extern const f32 float_25_80427468; extern const f32 float_640_8042746c;
    typedef struct Vec { f32 x,y,z; } Vec;
    u8* base=*(u8**)((u8*)effect+0xC); void* cam; Mtx baseMtx,trans,scale,model,perspective,lookAt; TexObj tex;
    Vec eye,target,up;
    f32 projection[7]; f32 viewport[6];
    s32 type=*(s32*)base; s32 frame=*(s32*)(base+0x1C); s32 i; s32 pass; s32 kind; u8* child;
    f32 size=float_0p9_80427450*float_1p7_80427454**(f32*)(base+0x14);
    camGetPtr(cameraId);
    if(*(s32*)((u8*)gp+0x16C)!=0) size*=float_0p6_80427458;
    GXGetProjectionv(projection); GXGetViewportv(viewport);
    cam=camGetPtr(3); GXSetProjection((u8*)cam+0x15C,*(s32*)((u8*)cam+0x19C));
    GXSetBlendMode(1,4,5,0); GXSetZCompLoc(1); GXSetAlphaCompare(7,0,0,7,0); GXSetZMode(0,3,0);
    PSMTXTrans(trans,*(f32*)(base+4),*(f32*)(base+8),*(f32*)(base+0xC));
    PSMTXScale(scale,size,size,size);PSMTXConcat(trans,scale,baseMtx);GXSetNumChans(0);GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);GXSetTexCoordGen2(1,1,0,0x1E,0,0x7D);GXSetCullMode(0);GXClearVtxDesc();
    GXSetVtxDesc(9,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,13,1,4,0);
    kind=type%10;
    for(pass=1;pass>=0;pass--){
        child=base+0x2C;
        for(i=1;i<*(s32*)((u8*)effect+8);i++,child+=0x2C){
            if(pass!=0) {
                s32 texId;
                u32 tevColor=dat_80427448;
                if(kind!=0) {
                    texId=i+0x4F;
                } else {
                    s32 lang=*(s32*)((u8*)gp+0x16C);
                    texId=texid_tbl[lang].ids[i-1];
                    if(texId==-1) continue;
                }
                effGetTexObj(texId,tex); GXLoadTexObj(tex,0);
                GXSetNumTevStages(1); GXSetTevOrder(0,0,0,0xFF);
                GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
                GXSetTevColorIn(0,15,8,2,15); GXSetTevAlphaIn(0,7,4,5,7);
                GXSetTevColor(1,&tevColor);
            } else {
                if(kind==0) {
                    s32 lang=*(s32*)((u8*)gp+0x16C);
                    s32 texId=texid_tbl[lang].ids[i-1];
                    u32 tevColor;
                    if(texId==-1) continue;
                    effGetTexObj(texId,tex); GXLoadTexObj(tex,0);
                    effGetTexObj(niji_tbl[lang],tex); GXLoadTexObj(tex,1);
                    GXSetNumTevStages(3); GXSetTevOrder(0,0,0,0xFF);
                    GXSetTevColorOp(0,0xE,0,0,1,0); GXSetTevAlphaOp(0,0xE,0,0,1,0);
                    GXSetTevColorIn(0,8,15,8,15); GXSetTevAlphaIn(0,4,7,4,7);
                    GXSetTevSwapModeTable(1,0,1,2,0); GXSetTevSwapMode(0,1,1);
                    GXSetTevOrder(1,1,1,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
                    GXSetTevColorIn(1,15,0,8,15); GXSetTevAlphaIn(1,7,7,7,0);
                    GXSetTevOrder(2,0,0,0xFF); GXSetTevColorOp(2,0,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0);
                    GXSetTevColorIn(2,2,0,1,15); GXSetTevAlphaIn(2,7,7,7,4);
                    tevColor=dat_8042744c; GXSetTevColor(1,&tevColor);
                } else {
                    effGetTexObj(i+0x49,tex); GXLoadTexObj(tex,0);
                    GXSetNumTevStages(1); GXSetTevOrder(0,0,0,0xFF);
                    GXSetTevOp(0,3);
                }
            }
            PSMTXTrans(trans,*(f32*)(child+4)+(f32)(pass*4),*(f32*)(child+8)-(f32)(pass*4),float_0_80427460);
            PSMTXScale(scale,*(f32*)(child+0x14),*(f32*)(child+0x14),*(f32*)(child+0x14));
            PSMTXConcat(baseMtx,trans,model);PSMTXConcat(model,scale,model);PSMTXConcat((u8*)cam+0x11C,model,model);
            GXLoadPosMtxImm(model,0);GXSetCurrentMtx(0);
            eye.x=float_0_80427460; eye.y=float_0_80427460;
            eye.z=float_1_80427464/(f32)tan(*(volatile const f64*)&double_0p21817_802fed48);
            target.x=float_0_80427460; target.y=float_0_80427460; target.z=float_0_80427460;
            up.x=float_0_80427460; up.y=float_1_80427464; up.z=float_0_80427460;
            C_MTXLightPerspective(perspective,float_25_80427468,float_640_8042746c,
                                  float_0p5_80427470,float_0p5_80427470,float_0p5_80427470,float_0p5_80427470);
            C_MTXLookAt(lookAt,&eye,&target,&up);
            PSMTXConcat(perspective,lookAt,perspective);
            PSMTXConcat(perspective,model,model);
            PSMTXTrans(trans,(f64)-frame,float_0_80427460,float_0_80427460);
            PSMTXConcat(model,trans,model);
            GXLoadTexMtxImm(model,0x1E,1);
            GXBegin(0x80,0,4);DAT_cc008000=-float_24_80427474;DAT_cc008000=float_24_80427474;DAT_cc008000=float_0_80427460;DAT_cc008000=float_0_80427460;DAT_cc008000=float_0_80427460;
            DAT_cc008000=float_24_80427474;DAT_cc008000=float_24_80427474;DAT_cc008000=float_0_80427460;DAT_cc008000=float_1_80427464;DAT_cc008000=float_0_80427460;
            DAT_cc008000=float_24_80427474;DAT_cc008000=-float_24_80427474;DAT_cc008000=float_0_80427460;DAT_cc008000=float_1_80427464;DAT_cc008000=float_1_80427464;
            DAT_cc008000=-float_24_80427474;DAT_cc008000=-float_24_80427474;DAT_cc008000=float_0_80427460;DAT_cc008000=float_0_80427460;DAT_cc008000=float_1_80427464;
        }
    }
    GXSetTevSwapModeTable(1,0,1,2,3);
    GXSetProjectionv(projection);
    GXSetViewport(viewport[0],viewport[1],viewport[2],viewport[3],viewport[4],viewport[5]);
}

s32 effStageClearEndChk(void* effect) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 value = *(s32*)((s32)work + 0x28);
    return (u32)(-value | value) >> 31;
}

/*
 * Target-owned read-only catalogs.  These definitions are intentionally late:
 * the consuming functions above see extern declarations, preserving their
 * target-shaped load/codegen while restoring the owning object's sections.
 */
const char str_StageClear_802fed60[] = "StageClear";

const u32 dat_80427448 = 0x00000080;
const u32 dat_8042744c = 0x363636FF;
const f32 float_0p9_80427450 = 0.9f;
const f32 float_1p7_80427454 = 1.7f;
const f32 float_0p6_80427458 = 0.6f;
const f32 float_48_8042745c = 48.0f;
const f32 float_0_80427460 = 0.0f;
const f32 float_1_80427464 = 1.0f;
const f32 float_25_80427468 = 25.0f;
const f32 float_640_8042746c = 640.0f;
const f32 float_0p5_80427470 = 0.5f;
const f32 float_24_80427474 = 24.0f;
const f32 float_100_80427478 = 100.0f;
const f32 float_0p8_8042747c = 0.8f;
const f32 float_16_80427480 = 16.0f;
const f32 float_8_80427484 = 8.0f;
const f32 float_0p2_80427488 = 0.2f;
const f32 float_96_8042748c = 96.0f;
const f32 float_60_80427490 = 60.0f;
