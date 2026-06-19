#include "effect/eff_mobj_broken.h"

void* animGetPtr(void);
void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
void GXSetZCompLoc(s32 beforeTex);
void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
void GXSetZMode(s32 enable, s32 func, s32 updateEnable);

void rendermodeFunc(s32 mode) {
    void* anim = animGetPtr();

    *(s32*)((s32)anim + 0xE0) = mode;
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
}
