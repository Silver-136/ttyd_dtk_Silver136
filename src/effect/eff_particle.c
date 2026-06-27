#include "effect/eff_particle.h"

void EmitterEmit(void* emitter, void* data) {
    extern void ParticleEmit(void* emitter, void* particle, void* data);
    extern u32 __cvt_fp2unsigned(f32 value);

    void* work;
    u32 oldCount;
    u32 count;
    u32 overflow;
    u32 max;
    u32 index;
    u32 i;
    void* particles;
    u32 temp;
    u32 next;

    work = *(void**)((s32)emitter + 0xC);
    oldCount = __cvt_fp2unsigned(*(f32*)((s32)data + 0x54));
    *(f32*)((s32)data + 0x54) = *(f32*)((s32)data + 0x54) + *(f32*)((s32)data + 0x58);
    count = __cvt_fp2unsigned(*(f32*)((s32)data + 0x54)) - oldCount;
    *(u32*)((s32)work + 0xA8) += count;
    if (*(u32*)((s32)work + 0xA8) > *(u32*)((s32)work + 0xA4)) {
        overflow = *(u32*)((s32)work + 0xA8) - *(u32*)((s32)work + 0xA4);
        *(u32*)((s32)work + 0xA8) = *(u32*)((s32)work + 0xA4);
        count -= overflow;
    }
    if (count != 0) {
        max = *(u32*)((s32)work + 0xA4);
        i = 0;
        index = max + 1;
        while (i < count) {
            particles = *(void**)((s32)work + 0xA0);
            index = *(u32*)((s32)particles + (index << 5));
            ParticleEmit(emitter, (void*)((s32)particles + (index << 5)), data);
            i++;
        }
        particles = *(void**)((s32)work + 0xA0);
        temp = *(u32*)((s32)particles + (*(u32*)((s32)work + 0xA4) << 5));
        *(u32*)((s32)particles + (*(u32*)((s32)work + 0xA4) << 5)) =
            *(u32*)((s32)particles + (*(u32*)((s32)work + 0xA4) << 5) + 0x20);
        particles = *(void**)((s32)work + 0xA0);
        next = *(u32*)((s32)particles + (index << 5));
        max = *(u32*)((s32)work + 0xA4);
        particles = (void*)((s32)particles + (max << 5));
        *(u32*)((s32)particles + 0x20) = next;
        particles = *(void**)((s32)work + 0xA0);
        *(u32*)((s32)particles + (index << 5)) = temp;
    }
}


/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: effParticleDisp | missing_definition | ghidra_signature */
u8 effParticleDisp(void) {
    return 0;
}


/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: effParticleEntry | missing_definition | ghidra_signature */
u8 effParticleEntry(void) {
    return 0;
}


/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: ParticleEmit | prototype_only | source_prototype */
void ParticleEmit(void* emitter, void* particle, void* data) {
    return;
}


/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_particle 20260624_191429 */

/* fallback stub-fill: map=effParticleMain addr=0x80244f78 size=0x000001d8 */
int effParticleMain() {
    return 0;
}
