#include "effect/eff_fire.h"


u8 effFireDisp3(void) {
    return 0;
}


u8 effFireDisp2(int param_1, int param_2) {
    return 0;
}


u8 effFireDisp(int param_1, int param_2) {
    return 0;
}


u8 effFireMain(void) {
    return 0;
}


u8 effFireSmokeDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct EffectEntry {
        s32 flags;
        s32 unk4;
        s32 live;
        void* work;
        void* main;
        char* name;
    } EffectEntry;
    typedef struct FireWork {
        s32 type;
        Vec3 pos;
        u8 alpha;
        u8 pad11;
        u8 pad12;
        u8 pad13;
        s32 duration;
        s32 frame;
        s32 sparkTimer;
        s32 sparkDelay;
        f32 scale;
        s32 texFrame1;
        s32 texFrame2;
        s32 frameCount;
        s32 frameCount2;
        s32 alpha2;
        s32 patternIndex;
        s32 patternTimer;
        f32 scaleMul;
        f32 zero4c;
        f32 zero50;
        f32 zero54;
        f32 zero58;
        f32 zero5c;
        f32 zero60;
        u8 pad64[0x14];
        f32 scaleX;
        f32 scaleY;
        s32 unk80;
        s32 unk84;
        s32 unk88;
    } FireWork;
    extern EffectEntry* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern s32 rand(void);
    extern void effFireMain(void);
    extern char str_Fire_804244d8[];

    EffectEntry* effect;
    FireWork* work;
    s32 animLen;

    effect = effEntry();
    effect->name = str_Fire_804244d8;
    effect->live = 1;
    work = __memAlloc(3, 0x8C);
    effect->work = work;
    effect->main = effFireMain;
    effect->flags |= 2;

    if (duration <= 0) {
        duration = 1000;
    }

    work->type = type;
    work->alpha = 255;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->scale = scale;
    work->duration = duration;
    work->frame = 0;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    work->sparkTimer = 0;
    work->sparkDelay = rand() % 30 + 30;
    work->texFrame1 = 0;
    work->texFrame2 = 0;
    work->zero4c = 0.0f;
    work->zero50 = 0.0f;
    work->zero54 = 0.0f;
    work->zero58 = 0.0f;
    work->zero5c = 0.0f;
    work->zero60 = 0.0f;

    switch (type) {
        case 0:
            work->frameCount2 = 12;
            work->frameCount = work->frameCount2 * 2;
            break;
        case 1:
            work->frameCount2 = 4;
            work->frameCount = work->frameCount2 * 2;
            break;
        case 2:
            work->frameCount2 = 10;
            work->frameCount = work->frameCount2 * 2;
            break;
        case 3:
        case 4:
        case 8:
            work->frameCount2 = 12;
            work->frameCount = work->frameCount2 * 2;
            work->alpha = 204;
            break;
        case 5:
        case 6:
        case 7:
            work->frameCount2 = 12;
            work->frameCount = work->frameCount2 * 2;
            work->alpha = 255;
            break;
    }

    animLen = work->frameCount;
    work->frame = rand() % animLen;
    work->patternIndex = 0;
    work->patternTimer = 0;
    work->scaleMul = 0.0f;
    work->unk80 = 0;
    work->unk84 = 0;
    work->unk88 = 0;

    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effFireSmokeMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct FireSmokeWork {
        s32 type;
        Vec3 pos;
        Vec3 vel;
        s32 unk1c;
        s32 unk20;
        f32 scale;
        f32 rot;
        f32 rotStep;
        u8 r, g, b, a;
        s32 life;
        s32 timer;
        f32 drawZ;
    } FireSmokeWork;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effFireSmokeDisp(void);

    FireSmokeWork* work = *(FireSmokeWork**)((s32)effect + 0xC);
    s32 flags = *(s32*)effect;

    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 16) {
        work->a = work->life << 3;
    }
    if (work->life < 0) {
        effDelete(effect);
    }
    else {
        work->timer++;
        work->vel.x *= 0.95f;
        work->vel.y *= 0.95f;
        work->vel.z *= 0.95f;
        work->pos.x += work->vel.x;
        work->pos.y += work->vel.y;
        work->pos.z += work->vel.z;
        work->rot += work->rotStep;
        work->r = (u8)(work->r * 0.99f);
        work->g = (u8)(work->g * 0.99f);
        work->b = (u8)(work->b * 0.99f);
        dispEntry(4, 2, effFireSmokeDisp, effect, dispCalcZ(&work->pos));
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

