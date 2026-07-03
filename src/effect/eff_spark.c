#include "effect/eff_spark.h"


u8 effSparkMain(void) {
    return 0;
}


u8 effSparkDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSparkEntry(s32 type, f32 x, f32 y, f32 z) {
    typedef struct EffectEntry {
        s32 flags;
        s32 unk4;
        s32 live;
        void* work;
        void* main;
        char* name;
    } EffectEntry;
    extern EffectEntry* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effSparkMain(void);
    extern char str_Spark_8042460c[];
    extern s32 max_seq_num[];

    EffectEntry* effect;
    u8* work;
    f32 half;
    s32 i;

    effect = effEntry();
    effect->name = str_Spark_8042460c;
    effect->live = 6;
    work = __memAlloc(3, 0x150);
    effect->work = work;
    effect->main = effSparkMain;
    effect->flags |= 2;

    *(s32*)(work + 0x00) = type;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x28) = 1000;
    *(s32*)(work + 0x30) = 255;
    *(f32*)(work + 0x04) = x;
    *(f32*)(work + 0x08) = y;
    *(f32*)(work + 0x0C) = z;
    *(f32*)(work + 0x34) = 0.9f;

    half = (f32)max_seq_num[type] * 0.5f;
    for (i = 1; i <= 5; i++) {
        *(s32*)(work + 0x20 + (i * 0x38)) = (s32)(-1.0f - (half * (f32)(i & 1)));
    }

    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

